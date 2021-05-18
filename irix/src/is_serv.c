/* CS5 - Channel Services src/is_serv.c
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
int is_serv(char *line)
{
   struct servinfo *new, *current;
   char *sample = NULL, *samp2 = NULL, *samp3 = NULL, *samp4 = NULL, *samp5 = NULL, *samp6 = NULL, *samp7 = NULL, *samp8 = NULL;

   sample=strtok(line, " ");
   if(strlen(sample) > 1) 
   {
      return 0;
   }
   samp2=strtok(NULL, " ");
   if(strcmp(samp2, "SERVER")) return 1; 
   samp3=strtok(NULL, " ");
   samp4=strtok(NULL, " ");
   samp5=strtok(NULL, " ");
   samp6=strtok(NULL, " ");
   samp7=strtok(NULL, " ");
   samp8=strtok(NULL, " ");
   serv_used(*samp8, samp3);
   return 1;
}

void add_servinfo(char *server, char *rest)
{
   struct servinfo *new, *current;
   char *samp1, *samp2, *samp3, *samp4, *samp5, *samp6;

   samp1=strtok(rest, " ");
   samp2=strtok(NULL, " ");
   samp3=strtok(NULL, " ");
   samp4=strtok(NULL, " ");
   samp5=strtok(NULL, " ");
   serv_used(*samp5, server);
}
