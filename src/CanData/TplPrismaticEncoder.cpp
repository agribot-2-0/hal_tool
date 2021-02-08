/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <cstdint>

//Debug: 
#include <iostream>


void TplPrismaticEncoder::setParsedValue(const double val)
{
  ejection_meters_ = val;
}

std::uint32_t TplPrismaticEncoder::getId() const
{
  return 0;
}

void TplPrismaticEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  for (auto byte : data)
  {
    std::cout << byte << std::endl;
  }
}

double TplPrismaticEncoder::getAssembledValue() const
{
  return 0.0;
}
