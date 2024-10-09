#pragma once

#include <fmt/core.h>

namespace Logger
{
    void log(const char* message) {
        fmt::println(message);
    }
}
