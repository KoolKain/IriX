/* CS5 - Channel Services src/del_admin.c
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

void delete_admin(char *who, char *rest)
{
   struct userinfo *tmp;
   char *nwho = NULL, *chan = NULL, *nick = NULL, test[10000], *helpname;

   test[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   nick = strtok(rest, " ");
   helpname = get_command_name("deladmin");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   tmp = ret_user_nickinfo(nick, csadminchan);
   if(tmp == NULL) 
   {
      send_to_server(":%s NOTICE %s :Ce pseudo n'est pas dans la liste des CSA.\n", bot.nick, nwho);
      return;
   }
   if(added_level <= tmp->level) 
   {
      send_to_server(":%s NOTICE %s :Vous ne pouvez pas supprimer un Admin avec un level plus élevé ou égal au votre.\n", bot.nick, nwho);
      return;
   }
   delete_unode(csadminchan, nick);
   send_to_server(":%s NOTICE %s :CSA %s supprimé\n", bot.nick, nwho, nick);
   write_single_channel(csadminchan);
}
