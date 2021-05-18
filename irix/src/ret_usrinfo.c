/* CS5 - Channel Services src/ret_usrinfo.c
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

struct userinfo *return_maskinfo(char *who, char *chan)
{
   struct regsinfo *current;
   struct userinfo *current1;
   struct adderinfo *current2;
   int bool = 0;
   char *uhost, cuhost[100], buhost[100], *nuhost, *duhost, *host;

   uhost = getnickuserhost(who);
   if(uhost == NULL) return NULL;
   strcpy(buhost, uhost);
   nuhost = swap_host(buhost);
   if(nuhost == NULL)
   {
      nuhost = uhost;
   }
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan,current->channel))
      {
         current1 = current->userhead;
         while(current1 != NULL)
         {
               current2 = current1->adderhead;
               while(current2 != NULL)
               {
                  if(!wcmp(current2->address, uhost) || !wcmp(current2->address, nuhost))
                  {
                     if(current1->suspend == TRUE || current->suspend == TRUE) return NULL;
                     return current1;
                  }
                  current2 = current2->next;
            }
            current1 = current1->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct userinfo *return_userinfo(char *who, char *chan)
{
   struct regsinfo *current;
   struct userinfo *current1;
   struct adderinfo *current2;
   struct numinfo *current3;
   int bool = 0;
   char *uhost, cuhost[100], buhost[100], *nuhost, *duhost, *host, *nwho;
   if(!strchr(who, '@'))
   {
      nwho = nick2num(who);
      if(nwho == NULL) return NULL;
   }
   else 
   {
      uhost = who;
      bool = 1;
   }
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan,current->channel))
      {
         current1 = current->userhead;
         while(current1 != NULL) 
         {
            if(!bool) 
            {
                  current3 = current1->numhead;
                  while(current3 != NULL)
                  {
                     if(!strcmp(current3->numeric, nwho))
                     {
                        return current1;
                     }
                     current3 = current3->next;
                  }
            }
            else 
            {
               current2 = current1->adderhead;
               while(current2 != NULL)
               {
                  collapse(uhost);
                  if(!bancmp(uhost, current2->address) || !bancmp(current2->address, uhost))
                  {
                     return current1;
                  }
                  current2 = current2->next;
               }
            }
            current1 = current1->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct userinfo *ret_user_access(char *who, char *chan)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *num;
   char *nwho, *uhost, *nuhost, buhost[100];

   nwho = nick2num(who);
   if(nwho == NULL) return;
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            num = user->numhead;
            while(num != NULL)
            {
               if(!strcmp(num->numeric, nwho))
               {
                  return user;
               }
               num = num->next;
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
   return NULL;
}

struct adderinfo *ret_user_addrinfo(char *who, char *chan)
{
   struct regsinfo *current;
   struct userinfo *current1;
   struct adderinfo *current2;

   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan,current->channel))
      {
         current1 = current->userhead;
         while(current1 != NULL) 
         {
            current2 = current1->adderhead;
            while(current2 != NULL)
            {
               collapse(who);
               if(!bancmp(current2->address, who) || !bancmp(who, current2->address))
               {
                  if(current1->suspend == TRUE || current->suspend == TRUE) return NULL;
                  return current2;
               }
               current2 = current2->next;
            }
            current1 = current1->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct userinfo *ret_user_nickinfo(char *who, char *chan)
{
   struct regsinfo *current;
   struct userinfo *current1;
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan,current->channel))
      {
         current1 = current->userhead;
         while(current1 != NULL) 
         {
            if(!strcasecmp(who, current1->nick)) 
            {
               return current1;
            }
            current1 = current1->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct userinfo *ret_user_level(char *who, char *chan)
{
   struct regsinfo *current;
   struct userinfo *current1;
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan,current->channel))
      {
         current1 = current->userhead;
         while(current1 != NULL) 
         {
            if(!strcasecmp(who, current1->nick)) 
            {
               return current1;
            }
            current1 = current1->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct authinfo *return_authinfo(char *chan, char *nick)
{
   struct nickinfo *current;
   struct authinfo *auth;

   current = nickhead;
   while(current != NULL)
   {
      if(!strcasecmp(nick,current->nick))
      {
         auth = current->authhead;
         while(auth != NULL)
         {
            if(!strcasecmp(chan, auth->channel))
            {
               return auth;
            }
            auth = auth->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct nickinfo *return_chaninfo(char *nick, char *channel)
{
   struct nickinfo *current;
   struct chaninfo *chan;
   current = nickhead;
   while(current != NULL)
   {
      if(!strcasecmp(nick,current->nick))
      {
         chan = current->chanhead;
         while(chan != NULL)
         {
            if(!strcasecmp(channel, chan->channel))
            {
               return current;
            }
            chan = chan->next;
         }
      }
      current = current->next;
   }
   return NULL;
}

struct nickinfo *return_nickinfo(char *nick)
{
   struct nickinfo *current;
   current = nickhead;
   while(current != NULL)
   {
      if(!strcasecmp(nick,current->nick))
      {
         return current;
      }
      current = current->next;
   }
   return NULL;
}
