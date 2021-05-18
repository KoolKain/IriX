/* CS5 - Channel Services src/add_chaninfo.c
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

void add_mapinfo(char *line)
{
   struct mapinfo *new, *current;
   new = (struct mapinfo *) calloc(1, sizeof(struct mapinfo));
   if(new != NULL)
   {
      new->next = NULL;
      strcpy(new->mapline, line);
      if(maphead == NULL) maphead = new;
      else
      {
         current = maphead;
         while(current->next != NULL) current = current->next;
         current->next = new;
      }
   }
   else log(ERRLOG_FILE, "[%s] Ajout map: plus de mémoire\n", date());
}

void add_burst(char *channel, char *modes, long chanstamp)
{
   struct burstinfo *new, *current;
   new = (struct burstinfo *) calloc(1, sizeof(struct burstinfo));
   if(new != NULL)
   {
      new->next = NULL;
      strcpy(new->channel, channel);
      strcpy(new->modes, modes);
      new->chanstamp = chanstamp;
      if(bursthead == NULL) bursthead = new;
      else
      {
         current = bursthead;
         while(current->next != NULL) current = current->next;
         current->next = new;
      }
   }
   else log(ERRLOG_FILE, "[%s] Ajout Burst: plus de mémoire\n", date());
}

void add_suppinfo(char *uhost, int suppnum)
{
   struct mailinfo *mail;
   struct suppinfo *current, *new;

   mail = mailhead;
   while(mail != NULL)
   {
      if(!strcasecmp(uhost, mail->ownermask))
      {
            new = (struct suppinfo *) calloc(1, sizeof(struct suppinfo));
            if(new != NULL)
            {
               new->next = NULL;
               new->suppnum = suppnum;
               if(mail->supphead == NULL) mail->supphead = new;
               else
               {
                  current = mail->supphead;
                  while(current->next != NULL) current = current->next;
                  current->next = new;
               }
            }
            else 
            {
               log(ERRLOG_FILE, "[%s] Ajout Addr: plus de mémoire\n", date());
               return;
            }
      }
      mail = mail->next;
   }
}

void add_mailinfo(char *channel, char *idnick, char *email, char *ownermask, char *purpose, long created)
{
   struct mailinfo *new, *current;
   new = (struct mailinfo *) calloc(1, sizeof(struct mailinfo));
   if(new != NULL)
   {
      new->next = NULL;
      if(channel != NULL)
         strcpy(new->channel, channel);
      if(idnick != NULL)
         strcpy(new->idnick, idnick);
      if(email != NULL)
         strcpy(new->email, email);
      if(ownermask != NULL)
         strcpy(new->ownermask, ownermask);
      if(purpose != NULL)
         strcpy(new->purpose, purpose);
      new->created = created;
      new->supphead = NULL;
      if(mailhead == NULL) mailhead = new;
      else
      {
         current = mailhead;
         while(current->next != NULL) current = current->next;
         current->next = new;
      }
   }
   else log(ERRLOG_FILE, "[%s] Ajout mailinfo: plus de mémoire\n", date());
}

void add_templist(char *chan)
{
   struct templist *new, *current;
   new = (struct templist *) calloc(1, sizeof(struct templist));
   if(new != NULL)
   {
      new->next = NULL;
      strcpy(new->channel, chan);
      if(listhead == NULL) listhead = new;
      else
      {
         current = listhead;
         while(current->next != NULL) current = current->next;
         current->next = new;
      }
   }
   else log(ERRLOG_FILE, "[%s] Ajout mailinfo: plus de mémoire\n", date());
}

void add_modeinfo(char *chan, char *stamp, char *modes)
{
   struct regsinfo *regs;
 
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(regs->channel, chan))
      {
         strcpy(regs->channel, chan);
         if(stamp)
            strcpy(regs->chanstamp, stamp);
         if(modes)
            strcpy(regs->modes, modes);
         return;
      }
      regs = regs->next;
   }
}

void add_mode(char *chan, char *modes)
{
   struct regsinfo *current;
   if(!ison(chan)) return;
   if(regshead == NULL) return;
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         strcpy(current->modes, modes);
         return;
      }
      current = current->next;
   }
}

void add_topic(char *chan, char *topic)
{
   struct regsinfo *current;
   if(!ison(chan)) return;
   if(regshead == NULL) return;
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         strcpy(current->topic, topic);
         return;
      }
      current = current->next;
   }
}

void add_regsinfo(char *chan, long lastseen, char *ownaddr, char *url, char *deftopic, char *defmodes, char *ownernick, char *ownername, char *welcome, char *motd, long created, char *desc, char *ownemail, int strictop, int noban, int chmodlvl, int banlvl, int operop, int noops, int stay, int setwelc, int setpurge, int setvoice, int suspend, int tlock, long banmove, int banremove, int bantimeout, int autoops)
{
   struct regsinfo *new, *current;
   new = (struct regsinfo *) calloc(1, sizeof(struct regsinfo));
   if(new != NULL) 
   {
      new->next = NULL;
      if(chan != NULL)
         strcpy(new->channel, chan);
      if(ownaddr != NULL)
         strcpy(new->ownaddr, ownaddr);
      if(url != NULL)
         strcpy(new->url, url);
      if(deftopic != NULL)
         strcpy(new->deftopic, deftopic);
      if(defmodes != NULL)
         strcpy(new->defmodes, defmodes);
      if(ownernick != NULL)
         strcpy(new->ownernick, ownernick);
      if(ownername != NULL)
         strcpy(new->ownername, ownername);
      if(welcome != NULL)
         strcpy(new->welcome, welcome);
      if(motd != NULL)
         strcpy(new->motd, motd);
      if(desc != NULL)
         strcpy(new->desc, desc);
      if(ownemail != NULL)
         strcpy(new->ownemail, ownemail);
      new->lastseen = lastseen;
      new->created = created;
      new->strictop = strictop;
      new->noban = noban;
      new->chmodlvl = chmodlvl;
      new->banlvl = banlvl;
      new->operop = operop;
      new->noops = noops;
      new->stay = stay;
      new->setwelc = setwelc;
      new->setpurge = setpurge;
      new->setvoice = setvoice;
      new->suspend = suspend;
      new->tlock = tlock;
      new->banmove = banmove;
      new->banremove = banremove;
      new->bantimeout = bantimeout;
      new->autoops = autoops;
      new->userhead == NULL;
      new->banhead == NULL;
      if(regshead == NULL) regshead = new;
      else 
      {
         current = regshead;
         while(current->next != NULL) current = current->next;
         current->next = new;
      }
   }
   else log(ERRLOG_FILE, "[%s] Ajout channel: plus de mémoire\n", date());
}

int max_level(char *chan, char *ban, int old_lvl)
{
   struct regsinfo *current;
   struct baninfo *bantest;
   int new_lvl = 0;

   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         bantest = current->banhead;
         while(bantest != NULL)
         {
            if(!bancmp(ban, bantest->ban) || !bancmp(bantest->ban, ban))
            {
               if(bantest->level > old_lvl)
               {
                  old_lvl = bantest->level;
               }
            }
            bantest = bantest->next;
         }
      }
      current = current->next;
   }
   new_lvl = old_lvl;
   return new_lvl;
}

long max_duration(char *chan, char *ban, long old_dur)
{
   struct regsinfo *current;
   struct baninfo *bantest;
   long new_dur;

   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         bantest = current->banhead;
         while(bantest != NULL)
         {
            if(!bancmp(ban, bantest->ban) || !bancmp(bantest->ban, ban))
            {
               if(bantest->duration > old_dur)
               {
                  old_dur = bantest->duration;
               }
            }
            bantest = bantest->next;
         }
      }
      current = current->next;
   }
   new_dur = old_dur;
   return new_dur;
}

int add_baninfo(char *chan, char *ban, long timeset, int level, long duration , char *reason, char *nick, int botban)
{
   struct regsinfo *regs, *regstest;
   struct baninfo *new, *current, *bantest;
   char rban[100];
   int got_one = 0, new_lvl = 0;
   long new_dur;
 
   rban[0] = '\0';
   collapse(ban);
   regstest = regshead;
   while(regstest != NULL)
   {
      if(!strcasecmp(chan, regstest->channel))
      {
         bantest = regstest->banhead;
         while(bantest != NULL)
         {
           if(!strcasecmp(ban, bantest->ban) && botban && !joined)
           {
              strcpy(bantest->ban, ban);
              strcpy(bantest->nick, nick);
              bantest->timeset = timeset;
              bantest->level = level;
              bantest->duration = duration;
              bantest->botban = botban;
              if(reason != NULL)
                strcpy(bantest->reason, reason);
              return;
           }
           if(!strcasecmp(ban, bantest->ban)) return 1;
           if(!bancmp(ban, bantest->ban) || !bancmp(bantest->ban, ban))
           {
              if(got_one && strlen(ban) < strlen(bantest->ban))
              {
                 delete_bnode(regstest->channel, bantest->ban);   
              }
              else if(strlen(ban) < strlen(bantest->ban))
              {
                 if(duration != 0)
                    new_dur = max_duration(regstest->channel, bantest->ban, duration);
                 else
                    new_dur = duration;
                 new_lvl = max_level(regstest->channel, bantest->ban, level);
                 strcpy(bantest->ban, ban);
                 strcpy(bantest->nick, nick);
                 bantest->timeset = timeset;
                 if(!botban)
                    bantest->level = regstest->banlvl;
                 else
                    bantest->level = new_lvl;
                 bantest->duration = new_dur;
                 bantest->botban = botban;
                 if(reason != NULL)
                   strcpy(bantest->reason, reason);
                 got_one = 1;
              }
           }
           bantest = bantest->next;
         }
         if(got_one) return;
      }
      regstest = regstest->next;
   }
   if(regshead == NULL) return;
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
         new = (struct baninfo *) calloc(1, sizeof(struct baninfo));
         if(new != NULL)
         {
            new->next = NULL;
            strcpy(new->ban, ban);
            strcpy(new->nick, nick);
            new->timeset = timeset;
            new->level = level;
            new->duration = duration;
            new->botban = botban;
            if(reason != NULL)
              strcpy(new->reason, reason);
            if(regs->banhead == NULL) regs->banhead = new;
            else
            {
               current = regs->banhead;
               while(current->next != NULL) current = current->next;
               current->next = new;
            }
         }
         else log(ERRLOG_FILE, "[%s] Ajout ban: plus de mémoire\n", date());
      }
      regs = regs->next;
   }
   return 0;
}
