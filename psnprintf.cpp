/*
  QLib

  Portable snprintf wrapper

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "z_zone.h"

// On Windows, vsnprintf() is _vsnprintf().
#ifdef _WIN32
#if _MSC_VER < 1400 /* not needed for Visual Studio 2008 */
#define vsnprintf _vsnprintf
#endif
#endif

// Safe, portable vsnprintf().
int pvsnprintf(char *buf, size_t buf_len, const char *s, va_list args)
{
   int result;

   if (buf_len < 1)
   {
      return 0;
   }

   // Windows (and other OSes?) has a vsnprintf() that doesn't always
   // append a trailing \0. So we must do it, and write into a buffer
   // that is one byte shorter; otherwise this function is unsafe.
   result = vsnprintf(buf, buf_len, s, args);

   // If truncated, change the final char in the buffer to a \0.
   // A negative result indicates a truncated buffer on Windows.
   if (result < 0 || (size_t)result >= buf_len)
   {
      buf[buf_len - 1] = '\0';
      result = buf_len - 1;
   }

   return result;
}

// Safe, portable snprintf().
int psnprintf(char *buf, size_t buf_len, const char *s, ...)
{
   va_list args;
   int result;
   va_start(args, s);
   result = pvsnprintf(buf, buf_len, s, args);
   va_end(args);
   return result;
}

size_t pstrnlen(const char *s, size_t count)
{
   const char *p = s;
   while (*p && count-- > 0)
      p++;

   return p - s;
}

// EOF

