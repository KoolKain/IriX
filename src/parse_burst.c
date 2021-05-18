/* CS5 - Channel Services src/parse_burst.c
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
void parse_burst(char *who, char *rest)
{
   struct regsinfo *regs;
   char *nxtusr = NULL, *keys = NULL, *modes = NULL, *stamp = NULL, *numeric = NULL, *nuser = NULL, *user = NULL, *limt = NULL;
   char *umodes = NULL, *tuser = NULL, *chan = NULL, *host = NULL, *nwho = NULL, servnum[3], *server = NULL, chkmodes[20];
   int i = 0, j = 0, ops = 0, no_add= 0;
   char *ptr[10000], *ptr1[10000], *p = NULL, *more = NULL, testmodes[10000], reason1[3], newmore[10000];

   reason1[0] = '\0';


   if(strchr(rest, ','))
   {
      if(strchr(rest, '+') && *rest != '+')
      {
         chan = strtok(rest, " ");
         stamp = strtok(NULL, " ");
         modes = strtok(NULL, " ");
         strcpy(testmodes, modes);
         if(strchr(modes, '+'))
         {
            while(modes[0] != '\0')
            {
               modes++;
               if(*modes == 'k')
               {
                  keys=strtok(NULL, " ");
               }
               if(*modes == 'l')
               {
                  limt=strtok(NULL, " ");
               }
            }
            user = strtok(NULL, " ");
            more = strtok(NULL, ",");
            if((more != NULL) && strchr(more, '%'))
            {
               more+=2;
               if((p = strtok(more, " ")) != NULL) ptr1[j++] = p;
               while((p = strtok(NULL, " ")) != NULL) ptr1[j++] = p;
               ptr1[j] = NULL;
               j = 0;
               while(ptr1[j])
               {
                  add_baninfo(chan, ptr1[j], time(NULL), 0, 0, reason1, who, 0);
                  j++;
               }
            }
         }
         else 
         {
            user = modes;
         }
         more = strtok(NULL, ",");
         if((more != NULL) && strchr(more, '%'))
         {
            more+=2;
            if((p = strtok(more, " ")) != NULL) ptr1[j++] = p;
            while((p = strtok(NULL, " ")) != NULL) ptr1[j++] = p;
            ptr1[j] = NULL;
            j = 0;
            while(ptr1[j])
            {
               add_baninfo(chan, ptr1[j], time(NULL), 0, 0, reason1, who, 0);
               j++;
            }
         }
      }
      else
      {
         chan = strtok(rest, " ");
         stamp = strtok(NULL, " ");
         user = strtok(NULL, " ");
         more = strtok(NULL, ",");
         if((more != NULL) && strchr(more, '%'))
         {
            more+=2;
            if((p = strtok(more, " ")) != NULL) ptr1[j++] = p;
            while((p = strtok(NULL, " ")) != NULL) ptr1[j++] = p;
            ptr1[j] = NULL;
            j = 0;
            while(ptr1[j])
            {  
               add_baninfo(chan, ptr1[j], time(NULL), 0, 0, reason1, who, 0);
               j++;
            }
         }
      }
      add_burst(chan, testmodes, atol(stamp));
      if(ison(chan) && strcasecmp(chan, msg_chan))
         add_modeinfo(chan, stamp, testmodes);
      if(!strcasecmp(chan, msg_chan))
         add_modeinfo(chan, stamp, testmodes);
      if((p = strtok(user, ",")) != NULL) ptr[i++] = p;
/* OPUSNOTE: should the 'i' var here check for <10000 as well? */
      while((p = strtok(NULL, ",")) != NULL && i < 100) ptr[i++] = p;
      ptr[i] = NULL;
      i = 0;
      while(ptr[i])
      {
         if(strstr(ptr[i], ":"))
         {
            numeric=strtok(ptr[i], ":");
            umodes=strtok(NULL, " ");
            if(strstr(umodes, "o"))
              ops = 1;
         }
         else numeric = ptr[i];
         server=num2serv(*numeric);
         nwho = num2nick(numeric);
         regs = regshead;
         while(regs != NULL)
         {
            if(!strcasecmp(chan, regs->channel))
            {
               strcpy(regs->chanstamp, stamp);
            }
            regs = regs->next;
         }
         add_joininfo(chan, numeric, ops);
         i++;
      }
   }
   else
   {
      if(strchr(rest, '+') && *rest != '+')
      {
         chan = strtok(rest, " ");
         stamp = strtok(NULL, " ");
         modes = strtok(NULL, " ");
         strcpy(testmodes, modes);
         if(strchr(modes, '+'))
         {
            while(modes[0] != '\0')
            {
               modes++;
               if(*modes == 'k')
               {
                  keys=strtok(NULL, " ");
               }
               if(*modes == 'l')
               {
                  limt=strtok(NULL, " ");
               }
            }
            user = strtok(NULL, " ");
            more = strtok(NULL, ",");
            if((more != NULL) && strchr(more, '%'))
            {
               more+=2;
               if((p = strtok(more, " ")) != NULL) ptr1[j++] = p;
               while((p = strtok(NULL, " ")) != NULL) ptr1[j++] = p;
               ptr1[j] = NULL;
               j = 0;
               while(ptr1[j])
               {
                  add_baninfo(chan, ptr1[j], time(NULL), 0, 0, reason1, who, 0);
                  j++;
               }
            }
            if(strstr(user, ":"))
            {
               numeric=strtok(user, ":");
               umodes=strtok(NULL, " ");
               if(strstr(umodes, "o"))
                 ops = 1;
               user = numeric;
            }
         }
         else
         {
            user = modes;
            if(strstr(user, ":"))
            {
              numeric=strtok(user, ":");
              umodes=strtok(NULL, " ");
              if(strstr(umodes, "o"))
                ops = 1;
              user = numeric;
            }
         }
      }
      else
      {
         strcpy(newmore, rest);
         chan = strtok(rest, " ");
         stamp = strtok(NULL, " ");
         user = strtok(NULL, " ");
         if(user != NULL)
            more = strtok(NULL, ",");
         if(strstr(user, ":"))
         {
            if(*user == ':') *user++;
            numeric=strtok(user, ":");
	    if(numeric != NULL && *numeric != '%')
            {
               umodes=strtok(NULL, " ");
               if(strstr(umodes, "o"))
                 ops = 1;
               user = numeric;
            }
            else 
            {
               chan = strtok(newmore, " ");
               stamp = strtok(NULL, " ");
               more = strtok(NULL, ",");
               no_add = 1;
            }
         }
         if(more != NULL && strchr(more, '%'))
         {
            more+=2;
            if((p = strtok(more, " ")) != NULL) ptr1[j++] = p;
            while((p = strtok(NULL, " ")) != NULL) ptr1[j++] = p;
            ptr1[j] = NULL;
            j = 0;
            while(ptr1[j])
            {  
               add_baninfo(chan, ptr1[j], time(NULL), 0, 0, NULL, who, 0);
               j++;
            }
         }
      }
      add_burst(chan, testmodes, atol(stamp));
      if(ison(chan) && strcasecmp(chan, msg_chan))
         add_modeinfo(chan, stamp, testmodes);
      if(!strcasecmp(chan, msg_chan))
         add_modeinfo(chan, stamp, testmodes);
      numeric = user;
      server=num2serv(*numeric);
      nwho = num2nick(numeric);
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            strcpy(regs->chanstamp, stamp);
         }
         regs = regs->next;
      }
      if(!no_add)
      add_joininfo(chan, numeric, ops);
   }
}
