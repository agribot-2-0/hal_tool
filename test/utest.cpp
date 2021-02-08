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

TEST(testChassisEncoder, testFrameParsing)
{

  
}

TEST(testRgbPtoEncoder, testFrameParsing)
{
  
}

TEST(testPrismaticEncoder, testFrameParsing)
{

}

TEST(testRevoluteEncoder, testFrameParsing)
{

}


TEST_F(TestTplStatus, TestUno)
{
  //EXPECT_CALL(*can_test_, bReceiveFrame).WillRepeatedly(::testing::Return(true));
  tpl_state_test_->loadCanFrames();
  tpl_state_test_->assembleEncodersValues();

}

TEST_F(TestTplStatus, TestDuo)
{

}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}