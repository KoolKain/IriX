/* CS5 - Channel Services src/join_chan.c
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

void join_channels()
{
   struct modeinfo *mode;
   struct regsinfo *current;
   struct nickinfo *nicktmp;
   struct chaninfo *chantmp;
   char *usernum = NULL;
   int first_time = 0;
   long stamp;

   joined = FALSE;
   usernum=get_usernum(bot.nick);
   current = regshead;
   while(current != NULL) 
   {
      if(!current->suspend && strcasecmp(current->channel, msg_chan))
      {
        if(current->chanstamp != NULL && current->chanstamp[0] != '\0')
        {
           stamp = atol(current->chanstamp);
           send_to_server("%c BURST %s %ld %s:o\n", convert2y[server_num], current->channel, stamp, usernum);
           add_joininfo(current->channel, usernum, TRUE);
        }
        else
        {
           send_to_server("%s CREATE %s %ld\n", usernum, current->channel, time(NULL));
           add_joininfo(current->channel, usernum, TRUE);
        }
        default_modes(current->channel);
        default_topic(current->channel);
        send_to_server(":%s MODE %s -A\n", bot.nick, current->channel);
        if(deopchan)
           deopall(bot.nick, current->channel);
        load_infolines(current->channel);
        load_banfiles(current->channel);
     }
     current = current->next;
   }
   free_maplist();
   end_map = 0;
   send_to_server(":%s MAP\n", bot.nick);
   load_commands();
   free_burst();
}
