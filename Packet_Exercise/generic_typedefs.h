#ifndef GENERIC_TYPEDEFS_HH
#define GENERIC_TYPEDEFS_HH

#include <cstdint>

union Integer_Union_T {
  uint32_t Integer;
  uint8_t Byte[4];
};

union Short_Union_T {
  uint16_t Short;
  uint8_t Byte[2];
};

#endif
