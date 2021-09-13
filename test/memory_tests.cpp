/* Copyright [2021] <Ekumen>
 * Author: Steven Desvars
 */

#include <my_project/memory.hpp>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

TEST(MemoryTest, CheckSettingMemory1) {
  std::vector<std::string> names{"Kitchen", "Garage"};
  Memory memory(names);
  memory.setZoneStatus("Kitchen", false);
  EXPECT_FALSE(memory.getZoneStatus("Kitchen"));
}

TEST(MemoryTest, CheckSettingMemory2) {
  std::vector<std::string> names{"Kitchen", "Garage"};
  Memory memory(names);
  memory.setZoneStatus("Garage", true);
  EXPECT_TRUE(memory.getZoneStatus("Garage"));
}

TEST(MemoryTest, CheckSettingMemory3) {
  std::vector<std::string> names{"Kitchen", "Garage"};
  Memory memory(names);
  EXPECT_FALSE(memory.getZoneStatus("Garage"));
}

TEST(MemoryTest, CheckSettingMemory4) {
  std::vector<std::string> names{"Kitchen", "Garage"};
  Memory memory(names);
  EXPECT_THROW(memory.getZoneStatus("Garage2"), std::invalid_argument);
}

TEST(MemoryTest, CheckSettingMemory5) {
  std::vector<std::string> names{"Kitchen", "Garage"};
  Memory memory(names);
  EXPECT_FALSE(memory.setZoneStatus("Garage2", false));
}

TEST(MemoryTest, CheckSetMemory) {
  std::vector<std::string> names{"Kitchen", "Garage"};
  Memory memory(names);
  std::string password_setter = "412";
  memory.setPassword(password_setter);
  EXPECT_EQ(password_setter, memory.getPassword());
}