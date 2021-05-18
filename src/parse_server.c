/* CS5 - Channel Services src/parse_server.c
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

void parse_server(char *who, char *rest)
{
   struct hubinfo *new;
   char *link = NULL, *server = NULL;

   if(rest == NULL) return;
   link = strtok(rest, " ");
   server=num2serv(*who);
   if(joined)
   {
      free_maplist();
      end_map = 0;
      send_to_server(":%s MAP\n", bot.nick);
   }
   if(!strcasecmp(uworldhost, link)) return;
   if(!strcasecmp(uworldhost, server)) return;
   new = (struct hubinfo *) calloc(1, sizeof(struct hubinfo));
   if(new != NULL)
   {
      new->next = NULL;
      strcpy(new->main, server);
      strcpy(new->link, link);
      if(hubhead == NULL)
      {
        hubhead = new;
        hubtail = new;
        hubhead->prev=NULL;
      }
      else
      {
         new->prev=hubtail;
         hubtail->next = new;
         hubtail=new;
      }
   }
   else log(ERRLOG_FILE, "[%s] Ajout d'un nouveau Hub/Lien: plus de mémoire\n", date());
}

