#pragma once

#include "../include/bluefin.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace {

std::string captureErrors(std::string expr) {
    std::stringstream buffer;
    int res = bluefin::run(expr, buffer);
    std::cout << buffer.str() << std::endl;
    return buffer.str();
}

} // namespace
