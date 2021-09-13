/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/ux.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

// class MockKeyboard : public KeyboardInterface {
//  public:
//   MOCK_METHOD(std::string, send_input, (), (const));
// };

class MockLCD : public LCDInterface {
 public:
  MOCK_METHOD(void, printMessage, (const std::string message), (const));
};

class MockMemory : public MemoryInterface {
 public:
  MOCK_METHOD(bool, getZoneStatus, (const std::string& name), (const));
  MOCK_METHOD(bool, setZoneStatus, (const std::string& name, const bool status));
  MOCK_METHOD(void, setPassword, (const std::string& password));
  MOCK_METHOD(std::string, getPassword, (), (const));
  MOCK_METHOD(void, setAlarmStatus, (const bool status));
  MOCK_METHOD(bool, isAlarmEnabled, (), (const));
};

TEST(UxTest, CheckEnteringWrongPassword) {
  InSequence seq;
  auto lcd_mock = std::make_unique<MockLCD>();
  auto memory_mock = std::make_unique<MockMemory>();
  EXPECT_CALL(*lcd_mock, printMessage("Write the password to access the Menu")).Times(1);
  // EXPECT_CALL(*memory_mock, setPongCallback(_)).Times(1);
  std::unique_ptr<LCDInterface> lcd_mock_pointer = std::move(lcd_mock);
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  auto sf = std::make_unique<StateFactory>(std::move(lcd_mock_pointer), std::move(memory_mock_pointer));
  StateMachine(std::move(sf));
}

TEST(UxTest, CheckBlockedState) {
  InSequence seq;
  auto lcd_mock = std::make_unique<MockLCD>();
  auto memory_mock = std::make_unique<MockMemory>();
  std::string password = "12345";
  EXPECT_CALL(*lcd_mock, printMessage("Write the password to access the Menu")).Times(1);
  EXPECT_CALL(*memory_mock, getPassword()).WillOnce(Return(password));
  EXPECT_CALL(*lcd_mock, printMessage("Password Accepted")).Times(1);
  EXPECT_CALL(*memory_mock, getPassword()).WillOnce(Return(password));
  EXPECT_CALL(*lcd_mock, printMessage("Password Denied")).Times(1);
  std::unique_ptr<LCDInterface> lcd_mock_pointer = std::move(lcd_mock);
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  auto sf = std::make_unique<StateFactory>(std::move(lcd_mock_pointer), std::move(memory_mock_pointer));
  auto state = sf->createBlockedState();
  state->onEntry();
  // Test Correct Password
  auto new_state = state->handleEvent(password);
  EXPECT_NE(nullptr, dynamic_cast<MenuState*>(new_state.get()));
  // Test Incorrect Password
  state->handleEvent("wrong password");
}

TEST(UxTest, CheckMenuState) {
  InSequence seq;
  auto lcd_mock = std::make_unique<MockLCD>();
  auto memory_mock = std::make_unique<MockMemory>();
  EXPECT_CALL(*lcd_mock, printMessage("Select your option")).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("1. Enable Zone")).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("2. Disable Zone")).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("3. Arm Alarm")).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("4. Disarm Alarm")).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("Any other input blocks the terminal")).Times(1);
  // Expect entering ZoneState
  EXPECT_CALL(*lcd_mock, printMessage("Entering Zone Menu")).Times(2);
  // Expect Enabling the Alarm
  EXPECT_CALL(*memory_mock, setAlarmStatus(true)).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("Alarm Armed. Blocking Terminal")).Times(1);
  // Expect Disabling the Alarm
  EXPECT_CALL(*memory_mock, setAlarmStatus(false)).Times(1);
  EXPECT_CALL(*lcd_mock, printMessage("Alarm Disarmed. Blocking Terminal")).Times(1);
  // Expect entering BlockedState
  EXPECT_CALL(*lcd_mock, printMessage("Blocking Terminal")).Times(1);
  std::unique_ptr<LCDInterface> lcd_mock_pointer = std::move(lcd_mock);
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  auto sf = std::make_unique<StateFactory>(std::move(lcd_mock_pointer), std::move(memory_mock_pointer));
  auto state = sf->createMenuState();
  state->onEntry();
  // Test entering in Zone Menu
  auto new_state_1 = state->handleEvent("1");
  EXPECT_NE(nullptr, dynamic_cast<ZoneState*>(new_state_1.get()));
  auto new_state_2 = state->handleEvent("2");
  EXPECT_NE(nullptr, dynamic_cast<ZoneState*>(new_state_2.get()));
  // Test entering Alarm Menu
  auto new_state_3 = state->handleEvent("3");
  EXPECT_NE(nullptr, dynamic_cast<BlockedState*>(new_state_3.get()));
  auto new_state_4 = state->handleEvent("4");
  EXPECT_NE(nullptr, dynamic_cast<BlockedState*>(new_state_4.get()));
  // Test entering wrong input
  auto new_state_5 = state->handleEvent("asdf");
  EXPECT_NE(nullptr, dynamic_cast<BlockedState*>(new_state_5.get()));
}

TEST(UxTest, CheckZoneState) {
  InSequence seq;
  auto lcd_mock = std::make_unique<MockLCD>();
  auto memory_mock = std::make_unique<MockMemory>();
  const std::string zone = "Kitchen";
  EXPECT_CALL(*lcd_mock, printMessage("Enter the name of the Zone you want to Enable/Disable")).Times(1);
  EXPECT_CALL(*memory_mock, setZoneStatus(zone, true)).WillOnce(Return(true));
  EXPECT_CALL(*lcd_mock, printMessage("SUCCESS")).Times(1);
  std::unique_ptr<LCDInterface> lcd_mock_pointer = std::move(lcd_mock);
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  auto sf = std::make_unique<StateFactory>(std::move(lcd_mock_pointer), std::move(memory_mock_pointer));
  auto state = sf->createZoneState(true);
  state->onEntry();
  auto new_state = state->handleEvent("Kitchen");
  EXPECT_NE(nullptr, dynamic_cast<BlockedState*>(new_state.get()));
}
