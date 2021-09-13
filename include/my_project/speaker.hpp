#pragma once

#include <functional>
#include <string>
#include <vector>

class SpeakerInterface {
 public:
  virtual ~SpeakerInterface() = default;
  virtual bool setSpeaker(const bool status) const = 0;
};
