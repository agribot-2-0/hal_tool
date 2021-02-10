/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#include <hal_tool/TplStatusNode/TplStatusNode.h>

TplStatusNode::TplStatusNode(int argc, char** argv)
{
  ros::init(argc, argv, "hal_tpl_status_node");
  node_handle_ = std::make_unique<ros::NodeHandle>();
  tpl_status_front_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/front", 1);
  tpl_status_back_pub_ = node_handle_->advertise<agribot_msgs::AgribotTool>("/agribot/encoder/tool/back", 1);
}

void TplStatusNode::run()
{
  ros::Rate rate(ros_loop_rate_);
  while (ros::ok())
  {
    ros::spinOnce();
    rate.sleep();
  }
}
