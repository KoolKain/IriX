/* CS5 - Channel Services src/motd.c
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

void show_welcome(char *who, char *chan)
{
   struct regsinfo *tmp;
   char *nwho = NULL, *ptr = NULL;

   tmp = return_regsinfo(chan);
   if(tmp == NULL) return;
   if(!tmp->setwelc) return;
   if(*tmp->welcome == '\0' || *tmp->welcome == ' ') return;
   nwho = nick2num(who);
   send_to_server(":%s NOTICE %s :Bienvenue - %s\n", bot.nick, nwho, tmp->welcome);
}
 
void do_voice(char *who, char *chan)
{
   struct regsinfo *tmp;
   struct userinfo *usertmp;
   char *nwho = NULL;

   tmp = return_regsinfo(chan);
   if(tmp == NULL) return;
   if(!tmp->setvoice) return;
   usertmp = return_userinfo(who, chan);
   if(usertmp == NULL) return;
   nwho = nick2num(who);
   send_to_server(":%s MODE %s +v %s\n", bot.nick, chan, nwho);
}

void show_motd(char *who, char *chan)
{
   struct regsinfo *tmp;
   char *nwho = NULL, *ptr = NULL;

   tmp = return_regsinfo(chan);
   if(tmp == NULL) return;
   if(*tmp->motd == '\0' || *tmp->motd == ' ' || tmp->motd == NULL) return;
   nwho = nick2num(who);
   send_to_server(":%s NOTICE %s :MOTD for %s - %s\n", bot.nick, nwho, chan, tmp->motd);
}
