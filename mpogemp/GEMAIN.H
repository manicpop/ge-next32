
/***************************************************************************
 *                                                                         *
 *   GEMAIN.H                                                              *
 *                                                                         *
 *   Copyright (C) 1988, 89, 90, 91, 92 Michael B. Murdock                 *
 *                                                                         *
 *   This is the source for the Galactic Empire game module                *
 *                                                                         *
 *                                         M. Murdock     03/17/92         *
 *                                                                         *
 *   ge-next                                                               *
 *                                                                         *
 *   Copyright (C) 2024-2026 Anthony Schmidt, anthony@manicpop.org         *
 *                                                                         *
 *   https://manicpop.org/ge-next/  https://github.com/manicpop/ge-next    *
 *                                                                         *
 ***************************************************************************/

 /**************************************************************************
  * This program is free software; you can redistribute it and/or modify   *
  * under the terms of the GNU General Public License as published by the  *
  * Free Software Foundation; either version 2 of the License, or (at your *
  * option) any later version.                                             *
  *                                                                        *
  * This program is distributed in the hope that it will be useful,        *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
  * General Public License for more details.                               *
  *                                                                        *
  * You should have received a copy of the GNU General Public License      *
  * along with this program; if not, write to the Free Software Foundation,*
  * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA            *
  *************************************************************************/

#define TRUE 1
#define FALSE 0

/* if this is the trainer version this define needs to be made */
/*
#define	GETRAINER 1
*/

#ifdef GETRAINER
#define VERSION "ge-next 0.3-beta2 Trainer"
#define GEHELP  "MPOG2HLP.MCV"
#define GEMSG   "MPOG2MSG.MCV"
#else
#define VERSION "ge-next 0.3-beta2"
#define GEHELP  "MPOGEHLP.MCV"
#define GEMSG   "MPOGEMSG.MCV"
#endif

#ifdef GETRAINER
#include "mpog2msg.h"
#include "mpog2hlp.h"
#include "mpogeshp.h"
#else
#include "mpogemsg.h"
#include "mpogehlp.h"
#include "mpogeshp.h"
#endif

/* workarounds for mbbsemu math functions */
/* hopefully not necessary much longer */
/* #define MBBSEMU 1 */


/***************************************************************************/
/* The following defines have been carefully chosen and refined to provide */
/* a well balanced and entertaining game play. Should they be modified,    */
/* do so with caution and consideration                                    */
/***************************************************************************/

#define NEUTRAL_X 0
#define NEUTRAL_Y 0

#define ACCENGAMT 120		/* Acceleration energy usage                 */

#define PRELOAD 10		/* Phasor reload rate / tick                 */
#define PMINFIRE 60		/* Minimum amount to fire phasor             */
#define PENGUSE 57		/* Amount of energy used by phasors charging */
#define PMINENG 500		/* minimum energy to fire                    */
#define PHABIAS 2		/* Degrees to bias "spread" to better aim    */

#define HPFIRAMT 5000
#define HPBEAMW 5

#define ENGYMAX 65000U		/* Maximum energy one can have               */
#define ENGRECHG 2		/* Energy recharge amount / tick             */

#define SHENGUSE 100		/* Energy used every tick for shields        */
#define SHHITENG 1000		/* Energy drained when hit                   */
#define SHMAXCHG 10		/* Max (full) charge in the shields          */
#define SHMINCHG 5		/* Minimum charge to be effective            */

#define SSMAX 10000

#define SNMSIZ 25

#define PI 3.1415927
#define SLCAP 2147483647L	/* maximum value of signed long */
#define ULCAP 4294967295UL	/* maximum value of unsigned long */
#define FARSPEED 54321L		/* speed constant for cyb long pursuit */
#define VEC_EPS 1e-9

#define GELOOKUP 1
#define GEADD 2
#define GEDELETE 3
#define GEUPDATE 4
#define GEGET 5
#define GENEXT 6
#define GELOOKUPNAME 7
#define GEGETNOW 8

#define MAXPLANETS 9

#define MAXX 30
#define MAXY 15

/* do not change these as they effect structure sizes */
#define MAXTORPS 3
#define MAXMISSL 3
#define MAXDECOY 10

