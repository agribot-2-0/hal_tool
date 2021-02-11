/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatusNode/TplStatusNode.h>

constexpr char selected_can[] = "can1";

TplStatusNode::TplStatusNode(int argc, char** argv)
{
  // ros stuff
  ros::init(argc, argv, "hal_tpl_status_node");
  node_handle_ = std::make_unique<ros::NodeHandle>();
  tpl_status_front_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/front", 1);
  tpl_status_back_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/back", 1);

  // monitor
  std::unique_ptr<ICan> can_handle_ = std::make_unique<Can>();
  can_handle_->iInitialize(selected_can);  // change to param!!!!! same for encoders ID
  tpl_state_ = std::make_unique<hal_tool::TplStatus>(std::move(can_handle_));
}

void TplStatusNode::run()
{
  ros::Rate rate(ros_loop_rate_);
  while (ros::ok())
  {
    tpl_state_->parseTplEncodersFrames();

    ros::spinOnce();
    rate.sleep();
  }
}
