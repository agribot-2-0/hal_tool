/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/ChassisEncoder.h>
#include <cstdint>

//Debug: 
#include <iostream>

void ChassisEncoder::setParsedValue(const double val)
{
  chassis_angle_radians_ = val;
}

std::uint32_t ChassisEncoder::getId() const
{
  return 0;
}

void ChassisEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  for (auto byte : data)
  {
    std::cout << byte << std::endl;
  }
}

double ChassisEncoder::getAssembledValue() const
{
  return 0.0;
}
