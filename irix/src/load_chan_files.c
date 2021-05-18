/* CS5 - Channel Services src/load_chan_files.c
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

void load_channels()
{
   FILE *fp, *fp1, *fp2, *fp3;
   char buff[1000], buff1[10000], buff2[10000], buff3[10000], *chan = NULL, *nchan = NULL, *temp = NULL, *ownaddr = NULL;
   char url[300], deftopic[500], defmodes[500], welcome[500], motd[500], stay[2], created[30];
   char strictop[2], lastseen[10], noban[2], chmodlvl[5], ownernick[50], ownername[1000];
   char ownemail[500], banlvl[5], desc[500], operop[2], setupdate[10];
   char setwelc[2], setpurge[2], setvoice[2], suspend[2], tlock[2], noops[2];
   char banmove[10], banremove[10], bantimeout[10], autoops[2];
   char *nick = NULL, *addr = NULL, *lvl = NULL, *aut = NULL, *prot = NULL, *passw = NULL, *ususpend = NULL, *field = NULL, *seen = NULL, *info = NULL;
   buff[0]='\0';buff1[0]='\0';buff2[0]='\0';buff3[0]='\0';url[0]='\0';deftopic[0]='\0';defmodes[0]='\0';welcome[0]='\0';
   motd[0]='\0';stay[0]='\0';created[0]='\0';strictop[0]='\0';lastseen[0]='\0';noban[0]='\0';chmodlvl[0]='\0';ownernick[0]='\0';
   ownername[0]='\0';ownemail[0]='\0';banlvl[0]='\0';desc[0]='\0';operop[0]='\0';noops[0]='\0';setupdate[0]='\0';setwelc[0]='\0';
   setpurge[0]='\0';setvoice[0]='\0';suspend[0]='\0';tlock[0]='\0';banmove[0]='\0';banremove[0]='\0';bantimeout[0]='\0';
   autoops[0]='\0';
   if((fp = fopen(REG_FILE, "r")) == NULL)
   {
      file_error(REG_FILE);
      return;
   }
   while(fgets(buff, 1000, fp) != NULL)
   {
      int first_time = 1;
      strip_newline(buff);
      chan = strtok(buff, " ");
      ownaddr = strtok(NULL, " ");
      nchan = conv_char(chan);
      strlwr(nchan);
      sprintf(buff1, "database/chanopt/%s", nchan);
      if((fp1 = fopen(buff1, "r")) == NULL)
      {
         file_error(buff1);
         continue;
      }
      buff1[0] = '\0';
      while(fgets(buff1, 1000, fp1) != NULL)
      {
         strip_newline(buff1);
         if(field = strchr(buff1, ' ')) *field++ = '\0';
            if(!strcmp(buff1, "STRICTOP"))
            {
              strcpy(strictop, field);
            }
            if(!strcmp(buff1, "LASTSEEN"))
            {
              strcpy(lastseen, field);
            }
            if(!strcmp(buff1, "NOBAN"))
            {
              strcpy(noban, field);
            }
            if(!strcmp(buff1, "CHMODESLEVEL"))
            {
              strcpy(chmodlvl, field);
            }
            if(!strcmp(buff1, "DEFMODES"))
            {
              strcpy(defmodes, field);
            }
            if(!strcmp(buff1, "OWNERNICK"))
            {
              strcpy(ownernick, field);
            }
            if(!strcmp(buff1, "OWNERNAME"))
            {
              strcpy(ownername, field);
            }
            if(!strcmp(buff1, "OWNEREMAIL"))
            {
              strcpy(ownemail, field);
            }
            if(!strcmp(buff1, "WELCOME"))
            {
              strcpy(welcome, field);
            }
            if(!strcmp(buff1, "MOTD"))
            {
              strcpy(motd, field);
            }
            if(!strcmp(buff1, "BANLEVEL"))
            {
              strcpy(banlvl, field);
            }
            if(!strcmp(buff1, "DESCRIPTION"))
            {
              strcpy(desc, field);
            }
            if(!strcmp(buff1, "OPEROP"))
            {
              strcpy(operop, field);
            }
            if(!strcmp(buff1, "NOOPS"))
            {
              strcpy(noops, field);
            }
            if(!strcmp(buff1, "DEFTOPIC"))
            {
              strcpy(deftopic, field);
            }
            if(!strcmp(buff1, "CHANURL"))
            {
              strcpy(url, field);
            }
            if(!strcmp(buff1, "SETUPDATE"))
            {
              created[0] = '\0';
              strcpy(created, field);
            }
            if(!strcmp(buff1, "STAY"))
            {
              strcpy(stay, field);
            }
            if(!strcmp(buff1, "SETWELCOME"))
            {
              strcpy(setwelc, field);
            }
            if(!strcmp(buff1, "NOPURGE"))
            {
              strcpy(setpurge, field);
            }
            if(!strcmp(buff1, "SETVOICE"))
            {
              strcpy(setvoice, field);
            }
            if(!strcmp(buff1, "SUSPEND"))
            {
              strcpy(suspend, field);
            }
            if(!strcmp(buff1, "TOPICLOCK"))
            {
              strcpy(tlock, field);
            }
            if(!strcmp(buff1, "BANMOVE"))
            {
              strcpy(banmove, field);
            }
            if(!strcmp(buff1, "BANREMOVE"))
            {
              strcpy(banremove, field);
            }
            if(!strcmp(buff1, "BANTIMEOUT"))
            {
              strcpy(bantimeout, field);
            }
            if(!strcmp(buff1, "AUTOOPS"))
            {
              strcpy(autoops, field);
              add_regsinfo(chan, atol(lastseen), ownaddr, url, deftopic, defmodes, ownernick, ownername, welcome, motd, atol(created), desc, ownemail, atoi(strictop), atoi(noban), atoi(chmodlvl), atoi(banlvl), atoi(operop), atoi(noops), atoi(stay), atoi(setwelc), atoi(setpurge), atoi(setvoice), atoi(suspend), atoi(tlock), atol(banmove), atoi(banremove), atoi(bantimeout), atoi(autoops));
            }
            field[0] = '\0';
      }
      fclose(fp1);
       sprintf(buff2, "database/chanusr/%s", nchan);
			//puts(buff2);
      if((fp2 = fopen(buff2, "r")) == NULL)
      {
         file_error(buff2);
         continue;
      }
      /*STE*/
      while(fgets(buff2, 8192, fp2) != NULL)
      {
				//puts(buff2);
         strip_newline(buff2);
         nick = strtok(buff2, " ");
         lvl = strtok(NULL, " ");
         aut = strtok(NULL, " ");
         prot = strtok(NULL, " ");
         ususpend = strtok(NULL, " ");
         passw = strtok(NULL, " ");
         seen = strtok(NULL, " ");
         add_userinfo(chan, nick, atoi(lvl), atoi(aut), atoi(prot), atoi(ususpend), passw, atol(seen));
         while((addr = strtok(NULL, " ")) != NULL)
         {
            add_user_addrinfo(chan, nick, addr);
         }
      }
      fclose(fp2);
   }
   fclose(fp);
}

