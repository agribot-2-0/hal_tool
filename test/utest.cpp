/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <TestTplStatus.h>
#include <hal_tool/CanData/ChassisEncoder.h>
#include <hal_tool/CanData/FlowEncoder.h>
#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <hal_tool/CanData/TplRevoluteEncoder.h>

#include <gtest/gtest.h>

#include <list>
#include <string>

using hal_tool_test::TestTplStatus;

constexpr double degrees_to_radians = M_PI / 180;

TEST_F(TestTplStatus, WrongHashReferenced)
{
  const double angle_degrees = -139;

  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(hal_tool_test::getChassisFrame(angle_degrees, hal_tool::encoder_can_id::chassis)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test chassis parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();

  ASSERT_EQ(tpl_state_test_->getEncoderValue("chasis"), std::nullopt);
}

TEST_F(TestTplStatus, TestChassisFrameParser)
{
  const double angle_degrees = -30;
  const double angle_radians = angle_degrees * degrees_to_radians;

  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(hal_tool_test::getChassisFrame(angle_degrees, hal_tool::encoder_can_id::chassis)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test chassis parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("chassis")));
}

TEST_F(TestTplStatus, TestPrismaticFrontFrameParser)
{
  const double encoded_ejection_in_meters = 0.0553;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getPrismaticFrame(encoded_ejection_in_meters, hal_tool::encoder_can_id::tpl_prismatic_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test prismatic parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_prismatic_front")));
}

TEST_F(TestTplStatus, TestPrismaticBackFrameParser)
{
  const double encoded_ejection_in_meters = -0.8827;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getPrismaticFrame(encoded_ejection_in_meters, hal_tool::encoder_can_id::tpl_prismatic_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test prismatic parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_prismatic_back")));
}

TEST_F(TestTplStatus, TestRevoluteFrontFrameParser)
{
  const double angle_degrees = 12.5;
  const double angle_radians = angle_degrees * degrees_to_radians;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRevoluteFrame(angle_degrees, hal_tool::encoder_can_id::tpl_revolute_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("tpl_revolute_front")));
}

TEST_F(TestTplStatus, TestRevoluteBackFrameParser)
{
  const double angle_degrees = -32.5;
  const double angle_radians = angle_degrees * degrees_to_radians;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRevoluteFrame(angle_degrees, hal_tool::encoder_can_id::tpl_revolute_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("tpl_revolute_back")));
}

TEST_F(TestTplStatus, TestRgbPtoFrontFrameParser)
{
  const int8_t red = 12;
  const int8_t green = -23;
  const int8_t blue = 3;
  const int8_t pto = -22;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRgbPtoFrame(pto, red, green, blue, hal_tool::encoder_can_id::flow_pto_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_EQ(pto, *(tpl_state_test_->getEncoderValue("pto_flow_front")));
  ASSERT_EQ(red, *(tpl_state_test_->getEncoderValue("red_flow_front")));
  ASSERT_EQ(green, *(tpl_state_test_->getEncoderValue("green_flow_front")));
  ASSERT_EQ(blue, *(tpl_state_test_->getEncoderValue("blue_flow_front")));
}

TEST_F(TestTplStatus, TestRgbPtoBackFrameParser)
{
  const int8_t red = -77;
  const int8_t green = 0;
  const int8_t blue = 99;
  const int8_t pto = -99;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRgbPtoFrame(pto, red, green, blue, hal_tool::encoder_can_id::flow_pto_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  ASSERT_EQ(pto, *(tpl_state_test_->getEncoderValue("pto_flow_back")));
  ASSERT_EQ(red, *(tpl_state_test_->getEncoderValue("red_flow_back")));
  ASSERT_EQ(green, *(tpl_state_test_->getEncoderValue("green_flow_back")));
  ASSERT_EQ(blue, *(tpl_state_test_->getEncoderValue("blue_flow_back")));
}

TEST_F(TestTplStatus, TestRosMessagePacketsAssembly)
{
  
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
