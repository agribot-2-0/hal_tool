/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <ros/ros.h>
#include <ros/console.h>
#include <agribot_msgs/AgribotTool.h>
#include <memory>

class TplStatusNode
{
 private:
  int ros_loop_rate_ = 10;
  std::unique_ptr<ros::NodeHandle> node_handle_;

  ros::Publisher tpl_status_front_pub_;
  ros::Publisher tpl_status_back_pub_;

 public:
  TplStatusNode(int argc, char** argv);
  void run();
};
