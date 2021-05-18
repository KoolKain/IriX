/* CS5 - Channel Services src/topic.c
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

void topic(char *who, char *rest)
{
   struct regsinfo *tmp;
   char *nwho = NULL, *top = NULL, buff[10000], *chan = NULL, test[10000];
   FILE *fp;
   int i = 0;

   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if(top = strchr(rest, ' ')) *top++ = '\0';
   else top = NULL; 
   tmp = return_regsinfo(chan);
   if(tmp->tlock)
   {
      send_to_server(":%s NOTICE %s :Le topic du salon est protégé\n",bot.nick, nwho);
      return;  
   }
   if(top != NULL) 
   {
     send_to_server(":%s TOPIC %s :%s\n", bot.nick, rest, top);
     send_to_server(":%s TOPIC %s\n", bot.nick, rest);
     return;
   }
   if((fp = fopen(TOPIC_FILE, "r")) == NULL) 
   {
      file_error(TOPIC_FILE);
      return;
   }
   while(fgets(buff, 300, fp) != NULL) 
   {
      if(buff[0] == '-') trk_topics = 0;
      else if(i++ == trk_topics) 
      {
         send_to_server(":%s TOPIC %s :%s\n", bot.nick, rest, buff);
         send_to_server(":%s TOPIC %s\n", bot.nick, rest);
         trk_topics++;
         fclose(fp);
         return;
      }
   }
   fclose(fp);
}

void set_topicnumber()
{
   FILE *fp;
   char buff[300];
   if((fp = fopen(TOPIC_FILE, "r")) == NULL) 
   {
      file_error(TOPIC_FILE);
      return;
   }
   while(fgets(buff, 300, fp)[0] != '-') no_topics++;
   fclose(fp);
}

void default_topic(char *chan)
{
   struct regsinfo *current, *tmp;

   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         send_to_server(":%s TOPIC %s :%s\n", bot.nick, chan, current->deftopic);
         send_to_server(":%s TOPIC %s\n", bot.nick, chan);
         return;
      }
      current = current->next;
   }
}

void reset_all_defs(char *who, char *rest)
{
   struct regsinfo *tmp;
   char *chan = NULL;
  
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   tmp = return_regsinfo(chan);
   if(tmp == NULL) return;
   send_to_server(":%s MODE %s %s\n", bot.nick, chan, tmp->defmodes);
   send_to_server(":%s TOPIC %s :%s\n", bot.nick, chan, tmp->deftopic);
   send_to_server(":%s MODE %s\n", bot.nick, chan);
   send_to_server(":%s TOPIC %s\n", bot.nick, chan);
}

void reset_defmodes(char *who, char *rest)
{
   struct regsinfo *tmp;
   char *chan = NULL, *rmodes = NULL;
  
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   tmp = return_regsinfo(chan);
   if(tmp == NULL) return;
   rmodes = list_modes(chan);
   send_to_server(":%s MODE %s -%s\n", bot.nick, chan, ++rmodes);
   send_to_server(":%s MODE %s %s\n", bot.nick, chan, tmp->defmodes);
   send_to_server(":%s MODE %s\n", bot.nick, chan);
}

void reset_deftopic(char *who, char *rest)
{
   struct regsinfo *tmp;
   char *chan = NULL;
  
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   tmp = return_regsinfo(chan);
   if(tmp == NULL) return;
   send_to_server(":%s TOPIC %s :%s\n", bot.nick, chan, tmp->deftopic);
   send_to_server(":%s TOPIC %s\n", bot.nick, chan);
}

void reset_all_deftopics()
{
   struct regsinfo *regs;
  
   regs = regshead;
   while(regs != NULL)
   {
      send_to_server(":%s TOPIC %s :%s\n", bot.nick, regs->channel, regs->deftopic);
      send_to_server(":%s TOPIC %s\n", bot.nick, regs->channel);
      regs = regs->next;
   }
}

void reset_all_current_topics()
{
   struct regsinfo *regs;
  
   regs = regshead;
   while(regs != NULL)
   {
      send_to_server(":%s TOPIC %s :%s\n", bot.nick, regs->channel, regs->topic);
      send_to_server(":%s TOPIC %s\n", bot.nick, regs->channel);
      regs = regs->next;
   }
}
 
void parse_topic(char *who, char *rest)
{
   
   struct regsinfo *tmp;

   char *ptr = NULL;
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   ptr++;
   tmp = return_regsinfo(rest);
   if(tmp != NULL)
   {
      if(tmp->tlock)
      {
         send_to_server(":%s TOPIC %s :%s\n", bot.nick, rest, tmp->topic);
         send_to_server(":%s TOPIC %s\n", bot.nick, rest);
         return;
      }
   }
   send_to_server(":%s TOPIC %s\n", bot.nick, rest);
}
