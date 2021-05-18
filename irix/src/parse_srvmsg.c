/* CS5 - Channel Services src/parse_srvmsg.c
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

void parse_server_msg(fd_set *read_fds)
{
   if(FD_ISSET(socketfd, read_fds))
   {
      if(readln() > 0) 
      {
         parse(line);
      }
      else 
      {
        close(socketfd);
      }
   }
}
void parse_ping(char *who, char *rest)
{
   send_to_server("PONG %s\n", who);
   bot.server_active = TRUE;
}


void parse_pong(char *who, char *rest)
{
   char *ptr = NULL;
   if(ptr = strchr(rest, ' ')) *ptr = '\0';
   if(!strcasecmp(who, rest)) bot.server_active = TRUE;
}

void parse_squit(char *who, char *rest)
{
   char *server = NULL;

   free_maplist();
   end_map = 0;
   send_to_server(":%s MAP\n", bot.nick);
   server = strtok(rest, " ");
   if(!strcasecmp(server, uworldhost)) return;
   remove_hub(server,NULL);
}

void parse_pass(char *who, char *rest)
{
   char *usernum = NULL;

   usernum=get_free_usernum(bot.nick);
   send_to_server("%c NICK %s 1 %lu %s %s DQx8MG %s :%s\n", convert2y[server_num], bot.nick, time(NULL), ircuser, myhost, usernum, ircname);
   send_to_server("%c END_OF_BURST\n", convert2y[server_num]);
   add_nickinfo(bot.nick, ircuser, myserver, usernum, myserver, reason);
   send_to_server(":%s MODE %s :%s\n", bot.nick, bot.nick, mymodes);
   bot.server_active = TRUE;  
   alarm(ALARMINTERVAL);
}

