/* CS5 - Channel Services src/mailreg.c
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

void mailreg(char *who, char *rest)
{
   struct regsinfo *regs, *tmp;
   struct userinfo *user;
   struct mailinfo *mailcheck;

   FILE *userf, *regf;
   char *chan, *email, ip_addr[100];
   char *botnum = NULL, *uhost = NULL, usrhost[100], *caddr=NULL, *nwho = NULL;
   char buhost[100], duhost[100], buff[300], *nickhost = NULL, *helpname;
   char *ptr = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL;
   int howmany = 0, i = 1;
   char  test1[10000], *chan1 = NULL;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   strcpy(test1, chan);
   chan1 = test1;

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
   if(strlen(chan) > 200) chan[199] = '\0';
   if(*chan == '+')
   {
      send_to_server(":%s NOTICE %s :Vous ne pouvez pas enregistrer un salon sans modes!\n", bot.nick, nwho);
      return;
   }
   if(chan_check(chan, who) == -1) return;
   if(check_chan(chan)) 
   {
      send_to_server(":%s NOTICE %s :Le salon est déjà enregistré.\n", bot.nick, nwho);
      return;
   }
   botnum=get_usernum(bot.nick);
   email = strtok(NULL, " ");
   helpname = get_command_name("register");
   if(email == NULL || !strchr(email, '@')) 
   {
      do_help(who, helpname);
      return;
   }
   if(strlen(email) > 100)
   {
      send_to_server(":%s NOTICE %s :L'adresse e-mail est trop longue.\n", bot.nick, nwho);
      return;
   }
   if((uhost = getuserhost(who)) == NULL) return;
   ptr = strchr(uhost, '@'); 
   ptr++;
   if(!is_ip(ptr)) 
   {
      if(ptr = strchr(uhost, '@')) *ptr++ = '\0';
      strcpy(duhost, uhost);
      howmany = countdots(ptr);
      if(strlen(uhost) > 8)
      {
         uhost[8] = '*';
         uhost[9] = '\0';
      }

      if(howmany == 1)
      {
          sprintf(usrhost, "*!*%s", uhost);
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
   mailcheck = return_mailinfo(usrhost);
   if(mailcheck == NULL)
   {
      add_mailinfo(chan, who, email, usrhost, NULL, time(NULL));
      for(i = 1; i <= supporters; i++)
      {
         add_suppinfo(usrhost, i);
      }
   }
   else
      update_mailinfo(chan, who, email, usrhost, NULL, time(NULL));

   send_to_server(":%s NOTICE %s : %s %s Progression de l'enregistrement [1]:\n", bot.nick, nwho, regsuffix, bot.nick);
   send_to_server(":%s NOTICE %s : -------------------------------------\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s : Nom du salon:         %s\n", bot.nick, nwho, chan);
   send_to_server(":%s NOTICE %s : Votre pseudo:        %s\n", bot.nick, nwho, who);
   send_to_server(":%s NOTICE %s : Votre e-mail:       %s\n", bot.nick, nwho, email);
   send_to_server(":%s NOTICE %s : Votre masque:      %s\n", bot.nick, nwho, usrhost);
   send_to_server(":%s NOTICE %s : -------------------------------------\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s : Veuillez taper /msg %s PURPOSE <purpose>\n", bot.nick, nwho, bot.nick);
   send_to_server(":%s NOTICE %s :       Avec une courte description de votre\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s :       salon et son but.\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s : NOTE: Si cette information est incorrecte\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s :       tapez /msg %s avec la même commande\n", bot.nick, nwho, bot.nick);
   send_to_server(":%s NOTICE %s :       et les informations correctes\n", bot.nick, nwho);
}

void update_mailinfo(char *chan, char *idnick, char *email, char *ownermask, char *purpose, long created)
{
   struct mailinfo *mail;

   mail = mailhead;
   while(mail != NULL)
   {
      if(!strcasecmp(ownermask, mail->ownermask))
      {
         if(idnick != NULL)
            strcpy(mail->idnick, idnick);
         if(email != NULL)
            strcpy(mail->email, email);
         if(purpose != NULL)
            strcpy(mail->purpose, purpose);
         mail->created = created;
         return;
      }
      mail = mail->next;
   }
}

void purpose(char *who, char *rest)
{
   struct mailinfo *mailcheck;
   char *uhost, *nwho, *usrhost;
   
   nwho = nick2num(who);
   if(strlen(rest) < 4)
   {
      send_to_server(":%s NOTICE %s : Le but specifié est trop court!\n", bot.nick, nwho);
      return;
   }
   if(strlen(rest) > 100)
   {
      send_to_server(":%s NOTICE %s : Le but specifié est trop long! Il est limité à 99 caractères\n", bot.nick, nwho);
      return;
   }
   uhost = getuserhost(who);
   usrhost = get_ban_mask(uhost);
   mailcheck = return_mailinfo(usrhost);
   if(mailcheck->channel == NULL)
   {
      send_to_server(":%s NOTICE %s : Vous n'avez pas fini le stade [1] du processus d'enregistrement\n", bot.nick, nwho);
      send_to_server(":%s NOTICE %s : /msg %s help REGISTER\n", bot.nick, nwho, bot.nick);
       return;
   }
   update_mailinfo(NULL, NULL, NULL, usrhost, rest, time(NULL));
   send_to_server(":%s NOTICE %s : %s %s Progression de l'enregistrement [2]:\n", bot.nick, nwho, regsuffix, bot.nick);
   send_to_server(":%s NOTICE %s : -------------------------------------\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s : Nom du salon:    %s\n", bot.nick, nwho, mailcheck->channel);
   send_to_server(":%s NOTICE %s : But du salon: %s\n", bot.nick, nwho, rest);
   send_to_server(":%s NOTICE %s : -------------------------------------\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s : NOTE: Vous devez avoir %d supporteurs, thus\n", bot.nick, nwho, supporters);
   send_to_server(":%s NOTICE %s :       Tapez /msg %s %s nombre de fois avec le\n", bot.nick, nwho, bot.nick, bot.nick, supporters);
   send_to_server(":%s NOTICE %s :       Nombre des supporteurs et leurs infos\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s : Exemple:\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s :  /msg %s SUPPORTER 1 Billy bgates@microsoft.com\n", bot.nick, nwho, bot.nick);
   send_to_server(":%s NOTICE %s :  /msg %s SUPPORTER 2 Clinton bclinton@whitehouse.gov\n", bot.nick, nwho, bot.nick);
   send_to_server(":%s NOTICE %s :  /msg %s SUPPORTER 3 Alice alice@wonderland.com\n", bot.nick, nwho, bot.nick);
   send_to_server(":%s NOTICE %s :  ... etc (NOTE: Vous devez spécifier %d entrées DIFFERENTES\n", bot.nick, nwho, supporters);
   send_to_server(":%s NOTICE %s :           si vous faites des fautes, retapez la même commande\n", bot.nick, nwho);
   send_to_server(":%s NOTICE %s :           avec les informations corrigées et le même nombre)\n", bot.nick, nwho);
}

void supporter(char *who, char *rest)
{
   struct mailinfo *mail, *mailcheck;
   struct suppinfo *supp;
   char *uhost, *nwho, *usrhost, *nick, *email, *helpname;
   int cnt = 1;

   nwho = nick2num(who);
   uhost = getuserhost(who);
   usrhost = get_ban_mask(uhost);
   mailcheck = return_mailinfo(usrhost);
   helpname = get_command_name("register");
   if(mailcheck->channel == NULL)
   {
      send_to_server(":%s NOTICE %s : Vous n'avez pas fini le stade [1] du processus d'enregistrement\n", bot.nick, nwho);
      send_to_server("%s NOTICE %s : -\n", bot.nick, nwho);
      return;
   }
   if(mailcheck->purpose == NULL)
   {
      send_to_server(":%s NOTICE %s : Vous n'avez pas fini le stade [2] du processus d'enregistrement\n", bot.nick, nwho);
      send_to_server("%s NOTICE %s : -\n", bot.nick, nwho);
      return;
   }
   if(nick = strchr(rest, ' ')) *nick++ = '\0';
   else
   {
      send_to_server("%s NOTICE %s : Syntaxe incorrecte\n", bot.nick, nwho);
      send_to_server("%s NOTICE %s : Ex. /msg %s SUPPORTER 1 Billy bgates@microsoft.com\n", bot.nick, nwho, bot.nick);
      return;
   }
   if(!is_num(rest) || atoi(rest) > supporters)
   {
      send_to_server("%s NOTICE %s : %s n'est pas une valeur valide qui peut être utilisée pour le nombre de supporteurs\n", bot.nick, nwho, rest);
      send_to_server("%s NOTICE %s : -\n", bot.nick, nwho);
      return;
   }
   if(email = strchr(nick, ' ')) *email++ = '\0';
   else
   {
      send_to_server("%s NOTICE %s : Syntaxe incorrecte\n", bot.nick, nwho);
      send_to_server("%s NOTICE %s : Ex. /msg %s SUPPORTER 1 Billy bgates@microsoft.com\n", bot.nick, nwho, bot.nick);
      return;
   }
   if(!strchr(email, '@') && strchr(email, '.'))
   {
      send_to_server("%s NOTICE %s : %s n'est pas une adresse e-mail valide\n", bot.nick, nwho, email);
      send_to_server("%s NOTICE %s : -\n", bot.nick, nwho);
      return;
   }
   mail = mailhead;
   while(mail != NULL)
   {
      if(!strcasecmp(usrhost, mail->ownermask))
      {
         supp = mail->supphead;
         while(supp != NULL)
         {
            if(atoi(rest) == supp->suppnum)
            {
               strcpy(supp->nick, nick);
               strcpy(supp->email, email);
               cnt = suppcount(usrhost);
               if(cnt == supporters)
               {
		  send_to_server(":%s NOTICE %s : Le stade [3] a été complété\n", bot.nick, nwho);
		  send_to_server(":%s NOTICE %s :Si toutes les informations sont correctes\n", bot.nick, nwho);
		  send_to_server(":%s NOTICE %s :Veuillez taper /msg %s SENDREG\n", bot.nick, nwho, bot.nick);
		  send_to_server(":%s NOTICE %s :Ceci enverra votre demande\n", bot.nick, nwho);
                  return;
               }
               else
               {
		  send_to_server(":%s NOTICE %s : Vous avez fini [%d] sur les [%d] supporteurs demandés\n", bot.nick, nwho, cnt, supporters);
                  return;
               }
            }
            supp = supp->next;
         }
      }
      mail = mail->next;
   }
}   

int suppcount(char *usrhost)
{
   struct mailinfo *mail;
   struct suppinfo *supp;
   int i = 0;

   mail = mailhead;
   while(mail != NULL)
   {
      if(!strcasecmp(usrhost, mail->ownermask))
      {
         supp = mail->supphead;
         while(supp != NULL)
         {
            if(supp->nick != NULL && *supp->nick != '\0')
              i++;
            supp = supp->next;
         }
         return i;
      }
      mail = mail->next;
   }
}

void mail_it(char *who, char *rest)
{
   struct mailinfo *mailcheck, *mail;
   struct suppinfo *supp;
   struct userinfo *user;
   struct regsinfo *regs;
   char *uhost, *usrhost, buff[10000], send_mail[4096], *nwho, *nchan, regit[1000], *nickhost;
   int cnt = 0;
   FILE *mailfile;

   nwho = nick2num(who);
   uhost = getuserhost(who);
   usrhost = get_ban_mask(uhost);
   mailcheck = return_mailinfo(usrhost);
   if(mailcheck->channel == NULL)
   {
      send_to_server(":%s NOTICE %s : Vous n'avez pas fini le stade [1] du processus d'enregistrement\n", bot.nick, nwho);
       send_to_server(":%s NOTICE %s : /msg %s help REGISTER\n", bot.nick, nwho, bot.nick);
       return;
   }
   if(mailcheck->purpose == NULL)
   {
      send_to_server(":%s NOTICE %s : Vous n'avez pas fini le stade [2] du processus d'enregistrement\n", bot.nick, nwho);
      send_to_server("%s NOTICE %s : -\n", bot.nick, nwho);
      return;
   }
   cnt = suppcount(usrhost);
   if(cnt < supporters)
   {
      send_to_server(":%s NOTICE %s : Vous n'avez pas fini le stade [3] du processus d'enregistrement\n", bot.nick, nwho);
      send_to_server(":%s NOTICE %s : Vous avez seulement fait [%d] sur les [%d] suppporteurs requis\n", bot.nick, nwho, cnt, supporters);
      return;
   }
   
   mail = mailhead;
   while(mail != NULL)
   {
      if(!strcasecmp(usrhost, mail->ownermask))
      {
         nchan=conv_char(mail->channel);
         sprintf(buff, "database/regmail/%s", nchan);
         strlwr(buff);
         if((mailfile = fopen(buff, "w")) == NULL)
         {
            file_error(buff);
            return;
         }
         nickhost = getnickuserhost(who);
         if(nickhost == NULL) return;
         regs = is_registered(nickhost);
         fprintf(mailfile, "Subject: %s\n", emailsub);
         fprintf(mailfile, "\n");
         if(regs != NULL)
         {
            fprintf(mailfile, "** NOTE ** This user already has channel [%s] registered\n", regs->channel);
         }
         fprintf(mailfile, "\n");
	   fprintf(mailfile, "Nom du salon: [%s]\n", mail->channel);
	   fprintf(mailfile, "Pseudo      : [%s]\n", mail->idnick);
	   fprintf(mailfile, "E-mail      : [%s]\n", mail->email);
	   fprintf(mailfile, "Masque      : [%s]\n", mail->ownermask);
	   fprintf(mailfile, "But         :\n");
         fprintf(mailfile, "--------\n");
         fprintf(mailfile, "%s\n", mail->purpose);
         fprintf(mailfile, "--------\n");
         supp = mail->supphead;
         while(supp != NULL)
         {
            fprintf(mailfile, "     Supporteur:     [%d] %s %s\n", supp->suppnum, supp->nick, supp->email);
            supp = supp->next;
         }
         fclose(mailfile);
         sprintf(send_mail, "%s -f %s@%s %s < %s\n", mailprog, bot.nick, regsuffix, mail->email, buff);
         system(send_mail);
         send_mail[0] = '\0';
         sprintf(send_mail, "%s -f %s@%s %s < %s\n", mailprog, bot.nick, regsuffix, cserv_email, buff);
         system(send_mail);
         if(autoregister && regs == NULL)
         {
            sprintf(regit, "%s %s %s ON OFF %s\n", mail->channel, mail->idnick, mail->ownermask, mail->idnick);
            register_channel(who, regit);
	      send_to_server(":%s NOTICE %s : Le salon a été enregistré et activé!\n", bot.nick, nwho);
	      send_to_server(":%s NOTICE %s : Votre mot de passe est %s\n", bot.nick, nwho, mail->idnick);
	      send_to_server(":%s NOTICE %s : Pour être autorisé tapez /msg %s AUTH %s %s %s\n", bot.nick, nwho, bot.nick, mail->channel, mail->idnick, mail->idnick);
	      send_to_server(":%s NOTICE %s : Veuillez utiliser NEWPASS pour changer votre mot de passe après avoir été autorisé\n", bot.nick, nwho);
            delete_mail(mail->ownermask);
            return;
         }
         else
         {
            if(regs != NULL) 
            {
	      send_to_server(":%s NOTICE %s :Vous ne pouvez pas enregistrer plus d'un salon.\n", bot.nick, nwho);
            }
	      send_to_server(":%s NOTICE %s : Le stade [4] est fini! Un e-mail de cette\n", bot.nick, nwho);
	      send_to_server(":%s NOTICE %s : application sera envoyé aux Admins\n", bot.nick, nwho);
            delete_mail(mail->ownermask);
            return;
         }
      }
      mail = mail->next;
   }
}
