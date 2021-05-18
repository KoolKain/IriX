/* CS5 - Channel Services src/showcommands.c
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

void showcommands(char *who, char *rest)
{
   struct userinfo *user, *admn;
   struct authinfo *auth, *admnauth;
   struct nickinfo *nick;
   char *nwho, first[1000], second[1000], *garbage, *cmds1[1000];
   int i = 0, j = 0, k = 0, lngth = 0, level = 0, m = 0, l = 0, p = 0, commlevel = 0;
 
   cmds1[0] = '\0';
   if(rest == NULL) level = 0 ;
   if(rest != NULL)
   {
      user = return_userinfo(who, rest);
      if(user == NULL)
      {
         level = 0;
      }
      else
      {
         auth = return_authinfo(rest, who);
         if(auth == NULL)
           level = 0;
         else
           level = user->level;
      }
   }
   nick = return_nickinfo(who);
   admn = return_userinfo(who, csadminchan);
   admnauth = return_authinfo(csadminchan, who);
   if(admn != NULL)
   {
      if(admnauth != NULL)
      {
         level = admn->level;
         commlevel = maxadmnlevel;
      }
      else
        commlevel = maxuserlevel;
   }
   else
     commlevel = maxuserlevel;
   nwho = nick2num(who);
   p = 0;
   for(i = 0; *commands[i].name; i++)
   {
     if(commlevel >= commands[i].level)
     {
        if(!commands[i].chanopt)
        {
           if(!strcasecmp(commands[i].name, "opers"))
           {
              if(nick->is_oper || admnauth != NULL)
              {
                 cmds1[p] = commands[i].name;
                 p++;
              }
           }
           else if(commands[i].disable && admnauth != NULL)
           {
              cmds1[p] = commands[i].name;
              p++;
           }
           else if(!commands[i].disable && !strchr(commands[i].realname, ' '))
           {
              cmds1[p] = commands[i].name;
              p++;
           }
        }
     }
   }
/* NiX
   send_to_server(":%s NOTICE %s :| \037   Command      \037 [\037\0034Level\003\037] | \037   Command      \037 [\037\0034Level\003\037]\n", bot.nick, nwho);
*/
   send_to_server(":%s NOTICE %s :Commande         [Accès]  Commande         [Accès]\n", bot.nick, nwho);
   do_sort(p, cmds1);
   j = p/2;
   for(p = 0; p < j; p++)
   {
      if(rest != NULL)
      {
         if(level >= sortcom[p].level)
         {
            lngth = strlen(sortcom[p].name);
            first[0] = '\0';
            sprintf(first, "%s ", strupr(sortcom[p].name));
            while(lngth != 15)
            {
               strcat(first, "-");
               lngth++;
            }
         }
         else
         {
           first[0] = '\0';
           sprintf(first, "%s ", strupr(sortcom[p].name));
         }
         if(level >= sortcom[p+j].level)
         {
            lngth = strlen(sortcom[p+j].name);
            second[0] = '\0';
            sprintf(second, "%s ",  strupr(sortcom[p+j].name));
            while(lngth != 15)
            {
               strcat(second, "-");
               lngth++;
            }
         }
         else
         {
            second[0] = '\0';
            sprintf(second, "%s ",  strupr(sortcom[p+j].name));
         }
      }
      else
      {
         first[0] = '\0';
         strcpy(first,  strupr(sortcom[p].name));
         second[0] = '\0';
         strcpy(second, strupr(sortcom[p+j].name));
      }

      send_to_server(":%s NOTICE %s :%-16s\003 [ %03d ]  %-16s [ %03d ]\n", bot.nick, nwho, first, sortcom[p].level, second, sortcom[p+j].level);

/* NiX
      send_to_server(":%s NOTICE %s :| \00312%-16s\003 [\0034 %03d \003] | \00312%-16s\003 [\0034 %03d \003]\n", bot.nick, nwho, first, sortcom[p].level, second, sortcom[p+j].level);
*/
      first[0] = '\0';
      second[0] = '\0';
   }
/* NiX
   send_to_server(":%s NOTICE %s : \00312BAN niveau mis par le proprietaire\n", bot.nick, nwho);
*/
}

int compare (const void *arg1, const void *arg2)                                                
{                                                                                               
    return(strcmp(*(char **)arg1, *(char **)arg2));                                             
}                                                             

int do_sort(int cnt, char *cmds[ ])
{
   int l = 0, m = 0;
   qsort((char *)cmds, cnt, sizeof(char *), compare);
      for(l = 0;l < cnt; l++)
      {
            strcpy(sortcom[l].name, cmds[l]);
            for(m = 0; *commands[m].name; m++)
            {
               if(!strcasecmp(cmds[l], commands[m].name))
               {
                  sortcom[l].level = commands[m].level;
               }
            }
      }
}
