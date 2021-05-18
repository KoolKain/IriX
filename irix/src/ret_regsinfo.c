/* CS5 - Channel Services src/ret_regsinfo.c
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

int ban_ccount(char *channel)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   int i = 0;

   regs = regshead;
   while(regs != NULL)
   { 
      ban = regs->banhead;
      while(ban != NULL)
      {
         if(!ban->botban)
           i++;
         ban = ban->next;
      }
      regs = regs->next;
   }
   bccount = i;
   return(bccount);
}

int ban_icount(char *channel)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   int i = 0;

   regs = regshead;
   while(regs != NULL)
   { 
      ban = regs->banhead;
      while(ban != NULL)
      {
         if(ban->botban)
           i++;
         ban = ban->next;
      }
      regs = regs->next;
   }
   bicount = i;
   return(bicount);
}

int user_count(char *channel)
{
   struct nickinfo *nick;
   struct chaninfo *chan;
   int i = 0;

   nick = nickhead;
   while(nick != NULL)
   { 
      chan = nick->chanhead;
      while(chan != NULL)
      {
         if(!strcasecmp(channel, chan->channel))
         {
            i++;
         }
         chan = chan->next;
      }
      nick = nick->next;
   }
   ucount = i;
   return(ucount);
}
      
struct regsinfo *return_regsinfo(char *chan)
{
   struct regsinfo *current;

   current = regshead;
   while(current != NULL) 
   {
      if(!strcasecmp(chan, current->channel)) 
      {
         return current;
      }
      current = current->next;
   }
   return NULL;
}

struct mailinfo *return_mailinfo(char *uhost)
{
   struct mailinfo *mail;
   
   mail = mailhead;
   while(mail != NULL)
   {
      if(!strcasecmp(uhost, mail->ownermask))
      {
         return mail;
      }
      mail = mail->next;
   }
   return NULL;
}
      
