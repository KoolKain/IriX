/* CS5 - Channel Services src/server.c
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

int get_connection(char *hostname, int port)
{
   struct sockaddr_in sa;
   struct hostent *hp;
   int sckfd;
   if((hp = gethostbyname(hostname)) == NULL) 
   {
      errno = ECONNREFUSED;
      return(-1);
   }
   bzero(&sa, sizeof(sa));
   bcopy(hp->h_addr, (char *) &sa.sin_addr, hp->h_length);
   sa.sin_family = hp->h_addrtype;
   sa.sin_port = htons((u_short) port);
   if((sckfd = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0) return(-1);
   setsockopt(sckfd, SOL_SOCKET, SO_LINGER, 0, 0);
   setsockopt(sckfd, SOL_SOCKET, SO_REUSEADDR, 0, 0);
   setsockopt(sckfd, SOL_SOCKET, SO_KEEPALIVE, 0, 0);
   if(connect(sckfd, (struct sockaddr *) &sa, sizeof(sa)) < 0) 
   {
      close(sckfd);
      return(-1);
   }
   return(sckfd);
}

int readln()
{
   char ch, *botnum;
   int i = 0;
   do 
   {
      if(read(socketfd, &ch, 1) < 1) return(0);
      if(ch >= 32 || ch <= 126)
         if(i < MAXLEN - 1) line[i++] = ch;
   } while(ch != '\n');
   line[i] = '\0';
   if(strstr(line, "PRIVMSG")) return 1;
   if(logging)
      log(BOT_LOG, "R [%s] %s",date(), line);
#ifdef DEBUG
   printf("Read - %s", line);
#endif
   return 1;
}

int read_from_socket(int sock, char *buffer)
{
   char ch;
   int i = 0;
   do 
   {
      if(read(sock, &ch, 1) < 1) return(0);
      if(ch >= 32 || ch <= 126)
         if(i < MAXLEN - 1) buffer[i++] = ch;
   } while(ch != '\n');
   buffer[i] = '\0';
#ifdef DEBUG
   printf("Read from socket - %s\n", buffer);
#endif
   return 1;
}

int writeln(char *buffer)
{
   if(write(socketfd, buffer, strlen(buffer)) < 0) return(0);
   if(logging)
      log(BOT_LOG, "W [%s] %s",date(), buffer);
#ifdef DEBUG
   printf("Write - %s\n", buffer);
#endif
}

int writeln2(int sock, char *buffer)
{
   if(write(sock, buffer, strlen(buffer)) < 0) return(0);
   return 1;
}

void sendping()
{
   send_to_server("PING :%s\n", myserver);
   bot.server_active = FALSE;
}

int send_to_socket(int sck, char *format, ...)
{
   va_list arglist;
   char buffer[MAXBUF];
   va_start(arglist, format);
   vsprintf(buffer, format, arglist);
   va_end(arglist);
   return(writeln2(sck, buffer));
}

void init_bot()
{
   int mypid;
   FILE *out;
   strcpy(bot.nick, botnick);
   joined = FALSE;
   send_to_server("PASS %s :TS\n", netpass);
   send_to_server("CAPAB TS3\n");
   send_to_server("SERVER %s 1 :%s\n", myserver, reason);
   send_to_server("SVINFO 3 1 0 :%ld\n", time(NULL));
   mypid = getpid();
   if (out = fopen(pid_file, "wt"))
   {
       fprintf(out, "%i\n", mypid);
       fclose(out);
   }

}

void reinit()
{
   char *usernum;
   usernum = makenum(bot.nick);
   send_to_server(":%s WALLOPS :Re-initialising Channels Service\n", myserver);
   
//send_to_server("NICK %s 1 %ld %s %s %s %s 0 :%s\n", bot.nick, time(NULL), mymodes, ircuser, myhost, myserver, ircname);
send_to_server("NICK %s 1 %lu +oa %s %s %s 0 0 :%s\n", bot.nick, time(NULL), bot.nick, myhost, myserver, ircname);
//add_nickinfo(bot.nick, ircuser, myhost, usernum, myserver, reason);
add_nickinfo(bot.nick, bot.nick, myhost, bot.nick, myserver, ircname);
   send_to_server(":%s MODE %s %s\n", bot.nick, bot.nick, mymodes);
   join_msgchan();
   join_channels(); 
   return;
}

int reconnect()
{
   if(bot.server_active == FALSE) 
   {
      close(socketfd);
      return 0;
   }
   return 1;
}

void die(char *who, char *rest)
{
   char buff[300], *chan;
   FILE *fp;
   int i = 0;

   send_to_server(":%s PRIVMSG %s :Channels Services will now terminate.\n", bot.nick, msg_chan);
   send_to_server(":%s QUIT :%s\n", bot.nick, quit_msg);
   send_to_server(":%s SQUIT %s :Just keeps getting better and better !\n", myserver, myserver);
   write_all_files();
   exit(0);
}

