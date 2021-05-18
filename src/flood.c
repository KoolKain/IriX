/* CS5 - Channel Services src/flood.c
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

int check_flood(char *who)
{
   int flood_num;
   char *nwho = NULL;
   nwho = nick2num(who);
   flood_num = find_flood(who);
   if(flood_num == -1) 
   {
      add_to_flood_list(who);
      return 0;
   }
   if(flood[flood_num].bool) return 1;
   flood[flood_num].count++;
   if((time(NULL) - flood[flood_num].time) > flood_time)
      flood[flood_num].count = 0;
   else if((time(NULL) - flood[flood_num].time) <= flood_time && flood[flood_num].count >= flood_rate) 
   {
     flood[flood_num].bool = 1;
     if(!flood[flood_num].kick) 
     {
       send_to_server(":%s NOTICE %s :Arrêtez de flooder! Vous êtes ignoré pendant %d secondes.\n", bot.nick, nwho, ALARMINTERVAL);
       flood[flood_num].kick = 1;
     }
     return 1;
   }
   flood[flood_num].time = time(NULL);
   return 0;
}

int find_flood(char *who)
{
   int i;
   for(i = 0; i < flood_count; i++)
       if(!strcasecmp(flood[i].name, who)) return i;
   return -1;
}

void add_to_flood_list(char *who)
{
   if(++flood_count > 100) flood_count = 0;
   flood_count--;
   strcpy(flood[flood_count].name, who);
   flood[flood_count].time = time(NULL);
   flood[flood_count].count = 0;
   flood[flood_count].bool = 0;
   flood_count++;
}

void reset_flood()
{
   int i;
   for(i = 0; i < flood_count; i++) 
   {
      if(flood[i].bool && (time(NULL) - flood[i].time) > reset_time) 
      {
         flood[i].count = 0;
         flood[i].time = time(NULL);
         flood[i].bool = 0;
         flood[i].kick = 0;
      }
   }
}

void set_flood_rate(char *who, char *rest)
{
   char *nwho = NULL, *floodr = NULL, *floodt = NULL;
   nwho = nick2num(who);
   floodr = strtok(rest, " ");
   floodt = strtok(NULL, " ");
   if(floodr == NULL || floodt == NULL) return;
   flood_rate = atoi(floodr);
   flood_time = atoi(floodt);
   send_to_server(":%s NOTICE %s :Taux de flood: %d ligne(s) en %d seconde(s)\n", bot.nick, nwho, flood_rate, flood_time);
}

void flood_user(char *who, char *rest)
{
   char *nwho = NULL, *num = NULL, *text = NULL;
   int i;
   if(num = strchr(rest, ' ')) *num++ = '\0';
   nwho = nick2num(rest);
   if(text = strchr(num, ' ')) *text++ = '\0';
   for(i = 0; i < atoi(num); i++) 
   {
      send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, text);
      send_to_server(":%s PRIVMSG %s :%s\n", bot.nick, rest, text);
   }
}

