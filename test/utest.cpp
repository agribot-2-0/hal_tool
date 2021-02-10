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

TEST_F(TestTplStatus, TestChassis)
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
  std::cerr << "chassis angle value : " << *(tpl_state_test_->getEncoderValue("chassis")) << std::endl;
}

TEST_F(TestTplStatus, TestPrismaticFront)
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

TEST_F(TestTplStatus, TestPrismaticBack)
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

TEST_F(TestTplStatus, TestRevoluteFront)
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

TEST_F(TestTplStatus, TestRevoluteBack)
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

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
