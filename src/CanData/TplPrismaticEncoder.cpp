/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <cstdint>

// Debug:
#include <iostream>

namespace hal_tool
{
constexpr double encoder_resolution_meters = 0.0001;
constexpr uint8_t position_lsb_location = 0;

void TplPrismaticEncoder::setParsedValue(const double val)
{
  ejection_meters_ = val;
}

void TplPrismaticEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  setParsedValue(static_cast<double>(CanHelperFunctions::getI16Lsbf(position_lsb_location, data)) *
                 encoder_resolution_meters);
}

std::optional<double> TplPrismaticEncoder::getAssembledValue() const
{
  return ejection_meters_;
}
}  // namespace hal_tool
