#ifndef __INET_NTOP_INL_
#define __INET_NTOP_INL_
/*%
 * WARNING: Don't even consider trying to compile this on a system where
 * sizeof(int) < 4. sizeof(int) > 4 is fine; all the world's not a VAX.
 */
/*
 * Copy src to string dst of size siz. At most siz-1 characters
 * will be copied. Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy(char* __restrict dst, const char* __restrict src, size_t siz)
{
  char* d = dst;
  const char* s = src;
  size_t n = siz;

  /* Copy as many bytes as will fit */
  if (n != 0) {
    while (--n != 0) {
      if ((*d++ = *s++) == '\0') {
        break;
      }
    }
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (siz != 0) {
      *d = '\0'; /* NUL-terminate dst */
    }

    while (*s++)
      ;
  }

  return(s - src - 1); /* count does not include NUL */
}
#define bzero(s, n) memset(s, 0, n)
/* const char *
 * inet_ntop4(src, dst, size)
 * format an IPv4 address
 * return:
 * `dst' (as a const)
 * notes:
 * (1) uses no statics
 * (2) takes a u_char* not an in_addr as input
 * author:
 * Paul Vixie, 1996.
 */
static char* inet_ntop4(const u_char* src, char* dst, int size)
{
  static const char fmt[] = "%u.%u.%u.%u";
  char tmp[sizeof "255.255.255.255"];
  int l;
  l = _snprintf(tmp, sizeof(tmp), fmt, src[0], src[1], src[2], src[3]);

  if (l <= 0 || (int) l >= size) {
    return (NULL);
  }

  strlcpy(dst, tmp, size);
  return (dst);
}
/* const char *
 * inet_ntop6(src, dst, size)
 * convert IPv6 binary address into presentation (printable) format
 * author:
 * Paul Vixie, 1996.
 */
static char* inet_ntop6(const u_char* src, char* dst, int size)
{
  /*
  * Note that int32_t and int16_t need only be "at least" large enough
  * to contain a value of the specified size. On some systems, like
  * Crays, there is no such thing as an integer variable with 16 bits.
  * Keep this in mind if you think this function should have been coded
  * to use pointer overlays. All the world's not a VAX.
  */
  char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"], *tp;
  struct {
    int base, len;
  } best, cur;
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ 2
  u_int words[NS_IN6ADDRSZ / NS_INT16SZ];
  int i;
  /*
  * Preprocess:
  * Copy the input (bytewise) array into a wordwise array.
  * Find the longest run of 0x00's in src[] for :: shorthanding.
  */
  memset(words, '\0', sizeof words);

  for (i = 0; i < NS_IN6ADDRSZ; i++) {
    words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
  }

  best.base = -1;
  best.len = 0;
  cur.base = -1;
  cur.len = 0;

  for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
    if (words[i] == 0) {
      if (cur.base == -1) {
        cur.base = i, cur.len = 1;
      }
      else {
        cur.len++;
      }
    }
    else {
      if (cur.base != -1) {
        if (best.base == -1 || cur.len > best.len) {
          best = cur;
        }

        cur.base = -1;
      }
    }
  }

  if (cur.base != -1) {
    if (best.base == -1 || cur.len > best.len) {
      best = cur;
    }
  }

  if (best.base != -1 && best.len < 2) {
    best.base = -1;
  }

  /*
  * Format the result.
  */
  tp = tmp;

  for (i = 0; i < (NS_IN6ADDRSZ / NS_INT16SZ); i++) {
    /* Are we inside the best run of 0x00's? */
    if (best.base != -1 && i >= best.base &&
        i < (best.base + best.len)) {
      if (i == best.base) {
        *tp++ = ':';
      }

      continue;
    }

    /* Are we following an initial run of 0x00s or any real hex? */
    if (i != 0) {
      *tp++ = ':';
    }

    /* Is this address an encapsulated IPv4? */
    if (i == 6 && best.base == 0 && (best.len == 6 ||
        (best.len == 7 && words[7] != 0x0001) ||
        (best.len == 5 && words[5] == 0xffff))) {
      if (!inet_ntop4(src + 12, tp, sizeof tmp - (tp - tmp))) {
        return (NULL);
      }

      tp += strlen(tp);
      break;
    }

    tp += sprintf(tp, "%x", words[i]);
  }

  /* Was it a trailing run of 0x00's? */
  if (best.base != -1 && (best.base + best.len) ==
      (NS_IN6ADDRSZ / NS_INT16SZ)) {
    *tp++ = ':';
  }

  *tp++ = '\0';

  /*
  * Check for overflow, copy, and we're done.
  */
  if ((int)(tp - tmp) > size) {
    return (NULL);
  }

  strcpy(dst, tmp);
  return (dst);
}
/* char *
 * inet_ntop(af, src, dst, size)
 * convert a network format address to presentation format.
 * return:
 * pointer to presentation format address (`dst'), or NULL (see errno).
 * author:
 * Paul Vixie, 1996.
 */
char* _inet_ntop(int af, const void* src, char* dst, int size)
{
  switch (af) {
  case AF_INET:
    return (inet_ntop4((const unsigned char*)src, dst, size));

  case AF_INET6:
    return (inet_ntop6((const unsigned char*)src, dst, size));

  default:
    return (NULL);
  }

  /* NOTREACHED */
}
int net_ntop(const void* src, char* dst, int size)
{
  int af = *(const short*)src;
  const struct sockaddr_in* addr = (const struct sockaddr_in*)src;
  const struct sockaddr_in6* addr6 = (const struct sockaddr_in6*)src;
  char* ret = NULL;
  int port = 0;

  switch (af) {
  case AF_INET:
    port = ntohs(addr->sin_port);
    ret = (inet_ntop4((const unsigned char*)(&addr->sin_addr), dst, size));
    break;

  case AF_INET6:
    port = ntohs(addr6->sin6_port);
    ret = (inet_ntop6((const unsigned char*)(&addr6->sin6_addr), dst, size));
    break;
  }

  return ret ? port : 0;
}
/*%
 * WARNING: Don't even consider trying to compile this on a system where
 * sizeof(int) < 4. sizeof(int) > 4 is fine; all the world's not a VAX.
 */
