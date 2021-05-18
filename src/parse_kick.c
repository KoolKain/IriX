/* CS5 - Channel Services src/parse_kick.c
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

void parse_kick(char *who, char *rest)
{
   char *chan = NULL, *nick = NULL, *reason = NULL, *nnick = NULL, *nwho = NULL, *ban_per, *uhost;
   struct userinfo *tmp, *tmp1, *tmp3;
   struct regsinfo *tmp2;
   struct authinfo *admnauth, *userauth;

   chan = strtok(rest, " ");
   nick = strtok(NULL, " ");
   reason = strtok(NULL, " ");
   nnick=num2nick(nick);
   nwho=nick2num(who);
   delete_cnode(chan, nnick);
   if(nwho == NULL) return;
   tmp2 = return_regsinfo(chan);
   if(tmp2 == NULL) return;
   if(tmp2->suspend) return;
   tmp3 = return_userinfo(who, chan);
   tmp1 = return_userinfo(nnick, csadminchan);
   admnauth = return_authinfo(csadminchan, nnick);
   if(tmp1 != NULL &&  admnauth != NULL)
   {
      if(tmp1->protect)
      {
          if(tmp3 != NULL && tmp3->level == maxuserlevel) return;
          uhost = getuserhost(who);
          ban_per = get_ban_mask(uhost);
          send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, nwho);
      }
   }
   else 
   {
      tmp = return_userinfo(nnick, chan); 
      userauth = return_authinfo(chan, nnick);
      if(tmp != NULL &&  userauth != NULL)
      {
         if(tmp->protect)
         {
             if(tmp3 != NULL && tmp3->level == maxuserlevel) return;
             uhost = getuserhost(who);
             ban_per = get_ban_mask(uhost);
             send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, nwho);
         }
      }
   }
}

/* NiX
   if(tmp1 != NULL &&  admnauth != NULL)
   {
      if(tmp1->protect)
      {
          if(tmp3 != NULL && tmp3->level == maxuserlevel) return;
          uhost = getuserhost(who);
          ban_per = get_ban_mask(uhost);
          send_to_server(":%s MODE %s -o+b %s %s\n", bot.nick, chan, nwho, ban_per);
          send_to_server(":%s KICK %s %s :%s is protected!\n", bot.nick, chan, nwho, nnick);
          add_baninfo(chan, ban_per, time(NULL), tmp2->banlvl, tmp2->bantimeout, "Kicked a Protected user", nnick, 1);
      }
   }
   else 
   {
      tmp = return_userinfo(nnick, chan); 
      userauth = return_authinfo(chan, nnick);
      if(tmp != NULL &&  userauth != NULL)
      {
         if(tmp->protect)
         {
             if(tmp3 != NULL && tmp3->level == maxuserlevel) return;
             uhost = getuserhost(who);
             ban_per = get_ban_mask(uhost);
             send_to_server(":%s MODE %s -o+b %s %s\n", bot.nick, chan, nwho, ban_per);
             send_to_server(":%s KICK %s %s :%s est protégé!\n", bot.nick, chan, nwho, nnick);
             add_baninfo(chan, ban_per, time(NULL), tmp2->banlvl, tmp2->bantimeout, "Kicked a Protected user", nnick, 1);
         }
      }
   }
}
*/