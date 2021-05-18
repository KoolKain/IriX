/* CS5 - Channel Services src/parse_nick.c
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

void parse_nick(char *who, char *rest)
{
   struct nickinfo *current;
   int i = 0, is_ops=0, got_i = 0, got_w = 0, got_both = 0;
   char *waste = NULL, *user = NULL, *host = NULL, *ptr = NULL, *jtime = NULL, *reason = NULL, *what = NULL, *num = NULL, *server = NULL, srv, *nwho = NULL;

   if(who[0] == '\0')
   {
      if(waste = strchr(rest, ' ')) *waste++ = '\0';
      if(jtime = strchr(waste, ' ')) *jtime++ = '\0';
      if(user = strchr(jtime, ' ')) *user++ = '\0';
      if(host = strchr(user, ' ')) *host++ = '\0';
      if(ptr = strchr(host, ' ')) *ptr++ = '\0';
      if(ptr[0] == '+')
      {
         if(what = strchr(ptr, ' ')) *what++ = '\0';
         if(num = strchr(what, ' ')) *num++ = '\0';
         if(reason = strchr(num, ' ')) *reason++ = '\0';
         reason++;
         server=num2serv(*num);
         if(*user == '~') user++;
         add_nickinfo(rest, user, host, num, server, reason);
         if (strchr(ptr,'+'))
         {
            current = nickhead;
            while(current != NULL)
            {
              if(!strcasecmp(rest,current->nick))
              {
                 if(strchr(ptr, 'o'))
                    current->is_oper=TRUE;
                 else 
                    current->is_oper=FALSE;
                 if(strchr(ptr, 'i'))
                    got_i = 1;
                 if(strchr(ptr, 'w'))
                    got_w = 1;
                 if(got_i && got_w)
                 {
                   strcpy(current->modes, "iw");
                   break;
                 }
                 if(got_i)
                   strcpy(current->modes, "i");
                 if(got_w)
                   strcpy(current->modes, "w");
              }
              current = current->next;
            }
         }
      }
      else
      {
         if(num = strchr(ptr, ' ')) *num++ = '\0';
         if(reason = strchr(num, ' ')) *reason++ = '\0';
         reason++;
         server=num2serv(*num);
         if(*user == '~') user++;
         add_nickinfo(rest, user, host, num, server, reason);
      }
   }
   else
   {
      if(ptr = strchr(rest, ' ')) *ptr = '\0';
      nwho=num2nick(who);
      replace_node(who, rest);
   }
}
