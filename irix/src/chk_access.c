/* CS5 - Channel Services src/chk_access.c
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

int check_access(char *who, char *rest, int level, char *cmd)
{
   struct userinfo *user, *usertmp, *admn, *admntmp;
   struct authinfo *authtmp, *admnauth;
   struct adderinfo *addr;
   struct numinfo *num;
   struct regsinfo *regs, *regstmp;
   char *ln = NULL, restbuf[1000], *nuhost = NULL, *uhost = NULL, buff[1000], *chan = NULL, *password = NULL;
   char *nwho = NULL, *nick = NULL, *pass = NULL, tmppass[13], keys[3], *cryptpass = NULL, *test = NULL, buhost[100];

   restbuf[0]='\0';buff[0]='\0';tmppass[0]='\0';keys[0]='\0';buhost[0]='\0';
   if(!strcasecmp(get_command_rname(cmd), "CSAUTH") || !strcasecmp(get_command_rname(cmd), "AUTH") || !strcasecmp(get_command_rname(cmd), "AUTHOP"))
   {
      return 1;
   }
   added_level = 0;
   if(!strcasecmp(who, uworldnick) || !strcasecmp(who, uworldhost)) return 1;
   nwho = nick2num(who);
   if(rest != NULL)
   {
      if(strlen(rest) > 549) return;
      strip_slash(rest);
      strcpy(restbuf, rest);
      strcpy(buff, rest);
   }
   else rest = "0";
   admntmp = return_userinfo(who, csadminchan);
   admnauth = return_authinfo(csadminchan, who);
   if(admntmp != NULL && admnauth != NULL && strcasecmp(get_command_rname(cmd), "AUTH") && strcasecmp(get_command_rname(cmd), "AUTHOP"))
   {
      if(admntmp->suspend)
      {
	 send_to_server(":%s NOTICE %s :Votre accès Admin a été suspendu\n", bot.nick, nwho, bot.nick);
          return 0;
      }      
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(csadminchan, regs->channel))
         {
            user = regs->userhead;
            while(user != NULL)
            {
               num = user->numhead;
               while(num != NULL)
               {
                  if(!strcmp(nwho, num->numeric))
                  {
                     added_level = user->level;
                     if(user->level < level)
                     {
                        send_denied_msg(who);
                        return 0;
                     }
                     user->lastseen = time(NULL);
                     return 1;
                  }
                  num = num->next;
               }
               user = user->next;
            }
         }  
         regs = regs->next;
      }  
      if(admntmp->level < level) 
      {
         send_denied_msg(who);
         return 0;
      }
      return 1;
   }
   if(level > maxuserlevel)
   {
      send_to_server(":%s NOTICE %s :L'accès Admin est requis pour cette commande.\n", bot.nick, nwho, bot.nick);
      return 0;
   }
   if(rest == NULL) 
   {
      send_denied_msg(who);
      return 0;
   }

   added_level = 0;
   chan = strtok(restbuf, " ");
   nick = strtok(NULL, " ");
   strlwr(chan);
   if(chan_check(chan, who) == -1) return 0;
   regstmp = return_regsinfo(chan);
   if(regstmp != NULL)
   {
      if(admntmp != NULL)
      {
         if(regstmp->suspend && admnauth == NULL)
         {
	    send_to_server(":%s NOTICE %s :Le salon %s a été suspendu\n", bot.nick, nwho, chan);
             return 0;
         }
      }
      else if(regstmp->suspend)
      {
	 send_to_server(":%s NOTICE %s :Le salon %s a été suspendu\n", bot.nick, nwho, chan);
          return 0;
      }
   }
   usertmp = return_userinfo(who, chan);
   if(usertmp != NULL)
   {
      if(usertmp->suspend)
      {
	 send_to_server(":%s NOTICE %s :Votre accès sur %s a été suspendu\n", bot.nick, nwho, chan);
          return 0;
      }      
   }
   authtmp = return_authinfo(chan, who);
   if(authtmp != NULL)
   {
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
                  if(!strcmp(nwho, num->numeric))
                  {
                     added_level = user->level;
                     if(user->level < level)   
                     {
                        send_denied_msg(who);
                        return 0;
                     }
                     user->lastseen = time(NULL);
                     return 1;
                  }
                  num = num->next;
               }
               user = user->next;
            }
         }
         regs = regs->next;
      }
   }
   if(usertmp != NULL)
   {
      if(usertmp->level < level)
      {
         send_denied_msg(who);
         return 0;
      }
      if(authtmp == NULL)
      {
	 send_to_server(":%s NOTICE %s :Veuillez utiliser la commande AUTH pour vous identifier sur %s\n", bot.nick, nwho, bot.nick);
	 send_to_server(":%s NOTICE %s :Tapez       /msg %s help auth\n", bot.nick, nwho, bot.nick);
         return 0;
      }
      if(rest != NULL)
      {
         if(pass = strrchr(rest, ' ')) *pass++ = '\0';
         if(pass != NULL)
         {
            strcpy(tmppass, admntmp->password);
            sprintf(keys, "%c%c", tmppass[0], tmppass[1]);
            cryptpass = mkpass(pass, keys);
            if(!strcmp(usertmp->password, cryptpass))
            {
	       send_to_server(":%s NOTICE %s :Veuillez ne pas incluer pas votre mot de passe.\n", bot.nick, nwho);
               return 0;
            }
         }
      }
   }
   if(rest == NULL)
   {
      send_denied_msg(who);
      return 0;
   }
   send_denied_msg(who);
   added_level = 0;
   return 0;
}
