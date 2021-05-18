/* CS5 - Channel Services src/operlist.c
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

void operlist(char *who, char *rest)
{
   struct nickinfo *nick;
   char *nwho = NULL;
   int hide = 1;

   nwho = nick2num(who);

   nick = nickhead;
   while(nick != NULL) {
     if(!strcmp(who,nick->nick)) {
       if(nick->is_oper) {
	 hide = 0;
       }
       goto find;
     }
     nick = nick->next;
   }

   find:

   nick = nickhead;
   while(nick != NULL)
   {
      if(nick->is_oper)
      {
	if(hide) {	  
         send_to_server(":%s NOTICE %s : %s\n", bot.nick, nwho, nick->nick);
	} else {
         send_to_server(":%s NOTICE %s : %s!%s@%s\n", bot.nick, nwho, nick->nick, nick->user, nick->host);
	}
      }
      nick = nick->next;
   }
}
