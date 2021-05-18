/* CS5 - Channel Services src/check_ban.c
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

void enforce_ban(char *chan, char *nick)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   char *nnick = NULL, *uhost = NULL;

   uhost = getnickuserhost(nick);
   if(uhost == NULL) return;
   nnick = nick2num(nick);
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         ban = regs->banhead;
         while(ban != NULL)
         {
            if(!wcmp(ban->ban, uhost) && ban->botban)
            {
               send_to_server(":%s MODE %s +b %s\n", bot.nick, chan, ban->ban);
               send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnick, "Enforcing Current Bans");
               return;
            }
            ban = ban->next;
         }
      }
      regs = regs->next;
   }
   return;
}

void check_single_ban_time(char *chan)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   struct nickinfo *nicks;
   struct chaninfo *chans;
   char newban[1000];

   newban[0] = '\0';
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(regs->channel, chan))
      {
         ban = regs->banhead;
         while(ban != NULL)
         {
            if(!ban->botban && regs->banmove != -1)
            {
               if(regs->banmove == 0 || time(NULL) >= regs->banmove + ban->timeset)
               {
                  strcpy(newban, ban->ban);
                  delete_bnode(regs->channel, ban->ban);
                  add_baninfo(regs->channel, newban, time(NULL), regs->banlvl, 0, "Moved from Channel Ban Listing", bot.nick, 1);
                  write_single_banfile(regs->channel);
               }
            }
            else if(!ban->botban && regs->banremove != 0)
            {
               if(time(NULL) >= regs->banremove + ban->timeset)
               {
                  send_to_server(":%s MODE %s -b %s\n", bot.nick, regs->channel, ban->ban);
                  delete_bnode(regs->channel, ban->ban);
                  write_single_banfile(regs->channel);
               }
            }
            else if(ban->botban && time(NULL) >= ban->timeset + ban->duration && ban->duration != 0)
            {
               send_to_server(":%s MODE %s -b %s\n", bot.nick, regs->channel, ban->ban);
               load_banfiles(regs->channel);
               delete_bnode(regs->channel, ban->ban);
               write_single_banfile(regs->channel);
            }
            ban = ban->next;
         }
      }
      regs = regs->next;
   }
}

void check_ban_times()
{
   struct regsinfo *regs;
   struct baninfo *ban;
   struct nickinfo *nicks;
   struct chaninfo *chans;
   char newban[1000];

   newban[0] = '\0';
   regs = regshead;
   while(regs != NULL)
   {
      ban = regs->banhead;
      while(ban != NULL)
      {
         if(!ban->botban && regs->banmove != -1)
         {
            if(regs->banmove == 0 || time(NULL) >= regs->banmove + ban->timeset)
            {
               strcpy(newban, ban->ban);
               delete_bnode(regs->channel, ban->ban);
               add_baninfo(regs->channel, newban, time(NULL), regs->banlvl, 0, "Moved from Channel Ban Listing", bot.nick, 1);
               write_single_banfile(regs->channel);
            }
         }
         else if(!ban->botban && regs->banremove != 0)
         {
            if(time(NULL) >= regs->banremove + ban->timeset)
            {
               send_to_server(":%s MODE %s -b %s\n", bot.nick, regs->channel, ban->ban);
               delete_bnode(regs->channel, ban->ban);
               write_single_banfile(regs->channel);
            }
         }
         else if(ban->botban && time(NULL) >= ban->timeset + ban->duration && ban->duration != 0)
         {
            send_to_server(":%s MODE %s -b %s\n", bot.nick, regs->channel, ban->ban);
            load_banfiles(regs->channel);
            delete_bnode(regs->channel, ban->ban);
            write_single_banfile(regs->channel);
         }
         ban = ban->next;
      }
      regs = regs->next;
   }
}
