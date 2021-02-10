/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplRevoluteEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <cmath>
#include <cstdint>

// Debug:
#include <iostream>

namespace hal_tool
{
constexpr double degrees_to_radians = M_PI / 180;
constexpr double encoder_resolution_degrees = 0.1;
constexpr uint8_t pitch_lsb_location = 0;

void TplRevoluteEncoder::setParsedValue(const double val)
{
  angle_radians_ = val;
}

void TplRevoluteEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  setParsedValue(static_cast<double>(CanHelperFunctions::getI16Lsbf(pitch_lsb_location, data)) *
                 encoder_resolution_degrees * degrees_to_radians);
}

std::optional<double> TplRevoluteEncoder::getAssembledValue() const
{
  return angle_radians_;
}
}  // namespace hal_tool
