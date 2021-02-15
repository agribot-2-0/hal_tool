/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/ICan.h>
#include <agribot_common/Can.h>

#include <hal_tool/CanData/ICanData.h>
#include <hal_tool/CanData/ChassisEncoder.h>
#include <hal_tool/CanData/FlowEncoder.h>
#include <hal_tool/CanData/TplPrismaticEncoder.h>
#include <hal_tool/CanData/TplRevoluteEncoder.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace hal_tool
{
namespace encoder_can_id
{
constexpr uint32_t chassis = 0xFF0119;
constexpr uint32_t flow_pto_front = 0xFFD464;
constexpr uint32_t tpl_prismatic_front = 0xFFFF7F;
constexpr uint32_t tpl_revolute_front = 0xFFAB7C;
constexpr uint32_t flow_pto_back = 0xFFD564;
constexpr uint32_t tpl_prismatic_back = 0xFFFF7E;
constexpr uint32_t tpl_revolute_back = 0xFFAB7D;
}  // namespace encoder_can_id

class TplStatus
{
 private:
  std::unique_ptr<ICan> can_handle_;
  std::unordered_map<std::string, std::unique_ptr<ICanData>> encoders_;

  void assembleChassis(const std::vector<uint8_t>& frame_data);
  void assembleRgbFlowAndPto(const std::vector<uint8_t>& frame_data, const std::string& suffix);
  void assembleTplPrismatic(const std::vector<uint8_t>& frame_data, const std::string& suffix);
  void assembleTplRevolute(const std::vector<uint8_t>& frame_data, const std::string& suffix);
  void registerEncoder(const std::string& encoder_name, std::unique_ptr<ICanData> encoder);
  void assembleEncodersValues(uint32_t frame_id, const std::vector<uint8_t>& frame_data);
  bool keyExists(const std::string& encoder_name);
  bool keyContainsSuffix(const std::string& key, const std::string& suffix);

 public:
  explicit TplStatus(std::unique_ptr<ICan> can_handle);
  void parseTplEncodersFrames();
  // nullopt will be returned in two scenarios from getEncoderValue method
  // 1. encoder_type key is not present on encoders_ map
  // 2. the encoder frame was never received
  std::optional<double> getEncoderValue(const std::string& encoder_type);
  void resetVisitedEncodersFlag(const std::string& suffix);
  bool dataReadyToSend(const std::string& suffix);
};
}  // namespace hal_tool
