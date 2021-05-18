/* CS5 - Channel Services src/join_msgchan.c
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

T BURST #southpole 913851119 +stn TEV:o,TEN
TAA CREATE #test 914856643

 */           


#include <bot.h>

void join_msgchan()
{
   struct burstinfo *burst;
   char *usernum = NULL;
   int gotit = 0;

   if(*msg_chan != '#') return;
   usernum=get_usernum(bot.nick);
   burst = bursthead;
   while(burst != NULL)
   {
      if(!strcasecmp(msg_chan, burst->channel))
      {
         send_to_server("%c BURST %s %ld %s:o\n", convert2y[server_num], msg_chan, burst->chanstamp, usernum);
         send_to_server(":%s MODE %s +Cmints\n", bot.nick, msg_chan);
         gotit = 1;
      }
      burst = burst->next;
   }
   if(!gotit)
   {
      send_to_server("%s CREATE %s %ld\n", usernum, msg_chan, time(NULL));
      send_to_server(":%s MODE %s +Cmints\n", bot.nick, msg_chan);
   }
}
