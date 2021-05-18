/* CS5 - Channel Services src/char_tools.c
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

int is_char(char *num)
{

   while(*num)
   {
     if(!isalpha(*num)) return 0;
     num++;
   }
   return 1;
}

void strip_chars(char *buff)
{
   char *ptr = NULL;
   if(ptr = strchr(buff, '+')) ptr++;
   else if(ptr = strchr(buff, '-')) ptr++;
}


void log(char *filename, char *format, ...)
{
   va_list arglist;
   FILE *fp;
   if((fp = fopen(filename, "a")) == NULL)
   {
      file_error(filename);
      return;
   }
   va_start(arglist, format);
   vfprintf(fp, format, arglist);
   va_end(arglist);
   fclose(fp);
}

void strip_newline(char *string)
{
   char *ptr = NULL;
   if(ptr = strchr(string, '\n')) *ptr = '\0';
   if(ptr = strchr(string, '\r')) *ptr = '\0';
}

char strlwr(char *buf)
{
   int i = 0;
   while(buf[i] != '\0')
   {
      buf[i] = tolower(buf[i]);
      i++;
   }
   buf[i] = '\0';
}

char *strupr(char *buf)
{
   int i = 0;
   char tbuff, nbuff;
   while(buf[i] != '\0')
   {
      if(islower(buf[i]))
      {
          tbuff = toupper(buf[i]);
          strbuff[i++] = tbuff;
      }
      else if(isspace(buf[i]))
          strbuff[i++] = ' ';
      else
      {
         nbuff = buf[i-1];
         strbuff[i++] = nbuff;
      }
   }
   strbuff[i] = '\0';
   return(strbuff);
}

void del_wild(char *txt)
{
   char buff[1000];
   int i = 0, j = 0;
   buff[0]='\0';
   while(txt[i] != '\0')
   {
      if(txt[i] != '*' && txt[i] != '?') buff[j++] = txt[i];
      i++;
   }
   buff[j] = '\0';
   strcpy(txt, buff);
   strip_period(txt);
}

void strip_period(char *txt)
{
   char buff[1000];
   int i = 0, j = 0;
   buff[0]='\0';
   while(txt[i] != '\0')
   {
      buff[j++] = txt[i];
      if(txt[i] == '*' && txt[i+1] == '.') i+=2;
      else i++;
   }
   buff[j] = '\0';
   strcpy(txt, buff);
}

void strip_slash(char *ptr)
{
   while(*ptr && *ptr++ != ' ') if(*ptr == '/') *ptr = '\\';
}

void strip_bslash(char *text)
{
   char *ptr = NULL;
   while(ptr = strchr(text, '\\')) *ptr = '/';
}

void trailing_blanks(char *str)
{
   int i;
   for(i = strlen(str); i > 0; i--)
   {
      if(isspace(str[i-1])) str[i-1] = '\0';
      else return;
   }
}

char *conv_char(char *chan)
{
   char octval[1500], charval[1500];
   int i, chanlen;
   octval[0]='\0';charval[0]='\0';
   chanlen = strlen(chan);

   newchan[0]='\0';
   for(i = 0; i < chanlen; i++)
   {
       if(!isalnum(chan[i]))
       {
          sprintf(octval, "%%%03o", chan[i]);
          strcat(newchan, octval);
       }
       else
       {
          sprintf(charval, "%c", chan[i]);
          strcat(newchan, charval);
       }
   }
   return(newchan);
}

char *conv_oct(char *chan)
{
   char octval[1500], charval[1500], newval[100];
   int i, chanlen, newchanend;
   octval[0]='\0';charval[0]='\0';newval[0];
   chanlen = strlen(chan);


   for(i = 0; i < chanlen; i++)
   {
       newchanend=strlen(newchan);
       if(chan[i] == '%')
       {
          strncpy(charval,&chan[i+1],3);
          charval[3]='\0';
          newchan[newchanend]=(char)strtol(charval,(char **)NULL,8);
          i+=3;
       }
       else
       {
          strncat(newchan, &chan[i],1);
       }
       newchan[newchanend+1]='\0';
   }
   return(newchan);
}
