/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/ICan.h>
#include <agribot_common/Can.h>

#include <agribot_common/CanBus/ICanReceivedData.h>
#include <hal_tool/CanData/ChassisInclination.h>
#include <hal_tool/CanData/QcfFlow.h>
#include <hal_tool/CanData/TplExtension.h>
#include <hal_tool/CanData/TplAngle.h>

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace hal_tool
{
namespace tpl_sensors_can_id
{
constexpr uint32_t chassis_inclinometer = 0xFF0119;
constexpr uint32_t pto_and_qcf_flow_front = 0xFFD464;
constexpr uint32_t tpl_prismatic_encoder_front = 0xFFFF7F;
constexpr uint32_t tpl_inclinometer_front = 0xFFAB7C;
constexpr uint32_t pto_and_qcf_flow_back = 0xFFD564;
constexpr uint32_t tpl_prismatic_encoder_back = 0xFFFF7E;
constexpr uint32_t tpl_inclinometer_back = 0xFFAB7D;
}  // namespace tpl_sensors_can_id

class TplStatus
{
 private:
  std::unique_ptr<ICan> can_handler_;
  std::unordered_map<std::string, std::unique_ptr<ICanReceivedData<double>>> tpl_sensor_data_;

  void assembleChassisInclination(const CanFrame& can_frame);
  void assemblePtoAndQcfFlow(const CanFrame& can_frame, const std::string& suffix);
  void assembleTplExtension(const CanFrame& can_frame, const std::string& suffix);
  void assembleTplAngle(const CanFrame& can_frame, const std::string& suffix);
  void registerTplSensorData(const std::string& encoder_name, std::unique_ptr<ICanReceivedData<double>> encoder);
  void assembleEncodersValues(const CanFrame& can_frame);
  bool keyExists(const std::string& encoder_name);
  bool keyContainsSuffix(const std::string& key, const std::string& suffix);

 public:
  explicit TplStatus(std::unique_ptr<ICan> can_handle);
  void parseTplEncodersFrames();
  // nullopt will be returned in two scenarios from getEncoderValue method
  // 1. encoder_type key is not present on tpl_sensor_data_ map
  // 2. the encoder frame was never received
  std::optional<double> getEncoderValue(const std::string& encoder_type);
  void resetVisitedEncodersFlag(const std::string& suffix);
  bool dataReadyToSend(const std::string& suffix);
};
}  // namespace hal_tool
