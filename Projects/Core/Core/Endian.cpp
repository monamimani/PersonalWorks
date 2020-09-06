#include "Core/Endian.h"

#include <cstdint>

namespace Core
{
//Endian detectEndianness() noexcept
//{
//  union {
//    uint32_t m_value;
//    uint8_t m_data[sizeof(uint32_t)];
//  } number = {};
//
//  number.m_data[0] = 0x00;
//  number.m_data[1] = 0x01;
//  number.m_data[2] = 0x02;
//  number.m_data[3] = 0x03;
//
//  switch (number.m_value)
//  {
//    case UINT32_C(0x00010203):
//      return Endian::Big;
//    case UINT32_C(0x03020100):
//      return Endian::Little;
//    case UINT32_C(0x02030001):
//      return Endian::BigWord;
//    case UINT32_C(0x01000302):
//      return Endian::LittleWord;
//    default:
//      return Endian::Unknown;
//  }
//
//}
//
//Endian endian = detectEndianness();

}