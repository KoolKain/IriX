/* CS5 - Channel Services src/load_cfg.c
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

void load_cfg()
{
   FILE *fp;
   char buffer[MAXDEF], *field;
   buffer[0]='\0';
   if((fp = fopen(CFG_FILE, "r")) == NULL) 
   {
      file_error(CFG_FILE);
      return;
   }
   while(fgets(buffer, MAXDEF, fp)) 
   {
      if(buffer[0] == '*' || buffer[0] == '\n') continue;
      strip_newline(buffer);
      trailing_blanks(buffer);
      if(field = strchr(buffer, ' ')) *field++ = '\0';
      if(!strcmp(buffer, "DEFAULT_SERVER")) 
      {
         strcpy(bot.server, field);
      }
      if(!strcmp(buffer, "DEFAULT_PORT")) 
      {
         bot.port = atoi(field);
      }
      if(!strcmp(buffer, "ACCESS_DENIED")) 
      {
         strcpy(denied_msg, field);
      }
      if(!strcmp(buffer, "FLOOD_RATE")) 
      {
         flood_rate = atoi(field);
      }
      if(!strcmp(buffer, "FLOOD_TIME")) 
      {
         flood_time = atoi(field);
      }
      if(!strcmp(buffer, "RESET_TIME")) 
      {
         reset_time = atoi(field);
      }
      if(!strcmp(buffer, "MSG_CHAN")) 
      {
         strcpy(msg_chan, field);
      }
      if(!strcmp(buffer, "NETPASS")) 
      {
         strcpy(netpass, field);
      }
      if(!strcmp(buffer, "MYSERVER")) 
      {
         strcpy(myserver, field);
      }
      if(!strcmp(buffer, "MYMODES")) 
      {
         strcpy(mymodes, field);
      }
      if(!strcmp(buffer, "REASON")) 
      {
         strcpy(reason, field);
      }
      if(!strcmp(buffer, "IRCNAME")) 
      {
         strcpy(ircname, field);
      }
      if(!strcmp(buffer, "IRCUSER")) 
      {
         strcpy(ircuser, field);
      }
      if(!strcmp(buffer, "MYHOST")) 
      {
         strcpy(myhost, field);
      }
      if(!strcmp(buffer, "BOTNICK")) 
      {
         strcpy(botnick, field);
      }
      if(!strcmp(buffer, "PID_FILE")) 
      {
         strcpy(pid_file, field);
      }
      if(!strcmp(buffer, "PURGE_TIME")) 
      {
         purge_time = atol(field);
      }
      if(!strcmp(buffer, "QUIT_MSG")) 
      {
         strcpy(quit_msg, field);
      }
      if(!strcmp(buffer, "GENDER_RPL")) 
      {
         strcpy(gender_rpl, field);
      }
      if(!strcmp(buffer, "OPERSERV_HOST")) 
      {
         strcpy(uworldhost, field);
      }
      if(!strcmp(buffer, "OPERSERV_NICK")) 
      {
         strcpy(uworldnick, field);
      }
      if(!strcmp(buffer, "CSADMINCHAN")) 
      {
         strcpy(csadminchan, field);
      }
      if(!strcmp(buffer, "SERVER_NUM"))
      {
         server_num=atoi(field);
      }
      if(!strcmp(buffer, "MAXUSERLEVEL"))
      {
         maxuserlevel=atoi(field);
      }
      if(!strcmp(buffer, "MAXADMNLEVEL"))
      {
         maxadmnlevel=atoi(field);
      }
      if(!strcmp(buffer, "COMCHAR")) 
      {
         comchar =  *field;
      }
      if(!strcmp(buffer, "CSERV_EMAIL")) 
      {
         strcpy(cserv_email, field);
      }
      if(!strcmp(buffer, "AUTOREGISTER"))
      {
         autoregister=atoi(field);
      }
      if(!strcmp(buffer, "SUPPORTERS"))
      {
         supporters=atoi(field);
      }
      if(!strcmp(buffer, "REG_SUFFIX"))
      {
         strcpy(regsuffix, field);
      }
      if(!strcmp(buffer, "MAXNICKLEN"))
      {
         maxnicklen=atoi(field);
      }
      if(!strcmp(buffer, "EMAIL_SUB"))
      {
         strcpy(emailsub, field);
      }
      if(!strcmp(buffer, "MAIL_PROG"))
      {
         strcpy(mailprog, field);
      }
      if(!strcmp(buffer, "DEOP_CHAN"))
      {
         deopchan=atoi(field);
      }
      if(!strcmp(buffer, "LOGGING"))
      {
         logging=atoi(field);
      }
  }
   fclose(fp);
}
