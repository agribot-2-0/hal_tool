/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/can_data/QcfFlow.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>

#include <cstdint>

namespace hal_tool
{

constexpr int8_t parse_constant = 110;

void QcfFlow::setParsedValue(const double val)
{
  valve_open_percentage_.emplace(val);
}

// QcfFlowType enum is also data position inside CAN frame
std::uint8_t QcfFlow::getDataBytePosition()
{
  return static_cast<std::uint8_t>(encoder_type_);
}

QcfFlow::QcfFlow(QcfFlowType selected_encoder)
{
  encoder_type_ = selected_encoder;
}

void QcfFlow::assembleValueFromFrame(const CanFrame& can_frame)
{
  setParsedValue(CanHelperFunctions::getU8(getDataBytePosition(), can_frame.getData()) - parse_constant);
}

std::optional<double> QcfFlow::getAssembledValue() const
{
  return valve_open_percentage_;
}

void QcfFlow::resetParsedValue()
{
  valve_open_percentage_.reset();
}
}  // namespace hal_tool
