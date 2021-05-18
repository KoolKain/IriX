/* CS5 - Channel Services src/free_lists.c
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

void delete_all_mail()
{
   struct mailinfo *current, *tmp;
   struct suppinfo *supp, *tmp1;

   log(ERRLOG_FILE, "[%s] Mail-liste libre\n", date());
   current = mailhead;
   while(current != NULL)
   {
      supp = current->supphead;
      while(supp != NULL)
      {
         if(supp == current->supphead) current->supphead = current->supphead->next;
         tmp1 = supp->next;
         free(supp);
         supp=tmp1;
      }
      if(current == mailhead) mailhead = mailhead->next;
      tmp = current->next;
      free(current);
      current=tmp;
   }
}

void free_maplist()
{
   struct mapinfo *current, *tmp;
   log(ERRLOG_FILE, "[%s] Mapliste libre\n", date());
   current = maphead;
   while(current != NULL)
   {
      if(current == maphead) maphead = maphead->next;
      tmp = current->next;
      free(current);
      current=tmp;
   }
}

void free_burst()
{
   struct burstinfo *current, *tmp;
   log(ERRLOG_FILE, "[%s] Burst libre\n", date());
   current = bursthead;
   while(current != NULL)
   {
      if(current == bursthead) bursthead = bursthead->next;
      tmp = current->next;
      free(current);
      current=tmp;
   }
}

void free_hublist()
{
   struct hubinfo *current, *tmp;
   log(ERRLOG_FILE, "[%s] Hublist libre\n", date());
   current = hubhead;
   while(current != NULL)
   {
      if(current == hubhead) hubhead = hubhead->next;
      tmp = current->next;
      free(current);
      current=tmp;
   }
}

void free_templist()
{
   struct templist *current, *tmp;
   log(ERRLOG_FILE, "[%s] Templist libre\n", date());
   current = listhead;
   while(current != NULL)
   {
      if(current == listhead) listhead = listhead->next;
      tmp = current->next;
      free(current);
      current=tmp;
   }
}

void free_regslist()
{
   struct regsinfo *regs, *tmp;
   struct userinfo *user, *tmp1;
   struct baninfo *ban, *tmp2;

   log(ERRLOG_FILE, "[%s] Regslist libre\n", date());
   regs = regshead;
   while(regs != NULL)
   {
      ban = regs->banhead;
      while(ban != NULL)
      {
         if(ban == regs->banhead) regs->banhead = regs->banhead->next;
         tmp2 = ban->next;
         free(ban);
         ban=tmp2;
      }
      user = regs->userhead;
      while(user != NULL)
      {
         if(user == regs->userhead) regs->userhead = regs->userhead->next;
         tmp1 = user->next;
         free(user);
         user=tmp1;
      }
      if(regs == regshead) regshead = regshead->next;
      tmp = regs->next;
      free(regs);
      regs=tmp;
   }
}

void free_nicklist()
{
   struct nickinfo *nick, *tmp;
   struct chaninfo *chan, *tmp1;
   log(ERRLOG_FILE, "[%s] Nicklist libre\n", date());
   nick = nickhead;
   chan = nick->chanhead;
   while(nick != NULL)
   {
      while(chan != NULL)
      {
         if(chan == nick->chanhead) nick->chanhead = nick->chanhead->next;
         tmp1 = chan->next;
         free(chan);
         chan=tmp1;
      }
      if(nick == nickhead) nickhead = nickhead->next;
      tmp = nick->next;
      free(nick);
      nick=tmp;
   }
}
