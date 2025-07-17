//
// Created by licks on 24-7-15.
//

#include "soap_helper.h"
#include "stdsoap2.h"

std::string SoapHelper::GenerateUuid()
{
    int r1, r2, r3, r4;
#ifdef WITH_OPENSSL
    r1 = soap_random;
    r2 = soap_random;
#else
  size_t i;
  static int k = 0xFACEB00C;
  int lo = k % 127773;
  int hi = k / 127773;
# if defined(HAVE_GETTIMEOFDAY)
  struct timeval tv;
  gettimeofday(&tv, NULL);
  r1 = 10000000 * tv.tv_sec + tv.tv_usec;
# elif defined(UNDER_CE)
  r1 = (int)Random();
# elif !defined(WITH_LEAN)
  r1 = (int)time(NULL);
# else
  r1 = k;
# endif
  k = 16807 * lo - 2836 * hi;
  if (k <= 0)
    k += 0x7FFFFFFF;
  r2 = k;
  /* k &= 0x8FFFFFFF; */
  char buf[SOAP_BUFLEN] = {0};
  for (i = 0; i < (sizeof(buf) < 16UL ? sizeof(buf) : 16UL); i++)
    r2 += buf[i];
#endif
    r3 = soap_random;
    r4 = soap_random;
    char tmpbuf[SOAP_TMPLEN] = {0};
    (SOAP_SNPRINTF(tmpbuf, sizeof(tmpbuf), 37), "%8.8x-%4.4hx-4%3.3hx-%4.4hx-%4.4hx%8.8x",
                                                     r1, static_cast<short>(r2 >> 16),
                                                     static_cast<short>((static_cast<short>(r2) >> 4) & 0x0FFF),
                                                     static_cast<short>((static_cast<short>(r3 >> 16) & 0x3FFF) | 0x8000), static_cast<short>(r3), r4);
    return tmpbuf;
}
