/* CS5 - Channel Services src/nickban.c
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

void nickban(char *who, char *rest)
{
   char *uhost = NULL, *nick = NULL, *level = NULL, *chan = NULL, *tlevel = NULL;
   char test[10000], *therest = NULL, newban[1000], *nwho = NULL, *banmask = NULL, *helpname;

   test[0]='\0';newban[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   helpname = get_command_name("nickban");
   if(level = strchr(rest, ' '))
   {
      *level++ = '\0';
      tlevel = strtok(NULL, " ");
      if(is_num(tlevel))
      {
         if(nick = strchr(level, ' '))
         {
            *nick++ = '\0';
            if(strchr(nick, '!') || strchr(nick, '@'))
            {
               do_help(who, helpname);
               return;
            }
            if(therest = strchr(nick, ' ')) *therest++ = '\0';
            else
               therest = NULL;
            if(uhost = getuserhost(nick));
            if(uhost == NULL)
            {
	       send_to_server(":%s NOTICE %s : L'utilisateur n'est pas actuellement connecté\n", bot.nick, nwho);
               return;
            }
            banmask = get_ban_mask(uhost);
            if(therest != NULL)
               sprintf(newban, "%s %s %s %s", chan, level, banmask, therest);
            else
               sprintf(newban, "%s %s %s", chan, level, banmask);
            ban(who, newban);
         }
         else
         {
            do_help(who, helpname);
            return;
         }
      }
      else
      {
         if(strchr(level, '!') || strchr(level, '@'))
         {
            do_help(who, helpname);
            return;
         }
         if(therest = strchr(level, ' ')) *therest++ = '\0';
         else
            therest = NULL;
         uhost = getuserhost(level);
         if(uhost == NULL)
         {
	    send_to_server(":%s NOTICE %s : L'utilisateur n'est pas actuellement connecté\n", bot.nick, nwho);
            return;
         }
         banmask = get_ban_mask(uhost);
         if(therest != NULL)
            sprintf(newban, "%s %s %s", chan, banmask, therest);
         else
            sprintf(newban, "%s %s", chan, banmask);
         ban(who, newban);
      }
   }
   else
   {
      do_help(who, helpname);
      return;
   }
}
