#ifndef _BSCANF_H
#define _BSCANF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int bscanf(const char *buffer, const char *format, ...)
  __attribute__ ((format (scanf, 2, 3)));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BSCANF_H */
