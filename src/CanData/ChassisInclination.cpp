/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/ChassisInclination.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <agribot_common/CommonFunctions/common_functions.h>

#include <cstdint>

namespace hal_tool
{
constexpr double encoder_resolution_degrees = 0.1;
constexpr std::uint8_t pitch_lsb_location = 4;

void ChassisInclination::setParsedValue(const double val)
{
  chassis_angle_radians_.emplace(val);
}

void ChassisInclination::assembleValueFromFrame(const CanFrame& can_frame)
{
  setParsedValue(common_functions::radians(
      static_cast<double>(CanHelperFunctions::getI32Lsbf(pitch_lsb_location, can_frame.getData())) *
      encoder_resolution_degrees));
}

std::optional<double> ChassisInclination::getAssembledValue() const
{
  return chassis_angle_radians_;
}

void ChassisInclination::resetParsedValue()
{
  chassis_angle_radians_.reset();
}
}  // namespace hal_tool
