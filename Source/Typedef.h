#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>

using String     = std::string;
using StringView = std::string_view;

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;