#define DECOYTIME 15		/* times TICKTIME how long a decoy lives */
#define TICKTIME 6		/* how often does RTKICK fire    6   */
#define TICKTIME2 1		/* how often does RTKICK fire       */
#define CYBTICKTIME 5		/* Cyborgs do stuff this fast       */

#define FIRETICKS 10		/* how many ticks to hold a player  */
				/* when involved in battle          */

#define LOCKGRACE 5		/* ticks to retain a hidden lock target */
#define CYBGRACE 10		/* ticks to retain hidden cyb target    */

#define NUMITEMS 14

#define I_MEN 0
#define I_MISSILE 1
#define I_TORPEDO 2
#define I_IONCANNON 3
#define I_FLUXPOD 4
#define I_FOOD 5
#define I_FIGHTER 6
#define I_DECOYS 7
#define I_TROOPS 8
#define I_ZIPPERS 9
#define I_JAMMERS 10
#define I_MINE 11
#define I_GOLD 12
#define I_SPY 13

#define SHIELDUP 1
#define SHIELDDN 2
#define SHIELDDM 3

#define SHIELD_FACTOR 4

#define COUNTDOWN 20		/* self destruct countdown */

#define CYB_MINDAM 75		/* % of damage when cyb jams and mines */

#define TELEDAM 17		/* amount of damage done when edge reached */

#define MINERANGE 10000		/* maximum Mine range                     */

#define DESTRUCTRANGE 10000

#define NEBRNG 1200		/* SE scan visibility range inside nebulas */

#define NUM_MINES 20		/* 20 mines in the game max */

#define TOPSHIELD 19

#define TOPPHASOR 19

#define SECTYPE_NORMAL 1
#define PLTYPE_PLNT 2
#define PLTYPE_WORM 3

#define GESTAT_AVAIL 0
#define GESTAT_USER 1
#define GESTAT_AUTO 2
#define GESTAT_IDLE 3

#define CLASSTYPE_USER 1
#define CLASSTYPE_CYBORG 2
#define CLASSTYPE_DROID 3
#define CLASSTYPE_NONE 4


#define MAIL_CLASS_DISTRESS 1
#define MAIL_CLASS_UNUSED1 2
#define MAIL_CLASS_PRODRPT 3
#define MAIL_CLASS_UNUSED2 4
#define MAIL_CLASS_UNUSED3 5

/* user options */

#define SCANOPTS 0
#define UNUSED1 1
#define UNUSED2 2
#define MSG_FILTER 3
#define UNUSED4 4
#define UNUSED5 5

#define	MAXTEAMS 50

#define BEACONMSGSZ 75

#define	GEMSGSIZ 784
#define FIXEDMSGSIZ sizeof(struct message)+GEMSGSIZ

#define HSTSIZ 57
#define TPCSIZ 51

/* message filter types */

#define	FLT_CYB_ALL 0
#define	FLT_CYB_BAT 1
#define	FLT_CYB_APP 2
#define	FLT_DISTRESS 3
#define	FLT_BEACON 4
#define	FLT_HAIL 5
#define	FLT_ENTRY 6
#define	FLT_SHIP 7
#define	FLT_NONE 8

/* how many seconds to delay 'reduced' entry messages */
#define	ENTRYWAIT 30

/* message filter bit masks */

#define	MSGF_CYBS_MASK 0x03
#define	MSGF_DISTRESS 0x04
#define	MSGF_BEACON 0x08
#define	MSGF_HAIL 0x10
#define	MSGF_ENTRY_MASK 0x60
#define	MSGF_SHIP 0x80

/* Cybertron message types */

#define APPROACH 0
#define LOATTACK 1
#define HIATTACK 2
#define IGNORE 3
#define NEUTRAL 4
#define TAUNT 5
#define CYBTORP 6
#define FLEE 7
#define CYBBASEA 8
#define CYBBASEB 9

/* map types */
#define SECTOR 0
#define SECTORFULL 1
#define RANGE 2
#define RANGEFULL 3
#define RANGENAMES 4
#define RANGEEXTRA 5
#define LONG 6
#define RANGENOMAP 7
#define SECTORNOMAP 8

