#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "bscanf.h"

/*#define _BSCANF_IS_SPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' \
  || (c) == '\v' || (c) == '\f' || (c) == '\r')*/

int bscanf(const char *buffer, const char *format, ...)
{
  int num_args_set = 0;
  va_list args;
  size_t buf_offset = 0;
  size_t fmt_offset = 0;
  char buf_char = buffer[buf_offset];
  char fmt_char = format[fmt_offset];

  if (NULL == buffer || NULL == format) {
    return -1;
  }

  va_start(args, format);

  while ('\0' != fmt_char) {
    fmt_char = format[fmt_offset];
    /* We ignore spaces before specifiers. */
    if (isspace(fmt_char)) {
      fmt_offset++;
      continue;
    }

    if ('%' == fmt_char) {
      /* Handle conversion specifier. */
    } else {
      /* Match character with that in buffer. */
      if (buf_char != fmt_char) {
        return num_args_set;
      }
    }

  }

  return 0;
}
