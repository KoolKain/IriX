/* CS5 - Channel Services src/add_userinfo.c
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

void add_userinfo(char *chan, char *nick, int lvl, int aut, int prot, int suspend, char *passwd, long lastseen)
{
   struct regsinfo *regs;
   struct userinfo *new, *current;

   if(regshead == NULL) return;
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         new = (struct userinfo *) calloc(1, sizeof(struct userinfo));
         if(new != NULL)
         {
            new->next = NULL;
            strcpy(new->nick, nick);
            strcpy(new->password, passwd);
            new->autoop = aut;
            new->protect = prot;
            new->level = lvl;
            new->suspend = suspend;
            new->lastseen = lastseen;
            new->adderhead = NULL;
            new->numhead = NULL;
            if(regs->userhead == NULL) regs->userhead = new;
            else 
            {
               current = regs->userhead;
               while(current->next != NULL) current = current->next;
               current->next = new;
            }
         }
	 else log(ERRLOG_FILE, "[%s] Ajout User: plus de mémoire\n", date());
      }
      regs = regs->next;
   }
}

int add_user_addrinfo(char *chan, char *nick, char *address)
{
   struct regsinfo *regs, *regstest;
   struct userinfo *user, *usertest;
   struct adderinfo *new, *current, *addrtest;
   char raddr[100];
   int got_one = 0;

   raddr[0] = '\0';
   regstest = regshead;
   while(regstest != NULL)
   {
      if(!strcasecmp(chan, regstest->channel))
      {
         usertest = regstest->userhead;
         while(usertest != NULL)
         {
            if(!strcasecmp(nick, usertest->nick))
            {
               addrtest = usertest->adderhead;
               while(addrtest != NULL)
               {
                  if(!strcasecmp(address, addrtest->address)) return 1;
                  collapse(address);
                  if(!bancmp(address, addrtest->address) || !bancmp(addrtest->address, address))
                  {
                     if(strlen(address) < strlen(addrtest->address))
                     {
                        strcpy(raddr, addrtest->address);
                        got_one = 1;
                     }
                     else return 1;
                  }
                  addrtest = addrtest->next;
               }
            }
            usertest = usertest->next;
         }
      }
      regstest = regstest->next;
   }
   if(got_one)
      delete_addrnode(chan, nick, raddr);
   got_one = 0; 
   if(regshead == NULL) return;
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
               new = (struct adderinfo *) calloc(1, sizeof(struct adderinfo));
               if(new != NULL)
               {
                  new->next = NULL;
                  strcpy(new->address, address);
                  if(user->adderhead == NULL) user->adderhead = new;
                  else 
                  {
                     current = user->adderhead;
                     while(current->next != NULL) current = current->next;
                     current->next = new;
                  }
               }
		  else log(ERRLOG_FILE, "[%s] Ajout Addr: plus de mémoire\n", date());
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
   return 0;
}

void add_user_numinfo(char *chan, char *nick, char *num)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *new, *current;

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
               new = (struct numinfo *) calloc(1, sizeof(struct numinfo));
               if(new != NULL)
               {
                  new->next = NULL;
                  strcpy(new->numeric, num);
                  if(user->numhead == NULL) user->numhead = new;
                  else
                  {
                     current = user->numhead;
                     while(current->next != NULL) current = current->next;
                     current->next = new;
                  }
               }
		  else log(ERRLOG_FILE, "[%s] Ajout Numeric: plus de mémoire\n", date());
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
   return;
}

void add_nickinfo(char *nick, char *user, char *host, char *num, char *server, char *name)
{
   struct nickinfo *new, *current;
   new = (struct nickinfo *) calloc(1, sizeof(struct nickinfo));
   if(new != NULL) 
   {
      new->next = NULL;
      strcpy(new->nick, nick);
      strcpy(new->user, user);
      strcpy(new->host, host);
      strcpy(new->numeric, num);
      strcpy(new->server, server);
      strcpy(new->name, name);
      new->is_oper=FALSE;
      strcpy(new->away, "H");
      new->chanhead = NULL;
      new->authhead = NULL;
      if(nickhead == NULL) nickhead = new;
      else 
      {
         current = nickhead;
         while(current->next != NULL) 
         {
            current = current->next;
         }
         current->next = new;
      }
   }
      else log(ERRLOG_FILE, "[%s] Ajout new nick: plus de mémoire\n", date());
}

void add_authinfo(char *chan, char *who)
{
   struct nickinfo *nick;
   struct authinfo *new, *current;

   if(nickhead == NULL) return;
   nick = nickhead;
   while(nick != NULL)
   {
      if(!strcasecmp(who, nick->nick))
      {
         new = (struct authinfo *) calloc(1, sizeof(struct authinfo));
         if(new != NULL)
         {
            new->next = NULL;
            strcpy(new->channel, chan);
            if(nick->authhead == NULL) nick->authhead = new;
            else 
            {
               current = nick->authhead;
               while(current->next != NULL) current = current->next;
               current->next = new;
            }
         }
      }
      nick = nick->next;
   }
}

void add_joininfo(char *chan, char *num, int is_ops)
{
   struct nickinfo *nick;
   struct chaninfo *new, *current;
   char *nwho = NULL;

   nwho = num2nick(num);
   if(nickhead == NULL) return;
   nick = nickhead;
   while(nick != NULL)
   {
      if(!strcasecmp(nwho, nick->nick))
      {
         new = (struct chaninfo *) calloc(1, sizeof(struct chaninfo));
         if(new != NULL)
         {
            new->next = NULL;
            new->is_ops = is_ops;
            strcpy(new->channel, chan);
            if(nick->chanhead == NULL) nick->chanhead = new;
            else 
            {
               current = nick->chanhead;
               while(current->next != NULL) current = current->next;
               current->next = new;
            }
         }
      }
      nick = nick->next;
   }
}

void add_infoinfo(char *chan, char *nick, char *info)
{
   struct regsinfo *regs;
   struct userinfo *user;

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
               strcpy(user->info, info);
               return;
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
}