/* scan settigns */
#define SIMPLE 0
#define FULL 1
#define FULLNAMES 2
#define FULLEXTRA 3
#define NOMAP 4

/* colors */
#define CLR_RED1	"\33[0;31m"
#define CLR_RED2	"\33[1;31m"
#define CLR_GREEN1	"\33[0;32m"
#define CLR_GREEN2	"\33[1;32m"
#define CLR_YELLOW1	"\33[0;33m"
#define CLR_YELLOW2	"\33[1;33m"
#define CLR_BLUE1	"\33[0;34m"
#define CLR_BLUE2	"\33[1;34m"
#define CLR_PURPLE1	"\33[0;35m"
#define CLR_PURPLE2	"\33[1;35m"
#define CLR_CYAN1	"\33[0;36m"
#define CLR_CYAN2	"\33[1;36m"
#define CLR_WHITE1	"\33[0;37m"
#define CLR_WHITE2	"\33[1;37m"

/* ship upgrades */
#define ENHSCAN 1
#define ARMOR 2
#define ACCELBST 4
#define SCANBST 8
#define ENHLOCK 16
#define DAMCTRL 32
#define TPONDER 64
#define NCORE 128

#define TPONNORM 0
#define TPONLOW 1
#define TPONHIGH 2

/*****************************************************************************/
/* End of Defines                                                            */
/*****************************************************************************/

#ifndef byte
#define byte unsigned char
#endif

/* message structure from GME.H, Copyright (c) 1994-1995 GALACTICOMM, Inc. */

struct message {
	long msgno;
	char userto[UIDSIZ];
	char from[UIDSIZ];
	char to[UIDSIZ];
	char topic[TPCSIZ];
	char auxtpc[HSTSIZ];
	unsigned crdate;
	unsigned crtime;
	int nreply;
	int flags;
	char spare[5];
	char text[1];
};

#define COORD struct _coord
extern
COORD	{
	double xcoord;
	double ycoord;
	};

#define MINE struct _mine
MINE	{
	byte channel;
	unsigned timer;
	COORD coord;
	};

#define MISSILE struct _missile
MISSILE	{
	unsigned char channel;
	unsigned distance;
	unsigned energy;
	};

#define TORPEDO struct _torpedo
TORPEDO	{
	unsigned char channel;
	unsigned distance;
	};


#define WARUSR struct warusr
extern


WARUSR	{
	char		userid[UIDSIZ];		/*   userid of player                     */
/*30*/
	unsigned long	score;			/*   Your score                           */
/*34*/
	char		waste[40];
/*74*/
	unsigned	noships;		/*   Number of ships                      */
/*76*/
	int		topshipno;		/*   Top ship number used                 */
/*78*/
	unsigned	kills;			/*   No kills                             */
/*80*/
	unsigned	rospos;			/*   unused atm                           */
/*82*/
	int		planets;		/*   Planets owned                        */
/*84*/
	unsigned long	cash;			/*   cash on hand                         */
/*88*/
	unsigned long	debt;			/*   this isn't used but it could be helpful later   */
/*92*/
	unsigned long	plscore;
/*96*/
	unsigned long	klscore;
/*100*/
	unsigned long	population;
/*104*/
	byte		options[30];
/*134*/
	unsigned long	teamcode;
/*138*/
	byte		factions[8];		/* do NPCs like me */
/*146*/
	unsigned	ukills;			/* just user kills */
/*148*/
			char		filler[256-115-UIDSIZ]; /*   WARUSR in GE classic is 256             */
	};


#define WARSHP struct warshp
extern


