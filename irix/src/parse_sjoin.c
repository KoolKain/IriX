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
 *
 *   while(*nick){
 *   
 *	if(!nick) return;
 * 	else if((user = strrchr(nick, ' ')) != NULL) *user++ = '\0';
 *   else if((user = strchr(nick, ' ')) != NULL) *user++;
 *	else return;
 *  	
 *	if(strchr(user, ':')) *user++;
 *
 *
 *
 *
 */           


#include <bot.h>

void parse_sjoin(char *who, char *rest)
{
   struct userinfo *tmp, *tmp1, *mask;
   struct authinfo *admnauth, *authtmp;
   struct regsinfo *tmp2;
   struct nickinfo *current, *tmp3;
   char *ptr[12], *p = NULL, *server = NULL, *numeric = NULL, infoline[1500], usr[50], usr2[50], *user = NULL , *nick = NULL, *arg1 = NULL, *arg2 = NULL, *arg3 = NULL, *arg4 = NULL, *nwho = NULL, *usernum;
   int got_ops = 0;
   int primary = 0;
   int got_ops_ok = 0;
   int check_suspend = 0;
   int i = 0;
   
	if (arg1 = strchr(rest, ' ')) *arg1++ = '\0'; /* TS */
    if (arg2 = strchr(arg1, ' ')) *arg2++ = '\0'; /* CHAN */
	if (arg3 = strchr(arg2, ' ')) *arg3++ = '\0'; /* CHAN MODES */
	if (nick = strchr(arg3, ' ')) *nick++ = '\0'; /* JOIN NICK */
   
   strlwr(arg2); 
   
   /* check des nicks */

while(*nick){
	if((user = strrchr(nick, ' ')) != NULL) *user++ = '\0';
    else if((user = strchr(nick, ' ')) != NULL) *user++;
	else if((user = strchr(nick, ':')) != NULL) *user++ = '\0';
	else return;
    
	tmp2 = return_regsinfo(arg2);
   #ifdef DEBUG
   printf("Join who [%s], rest [%s]\n", who, rest);
   printf("Join reste nick [%s]\n", nick);
   printf("Join arg1 [%s], arg2 [%s], arg3 [%s], nick [%s]\n", arg1, arg2, arg3, user);
   printf("Join: traitement nick: [%s]\n", user);
   #endif

	   strcpy(usr2, user);
	   if (strchr(user, ':')) *user++; /* Normal user */
	   if (strchr(user, '@')) *user++; /* Op user */
	   if (strchr(user, '+')) *user++; /* voice user */
	   if (strchr(user, '@')) *user++; /* Op after voice user */
	   
	   if(chan_check(arg2, user) == -1) return;

	   got_ops = 0;
			 /* join chan */

         if(strcasecmp(user, bot.nick) && !check_suspend && strrchr(usr2, '@')) 
	      {
            if(tmp2 != NULL)
             {
               if(!tmp2->suspend)
                {		 
				          usernum = nick2num(bot.nick);
                          send_to_server(":%s SJOIN %ld %ld %s + :@%s\n", myserver, time(NULL), time(NULL), arg2, bot.nick);
                          send_to_server(":%s MODE %s +o %s\n", bot.nick, arg2, bot.nick);
						  add_joininfo(arg2, usernum, TRUE);
						  deopall(bot.nick, arg2);
						  send_to_server(":%s TOPIC %s :%s\n", bot.nick, tmp2->channel, tmp2->topic);
		                  send_to_server(":%s MODE %s %s\n", bot.nick, tmp2->channel, tmp2->defmodes);
               }
                else
                  send_to_server(":%s NOTICE %s : This Channel is registered, but is Suspended\n", botnick, user);
             }
			 check_suspend = 1;
         }

	   if(joined)
       {
          if(tmp2 != NULL  && !tmp2->suspend)
          {
             show_welcome(user, arg2);
             check_single_ban_time(arg2);
             load_banfiles(arg2);
             enforce_ban(arg2, user);
          }
       }
	       
       numeric = nick2num(user);
       server = myserver;
		  if(ison(arg2))
          {
             send_to_server(":%s MODE %s\n", bot.nick, arg2);
             tmp3 = return_nickinfo(user);

             if(tmp2->operop && tmp3->is_oper && !tmp2->noops && !tmp2->suspend && tmp2->autoops)
             {
                send_to_server(":%s MODE %s +o %s\n", bot.nick, arg2, user);
                got_ops = 1;
				got_ops_ok = 1;
             }
             tmp = return_userinfo(user, arg2);
             do_voice(user, arg2);
             mask = return_maskinfo(user, arg2);
             if(mask != NULL && strlen(mask->info) > 0 && !tmp2->suspend)
             {
                sprintf(infoline, "[%s] %s", mask->nick, mask->info);
                send_to_server(":%s PRIVMSG %s :%s\n", bot.nick, arg2, infoline);
             }

             if(tmp != NULL && !tmp2->suspend)
             {
                current = nickhead;
                if(tmp->autoop && tmp2->autoops && !tmp2->noops)
                {
                   while(current != NULL)
                   {
                      if(!strcasecmp(user,current->nick))
                      {
                         authtmp = return_authinfo(arg2, user);
                         if(authtmp != NULL && !got_ops)
                         {
                            send_to_server(":%s MODE %s +o %s\n", bot.nick, arg2, user);
                            got_ops = 1;
							got_ops_ok = 1;
                         }
                      }
                      current = current->next;
                   }
                }
             }
             else
             {
                tmp1 = return_userinfo(user, csadminchan);
                if(tmp1 != NULL && !tmp2->suspend)
                {
                   current = nickhead;
                   if(tmp1->autoop && !tmp2->noops && tmp2->autoops)
                   {
                      while(current != NULL)
                      {
                         if(!strcasecmp(user,current->nick))
                         {
                            admnauth = return_authinfo(csadminchan, user);
                            if(admnauth != NULL && !got_ops)
                            {
                               send_to_server(":%s MODE %s +o %s\n", bot.nick, arg2, user);
                               got_ops = 1;
							   got_ops_ok = 1;
                            }
                         }
                         current = current->next;
                      }
                   }
                }
             }
		  }
		  /* cas d'un SJOIN a nick unique et non-op */
		  if(strrchr(usr2, '@') && !got_ops_ok) got_ops = 1;
          /* */
		  if(got_ops) 
             add_joininfo(arg2, numeric, TRUE);
          else  
		     add_joininfo(arg2, numeric, FALSE);
   }
  
}
