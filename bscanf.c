#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "bscanf.h"

#include <stdio.h>

#define _BSCANF_CHECK(x) if (!(x)) return num_args_set;
#define _BSCANF_CHECK_NULL(ptr) _BSCANF_CHECK(NULL != ptr);
#define _BSCANF_CHECK_STRING() _BSCANF_CHECK(0 != max_width);
#define _BSCANF_CHECK_BUFFER() _BSCANF_CHECK('\0' != *buffer);
#define _BSCANF_CHECK_STRTONUM() _BSCANF_CHECK(buf_ptr != end_ptr);
#define _BSCANF_MATCH() _BSCANF_CHECK(*buf_ptr == *fmt_ptr);

int bscanf(const char *buffer, const char *format, ...)
{
  va_list args;
  int num_args_set = 0;

  const char *fmt_ptr = format;
  const char *buf_ptr = buffer;

  /* Different types of pointers.
     Handy for pulling variadic arguments from va_arg. */
  long *long_ptr;
  int *int_ptr;
  short *short_ptr;
  /* TODO: Pointers for unsigned. */
  /* unsigned long *ulong_ptr;
     unsigned short *ushort_ptr;
     unsigned int *uint_ptr; */
  double *double_ptr;
  float*float_ptr;
  char *char_ptr;
  wchar_t *wchar_ptr;

  char* end_ptr;
  int is_suppressed = 0;
  size_t max_width = 0;
  char length_mod = '\0';

  if (NULL == buffer || NULL == format) {
    return -1;
  }

  va_start(args, format);

  while ('\0' != *fmt_ptr) {
    /* We ignore spaces before specifiers. */
    if (isspace(*fmt_ptr)) {
      /* Any whitespace in the format consumes all of the whitespace in the
         buffer. */
      while (isspace(*buf_ptr)) {
        buf_ptr++;
      }
      fmt_ptr++;
      continue;
    }

    if ('%' == *fmt_ptr) {
      /* Handle conversion specifier. */
      fmt_ptr++;

      /* Check for assignment-suppressing character. */
      if ('*' == *fmt_ptr) {
        is_suppressed = 1;
        fmt_ptr++;
      } else {
        is_suppressed = 0;
      }

      /* Check for maximum field width. */
      if (isdigit(*fmt_ptr)) {
        max_width = strtoul(fmt_ptr, &end_ptr, 0);
        /* Check if the sequence is a number > 0. */
        _BSCANF_CHECK(fmt_ptr != end_ptr);
        _BSCANF_CHECK(max_width > 0);

        fmt_ptr = end_ptr;
      }

      /* Check for a length modifier. */
      if ('h' == *fmt_ptr || 'l' == *fmt_ptr) {
        length_mod = *fmt_ptr;
        fmt_ptr++;
      } else {
        length_mod = '\0';
      }

      /* Handle the conversion format specifier. */
      if ('n' == *fmt_ptr) {
        /* Technically undefined, but just stop here for safety. */
        _BSCANF_CHECK(!is_suppressed);

        if ('l' == length_mod) {
          long_ptr = va_arg(args, long*);
          _BSCANF_CHECK_NULL(long_ptr);
          *long_ptr = (long) (buf_ptr - buffer);
        } else if ('h' == length_mod) {
          short_ptr = va_arg(args, short*);
          _BSCANF_CHECK_NULL(short_ptr);
          *short_ptr = (short) (buf_ptr - buffer);
        } else {
          int_ptr = va_arg(args, int*);
          _BSCANF_CHECK_NULL(int_ptr);
          *int_ptr = (int) (buf_ptr - buffer);
        }
        fmt_ptr++;
        num_args_set++;
        continue;
      }

      /* All other specifiers move the buffer pointer, so check that it's not
         NUL. */
      _BSCANF_CHECK_BUFFER();

      if ('%' == *fmt_ptr) {
        /* Match literal % */
        _BSCANF_MATCH();
        buf_ptr++;
      } else if ('c' == *fmt_ptr || 's' == *fmt_ptr) {
        /* String conversion requires a width. */
        _BSCANF_CHECK_STRING();

        if ('l' == length_mod) {
          wchar_ptr = va_arg(args, wchar_t*);
          _BSCANF_CHECK_NULL(char_ptr);
          /* TODO: Implementation. */
          _BSCANF_CHECK(0);

        } else {
          char_ptr = va_arg(args, char*);
          _BSCANF_CHECK_NULL(char_ptr);

          for (; max_width > 0; max_width--) {
            *char_ptr = *buf_ptr;
            if (*buf_ptr == '\0') {
              break;
            }
            char_ptr++;
            buf_ptr++;
          }

          /* Strings are null-terminated. */
          if ('s' == *fmt_ptr) {
            *char_ptr = '\0';
          }

          num_args_set++;
        }

      } else if ('[' == *fmt_ptr) {
        /* String conversion requires a width. */
        _BSCANF_CHECK_STRING();
        /* TODO: Implementation. */
        _BSCANF_CHECK(0);
      } else if ('d' == *fmt_ptr) {
        int_ptr = va_arg(args, int*);
        _BSCANF_CHECK_NULL(int_ptr);
        *int_ptr = (int) (strtol(buf_ptr, &end_ptr, 10));
        _BSCANF_CHECK_STRTONUM();
        buf_ptr = end_ptr;
        num_args_set++;
      } else if ('g' == *fmt_ptr) {
        if ('l' == length_mod) {
          double_ptr = va_arg(args, double*);
          _BSCANF_CHECK_NULL(double_ptr);
          *double_ptr = (double) (strtod(buf_ptr, &end_ptr));
          _BSCANF_CHECK_STRTONUM();
          buf_ptr = end_ptr;
          num_args_set++;
        } else {
          float_ptr = va_arg(args, float*);
          _BSCANF_CHECK_NULL(float_ptr);
          *float_ptr = (float) (strtod(buf_ptr, &end_ptr));
          _BSCANF_CHECK_STRTONUM();
          buf_ptr = end_ptr;
          num_args_set++;
        }
      } else {
        /* Unknown conversion specifier. */
        _BSCANF_CHECK(0);
      }
    } else {
      /* Match character with that in buffer. */
      _BSCANF_MATCH();
      buf_ptr++;
    }

    fmt_ptr++;
  }

  return num_args_set;
}
