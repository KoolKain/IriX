/* CS5 - Channel Services src/parse_join.c
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

void parse_join(char *who, char *rest)
{
   struct userinfo *tmp, *tmp1, *mask;
   struct authinfo *admnauth, *authtmp;
   struct regsinfo *tmp2;
   struct nickinfo *current, *tmp3;
   char *ptr[12], *p = NULL, *server = NULL, *numeric = NULL, infoline[1500];
   int got_ops = 0;
   int i = 0;
   if((p = strtok(rest, ",")) != NULL) ptr[i++] = p;
   strlwr(p);
   if(chan_check(p, who) == -1) return;
   while((p = strtok(NULL, ",")) != NULL && i < 11) ptr[i++] = p;
   ptr[i] = NULL;
   i = 0;
   while(ptr[i])
   {
       tmp2 = return_regsinfo(ptr[i]);
       got_ops = 0;
       if(joined)
       {
          if(tmp2 != NULL && !tmp2->suspend)
          {
             show_welcome(who, ptr[i]);
             check_single_ban_time(ptr[i]);
             load_banfiles(ptr[i]);
             enforce_ban(ptr[i], who);
          }
       }
       if(*ptr[i] != '+')
         send_to_server(":%s MODE %s\n", bot.nick, ptr[i]);
       numeric = nick2num(who);
       server=num2serv(*numeric);
       if(*ptr[i] == '#' || *ptr[i] == '+')
       {
          if(ison(ptr[i]))
          {
             send_to_server(":%s MODE %s\n", bot.nick, ptr[i]);
             tmp3 = return_nickinfo(who);
             if(tmp2->operop && tmp3->is_oper && !tmp2->noops && !tmp2->suspend && tmp2->autoops)
             {
                send_to_server(":%s MODE %s +o %s\n", bot.nick, ptr[i], numeric);
                got_ops = 1;
             }
             tmp = return_userinfo(who, ptr[i]);
             do_voice(who, ptr[i]);
             mask = return_maskinfo(who, ptr[i]);
             if(mask != NULL && strlen(mask->info) > 0 && !tmp2->suspend)
             {
                sprintf(infoline, "[%s] %s", mask->nick, mask->info);
                send_to_server(":%s PRIVMSG %s :%s\n", bot.nick, ptr[i], infoline);
             }
             if(tmp != NULL && !tmp2->suspend)
             {
                current = nickhead;
                if(tmp->autoop && tmp2->autoops && !tmp2->noops)
                {
                   while(current != NULL)
                   {
                      if(!strcasecmp(who,current->nick))
                      {
                         authtmp = return_authinfo(ptr[i], who);
                         if(authtmp != NULL && !got_ops)
                         {
                            send_to_server(":%s MODE %s +o %s\n", bot.nick, ptr[i], numeric);
                            got_ops = 1;
                         }
                      }
                      current = current->next;
                   }
                }
             }
             else
             {
                tmp1 = return_userinfo(who, csadminchan);
                if(tmp1 != NULL && !tmp2->suspend)
                {
                   current = nickhead;
                   if(tmp1->autoop && !tmp2->noops && tmp2->autoops)
                   {
                      while(current != NULL)
                      {
                         if(!strcasecmp(who,current->nick))
                         {
                            admnauth = return_authinfo(csadminchan, who);
                            if(admnauth != NULL && !got_ops)
                            {
                               send_to_server(":%s MODE %s +o %s\n", bot.nick, ptr[i], numeric);
                               got_ops = 1;
                            }
                         }
                         current = current->next;
                      }
                   }
                }
             }
          }
          if(got_ops)
             add_joininfo(ptr[i], numeric, TRUE);
          else
             add_joininfo(ptr[i], numeric, FALSE);
       }
       i++;
   }
}