void load_banfiles(char *chan)
{
   FILE *fp;
   char buff[10000], *nchan = NULL, rest[10000];
   char *who = NULL, *mask = NULL, *timeset = NULL, *level = NULL, *duration = NULL, *reason = NULL;
   long timeout;

   buff[0]='\0';rest[0]='\0';
      strlwr(chan);
      nchan = conv_char(chan);
      sprintf(buff, "database/chanban/%s", nchan);
      if(access(buff, F_OK) == -1) return;
      if((fp = fopen(buff, "r")) == NULL)
      {
         file_error(buff);
      }
      while(fgets(buff, 1500, fp) != NULL)
      {
         strip_newline(buff);
         if(who = strrchr(buff, ' ')) *who++ = '\0';
         if(mask = strchr(buff, ' ')) *mask++ = '\0';
         if(timeset = strchr(mask, ' ')) *timeset++ = '\0';
         if(level = strchr(timeset, ' ')) *level++ = '\0';
         if(duration = strchr(level, ' ')) *duration++ = '\0';
         if(reason = strchr(duration, ' ')) *reason++ = '\0';
         add_baninfo(buff, mask, atol(timeset), atoi(level), atol(duration), reason, who, 1);
      }
      fclose(fp);
}

void load_infolines(char *chan)
{
   struct regsinfo *regs;
   FILE *fp;
   char buff[10000], *nchan= NULL, *info= NULL, *nick= NULL;

   buff[0]='\0';
      strlwr(chan);
      nchan = conv_char(chan);
      sprintf(buff, "database/chaninfo/%s", nchan);
      if(access(buff, F_OK) == -1) return;
      if((fp = fopen(buff, "r")) == NULL)
      {
         file_error(buff);
      }
      while(fgets(buff, 1500, fp) != NULL)
      {
         strip_newline(buff);
         if(info = strchr(buff, ' ')) *info++ = '\0';
         add_infoinfo(chan, buff, info);
      }
      fclose(fp);
}

void load_commands()
{
   FILE *fp;
   char buff[10000], *command= NULL, *funct= NULL, *level= NULL, *disable= NULL, *rest= NULL, cmds_out[150];
   int i = 0;

   buff[0]='\0';cmds_out[0]='\0';
   strcpy(buff, COM_FILE);
   if((fp = fopen(buff, "r")) == NULL)
   {
      file_error(buff);
      return;
   }
   while(fgets(buff, 100, fp) != NULL)
   {
      i = 0;
      if(buff[0] == '*') continue;
      strip_newline(buff);
      if(funct = strchr(buff, ' ')) *funct++ = '\0';
      while(*funct == ' ') *funct++;
      if(level = strchr(funct, ' ')) *level++ = '\0';
      while(*level == ' ') *level++;
      if(disable = strchr(level, ' ')) *disable++ = '\0';
      while(*disable == ' ') *disable++;
      if(rest = strchr(disable, ' '));
      for(i = 0; *commands[i].name; i++)
      {
         if(!strcasecmp(commands[i].name, funct))
         {
            strcpy(commands[i].name, buff);
            strcpy(commands[i].realname, funct);
            commands[i].level = atoi(level);
            commands[i].disable = atoi(disable);
         }
      }
   }
   fclose(fp);
}
