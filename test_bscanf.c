#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static void test_date()
{
  unsigned short day;
  char *month = malloc(sizeof(char) * 10);
  unsigned long year;
  float time;
  char timezone[3];
  /* "One small step for man..." */
  const char* buffer = "July 20, 3661 10.9333 PM EDT";
  char* out_buf = malloc(sizeof(char) * (strlen(buffer) + 1));
  const char* format = "%9s%hu,%lo%f %*2c %3c";
  int ret_val;
  int i;

  ret_val = bscanf(buffer, format, month, &day, &year, &time, timezone);
  assert(5 == ret_val);
  sprintf(out_buf, "%s %hu, %lo %g PM %.3s", month, day, year, time, timezone);

  for (i = 0; i < strlen(buffer) + 1; i++) {
    assert(buffer[i] == out_buf[i]);
  }

  free(out_buf);
  free(month);
}

int main(void)
{
  test_basic();
  test_decimal();
  test_double();
  test_date();
  printf("%s\n", "All tests passed.");
  return 0;
}
