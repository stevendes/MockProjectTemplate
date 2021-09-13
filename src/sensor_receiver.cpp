/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/sensor_receiver.hpp>

void SensorReceiver::pongCallback() { ping_received_ = true; }

void SensorReceiver::timerCallback() {
  if (ping_received_ == false) {
    alarm_status_ = true;
  } else {
    alarm_status_ = false;
  }
  ping_received_ = false;
  remote_sensor_->ping();
}

bool SensorReceiver::getAlarmStatus() const { return alarm_status_; }
