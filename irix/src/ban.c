/* CS5 - Channel Services src/ban.c
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

void ban(char *who, char *rest)
{
  struct userinfo *tmp, *usertmp, *adminban, *admnwho;
  struct regsinfo *regs;
  struct authinfo *admn;
  char *level = NULL, *mask = NULL, *timeout = NULL, *duration = NULL, *units = NULL, *reason = NULL, *testtime = NULL;
  char test[1000], *nchan = NULL, *nwho = NULL, reason1[3], my[100], test1[10000], *chan = NULL, *timetest = NULL;
  char *helpname;
  long newtime;
  int bantest;

  reason1[0]='\0';test[0]='\0';my[0]='\0';test1[0]='\0';
  strcpy(test1, rest);
  chan = strtok(test1, " ");
  if(chan_check(chan, who) == -1) return;
  if(check(chan, who) == -1) return;
  nwho = nick2num(who);
  if(level = strchr(rest, ' '))
    {
      *level++ = '\0';
      if(level[0] == ' ' || test1[0] == ' ') /* STE */
	{
	 send_to_server(":%s NOTICE %s :Trop d'espaces dans la commande\n",bot.nick, nwho);
	  return;
	}
      admn = return_authinfo(csadminchan, who);
      admnwho = return_userinfo(who, csadminchan);
      regs = return_regsinfo(chan);/* STE */
      tmp = return_userinfo(who, chan); /* STE */

      /* STE */
      if(joined && admn == NULL && (tmp == NULL || regs == NULL)) {
	helpname = get_command_name("ban");
	do_help(who, helpname);
	return;
      }
      if(joined && admn == NULL && tmp->level < regs->banlvl)
	{
	 send_to_server(":%s NOTICE %s :Vous ne pouvez pas utiliser le BAN, le Level de ban est de %d\n",bot.nick, nwho, regs->banlvl);
	  return;
	}
      if(joined && admn != NULL && admn == NULL && tmp == NULL)
	{
	 send_to_server(":%s NOTICE %s :Vous ne pouvez pas utiliser le BAN, le Level de ban est de %d\n",bot.nick, nwho, regs->banlvl);
	  return;
	}
      if(regs->noban && admn == NULL)
	{
	 send_to_server(":%s NOTICE %s :Aucun ban autorisé - NOBANS est à ON\n",bot.nick, nwho);
	  return;
	}
      nchan = conv_char(rest);
      if(mask = strchr(level, ' '))
	{
	  *mask++ = '\0';
	  if(level[0] == ' ' || mask[0] == ' ')
	    {
	    send_to_server(":%s NOTICE %s :Trop d'espaces dans la commande\n",bot.nick, nwho);
	      return;
	    }
	  if(strchr(level, '!'))
	    {
	      if(strchr(level, '@'))
		{
		  if(!strcmp(level, "*!*@*") || !strcmp(level, "*!*@*.*") || !strcmp(level, "*!*@*.*.*"))
		    {
		  send_to_server(":%s NOTICE %s :Veuillez entrer un hostname plus spécifique.\n", bot.nick, nwho);
		      return;
		    }
		  usertmp = return_userinfo(level, rest);
		  adminban = return_userinfo(level, csadminchan);
		  if(adminban != NULL && adminban->protect)
		    {
		      if(admnwho != NULL && adminban->level > admnwho->level)
			{
		     send_to_server(":%s NOTICE %s : Ce masque représente un Admin avec un level plus élevé que le votre\n", bot.nick, nwho);
			  return;
			}
		    } 
		  else if((usertmp != NULL && usertmp->protect) || (usertmp != NULL && usertmp->level == maxuserlevel))
		    {
		      if(tmp != NULL && admn == NULL && usertmp->level > tmp->level)
			{
		     send_to_server(":%s NOTICE %s : Ce masque représente un utilisateur protégé avec un level plus élevé que le votre\n", bot.nick, nwho);
			  return;
			}
		    }
		  /* STE */
		  sprintf(my, "%s!*@%s", bot.nick,bot.server);
		  if(!bancmp(level,my) || !bancmp(my,level)) {
		    send_to_server(":%s NOTICE %s : That mask matches me\n", bot.nick, nwho);
		    return;
		  }
	       
		  sprintf(my, "*!*%s@%s", ircname, myhost);
		  collapse(level);
		  if(!bancmp(my, level) || !bancmp(level, my))
		    {
		      send_to_server(":%s MODE %s -b %s\n", bot.nick, rest, level);
		      return;
		    }
		  strcpy(test, mask);
		  testtime = strtok(test, " ");
		  if(!strchr(testtime, ':'))
		    {
		      if(strlen(level) > 100)  
			{
		     send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
			  return;
			}
		      if(strlen(mask) > 150) mask[149] = '\0';
		      if(tmp != NULL)
			bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, 0, mask, tmp->nick, 1);
		      else
			bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, 0, mask, admnwho->nick, 1);
		      if(bantest == 1) return;
		      send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, level);
		      write_single_banfile(rest);
		      return;
		    }
		  if(reason = strchr(mask, ' '))
		    {
		      *reason++ = '\0';
		      if(reason[0] == ' ' || mask[0] == ' ')
			{
		     send_to_server(":%s NOTICE %s :Trop d'espaces dans la commande\n",bot.nick, nwho);
			  return;
			}
		      if(!strchr(mask, ':'))
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(*mask == ':')
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(strlen(level) > 100)  
			{
		     send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
			  return;
			}
		      if(strlen(reason) > 150) reason[149] = '\0';
		      duration = strtok(mask, ":");
		      units = strtok(NULL, " ");
		      if(units == NULL)
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(strlen(duration) > 10 && strlen(units) > 10)
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(is_num(duration))
			{
			  newtime = convert_duration(units, atol(duration));
			  if(time(NULL) + newtime > 999999999 || newtime < 0)
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			      return;
			    }
			  if(newtime == 0)
			    {
			      if(tmp != NULL)
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, 0, reason, tmp->nick, 1);
			      else
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, 0, reason, admnwho->nick, 1);
			    }
			  else
			    {
			      if(tmp != NULL)
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, newtime, reason, tmp->nick, 1);
			      else
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, newtime, reason, admnwho->nick, 1);
			    }
			  if(bantest == 1) return;
			  send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, level);
			  write_single_banfile(rest);
			  return;
			}
		      else
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		    }
		  else
		    {
		      if(!strchr(mask, ':'))
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(*mask == ':')
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      duration = strtok(mask, ":");
		      units = strtok(NULL, " ");
		      if(units == NULL)
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(strlen(duration) > 10 && strlen(units) > 10)
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		      if(strlen(level) > 100)  
			{
		     send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
			  return;
			}
		      if(is_num(duration))
			{
			  newtime = convert_duration(units, atol(duration));
			  if(time(NULL) + newtime > 999999999 || newtime < 0)
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			      return;
			    }
			  if(newtime == 0)
			    {
			      if(tmp != NULL)
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, 0, reason1, tmp->nick, 1);
			      else
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, 0, reason1, admnwho->nick, 1);
			    }
			  else
			    {
			      if(tmp != NULL)
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, newtime, reason1,tmp->nick, 1);
			      else
				bantest = add_baninfo(rest, level, time(NULL), regs->banlvl, newtime, reason1, admnwho->nick, 1);
			    }
			  if(bantest == 1) return;
			  send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, level);
			  write_single_banfile(rest);
			  return;
			}
		      else
			{
		     send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			  return;
			}
		    }
		}      
	      else
		{
	       send_to_server(":%s NOTICE %s : Masque spécifié invalide\n", bot.nick, nwho);
		  return;
		}
	    }
	  else
	    {
	      if(!is_num(level))
		{
	       send_to_server(":%s NOTICE %s : Masque ou Level spécifié invalide\n", bot.nick, nwho);
		  return;
		}
	      if(timeout = strchr(mask, ' '))
		{
		  *timeout++ = '\0';
		  if(timeout[0] == ' ' || mask[0] == ' ')
		    {
		  send_to_server(":%s NOTICE %s :Trop d'espace dans la commande\n",bot.nick, nwho);
		      return;
		    }
		  if(!strcmp(mask, "*!*@*") || !strcmp(mask, "*!*@*.*") || !strcmp(mask, "*!*@*.*.*"))
		    {
		  send_to_server(":%s NOTICE %s :Veuillez entrer un hostname plus spécifique.\n", bot.nick, nwho);
		      return;
		    }
		  usertmp = return_userinfo(mask, rest);
		  adminban = return_userinfo(mask, csadminchan);
		  if(adminban != NULL && adminban->protect)
		    {
		      if(admnwho != NULL && adminban->level > admnwho->level)
			{
		     send_to_server(":%s NOTICE %s : Ce masque représente un Admin avec un level plus élevé que le votre\n", bot.nick, nwho);
			  return;
			}
		    } 
		  else if((usertmp != NULL && usertmp->protect) || (usertmp != NULL && usertmp->level == maxuserlevel))
		    {
		      if(tmp != NULL && admn == NULL && usertmp->level > tmp->level)
			{
		     send_to_server(":%s NOTICE %s : Ce masque représente un utilisateur protégé avec un level plus élevé que le votre\n", bot.nick, nwho);
			  return;
			}
		    } 
		  sprintf(my, "*!*%s@%s", ircname, myhost);
		  collapse(mask);
		  if(!bancmp(my, mask) || !bancmp(level, my))
		    {
		      send_to_server(":%s MODE %s -b %s\n", bot.nick, rest, mask);
		      return;
		    }
		  if(tmp != NULL && tmp->level < atoi(level))
		    {
		  send_to_server(":%s NOTICE %s : Vous ne pouvez pas bannir à un level plus élevé que le votre\n", bot.nick, nwho);
		      return;
		    }
		  /* STE */
		  if( admnwho != NULL && admnwho->level < atoi(level))
		    {
		  send_to_server(":%s NOTICE %s : Vous ne pouvez pas bannir à un level plus élevé que le votre\n", bot.nick, nwho);
		      return;
		    }
		  if(!strchr(mask, '!') && !strchr(mask, '@'))
		    {
		  send_to_server(":%s NOTICE %s : Masque spécifié invalide\n", bot.nick, nwho);
		      return;
		    }
		  else
		    {
		      strcpy(test, timeout);
		      timetest = strtok(test, " ");
		      if((reason = strchr(timeout, ' ')) && strchr(timetest, ':'))
			{
			  *reason++ = '\0';
			  if(timeout[0] == ' ' || reason[0] == ' ')
			    {
			send_to_server(":%s NOTICE %s :Trop d'espaces dans la commande\n",bot.nick, nwho);
			      return;
			    }
			  if(!is_num(level))
			    {
			send_to_server(":%s NOTICE %s :Le LEVEL entré est incorrect.  Ce doit être une valeur numérique\n", bot.nick, nwho);
			      return;
			    }
			  if(strlen(mask) > 100)  
			    {
			send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
			      return;
			    }
			  if(strlen(reason) > 150) reason[149] = '\0';
			  strcpy(test, timeout);
			  duration = strtok(test, " ");
			  if(!strchr(duration, ':'))
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			      return;
			    }
			  if(*timeout == ':')
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			      return;
			    }
			  duration = strtok(timeout, ":");
			  units = strtok(NULL, " ");
			  if(units == NULL)
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			      return;
			    }
			  if(strlen(duration) > 10 || strlen(units) > 10)
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
			      return;
			    }
			  if(strlen(mask) > 100)  
			    {
			send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
			      return;
			    }
			  if(is_num(duration))
			    {
			      newtime = convert_duration(units, atol(duration));
			      if(time(NULL) + newtime > 999999999 || newtime < 0)
				{
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
				  return;
				}
			      if(newtime == 0)
				{
				  if(tmp != NULL)
				    bantest = add_baninfo(rest, mask, time(NULL), atoi(level), 0, reason, tmp->nick, 1);
				  else
				    bantest = add_baninfo(rest, mask, time(NULL), atoi(level), 0, reason, admnwho->nick, 1);
				}
			      else
				{
				  if(tmp != NULL)
				    bantest = add_baninfo(rest, mask, time(NULL), atoi(level), newtime, reason, tmp->nick, 1);
				  else
				    bantest = add_baninfo(rest, mask, time(NULL), atoi(level), newtime, reason, admnwho->nick, 1);
				}
			      if(bantest == 1) return;
			      send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, mask);
			      write_single_banfile(rest);
			      return;
			    }
			  else
			    {
			send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);			      return;
	     	    }
			}
		      else
			{
			  if(!strchr(timeout, ':'))
			    {
			      if(strlen(mask) > 100)  
				{
			send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
				  return;
				}
			      if(strlen(timeout) > 150) reason[149] = '\0';
			      if(tmp != NULL)
				bantest = add_baninfo(rest, mask, time(NULL), atoi(level), 0, timeout, tmp->nick, 1);
			      else
				bantest = add_baninfo(rest, mask, time(NULL), atoi(level), 0, timeout, admnwho->nick, 1);
			      if(bantest == 1) return;
			      send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, mask);
			      write_single_banfile(rest);
			      return;
			    }
			  else
			    {
			      strcpy(test, timeout);
			      duration = strtok(test, " ");
			      if(!strchr(duration, ':'))
				{
			   send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
				  return;
				}
			      if(*timeout == ':')
				{
			   send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
				  return;
				}
			      duration = strtok(timeout, ":");
			      units = strtok(NULL, " ");
			      if(units == NULL)
				{
			   send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
				  return;
				}
			      if(strlen(duration) > 10 && strlen(units) > 10)
				{
			   send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
				  return;
				}
			      if(strlen(mask) > 100)  
				{
			   send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
				  return;
				}
			      if(is_num(duration))
				{
				  newtime = convert_duration(units, atol(duration));
				  if(time(NULL) + newtime > 999999999 || newtime < 0)
				    {
				      send_to_server(":%s NOTICE %s : Invalid Time Specified\n", bot.nick, nwho);
				      return;
				    }
				  if(newtime == 0)
				    {
				      if(tmp != NULL)
					bantest = add_baninfo(rest, mask, time(NULL), atoi(level), 0, reason1, tmp->nick, 1);
				      else
					bantest = add_baninfo(rest, mask, time(NULL), atoi(level), 0, reason1, admnwho->nick, 1);
				    }
				  else
				    {
				      if(tmp != NULL)
					bantest = add_baninfo(rest, mask, time(NULL), atoi(level), newtime, reason1, tmp->nick, 1);
				      else
					bantest = add_baninfo(rest, mask, time(NULL), atoi(level), newtime, reason1, admnwho->nick, 1);
				    }
				  if(bantest == 1) return;
				  send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, mask);
				  write_single_banfile(rest);
				  return;
				}
			    }
			}
		    }
		}
	      else
		{
		  sprintf(my, "*!*%s@%s", ircname, myhost);
		  collapse(mask);
		  if(!bancmp(my, mask) || !bancmp(level, my))
		    {
		      send_to_server(":%s MODE %s -b %s\n", bot.nick, rest, mask);
		      return;
		    }

		  /* STE */
		  if(tmp != NULL && tmp->level < atoi(level))
		    {
		  send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
		      return;
		    }
		  /* STE */
		  if( admnwho != NULL && admnwho->level < atoi(level))
		    {
	       send_to_server(":%s NOTICE %s : Ce masque représente un Admin avec un level plus élevé que le votre\n", bot.nick, nwho);
		      return;
		    }

		  if(strlen(mask) > 100)  
		    {
		  send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
		      return;
		    }
		  if(tmp != NULL)
		    add_baninfo(rest, mask, time(NULL), atoi(level), 0, reason1, tmp->nick, 1);
		  else
		    add_baninfo(rest, mask, time(NULL), atoi(level), 0, reason1, admnwho->nick, 1);
		  if(bantest == 1) return;
		  send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, mask);
		  write_single_banfile(rest);
		  return;
		}
	    }
	}
      else
	{
	  if(!strchr(level, '@') || !strchr(level, '!'))
	    {
	      send_to_server(":%s NOTICE %s : Hostmasque invalide\n", bot.nick, nwho);
	      return;
	    }
	  if(!strcmp(level, "*!*@*") || !strcmp(level, "*!*@*.*") || !strcmp(level, "*!*@*.*.*"))
	    {
	      send_to_server(":%s NOTICE %s :Veuillez entrer un hostmasque plus specifique.\n", bot.nick, nwho);
	      return;
	    }
	  usertmp = return_userinfo(level, rest);
	  adminban = return_userinfo(level, csadminchan);
	  if(adminban != NULL && adminban->protect)
	    {
	      if(admnwho != NULL && adminban->level > admnwho->level)
		{
	       send_to_server(":%s NOTICE %s : Ce masque représente un utilisateur protégé avec un level plus élevé que le votre\n", bot.nick, nwho);
		  return;
		}
	    } 
	  else if((usertmp != NULL && usertmp->protect) || (usertmp != NULL && usertmp->level == maxuserlevel))
	    {
	      if(tmp != NULL && admn == NULL && usertmp->level > tmp->level)
		{
	       send_to_server(":%s NOTICE %s : Ce masque représente un utilisateur protégé avec un level plus élevé que le votre\n", bot.nick, nwho);
		  return;
		}
	    } 

	  /* STE */
	  sprintf(my, "%s!*@%s", bot.nick,bot.server);
	  if(!bancmp(level,my) || !bancmp(my,level)) {
	    send_to_server(":%s NOTICE %s : That mask matches me\n", bot.nick, nwho);
	    return;
	  }
	 

	  sprintf(my, "*!*%s@%s", ircname, myhost);
	  collapse(level);
	  if(!bancmp(my, level) || !bancmp(level, my))
	    {
	      send_to_server(":%s MODE %s -b %s\n", bot.nick, rest, level);
	      return;
	    }
	  if(strlen(level) > 100)  
	    {
	    send_to_server(":%s NOTICE %s :Le masque de ban est trop court, essayez-en un plus spécifique\n",bot.nick, nwho);
	      return;
	    }
	  if(tmp != NULL)
            add_baninfo(rest, level, time(NULL), regs->banlvl, 0, reason1, tmp->nick, 1);
	  else
            add_baninfo(rest, level, time(NULL), regs->banlvl, 0, reason1, admnwho->nick, 1);
	  if(bantest == 1) return;
	  send_to_server(":%s MODE %s +b %s\n", bot.nick, rest, level);
	  write_single_banfile(rest);
	  return;
	}
    }
  else
    {
      helpname = get_command_name("ban");
      do_help(who, helpname);
    }
}

