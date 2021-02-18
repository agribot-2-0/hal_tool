/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplExtension.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>

#include <cstdint>

namespace hal_tool
{
constexpr double encoder_resolution_meters = 0.0001;
constexpr std::uint8_t position_lsb_location = 0;

void TplExtension::setParsedValue(const double val)
{
  extension_meters_.emplace(val);
}

void TplExtension::assembleValueFromFrame(const CanFrame& can_frame)
{
  setParsedValue(static_cast<double>(CanHelperFunctions::getI16Lsbf(position_lsb_location, can_frame.getData())) *
                 encoder_resolution_meters);
}

std::optional<double> TplExtension::getAssembledValue() const
{
  return extension_meters_;
}

void TplExtension::resetParsedValue()
{
  extension_meters_.reset();
}
}  // namespace hal_tool
