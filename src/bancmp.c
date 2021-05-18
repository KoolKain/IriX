/* CS5 - Channel Services src/bancmp.c
 * Copyright (c) 1998 Chris Birch <cs5@othernet.org>
 * Original code and design by Michael Dabrowski <devon@othernet.org> CS4.0
 * All further GNU development and publication rights of this software are
 * granted to Chris Birch by Michael Dabrowski.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */           


#include<bot.h>

int bancmp(const char *mask, const char *name)
{
  const char *m = mask;
  const char *n = name;
  const char *ma = mask;
  const char *na = name;
  int wild = 0;
  int q = 0;

  while (1)
  {
    if (*m == '*')
    {
      while (*m == '*')
	m++;
      wild = 1;
      ma = m;
      na = n;
    }

    if (!*m)
    {
      if (!*n)
        return 0;
      for (m--; (m > mask) && (*m == '?'); m--)
	;
      if ((*m == '*') && (m > mask) &&
	  (m[-1] != '\\'))
        return 0;
      if (!wild)
	return 1;
      m = ma;
      n = ++ na;
    }
    else if (!*n)
    {
      while (*m == '*')
	m++;
      return (*m != 0);
    }
    if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
    {
      m++;
      q = 1;
    }
    else
      q = 0;

    if ((tolower(*m) != tolower(*n)) && ((*m != '?') || q))
    {
      if (!wild)
	return 1;
      m = ma;
      n = ++na;
    }
    else
    {
      if (*m)
	m++;
      if (*n)
	n++;
    }
  }
}

char *collapse(char *pattern)
{
  Reg1 char *s = pattern, *s1, *t;

  if (BadPtr(pattern))
    return pattern;
  for (; *s; s++)
    if (*s == '\\')
      if (!*(s + 1))
	break;
      else
	s++;
    else if (*s == '*')
    {
      if (*(t = s1 = s + 1) == '*')
	while (*t == '*')
	  t++;
      else if (*t == '?')
	for (t++, s1++; *t == '*' || *t == '?'; t++)
	  if (*t == '?')
	    *s1++ = *t;
      while ((*s1++ = *t++));
    }
  return pattern;
}
