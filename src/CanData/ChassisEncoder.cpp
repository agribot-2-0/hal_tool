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
  std::cerr << "assemble method for chassis object called" << std::endl;
  double angle = 0.0;
  for (auto byte : data)
  {
    std::cerr << std::hex << static_cast<int>(byte) << std::endl;
  }
  setParsedValue(angle);
}

std::optional<double> ChassisEncoder::getAssembledValue() const
{
  return chassis_angle_radians_;
}
