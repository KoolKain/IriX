/* CS5 - Channel Services src/set_pass.c
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

void new_password(char *who, char *rest)
{
   char *chan, *newpass, *nwho, test[10000], *nuhost, *uhost, buhost[100], *cryptpass, *helpname;
   char *idnick, *anewpass;
   struct regsinfo *regs;
   struct userinfo *user, *tmp;
   struct numinfo *num;
   int gotit = 0;

   nwho = nick2num(who);
   tmp = return_userinfo(who, csadminchan);
   strcpy(test, rest);
   chan = strtok(test, " ");
   helpname = get_command_name("newpass");
   if(chan[0] != '#' && tmp != NULL)
   {
      chan = csadminchan;
      idnick = strtok(NULL, " ");
      if(idnick == NULL)
      {
         do_help(who, helpname);
         return;
      }
      newpass = strtok(NULL, " ");
      if(newpass == NULL)
      {
         do_help(who, helpname);
         return;
      }
   }
   else
   {
      if(chan_check(chan, who) == -1) return;
      if(check(chan, who) == -1) return;
      strlwr(chan);
      idnick = strtok(NULL, " ");
      if(idnick == NULL)
      {
         do_help(who, helpname);
         return;
      }
      newpass = strtok(NULL, " ");
      if(newpass == NULL)
      {
         do_help(who, helpname);
         return;
      }
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(regs->channel, chan))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            num = user->numhead;
            while(num != NULL)
            {
               if(!strcmp(nwho, num->numeric) && !strcasecmp(idnick, user->nick))
               {
                  cryptpass = mkpass(newpass, "0");
                  strcpy(user->password, cryptpass);
                  write_single_channel(chan);
                  send_to_server(":%s NOTICE %s :Mot de pass changé\n", bot.nick, nwho);
                  return;
               }
               num = num->next;
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
   send_to_server(":%s NOTICE %s :Mot de pass inchangé\n", bot.nick, nwho);
}
