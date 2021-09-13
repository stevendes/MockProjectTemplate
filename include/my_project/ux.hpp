#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <stdlib.h>

#include <my_project/keyboard.hpp>
#include <my_project/lcd.hpp>
#include <my_project/memory.hpp>

class UXInterface {
 public:
  virtual ~UXInterface() = default;
};

enum class Options { BLOCKED, MENU };

class StateFactory;

class State {
 public:
  virtual ~State() = default;
  virtual void onEntry(){};
  virtual void onExit(){};

  virtual std::unique_ptr<State> handleEvent(const std::string& message) = 0;
};

class BlockedState : public State {
 public:
  BlockedState(StateFactory* factory, LCDInterface* lcd, MemoryInterface* memory)
      : factory_{factory}, lcd_{lcd}, memory_{memory} {};
  void onEntry() override { lcd_->printMessage("Write the password to access the Menu"); }
  void onExit() override {}

  std::unique_ptr<State> handleEvent(const std::string& message) override;

 private:
  StateFactory* factory_;
  LCDInterface* lcd_;
  MemoryInterface* memory_;
};

class MenuState : public State {
 public:
  MenuState(StateFactory* factory, LCDInterface* lcd, MemoryInterface* memory)
      : factory_{factory}, lcd_{lcd}, memory_{memory} {};
  void onEntry() override {
    lcd_->printMessage("Select your option");
    lcd_->printMessage("1. Enable Zone");
    lcd_->printMessage("2. Disable Zone");
    lcd_->printMessage("3. Arm Alarm");
    lcd_->printMessage("4. Disarm Alarm");
    lcd_->printMessage("Any other input blocks the terminal");
  }
  void onExit() override {}

  std::unique_ptr<State> handleEvent(const std::string& message) override;

 private:
  StateFactory* factory_;
  LCDInterface* lcd_;
  MemoryInterface* memory_;
};

class ZoneState : public State {
 public:
  ZoneState(StateFactory* factory, LCDInterface* lcd, MemoryInterface* memory, const bool status)
      : factory_{factory}, lcd_{lcd}, memory_{memory}, status_{status} {};
  void onEntry() override { lcd_->printMessage("Enter the name of the Zone you want to Enable/Disable"); }
  void onExit() override {}

  std::unique_ptr<State> handleEvent(const std::string& message) override;

 private:
  StateFactory* factory_;
  LCDInterface* lcd_;
  MemoryInterface* memory_;
  bool status_;
};

class StateFactory {
 public:
  StateFactory(std::unique_ptr<LCDInterface> lcd, std::unique_ptr<MemoryInterface> memory)
      : lcd_{std::move(lcd)}, memory_{std::move(memory)} {};
  std::unique_ptr<State> createMenuState() { return std::make_unique<MenuState>(this, lcd_.get(), memory_.get()); }
  std::unique_ptr<State> createBlockedState() {
    return std::make_unique<BlockedState>(this, lcd_.get(), memory_.get());
  }
  std::unique_ptr<State> createZoneState(const bool status) {
    return std::make_unique<ZoneState>(this, lcd_.get(), memory_.get(), status);
  }

 private:
  std::unique_ptr<LCDInterface> lcd_;
  std::unique_ptr<MemoryInterface> memory_;
};

class StateMachine {
 public:
  StateMachine(std::unique_ptr<StateFactory> factory) : factory_{std::move(factory)} {
    // initialize the state machine on State1
    state_ = factory_->createBlockedState();
    state_->onEntry();
  }

  void handleEvent(std::string& message) {
    auto ret_ptr = state_->handleEvent(message);
    if (ret_ptr) {
      // the state event handler decided we should move to some other state
      state_->onExit();
      state_ = std::move(ret_ptr);
      state_->onEntry();
    }
  }

 private:
  std::unique_ptr<State> state_;
  std::unique_ptr<StateFactory> factory_;
};
