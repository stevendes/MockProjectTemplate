/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/memory.hpp>

Memory::Memory(std::vector<std::string> zone_names) {
  for (unsigned i = 0; i < zone_names.size(); i++) {
    zone_status_[zone_names[i]] = false;
  }
}

bool Memory::getZoneStatus(const std::string& name) const {
  if (zone_status_.count(name) == 0) {
    throw std::invalid_argument("Zone name does not exist");
  } else {
    return zone_status_.at(name);
  }
}

bool Memory::setZoneStatus(const std::string& name, const bool status) {
  const std::lock_guard<std::mutex> lock_joint(set_memory_mutex_);
  if (zone_status_.count(name) == 1) {
    zone_status_[name] = status;
    return true;
  } else {
    return false;
  }
}

void Memory::setPassword(const std::string& password) { password_ = password; }

std::string Memory::getPassword() const { return password_; }

void Memory::setAlarmStatus(const bool status) { alarm_status_ = status; }

bool Memory::isAlarmEnabled() const { return alarm_status_; }
