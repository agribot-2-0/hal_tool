/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplRevoluteEncoder.h>
#include <cstdint>

//Debug: 
#include <iostream>


void TplRevoluteEncoder::setParsedValue(const double val)
{
  angle_radians_ = val;
}

std::uint32_t TplRevoluteEncoder::getId() const
{
  return 0;
}

void TplRevoluteEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  for (auto byte : data)
  {
    std::cout << byte << std::endl;
  }
}

std::optional<double> TplRevoluteEncoder::getAssembledValue() const
{
  return 0.0;
}
