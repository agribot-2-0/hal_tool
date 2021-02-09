/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <hal_tool/CanData/ICanData.h>
#include <cstdint>

class TplPrismaticEncoder : public ICanData
{
 private:  
  double ejection_meters_;
  void setParsedValue(const double val) override;

 public:
  std::uint32_t getId() const override;
  void assembleValueFromFrame(const std::vector<uint8_t>& data) override;
  std::optional<double> getAssembledValue() const override;
};
