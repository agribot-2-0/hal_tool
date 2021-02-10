/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <cstdint>

// Debug:
#include <iostream>

constexpr double encoder_resolution_meters = 0.0001;  // m
constexpr uint8_t position_byte_location = 0;

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
  std::cerr << "setting encoder value -  " << CanHelperFunctions::makeU16Lsbf(position_byte_location, data)
            << std::endl;
  setParsedValue(static_cast<double>(CanHelperFunctions::makeU16Lsbf(position_byte_location, data)) *
                 encoder_resolution_meters);
}

std::optional<double> TplPrismaticEncoder::getAssembledValue() const
{
  return ejection_meters_;
}
