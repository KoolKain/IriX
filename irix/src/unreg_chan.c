/* CS5 - Channel Services src/unreg_chan.c
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

void unregister_channel(char *who, char *rest)
{
   char *chan, *nwho, test[10000], *nchan;

   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if(check_purge_flag(chan) == 1 || !strcasecmp(csadminchan, chan))
   {
      send_to_server(":%s NOTICE %s :Le salon %s ne peut PAS être désenregistré!\n", bot.nick, nwho, chan);
      return;
   }
   delete_channel_node(chan);
   write_regs_list();
   purge_channel(chan);
   strip_bslash(chan);
   send_to_server(":%s NOTICE %s :Le salon %s a été désenregistré!\n", bot.nick, nwho, chan);
   send_to_server(":%s WALLOPS :Le salon %s a été désenregistré par %s\n", botnick, chan, who);
   log(PURGE_LOG, "[%s] Le salon %s a été désenregistré par %s\n", date(), chan, who);
   send_to_server(":%s PART %s\n", bot.nick, chan);
   delete_cnode(chan, bot.nick);
}
