/* CS5 - Channel Services src/chan_node_tools.c
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

int delete_bnode(char *chan, char *ban)
{
   struct regsinfo *regs;
   struct baninfo *current, *previous;

   strip_newline(ban);
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         current = regs->banhead;
         while(current != NULL)
         {
            if(!strcasecmp(ban, current->ban)) break;
            else
            {
               previous = current;
               current = current->next;
            }
         }
         if(current == NULL) return 0;
         else
         {
            if(current == regs->banhead) regs->banhead = regs->banhead->next;
            else previous->next = current->next;
            free(current);
            return 1;
         }
      }
      regs = regs->next;
   }
   return 0;
}

void delete_cnode(char *chan, char *who)
{  
   struct nickinfo *nick;
   struct chaninfo *current, *previous;
   
   nick = nickhead;
   while(nick != NULL)
   {  
      if(!strcasecmp(who, nick->nick))
      {  
         current = nick->chanhead;
         while(current != NULL)
         {
            if(!strcasecmp(chan, current->channel)) break;
            else
            {
               previous = current;
               current = current->next;
            }
         }
         if(current == NULL) return;
         else
         {
            if(current == nick->chanhead) nick->chanhead = nick->chanhead->next;
            else previous->next = current->next;
            free(current);
            return;
         }
      }
      nick = nick->next;
   }
}

void replace_mnode(char *chan, char *modes)
{
   struct regsinfo *current;

   if(regshead == NULL) return;
   if(!ison(chan)) return;
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         strcpy(current->pmodes, current->modes);
         strcpy(current->modes, modes);
      }
      current = current->next;
   }
}

void delete_channel_node(char *chan)
{
   struct regsinfo *current, *previous, *tmp;
   struct baninfo *ban, *previous2, *tmp2;
   struct userinfo *user, *previous1, *tmp1;
   struct adderinfo *addr, *previous3, *tmp3;
   struct numinfo *num, *previous4, *tmp4;

   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel)) break;
      else
      {
         previous = current;
         current = current->next;
      }
   }
   if(current == NULL) return;
   else
   {
      user = current->userhead;
      while(user != NULL)
      {
         addr = user->adderhead;
         while(addr != NULL)
         {
            if(addr == user->adderhead) user->adderhead = user->adderhead->next;
            else previous3->next = addr->next;
            tmp3 = addr->next;
            free(addr);
            addr = tmp3;
         }
         num = user->numhead;
         while(num != NULL)
         {
            if(num == user->numhead) user->numhead = user->numhead->next;
            else previous4->next = num->next;
            tmp4 = num->next;
             free(num);
             num = tmp4;
         }
         if(user == current->userhead) current->userhead = current->userhead->next;
         else previous1->next = user->next;
         tmp1 = user->next;
         free(user);
         user=tmp1;
      }
      ban = current->banhead;
      while(ban != NULL)
      {
         if(ban == current->banhead) current->banhead = current->banhead->next;
         else previous2->next = ban->next;
         tmp2 = ban->next;
         free(ban);
         ban=tmp2;
      }
      if(current == regshead) regshead = regshead->next;
      else previous->next = current->next;
      tmp=current->next;
      free(current);
      current = tmp;
   }
}

void delete_mail(char *uhost)
{
   struct mailinfo *current, *previous, *tmp;
   struct suppinfo *supp, *tmp1;

   current = mailhead;
   while(current != NULL)
   {
      if(!strcasecmp(uhost, current->ownermask))
      {
         if(current == NULL) return;
         else
         {
            supp = current->supphead;
            while(supp != NULL)
            {
               tmp1 = supp->next;
               free(supp);
               supp=tmp1;
            }
            if(current == mailhead) mailhead = mailhead->next;
            else previous->next = current->next;
            tmp=current->next;
            free(current);
            current = tmp;
         }
      }
      else
      {
         previous = current;
         current = current->next;
      }
   }
}

void delete_snode(char *server)
{
   struct nickinfo *current;
   struct templist *temp;

   current = nickhead;
   while(current != NULL)
   {
      if(!strcasecmp(server, current->server))
      {
         add_templist(current->numeric);
      }
      current = current->next;
   }
   temp = listhead;
   while(temp != NULL)
   {
       delete_node(temp->channel);
       temp = temp->next;
   }
   free_templist();
}
