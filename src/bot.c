/* CS5 - Channel Services src/bot.c
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


#define DEF_GLOBALS
#include "bot.h"
#undef DEF_GLOBALS

void sig_alrm()
{
   char *botnum;
   int i;
   alarmed = 1;
   alarm(ALARMINTERVAL);
   reset_flood();
   if(!reconnect()) 
   {
      trk_servers++;
      if(trk_servers >= no_servers) trk_servers = 0;
      strcpy(bot.server, servers[trk_servers].name);
      bot.port = servers[trk_servers].port;
      if((i = get_connection(bot.server, bot.port)) != -1) 
      {

         botnum=get_usernum(bot.nick);
         delete_usernum(botnum);
         free_regslist();
         free_hublist();
         free_nicklist();
         log(LOG_FILE, "[%s] Reconnection sur %s\n", date(), bot.server);
         socketfd = i;
         init_bot();
      }
   }

   if((time(NULL) - pingtime) > PINGINTERVAL) 
   {
      sendping();
      pingtime = time(NULL);
   }
   if((time(NULL) - purgetime) > PURGEINTERVAL) 
   {
      check_channel_purge_time();
      purgetime = time(NULL);
      delete_all_mail();
   }
   if((time(NULL) - checkban) > CHECKBANINTERVAL)
   {
      check_ban_times();
      checkban = time(NULL);
   }
   signal(SIGALRM, sig_alrm);
}

void sig_kill()
{
   log(ERRLOG_FILE, "[%s] SIG KILL\n", date());
      write_all_files();
   exit(0);
}

void sig_term()
{
   log(ERRLOG_FILE, "[%s] SIG TERM\n", date());
      write_all_files();
   exit(0);
}

void sig_hup()
{
   log(ERRLOG_FILE, "[%s] SIG HUP\n", date());
   rehash();
}

void sig_segv()
{
   log(ERRLOG_FILE, "[%s] SIG SEGV\n", date());
   exit(0);
}

void sig_pipe()
{
   log(ERRLOG_FILE, "[%s] SIG PIPE\n", date());
   signal(SIGPIPE, sig_pipe);
}

void sig_bus()
{
   log(ERRLOG_FILE, "[%s] SIG BUS\n", date());
   signal(SIGBUS, sig_bus);
}

main(int argc, char **argv)
{
   struct timeval timeout;
   struct stat stbuf;
   fd_set fdvar;
   srand(time(NULL));
   botstart = lasttime = uptime = savetime = pingtime = purgetime = checkban = time(NULL);
   /*signal(SIGSEGV, sig_segv);*/
   signal(SIGHUP, sig_hup);
   signal(SIGBUS, sig_bus);
   signal(SIGPIPE, sig_pipe);
   signal(SIGALRM, sig_alrm);
   signal(SIGTERM, sig_term);
   signal(SIGKILL, sig_kill);
   if(fork()) exit(0);
   load_cfg();
   stat(mailprog, &stbuf);
   if(!S_ISREG(stbuf.st_mode))
   {
      printf("\nCan Not Loacate sendmail in specified location [%s]\n", mailprog);
      exit(0);
   }
   load_channels();
   set_topicnumber();
   trk_topics = rand() % no_topics;
   load_nums();
   switch(argc) 
   {
      case 1: socketfd = get_connection(bot.server, bot.port);
              if(socketfd == -1) exit(0);
              break;
      case 2: socketfd = get_connection(argv[1], bot.port);
              if(socketfd == -1) exit(0);
              strcpy(bot.server, argv[1]);
              break;
      case 3: socketfd = get_connection(argv[1], atoi(argv[2]));
              if(socketfd == -1) exit(0);
              strcpy(bot.server, argv[1]);
              break;
   }
   init_bot();

   while(1) 
   {
     timeout.tv_sec = WAITSEC;
     timeout.tv_usec = UWAITSEC;
     FD_ZERO(&fdvar);
     FD_SET(socketfd, &fdvar);
     switch(select(NFDBITS, &fdvar, (fd_set *) 0, (fd_set *) 0, &timeout)) 
     {
       case  0: break;
       case -1: 
                if(!alarmed) 
                {
                  sleep(RECHECK);
                  sig_alrm();
                }
                else alarmed = 0;
                break;
       default: 
                parse_server_msg(&fdvar);
                break;
      }
   }
}
