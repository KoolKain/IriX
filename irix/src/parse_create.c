/* CS5 - Channel Services src/parse_create.c
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

void parse_create(char *who, char *rest)
{  
   char *chan = NULL, *host = NULL, *numeric = NULL, *nwho = NULL, *stamp = NULL, servnum[3], *server = NULL, nrest[10000], *usernum = NULL;
   
   struct regsinfo *regs, *regstmp;
   char *ptr[12], *p, *chans;
   int i = 0;
   if(joined == FALSE)
   {
      strcpy(nrest, rest);
      if(stamp = strrchr(nrest, ' ')) *stamp++ = '\0';
      chans=strtok(rest, " ");
      if((p = strtok(chans, ",")) != NULL) ptr[i++] = p;
      strlwr(p);
      if(chan_check(p, who) == -1) return;
      while((p = strtok(NULL, ",")) != NULL && i < 11) ptr[i++] = p;
      ptr[i] = NULL;
      i = 0;
      while(ptr[i])
      {
         server=num2serv(*who);
         nwho = num2nick(who);
             if(chan_check(ptr[i], nwho) == -1) return;
             add_joininfo(ptr[i], who, TRUE);
             replace_onode(ptr[i], who, TRUE);
         i++;
      }
      return;
   }
   if(stamp = strrchr(rest, ' ')) *stamp++ = '\0';
   chans=strtok(rest, " ");
   if((p = strtok(chans, ",")) != NULL) ptr[i++] = p;
   strlwr(p);
   if(chan_check(p, who) == -1) return;
   while((p = strtok(NULL, ",")) != NULL && i < 11) ptr[i++] = p;
   ptr[i] = NULL;
   i = 0;
   while(ptr[i])
   {
      server=num2serv(*who);
      nwho = num2nick(who);
      if(strcasecmp(nwho, bot.nick))
      {
          if(chan_check(ptr[i], nwho) == -1) return;
          add_joininfo(ptr[i], who, TRUE);
          replace_onode(ptr[i], who, TRUE);
          regs = return_regsinfo(ptr[i]);
          if(regs != NULL)
          {
              if(!regs->suspend)
              {
                 usernum=get_usernum(bot.nick);
                 send_to_server(":%s JOIN %s\n", botnick, ptr[i]);
                 send_to_server(":%s MODE %s +o %s %lu\n", myserver, ptr[i], usernum, atol(stamp));
                 deopall(bot.nick, ptr[i]);
		 /* STE */
		 send_to_server(":%s TOPIC %s :%s\n", bot.nick,regs->channel,regs->topic);
		 send_to_server(":%s MODE %s %s\n", bot.nick,regs->channel, regs->defmodes);
/* send_to_server(":%s MODE %s %s\n", bot.nick, chan, tmp->defmodes); */
              }
              else
                 send_to_server(":%s NOTICE %s : Ce salon est enregistré ,mais il est suspendu.\n", botnick, who);
          }
      }
      i++;
   }
}
