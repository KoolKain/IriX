/* CS5 - Channel Services src/remove_hub.c
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

remove_hub(char *hub, struct hubinfo *ptr)
{
  struct hubinfo  *current,*tmp;

  current = hubhead;

  delete_snode(hub);
  serv_unused(hub);
   while(current != NULL)
   {
      if (!strcasecmp(hub, current->main))
      {
        remove_hub(current->link,current);
      }
      if((!strcasecmp(hub, current->main) || !strcasecmp(hub, current->link)) && ptr != current)
      {
         if(current == NULL) return;
         else
         {
            if(current == hubhead)
            {
              hubhead = hubhead->next;
              if(hubhead != NULL)
                hubhead->prev=NULL;
            }
            else
            {
              (current->prev)->next=current->next;
            }
            if (current->next == NULL)
              hubtail=current->prev;
            else
              (current->next)->prev=current->prev;
            tmp=current->next;
            free(current);
            current = tmp;
         }
      }
      else
      {
         current = current->next;
      }
   }
}
