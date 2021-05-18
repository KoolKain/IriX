/* CS5 - Channel Services src/rehash.c
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

void rehash()
{
    int i;
    char *botnum;

    botnum=get_usernum(bot.nick);
    write_all_files();
    delete_usernum(botnum);
    free_hublist();
    free_nicklist();
    send_to_server(":%s WALLOPS :Re-initializing Channel Service\n", myserver);
    send_to_server(":%s PRIVMSG %s :Re-initializing Channel Services.\n", bot.nick, msg_chan);
    send_to_server(":%s QUIT :%s\n", bot.nick, quit_msg);
    send_to_server(":%s SQUIT %s :Just keeps getting better and better !\n", myserver, myserver);
    close(socketfd);
    if((i = get_connection(bot.server, bot.port)) != -1)
    {
       log(LOG_FILE, "[%s] Rehashing %s\n", date(), bot.nick);
       socketfd = i;
       init_bot();
    }
}
