/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/FlowEncoder.h>
#include <cstdint>

//Debug: 
#include <iostream>


void FlowEncoder::setParsedValue(const double val)
{
  angle_radians_ = val;
}

std::uint32_t FlowEncoder::getId() const
{
  return 0;
}

void FlowEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  for (auto byte : data)
  {
    std::cout << byte << std::endl;
  }
}

double FlowEncoder::getAssembledValue() const
{
  return 0.0;
}
