/* CS5 - Channel Services src/load_nums.c
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

void load_nums()
{
  int i,res;
  char a,b;

  for (i=0;i<MAXNUM;i++)
  {
   res=i/64;
   a=convert2y[res];
   b=convert2y[i-res*64];
   usernums[i].nick[0]=(char)NULL;
   usernums[i].used=0;
   if (i <NUMNICKBASE)
   {
    servinfo[i].server[0]=(char)NULL;
    servinfo[i].used=0;
    servinfo[i].chars=convert2y[i];
   }
   sprintf(usernums[i].chars,"%c%c%c",convert2y[server_num],a,b);
  }
}
