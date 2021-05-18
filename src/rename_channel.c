/* CS5 - Channel Services src/rename_channel.c
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

void renchan(char *who, char *rest)
{
  struct regsinfo *regs;
  char chanopt[10000], chanopt1[10000], chanusr[10000], chanusr1[10000], chaninfo[10000], chaninfo1[10000], *helpname;
  char *newchan, *oldchan, *chan1, test1[10000], *nwho, *usernum, *ochan, *nchan, chanban[10000], chanban1[10000];

  nwho = nick2num(who);
  oldchan = strtok(rest, " ");
  strlwr(oldchan);
  if(chan_check(oldchan, who) == -1) return;
  if(check(oldchan, who) == -1) return;
  newchan = strtok(NULL, " ");
  helpname = get_command_name("renchan");
  if(newchan == NULL)
  {
     do_help(who, helpname);
     return;
  }
  strlwr(newchan);
  strcpy(test1, newchan);
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
  if(chan_check(newchan, who) == -1) return;
  if(check_chan(newchan))
  {
      send_to_server(":%s NOTICE %s :Le salon %s est déjà enregistré.\n", bot.nick, nwho, newchan);
     return;
  }
  ochan=conv_char(oldchan);
  nchan=conv_char(newchan);
  regs = regshead;
  while(regs != NULL)
  {
     if(!strcasecmp(regs->channel, oldchan))
     {
        strcpy(regs->channel, newchan);
        break;
     }
     regs = regs->next;
  }
  usernum=get_usernum(bot.nick);
  sprintf(chanopt, "database/chanopt/%s", oldchan);
  sprintf(chanopt1, "database/chanopt/%s", newchan);
  sprintf(chanusr, "database/chanusr/%s", oldchan);
  sprintf(chanusr1, "database/chanusr/%s", newchan);
  sprintf(chaninfo, "database/chaninfo/%s", oldchan);
  sprintf(chaninfo1, "database/chaninfo/%s", newchan);
  sprintf(chanban, "database/chanban/%s", oldchan);
  sprintf(chanban1, "database/chanban/%s", newchan);
  send_to_server(":%s PART %s\n", bot.nick, oldchan);
  delete_cnode(oldchan, bot.nick);
  rename(chanopt, chanopt1);
  rename(chanusr, chanusr1);
  rename(chaninfo, chaninfo1);
  rename(chanban, chanban1);
  send_to_server(":%s JOIN %s\n", bot.nick, newchan);
  add_joininfo(newchan, usernum, TRUE);
  send_to_server(":%s MODE %s +o %s\n", myserver, newchan, usernum);
  send_to_server(":%s TOPIC %s\n", bot.nick, newchan);
  send_to_server(":%s MODE %s\n", bot.nick, newchan);
}
