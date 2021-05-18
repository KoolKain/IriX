/* CS5 - Channel Services src/csauth.c
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

void csauth(char *who, char *rest)
{
   struct userinfo *admn, *test;
   struct regsinfo *regs;
   struct adderinfo *addr;
   struct authinfo *admnauth, *userauth;
   char *nwho = NULL, *uhost = NULL, *nick = NULL, buff[10000], *passwd = NULL, *helpname;
   char *cryptpass = NULL, *pass = NULL, tmppass[1000], keys[3], *nuhost = NULL, buhost[100], *authuhost = NULL;
   int gotit = 0;

   tmppass[0]='\0';buff[0]='\0';keys[0]='\0';buhost[0]='\0';
   strcpy(buff, rest);
   helpname = get_command_name("csauth");
   if(nick = strchr(rest, ' ')) *nick++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   nwho = nick2num(who);
   if(rest[0] != '#')
   {
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(regs->channel, csadminchan))
         {
            admn = regs->userhead;
            while(admn != NULL)
            {
               if(!strcasecmp(rest, admn->nick))
               {
                  uhost = getnickuserhost(who);
                  strcpy(buhost, uhost);
                  nuhost = swap_host(buhost);
                  if(nuhost == NULL) nuhost = uhost;
                  addr = admn->adderhead;
                  while(addr != NULL)
                  {
                     if(!wcmp(addr->address, uhost) || !wcmp(addr->address, nuhost))
                     {
                        if(pass = strrchr(buff, ' ')) *pass++ = '\0';
                        else pass = "0";
                        strcpy(tmppass, admn->password);
                        sprintf(keys, "%c%c", tmppass[0], tmppass[1]);
                        cryptpass = mkpass(pass, keys);
                        if(strcmp(admn->password, cryptpass))
                        {
                           send_to_server(":%s NOTICE %s :Mauvais mot de pass!\n", bot.nick, nwho);
                           return;
                        }
                        if(admn->suspend)
                        {
                            send_to_server(":%s NOTICE %s :Votre accès Admin a été suspendu\n", bot.nick, nwho);
                            return;
                        }
                        admnauth = return_authinfo(csadminchan, who);
                        if(admnauth != NULL)
                        {
                           send_to_server(":%s NOTICE %s :Vous êtes déjà autorisé pour l'accès Admin\n", botnick, nwho);
                           return;
                        }
                        add_authinfo(csadminchan, who);
                        gotit = 1;
                        add_user_numinfo(csadminchan, rest, nwho);
                        admn->lastseen = time(NULL);
                        send_to_server(":%s NOTICE %s :Autorisation accordée pour l'accès CSA\n", botnick, nwho);
                        show_motd(who, csadminchan);
                        return;
                     }
                     addr = addr->next;
                  }
               }
               admn = admn->next;
            }
         }
         regs = regs->next;
      }
   }
   if(!gotit)
   {
      send_denied_msg(who);
   }
}
