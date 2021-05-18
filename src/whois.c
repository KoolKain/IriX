/* CS5 - Channel Services src/whois.c
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

void whois(char *who, char *rest)
{
   struct nickinfo *nicktmp, *nicktmp1, *chan, *nick3;
   struct chaninfo *chantmp;
   struct authinfo *authtmp, *auth;
   struct regsinfo *regs;
   char *nick, *uhost, *ptr, buhost[100], duhost[100], *caddr = NULL, ip_addr[100], buff[10000], buff1[10000];
   char *ptr2, usrhost[100], *ptr3, *ptr4, *ptr5, *auhost, *nwho, onchan[10000], authon[10000], *newmodes, *helpname;
   int howmany = 0, first_time =0, firsttime = 0;

      helpname = get_command_name("whois");
      nwho = nick2num(who);
      nick = strtok(rest, " ");
      if(nick == NULL)
      {
         do_help(who, helpname);
         return;
      }
      if(strlen(nick) > NICKLEN) return;
      if((uhost = getuserhost(nick)) == NULL) return;
      if(uhost == NULL)
      {
      send_to_server(":%s NOTICE %s : L'utilisateur n'est pas actuellement connecté\n", bot.nick, nwho);
         return;
      }
      if((auhost = getnickuserhost(nick)) == NULL) return;
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
 
   nicktmp = return_nickinfo(nick);
   if(nicktmp == NULL)
   {
      send_to_server(":%s NOTICE %s : L'utilisateur n'est pas actuellement connecté\n", bot.nick, nwho);
      return;
   }
   auth = return_authinfo(csadminchan, who);
   nick3 = return_nickinfo(who);
   nicktmp1 = nickhead;
   while(nicktmp1 != NULL)
   {
      if(!strcasecmp(nick, nicktmp1->nick))
      {
         first_time = 0;
         buff[0] = '\0';
         buff1[0] = '\0';
         onchan[0] = '\0';
         chantmp = nicktmp1->chanhead;
         while(chantmp != NULL)
         {
            regs = return_regsinfo(chantmp->channel);
            chan = return_chaninfo(who, chantmp->channel);
            if((strchr(nicktmp->modes, 'i') && (chan != NULL || nick3->is_oper)) || (!strchr(nicktmp->modes, 'i') || !strcasecmp(who, nick) || auth != NULL))
            {
/*
               if((nick3->is_oper || auth != NULL) || (!strchr(regs->modes, 's') && !strchr(regs->modes, 'p')))
               {
*/
                  if(!firsttime)
                  {
                     if(caddr != NULL)
                     {
		     send_to_server(":%s NOTICE %s : Masque       : %s\n", bot.nick, nwho, usrhost);
		     send_to_server(":%s NOTICE %s : Adresse IP   : %s\n", bot.nick, nwho, ip_addr);
		     send_to_server(":%s NOTICE %s : Masque actuel: %s\n", bot.nick, nwho, auhost);
		     if(nicktmp->is_oper)
			send_to_server(":%s NOTICE %s : Accès       : Est un IRCop\n", bot.nick, nwho, auhost);
		     send_to_server(":%s NOTICE %s : Vrai Nom     : %s\n", bot.nick, nwho, nicktmp->name);
		     send_to_server(":%s NOTICE %s : Serveur      : %s\n", bot.nick, nwho, nicktmp->server);
		  }
                     if(caddr == NULL)
                     {
    		     send_to_server(":%s NOTICE %s : Masque        : %s\n", bot.nick, nwho, usrhost);
		     send_to_server(":%s NOTICE %s : Masque actuel : %s\n", bot.nick, nwho, auhost);
		     if(nicktmp->is_oper)
			send_to_server(":%s NOTICE %s : Accès        : Is an IRC Operator\n", bot.nick, nwho, auhost);
		     send_to_server(":%s NOTICE %s : Vrai nom      : %s\n", bot.nick, nwho, nicktmp->name);
		     send_to_server(":%s NOTICE %s : Serveur       : %s\n", bot.nick, nwho, nicktmp->server);
		  }
                  firsttime = 1;
                  }
                  if(chantmp->is_ops)
                    sprintf(onchan, "@%s ", chantmp->channel);
                  else
                    sprintf(onchan, "%s ", chantmp->channel);
                  strcat(buff, onchan);
/*
               }
*/
            }
            chantmp = chantmp->next;
         }
         authtmp = nicktmp1->authhead;
         while(authtmp != NULL)
         {
            chan = return_chaninfo(who, authtmp->channel);
            if((strchr(nicktmp->modes, 'i') && (chan != NULL || nick3->is_oper)) || !strchr(nicktmp->modes, 'i') || !strcasecmp(who, nick) || auth != NULL)
            {
               strcat(buff1, authtmp->channel);
               strcat(buff1, "  ");
            }
             authtmp = authtmp->next;
         }
      }
      nicktmp1 = nicktmp1->next;
   }
   if(!strcasecmp(nick, bot.nick) || !strcasecmp(nick, bot.nick)) return;
   if(firsttime)
   {
      send_to_server(":%s NOTICE %s : Sur les salons : %s\n", bot.nick, nwho, buff);
      send_to_server(":%s NOTICE %s : Autorisé sur   : %s\n", bot.nick, nwho, buff1);
   }
   else
      send_to_server(":%s NOTICE %s : Pas d'information à fournir\n", bot.nick, nwho);
}
