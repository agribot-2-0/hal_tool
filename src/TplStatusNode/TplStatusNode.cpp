/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatusNode/TplStatusNode.h>

TplStatusNode::TplStatusNode(int argc, char** argv)
{
  std::string selected_can;
  ros::init(argc, argv, "hal_tpl_status_node");
  node_handler_ = std::make_unique<ros::NodeHandle>();
  tpl_status_front_pub_ = node_handler_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/front", 1);
  tpl_status_back_pub_ = node_handler_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/back", 1);
  node_handler_->param<std::string>("/hal_tool/can_bus_name", selected_can, "can0");
  std::unique_ptr<ICan> handle = std::make_unique<Can>();

  handle->iInitialize(selected_can.c_str());
  tpl_state_ = std::make_unique<hal_tool::TplStatus>(std::move(handle));
}

double TplStatusNode::getFrontTplAngleRelativeToChassis()
{
  return *(tpl_state_->getEncoderValue("tpl_angle_front")) - *(tpl_state_->getEncoderValue("chassis_"
                                                                                           "inclination"));
}

double TplStatusNode::getBackTplAngleRelativeToChassis()
{
  return *(tpl_state_->getEncoderValue("tpl_angle_back")) + *(tpl_state_->getEncoderValue("chassis_"
                                                                                          "inclination"));
}

void TplStatusNode::sendFrontStatusMsg()
{
  agribot_msgs::AgribotTool msg;
  msg.pto_rpm = *(tpl_state_->getEncoderValue("pto_flow_front"));
  msg.red_valve_flow = *(tpl_state_->getEncoderValue("red_qfc_flow_front"));
  msg.green_valve_flow = *(tpl_state_->getEncoderValue("green_qfc_flow_front"));
  msg.blue_valve_flow = *(tpl_state_->getEncoderValue("blue_qfc_flow_front"));
  msg.tpl_height = *(tpl_state_->getEncoderValue("tpl_extension_front"));
  msg.tpl_angle = getFrontTplAngleRelativeToChassis();
  tpl_status_front_pub_.publish(msg);
}

void TplStatusNode::sendBackStatusMsg()
{
  agribot_msgs::AgribotTool msg;
  msg.pto_rpm = *(tpl_state_->getEncoderValue("pto_flow_back"));
  msg.red_valve_flow = *(tpl_state_->getEncoderValue("red_qfc_flow_back"));
  msg.green_valve_flow = *(tpl_state_->getEncoderValue("green_qfc_flow_back"));
  msg.blue_valve_flow = *(tpl_state_->getEncoderValue("blue_qfc_flow_back"));
  msg.tpl_height = *(tpl_state_->getEncoderValue("tpl_extension_back"));
  msg.tpl_angle = getBackTplAngleRelativeToChassis();
  tpl_status_back_pub_.publish(msg);
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
