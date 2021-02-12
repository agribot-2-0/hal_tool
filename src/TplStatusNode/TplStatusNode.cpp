/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatusNode/TplStatusNode.h>

constexpr char selected_can[] = "can1";

TplStatusNode::TplStatusNode(int argc, char** argv)
{
  ros::init(argc, argv, "hal_tpl_status_node");
  node_handle_ = std::make_unique<ros::NodeHandle>();
  tpl_status_front_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/front", 1);
  tpl_status_back_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/back", 1);

  std::unique_ptr<ICan> handle = std::make_unique<Can>();
  handle->iInitialize(selected_can);  // change to param!!!!! same for encoders ID
  tpl_state_ = std::make_unique<hal_tool::TplStatus>(std::move(handle));
}

void TplStatusNode::sendFrontStatusMsg()
{
  agribot_msgs::AgribotTool msg;
  msg.pto_rpm = *(tpl_state_->getEncoderValue("pto_flow_front"));
  msg.red_valve_flow = *(tpl_state_->getEncoderValue("red_flow_front"));
  msg.green_valve_flow = *(tpl_state_->getEncoderValue("green_flow_front"));
  msg.blue_valve_flow = *(tpl_state_->getEncoderValue("blue_flow_front"));
  msg.tpl_height = *(tpl_state_->getEncoderValue("tpl_prismatic_front"));
  msg.tpl_angle = *(tpl_state_->getEncoderValue("tpl_revolute_front"));
  tpl_status_front_pub_.publish(msg);
}

void TplStatusNode::sendBackStatusMsg()
{
  agribot_msgs::AgribotTool msg;
  msg.pto_rpm = *(tpl_state_->getEncoderValue("pto_flow_back"));
  msg.red_valve_flow = *(tpl_state_->getEncoderValue("red_flow_back"));
  msg.green_valve_flow = *(tpl_state_->getEncoderValue("green_flow_back"));
  msg.blue_valve_flow = *(tpl_state_->getEncoderValue("blue_flow_back"));
  msg.tpl_height = *(tpl_state_->getEncoderValue("tpl_prismatic_back"));
  msg.tpl_angle = *(tpl_state_->getEncoderValue("tpl_revolute_back"));
  tpl_status_front_pub_.publish(msg);
}

void TplStatusNode::run()
{
  ros::Rate rate(ros_loop_rate_);
  while (ros::ok())
  {
    tpl_state_->parseTplEncodersFrames();
    if (tpl_state_->dataReadyToSend("front"))
    {
      sendFrontStatusMsg();
      tpl_state_->resetVisitedEncodersFlag("front");
    }
    if (tpl_state_->dataReadyToSend("back"))
    {
      sendBackStatusMsg();
      tpl_state_->resetVisitedEncodersFlag("back");
    }
    ros::spinOnce();
    rate.sleep();
  }
}
