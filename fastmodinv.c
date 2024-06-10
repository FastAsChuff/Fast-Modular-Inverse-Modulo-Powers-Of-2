#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include </home/simon/int128fns.c>
#include </home/simon/fastmodinvpow2fns.c>

// gcc fastmodinv.c -o fastmodinv.bin -O3 -march=native -Wall -lm

uint64_t atolu(char* arg) {
  uint64_t res = 0;
  while (*arg) {
    res = (res * 10) + (*arg - '0');
    arg++;
  }
  return res;
}

uint64_t  get_cycles () {
  uint32_t lo,hi;
  asm  volatile("rdtsc":"=a"(lo),"=d"(hi));
  return  (( uint64_t)hi<<32 | lo);
}
#ifndef alignof
  #define alignof _Alignof
#endif
int warmup() {
  uint32_t i;
  for(i=0; i<100000000; i++) {
    if (!get_cycles()) {
      printf("Warm up message\n");
    }
  };
  return 0;
}


/*
uint8_t modinv8x(uint8_t n) {
  for (uint32_t i=1; i<=255; i+=2) {
    if (((i*n) & 0xff) == 1) return i;
  }
  return 0;
}

void getmodinv8lut() {
  uint32_t ix=1;
  for (uint32_t i=1; i<=8; i++) {
    for (uint32_t j=0; j<16; j++) {
      printf("%u, ", modinv8x(ix));
      ix+=2;
    }
    printf("\n");
  }  
}
// c = floor ( (1 <<64) / d ) + 1; Take L = N + 1
  uint64_t c = 0xFFFFFFFFFFFFFFFFULL / d
+ 1 + (( d & ( d - 1) ) ==0 ? 1 : 0) ;
static inline uint32_t mod190201961(uint32_t d, uint32_t n, uint64_t c) {
  // From https://arxiv.org/pdf/1902.01961
  // (NOT FAST!) fastmod computes ( n mod d ) given precomputed c
  // Requires n, d < 2^31
  uint64_t lowbits = c * n ;
  int32_t highbits = ((__int128_t)lowbits * d ) >> 64;
  // answer is equivalent to (n <0) ? highbits - 1 + d : highbits
  return highbits - ((d - 1) & (n >> 31));
}

*/

void printhelp(char* argv0) {
  printf("Benchmark test of modular inverse mod 2^64 calculations.\nUsage %s numstart\nnumstart must be odd and numstart < 2^64 - 400000\n", argv0);
  exit(0);
}

int main(int argc, char **argv) { 
  if (argc < 2) printhelp(argv[0]);
  uint64_t n = atolu(argv[1]); 
  if ((n & 1) == 0) printhelp(argv[0]);
  if (n >= 0xFFFFFFFFFFF9E580ULL) printhelp(argv[0]);
  warmup();
  uint64_t sum = 0; 
  uint64_t cyclesstart = get_cycles();
  for (uint32_t i=0; i<200000; i++) {
    sum += modinv64(n); 
    n+=2;
  }
  uint64_t cyclesend = get_cycles();
  printf("%lu\n", sum);
  printf("%lu cycles\n", cyclesend - cyclesstart);
  n = atolu(argv[1]);
  myint128_t A, N, C, S;
  C.i128 = 0;
  S.i128 = 0;
  A.i128 = n;
  N.i128 = (__int128)1 << 64;
  uint32_t res = 0;
  cyclesstart = get_cycles();
  for (uint32_t i=0; i<200000; i++) {
    res |= modinv(A, N, &C);
    S.i128 += C.i128;
    A.i128+=2;
  }
  cyclesend = get_cycles();
  assert(res == 0);
  sum = S.i128;
  printf("%lu\n", sum);
  printf("%lu cycles\n", cyclesend - cyclesstart);
}
