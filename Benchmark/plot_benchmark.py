import csv
import math
import sys

LABELS = {
    "cls_printf_per_object": "cls + 오브젝트마다 printf",
    "printf_per_object": "오브젝트마다 printf (cls 없음)",
    "array_printf_once": "문자 배열 + printf 1회",
    "back_buffer_write_console_output": "백버퍼 + WriteConsoleOutputW 1회",
}

FRAME_BUDGET_MS = 1000 / 30

WIDTH = 760
HEIGHT = 332
PLOT_LEFT = 270
PLOT_RIGHT = 700
ROW_TOP = 94
ROW_GAP = 48
BAR_THICKNESS = 24
AXIS_Y = ROW_TOP + ROW_GAP * 4 - 10
LOG_MIN = -2
LOG_MAX = 2


def to_x(value):
    ratio = (math.log10(value) - LOG_MIN) / (LOG_MAX - LOG_MIN)
    return PLOT_LEFT + ratio * (PLOT_RIGHT - PLOT_LEFT)


def bar_path(x0, x1, y, h, r=4):
    return (
        f"M{x0:.1f},{y:.1f} H{x1 - r:.1f} Q{x1:.1f},{y:.1f} {x1:.1f},{y + r:.1f} "
        f"V{y + h - r:.1f} Q{x1:.1f},{y + h:.1f} {x1 - r:.1f},{y + h:.1f} H{x0:.1f} Z"
    )


def format_ms(value):
    return f"{value:.1f} ms" if value >= 10 else f"{value:.2f} ms"


def build_svg(rows):
    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{WIDTH}" height="{HEIGHT}" viewBox="0 0 {WIDTH} {HEIGHT}" font-family="Malgun Gothic, Apple SD Gothic Neo, Noto Sans KR, sans-serif">',
        "<style>",
        ".surface{fill:#fcfcfb}.primary{fill:#0b0b0b}.secondary{fill:#52514e}.grid{stroke:#e4e3df}.budget{stroke:#52514e}.bar{fill:#2a78d6}",
        "@media (prefers-color-scheme: dark){.surface{fill:#1a1a19}.primary{fill:#ffffff}.secondary{fill:#c3c2b7}.grid{stroke:#353533}.budget{stroke:#c3c2b7}.bar{fill:#3987e5}}",
        "</style>",
        f'<rect class="surface" width="{WIDTH}" height="{HEIGHT}" rx="8"/>',
        '<text class="primary" x="24" y="34" font-size="16" font-weight="bold">프레임당 출력 시간</text>',
        '<text class="secondary" x="24" y="54" font-size="12">100×30 화면, 오브젝트 150개, 60프레임 × 3회 평균 · 로그 눈금 · 짧을수록 빠름</text>',
    ]

    for exponent in range(LOG_MIN, LOG_MAX + 1):
        x = to_x(10**exponent)
        out.append(f'<line class="grid" x1="{x:.1f}" y1="{ROW_TOP - 8}" x2="{x:.1f}" y2="{AXIS_Y}" stroke-width="1"/>')
        out.append(f'<text class="secondary" x="{x:.1f}" y="{AXIS_Y + 18}" font-size="11" text-anchor="middle">{10**exponent:g}</text>')
    out.append(f'<text class="secondary" x="{PLOT_RIGHT}" y="{AXIS_Y + 34}" font-size="11" text-anchor="end">ms / 프레임</text>')

    budget_x = to_x(FRAME_BUDGET_MS)
    out.append(f'<line class="budget" x1="{budget_x:.1f}" y1="{ROW_TOP - 14}" x2="{budget_x:.1f}" y2="{AXIS_Y}" stroke-width="1.5" stroke-dasharray="4 3"/>')
    out.append(f'<text class="secondary" x="{budget_x - 6:.1f}" y="{ROW_TOP - 18}" font-size="11" text-anchor="end">30fps 한계 {FRAME_BUDGET_MS:.1f} ms</text>')

    baseline = to_x(10**LOG_MIN)
    for index, (method, value) in enumerate(rows):
        y = ROW_TOP + index * ROW_GAP
        tip = to_x(value)
        out.append(f'<text class="primary" x="{PLOT_LEFT - 12}" y="{y + BAR_THICKNESS / 2 + 4:.1f}" font-size="13" text-anchor="end">{LABELS.get(method, method)}</text>')
        out.append(f'<path class="bar" d="{bar_path(baseline, tip, y, BAR_THICKNESS)}"/>')
        out.append(f'<text class="primary" x="{tip + 8:.1f}" y="{y + BAR_THICKNESS / 2 + 4:.1f}" font-size="13" font-weight="bold">{format_ms(value)}</text>')

    out.append("</svg>")
    return "\n".join(out) + "\n"


def main():
    if len(sys.argv) != 3:
        print("usage: python plot_benchmark.py <result.csv> <output.svg>")
        return 1

    with open(sys.argv[1], encoding="utf-8") as file:
        rows = [(row["method"], float(row["ms_per_frame"])) for row in csv.DictReader(file)]

    with open(sys.argv[2], "w", encoding="utf-8") as file:
        file.write(build_svg(rows))
    return 0


if __name__ == "__main__":
    sys.exit(main())
