/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/ChassisEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <cmath>
#include <cstdint>

// Debug:
#include <iostream>

namespace hal_tool
{
constexpr double degrees_to_radians = M_PI / 180;
constexpr double encoder_resolution_degrees = 0.1;
constexpr uint8_t pitch_lsb_location = 4;

void ChassisEncoder::setParsedValue(const double val)
{
  chassis_angle_radians_ = val;
}

void ChassisEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  std::cerr << "assemble method for chassis object called" << std::endl;
  setParsedValue(static_cast<double>(CanHelperFunctions::getI32Lsbf(pitch_lsb_location, data)) *
                 encoder_resolution_degrees * degrees_to_radians);
}

std::optional<double> ChassisEncoder::getAssembledValue() const
{
  return chassis_angle_radians_;
}
}  // namespace hal_tool
