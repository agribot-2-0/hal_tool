/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/ICanReceivedData.h>
#include <vector>
#include <cstdint>

namespace hal_tool
{
class TplAngle : public ICanReceivedData<double>
{
 private:
  std::optional<double> angle_radians_;
  void setParsedValue(const double val) override;

 public:
  double invertInclinometerAxis(double angle) const;
  void assembleValueFromFrame(const CanFrame& can_frame) override;
  std::optional<double> getAssembledValue() const override;
  void resetParsedValue() override;
};
}  // namespace hal_tool