/* int
 * inet_pton4(src, dst)
 * like inet_aton() but without all the hexadecimal and shorthand.
 * return:
 * 1 if `src' is a valid dotted quad, else 0.
 * notice:
 * does not touch `dst' unless it's returning 1.
 * author:
 * Paul Vixie, 1996.
 */
static int inet_pton4(const char* src, u_char* dst)
{
  static const char digits[] = "0123456789";
  int saw_digit, octets, ch;
#define NS_INADDRSZ 4
  u_char tmp[NS_INADDRSZ], *tp;
  saw_digit = 0;
  octets = 0;
  *(tp = tmp) = 0;

  while ((ch = *src++) != '\0') {
    const char* pch;

    if ((pch = strchr(digits, ch)) != NULL) {
      u_int newt = *tp * 10 + (pch - digits);

      if (saw_digit && *tp == 0) {
        return (0);
      }

      if (newt > 255) {
        return (0);
      }

      *tp = newt;

      if (!saw_digit) {
        if (++octets > 4) {
          return (0);
        }

        saw_digit = 1;
      }
    }
    else if (ch == '.' && saw_digit) {
      if (octets == 4) {
        return (0);
      }

      *++tp = 0;
      saw_digit = 0;
    }
    else {
      return (0);
    }
  }

  if (octets < 4) {
    return (0);
  }

  memcpy(dst, tmp, NS_INADDRSZ);
  return (1);
}
/* int
 * inet_pton6(src, dst)
 * convert presentation level address to network order binary form.
 * return:
 * 1 if `src' is a valid [RFC1884 2.2] address, else 0.
 * notice:
 * (1) does not touch `dst' unless it's returning 1.
 * (2) :: in a full address is silently ignored.
 * credit:
 * inspired by Mark Andrews.
 * author:
 * Paul Vixie, 1996.
 */
static int inet_pton6(const char* src, u_char* dst)
{
  static const char xdigits_l[] = "0123456789abcdef",
      xdigits_u[] = "0123456789ABCDEF";
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ 2
  u_char tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
  const char* xdigits, *curtok;
  int ch, seen_xdigits;
  u_int val;
  memset((tp = tmp), '\0', NS_IN6ADDRSZ);
  endp = tp + NS_IN6ADDRSZ;
  colonp = NULL;

  /* Leading :: requires some special handling. */
  if (*src == ':')
    if (*++src != ':') {
      return (0);
    }

  curtok = src;
  seen_xdigits = 0;
  val = 0;

  while ((ch = *src++) != '\0') {
    const char* pch;

    if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL) {
      pch = strchr((xdigits = xdigits_u), ch);
    }

    if (pch != NULL) {
      val <<= 4;
      val |= (pch - xdigits);

      if (++seen_xdigits > 4) {
        return (0);
      }

      continue;
    }

    if (ch == ':') {
      curtok = src;

      if (!seen_xdigits) {
        if (colonp) {
          return (0);
        }

        colonp = tp;
        continue;
      }
      else if (*src == '\0') {
        return (0);
      }

      if (tp + NS_INT16SZ > endp) {
        return (0);
      }

      *tp++ = (u_char)(val >> 8) & 0xff;
      *tp++ = (u_char) val & 0xff;
      seen_xdigits = 0;
      val = 0;
      continue;
    }

    if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) && inet_pton4(curtok, tp) > 0) {
      tp += NS_INADDRSZ;
      seen_xdigits = 0;
      break; /*%< '\\0' was seen by inet_pton4(). */
    }

    return (0);
  }

  if (seen_xdigits) {
    if (tp + NS_INT16SZ > endp) {
      return (0);
    }

    *tp++ = (u_char)(val >> 8) & 0xff;
    *tp++ = (u_char) val & 0xff;
  }

  if (colonp != NULL) {
    /*
    * Since some memmove()'s erroneously fail to handle
    * overlapping regions, we'll do the shift by hand.
    */
    const int n = tp - colonp;
    int i;

    if (tp == endp) {
      return (0);
    }

    for (i = 1; i <= n; i++) {
      endp[- i] = colonp[n - i];
      colonp[n - i] = 0;
    }

    tp = endp;
  }

  if (tp != endp) {
    return (0);
  }

  memcpy(dst, tmp, NS_IN6ADDRSZ);
  return (1);
}
/* int
 * inet_pton(af, src, dst)
 * convert from presentation format (which usually means ASCII printable)
 * to network format (which is usually some kind of binary format).
 * return:
 * 1 if the address was valid for the specified address family
 * 0 if the address wasn't valid (`dst' is untouched in this case)
 * -1 if some other error occurred (`dst' is untouched in this case, too)
 * author:
 * Paul Vixie, 1996.
 */
int _inet_pton(int af, const char* src, void* dst)
{
  //printf("_inet_pton\n");
  switch (af) {
  case AF_INET:
    return (inet_pton4(src, (unsigned char*)dst));

  case AF_INET6:
    return (inet_pton6(src, (unsigned char*)dst));

  default:
    return (-1);
  }

  /* NOTREACHED */
}
#define inet_pton _inet_pton
#define inet_ntop _inet_ntop
#endif // __INET_NTOP_INL_

