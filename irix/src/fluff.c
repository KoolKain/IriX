/* CS5 - Channel Services src/fluff.c
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

void parse_admin(char *who, char *rest)
{
   if(check_flood(who)) return;
   trailing_blanks(rest);
   send_to_server(":%s 256 %s :Informations administratives sur %s\n", myserver, who, myserver);
   send_to_server(":%s 257 %s :CService Entrechat\n", myserver, who);
   send_to_server(":%s 258 %s :Ce serice est gratuit\n", myserver, who);
   send_to_server(":%s 259 %s :Et est lancé par les Admins.\n", myserver, who);
}

void parse_stats(char *who, char *rest)
{
   if(check_flood(who)) return;
   trailing_blanks(rest);
   send_to_server(":%s 213 %s :Commande non-supportée sur ce serveur.\n", myserver, who);
   send_to_server(":%s 219 %s c :Fin du rapport de /STATS\n", myserver, who);
}

void parse_version(char *who, char *rest)
{
   if(check_flood(who)) return;
   trailing_blanks(rest);
   send_to_server(":%s 351 %s %s Veuillez taper /ctcp %s version\n", myserver, who, myserver, bot.nick);
}

