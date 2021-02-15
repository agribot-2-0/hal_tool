/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/ICanReceivedData.h>
#include <vector>
#include <cstdint>

namespace hal_tool
{
enum class QfcFlowType : uint8_t
{
  pto = 0,
  red_qcf = 1,
  green_qcf = 2,
  blue_qcf = 3
};

class QfcFlow : public ICanReceivedData<double>
{
  bool parsed_at_least_once_ = false;
  std::optional<double> valve_open_percentage_;
  QfcFlowType encoder_type_;
  void setParsedValue(const double val) override;
  uint8_t getDataBytePosition();

 public:
  explicit QfcFlow(QfcFlowType selected_encoder);
  void assembleValueFromFrame(const CanFrame& can_frame) override;
  std::optional<double> getAssembledValue() const override;
  void resetParsedFlag() override;
};
}  // namespace hal_tool
