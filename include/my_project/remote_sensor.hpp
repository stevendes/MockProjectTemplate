#pragma once

#include <stdlib.h>

#include <functional>

class RemoteSensorInterface {
 public:
  virtual void ping() const = 0;
  virtual void setPongCallback(std::function<void()> callback) = 0;
  virtual ~RemoteSensorInterface() = default;
};
