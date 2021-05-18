/* CS5 - Channel Services src/misc_cmds.c
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

void kick_ban_all(char *who, char *rest)
{
   struct nickinfo *nick;
   struct chaninfo *current;
   char *nwho = NULL, *chan = NULL, *banmask = NULL, test[10000], *reason = NULL, *uhost = NULL;
   
   test[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if((reason = strchr(rest, ' ')) != NULL) *reason++ = '\0';
   else
   {
      send_to_server(":%s NOTICE %s :Vous devez spécifier une raison de kick, lisez l'aide\n", bot.nick, nwho);
      return;
   }
   nick = nickhead;
   while(nick != NULL)
   {
      current = nick->chanhead;
      while(current != NULL)
      {
         if(!strcasecmp(rest, current->channel) && strcasecmp(nick->nick, who) && strcasecmp(nick->nick, bot.nick))
         {
            if(uhost = getuserhost(nick->nick));
            if(uhost != NULL)
            {
               banmask = get_ban_mask(uhost);
               send_to_server(":%s MODE %s -o+b %s %s\n", bot.nick, rest, nick->numeric, banmask);
               send_to_server(":%s KICK %s %s :%s\n", bot.nick, rest, nick->numeric, reason);
            }
         }
         current = current->next;
      }
      nick = nick->next;
   }
}

void kick_all(char *who, char *rest)
{
   struct nickinfo *nick;
   struct chaninfo *current;
   char *nwho = NULL, *chan = NULL, test[10000], *reason = NULL;
   
   test[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   if((reason = strchr(rest, ' ')) != NULL) *reason++ = '\0';
   else
   {
      send_to_server(":%s NOTICE %s :Vous devez spécifier une raison de kick, lisez l'aide\n", bot.nick, nwho);
      return;
   }
   nick = nickhead;
   while(nick != NULL)
   {
      current = nick->chanhead;
      while(current != NULL)
      {
         if(!strcasecmp(rest, current->channel) && strcasecmp(nick->nick, who) && strcasecmp(nick->nick, bot.nick))
         {
            send_to_server(":%s KICK %s %s :%s\n", bot.nick, rest, nick->numeric, reason);
         }
         current = current->next;
      }
      nick = nick->next;
   }
}

void kick_user(char *who, char *rest)
{
   char *nwho = NULL, *nick = NULL, *reason = NULL, *ptr = NULL, *chan = NULL, *nnick = NULL, test[10000], *uhost = NULL;
   char *helpname;
   struct nickinfo *tmp1;
   struct chaninfo *tmp2;
   struct authinfo *tmp3;

   test[0]='\0';
   nwho=nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   helpname = get_command_name("kick");
   if(nick = strchr(rest, ' ')) *nick++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   if(reason = strchr(nick, ' ')) *reason++ = '\0';
   else reason = "Lamer!";
   strlwr(chan);
   if(strlen(reason) > 100) reason[99] = '\0';
   if((uhost = getuserhost(nick)) == NULL)
   {
      send_to_server(":%s NOTICE %s :   L'utilisateur %s n'est pas connecté\n", bot.nick, nwho, nick);
     return;
   }
   nnick=nick2num(nick);
   if(!strcasecmp(bot.nick, nick)) return;
   send_to_server(":%s KICK %s %s :%s\n", bot.nick, chan, nnick, reason);
   tmp1 = nickhead;
   while(tmp1 != NULL)
   {
      tmp2 = tmp1->chanhead;
      while(tmp2 != NULL)
      {
         if(!strcasecmp(chan, tmp2->channel))
         {
            tmp3 = return_authinfo(chan, tmp1->nick);
            if(tmp3 != NULL && tmp2->is_ops)
               send_to_server(":%s NOTICE %s : IriX a kické %s du salon %s ( %s [%s])\n", bot.nick, tmp1->numeric, nick, chan, reason, who);
         }
               tmp2 = tmp2->next;
      }
      tmp1 = tmp1->next;
   }
}

void clearmode(char *who, char *rest)
{
   char *chan = NULL, test[10000], *modes = NULL;

   test[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   strlwr(chan);
   modes = list_modes(chan);
   send_to_server(":%s MODE %s -%s\n", bot.nick, chan, ++modes);
   send_to_server(":%s MODE %s\n", bot.nick, chan);
}

void show_uptime(char *who, char *rest)
{
   int days = 0, hours = 0, mins = 0, secs = 0;
   char *nwho = NULL;
   nwho=nick2num(who);

   secs = time(NULL) - uptime;
   if(secs > 60) 
   {
      mins = secs / 60;
      secs -= 60 * mins;
   } else mins = 0;
   if(mins > 60) 
   {
      hours = mins / 60;
      mins -= 60 * hours;
   } else hours = 0;
   if(hours > 24) 
   {
      days = hours / 24;
      hours -= 24 * days;
   } else days = 0;
   send_to_server(":%s NOTICE %s :L'uptime est de %02d jours, %02d heures, %02d minutes, %02d secondes\n", bot.nick, nwho, days, hours, mins, secs);
}

void moderate(char *who, char *rest)
{
   struct nickinfo *nick;
   struct chaninfo *current;
   char nickbuff[10000], md[10], test[10000], *chan = NULL;
   int num_nicks=0;

   test[0]='\0';md[0]='\0';nickbuff[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nickbuff[0]='\0';
   md[0]='\0';
   send_to_server(":%s MODE %s +m\n", bot.nick, chan);
   send_to_server(":%s MODE %s\n", bot.nick, chan);
   nick = nickhead;
   while(nick != NULL)
   {
      current = nick->chanhead;
      while(current != NULL)
      {
         if(!strcasecmp(chan,current->channel))
         {
            if(num_nicks < 6)
            {
               strcat(nickbuff, nick->numeric);
               strcat(nickbuff, " ");
               strcat(md, "v");
               num_nicks++;
            }
            if(num_nicks == 5)
            {
               send_to_server(":%s MODE %s +%s %s\n", bot.nick, chan, md, nickbuff);
               num_nicks = 0;
               nickbuff[0]='\0';
               md[0]='\0';
            }
         }
         current=current->next;
      }
      nick=nick->next;
   }
   if(num_nicks > 0)
   {
      send_to_server(":%s MODE %s +%s %s\n", bot.nick, chan, md, nickbuff);
   }
}

void deopall(char *who, char *rest)
{
   struct nickinfo *nick;
   struct chaninfo *current;
   char nickbuff[10000], md[1000], test[10000], *chan = NULL;
   int num_nicks=0;

   test[0]='\0';md[0]='\0';nickbuff[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nickbuff[0]='\0';
   md[0]='\0';
   nick = nickhead;
   while(nick != NULL)
   {
      current = nick->chanhead;
      while(current != NULL)
      {
         if(!strcasecmp(chan,current->channel) && strcasecmp(bot.nick, nick->nick))
         {
            
            if(num_nicks < 6)
            {
               strcat(nickbuff, nick->numeric);
               strcat(nickbuff, " ");
               strcat(md, "o");
               num_nicks++;
            }
            if(num_nicks == 6)
            {
               send_to_server(":%s MODE %s -%s %s\n", bot.nick, chan, md, nickbuff);
               num_nicks = 0;
               nickbuff[0]='\0';
               md[0]='\0';
            }
         }
         current=current->next;
      }
      nick=nick->next;
   }
   if(num_nicks > 0 && num_nicks < 6)
   {
      send_to_server(":%s MODE %s -%s %s\n", bot.nick, rest, md, nickbuff);
   }
}

void opall(char *who, char *rest)
{
   struct nickinfo *nick;
   struct chaninfo *current;
   struct authinfo *auth;
   struct regsinfo *regs;
   char nickbuff[10000], md[1000], test[10000], *chan = NULL, *nwho = NULL;
   int num_nicks=0;

   test[0]='\0';md[0]='\0';nickbuff[0]='\0';
   nwho = nick2num(who);
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   regs = return_regsinfo(chan);
   if(regs->noops)
   {
      send_to_server(":%s NOTICE %s :   Le NOOPS est à ON, les ops ne sont pas autorisés pour le moment. \n", bot.nick, nwho);
      return;
   }
   nick = nickhead;
   while(nick != NULL)
   {
      current = nick->chanhead;
      while(current != NULL)
      {
         if(!strcasecmp(chan,current->channel) && strcasecmp(bot.nick, nick->nick))
         {
            auth = return_authinfo(chan, nick->nick);
            if(auth != NULL && regs->strictop)
            {
               if(num_nicks < 6)
               {
                  strcat(nickbuff, nick->numeric);
                  strcat(nickbuff, " ");
                  strcat(md, "o");
                  num_nicks++;
               }
               if(num_nicks == 6)
               {
                  send_to_server(":%s MODE %s +%s %s\n", bot.nick, chan, md, nickbuff);
                  num_nicks = 0;
                  nickbuff[0]='\0';
                  md[0]='\0';
               }
            }
            else if(!regs->strictop)
            {
               if(num_nicks < 6)
               {
                  strcat(nickbuff, nick->numeric);
                  strcat(nickbuff, " ");
                  strcat(md, "o");
                  num_nicks++;
               }
               if(num_nicks == 6)
               {
                  send_to_server(":%s MODE %s +%s %s\n", bot.nick, chan, md, nickbuff);
                  num_nicks = 0;
                  nickbuff[0]='\0';
                  md[0]='\0';
               }
            }
         }
         current=current->next;
      }
      nick=nick->next;
   }
   if(num_nicks > 0 && num_nicks < 6)
   {
      send_to_server(":%s MODE %s +%s %s\n", bot.nick, chan, md, nickbuff);
   }
}

void opers_invite(char *who, char *rest)
{
   struct nickinfo *nick;
   char *chan = NULL, *nwho = NULL;

   nwho=nick2num(who);
   nick = nickhead;
   while(nick != NULL)
   {
      if(!strcasecmp(nick->nick, who) && (nick->is_oper))
      {
         gotone = TRUE;
         chan = strtok(rest, " ");
         if(chan_check(chan, who) == -1) return;
         if(check(chan, who) == -1) return;
         send_to_server(":%s INVITE %s %s\n",bot.nick, who, chan);
      }
      nick = nick->next;
   }
   if(gotone != TRUE)
   {
      send_to_server(":%s NOTICE %s :Je n'y songerai pas!\n", bot.nick, nwho);
   }
   gotone = FALSE;
}

void invite(char *who, char *rest)
{
   char *nick = NULL, *chan = NULL, test[10000];
 
   test[0]='\0';
   strcpy(test, rest);
   if(rest != NULL)
   {
     chan = strtok(test, " ");
     if(chan_check(chan, who) == -1) return;
     if(check(chan, who) == -1) return;
     nick = strtok(NULL, " ");
     if(nick != NULL)
       send_to_server(":%s INVITE %s %s\n",bot.nick, nick, chan);
     else
       send_to_server(":%s INVITE %s %s\n",bot.nick, who, chan);
   }
   else
     return;
}

void inviteme(char *who, char *rest)
{
   char *nwho = NULL, *nick = NULL;
   nwho = nick2num(who);
   if(*msg_chan != '#')
   {
     send_to_server(":%s NOTICE %s : Impossible d'écrire dans le salon, la commande n'est pas mise en service\n",bot.nick, nwho);
     return;
   }
   if(rest != NULL)
   {
     if(strlen(rest) > 25 && *rest == '#') return;
     nick = strtok(rest, " ");
     send_to_server(":%s INVITE %s %s\n",bot.nick, nick, msg_chan);
   }
   else
     send_to_server(":%s INVITE %s %s\n",bot.nick, who, msg_chan);
}

void say_text(char *who, char *rest)
{
   char *text = NULL, *chan = NULL, test[10000], *helpname;

   test[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   helpname = get_command_name("say");
   if(text = strchr(rest, ' ')) *text++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   send_to_server(":%s PRIVMSG %s :%s\n", bot.nick, chan, text);
}

void do_action(char *who, char *rest)
{
   char *text = NULL, *chan = NULL, test[10000], *helpname;

   test[0]='\0';
   strcpy(test, rest);
   chan = strtok(test, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   helpname = get_command_name("do");
   if(text = strchr(rest, ' ')) *text++ = '\0';
   else
   {
      do_help(who, helpname);
      return;
   }
   send_to_server(":%s PRIVMSG %s :\001ACTION %s\001\n", bot.nick, chan, text);
}

void default_modes(char *chan)
{
   struct regsinfo *tmp;

   tmp = return_regsinfo(chan);
   send_to_server(":%s MODE %s %s\n", bot.nick, chan, tmp->defmodes);
   send_to_server(":%s MODE %s\n", bot.nick, chan);
}

void re_op_x()
{
   struct regsinfo *current;
   char *usernum = NULL;

   usernum=get_usernum(bot.nick);
   current = regshead;
   while(current != NULL)
   {
      if(current->chanstamp != NULL && current->chanstamp[0] != '\0')
      {
         send_to_server(":%s MODE %s +o %s %s\n", myserver, current->channel, usernum, current->chanstamp);
      }
      else
      {
          send_to_server(":%s MODE %s +o %s %lu\n", myserver, current->channel, usernum, time(NULL));
      }
      current = current->next;
   }
}

void lastseen(char *who, char *rest)
{
   char *nick = NULL, *nwho = NULL, *chan = NULL, *helpname;
   struct userinfo *tmp;

   nwho=nick2num(who);
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   nick = strtok(NULL, " ");
   helpname = get_command_name("seen");
   if(nick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   tmp = ret_user_nickinfo(nick, chan);
   if(tmp == NULL)
   {
      send_to_server(":%s NOTICE %s : %s n'est pas un membre de la liste des utilisateurs de %s\n", bot.nick, nwho, nick, chan);
      return;
   }
   send_to_server(":%s NOTICE %s :%s dernière présence sur le salon %s - %s\n", bot.nick, nwho, nick, chan, get_time(tmp->lastseen));
}

void admin(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *num;
   char *nwho = NULL, *newnick = NULL, *admnaddr = NULL, *what = NULL;
   int admnlvl = 0, got_nick = 0;

   nwho = nick2num(who);
//   send_to_server(":%s NOTICE %s :  \037\0034Authed\003\037    \037\0034Access\003\037     \037\0034IDnick\003\037         \037\0034Address\003\037\n", bot.nick, nwho);
 send_to_server(":%s NOTICE %s :  \037\0034Autorisé\003\037    \037\0034Accès\003\037    \037\0034Pseudo\003\037\n", bot.nick, nwho);
   regs = regshead;
   while(regs != NULL)
   {  
      if(!strcasecmp(csadminchan, regs->channel))
      {  
         user = regs->userhead;
         while(user != NULL)
         {
             got_nick = 0;
             num = user->numhead;
             while(num != NULL)
             {
                newnick = num2nick(num->numeric);
                got_nick = 1;
                break;
                num = num->next;
             }
             if(got_nick)
             {
                admnaddr = getnickuserhost(newnick);
             }
             if(admnaddr != NULL && got_nick) what = "YES";
             else what = "NO";
/*             if(admnaddr != NULL && got_nick)
             {
                send_to_server(":%s NOTICE %s :  %-3s       %4d       %-12s   %s\n", bot.nick, nwho, what, user->level, user->nick, admnaddr);
             }
             else
                send_to_server(":%s NOTICE %s :  %-3s       %4d       %-12s   --------\n", bot.nick, nwho, what, user->level, user->nick);
*/
			 send_to_server(":%s NOTICE %s :  %-3s       %4d       %-12s \n", bot.nick, nwho, what, user->level, user->nick);
			 user = user->next;
          }
       }
       regs = regs->next;
    }
 }

