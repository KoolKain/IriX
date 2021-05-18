/* CS5 - Channel Services src/user_node_tools.c
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

int delete_addrnode(char *chan, char *nick, char *addr)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct adderinfo *current, *previous;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(nick, user->nick))
            {
               current = user->adderhead;
               if(current->next == NULL) return 0;
               while(current != NULL)
               {
                  if(!strcasecmp(addr, current->address)) break;
                  else
                  {
                     previous = current;
                     current = current->next;
                  }
               }
               if(current == NULL) return 1;
               else
               {
                  if(current == user->adderhead) user->adderhead = user->adderhead->next;
                  else previous->next = current->next;
                  free(current);
                  return 1;
               }
                
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
   return 1;
}

int delete_numnode(char *chan, char *nick, char *num)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *current, *previous;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(nick, user->nick))
            {
               current = user->numhead;
               while(current != NULL)
               {
                  if(!strcmp(num, current->numeric)) break;
                  else
                  {
                     previous = current;
                     current = current->next;
                  }
               }
               if(current == NULL) return 1;
               else
               {
                  if(current == user->numhead) user->numhead = user->numhead->next;
                  else previous->next = current->next;
                  free(current);
                  return 1;
               }

            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
   return 1;
}

void delete_unode(char *chan, char *nick)
{
   struct regsinfo *regs;
   struct userinfo *current, *previous;
   struct adderinfo *current1, *previous1, *tmp;
   struct numinfo *current2, *previous2, *tmp2;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         current = regs->userhead;
         while(current != NULL)
         {
            if(!strcasecmp(nick, current->nick)) break;
            else
            {
               previous = current;
               current = current->next;
            }
         }
         if(current == NULL) return;
         else
         {
            current1 = current->adderhead;
            while(current1 != NULL)
            {
               if(current1 == current->adderhead) current->adderhead = current->adderhead->next;
               else previous1->next = current1->next;
               tmp=current1->next;
               free(current1);
               current1 = tmp;               
            }
            current2 = current->numhead;
            while(current2 != NULL)
            {
               if(current2 == current->numhead) current->numhead = current->numhead->next;
               else previous2->next = current2->next;
               tmp2=current2->next;
               free(current2);
               current2 = tmp2;               
            }
            if(current == regs->userhead) regs->userhead = regs->userhead->next;
            else previous->next = current->next;
            free(current);
         }
      }
      regs = regs->next;
   }
}

void delete_auth(char *chan, char *who)
{
   struct nickinfo *nick;
   struct authinfo *current, *previous;

   nick = nickhead;
   while(nick != NULL)
   {
      if(!strcasecmp(who, nick->nick))
      {
         current = nick->authhead;
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
            if(current == nick->authhead) nick->authhead = nick->authhead->next;
            else previous->next = current->next;
            free(current);
            return;
         }
      }
      nick = nick->next;
   }
}

void purge_auth(char *chan)
{
   struct nickinfo *nick;
   struct authinfo *current, *previous, *tmp;

   nick = nickhead;
   while(nick != NULL)
   {
      current = nick->authhead;
      while(current != NULL)
      {
         if(!strcasecmp(chan, current->channel))
         {
            if(current == NULL) return;
            else
            {
               if(current == nick->authhead) nick->authhead = nick->authhead->next;
               else previous->next = current->next;
               tmp = current->next;
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
   nick = nick->next;
   }
}

void replace_onode(char *chan, char *numeric, int ops)
{
   struct nickinfo *nick;
   struct chaninfo *current;
   if(nickhead == NULL) return;
   nick = nickhead;
   while(nick != NULL)
   {
      if(!strcmp(numeric, nick->numeric))
      {
         current = nick->chanhead;
         while(current != NULL)
         {
            if(!strcasecmp(chan, current->channel))
            {
               current->is_ops = ops;
               return;
            }
            current = current->next;
         }
      }
      nick = nick->next;
   }
}

void replace_node(char *numeric, char *newnick)
{
   struct nickinfo *current;
   if(nickhead == NULL) return;
   current = nickhead;
   while(current != NULL)
   {
      if(!strcmp(numeric, current->numeric))
      {
         strcpy(current->nick, newnick);
      }
      current = current->next;
   }
}

void delete_node(char *numeric)
{
   struct nickinfo *current, *previous, *tmp;
   struct chaninfo *chan, *previous1, *tmp1;
   struct authinfo *auth, *previous2, *tmp2;
   char *uhost, *who;

   who = num2nick(numeric);
   uhost = getuserhost(who);
   if(uhost == NULL) return;
   
   current = nickhead;
   while(current != NULL)
   {
      if(!strcmp(numeric, current->numeric)) break;
      else
      {
         previous = current;
         current = current->next;
      }
   }
   if(current == NULL) return;
   else
   {
      chan = current->chanhead;
      while(chan != NULL)
      {
         if(chan == current->chanhead) current->chanhead = current->chanhead->next;
         else previous1->next = chan->next;
         tmp1 = chan->next;
         free(chan);
         chan=tmp1;
      }            
      auth = current->authhead;
      while(auth != NULL)
      {
         delete_auth_chan(auth->channel, numeric);
         auth = auth->next;
      }
      auth = current->authhead;
      while(auth != NULL)
      {
         if(auth == current->authhead) current->authhead = current->authhead->next;
         else previous2->next = auth->next;
         tmp2 = auth->next;
         free(auth);
         auth=tmp2;
      }            
      if(current == nickhead) nickhead = nickhead->next;
      else previous->next = current->next;
      free(current);
      return;
   }
}

void delete_auth_chan(char *chan, char *numeric)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *current, *previous;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(regs->channel, chan))
      {
          user = regs->userhead;
          while(user != NULL)
          {
             current = user->numhead;
             while(current != NULL)
             {
                if(!strcmp(current->numeric, numeric))
                {
                   if(current == user->numhead) user->numhead = user->numhead->next;
                   else previous->next = current->next;
                   free(current);
                   return;
                }
                else
                {
                   previous = current;
                   current = current->next;
                }
             }
             user = user->next;
          }
      }
      regs = regs->next;
   }
}