long convert_duration(char *unit, long duration)
{
   long newtime;
   if(!strcasecmp(unit, "secondes") || !strcasecmp(unit, "seconde"))
   {
      return duration;
   }
   if(!strcasecmp(unit, "minutes") || !strcasecmp(unit, "minute"))
   {
      newtime = duration * 60;
      return newtime;
   }
   if(!strcasecmp(unit, "heures") || !strcasecmp(unit, "houre"))
   {
      newtime = duration * 60 * 60;
      return newtime;
   }
   if(!strcasecmp(unit, "jours") || !strcasecmp(unit, "jour"))
   {
      newtime = duration * 60 * 60 * 24;
      return newtime;
   }
   if(!strcasecmp(unit, "semaines") || !strcasecmp(unit, "semaine"))
   {
      newtime = duration * 60 * 60 * 24 * 7;
      return newtime;
   }
   if(!strcasecmp(unit, "mois") || !strcasecmp(unit, "mois"))
   {
      newtime = duration * 60 * 60 * 24 * 30;
      return newtime;
   }
   return 0;
}

void show_bans(char *who, char *rest)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   char *masktmp, *mask, *chan, *nwho, createtime[100], endtime[20], nick2[50], reason2[512], test[10000];
   long temptime, tmptime;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   createtime[0] = '\0';
   endtime[0] = '\0';
   nwho = nick2num(who);
   if(masktmp = strchr(rest, ' '))
   {
      *masktmp++ = '\0';
      mask = strtok(masktmp, " ");
      chan = rest;
      if(chan_check(chan, who) == -1) return;
      if(!strchr(mask, '!') || !strchr(mask, '@'))
      {
	 send_to_server(":%s NOTICE %s :Le masque doit être selon la forme nick!user@host\n", bot.nick, nwho, reason2);
          return;
      }
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            ban = regs->banhead;
            while(ban != NULL)
            {
               if(!bancmp(mask, ban->ban) || !bancmp(ban->ban, mask))
               {
                  reason2[0] = '\0';
                  temptime = ban->timeset;
                  if(ban->duration == 0)
                     strcpy(endtime, "NEVER");
                  else
                  {
                     tmptime = ban->timeset + ban->duration;
                     strcpy(endtime, get_time(tmptime));
                  }
                  if(ban->reason == NULL || *ban->reason == '\0' || *ban->reason == ' ')
                    strcpy(reason2, "N/A");
                  else
                    strcpy(reason2, ban->reason);
                  if(ban->nick == NULL || *ban->nick == '\0')
                    strcpy(nick2, "N/A");
                  else
                    strcpy(nick2, ban->nick);
                  send_to_server(":%s NOTICE %s :\0031 Masque:\003\00312 %s\003\0031   mis part:\003\0034 %s\003\0031  Crée le:\003\0034 %s\003\n", bot.nick, nwho, ban->ban, nick2, get_time(ban->timeset));
                  send_to_server(":%s NOTICE %s :\0031       Level de suppression: [\003\0034 %d\003\0031 ]  Timeout:\003\0034 %s\003\n", bot.nick, nwho, ban->level, endtime);
                  if(!ban->botban)
                     send_to_server(":%s NOTICE %s :\0031       Status: \003\0034MODE  \003 \0031 Reason:\003\0034  %s\003\n", bot.nick, nwho, reason2);
                  else
                     send_to_server(":%s NOTICE %s :\0031       Status: \003\0034BOTBAN\003 \0031 Reason:\003\0034  %s\003\n", bot.nick, nwho, reason2);
               }
               ban = ban->next;
            }
            return;
         }
         regs = regs->next;
      }
   }
   else
   {
      chan = rest;
      if(chan_check(chan, who) == -1) return;
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            ban = regs->banhead;
            while(ban != NULL)
            {
               reason2[0] = '\0';
               temptime = ban->timeset;
               if(ban->duration == 0)
                  strcpy(endtime, "NEVER");
               else
               {
                  tmptime = ban->timeset + ban->duration;
                  strcpy(endtime, get_time(tmptime));
               }
               if(ban->reason == NULL || *ban->reason == '\0' || *ban->reason == ' ')
                 strcpy(reason2, "N/A");
               else
                 strcpy(reason2, ban->reason);
               if(ban->nick == NULL || *ban->nick == '\0')
                 strcpy(nick2, "N/A");
               else
                 strcpy(nick2, ban->nick);
               send_to_server(":%s NOTICE %s :\0031 Mask:\003\00312 %s\003\0031   mis par:\003\0034 %s\003\0031  Crée le:\003\0034%s\003\n", bot.nick, nwho, ban->ban, nick2, get_time(ban->timeset));
               send_to_server(":%s NOTICE %s :\0031       Level de suppression: [\003\0034 %d\003\0031 ]  Timeout:\003\0034 %s\003\n", bot.nick, nwho, ban->level, endtime);
               if(!ban->botban)
                  send_to_server(":%s NOTICE %s :\0031       Status: \003\0034MODE  \003 \0031 Reason:\003\0034  %s\003\n", bot.nick, nwho, reason2);
               else
                  send_to_server(":%s NOTICE %s :\0031       Status: \003\0034BOTBAN\003 \0031 Reason:\003\0034  %s\003\n", bot.nick, nwho, reason2);
               ban = ban->next;
            }
            return;
         }
         regs = regs->next;
      }
   }
}
