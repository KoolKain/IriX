/* CS5 - Channel Services src/parse_privmsg.c
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

void parse_privmsg(char *who, char *rest)
{
   struct userinfo *admntmp;
   struct authinfo *admnauth;
   int i, enablelevel;
   char *nwho = NULL, *chan = NULL, *msg = NULL, *cmd = NULL, nmsg[10000], nnmsg[10000], test[10000], *usernum = NULL;
   char ncmd[1000], *rmsg, *cmd1, *helpname;
   nwho = nick2num(who);
   if(check_flood(who)) return;
   trailing_blanks(rest);
   if(cmd = strchr(rest, ':')) *cmd++ = '\0';
   else return;
   if(msg = strchr(cmd, ' ')) *msg++ = '\0';
   else msg = NULL;
   
#ifdef DEBUG
printf("Parse Privmsg - Command: [%s] Who: [%s] Msg: [%s]\n", cmd, who, msg);
#endif
   if(strlen(cmd) > 25) return;
   for(i = 0; *commands[i].name; i++)
   {
      if(!strcasecmp(commands[i].name, cmd))
      {
         enablelevel = get_command_level("enablecom");
         admntmp = return_userinfo(who, csadminchan);
         admnauth = return_authinfo(csadminchan, who);
         if(admntmp == NULL && commands[i].disable)
         {
	    send_to_server(":%s NOTICE %s : La commande est hors service\n", bot.nick, nwho);
             return;
         }
         else if(admnauth == NULL && commands[i].disable)
         {
	    send_to_server(":%s NOTICE %s : La commande est hors service\n", bot.nick, nwho);
             return;
         }
         else if(admntmp != NULL && admnauth != NULL && admntmp->level < enablelevel && commands[i].disable)
         {
	    send_to_server(":%s NOTICE %s : La commande est hors service\n", bot.nick, nwho);
             return;
         }
         if(msg == NULL)
         {
            if(commands[i].needarg)
            {
               strlwr(cmd);
               if(commands[i].chanopt)
               {
                  helpname = get_command_name("chanopt");
                  cmd1 = get_command_rname(cmd);
                  sprintf(ncmd, "%s %s", helpname, cmd1);
                  strlwr(ncmd);
                  do_help(who, ncmd);
                  return;
               }
               strlwr(cmd);
               do_help(who, cmd);
               return;
            }
            if(commands[i].level > 0)
              if(!check_access(who, msg, commands[i].level, cmd)) return;
            command_log(who, cmd, msg);
            commands[i].function(who, msg);
            return;
         }
         else
         {
            strcpy(nmsg, msg);
            if(commands[i].level > 0)
              if(!check_access(who, msg, commands[i].level, cmd)) return;
            if(commands[i].chanopt)
            {
               if(rmsg = strchr(nmsg, ' ')) *rmsg++ = '\0';
               if(chan_check(nmsg, who) == -1) return;
               if(check(nmsg, who) == -1) return;
               cmd1 = get_command_rname(cmd);
               if(rmsg == NULL)
                  sprintf(nnmsg, "%s %s", nmsg, cmd1);
               else
                  sprintf(nnmsg, "%s %s %s", nmsg, cmd1, rmsg);
               channel_options(who, nnmsg);
               helpname = get_command_name("chanopt");
               command_log(who, helpname, nnmsg);
               return;
            }
            commands[i].function(who, nmsg);
            command_log(who, cmd, nmsg);
            return;
         }
      }
   }
   send_to_server(":%s NOTICE %s :La commande [ %s ] n'est pas une commande appropriée.\n", bot.nick, nwho, cmd);
   send_to_server(":%s NOTICE %s :Veuillez vérifier que vous avez tapé correctement.\n", bot.nick, nwho);
}
