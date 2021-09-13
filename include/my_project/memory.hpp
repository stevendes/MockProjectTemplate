#pragma once

#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

class MemoryInterface {
 public:
  virtual ~MemoryInterface() = default;
  virtual bool getZoneStatus(const std::string& name) const = 0;
  virtual bool setZoneStatus(const std::string& name, const bool status) = 0;
  virtual void setPassword(const std::string& password) = 0;
  virtual std::string getPassword() const = 0;
  virtual void setAlarmStatus(const bool status) = 0;
  virtual bool isAlarmEnabled() const = 0;
};

class Memory : public MemoryInterface {
 public:
  Memory(std::vector<std::string> zone_names);
  bool getZoneStatus(const std::string& name) const;
  bool setZoneStatus(const std::string& name, const bool status);
  void setPassword(const std::string& password);
  void setAlarmStatus(const bool status);
  bool isAlarmEnabled() const;

  std::string getPassword() const;

 private:
  std::map<std::string, bool> zone_status_;
  std::string password_;
  bool alarm_status_{true};
  std::mutex set_memory_mutex_;
};
