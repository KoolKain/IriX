#ifdef DEF_GLOBALS
char line[MAXLEN];     /* line parsed by the bot */
char linebuff[100];
char linebuff1[100];
char ip[100];
char dnsname[100];
char chanslist[6000];
char whoislist[6000];
char topiclist[1000];
char modelist[1000];
char banlist[6000];
char userhost[100];    /* global user@host of user */
char banmask[100]; 
char denied_msg[100];  /* msg to send when he is denied access */
char strbuff[70];  /* used in strupr and strlwr */
int socketfd;          /* socket descriptor */
int webfd;          /* socket descriptor */
int flood_count;       /* number of people who are flooding */
int flood_rate;        /* how many lines of message... */
int flood_time;        /* ...within what span of time? */
int reset_time;        /* time to wait for until it resets flood */
int htr;               /* number of help file topic tracker */
int alarmed;           /* did we call an alarm() call */
int msglog;
int added_level;
int servsync;
int no_servers;
int no_chans;
int found_it;
int end_map;
int gotone;
int joined;
int ucount;
int bicount;
int bccount;
int trk_servers;
int no_topics;
int trk_topics;
long uptime;           /* how long has the bot been up? */
long savetime;         /* when was the last time we saved the userlist? */
long pingtime;         /* when was the last time we pinged the server? */
long lasttime;         /* when was the last time we pinged the server? */
long purgetime;
long checkban;
long purge_time;
long botstart;
int purge_days;
char default_level[100];
char pid_file[100];
char botnick[100];
char ircuser[100];
char reason[100];
char mymodes[100];
char myserver[100];
char netpass[100];
char myuhost[100];
char myhost[100];
char ircname[100];
char msg_chan[100];
char quit_msg[100];
char gender_rpl[100]; 
char version_rpl[100]; 
char uworldhost[100]; 
char uworldnick[100]; 
char csadminchan[100]; 
char emailsub[100];
char mailprog[100];
char newtime[100];
char comchar; 
char cserv_email[100];
char regsuffix[100];
int autoregister;
int supporters;
int maxuserlevel; 
int maxadmnlevel; 
int maxnicklen; 
int deopchan; 
int logging; 
int webport;
int server_num;
char newchan[1000];

struct SERVERS servers[20];
struct HELPLIST helplist[100];
struct servinfo servinfo[NUMNICKBASE];
struct usernums usernums[MAXNUM];
struct burstinfo *bursthead = NULL;
struct mapinfo *maphead = NULL;
struct hubinfo *hubhead = NULL;
struct regsinfo *regshead = NULL;
struct modeinfo *modehead = NULL;
struct hubinfo *hubtail = NULL;
struct templist *listhead = NULL;
struct nickinfo *nickhead = NULL;
struct mailinfo *mailhead = NULL;
struct admninfo *admnhead = NULL;
struct flooding flood[100];
struct sortit sortcom[100];
struct bots bot;
struct PARSE_COMMAND parse_command[] = 
{
   { "SERVER",   parse_server  },
   { "ACTION",   parse_action  },
   { "PING",     parse_ping    },
   { "PONG",     parse_pong    },
   { "KICK",     parse_kick    },
   { "PRIVMSG",  parse_privmsg },
   { "MODE",     parse_mode    },
   { "KILL",     parse_kill    },
   { "JOIN",     parse_join    },
   { "NICK",     parse_nick    },
   { "QUIT",     parse_quit    },
   { "SQUIT",    parse_squit   },
   { "TOPIC",    parse_topic   },
   { "PASS",     parse_pass    },
   { "PART",     parse_part    },
   { "367",      parse_367     },
   { "353",      parse_353     },
   { "324",      parse_324     },
   { "332",      parse_332     },
   { "005",	 parse_005     },
   { "007",	 parse_007     },
   { "ADMIN",    parse_admin   },
   { "AWAY",     parse_away    },
   { "STATS",    parse_stats   },
   { "VERSION",  parse_version },
   { "BURST",    parse_burst   },
   { "CREATE",   parse_create  },
   { "END_OF_BURST",   parse_eoburst  },
   { "EOB_ACK",   parse_eob_ack},
   { NULL,       NULL          }
};

