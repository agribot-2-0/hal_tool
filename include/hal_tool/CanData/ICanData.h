/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanFrame.h>
#include <vector>
#include <optional>
#include <cstdint>

// Interface for data received by CAN (inclinometer of chassis, TPL encoders, RGB/PTO encoders)
class ICanData
{
  virtual void setParsedValue(const double val) = 0;
 public:
  virtual ~ICanData() {}
  virtual std::uint32_t getId() const = 0;
  virtual void assembleValueFromFrame(const std::vector<uint8_t>& data) = 0;
  virtual std::optional<double> getAssembledValue() const = 0;
};
