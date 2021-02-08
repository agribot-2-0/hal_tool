/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/ICan.h>

#include <hal_tool/CanData/ICanData.h>
#include <hal_tool/CanData/ChassisEncoder.h>
#include <hal_tool/CanData/FlowEncoder.h>
#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <hal_tool/CanData/TplRevoluteEncoder.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <string_view>

namespace encoder_can_id
{
constexpr uint16_t chassis = 0x00;
constexpr uint16_t flow_pto_front = 0x01;
constexpr uint16_t tpl_prismatic_front = 0x02;
constexpr uint16_t tpl_revolute_front = 0x03;
constexpr uint16_t flow_pto_back = 0x04;
constexpr uint16_t tpl_prismatic_back = 0x05;
constexpr uint16_t tpl_revolute_back = 0x06;
}  // namespace encoder_can_id

// encoders factory
class TplStatus
{
 private:
  std::unique_ptr<ICan> can_handle_;
  std::vector<CanFrame> can_frames_;
  std::unordered_map<std::string_view, std::unique_ptr<ICanData>> encoders_;

  void setupRgbFlowAndPto(const std::vector<uint8_t>& frame_data);
  void registerEncoder(const std::string_view& encoder_name, std::unique_ptr<ICanData> encoder);
  void setupEncoderValue(uint16_t frame_id, const std::vector<uint8_t>& frame_data);

 public:
  TplStatus(std::unique_ptr<ICan> can_handle);
  void loadCanFrames();
  void assembleEncodersValues();
  double getEncoderValue(std::string_view& encoder_type);  // std::optional
};
