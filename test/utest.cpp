/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <gtest/gtest.h>

#include <list>
#include <string>

#include <TestTplStatus.h>
#include <hal_tool/CanData/ChassisEncoder.h>
#include <hal_tool/CanData/FlowEncoder.h>
#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <hal_tool/CanData/TplRevoluteEncoder.h>

using hal_tool_test::TestTplStatus;

CanFrame getChassisFrame(double angle, uint32_t id)
{
  can_frame frame_tmp;
  CanFrame frame;
  frame_tmp.can_id = id;
  frame_tmp.can_dlc = 8;
  frame_tmp.data[0] = angle;
  frame.setupFrame(frame_tmp);
  return frame;
}

CanFrame getPrismaticFrame(double ejection, uint32_t id)
{
  can_frame frame_tmp;
  CanFrame frame;
  uint16_t ejection_to_send = static_cast<uint16_t>(ejection * 10000);
  frame_tmp.can_id = id;
  frame_tmp.can_dlc = 8;
  frame_tmp.data[0] = ejection_to_send & 0xff;
  frame_tmp.data[1] = ejection_to_send >> 8;
  frame.setupFrame(frame_tmp);
  return frame;
}

TEST_F(TestTplStatus, TestChassis)
{
  const double encoded_angle_in_radians = 0.3;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(getChassisFrame(encoded_angle_in_radians, encoder_can_id::chassis)));
  tpl_state_test_ = std::make_unique<TplStatus>(std::move(can_test_));
  // Test chassis parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  //ASSERT_DOUBLE_EQ(encoded_angle_in_radians, tpl_state_test_->getEncoderValue("chassis"));
}

TEST_F(TestTplStatus, TestPrismaticFront)
{
  const double encoded_ejection_in_meters = 0.0553;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(getPrismaticFrame(encoded_ejection_in_meters, encoder_can_id::tpl_prismatic_front)));
  tpl_state_test_ = std::make_unique<TplStatus>(std::move(can_test_));
  // Test prismatic parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  if(tpl_state_test_->getEncoderValue("tpl_prismatic_front"))
  {
    ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_prismatic_front")));
  }
  else
  {
    ASSERT_EQ(0,1);
  }
}

TEST_F(TestTplStatus, TestPrismaticBack)
{
  const double encoded_ejection_in_meters = 0.0227;
  // Test setup:
  EXPECT_CALL(*can_test_, bReceiveFrame()).WillOnce(::testing::Return(true)).WillOnce(::testing::Return(false));
  EXPECT_CALL(*can_test_, getMostRecentCanFrame())
      .WillOnce(::testing::Return(getPrismaticFrame(encoded_ejection_in_meters, encoder_can_id::tpl_prismatic_back)));
  tpl_state_test_ = std::make_unique<TplStatus>(std::move(can_test_));
  // Test prismatic parsing:
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->parseTplEncoders();
  if(tpl_state_test_->getEncoderValue("tpl_prismatic_back"))
  {
    ASSERT_DOUBLE_EQ(encoded_ejection_in_meters, *(tpl_state_test_->getEncoderValue("tpl_prismatic_back")));
  }
  else
  {
    ASSERT_EQ(0,1);
  }
}


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
