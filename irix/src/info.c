/* CS5 - Channel Services src/info.c
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

void info(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *num;
   
   char *uhost = NULL, *nuhost = NULL, buhost[100], *text = NULL, *nwho = NULL, text1[2];
 
  buhost[0]='\0';text1[0]='\0';
  nwho = nick2num(who);
  if(rest == NULL) return;
  if(text = strchr(rest, ' ')) *text++ = '\0';
  if(chan_check(rest, who) == -1) return;
  if(check(rest, who) == -1) return;
  if(text != NULL)
     if(strlen(text) > 150) text[149] = '\0';
  regs = regshead;
  while(regs != NULL)
  {
     if(!strcasecmp(rest, regs->channel))
     {
        user = regs->userhead;
        while(user != NULL)
        {
           num = user->numhead;
           while(num != NULL)
           {
              if(!strcmp(nwho, num->numeric))
              {
                 if(text == NULL || !strcasecmp(text, "delete"))
                 {
                    text1[0] = '\0';
                    add_infoinfo(rest, user->nick, text1);
                    send_to_server(":%s NOTICE %s :Infoline supprimée.\n", bot.nick, nwho);
                 }
                 else
                 {
                    send_to_server(":%s NOTICE %s :[%s] %s\n", bot.nick, nwho, user->nick, text);
                    add_infoinfo(rest, user->nick, text);
                 }
                 write_single_infolines(rest);
                 return;
              }
              num = num->next;
           }
           user = user->next;
        }
     }
     regs = regs->next;
  }
}

void suspend(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user, *user1, *user2, *admnwho;
   struct authinfo *admn;
   char *idnick = NULL, *what = NULL, *nwho = NULL, *reason = NULL, *helpname;

   nwho = nick2num(who);
   admn = return_authinfo(csadminchan, who);
   if(rest == NULL)
   {
     send_to_server(":%s NOTICE %s : Tapez /msg %s help suspendnick\n", bot.nick, nwho, bot.nick, bot.nick);
     return;
   }
   helpname = get_command_name("suspendnick");
   if(idnick = strchr(rest, ' ')) *idnick++ = '\0';
   if(idnick == NULL || idnick[0] == '\0')
   {
     do_help(who, helpname);
     return;
   }
   if(reason = strchr(idnick, ' ')) *reason++ = '\0';
   if(chan_check(rest, who) == -1) return;
   if(check(rest, who) == -1) return;
   nwho = nick2num(who);
   user1 = ret_user_level(idnick, rest);
   if(user1 == NULL)
   {
      send_to_server(":%s NOTICE %s :Ce pseudo %s n'existe pas dans le salon %s\n", bot.nick, nwho, idnick, rest);
      return;
   }
   user2 = ret_user_access(who, rest);
   admnwho = return_userinfo(who, csadminchan);
   admn = return_authinfo(csadminchan, who);
   if(admn == NULL)
   {
      if(user2->level <= user1->level)
      {
         send_to_server(":%s NOTICE %s :Vous ne pouvez pas alterner SUSPENDNICK pour un utilisateur ayant un level plus élevé ou égal au votre\n", bot.nick, nwho, what, idnick, rest);
         return;
      }
   }
   else
   {
      if(admnwho->level <= user1->level)
      {
         send_to_server(":%s NOTICE %s :Vous ne pouvez pas alterner SUSPENDNICK pour un utilisateur ayant un level plus élevé ou égal au votre\n", bot.nick, nwho, what, idnick, rest);
         return;
      }
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(rest, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(idnick, user->nick))
            {
                if(user->suspend)
                {
                   user->suspend = 0;
                   what = "OFF";
                }
                else
                {
                   user->suspend = 1;
                   what = "ON";
                }
                send_to_server(":%s NOTICE %s :La suspension d'accès a été mise à %s pour %s sur %s\n", bot.nick, nwho, what, idnick, rest);
                write_single_channel(rest);
                return;
             }
             user = user->next;
          }
      }
       regs = regs->next;
   }
}
