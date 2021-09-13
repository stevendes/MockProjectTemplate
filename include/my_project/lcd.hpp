#pragma once

#include <functional>
#include <string>
#include <vector>

class LCDInterface {
 public:
  virtual ~LCDInterface() = default;
  virtual void printMessage(const std::string message) const = 0;
};
