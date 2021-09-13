/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/ux.hpp>

std::unique_ptr<State> BlockedState::handleEvent(const std::string& message) {
  if (message == memory_->getPassword()) {
    lcd_->printMessage("Password Accepted");
    return std::make_unique<MenuState>(factory_, lcd_, memory_);
  } else {
    lcd_->printMessage("Password Denied");
    return nullptr;
  }
}

std::unique_ptr<State> MenuState::handleEvent(const std::string& message) {
  if (message == "1") {
    lcd_->printMessage("Entering Zone Menu");
    return std::make_unique<ZoneState>(factory_, lcd_, memory_, true);
  } else if (message == "2") {
    lcd_->printMessage("Entering Zone Menu");
    return std::make_unique<ZoneState>(factory_, lcd_, memory_, false);
  } else if (message == "3") {
    memory_->setAlarmStatus(true);
    lcd_->printMessage("Alarm Armed. Blocking Terminal");
    return std::make_unique<BlockedState>(factory_, lcd_, memory_);
  } else if (message == "4") {
    memory_->setAlarmStatus(false);
    lcd_->printMessage("Alarm Disarmed. Blocking Terminal");
    return std::make_unique<BlockedState>(factory_, lcd_, memory_);
  } else {
    lcd_->printMessage("Blocking Terminal");
    return std::make_unique<BlockedState>(factory_, lcd_, memory_);
  }
}

std::unique_ptr<State> ZoneState::handleEvent(const std::string& message) {
  if (memory_->setZoneStatus(message, status_) == true) {
    lcd_->printMessage("SUCCESS");
  } else {
    lcd_->printMessage("FAILURE");
  }
  return std::make_unique<BlockedState>(factory_, lcd_, memory_);
}
