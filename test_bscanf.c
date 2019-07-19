#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bscanf.h"

static void test_basic()
{
  int i, j, k;
  int ret_val;
  char arr[8];
  char *str = malloc(sizeof(char) * 8);
  const char* buffer = " abcd1234,efgh5678";
  const char* format = "%n%n %8c %n,%8s";

  ret_val = bscanf(buffer, format, &i, &j, arr, &k, str);

  assert(5 == ret_val);
  assert(0 == i);
  assert(0 == j);
  for (i = 0; i < 8; i++) {
    assert(arr[i] == buffer[i+1]);
    assert(str[i] == buffer[i+10]);
  }
  assert(9 == k);
  free(str);
}

static void test_decimal()
{
  int i, j, k;
  int ret_val;

  ret_val = bscanf("[4096:2048:-128]", "[%d:%d:%d]", &i, &j, &k);
  assert(ret_val == 3);
  assert(i == 4096);
  assert(j == 2048);
  assert(k == -128);


  ret_val = bscanf("64@(256,32)", "%d@(%d,%d)", &i, &j, &k);
  assert(ret_val == 3);
  assert(i == 64);
  assert(j == 256);
  assert(k == 32);

  ret_val = bscanf("[1024]", "[%d]", &i);
  assert(ret_val == 1);
  assert(i == 1024);

  ret_val = bscanf("-512", "%d", &i);
  assert(ret_val == 1);
  assert(i == -512);
}

static void test_double()
{
  double d;
  int ret_val;

  ret_val = bscanf("9.1093837015e-31", "%lg", &d);
  assert(ret_val == 1);
  assert(d == 9.1093837015e-31);
}

int main(void)
{
  test_basic();
  test_decimal();
  test_double();
  return 0;
}
