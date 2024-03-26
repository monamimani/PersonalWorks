#include "Core/EnumClassFlags.h"

#include "TestUtilities/GoogleTest.h"

namespace CoreTests
{

namespace EnumTest
{
  enum EnumTest
  {
    bit0 = 0b0000'0000,
    bit1 = 0b1111'1111,
    bit2 = 0b1111'0000,
    bit3 = 0b0000'1111,
    bit4 = 0b0011'1100,
    bit5 = 0b0000'0001,
    bit6 = 0b0000'0010,
    bit7 = 0b0000'0100,
    bit8 = 0b0000'1000,
  };

  ENUM_CLASS_FLAGS(EnumTest)
}

enum class EnumClassTest : std::uint8_t
{
  bit0 = 0b0000'0000,
  bit1 = 0b1111'1111,
  bit2 = 0b1111'0000,
  bit3 = 0b0000'1111,
  bit4 = 0b0011'1100,
  bit5 = 0b0000'0001,
  bit6 = 0b0000'0010,
  bit7 = 0b0000'0100,
  bit8 = 0b0000'1000,
};
ENUM_CLASS_FLAGS(EnumClassTest)

TEST(EnumFlags, Operators)
{
  {
    EnumTest::EnumTest flags = EnumTest::bit5;
    flags = flags | EnumTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit2;
    flags |= EnumTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b1111'1111));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit2;
    flags = flags & EnumTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0011'0000));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit1;
    flags &= EnumTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0010));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit3;
    flags = flags ^ EnumTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0011'0011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit1;
    flags ^= EnumTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b1111'0000));
  }
  {
    EnumTest::EnumTest flags = EnumTest::bit4;
    flags = ~flags;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b1111'1111'1111'1111'1111'1111'1100'0011));
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
  EXPECT_TRUE(Core::enumHasFlags(flags, EnumTest::bit5 | EnumTest::bit6));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumTest::bit5 | EnumTest::bit8));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumTest::bit8));
}

TEST(EnumFlags, EnumHasAnyFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5 | EnumTest::bit6;
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumTest::bit5));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumTest::bit6));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumTest::bit5 | EnumTest::bit7));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumTest::bit7 | EnumTest::bit8));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumTest::bit8));
}

TEST(EnumFlags, EnumAddFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5;
  Core::enumAddFlags(flags, EnumTest::bit6);
  Core::enumAddFlags(flags, EnumTest::bit7);
  EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0111));
}

TEST(EnumFlags, EnumRemoveFlags)
{
  EnumTest::EnumTest flags = EnumTest::bit5 | EnumTest::bit6 | EnumTest::bit7;
  Core::enumRemoveFlags(flags, EnumTest::bit6);
  Core::enumRemoveFlags(flags, EnumTest::bit7);
  Core::enumRemoveFlags(flags, EnumTest::bit5);
  EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0000));
}

TEST(EnumClassFlags, Operators)
{
  {
    EnumClassTest flags = EnumClassTest::bit5;
    flags = flags | EnumClassTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0011u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit2;
    flags |= EnumClassTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b1111'1111u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit2;
    flags = flags & EnumClassTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0011'0000u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit1;
    flags &= EnumClassTest::bit6;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0010u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit3;
    flags = flags ^ EnumClassTest::bit4;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0011'0011u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit1;
    flags ^= EnumClassTest::bit3;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b1111'0000u));
  }
  {
    EnumClassTest flags = EnumClassTest::bit4;
    flags = ~flags;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b1100'0011));
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
  EXPECT_TRUE(Core::enumHasFlags(flags, EnumClassTest::bit5 | EnumClassTest::bit6));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumClassTest::bit5 | EnumClassTest::bit8));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumClassTest::bit8));
}

TEST(EnumClassFlags, EnumHasAnyFlags)
{
  EnumClassTest flags = EnumClassTest::bit5 | EnumClassTest::bit6;
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumClassTest::bit5));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumClassTest::bit6));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumClassTest::bit5 | EnumClassTest::bit7));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumClassTest::bit7 | EnumClassTest::bit8));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumClassTest::bit8));
}

TEST(EnumClassFlags, EnumAddFlags)
{
  EnumClassTest flags = EnumClassTest::bit5;
  Core::enumAddFlags(flags, EnumClassTest::bit6);
  Core::enumAddFlags(flags, EnumClassTest::bit7);
  EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0111));
}

TEST(EnumClassFlags, EnumRemoveFlags)
{
  EnumClassTest flags = EnumClassTest::bit5 | EnumClassTest::bit6 | EnumClassTest::bit7;
  Core::enumRemoveFlags(flags, EnumClassTest::bit6);
  Core::enumRemoveFlags(flags, EnumClassTest::bit7);
  Core::enumRemoveFlags(flags, EnumClassTest::bit5);
  EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0000));
}

} // namespace CoreTests
