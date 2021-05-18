/* CS5 - Channel Services src/clearban.c
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

void clearban(char *who, char *rest)
{
   struct regsinfo *regs;
   struct baninfo *bantmp;
   struct userinfo *user, *admn;
   struct authinfo *admnauth;
   char test[1000], *chan = NULL, *uhost = NULL, *nwho = NULL;
   char *mask = NULL, *level = NULL, *helpname;
   int l = 0;

   test[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   uhost = getnickuserhost(who);
   if(uhost == NULL) return;
   user = return_userinfo(who, chan);
   admn = return_userinfo(who, csadminchan);
   admnauth = return_authinfo(csadminchan, who);
   helpname = get_command_name("clearbans");
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         bantmp = regs->banhead;
         while(bantmp != NULL)
         {
/* NiX
            if(!bancmp(uhost, bantmp->ban) || !bancmp(bantmp->ban, uhost))
            {
               send_to_server(":%s NOTICE %s : Nice try , but i don't think so!\n", bot.nick, nwho);
               send_to_server(":%s NOTICE %s : Vous etes banni de ce salon\n", bot.nick, nwho);
               return;
            }
*/
            bantmp = bantmp->next;
         }
      }
      regs = regs->next;
   }
   if(strchr(rest, '<') || strchr(rest, '>'))
   {
      if(level = strchr(rest, ' '))
      {
         *level++ = '\0';
         if(mask = strchr(level, ' '))
         {
            *mask++ = '\0';
            if(*level == '>')
            {
               *level++;
               if(!is_num(level))
               {
		  send_to_server(":%s NOTICE %s : Le level a été spécifié incorrectement\n", bot.nick, nwho);
                  return;
               }
               if(admn == NULL && user->level < atoi(level))
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               if(admnauth != NULL)
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               regs = regshead;
               while(regs != NULL)
               {
                  if(!strcasecmp(rest, regs->channel))
                  {
                     bantmp = regs->banhead;
                     {
                        while(bantmp != NULL)
                        {
                           if(atoi(level) <= bantmp->level && (!bancmp(bantmp->ban, mask) || !bancmp(mask, bantmp->ban)))
                           {
                              send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
                              delete_bnode(chan, bantmp->ban);
                           }
                           bantmp = bantmp->next;
                        }
                     }
                  }
                  regs = regs->next;
               }
               write_single_banfile(chan);
               return;
            }
            else if(*level == '<')
            {
               *level++;
               if(!is_num(level))
               {
		  send_to_server(":%s NOTICE %s : Le level a été spécifié incorrectement\n", bot.nick, nwho);
                  return;
               }
               if(admn == NULL && user->level < atoi(level))
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               if(admnauth != NULL)
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               regs = regshead;
               while(regs != NULL)
               {
                  if(!strcasecmp(rest, regs->channel))
                  {
                     bantmp = regs->banhead;
                     {
                        while(bantmp != NULL)
                        {
                           if(atoi(level) >= bantmp->level && (!bancmp(bantmp->ban, mask) || !bancmp(mask, bantmp->ban)))
                           {
                              send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
                              delete_bnode(chan, bantmp->ban);
                           }
                           bantmp = bantmp->next;
                        }
                     }
                  }
                  regs = regs->next;
               }
               write_single_banfile(chan);
               return;
            }
	    send_to_server(":%s NOTICE %s : Le level a été spécifié incorrectement\n", bot.nick, nwho);
         }
         else
         {
            if(*level == '>')
            {
               *level++;
               if(!is_num(level))
               {
		  send_to_server(":%s NOTICE %s : Le level a été spécifié incorrectement\n", bot.nick, nwho);
                  return;
               }
               if(admn == NULL && user->level < atoi(level))
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               if(admnauth != NULL)
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               regs = regshead;
               while(regs != NULL)
               {
                  if(!strcasecmp(rest, regs->channel))
                  {
                     bantmp = regs->banhead;
                     {
                        while(bantmp != NULL)
                        {
                           if(atoi(level) <= bantmp->level)
                           {
                              send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
                              delete_bnode(chan, bantmp->ban);
                           }
                           bantmp = bantmp->next;
                        }
                     }
                  }
                  regs = regs->next;
               }
               write_single_banfile(chan);
               return;
            }
            else if(*level == '<')
            {
               *level++;
               if(!is_num(level))
               {
		  send_to_server(":%s NOTICE %s : Le level a été spécifié incorrectement\n", bot.nick, nwho);
                  return;
               }
               if(admn == NULL && user->level < atoi(level))
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               if(admnauth != NULL)
               {
		  send_to_server(":%s NOTICE %s : Le level spécifié est plus élevé que le votre\n", bot.nick, nwho);
                   return;
               }
               regs = regshead;
               while(regs != NULL)
               {
                  if(!strcasecmp(rest, regs->channel))
                  {
                     bantmp = regs->banhead;
                     {
                        while(bantmp != NULL)
                        {
                            if(atoi(level) >= bantmp->level)
                            {
                               send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
                               delete_bnode(chan, bantmp->ban);
                            }
                            bantmp = bantmp->next;
                        }
                     }
                  }
                  regs = regs->next;
               }
               write_single_banfile(chan);
               return;
            }
	    send_to_server(":%s NOTICE %s : Le level a été spécifié incorrectement\n", bot.nick, nwho);
            write_single_banfile(chan);
            return;
         }
      }
      else
      {
         do_help(who, helpname);
         return;
      }
   }
   else if(strchr(rest, '@') && strchr(rest, '!'))
   {
      if(mask = strchr(rest, ' ')) *mask++ = '\0';
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            bantmp = regs->banhead;
            while(bantmp != NULL)
            {
               if(admn != NULL && (!bancmp(mask, bantmp->ban) || !bancmp(bantmp->ban, mask)) && (admn->level >= bantmp->level || admn->level == maxadmnlevel))
               {
                  send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
                  delete_bnode(chan, bantmp->ban);
               }
               if(admn == NULL && (!bancmp(mask, bantmp->ban) || !bancmp(bantmp->ban, mask)) && user->level >= bantmp->level)
               {
                  send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
                  delete_bnode(chan, bantmp->ban);
               }
               bantmp = bantmp->next;
            }
         }
         regs = regs->next;
      }
      write_single_banfile(chan);
      return;
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         bantmp = regs->banhead;
         while(bantmp != NULL)
         {
            if(admn != NULL && admnauth != NULL && (admn->level >= bantmp->level || admn->level == maxadmnlevel))
            {
               send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
               delete_bnode(chan, bantmp->ban);
            }
            else if(user != NULL && user->level >= bantmp->level)
            {
               send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, bantmp->ban);
               delete_bnode(chan, bantmp->ban);
            }
            bantmp = bantmp->next;
         }
      }
      regs = regs->next;
   }
   write_single_banfile(chan);
}
