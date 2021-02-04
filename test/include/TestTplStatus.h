/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <agribot_common/CanBus/tests/CanMock.h>
#include <TplStatus/TplStatus.h>

namespace hal_tool_test
{
// Fixture for testing of TplStatus class
struct TestTplStatus : public ::testing::Test
{
  std::unique_ptr<TplStatus> test = std::make_unique<TplStatus>();

  void SetUp() override
  {
    std::cout << "setup" << std::endl;
  }

  void TearDown() override
  {
    std::cout << "death" << std::endl;
  }
};
}  // namespace hal_tool_test