void generic(char *who, char *rest)
{
      char *helpname;

      helpname = get_command_name("chanopt");
      do_help(who, helpname);
      return;
}

void mode(char *who, char *rest)
{
  struct regsinfo *regs;
  struct userinfo *user;
  struct authinfo *auth;
  char *chan = NULL, *modes = NULL, test[10000], *nwho = NULL, *testmodes = NULL, *helpname;
  
   test[0]='\0';
  nwho = nick2num(who);
  helpname = get_command_name("mode");
  if(modes=strchr(rest, ' ')) *modes++ = '\0';
  else
  {
     do_help(who, helpname);
     return;
  }
  strcpy(test, modes);
  testmodes = strtok(test, " ");
  if(strchr(testmodes, 'o') || strchr(testmodes, 'b') || strchr(testmodes, 'A'))
  {
     send_to_server(":%s NOTICE %s : Vous n'êtes pas autorisé à utiliser ces modes avec cette commande\n", bot.nick, nwho);   
     return;
  }
  auth = return_authinfo(csadminchan, who);
  user = return_userinfo(who, rest);
  regs = return_regsinfo(rest);
  if(auth == NULL && (user == NULL || regs == NULL)) return;
  if(auth == NULL && user->level < regs->chmodlvl)
  {
     send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, denied_msg);
     return;
  }
  send_to_server(":%s MODE %s %s\n", bot.nick, rest, modes);
  send_to_server(":%s MODE %s\n", bot.nick, rest);
}