/*
command, filler, function, level, arg, disable, no_channel, security, chanopt
*/
struct COMMANDS commands[] = 
{
   { "addadmin",     " ",  add_admin,                800,   1, 0, 1, 1, 0  },
   { "deladmin",     " ",  delete_admin,             800,   1, 0, 1, 0, 0  },
   { "die",          " ",  die,                      900,   0, 0, 1, 0, 0  },
   { "modadmin",     " ",  mod_admin,                800,   1, 0, 1, 0, 0  },
   { "delchan",      " ",  admin_purge_channel,      700,   1, 0, 0, 0, 0  },
   { "nopurge",      " ",  purge_flag,               700,   1, 0, 0, 0, 0  },
   { "renchan",      " ",  renchan,                  700,   1, 0, 0, 0, 0  },
   { "write",        " ",  write_all_files,          800,   0, 0, 1, 0, 0  },
   { "adminreg",     " ",  opregister_channel,       700,   1, 0, 0, 1, 0  },
   { "restart",      " ",  rehash,                   900,   0, 0, 1, 0, 0  },
   { "chcomname",    " ",  change_comm_name,         900,   1, 0, 1, 0, 0  },
   { "chlevel",      " ",  change_comm_level,        900,   1, 0, 1, 0, 0  },
   { "disablecom",   " ",  disablecomm,              900,   1, 0, 1, 0, 0  },
   { "enablecom",    " ",  enablecomm,               900,   1, 0, 1, 0, 0  },
   { "showdiscom",   " ",  showdisabled,             501,   0, 0, 1, 0, 0  },
   { "chowner",      " ",  change_owner,             700,   1, 0, 0, 0, 0  },
   { "join",         " ",  join,                     800,   1, 0, 0, 0, 0  },
   { "leave",        " ",  part,                     800,   1, 0, 0, 0, 0  },
   { "inviteme",     " ",  inviteme,                 501,   0, 0, 1, 0, 0  },
   { "verifypass",   " ",  verify_pass,              501,   1, 0, 0, 1, 0  },
   { "rctopics",     " ",  reset_all_current_topics, 501,   0, 0, 1, 0, 0  },
   { "rdtopics",     " ",  reset_all_deftopics,      501,   0, 0, 1, 0, 0  },
   { "reop",         " ",  re_op_x,                  501,   0, 0, 1, 0, 0  },
   { "suspendchan",  " ",  suspend_channel,          700,   1, 0, 0, 0, 0  },
   { "listsuspend",  " ",  list_suspendchan,         501,   0, 0, 1, 0, 0  },
   { "uptime",       " ",  show_uptime,              000,   0, 0, 1, 0, 0  },
   { "cswall",       " ",  cswall,                   501,   1, 0, 1, 0, 0  },
   { "isauthed",     " ",  isauthed,                 501,   1, 0, 0, 0, 0  },
   { "csauth",       " ",  csauth,                   000,   1, 0, 1, 1, 0  },
   { "csdeauth",     " ",  csdeauth,                 000,   1, 0, 1, 0, 0  },
   { "unreg",        " ",  unregister_channel,       500,   1, 0, 0, 0, 0  },
   { "opall",        " ",  opall,                    400,   1, 0, 0, 0, 0  },
   { "suspendnick",  " ",  suspend,                  400,   1, 0, 0, 0, 0  },
   { "adduser",      " ",  add_user,                 450,   1, 0, 0, 1, 0  },
   { "kickbanall",   " ",  kick_ban_all,             450,   1, 0, 0, 0, 0  },
   { "kickall",      " ",  kick_all,                 400,   1, 0, 0, 0, 0  },
   { "clearbans",    " ",  clearban,                 300,   1, 0, 0, 0, 0  },
   { "clearmodes",   " ",  clearmode,                400,   1, 0, 0, 0, 0  },
   { "mode",         " ",  mode,                     100,   1, 0, 0, 0, 0  },
   { "deopall",      " ",  deopall,                  400,   1, 0, 0, 0, 0  },
   { "deluser",      " ",  delete_user,              450,   1, 0, 0, 0, 0  },
   { "moderate",     " ",  moderate,                 300,   1, 0, 0, 0, 0  },
   { "moduser",      " ",  mod_user,                 400,   1, 0, 0, 0, 0  },
   { "rdefaults",    " ",  reset_all_defs,           350,   1, 0, 0, 0, 0  },
   { "rdefmodes",    " ",  reset_defmodes,           350,   1, 0, 0, 0, 0  },
   { "rdeftopic",    " ",  reset_deftopic,           350,   1, 0, 0, 0, 0  },
   { "chanopt",      " ",  channel_options,          400,   1, 0, 0, 0, 0  },
   { "ban",          " ",  ban,                      100,   1, 0, 0, 0, 0  },
   { "unban",        " ",  unban,                    100,   1, 0, 0, 0, 0  },
   { "invite",       " ",  invite,                   100,   1, 0, 0, 0, 0  },
   { "nickban",      " ",  nickban,                  100,   1, 0, 0, 0, 0  },
   { "kickban",      " ",  kick_ban,                 100,   1, 0, 0, 0, 0  },
   { "auth",         " ",  authorize,                1,     1, 0, 0, 1, 0  },
   { "authop",       " ",  authop,                   1,     1, 0, 0, 1, 0  },
   { "deauth",       " ",  dauthorize,               0,     1, 0, 0, 0, 0  },
   { "deop",         " ",  deop,                     100,   1, 0, 0, 0, 0  },
   { "kick",         " ",  kick_user,                100,   1, 0, 0, 0, 0  },
   { "do",           " ",  do_action,                450,   1, 0, 0, 0, 0  },
   { "op",           " ",  giveop,                   100,   1, 0, 0, 0, 0  },
   { "newpass",      " ",  new_password,             1,     1, 0, 0, 1, 0  },
   { "say",          " ",  say_text,                 450,   1, 0, 0, 0, 1  },
   { "topic",        " ",  topic,                    100,   1, 0, 0, 0, 0  },
   { "info",         " ",  info,                     1,     1, 0, 0, 0, 0  },
   { "chaninfo",     " ",  show_chaninfo,            0,     1, 0, 0, 0, 0  },
   { "access",       " ",  show_access,              0,     1, 0, 0, 0, 0  },
   { "getdns",       " ",  get_dns,                  0,     1, 0, 1, 0, 0  },
   { "getip",        " ",  get_ip,                   0,     1, 0, 1, 0, 0  },
   { "help",         " ",  help,                     0,     0, 0, 1, 0, 0  },
   { "opers",        " ",  opers_invite,             0,     1, 0, 0, 0, 0  },
   { "register",     " ",  mailreg,                  0,     1, 0, 0, 0, 0  },
   { "purpose",      " ",  purpose,                  0,     1, 0, 1, 0, 0  },
   { "supporter",    " ",  supporter,                0,     1, 0, 1, 0, 0  },
   { "sendreg",      " ",  mail_it,                  0,     0, 0, 1, 0, 0  },
   { "seen",         " ",  lastseen,                 0,     1, 0, 0, 0, 0  },
   { "whois",        " ",  whois,                    0,     1, 0, 1, 0, 0  },
   { "admin",        " ",  admin,                    0,     0, 0, 1, 0, 0  },
   { "alist",        " ",  alist,                    501,   1, 0, 0, 0, 0  },
   { "isregistered", " ",  isregistered,             0,     1, 0, 0, 0, 0  },
   { "addthishost",  " ",  addthishost,              0,     1, 0, 0, 1, 0  },
   { "map",          " ",  show_map,                 0,     0, 0, 1, 0, 0  },
   { "operlist",     " ",  operlist,                 0,     0, 0, 1, 0, 0  },
   { "chanlist",     " ",  chan_list,                501,   0, 0, 1, 0, 0  },
   { "verify",       " ",  verify,                   0,     1, 0, 1, 0, 0  },
   { "banlist",      " ",  show_bans,                0,     1, 0, 0, 0, 0  },
   { "showcommands", " ",  showcommands,             0,     0, 0, 0, 0, 0  },
   { "strictop",     " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "notes",        " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "noban",        " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "chmodeslevel", " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "defmodes",     " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "ownernick",    " ",  generic,                  500,   1, 0, 0, 0, 1  },
   { "ownername",    " ",  generic,                  600,   1, 0, 0, 0, 1  },
   { "owneremail",   " ",  generic,                  600,   1, 0, 0, 0, 1  },
   { "welcome",      " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "motd",         " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "stay",         " ",  generic,                  500,   1, 0, 0, 0, 1  },
   { "banlevel",     " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "description",  " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "operop",       " ",  generic,                  500,   1, 0, 0, 0, 1  },
   { "noops",        " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "deftopic",     " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "chanurl",      " ",  generic,                  500,   1, 0, 0, 0, 1  },
   { "setwelcome",   " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "setvoice",     " ",  generic,                  400,   1, 0, 0, 0, 1  },
   { "locktopic",    " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "banmove",      " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "banremove",    " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "bantimeout",   " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "autoops",      " ",  generic,                  450,   1, 0, 0, 0, 1  },
   { "\001ping",     " ",  ctcp_ping,                0,     0, 0, 0, 0  },
   { "\001gender\001", " ",  ctcp_gender,            0,     0, 0, 0, 0  },
   { "\001version\001", " ",  ctcp_version,          0,     0, 0, 0, 0  },
   { "",             " ",  NULL,                     0,     0, 0, 0, 0  },
};
char convert2y[NUMNICKBASE] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M',
  'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
  'a','b','c','d','e','f','g','h','i','j','k','l','m',
  'n','o','p','q','r','s','t','u','v','w','x','y','z',
  '0','1','2','3','4','5','6','7','8','9',
  '[',']'};
#else
extern char line[];
extern char linebuff[];
extern char linebuff1[];
extern char ip[];
extern char dnsname[];
extern char chanslist[1000];
extern char whoislist[];
extern char topiclist[];
extern char banlist[];
extern char modelist[];
extern char userhost[];
extern char banmask[];
extern char denied_msg[];
extern char strbuff[];  
extern int socketfd;
extern int webfd;
extern int flood_count;
extern int flood_rate;
extern int flood_time;
extern int reset_time;
extern int htr;      
extern int alarmed; 
extern int added_level;
extern int servsync;
extern int no_servers;
extern int found_it;
extern int end_map;
extern int gotone;
extern int joined;
extern int ucount;
extern int bicount;
extern int bccount;
extern int no_chans;
extern int trk_servers;
extern int no_topics;
extern int trk_topics;
extern int msglog;
extern int server_num;
extern char newchan[];
extern long uptime;
extern long savetime;
extern long pingtime; 
extern long botstart;
extern long lasttime;
extern long purgetime;
extern long checkban;
extern long purge_time;
extern int purge_days;
extern char default_level[];
extern char pid_file[];
extern char botnick[];
extern char ircuser[];
extern char reason[];
extern char mymodes[];
extern char myserver[];
extern char netpass[];
extern char myuhost[];
extern char myhost[];
extern char ircname[];
extern char msg_chan[];
extern char quit_msg[];
extern char gender_rpl[]; 
extern char version_rpl[]; 
extern char uworldhost[]; 
extern char comchar; 
extern char cserv_email[];
extern char regsuffix[];
extern int autoregister;
extern int supporters;
extern int maxuserlevel; 
extern int maxadmnlevel; 
extern int maxnicklen; 
extern int deopchan; 
extern int logging; 
extern int webport;
extern char uworldnick[]; 
extern char csadminchan[];
extern char emailsub[];
extern char mailprog[];
extern char newtime[];
extern char convert2y[];

extern struct SERVERS servers[];
extern struct HELPLIST helplist[];
extern struct servinfo servinfo[];
extern struct usernums usernums[];
extern struct burstinfo *bursthead;
extern struct mapinfo *maphead;
extern struct regsinfo *regshead;
extern struct modeinfo *modehead;
extern struct hubinfo *hubhead;
extern struct hubinfo *hubtail;
extern struct nickinfo *nickhead;
extern struct mailinfo *mailhead;
extern struct templist *listhead;
extern struct admninfo *admnhead;
extern struct flooding flood[];
extern struct sortit sortcom[];
extern struct bots bot;
extern struct PARSE_COMMAND parse_command[];
extern struct COMMANDS commands[];
#endif
