/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/tests/CanMock.h>
#include <TplStatus/TplStatus.h>

#include <gtest/gtest.h>
#include <iostream>
#include <memory>

namespace hal_tool_test
{
// Fixture for testing of TplStatus class
struct TestTplStatus : public ::testing::Test
{
  std::unique_ptr<Can> can_test = std::make_unique<Can>();
  //std::unique_ptr<TplStatus> tpl_state_test = std::make_unique<TplStatus>(can_test);

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
