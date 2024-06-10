uint8_t modinv8(uint8_t n) {
// for odd n, returns m s.t. nm = 1 mod 2^8.
  uint8_t lut[128] = {
    1, 171, 205, 183, 57, 163, 197, 239, 241, 27, 61, 167, 41, 19, 53, 223, 
    225, 139, 173, 151, 25, 131, 165, 207, 209, 251, 29, 135, 9, 243, 21, 191, 
    193, 107, 141, 119, 249, 99, 133, 175, 177, 219, 253, 103, 233, 211, 245, 159, 
    161, 75, 109, 87, 217, 67, 101, 143, 145, 187, 221, 71, 201, 179, 213, 127, 
    129, 43, 77, 55, 185, 35, 69, 111, 113, 155, 189, 39, 169, 147, 181, 95, 
    97, 11, 45, 23, 153, 3, 37, 79, 81, 123, 157, 7, 137, 115, 149, 63, 
    65, 235, 13, 247, 121, 227, 5, 47, 49, 91, 125, 231, 105, 83, 117, 31, 
    33, 203, 237, 215, 89, 195, 229, 15, 17, 59, 93, 199, 73, 51, 85, 255
  };
  return lut[(n-1)/2];
}
uint16_t modinv16(uint16_t n) {
// for odd n, returns m s.t. nm = 1 mod 2^16.
  // Let M = 2^8, so n = AM + B
  // Result r = CM + D where BD = 1 mod 2^8
  // CB + AD + (BD >> 8) = 0 mod 2^8
  uint8_t B = n & 0xff;
  uint8_t A = n >> 8;
  uint8_t D = modinv8(B);
  uint8_t minusCB = (A*D) + (((uint16_t)B*D) >> 8);
  uint8_t C = minusCB * (0x100 - D);
  return D + ((uint16_t)C << 8);
}
uint32_t modinv32(uint32_t n) {
// for odd n, returns m s.t. nm = 1 mod 2^32.
  // Let M = 2^16, so n = AM + B
  // Result r = CM + D where BD = 1 mod 2^16
  // CB + AD + (BD >> 8) = 0 mod 2^16
  uint16_t B = n & 0xffff;
  uint16_t A = n >> 16;
  uint16_t D = modinv16(B);
  uint16_t minusCB = (A*D) + (((uint32_t)B*D) >> 16);
  uint16_t C = minusCB * (0x10000 - D);
  return D + ((uint32_t)C << 16);
}
uint64_t modinv64(uint64_t n) {
// for odd n, returns m s.t. nm = 1 mod 2^64.
  // Let M = 2^32, so n = AM + B
  // Result r = CM + D where BD = 1 mod 2^32
  // CB + AD + (BD >> 8) = 0 mod 2^32
  uint32_t B = n & 0xffffffff;
  uint32_t A = n >> 32;
  uint32_t D = modinv32(B);
  uint32_t minusCB = (A*D) + (((uint64_t)B*D) >> 32);
  uint32_t C = minusCB * (0x100000000ULL - D);
  return D + ((uint64_t)C << 32);
}

// Newton’s method per Warren ,
// Hacker’s Delight pp . 246 - -247
// From https://arxiv.org/pdf/1902.01961
uint32_t modinv32x(uint32_t d) {
  uint32_t x0 = d + 2 * ((d+1) & 4);
  uint32_t x1 = x0 * (2 - d * x0);
  uint32_t x2 = x1 * (2 - d * x1);
  return x2 * (2 - d * x2);
}

uint64_t modinv64x(uint64_t n) {
// for odd n, returns m s.t. nm = 1 mod 2^64.
  // Let M = 2^32, so n = AM + B
  // Result r = CM + D where BD = 1 mod 2^32
  // CB + AD + (BD >> 8) = 0 mod 2^32
  uint32_t B = n & 0xffffffff;
  uint32_t A = n >> 32;
  uint32_t D = modinv32x(B);
  uint32_t minusCB = (A*D) + (((uint64_t)B*D) >> 32);
  uint32_t C = minusCB * (0x100000000ULL - D);
  return D + ((uint64_t)C << 32);
}

