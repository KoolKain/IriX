/* CS5 - Channel Services src/help.c
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

void do_help(char *who, char *rest)
{
   FILE *fp;
   char buffer[1000], tbuf[MAXLEN], *nwho = NULL, cmdhelp[200], ptr[500], *ptr1 = NULL, *ptr2 = NULL, *trest = NULL, test[500];
   char *helpname;
   long pos;

   buffer[0] = '\0';test[0] = '\0';tbuf[0]='\0';cmdhelp[0]='\0';
   
   if(rest == NULL) 
   {
      strcpy(cmdhelp, "syntax/main/SHOWCOMMANDS");
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         file_error(cmdhelp);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL) 
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   strcpy(ptr, rest);
   if(!isalpha(*rest))
   {
       nwho=nick2num(who);
       send_to_server(":%s NOTICE %s :Caractère invalide.\n", bot.nick, nwho);
       return;
   }
   if(strlen(ptr) > 50) return;
   if(ptr1 = strchr(ptr, ' ')) *ptr1++ = '\0';
   helpname = get_command_name("chanopt");
   if(!strcasecmp(ptr, helpname) && ptr1 != NULL)
   {
      ptr2 = strupr(ptr1);
      sprintf(cmdhelp, "syntax/%s/%s", helpname, ptr2);
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL)
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   helpname = get_command_name("modadmin");
   if(!strcasecmp(ptr, helpname) && ptr1 != NULL)
   {
      ptr2 = strupr(ptr1);
      sprintf(cmdhelp, "syntax/%s/%s", helpname, ptr2);
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL)
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   helpname = get_command_name("moduser");
   if(!strcasecmp(ptr, helpname) && ptr1 != NULL)
   {
      ptr2 = strupr(ptr1);
      sprintf(cmdhelp, "syntax/%s/%s", helpname, ptr2);
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL)
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   ptr2 = strupr(ptr);
   sprintf(cmdhelp, "syntax/main/%s", ptr2);
   if((fp = fopen(cmdhelp, "r")) == NULL) 
   {
      nwho=nick2num(who);
      send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
      return;
   }
   while(fgets(buffer, 100, fp) != NULL)
   {
      translate_esc(buffer, tbuf);
      nwho=nick2num(who);
      send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
   }
   fclose(fp);
}

void help(char *who, char *rest)
{
   FILE *fp;
   char buffer[1000], tbuf[MAXLEN], *nwho = NULL, cmdhelp[200], ptr[500], *ptr1 = NULL, *ptr2 = NULL, *trest = NULL, test[500];
   char *helpname;
   long pos;

   buffer[0] = '\0';test[0] = '\0';tbuf[0]='\0';cmdhelp[0]='\0';
   if(rest == NULL) 
   {
      strcpy(cmdhelp, "help/main/SHOWCOMMANDS");
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         file_error(cmdhelp);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL) 
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   strcpy(ptr, rest);
   if(!isalpha(*rest))
   {
       nwho=nick2num(who);
       send_to_server(":%s NOTICE %s :Caractère invalide.\n", bot.nick, nwho);
       return;
   }
   if(strlen(ptr) > 50) return;
   if(ptr1 = strchr(ptr, ' ')) *ptr1++ = '\0';
   strlwr(ptr);
   helpname = get_command_name("chanopt");
   if(!strcasecmp(ptr, helpname) && ptr1 != NULL)
   {
      strlwr(ptr1);
      ptr2 = strupr(ptr1);
      sprintf(cmdhelp, "help/%s/%s", helpname, ptr2);
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL)
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   helpname = get_command_name("modadmin");
   if(!strcasecmp(ptr, helpname) && ptr1 != NULL)
   {
      strlwr(ptr1);
      ptr2 = strupr(ptr1);
      sprintf(cmdhelp, "help/%s/%s", helpname, ptr2);
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL)
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   helpname = get_command_name("moduser");
   if(!strcasecmp(ptr, helpname) && ptr1 != NULL)
   {
      strlwr(ptr1);
      ptr2 = strupr(ptr1);
      sprintf(cmdhelp, "help/%s/%s", helpname, ptr2);
      if((fp = fopen(cmdhelp, "r")) == NULL) 
      {
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
         return;
      }
      while(fgets(buffer, 100, fp) != NULL)
      {
         translate_esc(buffer, tbuf);
         nwho=nick2num(who);
         send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
      }
      fclose(fp);
      return;
   }
   ptr2 = strupr(ptr);
   sprintf(cmdhelp, "help/main/%s", ptr2);
   if((fp = fopen(cmdhelp, "r")) == NULL) 
   {
      nwho=nick2num(who);
      send_to_server(":%s NOTICE %s :Cette commande n'existe pas.\n", bot.nick, nwho);
      return;
   }
   while(fgets(buffer, 100, fp) != NULL)
   {
      translate_esc(buffer, tbuf);
      nwho=nick2num(who);
      send_to_server(":%s NOTICE %s :%s\n", bot.nick, nwho, tbuf);
   }
   fclose(fp);
}

char translate_esc(char *buffer, char *tbuffer)
{
   char *ptr = NULL;
   int i = 0, j = 0, k = 0;
   while(buffer[i] != '\0') 
   {
      if(buffer[i] == '\\') 
      {
         switch(buffer[i+1]) 
         {
            case 'b': tbuffer[k] = '\037';
                      break;
            case 'i': tbuffer[k] = '\026';
                      break;
            case 'r': tbuffer[k] = ' ';
                      break;
            case 'n': while(bot.nick[j] != '\0') 
                      {
                         tbuffer[k] = bot.nick[j];
                         k++;
                         j++;
                      }
                      k--;
                      break;
         }
         i+=2;
         k++;
         continue;
      }
      tbuffer[k] = buffer[i];
      i++;
      k++;
   }
   tbuffer[k] = '\0';
}