void cswall(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *num;

   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(regs->channel, csadminchan))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            num = user->numhead;
            while(num != NULL)
            {
               send_to_server(":%s NOTICE %s :CSWALL provenant de %s -  %s\n", bot.nick, num->numeric, who, rest);
               num = num->next;
            }
            user = user->next;
         }
         return;
      }
      regs = regs->next;
   }
}

void list_suspendchan(char *who, char *rest)
{
   struct regsinfo *regs;
   char *nwho = NULL;
   int gotone = 0;
   
   nwho = nick2num(who);
   regs = regshead;
   while(regs != NULL)
   {
      if(regs->suspend)
      {
         send_to_server(":%s NOTICE %s : Le salon %s est maintenant suspendu\n", bot.nick, nwho, regs->channel);
         gotone = 1;
      }
      regs = regs->next;
   }
   if(!gotone)
      send_to_server(":%s NOTICE %s : Aucun salon n'est actuellement suspendu\n", bot.nick, nwho);
}


void show_chan_num(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user;
   struct numinfo *num;
   char *chan, *idnick, *nwho;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   idnick = strtok(NULL, " ");
   if(idnick == NULL)
   {   
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            send_to_server(":%s NOTICE %s : %s\n", bot.nick, nwho, regs->channel);
            user = regs->userhead;
            while(user != NULL)
            {
               send_to_server(":%s NOTICE %s :   %s\n", bot.nick, nwho, user->nick);
               num = user->numhead;
               while(num != NULL)
               {
                  send_to_server(":%s NOTICE %s :     %s\n", bot.nick, nwho, num->numeric);
                  num = num->next;
               }
               user = user->next;
            }
         }
        regs = regs->next;
      }
   }
   else
   {
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            send_to_server(":%s NOTICE %s : %s\n", bot.nick, nwho, regs->channel);
            user = regs->userhead;
            while(user != NULL)
            {
               if(!strcasecmp(idnick, user->nick))
               {
                  send_to_server(":%s NOTICE %s :   %s\n", bot.nick, nwho, user->nick);
                  num = user->numhead;
                  while(num != NULL)
                  {
                     send_to_server(":%s NOTICE %s :     %s\n", bot.nick, nwho, num->numeric);
                     num = num->next;
                  }
               }
               user = user->next;
            }
         }
        regs = regs->next;
      }
   }
}

