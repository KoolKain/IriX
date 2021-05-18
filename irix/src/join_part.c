/* CS5 - Channel Services src/join_part.c
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

void join(char *who, char *rest)
{
   struct regsinfo *regs;
   char *chan = NULL, *usernum = NULL, test[10000], *nwho = NULL;

   test[0]='\0';
   nwho = nick2num(who);
   usernum=get_usernum(bot.nick);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   strlwr(chan);
   regs = return_regsinfo(chan);
   if(regs->suspend)
   {
      send_to_server(":%s NOTICE %s : Le salon %s est suspendu\n", bot.nick, nwho, chan);
       return;
   }
   send_to_server(":%s JOIN %s\n", bot.nick, chan);
   fprintf(stderr,"join_part.c");
   add_joininfo(chan, usernum, TRUE);
   send_to_server(":%s MODE %s +o-A %s\n", myserver, chan, usernum);
   if(deopchan)
      deopall(bot.nick, chan);
   send_to_server(":%s TOPIC %s\n", bot.nick, chan);
   send_to_server(":%s MODE %s\n", bot.nick, chan);
}

void part(char *who, char *rest)
{
   char *chan = NULL, test[10000];

   test[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   strlwr(chan);
   send_to_server(":%s PART %s\n", bot.nick, chan);
   delete_cnode(chan, bot.nick);
}
