/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/sensor_receiver.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MockRemoteSensor : public RemoteSensorInterface {
 public:
  MOCK_METHOD(void, ping, (), (const));
  MOCK_METHOD(void, setPongCallback, (std::function<void()> _callback));
};

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

std::function<void()> callback_;

void FakeSetPongCallback(std::function<void()> callback) { callback_ = callback; }

TEST(SensorReceiverTest, CheckIfPongCallbackIsBeingSet) {
  auto sensor_mock = std::make_unique<MockRemoteSensor>();
  EXPECT_CALL(*sensor_mock, setPongCallback(_)).Times(1);
  std::unique_ptr<RemoteSensorInterface> mock_pointer = std::move(sensor_mock);
  SensorReceiver sensor_receiver{mock_pointer};
}

TEST(SensorReceiverTest, CheckIfPingIsBeingCalledRepeatedly) {
  InSequence seq;
  auto sensor_mock = std::make_unique<MockRemoteSensor>();
  EXPECT_CALL(*sensor_mock, setPongCallback(_)).Times(1);
  EXPECT_CALL(*sensor_mock, ping()).Times(AtLeast(8));
  std::unique_ptr<RemoteSensorInterface> mock_pointer = std::move(sensor_mock);
  SensorReceiver sensor_receiver{mock_pointer};
  std::this_thread::sleep_for(std::chrono::milliseconds{850});
}

TEST(SensorReceiverTest, CheckPongCallback) {
  InSequence seq;
  auto sensor_mock = std::make_unique<MockRemoteSensor>();
  EXPECT_CALL(*sensor_mock, setPongCallback(_)).WillOnce(Invoke(FakeSetPongCallback));

  EXPECT_CALL(*sensor_mock, ping()).Times(8);
  std::unique_ptr<RemoteSensorInterface> mock_pointer = std::move(sensor_mock);
  SensorReceiver sensor_receiver{mock_pointer};
  for (int i = 0; i < 8; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    callback_();
    EXPECT_FALSE(sensor_receiver.getAlarmStatus());
  }
  std::this_thread::sleep_for(std::chrono::milliseconds{50});
}

TEST(SensorReceiverTest, CheckPongCallbackFalse) {
  InSequence seq;
  auto sensor_mock = std::make_unique<MockRemoteSensor>();
  EXPECT_CALL(*sensor_mock, setPongCallback(_)).WillOnce(Invoke(FakeSetPongCallback));

  EXPECT_CALL(*sensor_mock, ping()).Times(9);
  std::unique_ptr<RemoteSensorInterface> mock_pointer = std::move(sensor_mock);
  SensorReceiver sensor_receiver{mock_pointer};
  std::this_thread::sleep_for(std::chrono::milliseconds{50});
  for (int i = 0; i < 8; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    callback_();
    EXPECT_FALSE(sensor_receiver.getAlarmStatus());
  }
  std::this_thread::sleep_for(std::chrono::milliseconds{70});
}

TEST(SensorReceiverTest, CheckPongCallbackDynamically) {
  InSequence seq;
  auto sensor_mock = std::make_unique<MockRemoteSensor>();
  EXPECT_CALL(*sensor_mock, setPongCallback(_)).WillOnce(Invoke(FakeSetPongCallback));
  EXPECT_CALL(*sensor_mock, ping()).Times(24);
  std::unique_ptr<RemoteSensorInterface> mock_pointer = std::move(sensor_mock);
  SensorReceiver sensor_receiver{mock_pointer};
  std::this_thread::sleep_for(std::chrono::milliseconds{50});
  for (int i = 0; i < 8; i++) {
    callback_();
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    EXPECT_FALSE(sensor_receiver.getAlarmStatus());
  }
  //   std::this_thread::sleep_for(std::chrono::milliseconds{50});
  for (int i = 0; i < 8; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    EXPECT_TRUE(sensor_receiver.getAlarmStatus());
  }
  //   std::this_thread::sleep_for(std::chrono::milliseconds{50});
  for (int i = 0; i < 8; i++) {
    callback_();
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    EXPECT_FALSE(sensor_receiver.getAlarmStatus());
  }
}