WARSHP	{
	char		userid[UIDSIZ];		/*   userid of player                        */
/*30*/
	int		shipno;			/*   Ship id number                          */
/*32*/
	char		shipname[35];		/*   Ship name                               */
/*67*/
	int		shpclass;		/*   Ship Class   display                    */
/*69*/
	double		heading;		/*   Heading of this ship                    */
/*77*/
	double		head2b;			/*   direction rotating to                   */
/*85*/
	double		speed;			/*   Speed                                   */
/*93*/
	double		speed2b;		/*   Speed to be                             */
/*101*/
	COORD		coord;
/*117*/
	double		damage;			/*   Damage on this ship (0 - 100%)          */
/*125*/
	double		energy;			/*   Amount of energy available              */
/*133*/
	double		phasr;			/*   Phasor strength (0 - 100%)              */
/*141*/
	byte		phasrtype;		/*   Type of Phasor system                   */
/*142*/
	unsigned	kills;			/*   No kills                                */
/*144*/
	int		lastfired;		/*   usernumber of last user to fire on you  */
/*146*/
	byte		shieldtype;		/*   shield type                             */
/*147*/
	byte		shieldstat;		/*   shield status (supposed to be up/dn)    */
/*148*/
	int		shield;			/*   shield charge                           */
/*150*/
	int		cloak;			/*   cloak flag 0 = down, 10 = up, neg = dmg */
/*152*/
	int		degrees;		/*   work fields for calculating degrees     */
/*154*/
	int		percent;		/*   Work field for calculating percentages  */
/*156*/
	int		tactical;		/*   Tactical Display damage indicator       */
/*158*/
	int		helm;			/*   Helm Control damage indicator           */
/*160*/
	int		train;			/*   Training screen counter                 */
/*162*/
	int		where;			/*   Hyperspace flag                         */
/*164*/
	TORPEDO		ltorps[MAXTORPS];	/*   Locked on torpedoes                     */
/*173*/
	MISSILE		lmissl[MAXMISSL];	/*   Locked on missiles                      */
/*188*/
	unsigned	decout[MAXDECOY];	/*   decoys out                              */
/*208*/
	byte		jam_sev;		/*   jamming severity                        */
/*209*/
	byte		jam_time;		/*   time left in jamming                    */
/*210*/
	unsigned	freq;			/*   encoded message freq / npc state trckr  */
/*212*/
	byte		tponder;		/*   transponder control state               */
	byte		lock_grace;		/*   lock invisibility grace                 */
	byte		cyb_grace;		/*   cyb invisibility grace                  */
	char		filler2[1];		/*   eventually i will restruct this struct  */
/*216*/
	unsigned long	items[NUMITEMS];	/*   all the items                           */
/*272*/
	unsigned	titem;			/*   establishing a colony mode temp item    */
/*274*/
	unsigned	hostile;		/*   being hostile flag                      */
/*276*/
	unsigned	cantexit;		/*   cannot exit counter                     */
/*278*/
	unsigned	repair;			/*   repairs underway flag                   */
/*280*/
	unsigned	hypha;			/*   hyper-phasers cooling flag              */
/*282*/
	byte		torpcntl;		/*   torpedo damage flag                     */
/*283*/
	byte		mislcntl;		/*   misl damage flag                        */
/*284*/
	byte		destruct;		/*   self destruct countdown                 */
/*285*/
	byte		status;			/*   Status of this record                   */
/*286*/
	byte		cybmine;		/*   I have found me a ship to seek          */
/*287*/
	byte		upgrade;		/*   installed ship upgrades                  */
/*288*/
	byte		cybupdate;		/*   update cyborg counter                   */
/*289*/
	byte		tick;			/*   Ship Ticker                             */
/*290*/
	byte		distress;		/*   has sent distress message               */
/*291*/
	byte		minesnear;		/*   set when mines are near                 */
/*292*/
	int		lock;			/*   set to the ship we are locked onto      */
/*294*/
	byte		holdcourse;		/*   Hold course ticker                      */
/*295*/
	int		topspeed;		/*   top speed considering damage            */
/*297*/
	byte		npcmsg;			/*   keeps track of last annoy msg sent      */
/*298*/
	unsigned long	overspeed;		/*   counter for over maximum warp           */
/*302*/
	int		zipload;		/*   zipper launch throttle                  */
/*304*/
	int		jamload;		/*   jam launch throttle                     */
/*306*/
	int		decload;		/*   decoy launch throttle                   */
/*308*/
	int		mineload;		/*   mine launch throttle                    */
/*310*/
	byte		torps_fired;		/*   torpedoes fired this tick               */
/*311*/
	byte		missl_fired;		/*   missiles fired this tick                */
/*312*/
	unsigned	ukills;			/*   just user kills                         */
/*314*/
	char		filler[514-280-UIDSIZ]; /*   WARSHP in GE classic is 514             */
	};

