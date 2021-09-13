/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/sensor_zone.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MockSensorReceiver : public SensorReceiverInterface {
 public:
  MOCK_METHOD(bool, getAlarmStatus, (), (const));
};

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

TEST(SensorZoneTest, CheckZoneStatusOk) {
  InSequence seq;
  auto sensor_receiver_mock_1 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_2 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_3 = std::make_unique<MockSensorReceiver>();
  EXPECT_CALL(*sensor_receiver_mock_1, getAlarmStatus()).WillOnce(Return(false));
  EXPECT_CALL(*sensor_receiver_mock_2, getAlarmStatus()).WillOnce(Return(false));
  EXPECT_CALL(*sensor_receiver_mock_3, getAlarmStatus()).WillOnce(Return(false));
  std::vector<std::unique_ptr<SensorReceiverInterface>> mocked_sensor_receiver_v;
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_1));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_2));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_3));
  std::string name = "Kitchen";
  SensorZone sensor_zone{std::move(mocked_sensor_receiver_v), name};
  EXPECT_FALSE(sensor_zone.getZoneStatus());
  EXPECT_EQ(name, sensor_zone.getName());
}

TEST(SensorZoneTest, CheckZoneStatusFirstBad) {
  InSequence seq;
  auto sensor_receiver_mock_1 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_2 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_3 = std::make_unique<MockSensorReceiver>();
  EXPECT_CALL(*sensor_receiver_mock_1, getAlarmStatus()).WillOnce(Return(true));
  EXPECT_CALL(*sensor_receiver_mock_2, getAlarmStatus()).WillOnce(Return(false));
  EXPECT_CALL(*sensor_receiver_mock_3, getAlarmStatus()).WillOnce(Return(false));
  std::vector<std::unique_ptr<SensorReceiverInterface>> mocked_sensor_receiver_v;
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_1));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_2));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_3));
  std::string name = "Kitchen";
  SensorZone sensor_zone{std::move(mocked_sensor_receiver_v), name};
  EXPECT_TRUE(sensor_zone.getZoneStatus());
}

TEST(SensorZoneTest, CheckZoneStatusSecondBad) {
  InSequence seq;
  auto sensor_receiver_mock_1 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_2 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_3 = std::make_unique<MockSensorReceiver>();
  EXPECT_CALL(*sensor_receiver_mock_1, getAlarmStatus()).WillOnce(Return(false));
  EXPECT_CALL(*sensor_receiver_mock_2, getAlarmStatus()).WillOnce(Return(true));
  EXPECT_CALL(*sensor_receiver_mock_3, getAlarmStatus()).WillOnce(Return(false));
  std::vector<std::unique_ptr<SensorReceiverInterface>> mocked_sensor_receiver_v;
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_1));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_2));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_3));
  std::string name = "Kitchen";
  SensorZone sensor_zone{std::move(mocked_sensor_receiver_v), name};
  EXPECT_TRUE(sensor_zone.getZoneStatus());
}

TEST(SensorZoneTest, CheckZoneStatusThirdBad) {
  InSequence seq;
  auto sensor_receiver_mock_1 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_2 = std::make_unique<MockSensorReceiver>();
  auto sensor_receiver_mock_3 = std::make_unique<MockSensorReceiver>();
  EXPECT_CALL(*sensor_receiver_mock_1, getAlarmStatus()).WillOnce(Return(false));
  EXPECT_CALL(*sensor_receiver_mock_2, getAlarmStatus()).WillOnce(Return(false));
  EXPECT_CALL(*sensor_receiver_mock_3, getAlarmStatus()).WillOnce(Return(true));
  std::vector<std::unique_ptr<SensorReceiverInterface>> mocked_sensor_receiver_v;
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_1));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_2));
  mocked_sensor_receiver_v.emplace_back(std::move(sensor_receiver_mock_3));
  std::string name = "Kitchen";
  SensorZone sensor_zone{std::move(mocked_sensor_receiver_v), name};
  EXPECT_TRUE(sensor_zone.getZoneStatus());
}
