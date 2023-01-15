#include "Core/EnumClassFlags.h"

#include "TestUtilities/GoogleTest.h"

namespace Core
{

namespace EnumTest
{
  enum EnumTest
  {
    bit0 = 0b00000000,
    bit1 = 0b11111111,
    bit2 = 0b11110000,
    bit3 = 0b00001111,
    bit4 = 0b00111100,
    bit5 = 0b00000001,
    bit6 = 0b00000010,
    bit7 = 0b00000100,
    bit8 = 0b00001000,
  };

}
ENUM_CLASS_FLAGS(EnumTest::EnumTest)

enum class EnumClassTest : std::uint8_t
{
  bit0 = 0b00000000,
  bit1 = 0b11111111,
  bit2 = 0b11110000,
  bit3 = 0b00001111,
  bit4 = 0b00111100,
  bit5 = 0b00000001,
  bit6 = 0b00000010,
  bit7 = 0b00000100,
  bit8 = 0b00001000,
};
ENUM_CLASS_FLAGS(EnumClassTest)

TEST(EnumFlags, Operators)
{
  {
    EnumTest::EnumTest flags = EnumTest::bit5;
    flags = flags | EnumTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b00000011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit2;
    flags |= EnumTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b11111111));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit2;
    flags = flags & EnumTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b00110000));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit1;
    flags &= EnumTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b00000010));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit3;
    flags = flags ^ EnumTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b00110011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit1;
    flags ^= EnumTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b11110000));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit4;
    flags = ~flags;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b11111111111111111111111111000011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit0;
    EXPECT_FALSE(flags);
    EXPECT_TRUE(!flags);
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit2;
    EXPECT_TRUE(flags);
    EXPECT_FALSE(!flags);
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit4;
    EXPECT_TRUE(flags);
    EXPECT_FALSE(!flags);
  }
}

TEST(EnumFlags, EnumHasFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5 | EnumTest::bit6 | EnumTest::bit7;
  EXPECT_TRUE(enumHasFlags(flags, EnumTest::bit5 | EnumTest::bit6));
  EXPECT_FALSE(enumHasFlags(flags, EnumTest::bit5 | EnumTest::bit8));
  EXPECT_FALSE(enumHasFlags(flags, EnumTest::bit8));
}

TEST(EnumFlags, EnumHasAnyFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5 | EnumTest::bit6;
  EXPECT_TRUE(enumHasAnyFlags(flags, EnumTest::bit5));
  EXPECT_TRUE(enumHasAnyFlags(flags, EnumTest::bit6));
  EXPECT_TRUE(enumHasAnyFlags(flags, EnumTest::bit5 | EnumTest::bit7));
  EXPECT_FALSE(enumHasAnyFlags(flags, EnumTest::bit7 | EnumTest::bit8));
  EXPECT_FALSE(enumHasAnyFlags(flags, EnumTest::bit8));
}

TEST(EnumFlags, EnumAddFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5;
  enumAddFlags(flags, EnumTest::bit6);
  enumAddFlags(flags, EnumTest::bit7);
  EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b00000111));
}

TEST(EnumFlags, EnumRemoveFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5 | EnumTest::bit6 | EnumTest::bit7;
  enumRemoveFlags(flags, EnumTest::bit6);
  enumRemoveFlags(flags, EnumTest::bit7);
  enumRemoveFlags(flags, EnumTest::bit5);
  EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b00000000));
}

TEST(EnumClassFlags, Operators)
{
  {
    EnumClassTest flags = EnumClassTest::bit5;
    flags = flags | EnumClassTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b00000011u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit2;
    flags |= EnumClassTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b11111111u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit2;
    flags = flags & EnumClassTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b00110000u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit1;
    flags &= EnumClassTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b00000010u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit3;
    flags = flags ^ EnumClassTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b00110011u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit1;
    flags ^= EnumClassTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b11110000u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit4;
    flags = ~flags;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b11000011));
  }
  {
    EnumClassTest flags = EnumClassTest::bit0;
    EXPECT_FALSE(flags);
    EXPECT_TRUE(!flags);
  }
  {
    EnumClassTest flags = EnumClassTest::bit2;
    EXPECT_TRUE((bool)flags);
    EXPECT_FALSE(!flags);
  }
  {
    EnumClassTest flags = EnumClassTest::bit4;
    EXPECT_TRUE((bool)flags);
    EXPECT_FALSE(!flags);
  }
}

TEST(EnumClassFlags, EnumHasFlags)
{
  EnumClassTest flags = EnumClassTest::bit5 | EnumClassTest::bit6 | EnumClassTest::bit7;
  EXPECT_TRUE(enumHasFlags(flags, EnumClassTest::bit5 | EnumClassTest::bit6));
  EXPECT_FALSE(enumHasFlags(flags, EnumClassTest::bit5 | EnumClassTest::bit8));
  EXPECT_FALSE(enumHasFlags(flags, EnumClassTest::bit8));
}

TEST(EnumClassFlags, EnumHasAnyFlags)
{
  EnumClassTest flags = EnumClassTest::bit5 | EnumClassTest::bit6;
  EXPECT_TRUE(enumHasAnyFlags(flags, EnumClassTest::bit5));
  EXPECT_TRUE(enumHasAnyFlags(flags, EnumClassTest::bit6));
  EXPECT_TRUE(enumHasAnyFlags(flags, EnumClassTest::bit5 | EnumClassTest::bit7));
  EXPECT_FALSE(enumHasAnyFlags(flags, EnumClassTest::bit7 | EnumClassTest::bit8));
  EXPECT_FALSE(enumHasAnyFlags(flags, EnumClassTest::bit8));
}

TEST(EnumClassFlags, EnumAddFlags)
{
  EnumClassTest flags = EnumClassTest::bit5;
  enumAddFlags(flags, EnumClassTest::bit6);
  enumAddFlags(flags, EnumClassTest::bit7);
  EXPECT_EQ(flags, static_cast<EnumClassTest>(0b00000111));
}

TEST(EnumClassFlags, EnumRemoveFlags)
{
  EnumClassTest flags = EnumClassTest::bit5 | EnumClassTest::bit6 | EnumClassTest::bit7;
  enumRemoveFlags(flags, EnumClassTest::bit6);
  enumRemoveFlags(flags, EnumClassTest::bit7);
  enumRemoveFlags(flags, EnumClassTest::bit5);
  EXPECT_EQ(flags, static_cast<EnumClassTest>(0b00000000));
}

} // namespace Core