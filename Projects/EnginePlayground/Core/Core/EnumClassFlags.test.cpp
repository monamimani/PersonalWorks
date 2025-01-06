#include "Core/EnumClassFlags.h"

#include "TestUtilities/GoogleTest.h"

namespace CoreTests
{

namespace EnumTest
{
  enum EnumTest: std::uint8_t
  {
    Bit0 = 0b0000'0000,
    Bit1 = 0b1111'1111,
    Bit2 = 0b1111'0000,
    Bit3 = 0b0000'1111,
    Bit4 = 0b0011'1100,
    Bit5 = 0b0000'0001,
    Bit6 = 0b0000'0010,
    Bit7 = 0b0000'0100,
    Bit8 = 0b0000'1000,
  };

  ENUM_CLASS_FLAGS(EnumTest)
}

enum class EnumClassTest : std::uint8_t
{
  Bit0 = 0b0000'0000,
  Bit1 = 0b1111'1111,
  Bit2 = 0b1111'0000,
  Bit3 = 0b0000'1111,
  Bit4 = 0b0011'1100,
  Bit5 = 0b0000'0001,
  Bit6 = 0b0000'0010,
  Bit7 = 0b0000'0100,
  Bit8 = 0b0000'1000,
};
ENUM_CLASS_FLAGS(EnumClassTest)

TEST(EnumFlags, Operators)
{
  {
    EnumTest::EnumTest flags = EnumTest::Bit5;
    flags = flags | EnumTest::Bit6;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::Bit2;
    flags |= EnumTest::Bit3;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b1111'1111));
  }
  {
    EnumTest::EnumTest flags = EnumTest::Bit2;
    flags = flags & EnumTest::Bit4;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0011'0000));
  }
  {
    EnumTest::EnumTest flags = EnumTest::Bit1;
    flags &= EnumTest::Bit6;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0010));
  }
  {
    EnumTest::EnumTest flags = EnumTest::Bit3;
    flags = flags ^ EnumTest::Bit4;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0011'0011));
  }
  {
    EnumTest::EnumTest flags = EnumTest::Bit1;
    flags ^= EnumTest::Bit3;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b1111'0000));
  }
  {
    EnumTest::EnumTest flags = EnumTest::Bit4;
    flags = ~flags;
    EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b1111'1111'1111'1111'1111'1111'1100'0011));
  }
  {
    const EnumTest::EnumTest flags = EnumTest::Bit0;
    EXPECT_FALSE(flags);
    EXPECT_TRUE(!flags);
  }
  {
    const EnumTest::EnumTest flags = EnumTest::Bit2;
    EXPECT_TRUE(flags);
    EXPECT_FALSE(!flags);
  }
  {
    const EnumTest::EnumTest flags = EnumTest::Bit4;
    EXPECT_TRUE(flags);
    EXPECT_FALSE(!flags);
  }
}

TEST(EnumFlags, EnumHasFlags)
{
  const EnumTest::EnumTest flags = EnumTest::Bit5 | EnumTest::Bit6 | EnumTest::Bit7;
  EXPECT_TRUE(Core::enumHasFlags(flags, EnumTest::Bit5 | EnumTest::Bit6));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumTest::Bit5 | EnumTest::Bit8));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumTest::Bit8));
}

TEST(EnumFlags, EnumHasAnyFlags)
{
  const EnumTest::EnumTest flags = EnumTest::Bit5 | EnumTest::Bit6;
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumTest::Bit5));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumTest::Bit6));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumTest::Bit5 | EnumTest::Bit7));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumTest::Bit7 | EnumTest::Bit8));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumTest::Bit8));
}

TEST(EnumFlags, EnumAddFlags)
{
  EnumTest::EnumTest flags = EnumTest::Bit5;
  Core::enumAddFlags(flags, EnumTest::Bit6);
  Core::enumAddFlags(flags, EnumTest::Bit7);
  EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0111));
}

