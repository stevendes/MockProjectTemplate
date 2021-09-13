/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/manager.hpp>

void Manager::timerCallback() {
  bool alarm_value = false;
  // If the alarm is enabled, proceed with the checks
  {
    const std::lock_guard<std::mutex> lock_joint(memory_mutex_);
    alarm_value = memory_->isAlarmEnabled();
  }
  if (alarm_value == true) {
    if (last_speaker_state_ == false) {
      for (unsigned i = 0; i < zone_list_.size(); i++) {
        if (zone_list_[i]->getZoneStatus() == true) {
          speaker_->setSpeaker(true);
          last_speaker_state_ = true;
          break;
        }
      }
    }
  } else if (alarm_value == false && last_speaker_state_ == true) {
    speaker_->setSpeaker(false);
    last_speaker_state_ = false;
  }
}
