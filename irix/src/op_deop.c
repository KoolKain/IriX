/* CS5 - Channel Services src/op_deop.c
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

void deop(char *who, char *rest)
{
   char *chan = NULL, *nick = NULL, *num = NULL, *nwho = NULL, nickbuff[10000], md[1000], test[10000], *uhost = NULL, *dnick = NULL;
   int num_nicks = 0;

   nickbuff[0]='\0';md[0]='\0';test[0]='\0';
   nwho=nick2num(who);
   nickbuff[0] = '%';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   strlwr(chan);
   if((nick = strtok(NULL, " ")) != NULL)
   {
      if(nick != NULL) nickbuff[0] = '\0';
      if((uhost = getuserhost(nick)) == NULL)
      {
	 send_to_server(":%s NOTICE %s :   L'utilisateur %s n'est pas connecté\n", bot.nick, nwho, nick);
      }
      else
      {
         if(!strcasecmp(nick, bot.nick))
         {
	    send_to_server(":%s NOTICE %s : Veuillez NE PAS deoper %s\n", bot.nick, nwho, bot.nick);
            return;
         }
         num=nick2num(nick);
         strcpy(nickbuff, num);
         strcpy(md, "-o");
      }
      num_nicks++;
      while((nick = strtok(NULL, " ")) != NULL && num_nicks < 6)
      {
         if((uhost = getuserhost(nick)) == NULL)
         {
	    send_to_server(":%s NOTICE %s :   L'utilisateur %s n'est pas connecté\n", bot.nick, nwho, nick);
         }
         else
         {
            if(!strcasecmp(nick, bot.nick))
            {
	       send_to_server(":%s NOTICE %s : Veuillez NE PAS deoper %s\n", bot.nick, nwho, bot.nick);
               return;
            }
            num=nick2num(nick);
            strcat(nickbuff, " ");
            strcat(nickbuff, num);
            if(md == '\0')
            {
              strcpy(md, "-o");
            }
            else
            {
              strcat(md, "o");
            }
         }
         num_nicks++;
      }
   }
   if(nickbuff[0] == '%')
   {
      send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, nwho);
      replace_onode(chan, nwho, FALSE);
      return;
   }
   else if(nickbuff[0] != '\0')
   {
      send_to_server(":%s MODE %s %s %s\n", bot.nick, chan, md, nickbuff);
      dnick = strtok(nickbuff, " ");
      replace_onode(chan, nwho, FALSE);
      while((dnick = strtok(NULL, " ")) != NULL)
      {
         replace_onode(chan, dnick, FALSE);
      }
   }
}

void giveop(char *who, char *rest)
{
   struct regsinfo *regs;
   char *chan = NULL, *nick = NULL, *nwho = NULL, nickbuff[10000], md[100], fname[1500], *num = NULL, test[10000], *uhost = NULL, *dnick = NULL;
   int num_nicks = 0;

   nickbuff[0]='\0';md[0]='\0';test[0]='\0';fname[0]='\0';
   nwho=nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   regs = return_regsinfo(chan);
   if(regs->noops)
   {
      send_to_server(":%s NOTICE %s :   Le NOOPS est à ON, les ops ne sont pas autorisés pour le moment. \n", bot.nick, nwho);
      return;
   }
   nickbuff[0] = '%';
   md[0] = '\0';
   strlwr(chan);
   if((nick = strtok(NULL, " ")) != NULL)
   {
      if(nick != NULL) nickbuff[0] = '\0';
      if((uhost = getuserhost(nick)) == NULL)
      {
	 send_to_server(":%s NOTICE %s :   L'utilisateur %s n'est pas connecté\n", bot.nick, nwho, nick);
      }
      else
      {
      num=nick2num(nick);
      strcpy(nickbuff, num);
      strcpy(md, "+o");
      }
      num_nicks++;
      while((nick = strtok(NULL, " ")) != NULL && num_nicks < 6)
      {
         if((uhost = getuserhost(nick)) == NULL)
         {
	    send_to_server(":%s NOTICE %s :   L'utilisateur %s n'est pas connecté\n", bot.nick, nwho, nick);
         }
         else
         {
            num=nick2num(nick);
            strcat(nickbuff, " ");
            strcat(nickbuff, num);
            if(md == '\0')
            {
              strcpy(md, "+o");
            }
            else
            {
              strcat(md, "o");
            }
         }
         num_nicks++;
      }
   }
   if(nickbuff[0] == '%')
   {
      send_to_server(":%s MODE %s +o %s\n", bot.nick, chan, nwho);
      replace_onode(chan, nwho, TRUE);
      return;
   }
   else if(nickbuff[0] != '\0')
   {
      send_to_server(":%s MODE %s %s %s\n", bot.nick, chan, md, nickbuff);
      dnick = strtok(nickbuff, " ");
      replace_onode(chan, nwho, TRUE);
      while((dnick = strtok(NULL, " ")) != NULL)
      {
         replace_onode(chan, dnick, TRUE);
      }
   }
}
