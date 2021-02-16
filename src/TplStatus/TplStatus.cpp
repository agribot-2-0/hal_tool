/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatus/TplStatus.h>
#include <agribot_common/CanBus/CanHelperFunctions.h>

namespace hal_tool
{
void TplStatus::registerTplSensorData(const std::string& encoder_name, std::unique_ptr<ICanReceivedData<double>> encoder)
{
  tpl_sensor_data_.insert({ encoder_name, std::move(encoder) });
}

void TplStatus::assembleChassisInclination(const CanFrame& can_frame)
{
  tpl_sensor_data_["chassis_inclination"]->assembleValueFromFrame(can_frame);
}

void TplStatus::assemblePtoAndQcfFlow(const CanFrame& can_frame, const std::string& suffix)
{
  tpl_sensor_data_["red_qcf_flow_" + suffix]->assembleValueFromFrame(can_frame);
  tpl_sensor_data_["green_qcf_flow_" + suffix]->assembleValueFromFrame(can_frame);
  tpl_sensor_data_["blue_qcf_flow_" + suffix]->assembleValueFromFrame(can_frame);
  tpl_sensor_data_["pto_flow_" + suffix]->assembleValueFromFrame(can_frame);
}

void TplStatus::assembleTplExtension(const CanFrame& can_frame, const std::string& suffix)
{
  tpl_sensor_data_["tpl_extension_" + suffix]->assembleValueFromFrame(can_frame);
}

void TplStatus::assembleTplAngle(const CanFrame& can_frame, const std::string& suffix)
{
  tpl_sensor_data_["tpl_angle_" + suffix]->assembleValueFromFrame(can_frame);
}

void TplStatus::assembleEncodersValues(const CanFrame& can_frame)
{
  switch (CanHelperFunctions::getJ1939PgnAndSa(can_frame.getId()))
  {
    case (tpl_sensors_can_id::chassis_inclinometer):
      assembleChassisInclination(can_frame);
      break;
    case (tpl_sensors_can_id::pto_and_qcf_flow_front):
      assemblePtoAndQcfFlow(can_frame, "front");
      break;
    case (tpl_sensors_can_id::tpl_prismatic_encoder_front):
      assembleTplExtension(can_frame, "front");
      break;
    case (tpl_sensors_can_id::tpl_inclinometer_front):
      assembleTplAngle(can_frame, "front");
      break;
    case (tpl_sensors_can_id::pto_and_qcf_flow_back):
      assemblePtoAndQcfFlow(can_frame, "back");
      break;
    case (tpl_sensors_can_id::tpl_prismatic_encoder_back):
      assembleTplExtension(can_frame, "back");
      break;
    case (tpl_sensors_can_id::tpl_inclinometer_back):
      assembleTplAngle(can_frame, "back");
      break;
  }
}

bool TplStatus::keyExists(const std::string& encoder_name)
{
  if (tpl_sensor_data_.find(encoder_name) != tpl_sensor_data_.end())
  {
    return true;
  }
  return false;
}

bool TplStatus::keyContainsSuffix(const std::string& key, const std::string& suffix)
{
  return key.find(suffix) != std::string::npos;
}

TplStatus::TplStatus(std::unique_ptr<ICan> can_handle) : can_handler_{ std::move(can_handle) }
{
  registerTplSensorData("chassis_inclination", std::move(std::make_unique<ChassisInclination>()));
  registerTplSensorData("red_qcf_flow_front", std::move(std::make_unique<QcfFlow>(QcfFlowType::red_qcf)));
  registerTplSensorData("green_qcf_flow_front", std::move(std::make_unique<QcfFlow>(QcfFlowType::green_qcf)));
  registerTplSensorData("blue_qcf_flow_front", std::move(std::make_unique<QcfFlow>(QcfFlowType::blue_qcf)));
  registerTplSensorData("pto_flow_front", std::move(std::make_unique<QcfFlow>(QcfFlowType::pto)));
  registerTplSensorData("tpl_extension_front", std::move(std::make_unique<TplExtension>()));
  registerTplSensorData("tpl_angle_front", std::move(std::make_unique<TplAngle>()));
  registerTplSensorData("red_qcf_flow_back", std::move(std::make_unique<QcfFlow>(QcfFlowType::red_qcf)));
  registerTplSensorData("green_qcf_flow_back", std::move(std::make_unique<QcfFlow>(QcfFlowType::green_qcf)));
  registerTplSensorData("blue_qcf_flow_back", std::move(std::make_unique<QcfFlow>(QcfFlowType::blue_qcf)));
  registerTplSensorData("pto_flow_back", std::move(std::make_unique<QcfFlow>(QcfFlowType::pto)));
  registerTplSensorData("tpl_extension_back", std::move(std::make_unique<TplExtension>()));
  registerTplSensorData("tpl_angle_back", std::move(std::make_unique<TplAngle>()));
}

void TplStatus::parseTplEncodersFrames()
{
  while (can_handler_->bReceiveFrame())
  {
    assembleEncodersValues(can_handler_->getMostRecentCanFrame());
  }
}

std::optional<double> TplStatus::getEncoderValue(const std::string& encoder_type)
{
  if (keyExists(encoder_type))
  {
    return tpl_sensor_data_[encoder_type]->getAssembledValue();
  }
  return std::nullopt;
}

void TplStatus::resetVisitedEncodersFlag(const std::string& suffix)
{
  for (const auto& [encoder_name, unused_object] : tpl_sensor_data_)
  {
    (void)unused_object;  // cast to void in order to supress compilator error "unused variable"
    if (keyContainsSuffix(encoder_name, suffix))
    {
      tpl_sensor_data_[encoder_name]->resetParsedFlag();
    }
  }
}

bool TplStatus::dataReadyToSend(const std::string& suffix)
{
  // get keys containing suffix front/back
  for (const auto& [encoder_name, encoder_object] : tpl_sensor_data_)
  {
    if (keyContainsSuffix(encoder_name, suffix))
    {
      if (!encoder_object->getAssembledValue())
      {
        return false;
      }
    }
  }
  return true;
}

}  // namespace hal_tool
