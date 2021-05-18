/* CS5 - Channel Services src/parse_mode.c
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

void parse_mode(char *who, char *rest)
{
   struct nickinfo *current;
   struct userinfo *tmp, *tmp1, *tmp2, *admntmp, *admn;
   struct regsinfo *regstmp;
   struct authinfo *auth, *admnauth;
   char *modes = NULL, *nick = NULL, *ptr = NULL, *nwho = NULL, *botnum = NULL, *nchan = NULL, newmodes[100];
   char *plususer[100], *minususer[100], minusmode[100], plusmode[100];
   int i = 0, j = 0, l = 0, k = 0, got_i = 0, got_w = 0;
   char my[100], *bmask = NULL, temp[100], *chan = NULL, *users = NULL, *nmodes = NULL, *nusers = NULL, *mwho = NULL, testmodes[100], *modes1 = NULL;
   int a, got_one = 0;

   strcpy(testmodes, rest);
   mwho = nick2num(who);
   if(mwho == NULL)
     mwho=get_usernum(bot.nick);
   chan = strtok(rest, " ");
   modes = strtok(NULL, " ");
   /* STE */ if(chan == NULL || modes == NULL) return;
   regstmp = return_regsinfo(chan);
   admntmp = return_userinfo(who, csadminchan);
   admnauth = return_authinfo(csadminchan, who);
   botnum=get_usernum(bot.nick);
   if((strchr(modes, 'p') || strchr(modes,'s') || strchr(modes,'m') || strchr(modes,'n') || strchr(modes,'t') || strchr(modes,'i') || strchr(modes,'k') || strchr(modes,'l')) && !strchr(modes, ':'))
   {
      tmp1 = return_userinfo(who, chan);
      if(strchr(modes, 'k') || strchr(modes, 'l'))
      {
         if(modes1 = strchr(testmodes, ' ')) *modes1++ = '\0';
      }
      else
         modes1 = modes;
      if(regstmp != NULL && strcasecmp(who, uworldhost))
      {
         if(admntmp == NULL || admnauth == NULL)
         {
            if(tmp1 == NULL && regstmp->chmodlvl > 0)
            {
                send_to_server(":%s MODE %s -%s\n", bot.nick, chan, ++modes1);
                send_to_server(":%s MODE %s %s\n", bot.nick, chan, regstmp->modes);
            }
            if(tmp1 != NULL && tmp1->level < regstmp->chmodlvl)
            {
                send_to_server(":%s MODE %s -%s\n", bot.nick, chan, ++modes1);
                send_to_server(":%s MODE %s %s\n", bot.nick, chan, regstmp->modes);
            }
         }
      }
      send_to_server(":%s MODE %s\n", bot.nick, chan);
   }
   if(strchr(modes, '+') && strchr(modes, '-') && !strchr(modes, ':'))
   {
      users = strtok(NULL, ":");
      if(users == NULL) return;
      strip_newline(users);
      if(*modes == '+')
      {
         *modes++;
         while(*modes != '\0' && *modes != '-')
         {
            if(*modes != 's' && *modes != 'm' && *modes != 'n' && *modes != 't' && *modes != 'i' && *modes != 'p')
            {
               plusmode[l] = *modes;
               l++;
            }
            *modes++;
         }
         if(nmodes = strchr(modes, '-')) *nmodes++ = '\0';
         plusmode[l] = '\0';
         l = 0;
         if(plusmode[0] != '\0')
         {
            plususer[l] = strtok(users, " ");
            l++;
         }
         while(plusmode[l] != '\0')
         {
            plususer[l] = strtok(NULL, " ");
            l++;
         }
         plususer[l] = NULL;
         if(plusmode[0] != '\0')
            nusers = strtok(NULL, ":");
         else
            nusers = strtok(users, ":");
         for(l = 0; plusmode[l] && plusmode[l] != '\0'; l++)
         {
            if(plusmode[l] == 'o')
            {
               if(strlen(plususer[l]) == 3)
               {
                  replace_onode(chan, plususer[l], TRUE);
                  if(!strcasecmp(who, uworldhost)) return;
                  nwho=num2nick(plususer[l]);
                  auth = return_authinfo(chan, nwho);
                  if(regstmp != NULL)
                  {
                     if(auth == NULL && regstmp->strictop)
                     {
                        send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, plususer[l]);
                        replace_onode(chan, plususer[l], FALSE);
                     }
                     if(regstmp->noops && strcmp(botnum, plususer[l]))
                     {
                        send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, plususer[l]);
                        replace_onode(chan, plususer[l], FALSE);
                     }
                  }
               }
            }
            if(plusmode[l] == 'b')
            {
               if(strchr(plususer[l], '!') || strchr(plususer[l], '@'))
               {
                  add_baninfo(chan, plususer[l], time(NULL), 0, 0, NULL, who, 0);
                  if(!strcasecmp(who, uworldhost)) return;
                  if(regstmp == NULL) continue;
                  if(!strchr(plususer[l], '@') &&
                     !strchr(plususer[l], '!')) continue;
                  strcpy(temp, plususer[l]);
                  sprintf(my, "*!*%s@%s", ircname, myhost);
                  collapse(plususer[l]);
                  if(!bancmp(my, plususer[l]) || !bancmp(plususer[l], my))
                  {
                     send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                     continue;
                  }
                  tmp = return_userinfo(plususer[l], chan);
                  admn =  return_userinfo(plususer[l], csadminchan);
                  if(admn != NULL && admn->protect)
                  {
                     send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                     delete_bnode(chan, plususer[l]);
                     continue;
                  }
                  tmp2 = return_userinfo(who, chan);
                  if(tmp2 == NULL)
                  {
                      if(regstmp->banlvl > 0)
                      if(admntmp == NULL || admnauth == NULL)
                      {
                         send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                         delete_bnode(chan, plususer[l]);
                         continue;
                      }
                  }
                  else
                  {
                      if(tmp2->level < regstmp->banlvl)
                      {
                         send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                         delete_bnode(chan, plususer[l]);
                         continue;
                      }
                  }
                  if(tmp != NULL)
                  {
                     if(tmp->protect || regstmp->noban || tmp->level == maxuserlevel)
                     {
                         send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                         delete_bnode(chan, plususer[l]);
                     }
                  }
               }
            }
         }
         if(nusers != NULL)
         {
            sprintf(newmodes, "%s -%s %s", chan, nmodes, nusers);
            parse_mode(who, newmodes);
         }
      }
      else
      {
         *modes++;
         while(*modes != '\0' && *modes != '+')
         {
            if(*modes != 's' && *modes != 'm' && *modes != 'n' && *modes != 't' && *modes != 'i' && *modes != 'l' && *modes != 'p')
            {
               minusmode[l] = *modes;
               l++;
            }
            *modes++;
         }
         if(nmodes = strchr(modes, '+')) *nmodes++ = '\0';
         minusmode[l] = '\0';
         l = 0;
         if(minusmode[0] != '\0')
         {
            minususer[l] = strtok(users, " ");
            l++;
         }
         while(minusmode[l] != '\0')
         {
            minususer[l] = strtok(NULL, " ");
            l++;
         }
         minususer[l] = NULL;
         if(minusmode[0] != '\0')
            nusers = strtok(NULL, ":");
         else
            nusers = strtok(users, ":");
         for(l = 0; minusmode[l] && minusmode[l] != '\0'; l++)
         {
            if(minusmode[l] == 'o')
            {
               if(strlen(minususer[l]) == 3)
               {
                  replace_onode(chan, minususer[l], FALSE);
                  if(!strcasecmp(who, uworldhost)) return;
                  if(regstmp == NULL) continue;
                  if(!strcmp(botnum, minususer[l]))
                  {
                     send_to_server(":%s MODE %s +o %s\n", myserver, chan, minususer[l]);
                     continue;
                  }
                  nwho=num2nick(minususer[l]);
                  tmp = return_userinfo(nwho, chan);
                  admn = return_userinfo(nwho, csadminchan);
                  admnauth = return_authinfo(csadminchan, nwho);
                  if(admn != NULL && admn->protect && strcmp(minususer[l], mwho) && admnauth != NULL)
                  {
                     send_to_server(":%s MODE %s +o %s\n", bot.nick, chan, minususer[l]);
                     replace_onode(chan, minususer[l], TRUE);
                     continue;
                  }
                  auth = return_authinfo(chan, nwho);
                  if(tmp != NULL && tmp->protect && strcmp(minususer[l], mwho) && auth != NULL)
                  {
                     send_to_server(":%s MODE %s +o %s\n", bot.nick, chan, minususer[l]);
                     replace_onode(chan, minususer[l], TRUE);
                  }
               }
            }
            if(minusmode[l] == 'b')
            {
               if(strchr(minususer[l], '!') || strchr(minususer[l], '@'))
               {
                  delete_bnode(chan, minususer[l]);
               }
            }
         }
         if(nusers != NULL)
         {
            sprintf(newmodes, "%s +%s %s", chan, nmodes, nusers);
            parse_mode(who, newmodes);
         }
      }
      return;
   }
   switch(modes[0])
   {
      case '-':
               *modes++;
               while(*modes != '\0')
               {
                  if(*modes != 's' && *modes != 'm' && *modes != 'n' && *modes != 't' && *modes != 'i' && *modes != 'p' && *modes != 'l')
                  {
                     minusmode[l] = *modes;
                     l++;
                  }
                  *modes++;
               }
               if(minusmode[0] == '\0' || minusmode == NULL) return;
               minusmode[l] = '\0';
               l = 0;
               minususer[l] = strtok(NULL, " ");
               l++;
               while(minusmode[l] != '\0')
               {
                  minususer[l] = strtok(NULL, " ");
                  l++;
               }
               minususer[l] = NULL;
               l = 0;
               for(l = 0; minusmode[l] && minusmode[l] != '\0'; l++)
               {
                  if(minusmode[l] == 'o')
                  {
                     if(strlen(minususer[l]) == 3)
                     {
                        replace_onode(chan, minususer[l], FALSE);
                        if(!strcasecmp(who, uworldhost)) return;
                        if(regstmp == NULL) continue;
                        if(!strcmp(botnum, minususer[l]))
                        {
                            send_to_server(":%s MODE %s +o %s\n", myserver, chan, minususer[l]);
                            continue;
                        }
                        nwho=num2nick(minususer[l]);
                        tmp = return_userinfo(nwho, chan);
                        admn = return_userinfo(nwho, csadminchan);
                        admnauth = return_authinfo(csadminchan, nwho);
                        if(admn != NULL && admn->protect && strcmp(minususer[l], mwho) && admnauth != NULL)
                        {
                           send_to_server(":%s MODE %s +o %s\n", bot.nick, chan, minususer[l]);
                           continue;
                        }
                        auth = return_authinfo(chan, nwho);
                        if(tmp != NULL && tmp->protect && strcmp(minususer[l], mwho) && auth != NULL)
                        {
                           send_to_server(":%s MODE %s +o %s\n", bot.nick, chan, minususer[l]);
                           replace_onode(chan, minususer[l], TRUE);
                        }
                     }
                  }
                  if(minusmode[l] == 'b')
                  {
                     if(strchr(minususer[l], '!') || strchr(minususer[l], '@'))
                     {
                        delete_bnode(chan, minususer[l]);
                     }
                  }
               }
               return;
      case '+':
               *modes++;

               while(*modes != '\0')
               {
                  if(*modes != 's' && *modes != 'm' && *modes != 'n' && *modes != 't' && *modes != 'i' && *modes != 'p')
                  {
                     plusmode[l] = *modes;
                     l++;
                  }
                  *modes++;
               }
               if(plusmode[0] == '\0' || plusmode == NULL) return;
               plusmode[l] = '\0';
               l = 0;
               plususer[l] = strtok(NULL, " ");
               l++;
               while(plusmode[l] != '\0')
               {
                  plususer[l] = strtok(NULL, " ");
                  l++;
               }
               plususer[l] = NULL;
               l = 0;
               for(l = 0; plusmode[l]; l++)
               {
                  if(plusmode[l] == 'o')
                  {
                     if(strlen(plususer[l]) == 3)
                     {
                        replace_onode(chan, plususer[l], TRUE);
                        if(!strcasecmp(who, uworldhost)) return;
                        nwho=num2nick(plususer[l]);
                        auth = return_authinfo(chan, nwho);
                        if(regstmp != NULL)
                        {
                           if(auth == NULL && regstmp->strictop)
                           {
                              send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, plususer[l]);
                              replace_onode(chan, plususer[l], FALSE);
                           }
                          if(regstmp->noops && strcmp(botnum, plususer[l]))
                           {
                              send_to_server(":%s MODE %s -o %s\n", bot.nick, chan, plususer[l]);
                              replace_onode(chan, plususer[l], FALSE);
                           }
                        }
                     }
                  }
                  if(plusmode[l] == 'b')
                  {
                     if(strchr(plususer[l], '!') || strchr(plususer[l], '@'))
                     {
                        add_baninfo(chan, plususer[l], time(NULL), 0, 0, NULL, who, 0);
                        if(!strcasecmp(who, uworldhost)) return;
                        if(regstmp == NULL) continue;
                        if(!strchr(plususer[l], '@') &&
                           !strchr(plususer[l], '!')) continue;
                        strcpy(temp, plususer[l]);
                        sprintf(my, "*!*%s@%s", ircname, myhost);
                        collapse(plususer[l]);
                        if(!bancmp(my, plususer[l]) || !bancmp(plususer[l], my))
                        {
                           send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                           continue;
                        }
                        tmp = return_userinfo(plususer[l], chan);
                        admn =  return_userinfo(plususer[l], csadminchan);
                        if(admn != NULL && admn->protect)
                        {
                           send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                           delete_bnode(chan, plususer[l]);
                           continue;
                        }
                        tmp2 = return_userinfo(who, chan);
                        if(tmp2 == NULL)
                        {
                           if(regstmp->banlvl > 0)
                           if(admntmp == NULL || admnauth == NULL)
                           {
                              send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                              delete_bnode(chan, plususer[l]);
                              continue;
                           }
                        }
                        else
                        {
                           if(tmp2->level < regstmp->banlvl)
                           {
                              send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                              delete_bnode(chan, plususer[l]);
                              continue;
                           }
                        }
                        if(tmp != NULL)
                        {
                           if(tmp->protect || regstmp->noban || tmp->level == maxuserlevel)
                           {
                              send_to_server(":%s MODE %s -b %s\n", bot.nick, chan, plususer[l]);
                              delete_bnode(chan, plususer[l]);
                           }
                        }
                     }
                  }
               }
               return;
        case ':':
                 if(strchr(modes, '+') && strchr(modes, '-'))
                 {
                    *modes++;
                    if(modes[0] == '-')
                    {
                       if(ptr = strchr(modes, '+')) *ptr++ = '\0';
                       sprintf(newmodes, "%s :+%s", who, ptr);
                       got_one = 1;
                    }
                    if(modes[0] == '+')
                    {
                       if(ptr = strchr(modes, '-')) *ptr++ = '\0';
                       sprintf(newmodes, "%s :-%s", who, ptr);
                       got_one = 1;
                    }
                 }
                 if (strchr(modes,'+'))
                 {
                   current = nickhead;
                   while(current != NULL)
                   {
                     if(!strcasecmp(who,current->nick))
                     {
                        if(strchr(modes, 'o'))
                           current->is_oper=TRUE;
                        if(strchr(modes, 'i'))
                           got_i = 1;
                        if(strchr(modes, 'w'))
                           got_w = 1;
                        if(got_i && got_w)
                        {
                          strcpy(current->modes, "iw");
                          break;
                        }
                        if((!strcmp(current->modes, "NONE") || current->modes == NULL) && got_i)
                        {
                          strcpy(current->modes, "i");
                          break;
                        }
                        if((!strcmp(current->modes, "NONE") || current->modes == NULL) && got_w)
                        {
                          strcpy(current->modes, "w");
                          break;
                        }
                        if(!strcmp(current->modes, "i") && got_w)
                        {
                          strcpy(current->modes, "iw");
                          break;
                        }
                        if(!strcmp(current->modes, "w") && got_i)
                        {
                          strcpy(current->modes, "iw");
                          break;
                        }
                        if(got_i)
                          strcpy(current->modes, "i");
                        if(got_w)
                          strcpy(current->modes, "w");
                     }
                     current = current->next;
                   }
                   if(got_one)
                      parse_mode(who, newmodes);
                    got_one = 0;
                    break;
                 }
                 if (strchr(modes,'-'))
                 {
                   current = nickhead;
                   while(current != NULL)
                   {
                     if(!strcasecmp(who,current->nick))
                     {
                        if(strchr(modes, 'o'))
                           current->is_oper=FALSE;
                        if(strchr(modes, 'i'))
                           got_i = 1;
                        if(strchr(modes, 'w'))
                           got_w = 1;
                        if(got_i && got_w)
                        {
                          strcpy(current->modes, "NONE");
                          break;
                        }
                        if(!strcmp(current->modes, "w") && got_w)
                        {
                          strcpy(current->modes, "NONE");
                          break;
                        }
                        if(!strcmp(current->modes, "iw") && got_w)
                        {
                          strcpy(current->modes, "i");
                          break;
                        }
                        if(!strcmp(current->modes, "iw") && got_i)
                        {
                          strcpy(current->modes, "w");
                          break;
                        }
                        if(!strcmp(current->modes, "i") && got_i)
                        {
                          strcpy(current->modes, "NONE");
                          break;
                        }
                     }
                     current = current->next;
                   }
                   if(got_one)
                      parse_mode(who, newmodes);
                    got_one = 0;
                    break;
                 }
                 break;
   }
   return;         
}
