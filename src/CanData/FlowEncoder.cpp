/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/FlowEncoder.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>

#include <cstdint>

namespace hal_tool
{

constexpr uint8_t parse_constant = 100;

void FlowEncoder::setParsedValue(const double val)
{
  valve_open_percentage_ = val;
}

// flow_encoder enum type is also data position inside CAN frame
std::uint8_t FlowEncoder::getDataBytePosition()
{
  return static_cast<std::uint8_t>(encoder_type_);
}

FlowEncoder::FlowEncoder(flow_encoder selected_encoder)
{
  encoder_type_ = selected_encoder;
}

void FlowEncoder::assembleValueFromFrame(const std::vector<std::uint8_t>& data)
{
  parsed_at_least_once_ = true;
  setParsedValue(CanHelperFunctions::getI8(getDataBytePosition(), data) - parse_constant);
}

std::optional<double> FlowEncoder::getAssembledValue() const
{
  if (parsed_at_least_once_)
  {
    return valve_open_percentage_;
  }
  return std::nullopt;
}

void FlowEncoder::resetParsedFlag()
{
  parsed_at_least_once_ = false;
}
}  // namespace hal_tool
