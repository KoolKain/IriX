/* CS5 - Channel Services src/set_chinfo.c
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

int set_chaninfo(char *chan, char *rest, int what)
{
   struct regsinfo *current;

   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         switch(what) 
         {
            case 0: strcpy(current->ownernick, rest);
                    write_single_channel(chan);
                    return;
            case 1: if(current->strictop == 0)
                      current->strictop = 1;
                    else
                      current->strictop = 0;
                    write_single_channel(chan);
                    return;
            case 2: if(current->noban == 0)
                      current->noban = 1;
                    else
                      current->noban = 0;
                    write_single_channel(chan);
                    return;
            case 3: current->chmodlvl = atoi(rest);
                    write_single_channel(chan);
                    return;
            case 4: strcpy(current->defmodes, rest);
                    write_single_channel(chan);
                    return;
            case 5: strcpy(current->ownername, rest);
                    write_single_channel(chan);
                    return;
            case 6: strcpy(current->ownemail, rest);
                    write_single_channel(chan);
                    return;
            case 7: strcpy(current->welcome, rest);
                    write_single_channel(chan);
                    return;
            case 8: current->banlvl = atoi(rest);
                    write_single_channel(chan);
                    return;
            case 9: strcpy(current->desc, rest);
                    write_single_channel(chan);
                    return;
           case 10: if(current->operop == 0)
                      current->operop = 1;
                    else
                      current->operop = 0;
                    write_single_channel(chan);
                    return;
           case 11: if(current->noops == 0)
                      current->noops = 1;
                    else
                      current->noops = 0;
                    write_single_channel(chan);
                    return;
           case 12: strcpy(current->deftopic, rest);
                    write_single_channel(chan);
                    return;
           case 13: strcpy(current->url, rest);
                    write_single_channel(chan);
                    return;
           case 14: if(current->stay == 0)
                      current->stay = 1;
                    else
                      current->stay = 0;
                    write_single_channel(chan);
                    return;
           case 15: if(current->setwelc == 0)
                      current->setwelc = 1;
                    else
                      current->setwelc = 0;
                    write_single_channel(chan);
                    return;
           case 16: if(current->setvoice == 0)
                      current->setvoice = 1;
                    else
                      current->setvoice = 0;
                    write_single_channel(chan);
                    return;
           case 17: if(current->suspend == 0)
                      current->suspend = 1;
                    else
                      current->suspend = 0;
                    write_single_channel(chan);
                    return;
           case 18: if(current->tlock == 0)
                      current->tlock = 1;
                    else
                      current->tlock = 0;
                    write_single_channel(chan);
                    return;
           case 19: if(current->setpurge == 0)
                      current->setpurge = 1;
                    else
                      current->setpurge = 0;
                    write_single_channel(chan);
                    return;
           case 20: current->banmove = atoi(rest);
                    write_single_channel(chan);
                    return;
           case 21: current->banremove = atoi(rest);
                    write_single_channel(chan);
                    return;
           case 22: current->bantimeout = atol(rest);
                    write_single_channel(chan);
                    return;
           case 23: if(current->autoops == 0)
                      current->autoops = 1;
                    else
                      current->autoops = 0;
                    write_single_channel(chan);
                    return;
           case 24: strcpy(current->motd, rest);
                    write_single_channel(chan);
                    return;
         }
      }
      current = current->next;
   }
}

void set_welcome(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(rest, ptr, SETWELCOME);
   regs = return_regsinfo(chan);
   if(regs->setwelc == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :Le message de bienvenue a été mis à %s\n",bot.nick, nwho, what);
}

void set_ownername(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *nwho, test[10000];
   
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   strlwr(chan);
   if(strlen(ptr) > 100) ptr[99] = '\0';
   set_chaninfo(chan, ptr, OWNERNAME);
   regs = return_regsinfo(rest);
      send_to_server(":%s NOTICE %s :Le propriétaire de %s est %s\n", bot.nick, nwho, rest, ptr);
}

void set_url(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   strlwr(chan);
   if(strlen(ptr) > 300) ptr[299] = '\0';
   set_chaninfo(chan, ptr, CHANURL);
   regs = return_regsinfo(rest);
      send_to_server(":%s NOTICE %s :Le site du salon %s est %s\n", bot.nick, nwho, rest, regs->url);
}

void set_default_modes(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   strlwr(chan);
   if(strlen(ptr) > 100) ptr[99] = '\0'; 
   set_chaninfo(chan, ptr, DEFMODES);
   regs = return_regsinfo(rest);
   send_to_server(":%s NOTICE %s :Les modes par défaut du salon %s sont %s\n", bot.nick, nwho, rest, regs->defmodes);
   default_modes(chan);
}

void set_voice(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, SETVOICE);
   regs = return_regsinfo(chan);
   if(regs->setvoice == 1) what = "ON";
   else what = "OFF";
      send_to_server(":%s NOTICE %s :L'option SETVOICE a été mise à %s\n",bot.nick, nwho, what);
}

void suspend_channel(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000], *usernum;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, SUSPEND);
   regs = return_regsinfo(chan);
   if(regs->suspend == 1) what = "ON";
   else what = "OFF";
      send_to_server(":%s NOTICE %s :L'option SUSPEND du salon %s a été mise à %s\n",bot.nick, nwho, chan, what);
   if(regs->suspend)
      send_to_server(":%s PART %s\n", bot.nick, chan);
   else
   {
      usernum=get_usernum(bot.nick);
      send_to_server(":%s JOIN %s\n", bot.nick, chan);
      add_joininfo(chan, usernum, TRUE);
      send_to_server(":%s MODE %s +o %s\n", myserver, chan, usernum);
      send_to_server(":%s TOPIC %s\n", bot.nick, chan);
      send_to_server(":%s MODE %s\n", bot.nick, chan);
   }
}

void set_tlock(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, TLOCK);
   regs = return_regsinfo(chan);
   if(regs->tlock == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :La protection du topic pour %s a été mise à %s\n",bot.nick, nwho, chan, what);
}

void set_default_topic(char *who, char *rest)
{
   struct regsinfo *tmp;
   char *ptr, *chan, *nwho, test[10000];

   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   if(strlen(ptr) > 300) ptr[299] = '\0';
   tmp = return_regsinfo(chan);;
   if(tmp->tlock)
   {
	 send_to_server(":%s NOTICE %s :Le topic du salon est protégé\n",bot.nick, nwho);
      return;  
   }
   set_chaninfo(rest, ptr, DEFTOPIC);
   default_topic(rest);
}

void make_motd(char *who, char *rest)
{
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else
   {
      set_chaninfo(chan, " ", MOTD);
      send_to_server(":%s NOTICE %s :Message du jour hors service\n",bot.nick, nwho);
      return;  
   } 
   if(strlen(ptr) > 300) ptr[299] = '\0';
   set_chaninfo(chan, ptr, MOTD);
   show_motd(who, chan);
}

void make_welcome(char *who, char *rest)
{
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   if(strlen(ptr) > 300) ptr[299] = '\0';
   set_chaninfo(chan, ptr, WELCOME);
   show_welcome(who, chan);
}

void purge_flag(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   ptr = "0";
   set_chaninfo(chan, ptr, NOPURGE);
   regs = return_regsinfo(chan);
   if(regs->setpurge == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option NOPURGE du salon %s a été mise à %s\n",bot.nick, nwho, chan, what);
}

void set_strictop(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, STRICTOP);
   regs = return_regsinfo(chan);
   if(regs->strictop == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option STRICTOP a été mise à %s\n",bot.nick, nwho, what);
}

void set_noban(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000], clearem[1200];
   int cmd_lvl = 0;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, NOBAN);
   regs = return_regsinfo(chan);
   if(regs->noban == 1)
   {
      clearban(who,chan);
   }
   if(regs->noban == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option NOBAN a été mise à %s\n",bot.nick, nwho, what);
}

void set_stay(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, STAY);
   regs = return_regsinfo(chan);
   if(regs->stay == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option STAY a été mise à %s\n",bot.nick, nwho, what);
}

void set_noops(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, NOOPS);
   regs = return_regsinfo(chan);
   if(regs->noops == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option NOOPS a été mise à %s\n",bot.nick, nwho, what);
}

void set_autoops(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, AUTOOPS);
   regs = return_regsinfo(chan);
   if(regs->autoops == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option AUTOOPS a été mise à %s\n",bot.nick, nwho, what);
}

void set_operop(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *what, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = "0";
   set_chaninfo(chan, ptr, OPEROP);
   regs = return_regsinfo(chan);
   if(regs->operop == 1) what = "ON";
   else what = "OFF";
   send_to_server(":%s NOTICE %s :L'option OPEROPS a été mise à %s\n",bot.nick, nwho, what);
}

void set_desc(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *nwho, test[1000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   if(strlen(ptr) > 300) ptr[299] = '\0';
   set_chaninfo(chan, ptr, DESCRIPTION);
   regs = return_regsinfo(rest);
   send_to_server(":%s NOTICE %s :Description - %s\n",bot.nick, nwho, regs->desc);
}

void set_ownernick(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   if(strlen(ptr) > 9) ptr[9] = '\0';
   set_chaninfo(chan, ptr, OWNERNICK);
   regs = return_regsinfo(rest);
   send_to_server(":%s NOTICE %s :Le pseudo du propriétaire du salon est maintenant - %s\n",bot.nick, nwho, regs->ownernick);
}

void set_owneremail(char *who, char *rest)
{
   struct regsinfo *regs;
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   if(ptr = strchr(rest, ' ')) *ptr++ = '\0';
   else return;
   if(strlen(ptr) > 100) ptr[99] = '\0';
   set_chaninfo(chan, ptr, OWNEREMAIL);
   regs = return_regsinfo(rest);
   send_to_server(":%s NOTICE %s :E-mail - %s\n",bot.nick, nwho, regs->ownemail);
}

void set_chmodelvl(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user, *admn;
   struct authinfo *auth;
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = strtok(NULL, " ");
   if(!is_num(ptr))
   {
	send_to_server(":%s NOTICE %s :Ce doit être une valeur numérique\n",bot.nick, nwho);
      return;
   }
   user = return_userinfo(who, chan);
   auth = return_authinfo(csadminchan, who);
   admn = return_userinfo(who, csadminchan);
   if(admn != NULL && auth != NULL && admn->level < atoi(ptr))
   {
	send_to_server(":%s NOTICE %s :Cette valeur est plus élevée que votre accès\n",bot.nick, nwho);
      return;
   }
   else if(user != NULL && user->level <= atoi(ptr))
   {
      send_to_server(":%s NOTICE %s :Cette valeur est plus élevée que votre accès\n",bot.nick, nwho);
      return;
   }
   if(atoi(ptr) < 0) return;
   set_chaninfo(chan, ptr, CHMODESLEVEL);
   regs = return_regsinfo(chan);
   send_to_server(":%s NOTICE %s :L'option CHMODELEVEL a été mise à %d\n",bot.nick, nwho, regs->chmodlvl);
}

void set_bantimeout(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   char *ptr, *chan, *nwho, test[10000], *duration, *units, newtime[10];
   long tmptime;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = strtok(NULL, " ");
   if(!strchr(ptr, ':'))
   {
      if(*ptr == '0')
      {
         set_chaninfo(chan, "0", BANTIMEOUT);
	 send_to_server(":%s NOTICE %s :L'option BANTIMEOUT a été mise à: jamais\n",bot.nick, nwho);
      }
      else
	 send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   if(*ptr == ':')
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   duration = strtok(ptr, ":");
   units = strtok(NULL, " ");
   if(units == NULL)
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   if(strlen(duration) > 10 && strlen(units) > 10)
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide, il est trop long\n", bot.nick, nwho);
      return;
   }
   if(is_num(duration))
   {
      tmptime = convert_duration(units, atol(duration));
      if(time(NULL) + tmptime > 999999999 || tmptime < 0)
      {
	 send_to_server(":%s NOTICE %s : Temps spécifié invalide, il est trop long\n", bot.nick, nwho);
         return;
      }
   }
   else
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   sprintf(newtime, "%ld", tmptime);
   set_chaninfo(chan, newtime, BANTIMEOUT);
   regs = return_regsinfo(chan);
   send_to_server(":%s NOTICE %s :L'option BANTIMEOUT a été mise à %s %s\n",bot.nick, nwho, duration, units);
}

void set_banremove(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   char *ptr, *chan, *nwho, test[10000], *duration, *units, newtime[10];
   long tmptime;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = strtok(NULL, " ");
   if(!strchr(ptr, ':'))
   {
      if(!strcasecmp(ptr, "OFF"))
      {
         set_chaninfo(chan, "0", BANREMOVE);
	 send_to_server(":%s NOTICE %s :L'option BANMOVE a été mise à OFF\n",bot.nick, nwho);
      }
      else if(*ptr == '0')
      {
         set_chaninfo(chan, "0", BANREMOVE);
	 send_to_server(":%s NOTICE %s :L'option BANREMOVE a été mise à OFF\n",bot.nick, nwho);
      }
      else
        send_to_server(":%s NOTICE %s : Invalid Time Specified\n", bot.nick, nwho);
      return;
   }
   if(*ptr == ':')
   {
	 send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   duration = strtok(ptr, ":");
   units = strtok(NULL, " ");
   if(units == NULL)
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   if(strlen(duration) > 10 && strlen(units) > 10)
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   if(is_num(duration))
   {
      tmptime = convert_duration(units, atol(duration));
      if(time(NULL) + tmptime > 999999999 || tmptime < 0)
      {
	 send_to_server(":%s NOTICE %s : Temps spécifié invalide, il est trop long\n", bot.nick, nwho);
         return;
      }
   }
   else
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   sprintf(newtime, "%ld", tmptime);
   set_chaninfo(chan, newtime, BANREMOVE);
   set_chaninfo(chan, "-1", BANMOVE);
   regs = return_regsinfo(chan);
   send_to_server(":%s NOTICE %s :L'option BANMOVE a été mise à OFF\n",bot.nick, nwho);
   send_to_server(":%s NOTICE %s :L'option BANREMOVE a été mise à %s %s\n",bot.nick, nwho, duration, units);
   if(!is_num(ptr))
   {
      send_to_server(":%s NOTICE %s :This must be a numerical value\n",bot.nick, nwho);
      return;
   }
   if(atoi(ptr) < 0) return;
   set_chaninfo(chan, ptr, BANREMOVE);
   set_chaninfo(chan, "-1", BANMOVE);
   regs = return_regsinfo(chan);
   send_to_server(":%s NOTICE %s :L'option BANREMOVE a été mise à %s %s\n",bot.nick, nwho, duration, units);
   send_to_server(":%s NOTICE %s :L'option BANMOVE a été mise à OFF\n",bot.nick, nwho, regs->banmove);
}

void set_banmove(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   char *ptr, *chan, *nwho, test[10000], *duration, *units, newtime[10];
   long tmptime;

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = strtok(NULL, " ");
   if(!strchr(ptr, ':'))
   {
      if(!strcasecmp(ptr, "OFF"))
      {
         set_chaninfo(chan, "-1", BANMOVE);
	 send_to_server(":%s NOTICE %s :L'option BANMOVE a été mise à OFF\n",bot.nick, nwho);
      }
      else if(*ptr == '0')
      {
         set_chaninfo(chan, "0", BANMOVE);
         set_chaninfo(chan, "0", BANREMOVE);
	 send_to_server(":%s NOTICE %s :L'option BANMOVE a été mise à: toujours\n",bot.nick, nwho);
	 send_to_server(":%s NOTICE %s :L'option BANREMOVE a été mise à OFF\n",bot.nick, nwho);
      }
      else
	 send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   if(*ptr == ':')
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   duration = strtok(ptr, ":");
   units = strtok(NULL, " ");
   if(units == NULL)
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   if(strlen(duration) > 10 && strlen(units) > 10)
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide, il est trop long\n", bot.nick, nwho);
      return;
   }
   if(is_num(duration))
   {
      tmptime = convert_duration(units, atol(duration));
      if(time(NULL) + tmptime > 999999999 || tmptime < 0)
      {
	 send_to_server(":%s NOTICE %s : Temps spécifié invalide, il est trop long\n", bot.nick, nwho);
         return;
      }
   }
   else
   {
      send_to_server(":%s NOTICE %s : Temps spécifié invalide\n", bot.nick, nwho);
      return;
   }
   sprintf(newtime, "%ld", tmptime);
   set_chaninfo(chan, newtime, BANMOVE);
   set_chaninfo(chan, "0", BANREMOVE);
   regs = return_regsinfo(chan);
   send_to_server(":%s NOTICE %s :L'option BANMOVE a été mise à %s %s\n",bot.nick, nwho, duration, units);
   send_to_server(":%s NOTICE %s :L'option BANREMOVE a été mise à OFF\n",bot.nick, nwho);
}

void set_banlevel(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user, *admn;
   struct authinfo *auth;
   char *ptr, *chan, *nwho, test[10000];

   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nwho = nick2num(who);
   ptr = strtok(NULL, " ");
   if(!is_num(ptr))
   {
      send_to_server(":%s NOTICE %s :Ce doit être une valeur numérique\n",bot.nick, nwho);
      return;
   }
   user = return_userinfo(who, chan);
   auth = return_authinfo(csadminchan, who);
   admn = return_userinfo(who, csadminchan);
   if(admn != NULL && auth != NULL && admn->level < atoi(ptr))
   {
      send_to_server(":%s NOTICE %s :Cette valeur est plus élevée que votre accès\n",bot.nick, nwho);
      return;
   }
   if(user != NULL && user->level < atoi(ptr))
   {
      send_to_server(":%s NOTICE %s :Cette valeur est plus élevée que votre accès\n",bot.nick, nwho);
      return;
   }
   if(atoi(ptr) < 0) return;
   set_chaninfo(chan, ptr, BANLEVEL);
   regs = return_regsinfo(chan);
   send_to_server(":%s NOTICE %s :L'option CHMODELEVEL à été mise à %d\n",bot.nick, nwho, regs->banlvl);
}
