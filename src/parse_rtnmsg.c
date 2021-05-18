/* CS5 - Channel Services src/parse_rtnmsg.c
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

void parse_action(char *who, char *rest)
{
}

void parse_away(char *who, char *rest)
{
  struct nickinfo *current;
  
  current = nickhead;
  while(current != NULL)
  {
     if(!strcasecmp(who, current->nick))
     {
        if(rest == NULL)
        {
          strcpy(current->away, "H");
        }
        else
        {
          strcpy(current->away, "G");
        }
        return;
     }
     current = current->next;
  }
}
  
void parse_367(char *who, char *rest)
{
   char *chan = NULL, *ban_nick = NULL, *ptr = NULL;
   if(chan = strchr(rest, ' ')) *chan++ = '\0';
   if(ban_nick = strchr(chan, ' ')) *ban_nick++ = '\0';
   if(ptr = strchr(ban_nick, ' ')) *ptr = '\0';
   add_baninfo(chan, ban_nick, time(NULL), 0, 0, NULL, who, 0);
}

void parse_353(char *who, char *rest)
{
   char *parv[100], buff[10000], md[100], *nicklist = NULL, *ptr = NULL, *chan = NULL;
   int i = 0, j = 0, n;
   if(nicklist = strchr(rest, ':')) *nicklist++ = '\0';
   if(ptr = strrchr(rest, ' ')) *ptr = '\0';
   if(chan = strrchr(rest, ' ')) *chan++ = '\0';
   trailing_blanks(chan);
   ptr = strtok(nicklist, " ");
   if(*ptr == '@')
     if(strcasecmp(++ptr, bot.nick)) parv[i++] = ptr;
   while((ptr = strtok(NULL, " ")) != NULL)
   {
      if(*ptr == '@')
         if(strcasecmp(++ptr, bot.nick)) parv[i++] = ptr;
   }
   parv[i] = "%";
   parv[i+1] = NULL;
   if(parv[0][0] != '%')
   {
      strcpy(md, "-o");
      strcpy(buff, parv[0]);
   }
   i = 1;
   while(parv[i] != NULL)
   {
      if(j < 5 && parv[i][0] != '%')
      {
         if(strcasecmp(parv[i], bot.nick))
         {
            strcat(buff, " ");
            strcat(buff, parv[i]);
            j++;
         }
      }
      else
      {
         for(n = 0; n < j; n++) strcat(md, "o");
         send_to_server(":%s MODE %s %s %s\n", bot.nick, chan, md, buff);
         strcpy(buff, parv[i]);
         strcpy(md, "-o");
         j = 0;
      }
      i++;
   }
}

void parse_007(char *who, char *rest)
{
   end_map = 1;
   return;
}

void parse_005(char *who, char *rest)
{
   char *nwho = NULL, *map = NULL, nmap[1000];

   if(!end_map)
   {
      nwho = nick2num(who);
      map = strchr(rest, ':');
      sprintf(nmap, "%s\n", map);
      add_mapinfo(nmap);
   }
   return;
}

void parse_332(char *who, char *rest)
{
   char *ptr = NULL, *topic = NULL;
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   if(topic = strchr(ptr, ':')) *topic++ = '\0';
   trailing_blanks(ptr);
   add_topic(ptr, topic);
}

void parse_324(char *who, char *rest)
{
   char *chan = NULL, *mode = NULL, *remain = NULL;
   trailing_blanks(rest);
   if(chan = strchr(rest, ' ')) *chan++ = '\0';
   if(mode = strchr(chan, ' ')) *mode++ = '\0';
   replace_mnode(chan, mode);
}
