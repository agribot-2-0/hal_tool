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

using ::testing::Return;
using ::testing::_; // Matcher for parameters

constexpr char selected_can[] = "can1";

// Fixture for testing of TplStatus class
struct TestTplStatus : public ::testing::Test
{
  std::unique_ptr<CanMock> can_test_;  // in final app it should be: std::unique_ptr<ICan> can_test_;
  std::unique_ptr<TplStatus> tpl_state_test_;

  void SetUp() override
  {
    std::cout << "setup" << std::endl;
    can_test_ = std::make_unique<CanMock>();
    EXPECT_CALL(*can_test_, iInitialize(selected_can)).WillRepeatedly(Return(true));
    tpl_state_test_ = std::make_unique<TplStatus>(std::move(can_test_));  // ownership transfer
  }

  void TearDown() override
  {
    std::cout << "death" << std::endl;
    can_test_.reset();
    tpl_state_test_.reset();
  }
};
}  // namespace hal_tool_test
