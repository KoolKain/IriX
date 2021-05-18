#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <proto.h>

/*
#define DEBUG
*/
#define SECPERHR 3600
#define BadPtr(x) (!(x) || (*(x) == '\0'))
#define PRINT 1
#define CNTRL 2
#define ALPHA 4
#define PUNCT 8
#define DIGIT 16
#define SPACE 32
#define BOT_LOG "logs/bot.log"
#define PURGE_LOG "logs/purge.log"
#define AUTOOP 0
#define PROTECT 1
#define BURSTTIMER 15
#define HOPCOUNT 1
#define OWNERNICK 0
#define STRICTOP 1
#define NOBAN 2
#define CHMODESLEVEL 3
#define DEFMODES 4
#define OWNERNAME 5
#define OWNEREMAIL 6
#define WELCOME 7
#define BANLEVEL 8
#define DESCRIPTION 9
#define OPEROP 10
#define NOOPS 11
#define DEFTOPIC 12
#define CHANURL 13
#define STAY 14
#define SETWELCOME 15
#define SETVOICE 16
#define SUSPEND 17
#define TLOCK 18
#define NOPURGE 19
#define BANMOVE 20
#define BANREMOVE 21
#define BANTIMEOUT 22
#define AUTOOPS 23
#define MOTD 24
#define MAXWEBCONN 20
#define OWNER 0
#define MAXLEN 524
#define MAXTOPIC 500
#define MAXBUF 10240
#define TMP_FILE "temp.dat"
#define TOPIC_FILE "database/topics.dat"
#define CFG_FILE "config/cserve.conf"
#define LOG_FILE "logs/cmd.log"
#define ERRLOG_FILE "logs/err.log"
#define REG_FILE "database/regs.dat"
#define COM_FILE "config/commands.dat"
#define NICKLEN 50
#define PORTLEN 10
#define MAXDEF 80
#define TRUE 1
#define FALSE 0             /* 175 */
#define PINGINTERVAL 175  /* This should be less than ALARMINTERVAL */
#define WAITSEC 20
#define UWAITSEC 0
#define CHECKBANINTERVAL 7200 /*2 hours [7200] */
#define PURGEINTERVAL 86400 /*1 day [86400] */
#define ALARMINTERVAL 180  /* This should be greater than PINGINTERVAL */
#define SETTIMEINTERVAL 10800
#define RECHECK 25          /* Recheck to connect to server in seconds */
#define CTCP_DELIM_CHAR '\001'
#define NUMNICKBASE 64
#define MAXNUM 4096
#define Reg1 register
#define Reg2 register
#define Reg3 register

#include <structs.h>
#include <globals.h>
