/* CS5 - Channel Services src/parse_part.c
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

void parse_part(char *who, char *rest)
{
   struct regsinfo *regs;
   struct nickinfo *nick;
   struct chaninfo *chan;
   char *host = NULL, *msg = NULL, *p, *ptr[12];
   int i = 0;

   if(msg = strchr(rest, ' ')) *msg++ = '\0';
   if((p = strtok(rest, ",")) != NULL) ptr[i++] = p;
   strlwr(p);
   while((p = strtok(NULL, ",")) != NULL && i < 11) ptr[i++] = p;
   ptr[i] = NULL;
   i = 0;
   while(ptr[i])
   {
      delete_cnode(ptr[i], who);
      regs = return_regsinfo(ptr[i]);
      if(regs != NULL)
      {
         if(!regs->stay)
         {
            nick = nickhead;
            while(nick != NULL)
            {
               if(strcasecmp(nick->nick, bot.nick))
               {
                  chan = nick->chanhead;
                  while(chan != NULL)
                  {
                     if(!strcasecmp(ptr[i], chan->channel)) return;
                     chan = chan->next;
                  }
               }
               nick = nick->next;
            }
            send_to_server(":%s PART %s\n", bot.nick, ptr[i]);
         }
      }
      i++;
   }
}
