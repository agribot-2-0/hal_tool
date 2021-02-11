/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatus/TplStatus.h>

// DEBUG
#include <iostream>

namespace hal_tool
{
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
      assembleChassis(frame_data);
      break;
    case (encoder_can_id::flow_pto_front):
      assembleRgbFlowAndPto(frame_data, "front");
      break;
    case (encoder_can_id::tpl_prismatic_front):
      assembleTplPrismatic(frame_data, "front");
      break;
    case (encoder_can_id::tpl_revolute_front):
      assembleTplRevolute(frame_data, "front");
      break;
    case (encoder_can_id::flow_pto_back):
      assembleRgbFlowAndPto(frame_data, "back");
      break;
    case (encoder_can_id::tpl_prismatic_back):
      assembleTplPrismatic(frame_data, "back");
      break;
    case (encoder_can_id::tpl_revolute_back):
      assembleTplRevolute(frame_data, "back");
      break;
  }
}

bool TplStatus::keyExists(const std::string& encoder_name)
{
  if (encoders_.find(encoder_name) != encoders_.end())
  {
    return true;
  }
  return false;
}

TplStatus::TplStatus(std::unique_ptr<ICan> can_handle) : can_handle_{ std::move(can_handle) }
{
  registerEncoder("chassis", std::move(std::make_unique<ChassisEncoder>()));
  registerEncoder("red_flow_front", std::move(std::make_unique<FlowEncoder>(flow_encoder::red)));
  registerEncoder("green_flow_front", std::move(std::make_unique<FlowEncoder>(flow_encoder::green)));
  registerEncoder("blue_flow_front", std::move(std::make_unique<FlowEncoder>(flow_encoder::blue)));
  registerEncoder("pto_flow_front", std::move(std::make_unique<FlowEncoder>(flow_encoder::pto)));
  registerEncoder("tpl_prismatic_front", std::move(std::make_unique<TplPrismaticEncoder>()));
  registerEncoder("tpl_revolute_front", std::move(std::make_unique<TplRevoluteEncoder>()));
  registerEncoder("red_flow_back", std::move(std::make_unique<FlowEncoder>(flow_encoder::red)));
  registerEncoder("green_flow_back", std::move(std::make_unique<FlowEncoder>(flow_encoder::green)));
  registerEncoder("blue_flow_back", std::move(std::make_unique<FlowEncoder>(flow_encoder::blue)));
  registerEncoder("pto_flow_back", std::move(std::make_unique<FlowEncoder>(flow_encoder::pto)));
  registerEncoder("tpl_prismatic_back", std::move(std::make_unique<TplPrismaticEncoder>()));
  registerEncoder("tpl_revolute_back", std::move(std::make_unique<TplRevoluteEncoder>()));
}

void TplStatus::parseTplEncodersFrames()
{
  while (can_handle_->bReceiveFrame())
  {
    auto frame = can_handle_->getMostRecentCanFrame();
    assembleEncodersValues(frame.getId(), frame.getData());
  }
}

std::optional<double> TplStatus::getEncoderValue(const std::string& encoder_type)
{
  if (keyExists(encoder_type))
  {
    return encoders_[encoder_type]->getAssembledValue();
  }
  return std::nullopt;
}

void TplStatus::resetVisitedEncodersFlag(const std::vector<std::string>& encoders)
{
  for (const auto& encoder_to_reset : encoders)
  {
    if (keyExists(encoder_to_reset))
    {
      encoders_[encoder_to_reset]->resetParsedFlag();
    }
  }
}

}  // namespace hal_tool
