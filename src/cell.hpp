#pragma once

#include <cstdint>

namespace snek {
    enum class Cell : std::uint8_t { Empty, Tail, Body, Head, Food };
}