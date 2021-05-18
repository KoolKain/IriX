/* CS5 - Channel Services src/mod_admin.c
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

void mod_admin(char *who, char *rest)
{
   struct regsinfo *regs;
   struct userinfo *user, *tmp;
   struct adderinfo *addrtmp;
   char *chan = NULL, *usrhost = NULL, *test1 = NULL, *test2 = NULL, *ptr = NULL, *cryptpass = NULL;
   char *nwho = NULL, *field = NULL, *value = NULL, *waste = NULL, chk_mask[100], *helpname, tlevel[1000];
   char *what = NULL, *unick = NULL, test[10000], taddr[100], *ptr6 = NULL, *ptr7 = NULL, value1[3], newhelp[1000];
   int testmask;

   chk_mask[0]='\0';test[0]='\0';taddr[0]='\0';value1[0]='\0';newhelp[0]='\0';
   nwho = nick2num(who);
   helpname = get_command_name("modadmin");
   if(unick= strchr(rest, ' ')) *unick++ = '\0';
   else
   {
      field = rest;
      sprintf(newhelp, "%s %s", helpname, field);
      strlwr(newhelp);
      do_help(who, newhelp);
      return;
   }
   field = rest;
   if(value = strchr(unick, ' ')) *value++ = '\0';
   regs = regshead;
   while(regs != NULL)
   {
      if(!strcasecmp(csadminchan, regs->channel))
      {
         user = regs->userhead;
         while(user != NULL)
         {
            if(!strcasecmp(unick, user->nick))
            {
            	
							 /* Command newidnick */
               if(!strcasecmp(field, "newidnick")) 
               {
                  if(added_level < user->level)
                  {
                     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier le pseudo d'un Admin avec un level plus élevé que le votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(value == NULL)
                  {
                     sprintf(newhelp, "%s %s", helpname, field);
                     strlwr(newhelp);
                     do_help(who, newhelp);
                     return;
                  }
                  /* STE */
									if(strlen(value) > 32) {
                     send_to_server(":%s NOTICE %s :Le pseudo d'un CSA est limité a 32 caracteres\n", bot.nick, nwho);
                     return;
									}
                  tmp = ret_user_nickinfo(value, csadminchan);
                  if(tmp != NULL)
                  {
                     send_to_server(":%s NOTICE %s :Le pseudo %s est déjà dans la liste des CSA.\n", bot.nick, nwho, value);
                     return;
                  }
                  strcpy(user->nick, value);
                  send_to_server(":%s NOTICE %s :Le pseudo a été changé pour %s\n", bot.nick, nwho, value);
                  write_single_channel(csadminchan);
                  return;
               }

							 /* Command level */
               if(!strcasecmp(field, "level")) 
               {
									/* STE */
                  if(value == NULL)
                  {
                     sprintf(newhelp, "%s %s", helpname, field);
                     strlwr(newhelp);
                     do_help(who, newhelp);
                     return;
                  }
                  strcpy(tlevel, value);
                  if(!is_num(tlevel))
                  {
		     send_to_server(":%s NOTICE %s :Le level doit être une valeur numérique en dessous de %d.\n", bot.nick, nwho, added_level);
                     return;
                  }
                  if(added_level <= user->level)
                  {
		     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier le level d'un Admin ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(value == NULL)
                  {
                     sprintf(newhelp, "%s %s", helpname, field);
                     strlwr(newhelp);
                     do_help(who, newhelp);
                     return;
                  }
                  if(atoi(value) >= added_level) 
                  {
		     send_to_server(":%s NOTICE %s :Vous ne pouvez pas changer le level d'un Admin pour un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  user->level = atoi(value);
		  send_to_server(":%s NOTICE %s :Le level de %s a été changé à %s\n", bot.nick, nwho, unick, value);
                  write_single_channel(csadminchan);
                  return;
               }


							 /* Command autoop */
               if(!strcasecmp(field, "autoop")) 
               {
                  if(added_level < user->level)
                  {
		     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier l'option AUTOOP d'un Admin ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(user->autoop)
                  {
                     user->autoop = 0;
                     what = "OFF";
                  }
                  else 
                  {
                     user->autoop = 1;
                     what = "ON";
                  }
		  send_to_server(":%s NOTICE %s :L'option Autoop a été mise à %s pour %s\n", bot.nick, nwho, what, unick);
                  write_single_channel(csadminchan);
                  return;
               }


							 /* Command protect */
               if(!strcasecmp(field, "protect")) 
               {
                  if(added_level < user->level) 
                  {
		     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier l'option PROTECT d'un Admin ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(user->protect)
                  {
                     user->protect = 0;
                     what = "OFF";
                  }
                  else 
                  {
                     user->protect = 1;
                     what = "ON";
                  }
		  send_to_server(":%s NOTICE %s :L'option PROTECT a été mise à %s pour %s\n", bot.nick, nwho, what, unick);
                  write_single_channel(csadminchan);
                  return;
               }

							 /* Command newpass */
               if(!strcasecmp(field, "newpass")) 
               {
                  if(value == NULL)
                  {
                     sprintf(newhelp, "%s %s", helpname, field);
                     strlwr(newhelp);
                     do_help(who, newhelp);
                     return;
                  }
                  if(added_level < user->level) 
                  {
                     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier le mot de passe d'un Admin ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  cryptpass = mkpass(value, "0");
                  strcpy(user->password, cryptpass);
                  send_to_server(":%s NOTICE %s :Le mot de passe a été changé à %s for %s\n", bot.nick, nwho, value, unick);
                  write_single_channel(csadminchan);
                  return;
               }

							 /* Command addmask */
               if(!strcasecmp(field, "addmask")) 
               {
                  if(value == NULL)
                  {
                     sprintf(newhelp, "%s %s", helpname, field);
                     strlwr(newhelp);
                     do_help(who, newhelp);
                     return;
                  }
                  if(added_level < user->level) 
                  {
		     send_to_server(":%s NOTICE %s :Vous ne pouvez pas ajouter un masque à un CSA ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(!(ptr = strchr(value, '@')) || !strchr(value, '!'))
                  {
                     send_to_server(":%s NOTICE %s :Syntaxe incorrecte! Le format est nick!user@host\n", bot.nick, nwho);
                     return;
                  }
                  strcpy(taddr, value);
                  if(ptr7 = strchr(taddr, '@')) *ptr7++ = '\0';

                 /* NiX
                  if(!strcmp(taddr, "*!*"))
                  {
                     send_to_server(":%s NOTICE %s :Veuillez inclure l'username.\n", bot.nick, nwho);
                     return;
                  }  
                  if(!strcmp(ptr7, "*") || !strcmp(ptr7, "*.*") || !strcmp(ptr7, "*.*.*"))
                  {
                     send_to_server(":%s NOTICE %s :Veuillez entrer un hostmasque plus specifique.\n", bot.nick, nwho);
                     return;
                  }
                 */
                  testmask = add_user_addrinfo(csadminchan, unick, value);
                  if(testmask == 0)
                  {
                     send_to_server(":%s NOTICE %s :MASK %s ajouté pour %s\n", bot.nick, nwho, value, unick);
                  }
                  if(testmask == 1)
                  {
		     send_to_server(":%s NOTICE %s :MASQUE %s déjà existant pour %s\n", bot.nick, nwho, value, unick);
                  }
                  write_single_channel(csadminchan);
                  return;
               }

							 /* Command delmask */
               if(!strcasecmp(field, "delmask")) 
               {
                  if(value == NULL)
                  {
                     sprintf(newhelp, "%s %s", helpname, field);
                     strlwr(newhelp);
                     do_help(who, newhelp);
                     return;
                  }
                  if(added_level < user->level) 
                  {
		     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier le masque d'un CSA ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(delete_addrnode(csadminchan, unick, value) == 0)
                     send_to_server(":%s NOTICE %s :Vous ne pouvez pas supprimer le dernier masque\n", bot.nick, nwho, value, unick);
                  else  
                    send_to_server(":%s NOTICE %s :MASQUE %s supprimé pour %s\n", bot.nick, nwho, value, unick);
                  write_single_channel(csadminchan);
                  return;
               }

							 /* Command info */
               if(!strcasecmp(field, "info")) 
               {
                  if(strlen(value) > 150) value[149] = '\0';
                  if(added_level < user->level) 
                  {
                  send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier l'infoline d'un CSA ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(!value || !strcasecmp(value, "delete"))
                  {
                     value1[0] = '\0';
                     add_infoinfo(csadminchan, unick, value1);
                     send_to_server(":%s NOTICE %s :Infoline enlevée.\n", bot.nick, csadminchan);
                  }
                  else
                  {
                     send_to_server(":%s NOTICE %s :[%s] %s\n", bot.nick, nwho, unick, value);
                     add_infoinfo(csadminchan, unick, value);
                  }
                  write_single_infolines(csadminchan);
                  return;
               }

							 /* Command suspend */
               if(!strcasecmp(field, "suspend")) 
               {
                  if(added_level < user->level) 
                  {
                     send_to_server(":%s NOTICE %s :Vous ne pouvez pas modifier l'option SUSPEND d'un CSA ayant un level plus élevé ou égal au votre.\n", bot.nick, nwho);
                     return;
                  }
                  if(user->suspend)
                  {
                     user->suspend = 0;
                     what = "OFF";
                  }
                  else 
                  {
                     user->suspend = 1;
                     what = "ON";
                  }
                  send_to_server(":%s NOTICE %s :L'option SUSPEND a été mise à %s pour %s sur %s\n", bot.nick, nwho, what, unick, csadminchan);
                  write_single_channel(csadminchan);
                  return;
               }
               if(strcasecmp(field, "newpass") && strcasecmp(field, "protect") && strcasecmp(field, "autoop") && strcasecmp(field, "level") && strcasecmp(field, "addmask") && strcasecmp(field, "delmask") && strcasecmp(field, "newidnick") && strcasecmp(field, "suspend") || strcasecmp(field, "info"))
               {
                  do_help(who, helpname);
                  return;
               }
            }
            user = user->next;
         }
      }
      regs = regs->next;
   }
}
