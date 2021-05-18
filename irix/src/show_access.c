/* CS5 - Channel Services src/show_access.c
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

void show_access(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct adderinfo *addr, *tmp;
   char *aut = "OFF", *prot = "OFF", *suspend = "OFF";
   char *nwho, *chan, *nick, sbuff1[10000], sbuff[10000], buff[10000], test[10000], *helpname;
   int howmany = 0;
   
   helpname = get_command_name("access");
   if(rest == NULL)
   {
      do_help(who, helpname);
      return;
   }
   buff[0] = '\0';
   sbuff[0] = '\0';
   chan = strtok(rest, " ");
   strcpy(test, chan); 
   nick = strtok(NULL, " ");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   nwho = nick2num(who);

   if((nick[0] == '>' || nick[0] == '<' || nick[0] == '=') && nick[1] == ' ')
   {   
       send_to_server(":%s NOTICE %s : Il ne doit pas y avoir d'espace après < ou > ou le signe = \n", bot.nick, nwho);
       return;
   }   
   if((nick[0] == '>' || nick[0] == '<' || nick[0] == '=') && !isdigit(nick[1]))
   {   
       send_to_server(":%s NOTICE %s : La valeur de level doit être un signe < ou > \n", bot.nick, nwho);
       return;
   }   
   if(nick[0] == '>')
   {
      nick++;
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            user = regs->userhead;
            while(user != NULL)
            {
               if(atoi(nick) < user->level)
               {
                  if(user->autoop == 0) aut = "\0034OFF\003";
                  else aut = "\0033ON\003";
                  if(user->protect == 0) prot = "\0034OFF\003";
                  else prot = "\0033ON\003";
                  if(user->suspend == 0) suspend = "\0034OFF\003";
                  else suspend = "\0033ON\003";
                  send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                  send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                  sbuff[0] = '\0';
                  buff[0] = '\0';
                  howmany++;
                  addr = user->adderhead;
                  while(addr != NULL)
                  {
                     sprintf(sbuff, "%s  ", addr->address);
                     strcat(buff, sbuff);
                     addr = addr->next;
                  }
                  send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                  if(user->info != NULL && user->info[0] != '\0')
                     send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                  send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
               }    
               user = user->next;
            }
         }
         regs = regs->next;
      }
      send_to_server(":%s NOTICE %s : Un total de %d accès trouvés\n", bot.nick, nwho, howmany);
      return;
   }
   if(nick[0] == '<')
   {
      nick++;
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            user = regs->userhead;
            while(user != NULL)
            {
               if(atoi(nick) > user->level)
               {
                  if(user->autoop == 0) aut = "\0034OFF\003";
                  else aut = "\0033ON\003";
                  if(user->protect == 0) prot = "\0034OFF\003";
                  else prot = "\0033ON\003";
                  if(user->suspend == 0) suspend = "\0034OFF\003";
                  else suspend = "\0033ON\003";
                  send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                  send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                  sbuff[0] = '\0';
                  buff[0] = '\0';
                  howmany++;
                  addr = user->adderhead;
                  while(addr != NULL)
                  {
                     sprintf(sbuff, "%s  ", addr->address);
                     strcat(buff, sbuff);
                     addr = addr->next;
                  }
                  send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                  if(user->info != NULL && user->info[0] != '\0')
                     send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                  send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
               }    
               user = user->next;
            }
         }
         regs = regs->next;
      }
      send_to_server(":%s NOTICE %s : Un total de %d accès trouvés\n", bot.nick, nwho, howmany);
      return;
   }
   if(nick[0] == '=')
   {
      nick++;
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            user = regs->userhead;
            while(user != NULL)
            {
               sbuff[0] == '\0';
               buff[0] == '\0';
               if(atoi(nick) == user->level)
               {
                  if(user->autoop == 0) aut = "\0034OFF\003";
                  else aut = "\0033ON\003";
                  if(user->protect == 0) prot = "\0034OFF\003";
                  else prot = "\0033ON\003";
                  if(user->suspend == 0) suspend = "\0034OFF\003";
                  else suspend = "\0033ON\003";
                  send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                  send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                  sbuff[0] = '\0';
                  buff[0] = '\0';
                  howmany++;
                  addr = user->adderhead;
                  while(addr != NULL)
                  {
                     sprintf(sbuff, "%s  ", addr->address);
                     strcat(buff, sbuff);
                     addr = addr->next;
                  }
                  send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                  if(user->info != NULL && user->info[0] != '\0')
                     send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                  send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
               }    
               user = user->next;
            }
         }
         regs = regs->next;
      }
      send_to_server(":%s NOTICE %s : Un total de %d accès trouvés\n", bot.nick, nwho, howmany);
      return;
   }
   if(strlen(nick) > 99) return;
   if(*chan != '*')
   {
      if(chan_check(test, who) == -1) return;
      if(check(test, who) == -1) return;
   }
   sprintf(sbuff1, "*%s*", nick);

   /* Ste */
   if(*chan == '*')
     send_to_server(":%s NOTICE %s : I won't do what you tell me \n", bot.nick, nwho);     
   /*
   if(*chan == '*' && strlen(chan) < 99)
   {
      regs = regshead;
      while(regs != NULL)
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strchr(nick, '@') && !strchr(nick, '*'))
            {
               if(!wcmp(sbuff1, user->nick))
               {
                  if(user->autoop == 0) aut = "\0034OFF\003";
                  else aut = "\0033ON\003";
                  if(user->protect == 0) prot = "\0034OFF\003";
                  else prot = "\0033ON\003";
                  if(user->suspend == 0) suspend = "\0034OFF\003";
                  else suspend = "\0033ON\003";
                  send_to_server(":%s NOTICE %s : \0031Salon: \003\00312 %s\003\0031 \n", bot.nick, nwho, regs->channel);
                  send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                  send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                  sbuff[0] = '\0';
                  buff[0] = '\0';
                  howmany++;
                  addr = user->adderhead;
                  while(addr != NULL)
                  {
                     sprintf(sbuff, "%s  ", addr->address);
                     strcat(buff, sbuff);
                     addr = addr->next;
                  }
                  send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                  if(user->info != NULL && user->info[0] != '\0')
                     send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                  send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
               }
            }
            else if(!strchr(nick, '@') && strchr(nick, '*'))
            {
               if(!wcmp(nick, user->nick))
               {
                  if(user->autoop == 0) aut = "\0034OFF\003";
                  else aut = "\0033ON\003";
                  if(user->protect == 0) prot = "\0034OFF\003";
                  else prot = "\0033ON\003";
                  if(user->suspend == 0) suspend = "\0034OFF\003";
                  else suspend = "\0033ON\003";
                  send_to_server(":%s NOTICE %s : \0031Salon: \003\00312 %s\003\0031 \n", bot.nick, nwho, regs->channel);
                  send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                  send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                  sbuff[0] = '\0';
                  buff[0] = '\0';
                  howmany++;
                  addr = user->adderhead;
                  while(addr != NULL)
                  {
                     sprintf(sbuff, "%s  ", addr->address);
                     strcat(buff, sbuff);
                     addr = addr->next;
                  }
                  send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                  if(user->info != NULL && user->info[0] != '\0')
                     send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                  send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
               }
            }
            else
            {
               collapse(nick);
               addr = user->adderhead;
               while(addr != NULL)
               {
                  if(!bancmp(nick, addr->address) || !bancmp(addr->address, nick))
                  {
                     if(user->autoop == 0) aut = "\0034OFF\003";
                     else aut = "\0033ON\003";
                     if(user->protect == 0) prot = "\0034OFF\003";
                     else prot = "\0033ON\003";
                     if(user->suspend == 0) suspend = "\0034OFF\003";
                     else suspend = "\0033ON\003";
                     send_to_server(":%s NOTICE %s : \0031Salon: \003\00312 %s\003\0031 \n", bot.nick, nwho, regs->channel);
                     send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                     send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                     sbuff[0] = '\0';
                     buff[0] = '\0';
                     howmany++;
                     addr = user->adderhead;
                     while(addr != NULL)
                     {
                        sprintf(sbuff, "%s  ", addr->address);
                        strcat(buff, sbuff);
                        addr = addr->next;
                     }
                     send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                     if(user->info != NULL && user->info[0] != '\0')
                        send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                     send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
                     break;
                  }
                  addr = addr->next;
               }
            }
            user = user->next;
         }
         regs = regs->next;
      }
      send_to_server(":%s NOTICE %s : Un total de %d accès trouvés\n", bot.nick, nwho, howmany);
      return;
   }
   */

   
   if(*chan != '#') return;
   {
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            user = regs->userhead;
            while(user != NULL)
            {
               if(!strchr(nick, '@') && !strchr(nick, '*'))
               {
                  if(!wcmp(sbuff1, user->nick))
                  {
                     if(user->autoop == 0) aut = "\0034OFF\003";
                     else aut = "\0033ON\003";
                     if(user->protect == 0) prot = "\0034OFF\003";
                     else prot = "\0033ON\003";
                     if(user->suspend == 0) suspend = "\0034OFF\003";
                     else suspend = "\0033ON\003";
                        send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                        send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                     sbuff[0] = '\0';
                     buff[0] = '\0';
                     howmany++;
                     addr = user->adderhead;
                     while(addr != NULL)
                     {
                        sprintf(sbuff, "%s  ", addr->address);
                        strcat(buff, sbuff);
                        addr = addr->next;
                     }
                     send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                     if(user->info != NULL && user->info[0] != '\0')
                        send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                     send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
                  }
               }
               else if(!strchr(nick, '@') && strchr(nick, '*'))
               {
                  if(!wcmp(nick, user->nick))
                  {
                     if(user->autoop == 0) aut = "\0034OFF\003";
                     else aut = "\0033ON\003";
                     if(user->protect == 0) prot = "\0034OFF\003";
                     else prot = "\0033ON\003";
                     if(user->suspend == 0) suspend = "\0034OFF\003";
                     else suspend = "\0033ON\003";
                        send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                        send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                     sbuff[0] = '\0';
                     buff[0] = '\0';
                     howmany++;
                     addr = user->adderhead;
                     while(addr != NULL)
                     {
                        sprintf(sbuff, "%s  ", addr->address);
                        strcat(buff, sbuff);
                        addr = addr->next;
                     }
                     send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                     if(user->info != NULL && user->info[0] != '\0')
                        send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                     send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
                  }
               }
               else 
               {
               collapse(nick);
               addr = user->adderhead;
               while(addr != NULL)
               {
                  if(!bancmp(nick, addr->address) || !bancmp(addr->address, nick))
                  {
                     if(user->autoop == 0) aut = "\0034OFF\003";
                     else aut = "\0033ON\003";
                     if(user->protect == 0) prot = "\0034OFF\003";
                     else prot = "\0033ON\003";
                     if(user->suspend == 0) suspend = "\0034OFF\003";
                     else suspend = "\0033ON\003";
                        send_to_server(":%s NOTICE %s : \0031Salon: \003\00312 %s\003\0031 \n", bot.nick, nwho, regs->channel);
                        send_to_server(":%s NOTICE %s : \0031Profil: [\003\00312 %s\003\0031 ] [\003\0034 %d\003\0031 ] Autoop:\003 %s \0031Protégé:\003 %s \0031Suspendu:\003 %s\n", bot.nick, nwho, user->nick, user->level, aut, prot, suspend);
                        send_to_server(":%s NOTICE %s : \0031Dernière présence: \003\0034 %s\003\n", bot.nick, nwho, get_time(user->lastseen));
                     sbuff[0] = '\0';
                     buff[0] = '\0';
                     howmany++;
                     addr = user->adderhead;
                     while(addr != NULL)
                     {
                        sprintf(sbuff, "%s  ", addr->address);
                        strcat(buff, sbuff);
                        addr = addr->next;
                     }
                     send_to_server(":%s NOTICE %s : \0031Masques:\003\0032 %s\003\n", bot.nick, nwho, buff);
                     if(user->info != NULL && user->info[0] != '\0')
                        send_to_server(":%s NOTICE %s : \0031InfoLine:\003\0034 %s\003\n", bot.nick, nwho, user->info);
                     send_to_server(":%s NOTICE %s : -\n", bot.nick, nwho);
                     break;
                  }
                  addr = addr->next;
               }
            }
            user = user->next;
            }
         }
         regs = regs->next;
      }
      send_to_server(":%s NOTICE %s : Un total de %d accès trouvés\n", bot.nick, nwho, howmany);
      return;
   }
}
