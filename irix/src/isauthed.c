/* CS5 - Channel Services src/isauthed.c
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

void isauthed(char *who, char *rest)
{
   struct userinfo *user;
   struct authinfo *auth;
   char *nwho = NULL, *chan = NULL, *nick = NULL, *usertmp = NULL, *helpname;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nick = strtok(NULL, " ");
   helpname = get_command_name("isauthed");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   usertmp = getnickuserhost(nick);
   if(usertmp == NULL)
   {
      send_to_server(":%s NOTICE %s : Cet utilisateur n'est pas connecté\n", bot.nick, nwho);
      return;
   }
   user = ret_user_access(nick, chan);
   if(user == NULL)
   {
      send_to_server(":%s NOTICE %s : Le pseudo %s n'a pas d'accès sur %s\n", bot.nick, nwho, nick , chan);
      return;
   }
   auth = return_authinfo(chan, nick);
   if(auth == NULL)
   {
      send_to_server(":%s NOTICE %s : Le pseudo %s n'est pas autorisé sur %s\n", bot.nick, nwho, nick , chan);
      return;
   }
   send_to_server(":%s NOTICE %s : %s est autorisé sur %s avec un accès de %d\n", bot.nick, nwho, nick, chan, user->level);
   
}

void isregistered(char *who, char *rest)
{
   struct regsinfo *regs;
   char *nwho = NULL, *chan = NULL;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(regs->channel, chan))
      {
          send_to_server(":%s NOTICE %s : %s a déjà été enregistré\n", bot.nick, nwho, chan);
          return;
      }
      regs = regs->next;
   }
   send_to_server(":%s NOTICE %s : %s peut être enregistré\n", bot.nick, nwho, chan);
}  

void show_map(char *who, char *rest)
{
  struct mapinfo *map;
  char *nwho = NULL;

  nwho = nick2num(who);
  map = maphead;
  while(map != NULL)
  {
    send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, map->mapline);
    map = map->next;
  }
}
