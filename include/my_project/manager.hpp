#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <stdlib.h>

#include <my_project/Timer.hpp>
#include <my_project/memory.hpp>
#include <my_project/sensor_zone.hpp>
#include <my_project/speaker.hpp>

class ManagerInterface {
 public:
  virtual ~ManagerInterface() = default;
};

class Manager : public ManagerInterface {
 public:
  Manager(std::unique_ptr<MemoryInterface> memory, std::unique_ptr<SpeakerInterface> speaker,
          std::vector<std::unique_ptr<SensorZoneInterface>>&& zone_list)
      : memory_{std::move(memory)}, speaker_{std::move(speaker)}, zone_list_(std::move(zone_list)) {
    speaker_->setSpeaker(false);
    timer_.start(std::chrono::milliseconds{100});
  }
  ~Manager() = default;
  void timerCallback();

 private:
  std::unique_ptr<MemoryInterface> memory_;
  std::mutex memory_mutex_;
  std::unique_ptr<SpeakerInterface> speaker_;
  std::vector<std::unique_ptr<SensorZoneInterface>> zone_list_;
  my_project::utils::Timer timer_{[this]() { timerCallback(); }};
  bool last_speaker_state_{false};
};
