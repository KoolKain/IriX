/* CS5 - Channel Services src/get_dns.c
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

void get_ip(char *who, char *addr)
{
   char **buf, *nwho = NULL, *host = NULL, *nhost = NULL, *uhost = NULL;
   struct in_addr *ptr;
   struct hostent *hp;

   host = strtok(addr, " ");
   uhost = getuserhost(host);
   if(uhost != NULL)
   {
      if(nhost = strchr(uhost, '@')) *nhost++ = '\0';
      host = nhost;
   }
   nwho=nick2num(who);
   if(is_ip(host))
   {
      send_to_server(":%s NOTICE %s : Mauvaise commande, essayez \026getdns\026\n", bot.nick, nwho);
      return;
   }
   hp = gethostbyname(host);
   if(hp == NULL)
   {
      send_to_server(":%s NOTICE %s : Impossibilité de résoudre le DNS\n", bot.nick, nwho);
      return;
   }
   buf = hp->h_addr_list;
   ptr = (struct in_addr *) *buf++;
   sprintf(userhost, "%s", inet_ntoa(*ptr));
   send_to_server(":%s NOTICE %s : %s\n", bot.nick, nwho, userhost);
}

void get_dns(char *who, char *addr1)
{
   long addr;
   char *nwho = NULL, *host = NULL, *nhost = NULL, *uhost = NULL;

   host = strtok(addr1, " ");
   uhost = getuserhost(host);
   if(uhost != NULL)
   {
      if(nhost = strchr(uhost, '@')) *nhost++ = '\0';
      host = nhost;
   }
   nwho=nick2num(who);
   if(!is_ip(host))
   {
      send_to_server(":%s NOTICE %s : Mauvaise commande, essayez \026getip\026\n", bot.nick, nwho);
      return;
   }
   addr=inet_addr(host);
   sprintf(userhost, "%s", hostnamefromip(addr));
   if(is_ip(userhost))
   {
      send_to_server(":%s NOTICE %s : Impossibilité de résoudre le DNS\n", bot.nick, nwho);
      return;
   }
   send_to_server(":%s NOTICE %s : %s\n", bot.nick, nwho, userhost);
}
