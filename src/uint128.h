#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef __SIZEOF_INT128__
// do some fancy stuff here
typedef __uint128_t uint128_t;
typedef __uint128_t union_state;
#else
typedef intmax_t uint128_t;
typedef intmax_t union_state;
// do some fallback stuff here
#endif
/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x) #x
#define TO_STRING(x) STRINGIZER(x)

int print_uint128(uint128_t ui128);
int print_uint128_no_line(uint128_t ui128);