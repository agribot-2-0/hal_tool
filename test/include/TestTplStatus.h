/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/tests/CanMock.h>
#include <hal_tool/TplStatus/TplStatus.h>

#include <gtest/gtest.h>
#include <iostream>
#include <memory>

namespace hal_tool_test
{
constexpr char selected_can[] = "can1";

// Fixture for testing of TplStatus class
struct TestTplStatus : public ::testing::Test
{
  std::unique_ptr<CanMock> can_test_;
  std::unique_ptr<TplStatus> tpl_state_test_;

  void SetUp() override
  {
    std::cout << "setup" << std::endl;
    can_test_ = std::make_unique<CanMock>();
    EXPECT_CALL(*can_test_, iInitialize(selected_can)).WillRepeatedly(::testing::Return(true));
    tpl_state_test_ = std::make_unique<TplStatus>(std::move(can_test_));  // ownership transfer
  }

  void TearDown() override
  {
  }
};
}  // namespace hal_tool_test