/*
#define X1 ptr1->xcoord
#define X2 ptr2->xcoord
#define Y1 ptr1->ycoord
#define Y2 ptr2->ycoord
*/

#define ITEM    struct item
extern
ITEM	{
	unsigned long	qty;				/* quantity on hand */
	unsigned	rate;				/* rate of manufacture */
	char		sell;				/* sell to allies? */
	unsigned	reserve;			/* qty to reserve */
	unsigned	markup2a;			/* mark up to allies */
	unsigned long	sold2a;				/* # sold to allies  */
	};

#define PLNTCOORD struct _plntcoord
extern
PLNTCOORD	{
		int	type;				/* planet type */
		COORD	coord;				/* location */
		};


#define PLANETAB struct planetab

extern
PLANETAB	{
		PLNTCOORD	planets[MAXPLANETS];
		};

#define GALSECT struct galsect

extern
GALSECT	{					/* star system data record              */
	int	xsect;				/* sector x coord                       */
/*2*/
	int	ysect;				/* sector y coord                       */
/*4*/
	int	plnum;				/* always 0                             */
/*6*/
	int	type;				/* type of sector                       */
/*8*/

	int	numplan;			/* number of planetary objects          */
/*10*/
	PLNTCOORD	ptab[MAXPLANETS];
/*172*/
	char	filler[512-10-(sizeof(PLNTCOORD)*MAXPLANETS)];		/*   GALSECT in GE classic is 512            */
};

#define GALPLNT struct _galplnt

extern
GALPLNT {					/* star system data record              */
	int	xsect;				/* sector x coord                       */
/*2*/
	int	ysect;				/* sector y coord                       */
/*4*/
	int	plnum;				/* planet number                        */
/*6*/
	int	type;				/* type of sector                       */
/*8*/
	COORD	coord;
/*24*/
	char	userid[UIDSIZ];
/*54*/
	char	name[20];
/*74*/
	char	enviorn;			/* environment factor                   */
/*75*/
	char	resource;			/* resources                            */
/*76*/
	unsigned long cash;			/* cash on hand                         */
/*80*/
	unsigned long timestamp;		/* time of last update                  */
/*84*/
	unsigned long tax;			/* amount of tax collected              */
/*88*/
	int	taxrate;			/* rate to tax population               */
/*90*/
	char	warnings;			/* warnings to intruders                */
/*91*/
	char	password[11];			/* password for alliance                */
/*102*/
	char	filler2[UIDSIZ-1];		/* unused                               */
/*131*/
	ITEM	items[NUMITEMS];		/* 195                                  */
/*355*/
	char	beacon[BEACONMSGSZ];
/*430*/
	char	spyowner[UIDSIZ];
/*460*/
	unsigned int	plantimesave;
/*462*/
	unsigned long	teamcode;
/*466*/
	unsigned long	nebseed;			/*   random seed for nebula map (Zygor only) */
/*470*/
	char	filler[512-186-(UIDSIZ*2)-(sizeof(ITEM)*NUMITEMS)]; /*   GALPLNT in GE classic is 512            */

};

#define GALWORM struct galworm

extern
GALWORM {					/* star system data record              */
	int	xsect;				/* sector x coord                       */
/*2*/
	int	ysect;				/* sector y coord                       */
/*4*/
	int	plnum;				/* planet number                        */
/*6*/
	int	type;				/* type of sector                       */
/*8*/
	COORD	coord;
/*24*/
	int	visible;			/* is the wormhole visible flag         */
/*26*/
	COORD	destination;
/*42*/
	char	name[20];
/*62*/
	char	filler[512-30-(sizeof(COORD)*2)];	/*   GALWORM in GE classic is 512            */
};


#define PKEY struct _pkey

extern
PKEY {						/* star system data record key          */
	int	xsect;				/* sector x coord                       */
	int	ysect;				/* sector y coord                       */
	int	plnum;				/* always 0                             */
};

#define WORMTAB	struct _wormtab

extern
WORMTAB	{
	COORD	coord;
	COORD	dest;
	};


#define MAILKEY struct _mailkey

extern
MAILKEY	{
	char	userid[UIDSIZ];
	int	class;
	long	msgno;
	};

