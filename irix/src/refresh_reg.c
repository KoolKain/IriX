/* CS5 - Channel Services src/refresh_reg.c
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

void refresh_registered(char *chan, char *who)
{
   struct regsinfo *current;
   char *uhost;
   int found = 0;
   if((uhost = getuserhost(who)) == NULL) return;
   current = regshead;
   while(current != NULL) 
   {
      if(!strcasecmp(chan, current->channel))
      {
         current->lastseen = time(NULL);
         found = 1;
         break;
      }
      current = current->next;
   }
   if(!found) return;
}
