/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplAngle.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <agribot_common/CommonFunctions/common_functions.h>

#include <cmath>
#include <cstdint>

namespace hal_tool
{
constexpr double encoder_resolution_degrees = 0.1;
constexpr std::uint8_t pitch_lsb_location = 0;

void TplAngle::setParsedValue(const double val)
{
  angle_radians_.emplace(-val);
}

void TplAngle::assembleValueFromFrame(const CanFrame& can_frame)
{
  setParsedValue(common_functions::radians(
      static_cast<double>(CanHelperFunctions::getI16Lsbf(pitch_lsb_location, can_frame.getData())) *
      encoder_resolution_degrees));
}

std::optional<double> TplAngle::getAssembledValue() const
{
  return angle_radians_;
}

void TplAngle::resetParsedValue()
{
  angle_radians_.reset();
}
}  // namespace hal_tool
