/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <gtest/gtest.h>

#include <list>
#include <string>

#include <TestTplStatus.h>

using hal_tool_test::TestTplStatus;

TEST_F(TestTplStatus, TestUno)
{
  std::list<std::string> myList;
  myList.push_back("Nazwisko");
  ASSERT_EQ(1ul, myList.size());
}

// TEST_F(TestTplStatus, TestDuo)
// {
//   ASSERT_EQ(2 + 2, 4);
// }

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}