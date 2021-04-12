/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/tpl_status_node/TplStatusNode.h>
#include <agribot_common/CommonFunctions/ros_common_functions.h>

TplStatusNode::TplStatusNode(int argc, char** argv)
{
  ros::init(argc, argv, "hal_tpl_status_node");
  std::string selected_can;
  try
  {
    selected_can = ros_common_functions::getRosParam<std::string>("/hal_tool/can_bus_name");
  }
  catch (const std::exception& ex)
  {
    ROS_ERROR_STREAM("[TPL_STATUS_NODE] " << ex.what());
    ros::shutdown();
  }
  node_handler_ = std::make_unique<ros::NodeHandle>();
  tpl_status_front_pub_ = node_handler_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/front", 1);
  tpl_status_back_pub_ = node_handler_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/back", 1);
  std::unique_ptr<ICan> handler = std::make_unique<Can>();

  handler->iInitialize(selected_can.c_str());
  tpl_state_ = std::make_unique<hal_tool::TplStatus>(std::move(handler));
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

void TplStatusNode::sendStatusMsg(const std::string& suffix)
{
  agribot_msgs::AgribotTool msg;
  msg.pto_rpm = *(tpl_state_->getEncoderValue("pto_flow_" + suffix));
  msg.red_valve_flow = *(tpl_state_->getEncoderValue("red_qcf_flow_" + suffix));
  msg.green_valve_flow = *(tpl_state_->getEncoderValue("green_qcf_flow_" + suffix));
  msg.blue_valve_flow = *(tpl_state_->getEncoderValue("blue_qcf_flow_" + suffix));
  msg.tpl_height = *(tpl_state_->getEncoderValue("tpl_extension_" + suffix));
  if (suffix == "front")
  {
    msg.tpl_angle = getFrontTplAngleRelativeToChassis();
    tpl_status_front_pub_.publish(msg);
  }
  else if (suffix == "back")
  {
    msg.tpl_angle = getBackTplAngleRelativeToChassis();
    tpl_status_back_pub_.publish(msg);
  }
}

void TplStatusNode::run()
{
  ros::Rate rate(ros_loop_rate_);
  while (ros::ok())
  {
    tpl_state_->parseTplEncodersFrames();
    if (tpl_state_->dataReadyToSend("front"))
    {
      sendStatusMsg("front");
      tpl_state_->resetVisitedEncodersFlag("front");
    }
    if (tpl_state_->dataReadyToSend("back"))
    {
      sendStatusMsg("back");
      tpl_state_->resetVisitedEncodersFlag("back");
    }
    ros::spinOnce();
    rate.sleep();
  }
}
