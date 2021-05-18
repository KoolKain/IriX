/* CS5 - Channel Services src/chan_list.c
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

void chan_list(char *who, char *rest)
{
   struct regsinfo *regs;
   struct nickinfo *nick;
   struct chaninfo *chans;
   char *nwho, *value, value1[1000];
   int i = 0, val = 0;

   nwho = nick2num(who);
   value = strtok(rest, " ");
   strcpy(value1, value);
   if((value[0] == '>' || value[0] == '<' || value[0] == '=') && !isdigit(value[1]))
   {
      send_to_server(":%s NOTICE %s : La valeur doit être numérique après les < ou > ou le signe = \n", bot.nick, nwho);
       return;
   }
   *value++;
   regs = regshead;
   while(regs != NULL)
   {
         i = 0;
         nick = nickhead;
         while(nick != NULL)
         {
            chans = nick->chanhead;
            while(chans != NULL)
            {
               if(!strcasecmp(regs->channel, chans->channel) && strcasecmp(nick->nick, bot.nick))
               {
                  i++;
               }
               chans = chans->next;
            }
            nick=nick->next;
         }
         if(value1[0] == '>')
         {
            if(i > atoi(value))
               send_to_server(":%s NOTICE %s : %s : %d\n", bot.nick, nwho, regs->channel, i);
         }
         if(value1[0] == '*')
         {
               send_to_server(":%s NOTICE %s : %s : %d\n", bot.nick, nwho, regs->channel, i);
         }
         if(value1[0] == '=')
         {
            if(i == atoi(value))
               send_to_server(":%s NOTICE %s : %s : %d\n", bot.nick, nwho, regs->channel, i);
         }
         if(value1[0] == '<')
         {
            if(i < atoi(value))
               send_to_server(":%s NOTICE %s : %s : %d\n", bot.nick, nwho, regs->channel, i);
         }
         regs = regs->next;
   }
}

