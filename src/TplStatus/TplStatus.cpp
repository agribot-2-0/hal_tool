/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatus/TplStatus.h>

// DEBUG
#include <iostream>

void TplStatus::registerEncoder(const std::string& encoder_name, std::unique_ptr<ICanData> encoder)
{
  encoders_.insert({ encoder_name, std::move(encoder) });
}

void TplStatus::assembleChassis(const std::vector<uint8_t>& frame_data)
{
  encoders_["chassis"]->assembleValueFromFrame(frame_data);
}

void TplStatus::assembleRgbFlowAndPto(const std::vector<uint8_t>& frame_data, const std::string& suffix)
{
  encoders_["red_flow_" + suffix]->assembleValueFromFrame(frame_data);
  encoders_["green_flow_" + suffix]->assembleValueFromFrame(frame_data);
  encoders_["blue_flow_" + suffix]->assembleValueFromFrame(frame_data);
  encoders_["pto_flow_" + suffix]->assembleValueFromFrame(frame_data);
}

void TplStatus::assembleTplPrismatic(const std::vector<uint8_t>& frame_data, const std::string& suffix)
{
  encoders_["tpl_prismatic_" + suffix]->assembleValueFromFrame(frame_data);
}

void TplStatus::assembleTplRevolute(const std::vector<uint8_t>& frame_data, const std::string& suffix)
{
  encoders_["tpl_revolute_" + suffix]->assembleValueFromFrame(frame_data);
}

void TplStatus::assembleEncodersValues(uint32_t frame_id, const std::vector<uint8_t>& frame_data)
{
  switch (frame_id)
  {
    case (encoder_can_id::chassis):
      std::cerr << "chassis id detected" << std::endl;
      assembleChassis(frame_data);
      break;
    case (encoder_can_id::flow_pto_front):
      std::cerr << "flow_pto_front id detected" << std::endl;
      assembleRgbFlowAndPto(frame_data, "front");
      break;
    case (encoder_can_id::tpl_prismatic_front):
      std::cerr << "tpl_prismatic_front id detected" << std::endl;
      assembleTplPrismatic(frame_data, "front");
      break;
    case (encoder_can_id::tpl_revolute_front):
      std::cerr << "tpl_revolute_front id detected" << std::endl;
      assembleTplRevolute(frame_data, "front");
      break;
    case (encoder_can_id::flow_pto_back):
      std::cerr << "flow_pto_back id detected" << std::endl;
      assembleRgbFlowAndPto(frame_data, "back");
      break;
    case (encoder_can_id::tpl_prismatic_back):
      std::cerr << "tpl_prismatic_back id detected" << std::endl;
      assembleTplPrismatic(frame_data, "back");
      break;
    case (encoder_can_id::tpl_revolute_back):
      std::cerr << "tpl_revolute_back id detected" << std::endl;
      assembleTplRevolute(frame_data, "back");
      break;
  }
}

TplStatus::TplStatus(std::unique_ptr<ICan> can_handle) : can_handle_{ std::move(can_handle) }
{
  registerEncoder("chassis", std::move(std::make_unique<ChassisEncoder>()));
  registerEncoder("red_flow_front", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("green_flow_front", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("blue_flow_front", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("pto_flow_front", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("tpl_prismatic_front", std::move(std::make_unique<TplPrismaticEncoder>()));
  registerEncoder("tpl_revolute_front", std::move(std::make_unique<TplRevoluteEncoder>()));
  registerEncoder("red_flow_back", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("green_flow_back", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("blue_flow_back", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("pto_flow_back", std::move(std::make_unique<FlowEncoder>()));
  registerEncoder("tpl_prismatic_back", std::move(std::make_unique<TplPrismaticEncoder>()));
  registerEncoder("tpl_revolute_back", std::move(std::make_unique<TplRevoluteEncoder>()));
}

void TplStatus::loadCanFrames()
{
  while (can_handle_->bReceiveFrame())
  {
    std::cerr << "return true once" << std::endl;
    can_frames_.push_back(can_handle_->getMostRecentCanFrame());
  }
}

void TplStatus::parseTplEncoders()
{
  for (const auto& frame : can_frames_)
  {
    assembleEncodersValues(frame.getId(), frame.getData());
  }
  can_frames_.clear();
}

std::optional<double> TplStatus::getEncoderValue(const std::string& encoder_type)
{
  return encoders_[encoder_type]->getAssembledValue();
}
