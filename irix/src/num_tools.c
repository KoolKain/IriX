/* CS5 - Channel Services src/num_tools.c
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

void serv_unused(char *server)
{
   int i;

   for(i=0;i<NUMNICKBASE;i++)
   {
      if(!strcasecmp(server,servinfo[i].server))
      {
         servinfo[i].used = FALSE;
         return;
      }
   }
}

void serv_used(char chars, char *server)
{
   int i;

   for(i=0;i<NUMNICKBASE;i++)
   {
      if(chars == servinfo[i].chars)
      {
         servinfo[i].used = TRUE;
         strcpy(servinfo[i].server, server);
         return;
      }
   }
}

void delete_usernum(char *numeric)
{
  int i;

  for(i=0;i<MAXNUM;i++)
  {
    if(!strcmp(numeric, usernums[i].chars))
    {
       usernums[i].used=FALSE;
       return;
    }
  }
}

char *num2serv(char num)
{
  int i;
  char *entry=(char *)NULL;

  for(i=0;i<NUMNICKBASE;i++)
  {
    if(num == servinfo[i].chars)
    {
      entry=servinfo[i].server;
      break;
    }
  }
  return(entry);
}

char *nick2num(char *who)
{
  struct nickinfo *current;

  current = nickhead;
  while(current != NULL)
  {
     if(!strcasecmp(who, current->nick))
     {
        return(current->numeric);
     }
     current=current->next;
   }
   return NULL;
}

char *num2nick(char *num)
{
  struct nickinfo *current;

  current = nickhead;
  while(current != NULL)
  {
     if(!strcmp(num, current->numeric))
     {
        return(current->nick);
     }
     current=current->next;
   }
   return NULL;
}

char get_free_servnum(char *server, char numeric)
{ 
  int i;
  char entry=(char)NULL;
  
  for(i=0;i<NUMNICKBASE;i++) 
  { 
    if ( servinfo[i].used == FALSE && servinfo[i].chars == numeric)
    { 
      servinfo[i].used = TRUE;
      strcpy(servinfo[i].server, server);
      entry=servinfo[i].chars;
      break;
    }
  }
  return(entry);
}

char *get_free_usernum(char *nick)
{ 
  int i;
  char *entry=(char *)NULL;
  
  for(i=0;i<MAXNUM;i++) 
  {
    if(usernums[i].used == FALSE)
      {
         strcpy(usernums[i].nick, nick);
         usernums[i].used = TRUE;
         entry=usernums[i].chars;
         break;
      }
   }
  return(entry);
}

char *get_usernum(char *nick)
{
  int i;
  char *entry=(char *)NULL;

  for(i=0;i<MAXNUM;i++)
  {
    if(!strcasecmp(usernums[i].nick, nick))
    {
      entry=usernums[i].chars;
      break;
    }
  }
  return(entry);
}
