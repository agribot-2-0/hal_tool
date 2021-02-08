/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatus/TplStatus.h>

void TplStatus::registerEncoder(const std::string_view& encoder_name, std::unique_ptr<ICanData> encoder)
{
  encoders_.insert({ encoder_name, std::move(encoder) });
}

void TplStatus::setupRgbFlowAndPto(const std::vector<uint8_t>& frame_data, std::string_view side_suffix)
{
  encoders_[static_cast<std::string_view>("red_flow") + side_suffix]->assembleValueFromFrame(frame_data);
  encoders_["green_flow"]->assembleValueFromFrame(frame_data);
  encoders_["blue_flow"]->assembleValueFromFrame(frame_data);
  encoders_["pto_flow"]->assembleValueFromFrame(frame_data);
}

void TplStatus::setupEncoderValue(uint16_t frame_id, const std::vector<uint8_t>& frame_data)
{
  switch (frame_id)
  {
    case (encoder_can_id::chassis):
      break;
    case (encoder_can_id::flow_pto_front):
      setupRgbFlowAndPto(frame_data);
      break;
    case (encoder_can_id::tpl_prismatic_front):

      break;
    case (encoder_can_id::tpl_revolute_front):

      break;
    case (encoder_can_id::flow_pto_back):

      break;
    case (encoder_can_id::tpl_prismatic_back):

      break;
    case (encoder_can_id::tpl_revolute_back):

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
    can_frames_.push_back(can_handle_->getMostRecentCanFrame());
  }
}

void TplStatus::assembleEncodersValues()
{
  for (const auto& frame : can_frames_)
  {
    setupEncoderValue(frame.getId(), frame.getData());
  }
  can_frames_.clear();
}

double TplStatus::getEncoderValue(std::string_view& encoder_type)
{
  return encoders_[encoder_type]->getAssembledValue();
}