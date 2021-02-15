/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/CanData/QfcFlow.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>

#include <cstdint>

namespace hal_tool
{

constexpr int8_t parse_constant = 110;

void QfcFlow::setParsedValue(const double val)
{
  valve_open_percentage_.emplace(val);
}

// QfcFlowType enum is also data position inside CAN frame
std::uint8_t QfcFlow::getDataBytePosition()
{
  return static_cast<std::uint8_t>(encoder_type_);
}

QfcFlow::QfcFlow(QfcFlowType selected_encoder)
{
  encoder_type_ = selected_encoder;
}

void QfcFlow::assembleValueFromFrame(const CanFrame& can_frame)
{
  setParsedValue(CanHelperFunctions::getU8(getDataBytePosition(), can_frame.getData()) - parse_constant);
}

std::optional<double> QfcFlow::getAssembledValue() const
{
  return valve_open_percentage_;
}

void QfcFlow::resetParsedFlag()
{
  valve_open_percentage_.reset();
}
}  // namespace hal_tool
