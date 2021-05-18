/* CS5 - Channel Services src/ch_command.c
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

void change_comm_level(char *who, char *rest)
{
   int i = 0;
   char *command, *level, *nwho, *helpname;
   
   nwho = nick2num(who);
   strlwr(rest);
   helpname = get_command_name("chlevel");
   if(rest == NULL)
   {
      do_help(who, helpname);
      return;
   }
   command = strtok(rest, " ");
   if(command == NULL)
   {
      do_help(who, helpname);
      return;
   }
   level = strtok(NULL, " ");
   if(level == NULL)
   {
      do_help(who, helpname);
      return;
   }
   if(!is_num(level))
   {
      do_help(who, helpname);
      return;
   }
   if(atoi(level) > maxadmnlevel)
   {
      send_to_server(":%s NOTICE %s : Le level est hors de portée: /msg %s <command> <level>\n", bot.nick, nwho, botnick);
      return;
   }
   for(i = 0; *commands[i].name; i++)
   {
      if(!strcasecmp(commands[i].name, command))
      {
         commands[i].level = atoi(level);
	 send_to_server(":%s NOTICE %s : Le level de la commande %s est à %s\n", bot.nick, nwho, command, level);
         write_comm_list();
         return;
      }
   }
   send_to_server(":%s NOTICE %s : La commande %s n'existe pas\n", bot.nick, nwho, command);
}

void change_comm_name(char *who, char *rest)
{
   int i = 0;
   char *command, *newcomm, *nwho, c1[50], c2[50], c3[1000], *chanopt, *moduser, *modadmin, *helpname;
   
   nwho = nick2num(who);
   strlwr(rest);
   helpname = get_command_name("chcomname");
   if(rest == NULL)
   {
      do_help(who, helpname);
      return;
   }
   command = strtok(rest, " ");
   if(command == NULL)
   {
      do_help(who, helpname);
      return;
   }
   newcomm = strtok(NULL, " ");
   if(newcomm == NULL)
   {
      do_help(who, helpname);
      return;
   }
   if(strlen(newcomm) > 16)
   {
      send_to_server(":%s NOTICE %s : Nouvelle commande trop longue, limitez-la à 15 caractères\n", bot.nick, nwho);
      return;
   }
   for(i = 0; *commands[i].name; i++)
   {
      if(!strcasecmp(commands[i].name, command))
      {
         chanopt = get_command_name("chanopt");
         modadmin = get_command_name("modadmin");
         moduser = get_command_name("moduser");
         c1[0] = '\0';
         sprintf(c1, "%s", strupr(command));
         c2[0] = '\0';
         sprintf(c2, "%s", strupr(newcomm));
         c3[0] = '\0';
         if(!strcasecmp(command, chanopt))
            sprintf(c3, "./chng_help.sh %s %s %s %s", c1, c2, chanopt, newcomm);
         else if(!strcasecmp(command, modadmin))
            sprintf(c3, "./chng_help.sh %s %s %s %s", c1, c2, modadmin, newcomm);
         else if(!strcasecmp(command, moduser))
            sprintf(c3, "./chng_help.sh %s %s %s %s", c1, c2, moduser, newcomm);
         else
            sprintf(c3, "./chng_help.sh %s %s", c1, c2);
         system(c3);
         strcpy(commands[i].name, newcomm);
	 send_to_server(":%s NOTICE %s : Le level de la commande %s a été changé à %s \n", bot.nick, nwho, command, newcomm);
         write_comm_list();
         return;
      }
   }
   send_to_server(":%s NOTICE %s : La commande %s n'existe pas\n", bot.nick, nwho, command);
}

void disablecomm(char *who, char *rest)
{
   int i = 0;
   char *command, *nwho;

   nwho = nick2num(who);
   strlwr(rest);
   if(rest == NULL)
   {
      return;
   }
   command = strtok(rest, " ");
   if(command == NULL)
   {
      return;
   }
   for(i = 0; *commands[i].name; i++)
   {
      if(!strcasecmp(commands[i].name, command))
      {
         if(!strcasecmp(commands[i].realname, "enablecom"))
         {
	    send_to_server(":%s NOTICE %s : Vous ne pouvez pas mettre hors de service %s\n", bot.nick, nwho, command);
            return;
         }
         commands[i].disable = TRUE;
	 send_to_server(":%s NOTICE %s : La commande %s a été mise hors service\n", bot.nick, nwho, command);
         write_comm_list();
         return;
      }
   }
   send_to_server(":%s NOTICE %s : La commande %s n'existe pas\n", bot.nick, nwho, command);
}

void enablecomm(char *who, char *rest)
{
   int i = 0;
   char *command, *nwho;

   nwho = nick2num(who);
   strlwr(rest);
   if(rest == NULL)
   {
      return;
   }
   command = strtok(rest, " ");
   if(command == NULL)
   {
      return;
   }
   for(i = 0; *commands[i].name; i++)
   {
      if(!strcasecmp(commands[i].name, command))
      {
         commands[i].disable = FALSE;
	 send_to_server(":%s NOTICE %s : La commande %s a été remise en service\n", bot.nick, nwho, command);
         write_comm_list();
         return;
      }
   }
   send_to_server(":%s NOTICE %s : La commande %s n'existe pas\n", bot.nick, nwho, command);
}

void showdisabled(char *who, char *rest)
{
   int i = 0, got_one = 0;
   char *nwho;

   nwho = nick2num(who);
   for(i = 0; *commands[i].name; i++)
   {
      if(commands[i].disable && !commands[i].chanopt)
      {
         send_to_server(":%s NOTICE %s : %s\n", bot.nick, nwho, commands[i].name);
         got_one = TRUE;
      }
      else if(commands[i].disable && commands[i].chanopt)
      {
	 send_to_server(":%s NOTICE %s : Options du salon %s\n", bot.nick, nwho, commands[i].name);
         got_one = TRUE;
      }
   }
   if(!got_one)
      send_to_server(":%s NOTICE %s : Aucune commande n'est hors service\n", bot.nick, nwho);
}
