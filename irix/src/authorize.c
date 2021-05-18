/* CS5 - Channel Services src/authorize.c
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

void authorize(char *who, char *rest)
{
   struct adderinfo *addr;
   struct userinfo *user;
   struct regsinfo *regs;
   struct authinfo *authtmp;
   char *nwho = NULL, *chan = NULL, *uhost = NULL, *nick = NULL, buff[10000], *passwd = NULL, test[10000], *authuhost = NULL;
   char *cryptpass = NULL, *pass = NULL, tmppass[10000], keys[3], *nuhost = NULL, buhost[100], *helpname;

   buff[0]='\0';test[0]='\0';tmppass[0]='\0';keys[0]='\0';buhost[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   strcpy(buff, rest);
   helpname = get_command_name("auth");
   if(nick = strchr(rest, ' ')) *nick++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   if(passwd = strchr(nick, ' ')) *passwd++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   authtmp = return_authinfo(rest, who);
   if(authtmp != NULL)
   {
      send_to_server(":%s NOTICE %s :Déjà autorisé sur %s\n", bot.nick, nwho, rest);
       return;
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(rest, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(nick, user->nick))
            {
               uhost = getnickuserhost(who);
               strcpy(buhost, uhost);
               nuhost = swap_host(buhost);
               if(nuhost == NULL) nuhost = uhost;
               addr = user->adderhead;
               while(addr != NULL)
               {
                  if(!wcmp(addr->address, uhost) || !wcmp(addr->address, nuhost))
                  {
                     if(pass = strrchr(buff, ' ')) *pass++ = '\0';
                     else pass = "0";
                     strcpy(tmppass, user->password);
                     sprintf(keys, "%c%c", tmppass[0], tmppass[1]);
                     cryptpass = mkpass(pass, keys);
                     if(strcmp(user->password, cryptpass))
                     {
			send_to_server(":%s NOTICE %s :Mot de passe incorrect pour %s\n", bot.nick, nwho, chan);
                        return;
                     }
                     if(user->suspend)
                     {
			send_to_server(":%s NOTICE %s :Votre accès sur %s a été suspendu\n", bot.nick, nwho, chan);
                         return;
                     }
                     add_authinfo(rest, who);
                     add_user_numinfo(chan, nick, nwho);
                     user->lastseen = time(NULL);
                     if(user->level >= maxuserlevel - 100)
                       regs->lastseen = time(NULL);
                     send_to_server(":%s NOTICE %s :Autorisation accordée\n", botnick, nwho);
                     show_motd(who, chan);
                     return;
                  }
                  addr = addr->next;
               }
	       send_to_server(":%s NOTICE %s :Aucun masque correspondant, accès refusé sur le salon %s\n", botnick, nwho, chan);
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
}

void authop(char *who, char *rest)
{
   struct adderinfo *addr;
   struct userinfo *user;
   struct regsinfo *regs;
   struct authinfo *authtmp;
   char *nwho, *chan, *uhost, *nick, buff[10000], *passwd, test[10000], *authuhost;
   char *cryptpass, *pass, tmppass[10000], keys[3], *nuhost, buhost[100], *helpname;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   strcpy(buff, rest);
   helpname = get_command_name("authop");
   if(nick = strchr(rest, ' ')) *nick++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   if(passwd = strchr(nick, ' ')) *passwd++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   authtmp = return_authinfo(rest, who);
   if(authtmp != NULL)
   {
      send_to_server(":%s NOTICE %s :Déjà autorisé sur le salon %s\n", bot.nick, nwho, rest);
       return;
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(rest, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(nick, user->nick))
            {
               uhost = getnickuserhost(who);
               strcpy(buhost, uhost);
               nuhost = swap_host(buhost);
               if(nuhost == NULL) nuhost = uhost;
               addr = user->adderhead;
               while(addr != NULL)
               {
                  if(!wcmp(addr->address, uhost) || !wcmp(addr->address, nuhost))
                  {
                     if(pass = strrchr(buff, ' ')) *pass++ = '\0';
                     else pass = "0";
                     strcpy(tmppass, user->password);
                     sprintf(keys, "%c%c", tmppass[0], tmppass[1]);
                     cryptpass = mkpass(pass, keys);
                     if(strcmp(user->password, cryptpass))
                     {
			send_to_server(":%s NOTICE %s :Mot de passe incorrect pour %s\n", bot.nick, nwho, chan);
                        return;
                     }
                     if(user->suspend)
                     {
			send_to_server(":%s NOTICE %s :Votre accès pour %s à été suspendu\n", bot.nick, nwho, chan);
                         return;
                     }
                     add_authinfo(rest, who);
                     add_user_numinfo(chan, nick, nwho);
                     user->lastseen = time(NULL);
                     if(user->level >= maxuserlevel - 100)
                       regs->lastseen = time(NULL);
                     send_to_server(":%s NOTICE %s :Autorisation accordée\n", botnick, nwho);
                     giveop(who, chan);
                     show_motd(who, chan);
                     return;
                  }
                  addr = addr->next;
               }
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
}
