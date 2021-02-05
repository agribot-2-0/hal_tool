/*
 * Copyright (C) 2021 Agribot. All rights reserved.
 */

#pragma once

#include <agribot_common/CanBus/ICan.h>
#include <agribot_common/Can.h>
#include <memory>

class TplStatus
{
 private:
  std::shared_ptr<ICan> can_handle_;

 public:
  TplStatus(std::unique_ptr<ICan> can_handle) : can_handle_(std::move(can_handle)) {}
};
