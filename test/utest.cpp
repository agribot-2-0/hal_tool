/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <TestTplStatus.h>
#include <hal_tool/CanData/ChassisInclination.h>
#include <hal_tool/CanData/QcfFlow.h>
#include <hal_tool/CanData/TplExtension.h>
#include <hal_tool/CanData/TplAngle.h>

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
      .WillOnce(::testing::Return(
          hal_tool_test::getChassisFrame(angle_degrees, hal_tool::tpl_sensors_can_id::chassis_inclinometer)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test not relevant:
  tpl_state_test_->parseTplEncodersFrames();

  // Test wrong hash val:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_wrong_value"), std::nullopt);
}

TEST_F(TestTplStatus, ResetVisited)
{
  const double angle_degrees = -139;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getChassisFrame(angle_degrees, hal_tool::tpl_sensors_can_id::chassis_inclinometer)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));
  // Test not relevant:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_NE(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
  tpl_state_test_->resetVisitedEncodersFlag({ "chassis_inclination" });
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
}

TEST_F(TestTplStatus, ChassisFrameParser)
{
  const double angle_degrees = -30;
  const double angle_radians = angle_degrees * degrees_to_radians;

  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getChassisFrame(angle_degrees, hal_tool::tpl_sensors_can_id::chassis_inclinometer)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test chassis parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("chassis_inclination")));
}

TEST_F(TestTplStatus, PrismaticFrontFrameParser)
{
  const double encoded_ejection_in_meters = 0.0553;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(hal_tool_test::getPrismaticFrame(
          encoded_ejection_in_meters, hal_tool::tpl_sensors_can_id::tpl_prismatic_encoder_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test prismatic parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("tpl_extension_front"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_extension_front")));
}

TEST_F(TestTplStatus, PrismaticBackFrameParser)
{
  const double encoded_ejection_in_meters = -0.8827;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(hal_tool_test::getPrismaticFrame(
          encoded_ejection_in_meters, hal_tool::tpl_sensors_can_id::tpl_prismatic_encoder_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test prismatic parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("tpl_extension_back"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_extension_back")));
}

TEST_F(TestTplStatus, RevoluteFrontFrameParser)
{
  const double angle_degrees = 12.5;
  const double angle_radians = angle_degrees * degrees_to_radians;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRevoluteFrame(angle_degrees, hal_tool::tpl_sensors_can_id::tpl_inclinometer_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("tpl_angle_front"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("tpl_angle_front")));
}

TEST_F(TestTplStatus, RevoluteBackFrameParser)
{
  const double angle_degrees = -32.5;
  const double angle_radians = angle_degrees * degrees_to_radians;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRevoluteFrame(angle_degrees, hal_tool::tpl_sensors_can_id::tpl_inclinometer_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("tpl_angle_back"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("tpl_angle_back")));
}

TEST_F(TestTplStatus, RgbPtoFrontFrameParser)
{
  const int16_t red = 12;
  const int16_t green = -23;
  const int16_t blue = 3;
  const int16_t pto = -22;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRgbPtoFrame(pto, red, green, blue, hal_tool::tpl_sensors_can_id::pto_and_qcf_flow_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("pto_flow_front"), std::nullopt);
  ASSERT_EQ(tpl_state_test_->getEncoderValue("red_qcf_flow_front"), std::nullopt);
  ASSERT_EQ(tpl_state_test_->getEncoderValue("green_qcf_flow_front"), std::nullopt);
  ASSERT_EQ(tpl_state_test_->getEncoderValue("blue_qcf_flow_front"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_EQ(pto, *(tpl_state_test_->getEncoderValue("pto_flow_front")));
  ASSERT_EQ(red, *(tpl_state_test_->getEncoderValue("red_qcf_flow_front")));
  ASSERT_EQ(green, *(tpl_state_test_->getEncoderValue("green_qcf_flow_front")));
  ASSERT_EQ(blue, *(tpl_state_test_->getEncoderValue("blue_qcf_flow_front")));
}

TEST_F(TestTplStatus, RgbPtoBackFrameParser)
{
  const int16_t red = -99;
  const int16_t green = -13;
  const int16_t blue = 28;
  const int16_t pto = 70;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRgbPtoFrame(pto, red, green, blue, hal_tool::tpl_sensors_can_id::pto_and_qcf_flow_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("pto_flow_back"), std::nullopt);
  ASSERT_EQ(tpl_state_test_->getEncoderValue("red_qcf_flow_back"), std::nullopt);
  ASSERT_EQ(tpl_state_test_->getEncoderValue("green_qcf_flow_back"), std::nullopt);
  ASSERT_EQ(tpl_state_test_->getEncoderValue("blue_qcf_flow_back"), std::nullopt);
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_EQ(pto, *(tpl_state_test_->getEncoderValue("pto_flow_back")));
  ASSERT_EQ(red, *(tpl_state_test_->getEncoderValue("red_qcf_flow_back")));
  ASSERT_EQ(green, *(tpl_state_test_->getEncoderValue("green_qcf_flow_back")));
  ASSERT_EQ(blue, *(tpl_state_test_->getEncoderValue("blue_qcf_flow_back")));
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
