/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/ChassisEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>

#include <cmath>
#include <cstdint>

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
  setParsedValue(static_cast<double>(CanHelperFunctions::getI32Lsbf(pitch_lsb_location, data)) *
                 encoder_resolution_degrees * degrees_to_radians);
  parsed_at_least_once_ = true;
}

std::optional<double> ChassisEncoder::getAssembledValue() const
{
  if (parsed_at_least_once_)
  {
    return chassis_angle_radians_;
  }
  return std::nullopt;
}

void ChassisEncoder::resetParsedFlag()
{
  parsed_at_least_once_ = false;
}
}  // namespace hal_tool
