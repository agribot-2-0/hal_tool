/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <hal_tool/CanData/ICanData.h>
#include <vector>
#include <cstdint>

class TplRevoluteEncoder : public ICanData
{
 private:
  double angle_radians_;
  void setParsedValue(const double val) override;

 public:
  void assembleValueFromFrame(const std::vector<uint8_t>& data) override;
  std::optional<double> getAssembledValue() const override;
};
