struct mailinfo
{
  char channel[210];
  char idnick[50];
  char email[100];
  char ownermask[100];
  char purpose[512];
  long created;
  struct mailinfo *next;
  struct suppinfo *supphead;
};

struct suppinfo
{
  int suppnum;
  char nick[50];
  char email[100];
  struct suppinfo *next;
};

struct templist
{
  char channel[210];
  int level;
  struct templist *next;
};

struct mapinfo
{
  char mapline[100];
  struct mapinfo *next;
};

struct hubinfo
{
   char main[100];
   char link[100];
   struct hubinfo *next;
   struct hubinfo *prev;
};

struct usernums
{
   char nick[50];
   char chars[4];
   int used;
};

struct servinfo
{
   char server[100];
   char chars;
   int used;
};

struct SERVERS
{
   char name[100];
   int port;
};

struct HELPLIST
{
   char name[30];
   long pos;
};

struct nickinfo 
{
   char nick[50];
   char user[50];
   char host[100];
   char server[100];
   char name[200];
   int is_oper;
   char modes[10];
   int is_ops;
   char away[2];
   char numeric[4];
   struct chaninfo *chanhead;
   struct authinfo *authhead;
   struct nickinfo *next;
};

struct authinfo
{
   char channel[201];
   struct authinfo *next;
};

struct burstinfo
{
   char channel[201];
   char modes[100];
   long chanstamp;
   struct burstinfo *next;
};

struct chaninfo
{
   char channel[201];
   int is_ops;
   char modes[100];
   struct chaninfo *next;
};

struct regsinfo
{
   char channel[210];
   long lastseen;
   char ownaddr[100];
   char chanstamp[10];
   char modes[100];
   char pmodes[100];
   char topic[512];
   char url[512];
   char deftopic[512];
   char defmodes[100];
   char ownernick[50];
   char ownername[100];
   char welcome[512];
   char motd[512];
   char desc[512];
   char ownemail[100];
   char setupdate[10];
   long created;
   int strictop;
   int noban;
   int chmodlvl;
   int banlvl;
   int operop;
   int noops;
   int stay;
   int setwelc;
   int setpurge;
   int setvoice;
   int suspend;
   int tlock;
   long banmove;
   long banremove;
   long bantimeout;
   int autoops;
   struct baninfo *banhead;
   struct userinfo *userhead;
   struct regsinfo *next;
};

struct baninfo
{
  char ban[200];
  long timeset;
  int level;
  long duration;
  char reason[512];
  char nick[50];
  int botban;
  struct baninfo *next;
};

struct userinfo 
{
   char nick[50];
   char password[15];
   int autoop;
   int protect;
   int level;
   int suspend;
   long lastseen;
   char info[512];
   struct adderinfo *adderhead;
   struct numinfo *numhead;
   struct userinfo *next;
};

struct numinfo
{
   char numeric[4];
   struct numinfo *next;
};

struct adderinfo
{
   char address[100];
   struct adderinfo *next;
};

struct flooding 
{
   char name[50];
   long time;
   int count;
   int bool;
   int kick;
};

struct bots 
{
   char nick[NICKLEN];
   char ircname[MAXDEF];
   char username[MAXDEF];
   char server[MAXDEF];
   int port;
   long lastpinged;
   int server_active;
};

struct PARSE_COMMAND
{
   char *name;
   void (*function)(char *, char *);
}; 

struct COMMANDS
{
   char name[30];
   char realname[30];
   void (*function)(char *, char *);
   int level;
   int needarg;
   int disable;
   int nochan;
   int secure;
   int chanopt;
}; 

struct sortit
{
   char name[30];
   int level;
}; 