void verify_pass(char *who, char *rest)
{

   struct userinfo *user;
   struct regsinfo *regs;
   char *cryptpass = NULL, *passwd = NULL, tmppass[1000], keys[3];
   char *chan = NULL, *idnick = NULL, *nwho, *helpname;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   idnick = strtok(NULL, " ");
   helpname = get_command_name("verifypass");
   if(idnick == NULL)
   {
      do_help(who, helpname);
      return;
   }
   passwd = strtok(NULL, " ");
   if(passwd == NULL)
   {
      do_help(who, helpname);
      return;
   }
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(chan, regs->channel))
      {
          user = regs->userhead;
          while(user != NULL)
          {
             if(!strcasecmp(idnick, user->nick))
             {
                strcpy(tmppass, user->password);
                sprintf(keys, "%c%c", tmppass[0], tmppass[1]);
                cryptpass = mkpass(passwd, keys);
                if(!strcmp(user->password, cryptpass))
                {
                   send_to_server(":%s NOTICE %s : Mot de passe correspondant\n", bot.nick, nwho);
                   return;
                }
                else
                {
                   send_to_server(":%s NOTICE %s : Mot de passe non-correspondant\n", bot.nick, nwho);
                   return;
                }
             }
             user = user->next;
          }
          send_to_server(":%s NOTICE %s : Pseudo introuvable dans la liste des utilisateur du salon %s\n", bot.nick, nwho, chan);
      }
      regs = regs->next;
   }
}