#define MAIL struct _mail

extern
MAIL    {
	char	userid[UIDSIZ];
/*30*/
	int	class;
/*32*/
	int	type;
/*34*/
	int	stamp;
/*36*/
	char	dtime[20];
/*56*/
	char	topic[30];
/*86*/
	char	string1[80];
/*166*/
	char	name1[25];
/*191*/
	char	name2[25];
/*216*/
	int	int1;
/*218*/
	int	int2;
/*220*/
	int	int3;
/*222*/
	long	long1;
/*226*/
	long	long2;
/*230*/
	long	long3;
/*234*/
	char	filler[253-200-UIDSIZ];		/*   MAIL in GE classic is 253               */
	};

#define MAILSTAT struct _mailstat
extern
MAILSTAT	{
			char	userid[UIDSIZ];
	/*30*/
			int	class;
	/*32*/
			int	type;
	/*34*/
			int	stamp;
	/*36*/
			char	dtime[20];
	/*56*/
			char	topic[30];
	/*86*/
			char	name1[25];
	/*111*/
			int	int1;
	/*113*/
			int	int2;
	/*115*/
	unsigned long	cash;				/* cash on hand                 */
	/*119*/
	unsigned long	timestamp;			/* time of last update          */
	/*123*/
	unsigned long	tax;				/* amount of tax collected      */
	/*127*/
	unsigned long	itemqty[NUMITEMS];
	/*183*/
	char	filler[253-97-(UIDSIZ)-(sizeof(long)*NUMITEMS)];	/*   MAILSTAT in GE classic is 253           */
			};

#define SHPKEY struct _shpkey

extern
SHPKEY	{
	char	userid[UIDSIZ];
	int	shipno;
	};

#define SHIP struct _ship

extern
SHIP	{
	char		*typename;
	char		*npcprefx;
	unsigned char	max_shlds;
	unsigned char	max_phasr;
	unsigned char	max_torps;
	unsigned char	max_missl;
	unsigned char	has_decoy;
	unsigned char	has_jam;
	unsigned char	has_zip;
	unsigned char	has_mine;
	unsigned char	max_attk;
	unsigned char	max_cloak;
	unsigned int	max_accel;
	unsigned char	max_warp;
	long		max_tons;
	long		max_price;
	unsigned int	max_points;
	unsigned int	max_type;
	long		scanrange;
	unsigned char	cybs_can_att;
	unsigned char	lowest_to_attk;
	unsigned char	noclaim;
	unsigned int	tot_to_create;
	unsigned char	tough_factor;
	unsigned int	damfact;
	unsigned int	faction;
	unsigned int	loadout;

	int		hlpmsg;
	void		(*init_func)	(WARSHP *, int, int);
	void		(*tick_func)	(WARSHP *, int);
	void		(*kill_func)	(WARSHP *, int, WARSHP *);
	void		(*won_func)	(WARSHP *, int, WARSHP *);
	};


struct cmd
	{
	char *command;
	void (*func)();
	byte cando;
	};

#define SHIPTAB struct _shiptab

extern
SHIPTAB	{
	int	shipno;
	char	letter;
	char	flag;
	double	dist;
	int	bearing;
	int	heading;
	double	speed;
	};

#define NOSCANTAB 15
#define SCANTAB struct _scantab

extern
SCANTAB	{
	SHIPTAB ship[NOSCANTAB];
	};

#define ENTRYTAB struct _entrytab
extern
ENTRYTAB	{
	unsigned char	active;
	unsigned char	ticks;
	};


#define S00	struct _s00

extern
S00	{
	char	*name;
	char	*owner;
	int	type;
	double	xcoord;
	double	ycoord;
	int	env;
	int	res;
	};

#define TEAM	struct _team
extern
TEAM	{
	long		teamcode;
	char		teamname[31];
	unsigned int	teamcount;
	unsigned int	teamdeldate;
	char		password[11];
	char		secret[11];
	int		flag;
	};


extern unsigned char	beacontimer;
extern ENTRYTAB		*entrytab;
extern unsigned char	*entrysent;
extern unsigned char	*entrypend;
extern int		entrybytes;

#define FUNC _export
