/* CS5 - Channel Services src/alist.c
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

void alist(char *who, char *rest)
{
   struct userinfo *user;
   struct nickinfo *nick, *nick1;
   struct authinfo *auth, *auth1;
   char *chan = NULL, buff[10000], *nwho = NULL, temp[10000];

   buff[0] = '\0';
   temp[0] = '\0';
   nwho = nick2num(who);
   chan = strtok(rest, " ");
   nick = nickhead;
   while(nick != NULL)
   {
      auth = nick->authhead;
      while(auth != NULL)
      {
         if(!strcasecmp(chan, auth->channel))
         {
           user = return_userinfo(nick->nick, auth->channel);
	   if(user != NULL) {
	     sprintf(buff, "\0031 [\003\0033 %s \003|\0034 %d \003|\00312 %s\003\0031 ]\003  ",nick->nick, user->level, user->nick); } else {
	       sprintf(buff," - %s - ",nick->nick);
	     }
           strcat(temp, buff);
         }
         auth = auth->next;
      }
      nick = nick->next;
   }
   send_to_server(":%s NOTICE %s : [ %s ]\n", bot.nick, nwho, chan);
   if(temp[0] != '\0');
     send_to_server(":%s NOTICE %s :      %s\n", bot.nick, nwho, temp);
     
   chan = strtok(NULL, " ");
   while(chan != NULL)
   {
      buff[0] = '\0';
      temp[0] = '\0';
      nick = nickhead;
      while(nick != NULL)
      {
          auth = nick->authhead;
          while(auth != NULL)
          {
            if(!strcasecmp(chan, auth->channel))
            {
               user = return_userinfo(nick->nick, auth->channel);
               sprintf(buff, "\0031 [\003\0033 %s \003|\0034 %d \003|\00312 %s\003\0031 ]\003  ",nick->nick, user->level, user->nick);
               strcat(temp, buff);
            }
            auth = auth->next;
         }
         nick = nick->next;
      }
      send_to_server(":%s NOTICE %s : [ %s ]\n", bot.nick, nwho, chan);
      if(temp[0] != '\0');
        send_to_server(":%s NOTICE %s :      %s\n", bot.nick, nwho, buff);
      chan = strtok(NULL, " ");
   }
}