void change_owner(char *who, char *rest)
{
   struct regsinfo *regs;
   char *chan, *addr, *nwho, taddr[100], *ptr7, *helpname;

   nwho = nick2num(who);
   chan = strtok(rest, " ");
   if(chan_check(chan, who) == -1) return;
   if(check(chan, who) == -1) return;
   helpname = get_command_name("chowner");
   if((addr = strtok(NULL, " ")) != NULL && (strchr(addr, '@') != NULL))
   {
      if(!strchr(addr, '!'))
      {
         send_to_server(":%s NOTICE %s :Masque invalide,  le format est *!*user@host.\n", bot.nick, nwho);
         return;
      }
      if(strlen(addr) > 100) addr[99] = '\0';
      strcpy(taddr, addr);
      if(ptr7 = strchr(taddr, '@')) *ptr7++ = '\0';

/* NiX
      if(*ptr7 == '*' && !*(ptr7+1))
      {
         send_to_server(":%s NOTICE %s :Veuillez entrer un hostname plus spécifique.\n", bot.nick, nwho);
         return;
      } 
*/
      regs = regshead;
      while(regs != NULL)
      {
         if(!strcasecmp(chan, regs->channel))
         {
            send_to_server(":%s NOTICE %s :Le Masque du propriétaire a été changé de %s pour %s\n", bot.nick, nwho, regs->ownaddr, addr);
            strcpy(regs->ownaddr, addr);
            write_regs_list();
            return;
         }
         regs = regs->next;
      }
   }
   do_help(who, helpname);
}
