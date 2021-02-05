/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <hal_tool/CanData/ICanData.h>
#include <cstdint>

class ChassisEncoder : public ICanData
{
  double chassis_angle_radians_;
  void setParsedValue(const double val) override;

 public:
  std::uint32_t getId() const override;
  void assembleValueFromFrame(const CanFrame& data) override;
  double getAssembledValue() const override;
};
