#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include <stdlib.h>

#include <my_project/Timer.hpp>
#include <my_project/remote_sensor.hpp>

class SensorReceiverInterface {
 public:
  virtual ~SensorReceiverInterface() = default;
  virtual bool getAlarmStatus() const = 0;
};

class SensorReceiver : public SensorReceiverInterface {
 public:
  SensorReceiver(std::unique_ptr<RemoteSensorInterface>& remote_sensor) : remote_sensor_{std::move(remote_sensor)} {
    remote_sensor_->setPongCallback([this]() { pongCallback(); });
    timer_.start(std::chrono::milliseconds{100});
  };
  ~SensorReceiver() = default;
  void pongCallback();
  void timerCallback();
  bool getAlarmStatus() const;

 private:
  std::unique_ptr<RemoteSensorInterface> remote_sensor_;
  my_project::utils::Timer timer_{[this]() { timerCallback(); }};
  bool alarm_status_{false};
  bool ping_received_{true};
};
