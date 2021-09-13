#pragma once

#include <functional>
#include <string>
#include <vector>

class KeyboardInterface {
 public:
  virtual ~KeyboardInterface() = default;
  virtual std::string send_input() const = 0;
};
