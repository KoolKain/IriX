/* CS5 - Channel Services src/write_files.c
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

void write_single_channel(char *chan)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct adderinfo *addr;
   FILE *chanoptf, *chanusrf;
   char *nchan, buff[300], buff1[10000], tmp[10000], tmp1[10000], tmp2[10000];
   int first_time = 0;

   regs = regshead;
   buff[0] = '\0';
   buff1[0] = '\0';
   tmp[0] = '\0';
   tmp1[0] = '\0';
   tmp2[0] = '\0';
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         nchan=conv_char(regs->channel);
         sprintf(buff, "database/chanopt/%s", nchan);
         strlwr(buff);
         if((chanoptf = fopen(buff, "w")) == NULL)
         {
            file_error(buff);
            return;
         }
         fprintf(chanoptf, "STRICTOP %d\n", regs->strictop);
         fprintf(chanoptf, "LASTSEEN %lu\n", regs->lastseen);
         fprintf(chanoptf, "NOBAN %d\n", regs->noban);
         fprintf(chanoptf, "CHMODESLEVEL %d\n", regs->chmodlvl);
         fprintf(chanoptf, "DEFMODES %s\n", regs->defmodes);
         fprintf(chanoptf, "OWNERNICK %s\n", regs->ownernick);
         fprintf(chanoptf, "OWNERNAME %s\n", regs->ownername);
         fprintf(chanoptf, "OWNEREMAIL %s\n", regs->ownemail);
         fprintf(chanoptf, "WELCOME %s\n", regs->welcome);
         fprintf(chanoptf, "MOTD %s\n", regs->motd);
         fprintf(chanoptf, "BANLEVEL %d\n", regs->banlvl);
         fprintf(chanoptf, "DESCRIPTION %s\n", regs->desc);
         fprintf(chanoptf, "OPEROP %d\n", regs->operop);
         fprintf(chanoptf, "NOOPS %d\n", regs->noops);
         fprintf(chanoptf, "DEFTOPIC %s\n", regs->deftopic);
         fprintf(chanoptf, "CHANURL %s\n", regs->url);
         fprintf(chanoptf, "SETUPDATE %lu\n", regs->setupdate);
         fprintf(chanoptf, "STAY %d\n", regs->stay);
         fprintf(chanoptf, "SETWELCOME %d\n", regs->setwelc);
         fprintf(chanoptf, "NOPURGE %d\n", regs->setpurge);
         fprintf(chanoptf, "SETVOICE %d\n", regs->setvoice);
         fprintf(chanoptf, "SUSPEND %d\n", regs->suspend);
         fprintf(chanoptf, "TOPICLOCK %d\n", regs->tlock);
         fprintf(chanoptf, "BANMOVE %d\n", regs->banmove);
         fprintf(chanoptf, "BANREMOVE %d\n", regs->banremove);
         fprintf(chanoptf, "BANTIMEOUT %d\n", regs->bantimeout);
         fprintf(chanoptf, "AUTOOPS %d\n", regs->autoops);
         fclose(chanoptf);
         sprintf(buff1, "database/chanusr/%s", nchan);
         strlwr(buff1);
         if((chanusrf = fopen(buff1, "w")) == NULL)
         {
            file_error(buff1);
            return;
         }
         user = regs->userhead;
         while(user != NULL)
         {
            tmp1[0] = '\0';
            tmp2[0] = '\0';
            sprintf(tmp, "%s %d %d %d %d %s %lu", user->nick, user->level, user->autoop, user->protect, user->suspend, user->password, user->lastseen);
            addr = user->adderhead;
            first_time == 0;
            while(addr != NULL)
            {
               if(!first_time)
               {
                  strcpy(tmp1, addr->address);
                  first_time=1;
               }
               else
               {
                  sprintf(tmp2, "%s %s", tmp1, addr->address);
                  strcpy(tmp1, tmp2);
               }
               addr = addr->next;
            }
            fprintf(chanusrf, "%s %s\n", tmp, tmp1);
            user = user->next;
         }
         fclose(chanusrf);
         return;
      }
      regs = regs->next;
   }
}

void write_all_banfile()
{
   struct regsinfo *regs;
   struct baninfo *ban;
   FILE *baninfof;
   char buff[1500], *nchan;

   regs = regshead;
   while(regs != NULL)
   {
         nchan=conv_char(regs->channel);
         sprintf(buff, "database/chanban/%s", nchan);
         strlwr(buff);
         if((baninfof = fopen(buff, "w")) == NULL)
         {
            file_error(buff);
            return;
         }
         ban = regs->banhead;
         while(ban != NULL)
         {
            if(ban->botban)
               fprintf(baninfof, "%s %s %ld %d %ld %s %s\n", regs->channel, ban->ban, ban->timeset, ban->level, ban->duration, ban->reason, ban->nick);
            ban=ban->next;
         }
         fclose(baninfof);
         return;
      regs = regs->next;
   }
}

void write_single_banfile(char *chan)
{
   struct regsinfo *regs;
   struct baninfo *ban;
   FILE *baninfof;
   char buff[1500], *nchan;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         nchan=conv_char(regs->channel);
         sprintf(buff, "database/chanban/%s", nchan);
         strlwr(buff);
         if((baninfof = fopen(buff, "w")) == NULL)
         {
            file_error(buff);
            return;
         }
         ban = regs->banhead;
         while(ban != NULL)
         {
            if(ban->botban)
               fprintf(baninfof, "%s %s %ld %d %ld %s %s\n", chan, ban->ban, ban->timeset, ban->level, ban->duration, ban->reason, ban->nick);
            ban=ban->next;
         }
         fclose(baninfof);
         return;
      }
      regs = regs->next;
   }
}

void write_single_infolines(char *chan)
{
   struct regsinfo *regs;
   struct userinfo *user;
   FILE *chaninfof;
   char buff[1500], *nchan;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         nchan=conv_char(regs->channel);
         sprintf(buff, "database/chaninfo/%s", nchan);
         strlwr(buff);
         if((chaninfof = fopen(buff, "w")) == NULL)
         {
            file_error(buff);
            return;
         }
         user = regs->userhead;
         while(user != NULL)
         {
            fprintf(chaninfof, "%s %s\n", user->nick, user->info);
            user=user->next;
         }
         fclose(chaninfof);
         return;
      }
      regs = regs->next;
   }
}

void write_all_infolines()
{
   struct regsinfo *regs;
   struct userinfo *user;
   FILE *chaninfof;
   char buff[1500], *nchan;

   regs = regshead;
   while(regs != NULL)
   {
      nchan = conv_char(regs->channel);
      sprintf(buff, "database/chaninfo/%s", nchan);
      strlwr(buff);
      if((chaninfof = fopen(buff, "w")) == NULL)
      {
         file_error(buff);
         return;
      }
      user = regs->userhead;
      while(user != NULL)
      {
         fprintf(chaninfof, "%s %s\n", user->nick, user->info);
         user=user->next;
      }
      fclose(chaninfof);
      regs = regs->next;
   }
}
void write_channel_list()
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct adderinfo *addr;
   FILE *chanoptf, *chanusrf;
   char *nchan, buff[10000], buff1[10000], tmp[10000], tmp1[10000], tmp2[10000];
   int first_time = 0;

   buff[0] = '\0';
   buff1[0] = '\0';
   tmp[0] = '\0';
   tmp1[0] = '\0';
   tmp2[0] = '\0';
   regs = regshead;
   while(regs != NULL)
   {
      nchan=conv_char(regs->channel);
      sprintf(buff, "database/chanopt/%s", nchan);
      strlwr(buff);
      if((chanoptf = fopen(buff, "w")) == NULL)
      {
         file_error(buff);
         return;
      }
      fprintf(chanoptf, "STRICTOP %d\n", regs->strictop);
      fprintf(chanoptf, "LASTSEEN %lu\n", regs->lastseen);
      fprintf(chanoptf, "NOBAN %d\n", regs->noban);
      fprintf(chanoptf, "CHMODESLEVEL %d\n", regs->chmodlvl);
      fprintf(chanoptf, "DEFMODES %s\n", regs->defmodes);
      fprintf(chanoptf, "OWNERNICK %s\n", regs->ownernick);
      fprintf(chanoptf, "OWNERNAME %s\n", regs->ownername);
      fprintf(chanoptf, "OWNEREMAIL %s\n", regs->ownemail);
      fprintf(chanoptf, "WELCOME %s\n", regs->welcome);
      fprintf(chanoptf, "MOTD %s\n", regs->motd);
      fprintf(chanoptf, "BANLEVEL %d\n", regs->banlvl);
      fprintf(chanoptf, "DESCRIPTION %s\n", regs->desc);
      fprintf(chanoptf, "OPEROP %d\n", regs->operop);
      fprintf(chanoptf, "NOOPS %d\n", regs->noops);
      fprintf(chanoptf, "DEFTOPIC %s\n", regs->deftopic);
      fprintf(chanoptf, "CHANURL %s\n", regs->url);
      fprintf(chanoptf, "SETUPDATE %lu\n", regs->setupdate);
      fprintf(chanoptf, "STAY %d\n", regs->stay);
      fprintf(chanoptf, "SETWELCOME %d\n", regs->setwelc);
      fprintf(chanoptf, "NOPURGE %d\n", regs->setpurge);
      fprintf(chanoptf, "SETVOICE %d\n", regs->setvoice);
      fprintf(chanoptf, "SUSPEND %d\n", regs->suspend);
      fprintf(chanoptf, "TOPICLOCK %d\n", regs->tlock);
      fprintf(chanoptf, "BANMOVE %d\n", regs->banmove);
      fprintf(chanoptf, "BANREMOVE %d\n", regs->banremove);
      fprintf(chanoptf, "BANTIMEOUT %d\n", regs->bantimeout);
      fprintf(chanoptf, "AUTOOPS %d\n", regs->autoops);
      fclose(chanoptf);
      buff[0] = '\0';
      buff1[0] = '\0';
      tmp[0] = '\0';
      tmp1[0] = '\0';
      tmp2[0] = '\0';
      sprintf(buff1, "database/chanusr/%s", nchan);
      strlwr(buff1);
      if((chanusrf = fopen(buff1, "w")) == NULL)
      {
         file_error(buff1);
         return;
      }
      user = regs->userhead;
      while(user != NULL)
      {
         tmp1[0] = '\0';
         tmp2[0] = '\0';
         sprintf(tmp, "%s %d %d %d %d %s %lu", user->nick, user->level, user->autoop, user->protect, user->suspend, user->password, user->lastseen);
         addr = user->adderhead;
         first_time = 0;
         while(addr != NULL)
         {
            if(!first_time)
            {
               strcpy(tmp1, addr->address);
               first_time=1;
            }
            else
            {
               sprintf(tmp2, "%s %s", tmp1, addr->address);
               strcpy(tmp1, tmp2);
            }
            addr = addr->next;
         }
         fprintf(chanusrf, "%s %s\n", tmp, tmp1);
         user = user->next;
      }
      fclose(chanusrf);
      regs = regs->next;
   }
   write_all_infolines();
}

void write_comm_list()
{
   FILE *fp;
   int i = 0;
   if((fp = fopen(COM_FILE, "w")) == NULL)
   {
      file_error(COM_FILE);
      return;
   }
   fprintf(fp, "%s\n", "***************************************");
   fprintf(fp, "%s\n", "*   DO NOT Modify this File By Hand   *");
   fprintf(fp, "%s\n", "***************************************");
   for(i = 0; *commands[i].name; i++)
   {
      if(!strcmp(commands[i].realname, " "))
      {
         fclose(fp);
         return;
      }
      fprintf(fp, "%-15s %-15s %-5d %d\n", commands[i].name, commands[i].realname, commands[i].level, commands[i].disable);
   }
   fclose(fp);
}

void write_regs_list()
{
   FILE *fp;
   struct regsinfo *current;
   if((fp = fopen(REG_FILE, "w")) == NULL)
   {
      file_error(REG_FILE);
      return;
   }
   current = regshead;
   if(current == NULL) return;
   while(current != NULL)
   {
      fprintf(fp, "%s %s\n", current->channel, current->ownaddr);
      current = current->next;
   }
   fclose(fp);
}

void write_all_files()
{
   if(!joined)
   {
      printf(" ********************************\n");
      printf(" Seems to be a problem with joins\n");
      printf(" ********************************\n");
      exit(0);
   }
   if(joined)
   {
      write_regs_list();
      write_channel_list();
      write_all_banfile();
      write_comm_list();
      send_to_server(":%s WALLOPS :Wrote Out ALL Bot Files\n", myserver);
   }
}
