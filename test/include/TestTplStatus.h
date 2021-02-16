/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/test/CanMock.h>
#include <hal_tool/TplStatus/TplStatus.h>

#include <gtest/gtest.h>
#include <iostream>
#include <memory>

namespace hal_tool_test
{
constexpr char selected_can[] = "can1";

constexpr std::uint8_t can_frame_dlc = 8;
constexpr std::uint32_t first_byte_mask = 0x000000FF;
constexpr std::uint32_t second_byte_mask = 0x0000FF00;
constexpr std::uint32_t third_byte_mask = 0x00FF0000;
constexpr std::uint32_t angle_const = 10;

CanFrame getChassisFrame(double angle, uint32_t id)
{
  std::uint32_t angle_to_send = static_cast<int32_t>(angle * angle_const);
  can_frame frame_tmp;
  CanFrame frame;
  frame_tmp.can_id = id;
  frame_tmp.can_dlc = can_frame_dlc;
  frame_tmp.data[4] = (angle_to_send & first_byte_mask);
  frame_tmp.data[5] = (angle_to_send & second_byte_mask) >> 8;
  frame_tmp.data[6] = (angle_to_send & third_byte_mask) >> 16;
  frame_tmp.data[7] = angle_to_send >> 24;
  frame.setupFrame(frame_tmp);
  return frame;
}

CanFrame getPrismaticFrame(double ejection, uint32_t id)
{
  can_frame frame_tmp;
  CanFrame frame;
  uint16_t ejection_to_send = static_cast<uint16_t>(ejection * 10000);
  frame_tmp.can_id = id;
  frame_tmp.can_dlc = can_frame_dlc;
  frame_tmp.data[0] = ejection_to_send & first_byte_mask;
  frame_tmp.data[1] = ejection_to_send >> 8;
  frame.setupFrame(frame_tmp);
  return frame;
}

CanFrame getRevoluteFrame(double angle, uint32_t id)
{
  uint32_t angle_to_send = static_cast<int16_t>(angle * angle_const);
  can_frame frame_tmp;
  CanFrame frame;
  frame_tmp.can_id = id;
  frame_tmp.can_dlc = can_frame_dlc;
  frame_tmp.data[0] = (angle_to_send & first_byte_mask);
  frame_tmp.data[1] = (angle_to_send & second_byte_mask) >> 8;
  frame.setupFrame(frame_tmp);
  return frame;
}

CanFrame getRgbPtoFrame(double pto, double red, double green, double blue, uint32_t id)
{
  const uint8_t parse_constant = 110;
  can_frame frame_tmp;
  CanFrame frame;
  frame_tmp.can_id = id;
  frame_tmp.can_dlc = can_frame_dlc;
  frame_tmp.data[0] = (pto + parse_constant);
  frame_tmp.data[1] = (red + parse_constant);
  frame_tmp.data[2] = (green + parse_constant);
  frame_tmp.data[3] = (blue + parse_constant);
  frame.setupFrame(frame_tmp);
  return frame;
}

// Fixture for testing of TplStatus class
struct TestTplStatus : public ::testing::Test
{
  std::unique_ptr<CanMock> can_test_;
  std::unique_ptr<hal_tool::TplStatus> tpl_state_test_;

  void SetUp() override
  {
    can_test_ = std::make_unique<CanMock>();
    EXPECT_CALL(*can_test_, iInitialize(selected_can)).WillRepeatedly(::testing::Return(true));
  }

  void TearDown() override
  {
  }
};
}  // namespace hal_tool_test
