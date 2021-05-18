/* CS5 - Channel Services src/parse.c
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

void parse(char *line)
{
   int i = 0;
   char *sample = NULL, *nline = NULL, *cmd = NULL, *rest = NULL, *who = NULL, test[1000], *server = NULL, *botnum;
   char *samp1 = NULL, *samp2 = NULL, *samp3 = NULL, *samp4 = NULL, *samp5 = NULL, *samp6 = NULL, *ptr = NULL, *ptr1 = NULL, nrest[10000], logit[10000];
   char *usernum = NULL, *nwho = NULL;
   int nick_server = 0;

   usernum=get_usernum(bot.nick);
   trailing_blanks(line);
   if(strlen(line) > 524)
   {
       return;
   }
   strcpy(test, line);
   if(is_serv(test) || *line == ':')
   {
      if (!strncmp(line,":SERVER",7))
         nick_server = 1;
      line++;
         if(cmd = strchr(line, ' ')) *cmd++ = '\0';
         who = line;
         if(rest = strchr(cmd, ' ')) *rest++ = '\0';
         else
         {
            if(strcmp(cmd, "EOB_ACK") && strcmp(cmd, "END_OF_BURST") && strcmp(cmd, "AWAY"))
            {
               if(who = strchr(line, ' ')) *who++ = '\0';
               cmd = line;
            }
         }
         if(strcmp(cmd, "ACTION") && strcmp(cmd, "AWAY") && who[0] != '\0' && *rest == '#' && !strcmp(cmd, "PRIVMSG"))
         {
            if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
            *ptr++;
            if(*ptr == comchar)
            {
               *ptr++;
               if(ptr1 = strchr(ptr, ' ')) *ptr1++ = '\0';
               for(i = 0; *commands[i].name; i++) 
               {
                  if(!strcasecmp(ptr, commands[i].name) && commands[i].secure && ptr1 == NULL) 
                  {
                     strlwr(ptr);
                     do_help(who, ptr);
                     return;
                  }
                  if(!strcasecmp(ptr, commands[i].name) && commands[i].secure && ptr1 != NULL) 
                  {
                     nwho = nick2num(who);
		         send_to_server(":%s NOTICE %s : Violation de la sécurité - Un mot de passe a peut-être été dévoilé\n", bot.nick, nwho);
		         send_to_server(":%s NOTICE %s : Veuillez utiliser les messages privés avec %s pour l'utilisation des mot de passe\n", bot.nick, nwho, bot.nick);
                     return;
                  }
                  if(!strcasecmp(ptr, commands[i].name) && commands[i].nochan) 
                  {
                     if(ptr1 != NULL)
                        sprintf(nrest, "%s :%s %s", usernum, ptr, ptr1);
                     else
                        sprintf(nrest, "%s :%s", usernum, ptr);
                     rest = nrest;
                  }
                  else if(!strcasecmp(ptr, commands[i].name) && !commands[i].nochan) 
                  {
                     if(ptr1 != NULL)
                     {
                        if(ptr1[0] == '#')
                           sprintf(nrest, "%s :%s %s", usernum, ptr, ptr1);
                        else
                           sprintf(nrest, "%s :%s %s %s", usernum, ptr, rest, ptr1);
                     }
                     else
                        sprintf(nrest, "%s :%s %s", usernum, ptr, rest);
                     rest = nrest;
                  }
               }
   botnum=get_usernum(bot.nick);
   if(!strcmp(cmd, "PRIVMSG") && strstr(rest, botnum) && logging)
   {
      sprintf(logit, ":%s %s %s\n", who, cmd, rest);
      log(BOT_LOG, "R [%s] %s",date(), logit);
   }
            }
            else return;
         }
   }
   else
   {
      strcpy(test, line);
      who=strtok(test, " ");
      if(strcasecmp(who, "PASS"))
      {
         if(cmd = strchr(line, ' ')) *cmd++ = '\0';
         if(rest = strchr(cmd, ' ')) *rest++ = '\0';
      }
      else
      {
         if(who = strchr(line, ' ')) *who++ = '\0';
         cmd = line;
         rest = "NONE";
      }
   }
   if(rest != NULL)
      if(rest[0] == '#' && !strcmp(cmd, "PRIVMSG")) return;

#ifdef DEBUG
printf("Parse - Command: [%s] Who: [%s] Rest: [%s]\n", cmd, who, rest);  
#endif
   if(who != NULL)
   {
      if(!strcmp(who, "SERVER") && !nick_server)
      {
         samp1=strtok(rest, " ");
         samp2=strtok(NULL, " ");
         samp3=strtok(NULL, " ");
         samp4=strtok(NULL, " ");
         samp5=strtok(NULL, " ");
         serv_used(*samp5, cmd);
      }
      if(!strcmp(cmd, "SERVER"))
      {
         server=strtok(rest, " ");
         samp1=strtok(NULL, " ");
         samp2=strtok(NULL, " ");
         samp3=strtok(NULL, " ");
         samp4=strtok(NULL, " ");
         samp5=strtok(NULL, " ");
         serv_used(*samp5, server);
         sprintf(who, "%c", *test);
         cmd = "SERVER";
      }
   }
#ifdef DEBUG
   if(msglog)
   {
      if(strcmp(cmd,"PONG") && strcmp(cmd, "PRIVMSG") && strcmp(cmd, "BURST"))
      {
         command_log(who, cmd, rest);
      }
   }
#endif

   for(i = 0; parse_command[i].name; i++) 
   {
      if(!strcmp(cmd, parse_command[i].name)) 
      {
         parse_command[i].function(who, rest);
         return;
      }
   }
}
