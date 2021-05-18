/* CS5 - Channel Services src/chan_purge.c
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

void check_channel_purge_time()
{
   int found = 0;
   struct regsinfo *current;
   char chan[201];
   chan[0]='\0';
   current = regshead;
   while(current != NULL) 
   {
      strcpy(chan, current->channel);
      if(check_purge_flag(chan) != 1) 
      { 
         if((time(NULL) - current->lastseen) >= purge_time && strcasecmp(current->channel, csadminchan))
         {
	    log(PURGE_LOG, "[%s] Le salon %s est purgé, le propriétaire ne sera pas présent pendant %d jours\n", date(), chan, (((purge_time/60)/60)/24));
            send_to_server(":%s PART %s\n", bot.nick, chan);
	    send_to_server(":%s WALLOPS :Salon %s purgé - le propriétaire ne sera pas présent pendant %d jours\n", botnick, chan, (((purge_time/60)/60)/24));
            found = 1;
         }
      }
      current = current->next;
      if(found)
      {
         purge_channel(chan);
         delete_channel_node(chan);
         delete_cnode(chan, bot.nick);
         found = 0;
      }
   }
   write_regs_list();
}

void purge_channel(char *chan)
{
   char fname[300], *nchan = NULL;
   fname[0]='\0';
   strlwr(chan);
   purge_auth(chan);
   nchan = conv_char(chan);
   sprintf(fname, "database/chanopt/%s", nchan);
   unlink(fname);
   sprintf(fname, "database/chanusr/%s", nchan);
   unlink(fname);
   sprintf(fname, "database/chaninfo/%s", nchan);
   unlink(fname);
   sprintf(fname, "database/chanban/%s", nchan);
   unlink(fname);
}

void admin_purge_channel(char *who, char *rest)
{
   char *ptr = NULL, *chan = NULL, *nwho = NULL, test[10000];
   char *nchan = NULL, *helpname;

   test[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if(!strcasecmp(csadminchan, chan)) return;
   helpname = get_command_name("delchan");
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else 
   {
      do_help(who, helpname);
      return;
   }
   purge_channel(chan);
   delete_channel_node(chan);
   write_regs_list();
   send_to_server(":%s TOPIC %s :Salon purgé par %s ~ Raison : %s\n", botnick, chan, who, ptr);
   send_to_server(":%s NOTICE %s :Salon %s purgé par %s raison -  %s\n", botnick, nwho, chan, who, ptr);
   log(PURGE_LOG, "[%s] Salon %s purgé, par %s raison -  %s\n", date(), chan, who, ptr);
   send_to_server(":%s PART %s\n", bot.nick, chan);
   delete_cnode(chan, bot.nick);
}
