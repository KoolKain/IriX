/* CS5 - Channel Services src/reg_chan.c
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

void register_channel(char *who, char *rest)
{
   struct regsinfo *regs, *tmp;
   struct userinfo *user;
   long i = 0;
   FILE *userf, *regf;
   char *chan, *pass, ip_addr[100], *nchan, *cryptpass, *ownername = NULL, *welcome = NULL, *motd = NULL, *desc = NULL, *ownemail = NULL, *deftopic = NULL;
   char *botnum = NULL, *uhost = NULL, usrhost[100], *caddr=NULL, *nwho = NULL;
   char buhost[100], duhost[100], buff[10000];
   char *ptr = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL;
   int howmany = 0;
   char *url = NULL, test1[10000], *chan1 = NULL;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   strcpy(test1, chan);
   chan1 = test1;

/* NiX
   while(*chan1)
   {
      if(!isascii(*chan1))
      {
	 send_to_server(":%s NOTICE %s : Désolé, nous ne pouvons valider que les caractères ASCII\n", bot.nick, nwho);
        return;
      }
      *chan1++;
   }
*/
   if(strlen(chan) > 200) chan[199] = '\0';
   if(*chan == '+')
   {
      send_to_server(":%s NOTICE %s :Vous ne pouvez pas enregistrer un salon sans modes ou un salon local!\n", bot.nick, nwho);
      return;
   }
   if(chan_check(chan, who) == -1) return;
   botnum=get_usernum(bot.nick);
   pass = strtok(NULL, " ");
   if(pass == NULL) 
   {
      send_to_server(":%s NOTICE %s :Un mot de passe est requis pour enregistrer un salon!\n", bot.nick, nwho);
      return;
   }
   if(check_chan(chan)) 
   {
      send_to_server(":%s NOTICE %s :Ce salon est déjà enregistré.\n", bot.nick, nwho);
      return;
   }
   if((uhost = getuserhost(who)) == NULL) return;
   ptr = strchr(uhost, '@'); 
   ptr++;
   if(!is_ip(ptr)) 
   {
      strcpy(buhost, uhost);
      strcpy(duhost, uhost);
      howmany = countdots(ptr);

      if(howmany <= 3)
      {
         if((caddr = convert_addr(buhost)) != NULL)
             sprintf(ip_addr, "*!*%s", caddr);
         if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
          if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
          sprintf(usrhost, "*!*%s@*.%s", uhost, ptr2);
      }
      if(howmany == 4)
      {
         if((caddr = convert_addr(buhost)) != NULL)
             sprintf(ip_addr, "*!*%s", caddr);
         if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
          if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
          if(ptr3 = strchr(ptr2, '.')) *ptr3++ = '\0';
          if(ptr4 = strchr(ptr3, '.')) *ptr4++ = '\0';
          sprintf(usrhost, "*!*%s@*.%s", uhost, ptr4);
      }
      if(howmany >= 5)
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
   if(is_registered(usrhost)) 
   {
      send_to_server(":%s NOTICE %s :Vous ne pouvez pas enregistrer plus d'un salon..\n", bot.nick, nwho);
      return;
   }
   send_to_server(":%s JOIN %s\n", bot.nick, chan);
   send_to_server(":%s MODE %s +o %s\n", myserver, chan, botnum);
   deopall(bot.nick, chan);
   send_to_server(":%s NOTICE %s :Registered channel %s under %s.\n", bot.nick, nwho, chan, usrhost);
   send_to_server(":%s NOTICE %s :If this isn't correct, /msg %s moduser %s %s address <newaddress>.\n", bot.nick, nwho, bot.nick, chan, who);
   cryptpass = mkpass(pass, "0");
   add_regsinfo(chan, time(NULL), usrhost, url, deftopic, "+nt", who, ownername, welcome, motd, time(NULL), desc, ownemail, FALSE, FALSE, 50, 50, FALSE, FALSE, FALSE /* STE */, FALSE, FALSE, FALSE, FALSE, FALSE, -1, FALSE, FALSE, TRUE);
   add_userinfo(chan, who, maxuserlevel, TRUE, FALSE, FALSE, cryptpass, time(NULL));
   if(caddr == NULL)
   {
      add_user_addrinfo(chan, who, usrhost);
   }
   if(caddr != NULL)
   {
      add_user_addrinfo(chan, who, usrhost);
      add_user_addrinfo(chan, who, ip_addr);
   }
   write_regs_list();
   write_single_channel(chan);
   default_topic(chan);
   default_modes(chan);
}
