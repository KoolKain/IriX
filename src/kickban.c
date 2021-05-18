/* CS5 - Channel Services src/kickban.c
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

void kick_ban(char *who, char *rest)
{
  struct userinfo *tmp, *usertmp, *admn, *admntmp;
  struct regsinfo *regs;
  struct nickinfo *tmp1, *tmp4;
  struct chaninfo *tmp2;
  struct authinfo *admnauth, *userauth, *tmp3;
  char *nick = NULL, *level = NULL, *chan = NULL, *tlevel = NULL, *nnwho = NULL, *uhost = NULL;
  char test[10000], *therest = NULL, newban[1000], *nwho = NULL, *banmask = NULL, *waste, *tnick, *helpname;

  /* IRCL: addition of new variable to cover remainder of *rest */
  char *foobar = NULL, test2[1000], *testtime;

  test[0]='\0';newban[0]='\0';
  nwho = nick2num(who);
  strcpy(test, rest);
  chan = strtok(test, " ");
  if(chan_check(chan, who) == -1) return;
  if(check(chan, who) == -1) return;
  admn = return_userinfo(who, csadminchan);
  tmp = return_userinfo(who, chan);
  helpname = get_command_name("kickban");
  if(level = strchr(rest, ' '))
    {
      *level++ = '\0';
      tlevel = strtok(NULL, " ");
      if(is_num(tlevel))
	{
	  if(nick = strchr(level, ' '))
	    {
	      *nick++ = '\0';
	      tnick = strtok(NULL, " ");
	      if(strchr(tnick, '!') || strchr(tnick, '@'))
		{
		  do_help(who, helpname);
		  return;
		}
	      /* NiX            if(!strcasecmp(nick, bot.nick))
		 {
		 send_to_server(":%s NOTICE %s : Now you went and did it, I'm leaving and not coming back\n", bot.nick, nwho);
		 send_to_server(":%s PART %s\n", bot.nick, chan);
		 return;
		 } */
	      /* STE */
	      if(!strcasecmp(tnick, bot.nick))
		{
		  send_to_server(":%s KICK %s %s :mais bien sur!\n", bot.nick, chan, nwho);
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
	      usertmp = return_userinfo(banmask, chan);
	      admntmp = return_userinfo(banmask, csadminchan);
	      userauth = return_authinfo(chan, nick);
	      admnauth = return_authinfo(csadminchan, nick);
	      if(admntmp != NULL && admntmp->protect && admnauth != NULL)
		{
		  send_to_server(":%s NOTICE %s : Ce masque représente un CSA\n", bot.nick, nwho);
		  return;
		}
	      else if(usertmp != NULL && usertmp->protect && userauth != NULL)
		{
		  send_to_server(":%s NOTICE %s : Ce masque représente un utilisateur protégé\n", bot.nick, nwho);   
		  return;
		}
	      ban(who, newban);
	      nnwho=nick2num(nick);

	      /* IRCL: addition/change here in this section to get the reason (if any) */
	      if (therest != NULL)
		{
		  strcpy(test2, therest);
		  testtime = strtok(test2, " ");
		  if(strchr(testtime, ':'))
		    {
		      if(foobar = strchr(therest, ' ')) *foobar++ = '\0';
		      send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnwho, foobar);
		    }
		  else
		    {
		      send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnwho, therest);
		    }
		}
	      else
		{
		  send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnwho, reason);
		}
	      /* end of changes */

	      tmp1 = nickhead;
	      while(tmp1 != NULL)
		{
		  tmp2 = tmp1->chanhead;
		  while(tmp2 != NULL)
		    {
		      if(!strcasecmp(chan, tmp2->channel))
			{
			  tmp3 = return_authinfo(chan, tmp1->nick);
			  if(tmp3 != NULL && tmp2->is_ops)
			    send_to_server(":%s NOTICE %s : IriX a kickban %s depuis %s ( %s [%s])\n", bot.nick, tmp1->numeric, nick, chan, reason, who);
			}
		      tmp2 = tmp2->next;
		    }
		  tmp1 = tmp1->next;
		}
	    }
	  else
	    {
	      do_help(who, helpname);
	      return;
	    }
	}
      else
	{
	  if(strchr(tlevel, '!') || strchr(tlevel, '@'))
	    {
	      do_help(who, helpname);
	      return;
	    }
	  if(therest = strchr(level, ' ')) *therest++ = '\0';
	  else
            therest = NULL;
	  /* NiX          if(!strcasecmp(level, bot.nick))
	     {
	     send_to_server(":%s NOTICE %s : Now you went and did it, I'm leaving and not coming back\n", bot.nick, nwho);
	     send_to_server(":%s PART %s\n", bot.nick, chan);
	     return;
	     } */
	  /* STE */
	  if(!strcasecmp(tlevel, bot.nick))
            {
	      send_to_server(":%s KICK %s %s :mais bien sur!\n", bot.nick, chan, nwho);
	      return;
            }



	  if(uhost = getuserhost(level));
	  if(uhost == NULL)
	    {
	      send_to_server(":%s NOTICE %s : User is not currently online\n", bot.nick, nwho);
	      return;
	    }
	  banmask = get_ban_mask(uhost);
	  if(therest != NULL)
            sprintf(newban, "%s %s %s", chan, banmask, therest);
	  else
            sprintf(newban, "%s %s", chan, banmask);
	  usertmp = return_userinfo(banmask, chan);
	  admntmp = return_userinfo(banmask, csadminchan);
	  userauth = return_authinfo(chan, level);
	  admnauth = return_authinfo(csadminchan, level);
	  if(admntmp != NULL && admntmp->protect && admnauth != NULL)
	    {  
	      send_to_server(":%s NOTICE %s : Ce masque représente un CSA\n", bot.nick, nwho);
	      return;
	    }  
	  else if(usertmp != NULL && usertmp->protect && userauth != NULL)
	    {  
	      send_to_server(":%s NOTICE %s : Ce masque représente un utilisateur protégé\n", bot.nick, nwho);                        
	      return;
	    }
	  ban(who, newban);
	  nnwho=nick2num(level);

	  /* IRCL: similar addition/change here in this section to get the reason (if any) */
	  if (therest != NULL)
	    {
	      strcpy(test2, therest);
	      testtime = strtok(test2, " ");
	      if(strchr(testtime, ':'))
		{
                  if(foobar = strchr(therest, ' ')) *foobar++ = '\0';
		  send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnwho, foobar);
		}
	      else
		{
                  send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnwho, therest);
		}
	    }
	  else
	    {
	      send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnwho, reason);
	    }
	  /* end of changes */

	  tmp1 = nickhead;
	  while(tmp1 != NULL)
	    {
	      tmp2 = tmp1->chanhead;
	      while(tmp2 != NULL)
		{
		  if(!strcasecmp(chan, tmp2->channel))
		    {
		      tmp3 = return_authinfo(chan, tmp1->nick);
		      if(tmp3 != NULL && tmp2->is_ops)
			send_to_server(":%s NOTICE %s : IriX a kickban %s depuis %s ( %s [%s])\n", bot.nick, tmp1->numeric, level, chan, reason, who);
		    }
		  tmp2 = tmp2->next;
		}
	      tmp1 = tmp1->next;
	    }
	}
    }
  else
    {
      do_help(who, helpname);
      return;
    }
}
