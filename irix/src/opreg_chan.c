/* CS5 - Channel Services src/opreg_chan.c
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

void opregister_channel(char *who, char *rest)
{
   FILE *userf;
   char *nchan = NULL, *nwho = NULL, *chan = NULL, *nick = NULL, *addr = NULL, *pass = NULL, *ptr = NULL, *usernum = NULL, *welcome = NULL;
   char *ownername = NULL, *url = NULL, *deftopic = NULL, defmodes[4], *cryptpass = NULL, *motd = NULL, *desc = NULL, *ownemail = NULL;
   char test1[10000], *chan1 = NULL, taddr[100], *ptr6 = NULL, *ptr7 = NULL, *helpname;

   usernum=get_usernum(bot.nick);
   nwho = nick2num(who);
   chan = strtok(rest, " ");
   if(strlen(chan) > 200) chan[199] = '\0';
   strcpy(test1, chan);
   chan1 = test1;
   helpname = get_command_name("adminreg");

/* NiX
   while(*chan1)
   {
      if(!isascii(*chan1))
      {
	 send_to_server(":%s NOTICE %s : Désolé, nous ne pouvons valider que les caractères ASCII\n", bot.nick, nwho);
        return;
      }
      *chan1++;
   }
*/
   if(*chan == '+')
   {
      send_to_server(":%s NOTICE %s :Nous ne pouvons pas enregistrer un salon sans modes!\n", bot.nick, nwho);
      return;
   }
   if(chan_check(chan, who) == -1) return;
   nick = strtok(NULL, " ");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   addr = strtok(NULL, " ");
   if(addr == NULL)
   {
      do_help(who, helpname);
      return;
   }
   pass = strtok(NULL, " ");
   if(pass == NULL) 
   {
      send_to_server(":%s NOTICE %s :Un mot de passe est requis pour enregistrer un salon!\n", bot.nick, nwho);
      return;
   }
   if(!strchr(addr, '!'))
   {
      send_to_server(":%s NOTICE %s :Masque invalide,  le format est *!*user@host.\n", bot.nick, nwho);
      return;
   }
   strcpy(taddr, addr);
   if(ptr7 = strchr(taddr, '@')) *ptr7++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }

/* NiX
   if(!strcmp(taddr, "*!*"))
   {
      send_to_server(":%s NOTICE %s :Veuillez inclure le username.\n", bot.nick, nwho);
      return;
   }
   if(!strcmp(ptr7, "*") || !strcmp(ptr7, "*.*") || !strcmp(ptr7, "*.*.*"))
   {
      send_to_server(":%s NOTICE %s :Veuillez entrer un hostname plus spécifique.\n", bot.nick, nwho);
      return;
   }
*/
   if(check_chan(chan)) 
   {
      send_to_server(":%s NOTICE %s :Ce salon est déjà enregistré.\n", bot.nick, nwho);
      return;
   }
   if(pass == NULL) pass = "####";
   send_to_server(":%s JOIN %s\n", bot.nick, chan);
   send_to_server(":%s MODE %s -A+o %s\n", myserver, chan, usernum);
   deopall(bot.nick, chan);
   send_to_server(":%s MODE %s -Antspmlk *", bot.nick, chan);
   add_regsinfo(chan, time(NULL), addr, url,"Votre salon vient d'être créer. Veuillez consulter votre messagerie pour avoir vos identifiants (mot de pass IriX). Pour vous authé sur IriX, /msg IriX auth pseudo #votre-salon votre-pass ! ~ Les CSAdmins","+nt", nick, ownername, welcome,"Pour des raisons de sécurité, veuillez ne pas avoir plus de 5 mask à votre access. Faites plutôt comme ceci: votrePseudo!*@* et VotrePseudo|*!*@* avec la commande moduser addmask", time(NULL), desc, ownemail, FALSE, FALSE, 50, 50, FALSE, FALSE, TRUE /* STE */, FALSE, FALSE, FALSE, FALSE, FALSE, -1, FALSE, FALSE, FALSE);
   cryptpass = mkpass(pass, "0");
   add_userinfo(chan, nick, maxuserlevel, FALSE, FALSE, FALSE, cryptpass, time(NULL));
   add_user_addrinfo(chan, nick, addr);
   send_to_server(":%s NOTICE %s :%s est maintenant enregistré sous %s\n", bot.nick, nwho, chan, addr);
   write_regs_list();
   write_single_channel(chan);
   write_single_infolines(chan);
   default_modes(chan);
   default_topic(chan);
   if(deopchan)
   deopall(bot.nick, chan);   
}
