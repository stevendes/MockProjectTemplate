#pragma once

#include <memory>
#include <string>
#include <vector>

#include <my_project/sensor_receiver.hpp>

class SensorZoneInterface {
 public:
  virtual ~SensorZoneInterface() = default;
  virtual bool getZoneStatus() = 0;
  virtual std::string getName() const = 0;
};

class SensorZone : public SensorZoneInterface {
 public:
  SensorZone(std::vector<std::unique_ptr<SensorReceiverInterface>>&& sensors_list, std::string& name)
      : sensors_list_(std::move(sensors_list)), zone_name_(name) {}
  bool getZoneStatus() override;
  std::string getName() const override;

 private:
  std::vector<std::unique_ptr<SensorReceiverInterface>> sensors_list_;
  std::string zone_name_;
};
