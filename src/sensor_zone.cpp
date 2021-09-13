/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/sensor_zone.hpp>

bool SensorZone::getZoneStatus() {
  bool zone_status;
  unsigned counter = 0;
  for (unsigned i = 0; i < sensors_list_.size(); i++) {
    if (sensors_list_[i]->getAlarmStatus() == true) {
      zone_status = true;
    } else {
      counter++;
    }
  }
  if (counter == sensors_list_.size()) {
    zone_status = false;
  }
  return zone_status;
}

std::string SensorZone::getName() const { return zone_name_; }
