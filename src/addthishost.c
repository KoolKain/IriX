/* CS5 - Channel Services src/addthishost.c
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

#channel idnick passwd

 */


#include<bot.h>

void addthishost(char *who, char *rest)
{

   struct regsinfo *regs;
   struct userinfo *user, *tmp;
   char *chan, *idnick, *passwd, *nwho, *helpname, test[10000], *cryptpass, tmppass[10000], keys[3];
   char *uhost, *ptr, buhost[100], duhost[100], *caddr, ip_addr[100], usrhost[100], *ptr2, *ptr3, *ptr4, *ptr5;
   int howmany = 0, testmask = 0;

   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   idnick = strtok(NULL, " ");
   helpname = get_command_name("addthishost");
   if(idnick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   passwd = strtok(NULL, " ");
   if(passwd == NULL)
   {
      do_help(who, helpname);
      return;
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(idnick, user->nick))
            {
               strcpy(tmppass, user->password);
               sprintf(keys, "%c%c", tmppass[0], tmppass[1]);
               cryptpass = mkpass(passwd, keys);
               if(strcmp(user->password, cryptpass))
               {
		  send_to_server(":%s NOTICE %s :Mot de passe incorrect!\n", bot.nick, nwho);
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
               testmask = add_user_addrinfo(chan, idnick, usrhost);
               if(testmask == 0)
               {
		  send_to_server(":%s NOTICE %s :MASQUE %s ajouté pour %s\n", bot.nick, nwho, usrhost, idnick);
               }
               if(testmask == 1)
               {
		  send_to_server(":%s NOTICE %s :MASQUE %s déjà existant pour %s\n", bot.nick, nwho, usrhost, idnick);
               }
               write_single_channel(chan);
               return;
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
}