TEST(EnumFlags, EnumRemoveFlags)
{
  EnumTest::EnumTest flags = EnumTest::Bit5 | EnumTest::Bit6 | EnumTest::Bit7;
  Core::enumRemoveFlags(flags, EnumTest::Bit6);
  Core::enumRemoveFlags(flags, EnumTest::Bit7);
  Core::enumRemoveFlags(flags, EnumTest::Bit5);
  EXPECT_EQ(flags, static_cast<EnumTest::EnumTest>(0b0000'0000));
}

TEST(EnumClassFlags, Operators)
{
  {
    EnumClassTest flags = EnumClassTest::Bit5;
    flags = flags | EnumClassTest::Bit6;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0011u));
  }
  {
    EnumClassTest flags = EnumClassTest::Bit2;
    flags |= EnumClassTest::Bit3;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b1111'1111u));
  }
  {
    EnumClassTest flags = EnumClassTest::Bit2;
    flags = flags & EnumClassTest::Bit4;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0011'0000u));
  }
  {
    EnumClassTest flags = EnumClassTest::Bit1;
    flags &= EnumClassTest::Bit6;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0010u));
  }
  {
    EnumClassTest flags = EnumClassTest::Bit3;
    flags = flags ^ EnumClassTest::Bit4;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0011'0011u));
  }
  {
    EnumClassTest flags = EnumClassTest::Bit1;
    flags ^= EnumClassTest::Bit3;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b1111'0000u));
  }
  {
    EnumClassTest flags = EnumClassTest::Bit4;
    flags = ~flags;
    EXPECT_EQ(flags, static_cast<EnumClassTest>(0b1100'0011));
  }
  {
    const EnumClassTest flags = EnumClassTest::Bit0;
    EXPECT_FALSE(flags);
    EXPECT_TRUE(!flags);
  }
  {
    const EnumClassTest flags = EnumClassTest::Bit2;
    EXPECT_TRUE((bool)flags);
    EXPECT_FALSE(!flags);
  }
  {
    const EnumClassTest flags = EnumClassTest::Bit4;
    EXPECT_TRUE((bool)flags);
    EXPECT_FALSE(!flags);
  }
}

TEST(EnumClassFlags, EnumHasFlags)
{
  const EnumClassTest flags = EnumClassTest::Bit5 | EnumClassTest::Bit6 | EnumClassTest::Bit7;
  EXPECT_TRUE(Core::enumHasFlags(flags, EnumClassTest::Bit5 | EnumClassTest::Bit6));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumClassTest::Bit5 | EnumClassTest::Bit8));
  EXPECT_FALSE(Core::enumHasFlags(flags, EnumClassTest::Bit8));
}

TEST(EnumClassFlags, EnumHasAnyFlags)
{
  const EnumClassTest flags = EnumClassTest::Bit5 | EnumClassTest::Bit6;
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumClassTest::Bit5));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumClassTest::Bit6));
  EXPECT_TRUE(Core::enumHasAnyFlags(flags, EnumClassTest::Bit5 | EnumClassTest::Bit7));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumClassTest::Bit7 | EnumClassTest::Bit8));
  EXPECT_FALSE(Core::enumHasAnyFlags(flags, EnumClassTest::Bit8));
}

TEST(EnumClassFlags, EnumAddFlags)
{
  EnumClassTest flags = EnumClassTest::Bit5;
  Core::enumAddFlags(flags, EnumClassTest::Bit6);
  Core::enumAddFlags(flags, EnumClassTest::Bit7);
  EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0111));
}

TEST(EnumClassFlags, EnumRemoveFlags)
{
  EnumClassTest flags = EnumClassTest::Bit5 | EnumClassTest::Bit6 | EnumClassTest::Bit7;
  Core::enumRemoveFlags(flags, EnumClassTest::Bit6);
  Core::enumRemoveFlags(flags, EnumClassTest::Bit7);
  Core::enumRemoveFlags(flags, EnumClassTest::Bit5);
  EXPECT_EQ(flags, static_cast<EnumClassTest>(0b0000'0000));
}

} // namespace CoreTests
