/* CS5 - Channel Services src/chaninfo.c
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


void show_chaninfo(char *who, char *rest)
{
   struct nickinfo *nick1, *nick, *nick3, *chantmp;
   struct chaninfo *chans;
   struct regsinfo *current, *regs;
   struct userinfo *user, *usertmp, *admntmp;
   struct baninfo *ban;
   struct authinfo *current1, *authtmp;
   char *nwho = NULL, *what = NULL, *chan = NULL, test[10000], *what1 = NULL, *newmodes1 = NULL;
/* OPUSNOTE: buf sizes changed from 6000 -> 10000 */
   char *userhost = NULL, tmp[10000], tmp1[10000], tmp2[10000], tmp3[10000], oao[10], *tm = NULL;
   int i = 0, j = 0, firsttime = 0, gotit = 0, auth = 0, is_oper = 0, first_time = 0, bm = 0, cnt = 0;

/* OPUSNOTE: added an int for user counter */
   int ncnt = 0, bicnt = 0, bccnt = 0;

   oao[0]='\0';tmp[0] ='\0';tmp1[0] ='\0';tmp2[0] ='\0';tmp3[0] ='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   current = regshead;
   while(current != NULL)
   {
      if(!strcasecmp(chan, current->channel))
      {
         admntmp = return_userinfo(who, csadminchan);
	   send_to_server(":%s NOTICE %s : \002\0032Information pour le salon %s\003\002\n", bot.nick, nwho, chan);
         send_to_server(":%s NOTICE %s : \00314---------------------------------------------------\003\n", bot.nick, nwho);
         if(admntmp != NULL || added_level == maxuserlevel)
         {
	       send_to_server(":%s NOTICE %s : \0034Nom du propriétaire\003  : %s\n", bot.nick, nwho, current->ownername);
	       send_to_server(":%s NOTICE %s : \0034Email du propriétaire\003: %s\n", bot.nick, nwho, current->ownemail);
         }
         else
         {
            send_to_server(":%s NOTICE %s : \0034Owner Name\003     : \002\0034- Info protégée -\003\002\n", bot.nick, nwho);
            send_to_server(":%s NOTICE %s : \0034Owner E-mail\003   : \002\0034- Info protégée -\003\002\n", bot.nick, nwho);
         }
	    send_to_server(":%s NOTICE %s : \0034Nick du propriétaire\003 : %s\n", bot.nick, nwho, current->ownernick);
          send_to_server(":%s NOTICE %s : \0034Dernière présence\003    : %s\n", bot.nick, nwho, get_time(current->lastseen));
	    send_to_server(":%s NOTICE %s : \0034Topic par défaut\003     : %s\n", bot.nick, nwho, current->deftopic);
	    send_to_server(":%s NOTICE %s : \0034Message de bienvenue\003 : %s\n", bot.nick, nwho, current->welcome);
	    send_to_server(":%s NOTICE %s : \0034Description\003          : %s\n", bot.nick, nwho, current->desc);
	    send_to_server(":%s NOTICE %s : \0034Site du salon\003        : %s\n", bot.nick, nwho, current->url);
         send_to_server(":%s NOTICE %s : \00314---------------------------------------------------\003\n", bot.nick, nwho);
         if(current->strictop == 1) what = "\0033ON \003";
         else what = "\0034OFF\003";
         send_to_server(":%s NOTICE %s : \0034Settings\003       : \0032STRICTOP\003    : %s  \0032CHMODELEVEL\003: %03d\n",  bot.nick, nwho, what, current->chmodlvl);
         if(current->noban == 1) what = "\0033ON \003";
         else what = "\0034OFF\003";
         send_to_server(":%s NOTICE %s :                : \0032NOBAN\003       : %s  \0032DEFMODES\003   : %s\n",  bot.nick, nwho, what, current->defmodes);
         if(current->operop == 1) what = "\0033ON \003";
         else what = "\0034OFF\003";
         send_to_server(":%s NOTICE %s :                : \0032OPEROP\003      : %s  \0032BANLEVEL\003   : %03d\n",  bot.nick, nwho, what, current->banlvl);
         if(current->noops == 1) what = "\0033ON \003";
         else what = "\0034OFF\003";
         if(current->stay == 1) what1 = "\0033ON \003";
         else what1 = "\0034OFF\003";
         send_to_server(":%s NOTICE %s :                : \0032NOOPS\003       : %s  \0032STAY\003       : %s\n",  bot.nick, nwho, what, what1);
         if(current->setwelc == 1) what = "\0033ON \003";
         else what = "\0034OFF\003";
         if(current->tlock == 1) what1 = "\0033ON \003";
         else what1 = "\0034OFF\003";
         send_to_server(":%s NOTICE %s :                : \0032WELCOME\003     : %3s  \0032LOCK TOPIC\003 : %s\n",  bot.nick, nwho, what, what1);
         if(current->suspend == 1) what = "\0033ON \003";
         else what = "\0034OFF\003";
         if(current->setvoice == 1) what1 = "\0033ON \003";
         else what1 = "\0034OFF\003";
         send_to_server(":%s NOTICE %s :                : \0032CHAN SUSPEND\003: %s  \0032AUTO VOICE\003 : %s\n",  bot.nick, nwho, what, what1);
         if(current->banmove == -1)
         {
            what = "\0034OFF\003";
            send_to_server(":%s NOTICE %s :                : \0032BANMOVE     \003: %s  \0032BANREMOVE \003 : %03d\n",  bot.nick, nwho, what, current->banremove);
         }
         else 
         {
            if(current->banmove == 0)
            {
               bm = 0;
               tm = " ";
            }
            else
            {
               if(current->banmove <= 3600)
               {
                   bm = current->banmove/60;
                   tm = "m";
               }
               else
               {
                   bm = (current->banmove/60)/60;
                   tm = "h";
               }
            }
            send_to_server(":%s NOTICE %s :                : \0032BANMOVE     \003: %03d%s \0032BANREMOVE \003 : %03d\n",  bot.nick, nwho, bm, tm, current->banremove);
         }
         if(current->autoops == 1) what1 = "\0033ON \003";
         else what1 = "\0034OFF\003";
         send_to_server(":%s NOTICE %s :                : \0032BANTIMEOUT  \003: %03d  \0032AUTO OPS\003   : %s\n",  bot.nick, nwho, current->bantimeout, what1);
         if(admntmp != NULL)
         {
            if(current->setpurge == 1) what = "\0033ON \003";
            else what = "\0034OFF\003";
            send_to_server(":%s NOTICE %s :                : \0032NOPURGE\003     : %s \n",  bot.nick, nwho, what);
         }
         send_to_server(":%s NOTICE %s : \00314---------------------------------------------------\003\n", bot.nick, nwho);
         send_to_server(":%s NOTICE %s : Topic actuel  : %s\n", bot.nick, nwho, current->topic);


         nick = nickhead;
         nick3 = return_nickinfo(who);
         authtmp = return_authinfo(csadminchan, who);
         chantmp = return_chaninfo(who, chan);
         newmodes1 = strtok(current->modes, " ");
         ncnt = user_count(chan);
         if(ncnt < 21)
         while(nick != NULL)
         {
            chans = nick->chanhead;
            while(chans != NULL)
            {
               if(chantmp != NULL || (strchr(nick->modes, 'i') && (chantmp != NULL || nick3->is_oper)) || (!strchr(nick->modes, 'i') && !strchr(current->modes, 's') && !strchr(current->modes, 'p')) || !strcasecmp(who, nick->nick) || authtmp != NULL)
               {
                  if(!strcasecmp(chan,chans->channel) && strcasecmp(nick->nick, bot.nick))
                  {
                     nick1 = nickhead;
                     while(nick1 != NULL)
                     {
                        if(!strcasecmp(nick->nick, nick1->nick))
                        {
                           auth = 0;
                           current1 = nick1->authhead;
                           while(current1 != NULL)
                           {
                              if(!strcasecmp(chans->channel, current1->channel))
                              {
                                 auth = 1;
                              }
                              else auth = 0;
                              current1 = current1->next;
                           }
                        }
                        nick1 = nick1->next;
                     }
                     is_oper = nick->is_oper;
                     strcpy(oao, nick->away);
                     if(auth)
                     {
                        strcat(oao, "\0035$\003");
                     }
                     if(nick->is_oper)
                     {
                        strcat(oao, "\00310*\003");
                     }
                     if(chans->is_ops)
                     {
                       strcat(oao, "\0037@\003");
                     }
                     if(nick->modes != NULL && strcmp(nick->modes, "NONE"))
                     {
                        strcat(oao, nick->modes);
                     }
                     if(!gotit)
                        {
                        tmp1[0] = '\0';
                        sprintf(tmp1, "%s\0034(%s)\003", oao, nick->nick);
                        oao[0] = '\0';
                        tmp[0] = '\0';
                        strcpy(tmp, tmp1);
                     }
                     if(gotit)
                     {
                        tmp1[0] = '\0';
                        sprintf(tmp1, "%s, %s\0034(%s)\003", tmp, oao, nick->nick);
                        oao[0] = '\0';
                        tmp[0] = '\0';
                        strcpy(tmp, tmp1);
                     }
                     gotit = 1;
                     i++;
                     if((i == 5 && firsttime == 0))
	   	     {
                        send_to_server(":%s NOTICE %s : \0034Utilisateurs actuel\003  : %s\n", bot.nick, nwho, tmp);
                        i = 0;
                        firsttime++;
                        gotit = 0;
                        oao[0] = '\0';
                        tmp[0] = '\0';
                        tmp1[0] = '\0';
                     }
                     if((i == 5 && firsttime > 0))
                     {
                        send_to_server(":%s NOTICE %s :                : %s\n", bot.nick, nwho, tmp);
                        i = 0;
                        firsttime++;
                        gotit = 0;
                        oao[0] = '\0';
                        tmp[0] = '\0';
                        tmp1[0] = '\0';
                     }
                  }
               }
               chans = chans->next;
            }
            nick=nick->next;
         }
         if(i < 5 && firsttime > 0 && tmp[0] != '\0' && ncnt < 21) 
            send_to_server(":%s NOTICE %s :                : %s\n", bot.nick, nwho, tmp);
         else if(!firsttime && tmp[0] != '\0' && tmp != NULL && ncnt < 21)
            send_to_server(":%s NOTICE %s : \0034Utilisateurs actuel\003  : %s\n", bot.nick, nwho, tmp);
	 send_to_server(":%s NOTICE %s : \0034No. of Chan Users\003 : %d\n", bot.nick, nwho, ncnt);

         send_to_server(":%s NOTICE %s : \0034Modes actuels\003  : %s\n", bot.nick, nwho, newmodes1);
/*
         oao[0] = '\0';
         tmp[0] = '\0';
         tmp1[0] = '\0';
         firsttime = 0;
         cnt = 0;
         bicnt = ban_icount(chan);
         ban = current->banhead;
         if(bicnt < 11)
         while(ban != NULL)
         {
            if(cnt == 0 && ban->botban)
            {
               sprintf(tmp2, "\0034(\003 %s\0034 )\003", ban->ban);
               cnt++;
            }
            else if(ban->botban)
            {

               sprintf(tmp3, "%s \0034(\003 %s\0034 )\003", tmp2, ban->ban);
               strcpy(tmp2, tmp3);
               cnt++;
            }
            if(cnt == 3 && firsttime == 0 && tmp2[0] != '\0')
            {
               send_to_server(":%s NOTICE %s : \0034Internal Bans\003  : %s\n", bot.nick, nwho, tmp2);
               cnt = 0;
               tmp2[0] = '\0';
               tmp3[0] = '\0';
               firsttime++;
            }
            else if(cnt == 3 && firsttime > 0 && tmp2[0] != '\0' && tmp2 != NULL)
            {
               send_to_server(":%s NOTICE %s :                : %s\n", bot.nick, nwho, tmp2);
               cnt = 0;
               tmp2[0] = '\0';
               tmp3[0] = '\0';
               firsttime++;
            }
            ban = ban->next;
         }         
         if(cnt < 3 && firsttime >= 0 && tmp2[0] != '\0' && bicnt < 11)
            send_to_server(":%s NOTICE %s : \0034Internal Bans\003  : %s\n", bot.nick, nwho, tmp2);
         else if(firsttime > 0 && tmp2[0] != '\0' && bicnt < 11)
             send_to_server(":%s NOTICE %s :                : %s\n", bot.nick, nwho, tmp2);
         else if(bicnt > 11)
             send_to_server(":%s NOTICE %s :Utilisez BANLIST pour voir les bans actuels\n", bot.nick, nwho);
         first_time = 0;
         tmp2[0] = '\0';
         tmp3[0] = '\0';
         firsttime = 0;
         cnt = 0;
         bccnt = ban_ccount(chan);
         ban = current->banhead;
         if(bccnt < 11)
         while(ban != NULL)
         {
            if(cnt == 0 && !ban->botban)
            {
               sprintf(tmp2, "\0034(\003 %s\0034 )\003", ban->ban);
               cnt++;
            }
            else if(!ban->botban)
            {
               sprintf(tmp3, "%s \0034(\003 %s\0034 )\003", tmp2, ban->ban);
               strcpy(tmp2, tmp3);
               cnt++;
            }
            if(cnt == 3 && firsttime == 0 && tmp2[0] != '\0')
            {
               send_to_server(":%s NOTICE %s : \0034Normal Bans\003    : %s\n", bot.nick, nwho, tmp2);
               cnt = 0;
               tmp2[0] = '\0';
               tmp3[0] = '\0';
               firsttime++;
            }
            else if(cnt == 3 && firsttime > 0 && tmp2[0] != '\0' && tmp2 != NULL)
            {
               send_to_server(":%s NOTICE %s :                : %s\n", bot.nick, nwho, tmp2);
               cnt = 0;
               tmp2[0] = '\0';
               tmp3[0] = '\0';
               firsttime++;
            }
            ban = ban->next;
         }         
         if(cnt < 3 && firsttime >= 0 && tmp2[0] != '\0' && bccnt < 11)
            send_to_server(":%s NOTICE %s : \0034Normal Bans\003    : %s\n", bot.nick, nwho, tmp2);
         else if(firsttime > 0 && tmp2[0] != '\0' && bccnt < 11)
            send_to_server(":%s NOTICE %s :                : %s\n", bot.nick, nwho, tmp2);
         else if(bccnt > 11)
*/
         send_to_server(":%s NOTICE %s : Utilisez BANLIST pour voir les bans actuels\n", bot.nick, nwho);
         regs = regshead;
         while(regs != NULL)
         {
            if(!strcasecmp(chan, regs->channel))
            {
               usertmp = regs->userhead;
               while(usertmp != NULL)
               {
                 j++;
                 usertmp = usertmp->next;
               }
               break;
            }
            regs = regs->next;
         }
         send_to_server(":%s NOTICE %s : \0034Il y a %d utilisateur(s) registered on this channel\003\n", bot.nick, nwho, j);
      }
      current = current->next;
   }
}
