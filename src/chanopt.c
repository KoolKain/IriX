/* CS5 - Channel Services src/chanopt.c
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

int check_level(char *who, char *cmd)
{
    int cmdlvl = 0;
    cmdlvl = get_command_level(cmd);
    if(cmdlvl > added_level)
    {
       send_denied_msg(who);
       return 0;
    }
    return 1;
}

/* STE */
int checkAdminLevel(char *who, char *cmd)
{
    int cmdlvl = 0;
    cmdlvl = get_command_level(cmd);
    if(cmdlvl > added_level || added_level <= maxuserlevel)
    {
       send_denied_msg(who);
       return 0;
    }
    return 1;
}

int check_level_help(char *who, char *cmd, char *rest)
{
    int cmdlvl = 0;
    char newhelp[1000], *helpname;

    newhelp[0] = '\0';
    cmdlvl = get_command_level(cmd);
    if(cmdlvl > added_level)
    {
       send_denied_msg(who);
       return 0;
    }
    helpname = get_command_name("chanopt");
    if(rest == NULL)
    {
       sprintf(newhelp, "%s %s", helpname, cmd);
       strlwr(newhelp);
       do_help(who, newhelp);
       return 0;
    }
    return 1;
}

void channel_options(char *who, char *rest)
{
   char *ptr = NULL, *ptr1 = NULL, test[10000], newrest[10000], *chan = NULL;
   char *nwho = NULL, *helpname;
   
   test[0]='\0';newrest[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   helpname = get_command_name("chanopt");
   if(ptr == NULL || ptr[0] == '\0')
   {
      do_help(who, helpname);
      return;
   }
   if(ptr1 = strchr(ptr, ' ')) *ptr1++ = '\0';
   newrest[0] = '\0';
   if(!strcasecmp(ptr, "ownername"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_ownername(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "description"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_desc(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "chmodeslevel"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_chmodelvl(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "banlevel"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_banlevel(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "ownernick"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_ownernick(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "owneremail"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_owneremail(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "strictop"))
   {
      if(!check_level(who, ptr)) return;
      set_strictop(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "noban"))
   {
      if(!check_level(who, ptr)) return;
      set_noban(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "operop"))
   {
      if(!check_level(who, ptr)) return;
      set_operop(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "noops"))
   {
      if(!check_level(who, ptr)) return;
      set_noops(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "stay"))
   {
      if(!checkAdminLevel(who, ptr)) return; /* STE */
      set_stay(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "defmodes"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_default_modes(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "deftopic"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_default_topic(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "locktopic"))
   {
      if(!check_level(who, ptr)) return;
      set_tlock(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "motd"))
   {
      if(!check_level(who, ptr)) return;
      if(ptr1 != NULL)
         sprintf(newrest, "%s %s", rest, ptr1);
      else
         sprintf(newrest, "%s", rest);
      make_motd(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "welcome"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      make_welcome(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "setwelcome"))
   {
      if(!check_level(who, ptr)) return;
      set_welcome(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "banmove"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_banmove(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "banremove"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_banremove(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "bantimeout"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_bantimeout(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "chanurl"))
   {
      if(!check_level_help(who, ptr, ptr1)) return;
      sprintf(newrest, "%s %s", rest, ptr1);
      set_url(who, newrest);
      return;
   }
   if(!strcasecmp(ptr, "setvoice"))
   {
      if(!check_level(who, ptr)) return;
      set_voice(who, rest);
      return;
   }
   if(!strcasecmp(ptr, "autoops"))
   {
      if(!check_level(who, ptr)) return;
      set_autoops(who, rest);
      return;
   }
   send_to_server(":%s NOTICE %s :Cette option n'existe pas\n", bot.nick, nwho);
   return;
}
