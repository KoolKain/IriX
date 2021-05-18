/* CS5 - Channel Services src/misc_tools.c
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

char *list_modes(char *chan)
{
   struct regsinfo *regs;
   char mode_list[1000];

   mode_list[0] = '\0';
   modelist[0] = '\0';
   if(!ison(chan)) return(modelist);
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         sprintf(modelist, "%s", regs->modes);
         trailing_blanks(modelist);
         if(strlen(modelist) == 1) modelist[0] = '\0';
         return(modelist);
      }
      regs = regs->next;
   }
}

void debug(char *format, ...)
{
#ifdef DEBUG
   char buff[5000];
   va_list arglist;
   va_start(arglist, format);
   vsprintf(buff, format, arglist);
   if(*msg_chan == '#')
     send_to_server(":%s PRIVMSG %s :%s\n", bot.nick, msg_chan, buff);
   va_end(arglist);
#endif
}

char *date()
{
   time_t timer;
   time(&timer);
   strcpy(strbuff, ctime(&timer));
   strip_newline(strbuff);
   return strbuff;
}

char *bottime()
{
   time_t timer;
   struct tm *ntime;

   time(&timer);
   ntime=localtime(&timer);
   sprintf(strbuff, "%02d:%02d:%02d",ntime->tm_hour,ntime->tm_min,ntime->tm_sec);
   return (strbuff);
}

void command_log(char *who, char *cmd, char *rest)
{
   char nmsg[10], nrest[5000], *chan = NULL;
   nrest[0]='\0';nmsg[0]='\0';
   if(strcmp(cmd, "PRIVMSG") && joined == TRUE)
   {
      if(rest == NULL) 
      {
         log(LOG_FILE, "[%s] %s %s\n", date(), who, cmd);
         if(*msg_chan == '#')
           send_to_server(":%s PRIVMSG %s : [%s] %s par %s\n", botnick, msg_chan, bottime(), cmd, who);
      }
      else 
      {
         if(!strcasecmp(cmd, "AUTH"))
         {
            strcpy(nmsg, "*******");
            rest=nmsg;
         }
         if(!strcasecmp(cmd, "AUTHOP"))
         {
            strcpy(nmsg, "*******");
            chan = strtok(rest, " ");
            sprintf(nrest, "%s %s", chan, nmsg);
            rest = nrest;
         }
         log(LOG_FILE, "[%s] %s %s %s\n", date(), who, cmd, rest);
         if(*msg_chan == '#')
           send_to_server(":%s PRIVMSG %s : [%s] %s %s par %s\n", botnick, msg_chan, bottime(), cmd, rest, who);
      }
   }
}

int send_to_server(char *format, ...)
{
   va_list arglist;
   char buffer[MAXBUF];
   buffer[0]='\0';
   va_start(arglist, format);
   vsprintf(buffer, format, arglist);
   va_end(arglist);
   return(writeln(buffer));
}

void send_denied_msg(char *nick)
{
   char *nwho = NULL;
   nwho = nick2num(nick);
   send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, denied_msg);
}

char *get_command_name(char *rcommand)
{
   int i;
   for(i = 0; commands[i].name; i++)
   {
      if(!strcasecmp(rcommand, commands[i].realname)) return commands[i].name;
   }
   return;
}

char *get_command_rname(char *command)
{
   int i;
   for(i = 0; commands[i].name; i++)
   {
      if(!strcasecmp(command, commands[i].name)) return commands[i].realname;
   }
   return;
}

int get_command_level(char *command)
{
   int i;
   for(i = 0; commands[i].name; i++)
   {
      if(!strcasecmp(command, commands[i].realname)) return commands[i].level;
   }
   return -1;
}

void file_error(char *fname)
{
   fprintf(stderr, "Error opening %s\n", fname);
  log(ERRLOG_FILE, "[%s] Erreur en ouvrant %s\n", date(), fname);
}

char *get_time(long ttime)
{
  char tmp[200];
  struct tm *tmptr;
  int isdst=0,gmtoffset;

  newtime[0] = '\0';
  tmptr=localtime(&ttime);
  strcpy(tmp,ctime(&ttime));
  strip_newline(tmp);

  if (tmptr->tm_isdst)
   isdst=1;
#ifdef BSD
  gmtoffset=(tmptr->tm_gmtoff/SECPERHR-isdst)*(-1);
  sprintf(newtime,"%s %s %d",tmp,tmptr->tm_zone,gmtoffset);
#else
  gmtoffset=(timezone/SECPERHR-isdst)*(-1);
  sprintf(newtime,"%s %s %d",tmp,tzname[isdst],gmtoffset);
#endif
  return(newtime);
}
