/* CS5 - Channel Services src/dauthorize.c
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


#include <bot.h>

void dauthorize(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *num;
   char *nwho = NULL, *nick = NULL, *chan = NULL, *helpname;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nick = strtok(NULL, " ");
   helpname = get_command_name("deauth");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(nick, user->nick))
            {
               num = user->numhead;
               while(num != NULL)
               {
                  if(!strcmp(nwho, num->numeric))
                  {
                     delete_auth(chan, who);
                     delete_numnode(chan, nick, nwho);
                     send_to_server(":%s NOTICE %s :L'autorisation a été enlevée\n", botnick, nwho);
                     return;
                  }
                  num = num->next;
               }
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
}
