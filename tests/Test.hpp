#pragma once

#include "../include/bluefin.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

std::string captureErrors(std::string expr) {
    std::stringstream buffer;
    bluefin::run(expr, buffer);
    std::cout << buffer.str() << std::endl;
    return buffer.str();
}

} // namespace
