/* CS5 - Channel Services src/ip_tools.c
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

char *getuserhost(char *nick)
{
   struct nickinfo *current;
   char *newuser = NULL;

   if(nick == NULL) return NULL;
   current = nickhead;
   while(current != NULL)
   {
      if(!strcasecmp(nick, current->nick))
      {
         newuser = current->user;
         if(newuser[0] == '~')
         {
            newuser++;
         }
         sprintf(linebuff, "%s@%s", newuser, current->host);
         return(linebuff);
      }
      current = current->next;
   }
   return NULL;
}

char *getnickuserhost(char *nick)
{
   struct nickinfo *current;
   char *newuser = NULL;

   if(nick == NULL) return NULL;
   current = nickhead;
   while(current != NULL)
   {
      if(!strcasecmp(nick, current->nick))
      {
         newuser = current->user;
         if(newuser[0] == '~')
         {
            newuser++;
         }
         sprintf(linebuff1, "%s!%s@%s", current->nick, newuser, current->host);
         return(linebuff1);
      }
      current = current->next;
   }
   return NULL;
}

char *convert_addr(char *address)
{
   struct in_addr *ptr;
   struct hostent *hotptr;
   char **buf, *ptr2 = NULL, *addr = NULL, backup[100];
   backup[0]='\0';
   if(!is_ip(address))
   {
      if(addr = strchr(address, '@')) *addr++ = '\0';
      hotptr = gethostbyname(addr);
      if ( hotptr != NULL )
      {
        buf = hotptr->h_addr_list;
        ptr = (struct in_addr *) *buf++;
        strcpy(userhost, inet_ntoa(*ptr));
        if(ptr2 = strrchr(userhost, '.')) *++ptr2 = '*';
        *++ptr2 = '\0';
        sprintf(backup, "%s@%s", address, userhost);
        strcpy(userhost, backup);
        return userhost;
      }
   }
   return NULL;
}

int countdots(char *addr)
{
   int i = 0;
   while(*addr)
   {
     if(*addr == '.' )
     {
       i++;
     }
     addr++;
   }
   return(i);
}

int is_num(char *num)
{
   while(*num)
   {
     if(!isdigit(*num)) return 0;
     num++;
   }
   return 1;
}

int is_ip(char *addr)
{
   while(*addr)
   {
     if(!isdigit(*addr)&&( *addr != '.' )&&( *addr != '*' )) return 0;
     addr++;
   }
   return 1;
}

char *hostnamefromip(ip)
unsigned long ip;
{
  static char name[121]; struct hostent *hp; unsigned long addr=ip;
  unsigned char *p;
  hp=gethostbyaddr((char *)&addr,sizeof(addr),AF_INET);
  if (hp==NULL) {
    p=(unsigned char *)&addr;
    sprintf(name,"%u.%u.%u.%u",p[0],p[1],p[2],p[3]);
  }
  else strcpy(name,hp->h_name);
  return name;
}

char *get_ban_mask(char *uhost)
{
   char *ptr = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL;
   char usrhost[100], duhost[100];
   int howmany = 0;

   usrhost[0]='\0';duhost[0]='\0';
   ptr = strchr(uhost, '@');
   ptr++;
   if(!is_ip(ptr))
   {
      if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
      strcpy(duhost, ptr);
      howmany = countdots(ptr);
      if(strlen(uhost) > 8)
      {
         uhost[8] = '*';
         uhost[9] = '\0';
      }

      if(howmany == 1)
      {
          sprintf(usrhost, "*!*%s@%s", uhost, ptr);
      }
      else if(howmany == 3 || howmany == 2)
      {
         if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
         sprintf(usrhost, "*!*%s@*.%s", uhost, ptr2);
      }
      else if(howmany == 4)
      {
         if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
         if(ptr3 = strchr(ptr2, '.')) *ptr3++ = '\0';
         if(ptr4 = strchr(ptr3, '.')) *ptr4++ = '\0';
         sprintf(usrhost, "*!*%s@*.%s", uhost, ptr4);
      }
      else if(howmany >= 5)
      {
         if(ptr2 = strchr(ptr, '.')) *ptr2++ = '\0';
         if(ptr3 = strchr(ptr2, '.')) *ptr3++ = '\0';
         if(ptr4 = strchr(ptr3, '.')) *ptr4++ = '\0';
         if(ptr5 = strchr(ptr4, '.')) *ptr5++ = '\0';
         sprintf(usrhost, "*!*%s@*.%s", uhost, ptr5);
      }
   }
   else
   {
      if(ptr = strrchr(uhost, '.'))
      {
         *++ptr = '*';
         *++ptr = '\0';
      }
      sprintf(usrhost, "*!*%s", uhost);
   }
   strcpy(banmask, usrhost);
   return(banmask);
}
