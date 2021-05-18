/* CS5 - Channel Services src/wild_cmp.c
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

int wcmp(char *wld, char *str)
{
   char *st=NULL;

   if(!(*wld) || !(*str)) return 1;
   while(*wld || *str) 
   {
      if(tolower(*wld) == tolower(*str)) 
      {
         wld++;
         str++;
      }
      else if(tolower(*wld) != tolower(*str) && *wld == '*') 
      {
         int i = 0;
         st = (char *) malloc((char *) strlen(wld) + 1);
         while(*wld == '*' && *wld) 
         {
            wld++;
            if(!*wld)
            {
              free(st);
              return(0);
            }
         }
         while(*wld && *wld != '*' && *wld != '?') 
         {
           st[i] = *wld;
           i++;
           wld++;
         }
         st[i] = '\0';
         while(*str && strncasecmp(st, str, strlen(st))) str++;
         if(!strncasecmp(st, str, strlen(st))) 
           str+=strlen(st);
         else 
         {
           free(st);
           return(1);
         }
      }
      else if(tolower(*wld) != tolower(*str) && *wld == '?' && *wld && *str) 
      {
         wld++;
         str++;
      }
      else if(tolower(*wld) != tolower(*str)) return 1;
   }
   free(st);
   return(0);
}
