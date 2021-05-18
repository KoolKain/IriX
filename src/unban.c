/* CS5 - Channel Services src/unban.c
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

void unban(char *who, char *rest)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   struct userinfo *user, *admn;
   struct authinfo *admnauth;
   char *p, *nwho, *mask[100], *chan, *uhost, test[10000], *helpname;
   int l = 0;

   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   admn = return_userinfo(who, csadminchan);
   admnauth = return_authinfo(csadminchan, who);
   user = return_userinfo(who, chan);
   uhost = getnickuserhost(who);
   helpname = get_command_name("unban");
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         if((mask[l] = strtok(NULL, " ")) != NULL) l++;
         else
         {
            do_help(who, helpname);
            return;
         }
         while((p = strtok(NULL, " ")) != NULL) mask[l++] = p;
         mask[l++] = NULL;
         for(l = 0; mask[l]; l++)
         {
            ban = regs->banhead;
            while(ban != NULL)
            {
               if(!strcasecmp(mask[l], ban->ban))
               {
/* NiX
                  if(!bancmp(uhost, mask[l]) || !bancmp(mask[l], uhost))
                  {
                     send_to_server(":%s NOTICE %s : Nice try , but i don't think so!\n", bot.nick, nwho);
                     send_to_server(":%s NOTICE %s : You are banned from this channel\n", bot.nick, nwho);
                  }
                  else if(admn != NULL && admnauth != NULL && admn->level >= ban->level)
*/
                  if(admn != NULL && admnauth != NULL && (admn->level >= ban->level || admn->level == maxadmnlevel))
                  {
                     send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, ban->ban);
                     if(delete_bnode(chan, ban->ban))
                        send_to_server(":%s NOTICE %s : Ban pour %s retiré\n", bot.nick, nwho, mask[l]);
                     write_single_banfile(chan);
                  }
	       /* STE */
                  else if(user != NULL && user->level >= ban->level)
                  {
                     send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, ban->ban);
                     if(delete_bnode(chan, ban->ban))
                        send_to_server(":%s NOTICE %s : Ban pour %s retiré\n", bot.nick, nwho, mask[l]);
                     write_single_banfile(chan);
                  }
	       /* STE */
                  else if(admn != NULL && (admn->level >= ban->level || admn->level == maxadmnlevel))
                  {
                     send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, ban->ban);
                     if(delete_bnode(chan, ban->ban))
                        send_to_server(":%s NOTICE %s : Ban pour %s retiré\n", bot.nick, nwho, mask[l]);
                     write_single_banfile(chan);
                  }
                  else
		     send_to_server(":%s NOTICE %s : Votre accès n'est pas assez élevé pour supprimer le ban %s\n", bot.nick, nwho, ban->ban);
               }
               ban = ban->next;
            }
         }
      }
      regs = regs->next;
   }
}
