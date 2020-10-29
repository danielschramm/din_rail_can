/*********************************************************************
*
*       Prototypes (to be placed in a header file such as SWO.h)
*/
#ifndef __SWO_H__
#define __SWO_H__

#include <stdio.h>          // sprintf

#ifdef __cplusplus
  extern "C" {
#endif

#define logger(str) do { char buf[256];sprintf(buf, "%d\t%s\t%s\n", __LINE__, __FILE__, str);SWO_PrintString(buf);} while(0)

void SWO_PrintChar  (char c);
void SWO_PrintString(const char *s);

#ifdef __cplusplus
  }
#endif

#endif /* __SWO_H__ */
