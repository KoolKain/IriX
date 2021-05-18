/* CS5 - Channel Services src/chan_check.c
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

int chan_check(char *chan, char *who)
{
   char *nwho = NULL;
   int i, chanlen;
   nwho=nick2num(who);
   if(chan == NULL)
   {
      send_to_server(":%s NOTICE %s : Il y a trop d'espace avant le nom du salon \n", bot.nick, nwho);
       return -1;
   }

   if(strlen(chan) > 201)
   {
      send_to_server(":%s NOTICE %s : Le nom du salon est trop grand \n", bot.nick, nwho);
       return -1;
   }

   if(chan[0] == '\0')
   {
      send_to_server(":%s NOTICE %s : Il y a trop d'espace avant le nom du salon \n", bot.nick, nwho);
       return -1;
   }

   if(strstr(chan, " "))
   {
      send_to_server(":%s NOTICE %s : Il y a trop d'espace avant le nom du salon \n", bot.nick, nwho);
       return -1;
   }

   if(strchr(chan, ',') != NULL)
   {
      send_to_server(":%s NOTICE %s :[%s] ne fait pas référence à un salon\n", bot.nick, nwho, chan);
      return -1;
   }

   if(chan[0] != '#' && chan[0] != '+') 
   {
      send_to_server(":%s NOTICE %s :[%s] ne fait pas référence à un salon\n", bot.nick, nwho, chan);
          return -1;
   }
}

int check(char *chan, char *who)
{
   struct regsinfo *tmp;
   char *nwho;

   nwho = nick2num(who);
   if(chan[0] != '#') 
   {
      send_to_server(":%s NOTICE %s :[%s] ne fait pas référence à un salon\n", bot.nick, nwho, chan);
          return -1;
   }
   else 
   {
      tmp = return_regsinfo(chan);
      if(tmp == NULL)
      {
	 send_to_server(":%s NOTICE %s :[%s] ne fait pas référence à un salon\n", bot.nick, nwho, chan);
         return -1;
      }
   }
}

int check_chan(char *chan)
{
   struct regsinfo *current;
   current = regshead;
   while(current != NULL) 
   {
      if(!strcasecmp(current->channel, chan)) return 1;
      current = current->next;
   }
   return 0;
}
