/* CS5 - Channel Services src/add_user.c
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

void add_user(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *tmp;
   struct adderinfo *tmp1;
   char *chan = NULL, *address = NULL, *level = NULL, *aut = NULL, *pass = NULL, *waste = NULL, *addr = NULL, *passwd = NULL;
   char *nwho = NULL, *prot = NULL, *nick = NULL, *uhost = NULL, usrhost[100], *ptr2 = NULL, *caddr=NULL;
   char *ptr = NULL, ip_addr[100], test[10000], buhost[100], duhost[100], *garbage = NULL, *helpname;
   char *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL, *ptr6 = NULL, *ptr7 = NULL, *cryptpass = NULL, taddr[100], info1[100];
   int howmany = 0, autoop = 0, protect = 0;

   ip_addr[0]='\0';test[0]='\0';buhost[0]='\0';duhost[0]='\0';usrhost[0]='\0';taddr[0]='\0';info1[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if(!strcasecmp(csadminchan, chan))
   {
      send_to_server(":%s NOTICE %s : Vous ne pouvez pas utiliser cette commande sur ce salon\n", bot.nick, nwho);
      return;
   }
   nick = strtok(NULL, " ");
   helpname = get_command_name("adduser");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   if(strchr(nick, '@'))
     {
      send_to_server(":%s NOTICE %s : Vous devez entrer un pseudo et le séparer du *!*user@host\n", bot.nick, nwho);
      send_to_server(":%s NOTICE %s : Veuillez regarder l'aide pour adduser.\n", bot.nick, nwho);
       return;
     }
   if((addr = strtok(NULL, " ")) != NULL && (strchr(addr, '@') != NULL)) 
   {
      if(!strchr(addr, '!'))
      {
	 send_to_server(":%s NOTICE %s :Host masque invalide, la syntaxe est *!*user@host.\n", bot.nick, nwho);
         return;
      }      
      if(strlen(addr) > 100) addr[99] = '\0';
      strcpy(taddr, addr);
      if(ptr7 = strchr(taddr, '@')) *ptr7++ = '\0';
/* NiX
      if(!strcmp(taddr, "*!*"))
      {
         send_to_server(":%s NOTICE %s :Veuillez inclure l'username.\n", bot.nick, nwho);
         return;
      }  
      if(!strcmp(ptr7, "*") || !strcmp(ptr7, "*.*") || !strcmp(ptr7, "*.*.*"))
      {
	 send_to_server(":%s NOTICE %s :Veuillez entrer un hostname plus spécifique.\n", bot.nick, nwho);
         return;
      }
*/
      level = strtok(NULL, " ");
      if(level == NULL)
      {
         do_help(who, helpname);
         return;
      }
      if(!is_num(level))
      {
	 send_to_server(":%s NOTICE %s :Le LEVEL entré est incorrect.  Ce doit être une valeur numérique\n", bot.nick, nwho);
         return;
      }
      if(added_level <= atoi(level))
      {
	 send_to_server(":%s NOTICE %s :Vous ne pouvez pas ajouter un utilisateur avec un level plus élevé ou égal au votre\n", bot.nick, nwho);
	 send_to_server(":%s NOTICE %s :Votre level est de %d\n", bot.nick, nwho, added_level);
         return;
      }
      aut = strtok(NULL, " ");
      if(aut == NULL)
      {
         do_help(who, helpname);
         return;
      }
      if(!is_char(aut))
      {
	 send_to_server(":%s NOTICE %s :AUTOOP incorrectement entré.  Ce doit être ON ou OFF\n", bot.nick, nwho);
         return;
      }
      if(strcasecmp(aut, "ON"))
      {
         if(strcasecmp(aut, "OFF"))
         {
            do_help(who, helpname);
            return;
         }
      }
      prot = strtok(NULL, " ");
      if(prot == NULL)
      {
         do_help(who, helpname);
         return;
      }
      if(!is_char(prot))
      {
	 send_to_server(":%s NOTICE %s :PROTECT incorrectement entré.  Ce doit être ON ou OFF\n", bot.nick, nwho);
         return;
      }
      if(strcasecmp(prot, "ON"))
      {
         if(strcasecmp(prot, "OFF"))
         {
            do_help(who, helpname);
            return;
         }
      }
      pass = strtok(NULL, " ");
      if(pass == NULL) 
      {
         do_help(who, helpname);
         return;
      }
   }
   if(addr == NULL)
   {
      do_help(who, helpname);
      return;
   }
   if(addr && strchr(addr, '@')) 
   {
      tmp = ret_user_nickinfo(nick, chan);
      if(tmp != NULL)
      {
	 send_to_server(":%s NOTICE %s :Le pseudo %s est déjà dans la liste d'utilisateurs.\n", bot.nick, nwho, nick, chan);
         return;
      }
      cryptpass = mkpass(pass, "0");
      if(!strcasecmp(aut, "OFF")) autoop = 0;
      else autoop = 1;
      if(!strcasecmp(prot, "OFF")) protect = 0;
      else protect = 1;
      add_userinfo(chan, nick, atoi(level), autoop, protect, FALSE, cryptpass, time(NULL));
      add_user_addrinfo(chan, nick, addr);
      send_to_server(":%s NOTICE %s : Profil [%s] [ %s ] Autoop [%s] Protégé [%s] Suspendu [OFF]\n", bot.nick, nwho, nick, level, aut, prot);
      send_to_server(":%s NOTICE %s : Masques %s\n", bot.nick, nwho, addr);
      write_single_channel(chan);
      info1[0] = '\0';
      add_infoinfo(chan, nick, info1);
      write_single_infolines(chan);
      return;
   } 
   else 
   {
      char *ptr, *ptr2, usrhost[100];
      address = getuserhost(nick);
      if(address == NULL) 
      {
	 send_to_server(":%s NOTICE %s :Cet utilisateur n'est pas actuellement connecté.\n", bot.nick, nwho);
	 send_to_server(":%s NOTICE %s :Veuillez spécifier le pseudo et le user@host ensemble.\n", bot.nick, nwho);
         return;
      }
      level = addr;
      if(!is_num(level))
      {
	 send_to_server(":%s NOTICE %s :Le LEVEL entré est incorrect.  Ce doit être une valeur numérique\n", bot.nick, nwho);
         return;
      }
      if(added_level <= atoi(level))
      {
	 send_to_server(":%s NOTICE %s :Vous ne pouvez pas ajouter un utilisateur avec un level plus élevé ou égal au votre\n", bot.nick, nwho);
	 send_to_server(":%s NOTICE %s :Votre level est de %d\n", bot.nick, nwho, added_level);
         return;
      }
      passwd = strtok(NULL, " ");
      if(passwd == NULL) 
      {
         do_help(who, helpname);
         return;
      }
      garbage = strtok(NULL, " ");
      if(garbage != NULL) 
      {
         do_help(who, helpname);
         return;
      }
      cryptpass = mkpass(passwd, "0");
      if((uhost = getuserhost(nick)) == NULL) return;
      ptr = strchr(uhost, '@');
      ptr++;
      if(!is_ip(ptr)) 
      {
         strcpy(buhost, uhost);
         strcpy(duhost, uhost);
         howmany = countdots(ptr);

         if(howmany == 1)
         {
            if((caddr = convert_addr(buhost)) != NULL)
                sprintf(ip_addr, "*!*%s", caddr);
             sprintf(usrhost, "*!*%s", uhost);
         }
         else if(howmany == 3 || howmany == 2)
         {
            if((caddr = convert_addr(buhost)) != NULL)
                sprintf(ip_addr, "*!*%s", caddr);
            if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
            if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
            sprintf(usrhost, "*!*%s@*.%s", uhost, ptr2);
         }
         else if(howmany == 4)
         {
            if((caddr = convert_addr(buhost)) != NULL)
                sprintf(ip_addr, "*!*%s", caddr);
            if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
            if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
            if(ptr3 = strchr(ptr2, '.')) *ptr3++ = '\0';
            if(ptr4 = strchr(ptr3, '.')) *ptr4++ = '\0';
            sprintf(usrhost, "*!*%s@*.%s", uhost, ptr4);
         }
         else if(howmany >= 5)
         {
            if((caddr = convert_addr(buhost)) != NULL)
                sprintf(ip_addr, "*!*%s", caddr);
            if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
            if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
            if(ptr3 = strchr(ptr2, '.')) *ptr3++ = '\0';
            if(ptr4 = strchr(ptr3, '.')) *ptr4++ = '\0';
            if(ptr5 = strchr(ptr4, '.')) *ptr5++ = '\0';
            sprintf(usrhost, "*!*%s@*.%s", uhost, ptr5);
         }
      }
      else 
      {
         if(ptr = strrchr(uhost, '.')) 
         {
            *++ptr = '*';
            *++ptr = '\0';
         }
         sprintf(usrhost, "*!*%s", uhost);
      }
      tmp = ret_user_nickinfo(nick, chan);
      if(tmp != NULL)
      {
	 send_to_server(":%s NOTICE %s :Le pseudo %s est déjà dans la liste du salon %s.\n", bot.nick, nwho, nick, chan);
         return;
      }
      if(caddr == NULL)
      {
        add_userinfo(chan, nick, atoi(level), FALSE, FALSE, FALSE, cryptpass, time(NULL));
        add_user_addrinfo(chan, nick, usrhost);
      }
      if(caddr != NULL)
      {
        add_userinfo(chan, nick, atoi(level), FALSE, FALSE, FALSE, cryptpass, time(NULL));
        add_user_addrinfo(chan, nick, usrhost);
        add_user_addrinfo(chan, nick, ip_addr);
      }
      if(caddr == NULL)
      {
	 send_to_server(":%s NOTICE %s : Profil [%s] [ %d ] Autoop [OFF] Protégé [OFF] Suspendu [OFF]\n", bot.nick, nwho, nick,atoi(level));
	 send_to_server(":%s NOTICE %s : Masques %s\n", bot.nick, nwho, usrhost);
         write_single_channel(chan);
         info1[0] = '\0';
         add_infoinfo(chan, nick, info1);
         write_single_infolines(chan);
         return;
      }
      if(caddr != NULL)
      {
	 send_to_server(":%s NOTICE %s : Profil [%s] [ %d ] Autoop [OFF] Protégé [OFF] Suspendu [NO]\n", bot.nick, nwho, nick,atoi(level));
	 send_to_server(":%s NOTICE %s : Masques %s %s\n", bot.nick, nwho, usrhost, ip_addr);
         write_single_channel(chan);
         info1[0] = '\0';
         add_infoinfo(chan, nick, info1);
         write_single_infolines(chan);
         return;
      }
   } 
}
