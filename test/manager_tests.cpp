/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/manager.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

class MockMemory : public MemoryInterface {
 public:
  MOCK_METHOD(bool, getZoneStatus, (const std::string& name), (const));
  MOCK_METHOD(bool, setZoneStatus, (const std::string& name, const bool status));
  MOCK_METHOD(void, setPassword, (const std::string& password));
  MOCK_METHOD(std::string, getPassword, (), (const));
  MOCK_METHOD(void, setAlarmStatus, (const bool status));
  MOCK_METHOD(bool, isAlarmEnabled, (), (const));
};

class MockSpeaker : public SpeakerInterface {
 public:
  MOCK_METHOD(bool, setSpeaker, (const bool state), (const));
};

class MockZone : public SensorZoneInterface {
 public:
  MOCK_METHOD(bool, getZoneStatus, ());
  MOCK_METHOD(std::string, getName, (), (const));
};

TEST(UxTest, CheckAlarmNotBeingActivated) {
  InSequence seq;
  auto memory_mock = std::make_unique<MockMemory>();
  auto speaker_mock = std::make_unique<MockSpeaker>();
  auto zone_1_mock = std::make_unique<MockZone>();
  auto zone_2_mock = std::make_unique<MockZone>();
  auto zone_3_mock = std::make_unique<MockZone>();
  // Making sure the speaker is off on startup
  EXPECT_CALL(*speaker_mock, setSpeaker(false)).Times(1);
  //   First loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(true));
  EXPECT_CALL(*zone_1_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_2_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_3_mock, getZoneStatus()).WillOnce(Return(false));
  // Second loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(true));
  EXPECT_CALL(*zone_1_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_2_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_3_mock, getZoneStatus()).WillOnce(Return(false));
  // Third loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(true));
  EXPECT_CALL(*zone_1_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_2_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_3_mock, getZoneStatus()).WillOnce(Return(false));
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  std::unique_ptr<SpeakerInterface> speaker_mock_pointer = std::move(speaker_mock);
  std::vector<std::unique_ptr<SensorZoneInterface>> mocked_sensor_zone_v;
  mocked_sensor_zone_v.emplace_back(std::move(zone_1_mock));
  mocked_sensor_zone_v.emplace_back(std::move(zone_2_mock));
  mocked_sensor_zone_v.emplace_back(std::move(zone_3_mock));
  auto manager_pointer = std::make_unique<Manager>(std::move(memory_mock_pointer), std::move(speaker_mock_pointer),
                                                   std::move(mocked_sensor_zone_v));
  std::this_thread::sleep_for(std::chrono::milliseconds{350});
}

TEST(UxTest, CheckAlarmActivatedAndReset) {
  InSequence seq;
  auto memory_mock = std::make_unique<MockMemory>();
  auto speaker_mock = std::make_unique<MockSpeaker>();
  auto zone_1_mock = std::make_unique<MockZone>();
  auto zone_2_mock = std::make_unique<MockZone>();
  auto zone_3_mock = std::make_unique<MockZone>();
  // Making sure the speaker is off on startup
  EXPECT_CALL(*speaker_mock, setSpeaker(false)).Times(1);
  // First loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(true));
  EXPECT_CALL(*zone_1_mock, getZoneStatus()).WillOnce(Return(true));
  EXPECT_CALL(*speaker_mock, setSpeaker(true)).Times(1);
  // Second loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(false));
  ;
  EXPECT_CALL(*speaker_mock, setSpeaker(false)).Times(1);
  // Third loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(true));
  EXPECT_CALL(*zone_1_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_2_mock, getZoneStatus()).WillOnce(Return(false));
  EXPECT_CALL(*zone_3_mock, getZoneStatus()).WillOnce(Return(false));
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  std::unique_ptr<SpeakerInterface> speaker_mock_pointer = std::move(speaker_mock);
  std::vector<std::unique_ptr<SensorZoneInterface>> mocked_sensor_zone_v;
  mocked_sensor_zone_v.emplace_back(std::move(zone_1_mock));
  mocked_sensor_zone_v.emplace_back(std::move(zone_2_mock));
  mocked_sensor_zone_v.emplace_back(std::move(zone_3_mock));
  auto manager_pointer = std::make_unique<Manager>(std::move(memory_mock_pointer), std::move(speaker_mock_pointer),
                                                   std::move(mocked_sensor_zone_v));
  std::this_thread::sleep_for(std::chrono::milliseconds{350});
}

TEST(UxTest, CheckAlarmNotArmed) {
  InSequence seq;
  auto memory_mock = std::make_unique<MockMemory>();
  auto speaker_mock = std::make_unique<MockSpeaker>();
  auto zone_1_mock = std::make_unique<MockZone>();
  auto zone_2_mock = std::make_unique<MockZone>();
  auto zone_3_mock = std::make_unique<MockZone>();
  // Making sure the speaker is off on startup
  EXPECT_CALL(*speaker_mock, setSpeaker(false)).Times(1);
  // First loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(false));
  // Second loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(false));
  // Third loop
  EXPECT_CALL(*memory_mock, isAlarmEnabled()).WillOnce(Return(false));
  std::unique_ptr<MemoryInterface> memory_mock_pointer = std::move(memory_mock);
  std::unique_ptr<SpeakerInterface> speaker_mock_pointer = std::move(speaker_mock);
  std::vector<std::unique_ptr<SensorZoneInterface>> mocked_sensor_zone_v;
  mocked_sensor_zone_v.emplace_back(std::move(zone_1_mock));
  mocked_sensor_zone_v.emplace_back(std::move(zone_2_mock));
  mocked_sensor_zone_v.emplace_back(std::move(zone_3_mock));
  auto manager_pointer = std::make_unique<Manager>(std::move(memory_mock_pointer), std::move(speaker_mock_pointer),
                                                   std::move(mocked_sensor_zone_v));
  std::this_thread::sleep_for(std::chrono::milliseconds{350});
}
