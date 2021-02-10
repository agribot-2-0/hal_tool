/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <hal_tool/CanData/ICanData.h>
#include <vector>
#include <cstdint>

enum class flow_encoder : uint8_t
{
  pto = 0,
  red = 1,
  green = 2,
  blue = 3
};

class FlowEncoder : public ICanData
{
  double valve_open_percentage_;
  flow_encoder encoder_type_;
  void setParsedValue(const double val) override;
  uint8_t getDataBytePosition();

 public:
  FlowEncoder(flow_encoder selected_encoder);
  void assembleValueFromFrame(const std::vector<uint8_t>& data) override;
  std::optional<double> getAssembledValue() const override;
};
