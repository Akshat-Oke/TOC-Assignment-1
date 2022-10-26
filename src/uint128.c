#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "uint128.h"

static int print_u128_u(uint128_t u128, short newline)
{
  int rc;
  if (u128 > UINT64_MAX)
  {
    uint128_t leading = u128 / P10_UINT64;
    uint64_t trailing = u128 % P10_UINT64;
    rc = print_u128_u(leading, 0);
    rc += printf("%." TO_STRING(E10_UINT64) PRIu64, trailing);
  }
  else
  {
    uint64_t u64 = u128;
    rc = printf("%" PRIu64, u64);
  }
  if (newline)
    printf("\n");
  return rc;
}
int print_uint128(uint128_t ui128)
{
  return print_u128_u(ui128, 1);
}
int print_uint128_no_line(uint128_t ui128)
{
  return print_u128_u(ui128, 0);
}