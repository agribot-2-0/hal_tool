/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/FlowEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>
#include <cstdint>

// Debug:
#include <iostream>

namespace hal_tool
{
void FlowEncoder::setParsedValue(const double val)
{
  valve_open_percentage_ = val;
}

// flow_encoder enum type is also data position inside CAN frame
uint8_t FlowEncoder::getDataBytePosition()
{
  return static_cast<uint8_t>(encoder_type_);
}

FlowEncoder::FlowEncoder(flow_encoder selected_encoder)
{
  encoder_type_ = selected_encoder;
}

void FlowEncoder::assembleValueFromFrame(const std::vector<uint8_t>& data)
{
  setParsedValue(CanHelperFunctions::getI8(getDataBytePosition(), data));
}

std::optional<double> FlowEncoder::getAssembledValue() const
{
  return valve_open_percentage_;
}
}  // namespace hal_tool
