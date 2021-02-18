/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <TestTplStatus.h>
#include <hal_tool/CanData/ChassisInclination.h>
#include <hal_tool/CanData/QcfFlow.h>
#include <hal_tool/CanData/TplExtension.h>
#include <hal_tool/CanData/TplAngle.h>
#include <agribot_common/CommonFunctions/common_functions.h>

#include <gtest/gtest.h>

#include <list>
#include <string>

using hal_tool_test::TestTplStatus;

TEST_F(TestTplStatus, getEncoderValue__When_wrong_hash_referenced__Return_nullopt)
{
  const double angle_degrees = -139;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getChassisFrame(angle_degrees, hal_tool::tpl_sensors_can_id::chassis_inclinometer)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));
  tpl_state_test_->parseTplEncodersFrames();
  // Test wrong hash val:
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_wrong_value"), std::nullopt);
}

TEST_F(TestTplStatus, getEncoderValue__When_no_can_frame_received__Return_nullopt)
{
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
}

TEST_F(TestTplStatus, getEncoderValue__When_can_frame_received__Nullopt_should_not_be_returned)
{
  const double angle_degrees = -139;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getChassisFrame(angle_degrees, hal_tool::tpl_sensors_can_id::chassis_inclinometer)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_NE(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
}

TEST_F(TestTplStatus, resetVisitedEncodersFlag__When_reset_method_is_called__Retrun_nullopt)
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
  tpl_state_test_->resetVisitedEncodersFlag({ "chassis_inclination" });
  ASSERT_EQ(tpl_state_test_->getEncoderValue("chassis_inclination"), std::nullopt);
}

TEST_F(TestTplStatus, ChassisFrameParser__When_chasis_frame_received__Parsed_angle_should_match_expected_angle)
{
  const double angle_degrees = -30;
  const double angle_radians = common_functions::radians(angle_degrees);

  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getChassisFrame(angle_degrees, hal_tool::tpl_sensors_can_id::chassis_inclinometer)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));
  // Test chassis parsing:
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("chassis_inclination")));
}

TEST_F(
    TestTplStatus,
    PrismaticFrontFrameParser__When_front_tpl_extension_frame_received__Parsed_extension_should_match_expecte_extension)
{
  const double encoded_extension_in_meters = 0.0553;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(hal_tool_test::getPrismaticFrame(
          encoded_extension_in_meters, hal_tool::tpl_sensors_can_id::tpl_prismatic_encoder_front)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test prismatic parsing:
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(encoded_extension_in_meters, *(tpl_state_test_->getEncoderValue("tpl_extension_front")));
}

TEST_F(
    TestTplStatus,
    PrismaticBackFrameParser__When_back_tpl_extension_frame_received__Parsed_extension_should_match_expected_extension)
{
  const double encoded_ejection_in_meters = -0.8827;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(hal_tool_test::getPrismaticFrame(
          encoded_ejection_in_meters, hal_tool::tpl_sensors_can_id::tpl_prismatic_encoder_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test prismatic parsing:
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_extension_back")));
}

TEST_F(TestTplStatus,
       RevoluteFrontFrameParser__When_front_tpl_angle_frame_received__Parsed_angle_should_match_expected_angle)
{
  const double angle_degrees = 12.5;
  const double angle_radians = common_functions::radians(angle_degrees);
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

TEST_F(TestTplStatus,
       RevoluteBackFrameParser__When_back_tpl_angle_frame_received__Parsed_angle_should_match_expected_angle)
{
  const double angle_degrees = -32.5;
  const double angle_radians = common_functions::radians(angle_degrees);
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(
          hal_tool_test::getRevoluteFrame(angle_degrees, hal_tool::tpl_sensors_can_id::tpl_inclinometer_back)));
  tpl_state_test_ = std::make_unique<hal_tool::TplStatus>(std::move(can_test_));

  // Test revolute frame parsing:
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_DOUBLE_EQ(angle_radians, *(tpl_state_test_->getEncoderValue("tpl_angle_back")));
}

TEST_F(TestTplStatus, RgbPtoFrontFrameParser__When_front_flows_frame_received__Parsed_flows_should_match_expected_flows)
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
  tpl_state_test_->parseTplEncodersFrames();
  ASSERT_EQ(pto, *(tpl_state_test_->getEncoderValue("pto_flow_front")));
  ASSERT_EQ(red, *(tpl_state_test_->getEncoderValue("red_qcf_flow_front")));
  ASSERT_EQ(green, *(tpl_state_test_->getEncoderValue("green_qcf_flow_front")));
  ASSERT_EQ(blue, *(tpl_state_test_->getEncoderValue("blue_qcf_flow_front")));
}

TEST_F(TestTplStatus, RgbPtoBackFrameParser__When_back_flows_frame_received__Parsed_flows_should_match_expected_flows)
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
