/*
* Copyright (C) 2020 Agribot. All rights reserved.
*/

#include <gtest/gtest.h>

#include <list>
#include <string>


TEST(InclinometerTest, CheckValue1)
{
    
    std::list<std::string> myList;
    myList.push_back("Nazwisko");

    ASSERT_EQ(2ul, myList.size());
}

TEST(InclinometerTest, CheckValue)
{

  ASSERT_EQ(2+3, 4);

}


int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
