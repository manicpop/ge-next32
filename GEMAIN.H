
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
/* #define GETRAINER 1 */

/* if FASTPLANET defined updates happen every 3 seconds */
/* #define FASTPLANET 1 */

/* the DATA command is currently in disrepair and disabled */
/* #define DATACMD 1 */

/* workarounds for mbbsemu math functions */
/* hopefully not necessary much longer */
/* #define MBBSEMU 1 */

#ifdef GETRAINER
#define VERSION "ge-next 0.3-rc2 Trainer"
#define GEHELP  "MPOG2HLP.MCV"
#define GEMSG   "MPOG2MSG.MCV"
#else
#define VERSION "ge-next 0.3-rc2"
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

/***************************************************************************/
/* The following defines have been carefully chosen and refined to provide */
/* a well balanced and entertaining game play. Should they be modified,    */
/* do so with caution and consideration                                    */
/***************************************************************************/

#define NEUTRAL_X 0		/* neutral-zone x sector                   */
#define NEUTRAL_Y 0		/* neutral-zone y sector                   */

#define ACCENGAMT 120		/* Acceleration energy usage               */

#define PRELOAD 10		/* Phaser reload rate / tick               */
#define PMINFIRE 60		/* Minimum amount to fire phaser           */
#define PENGUSE 57		/* Energy used by phaser charging          */
#define PHABIAS 2		/* Degrees to bias "spread" to better aim  */

#define HPFIRAMT 5000		/* hyperphaser energy cost                 */
#define HPBEAMW 5		/* hyperphaser beam width                  */

#define ENGYMAX 65000U		/* Maximum energy one can have             */
#define ENGRECHG 2		/* Energy recharge amount / tick           */

#define SHENGUSE 100		/* Energy used every tick for shields      */
#define SHMINCHG 5		/* Minimum charge to be effective          */

#define SSMAX 10000		/* sector size in coordinate units         */

#define PI 3.1415927		/* pi                                      */
#define SLCAP 2147483647L	/* maximum value of signed long            */
#define ULCAP 4294967295UL	/* maximum value of unsigned long          */
#define FARSPEED 54321L		/* speed constant for cyb long pursuit     */
#define VEC_EPS 1e-9		/* epsilon for vector()                    */

#define GELOOKUP 1		/* database lookup by key                  */
#define GEADD 2			/* database insert                         */
#define GEDELETE 3		/* database delete                         */
#define GEUPDATE 4		/* database update                         */
#define GEGET 5			/* database fetch if not already loaded    */
#define GENEXT 6		/* database next-record iteration          */
#define GELOOKUPNAME 7		/* database lookup by alternate name key   */
#define GEGETNOW 8		/* database fetch unconditionally          */

#define MAXPLANETS 9		/* max planets/wormholes per sector        */

#define MAXX 30			/* scan map width                          */
#define MAXY 15			/* scan map height                         */

/* do not change these as they effect structure sizes */
#define MAXTORPS 3
#define MAXMISSL 3
#define MAXDECOY 10

#define DECOYTIME 15		/* times TICKTIME how long a decoy lives   */
#define TICKTIME 6		/* main ship-state tick interval in seconds */
#define TICKTIME2 1		/* movement tick interval in seconds       */
#define CYBTICKTIME 5		/* Cyborgs do stuff this fast              */

#define FIRETICKS 10		/* normal cantexit delay when in battle    */

#define LOCKGRACE 5		/* ticks to retain a hidden lock target    */
#define CYBGRACE 10		/* ticks to retain hidden cyb target       */

#define NUMITEMS 14		/* number of items below                   */

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

#define SHIELDUP 1		/* shields are up                          */
#define SHIELDDN 2		/* shields are down                        */
#define SHIELDDM 3		/* shields are damaged                     */

#define SHIELD_FACTOR 4		/* shieldhit() factor                      */

#define COUNTDOWN 20		/* self destruct countdown                 */

#define CYB_MINDAM 75		/* % of damage when cyb jams and mines     */

#define TELEDAM 17		/* damage when hitting galactic barrier    */

#define MINERANGE 10000		/* maximum mine range                      */

#define DESTRUCTRANGE 10000	/* maximum self-destruct explosion range   */

#define NEBRNG 1200		/* SE scan visibility range inside nebulas */

#define TOPSHIELD 19		/* highest shield class index              */

#define TOPPHASOR 19		/* highest phaser class index              */

#define SECTYPE_NORMAL 1	/* normal sector record                    */
#define PLTYPE_PLNT 2		/* planet record                           */
#define PLTYPE_WORM 3		/* wormhole record                         */

#define GESTAT_AVAIL 0		/* unused ship slot                        */
#define GESTAT_USER 1		/* live user-controlled ship               */
#define GESTAT_AUTO 2		/* live automaton ship                     */

#define CLASSTYPE_USER 1	/* user ship class                         */
#define CLASSTYPE_CYBORG 2	/* cyborg ship class                       */
#define CLASSTYPE_DROID 3	/* droid ship class                        */
#define CLASSTYPE_NONE 4	/* undefined/inactive class slot           */

#define VALID_SHPCLASS(c) ((c) >= 0 && (c) < tot_classes)
#define VALID_AUTOCLASS(c) (VALID_SHPCLASS(c) && \
	(shipclass[(c)].max_type == CLASSTYPE_CYBORG || \
	 shipclass[(c)].max_type == CLASSTYPE_DROID))

#define MAIL_CLASS_DISTRESS 1	/* distress-message mail class             */
#define MAIL_CLASS_UNUSED1 2
#define MAIL_CLASS_PRODRPT 3	/* production-report mail class            */
#define MAIL_CLASS_UNUSED2 4
#define MAIL_CLASS_UNUSED3 5

/* user options */
#define SCANOPTS 0		/* scan display mode option slot           */
#define UNUSED1 1		/* reserved user option slot               */
#define UNUSED2 2		/* reserved user option slot               */
#define MSG_FILTER 3		/* message filter option slot              */
#define UNUSED4 4		/* reserved user option slot               */
#define UNUSED5 5		/* reserved user option slot               */

#define	MAXTEAMS 50		/* max team records in memory              */

#define BEACONMSGSZ 75		/* max planet beacon message length        */

#define	GEMSGSIZ 784		/* max GE mail payload size                */
#define FIXEDMSGSIZ sizeof(GEMESSAGE)	/* fixed stored mail record size */

#define HSTSIZ 57		/* borrowed MajorBBS message topic size    */
#define TPCSIZ 51		/* borrowed MajorBBS message aux/history size */

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
#define SIMPLE 0		/* just the map                           */
#define FULL 1			/* map with ship details on right         */
#define FULLNAMES 2		/* FULL but also including ship names     */
#define FULLEXTRA 3		/* FULLNAMES with add'l ships under map   */
#define NOMAP 4			/* two column text only, no map           */

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

#define NUMUPGRADES 8

/* transponder upgrade settings */
#define TPONNORM 0
#define TPONLOW 1
#define TPONHIGH 2

/***************************************************************************/
/* End of Defines                                                          */
/***************************************************************************/

#ifndef byte
#define byte unsigned char
#endif

/* MPOGEMAL.DAT stores a V10 message header plus fixed GE text payload. */
#define GEMESSAGE struct _gemessage
GEMESSAGE {
	struct message m;		/* V10 message header; Btrieve keys use m.to and m.flags */
	CHAR text[GEMSGSIZ];	/* fixed GE mail text payload             */
};

#define COORD struct _coord

COORD {
	DOUBLE xcoord;		/* x coordinate                            */
	DOUBLE ycoord;		/* y coordinate                            */
};

typedef struct _mine {
	byte channel;		/* owning ship/user slot                   */
	USHORT timer;		/* countdown timer                         */
	COORD coord;		/* mine location                           */
} MINE;

typedef struct _missile {
	byte channel;		/* target ship/user slot                   */
	USHORT distance;	/* distance to target                      */
	USHORT energy;	/* missile energy payload                  */
} MISSILE;

typedef struct _torpedo {
	byte channel;		/* target ship/user slot                   */
	USHORT distance;	/* distance to target                      */
} TORPEDO;

typedef struct warusr {
	CHAR userid[UIDSIZ];		/* userid of player; Btrieve key 0         */
/*30*/
	byte options[30];		/* per-user options                        */
/*60*/
	ULONG score;		/* player score; Btrieve key 1             */
/*64*/
	ULONG cash;		/* cash on hand                            */
/*68*/
	ULONG reserved_debt;	/* reserved V10-compatible slot            */
/*72*/
	ULONG plscore;		/* score from planets only                 */
/*76*/
	ULONG klscore;		/* score from kills only                   */
/*80*/
	ULONG population;	/* total population                        */
/*84*/
	ULONG teamcode;		/* current team code; Btrieve key 2        */
/*88*/
	USHORT noships;		/* number of ships owned currently         */
/*90*/
	USHORT topshipno;		/* highest ship number ever assigned       */
/*92*/
	USHORT kills;			/* number of total kills                   */
/*94*/
	USHORT ukills;		/* just user kills                         */
/*96*/
	USHORT planets;		/* planets owned                           */
/*98*/
	byte factions[8];		/* do NPCs like me                         */
/*106*/
	CHAR filler[150];		/* pad WARUSR to the 256-byte V10 record   */
} WARUSR;

typedef struct warshp {
	CHAR userid[UIDSIZ];		/* userid of player; Btrieve keys 0 and 1  */
/*30*/
	SHORT shipno;			/* ship id number; Btrieve key 1           */
/*32*/
	CHAR shipname[20];		/* ship name                               */
/*52*/
	SHORT shpclass;			/* ship class                              */
/*54*/
	byte status;			/* status of this record (AVAIL/USER/AUTO) */
/*56*/
	COORD coord;			/* ship coordinates                        */
/*72*/
	DOUBLE heading;			/* heading of this ship                    */
/*80*/
	DOUBLE head2b;			/* pending heading                         */
/*88*/
	DOUBLE speed;			/* speed                                   */
/*96*/
	DOUBLE speed2b;			/* pending speed                           */
/*104*/
	SHORT topspeed;			/* top speed w/ damage                     */
/*106*/
	SHORT where;			/* impulse / hyper / orbit                 */
/*112*/
	DOUBLE damage;			/* ship damage                             */
/*120*/
	DOUBLE energy;			/* flux energy available                   */
/*128*/
	DOUBLE phasr;			/* phaser energy                           */
/*136*/
	byte phasrtype;			/* phaser class                            */
/*137*/
	byte shieldtype;		/* shield class                            */
/*138*/
	byte shieldstat;		/* shield status                           */
/*140*/
	SHORT shield;			/* shield charge                           */
/*142*/
	SHORT cloak;			/* cloak status                            */
/*144*/
	SHORT tactical;			/* tactical damage                         */
/*146*/
	SHORT helm;			/* helm damage                             */
/*148*/
	SHORT lastfired;			/* last to fire on this ship               */
/*150*/
	SHORT lock;			/* ship this ship is locked onto           */
/*152*/
	byte track_grace;		/* lock/NPC target invisibility grace      */
/*154*/
	USHORT kills;			/* kills in this ship                      */
/*156*/
	USHORT ukills;		/* just user kills                         */
/*158*/
	TORPEDO ltorps[MAXTORPS];	/* torps tracking this ship                */
/*170*/
	MISSILE lmissl[MAXMISSL];	/* misls tracking this ship                */
/*188*/
	USHORT decout[MAXDECOY];	/* decoys deployed                         */
/*208*/
	byte torps_fired;		/* torpedoes fired this tick               */
/*209*/
	byte missl_fired;		/* missiles fired this tick                */
/*212*/
	ULONG items[NUMITEMS];	/* all the items                           */
/*268*/
	USHORT titem;			/* colony temp item (User-only)            */
/*270*/
	byte upgrade;			/* installed ship upgrades                 */
/*271*/
	byte tponder;			/* transponder control (User-only)         */
/*272*/
	SHORT degrees;			/* degree workspace                        */
/*274*/
	SHORT percent;			/* percent workspace (phaser / engines)    */
/*276*/
	USHORT hostile;		/* just attacked a planet (User-only)      */
/*278*/
	USHORT cantexit;		/* can't exit battle timer                 */
/*280*/
	USHORT repair;		/* repairs underway (User-only)            */
/*282*/
	USHORT hypha;			/* hyper-phaser cooldown                   */
/*284*/
	byte torpcntl;			/* torpedo damage flag                     */
/*285*/
	byte mislcntl;			/* missile damage flag                     */
/*286*/
	byte destruct;			/* self destruct countdown (User-only)     */
/*287*/
	byte minesnear;			/* set when mines are near                 */
/*288*/
	USHORT overspeed;		/* counter for over max warp (User-only)   */
/*290*/
	SHORT zipload;			/* zipper throttle/damage                  */
/*292*/
	SHORT jamload;			/* jam throttle/damage                     */
/*294*/
	SHORT decload;			/* decoy throttle/damage                   */
/*296*/
	SHORT mineload;			/* mine throttle/damage                    */
/*298*/
	byte jam_sev;			/* jamming severity                        */
/*299*/
	byte jam_time;			/* time left in jamming                    */
/*300*/
	USHORT freq;			/* encoded-message frequency (User-only)   */
/*302*/
	USHORT npcstate;		/* auxiliary state (NPC-only)              */
/*304*/
	byte npcmsg;			/* message throttle/state (NPC-only)       */
/*305*/
	byte warpmsg;			/* over-warp warning throttle (User-only)  */
/*306*/
	byte cybmine;			/* target ship (NPC-only)                  */
/*307*/
	byte cybupdate;			/* save/update counter (NPC-only)          */
/*308*/
	byte tick;			/* ticker (NPC-only)                       */
/*309*/
	byte distress;			/* distress target/state (Droid-only)      */
/*310*/
	byte holdcourse;		/* hold-course timer/state (NPC-only)      */
/*311*/
	CHAR filler[197];		/* pad WARSHP to the 512-byte V10 record   */
} WARSHP;

typedef struct item {
	ULONG qty;		/* quantity on hand                        */
	USHORT rate;			/* rate of manufacture                     */
	CHAR sell;			/* sell to allies?                         */
	USHORT reserve;		/* quantity to reserve                     */
	USHORT markup2a;		/* markup charged to allies                */
	ULONG sold2a;		/* quantity sold to allies                 */
} ITEM;

typedef struct _plntcoord {
	SHORT type;			/* object type                             */
	COORD coord;			/* location                                */
} PLNTCOORD;

typedef struct planetab {
	PLNTCOORD planets[MAXPLANETS];	/* visible planet/wormhole table           */
} PLANETAB;

typedef struct galsect {
	SHORT xsect;			/* sector x coord; Btrieve key 0           */
/*2*/
	SHORT ysect;			/* sector y coord; Btrieve key 0           */
/*4*/
	SHORT plnum;			/* always 0; Btrieve key 0                 */
/*6*/
	SHORT type;			/* type of sector; Btrieve key 2           */
/*8*/
	SHORT numplan;			/* number of planetary objects             */
/*16*/
	PLNTCOORD ptab[MAXPLANETS];	/* planet/worm coord table                 */
/*232*/
	CHAR filler[280];		/* pad GALSECT to the 512-byte V10 record  */
} GALSECT;

typedef struct _galplnt {
	SHORT xsect;			/* sector x coord; Btrieve key 0           */
/*2*/
	SHORT ysect;			/* sector y coord; Btrieve key 0           */
/*4*/
	SHORT plnum;			/* planet number; Btrieve key 0            */
/*6*/
	SHORT type;			/* type of sector; Btrieve key 2           */
/*8*/
	COORD coord;			/* planet coordinates                      */
/*24*/
	CHAR userid[UIDSIZ];		/* owning userid; Btrieve key 1            */
/*54*/
	CHAR name[20];			/* planet name                             */
/*74*/
	CHAR enviorn;			/* environment factor                      */
/*75*/
	CHAR resource;			/* resource factor                         */
/*76*/
	ULONG cash;		/* cash on hand                            */
/*80*/
	ULONG timestamp;	/* time of last update                     */
/*84*/
	ULONG tax;		/* amount of tax collected                 */
/*88*/
	SHORT taxrate;			/* rate to tax population                  */
/*90*/
	CHAR password[11];		/* trade/team access password              */
/*104*/
	ITEM items[NUMITEMS];		/* planet inventory and trade settings     */
/*328*/
	CHAR beacon[BEACONMSGSZ];	/* beacon message                          */
/*403*/
	CHAR spyowner[UIDSIZ];		/* userid of current spy owner             */
/*434*/
	USHORT plantimesave;	/* saved planet update tick interval       */
/*436*/
	ULONG teamcode;		/* team lock/access code                   */
/*440*/
	ULONG nebseed;		/* random seed for nebula map (Zygor only) */
/*444*/
	CHAR filler[67];		/* pad GALPLNT to the 512-byte V10 record  */
} GALPLNT;

typedef struct galworm {
	SHORT xsect;			/* sector x coord; Btrieve key 0           */
/*2*/
	SHORT ysect;			/* sector y coord; Btrieve key 0           */
/*4*/
	SHORT plnum;			/* planet number; Btrieve key 0            */
/*6*/
	SHORT type;			/* type of sector; Btrieve key 2           */
/*8*/
	COORD coord;			/* wormhole coordinates                    */
/*24*/
	COORD destination;		/* destination coords                      */
/*40*/
	SHORT visible;			/* is the wormhole visible                 */
/*42*/
	CHAR name[20];			/* wormhole name                           */
/*62*/
	CHAR filler[450];		/* pad GALWORM to the 512-byte V10 record  */
} GALWORM;

typedef struct _pkey {			/* star system data record key             */
	SHORT xsect;			/* sector x coord                          */
	SHORT ysect;			/* sector y coord                          */
	SHORT plnum;			/* always 0                                */
} PKEY;

typedef struct _wormtab {
	COORD coord;			/* wormhole entry location                 */
	COORD dest;			/* wormhole destination                    */
} WORMTAB;

typedef struct _mailkey {
	CHAR userid[UIDSIZ];		/* mail owner userid                       */
	LONG class;			/* mail class                              */
} MAILKEY;

typedef struct _mail {
	CHAR userid[UIDSIZ];		/* GE mail recipient userid                */
/*30*/
	LONG class;			/* GE mail class / stored message flags    */
/*32*/
	SHORT type;			/* GE message template id                  */
/*34*/
	SHORT stamp;			/* GE day stamp                            */
/*36*/
	CHAR dtime[20];			/* display date/time string for mail body  */
/*56*/
	CHAR topic[30];			/* mail subject/topic text                 */
/*86*/
	CHAR string1[80];		/* aux text field (usually attacker userid) */
/*166*/
	CHAR name1[20];			/* primary name field (usually planet name) */
/*186*/
	CHAR filler1[5];		/* reserved for future primary-name growth */
/*191*/
	CHAR name2[20];			/* secondary name field (usually ship name) */
/*211*/
	CHAR filler2[5];		/* reserved for future secondary-name growth */
/*216*/
	SHORT int1;			/* generic integer field 1 (usually sector x) */
/*218*/
	SHORT int2;			/* generic integer field 2 (usually sector y) */
/*220*/
	SHORT int3;			/* generic integer field 3                 */
/*222*/
	LONG long1;			/* generic long field 1 (usually quantity/count) */
/*226*/
	LONG long2;			/* generic long field 2                    */
/*230*/
	LONG long3;			/* generic long field 3                    */
/*234*/
	CHAR filler[253-200-UIDSIZ];	/* pad to 257                              */
} MAIL;

typedef struct _mailstat {
	CHAR userid[UIDSIZ];		/* production-report recipient userid      */
/*30*/
	LONG class;			/* GE mail class / stored message flags    */
/*32*/
	SHORT type;			/* GE message template id                  */
/*34*/
	SHORT stamp;			/* GE day stamp                            */
/*36*/
	CHAR dtime[20];			/* display date/time string for mail body  */
/*56*/
	CHAR topic[30];			/* mail subject/topic text                 */
/*86*/
	CHAR name1[20];			/* planet name                             */
/*106*/
	CHAR filler1[5];		/* reserved for future planet-name growth  */
/*111*/
	SHORT int1;			/* planet sector x                         */
/*113*/
	SHORT int2;			/* planet sector y                         */
/*115*/
	ULONG cash;		/* planet cash on hand                     */
/*119*/
	ULONG timestamp;	/* planet timestamp at report time         */
/*123*/
	ULONG tax;		/* planet tax on hand                      */
/*127*/
	ULONG itemqty[NUMITEMS];	/* per-item quantities at report time */
/*183*/
	CHAR filler[253-97-(UIDSIZ)-(sizeof(LONG)*NUMITEMS)];	/* pad to 253      */
} MAILSTAT;

typedef struct _shpkey {
	CHAR userid[UIDSIZ];		/* owning userid                           */
	SHORT shipno;			/* ship number                             */
} SHPKEY;

typedef struct _ship {
	CHAR *typename;			/* class display name                      */
	CHAR *npcprefx;			/* NPC shipname prefix                     */
	byte max_shlds;			/* max shield class                        */
	byte max_phasr;			/* max phaser class                        */
	byte max_torps;			/* max torpedo tubes                       */
	byte max_missl;			/* max missile tubes                       */
	byte has_decoy;			/* decoy capability                        */
	byte has_jam;			/* jammer capability                       */
	byte has_zip;			/* zipper capability                       */
	byte has_mine;			/* mine capability                         */
	byte max_attk;			/* fighter capability                      */
	byte max_cloak;			/* cloak capability                        */
	USHORT max_accel;		/* max acceleration                        */
	byte max_warp;			/* max warp cruising speed                 */
	LONG max_tons;			/* cargo capacity                          */
	LONG max_price;			/* ship class price                        */
	USHORT max_points;	/* point value if destroyed                */
	USHORT max_type;		/* class type (user/cyborg/droid/none)     */
	LONG scanrange;			/* scanner range                           */
	byte cybs_can_att;		/* cybs of this class attack unprovoked    */
	byte lowest_to_attk;		/* lowest ship class this NPC will pursue  */
	byte noclaim;			/* max cyborg claims / claim pressure      */
	USHORT tot_to_create;	/* target live NPC count for this class    */
	byte tough_factor;		/* NPC toughness/aggression factor         */
	USHORT damfact;		/* damage/price scaling factor             */
	USHORT faction;		/* faction id used for NPC relations       */
	USHORT loadout;		/* upgrade/loadout profile                 */

	SHORT hlpmsg;			/* help message id                         */
	void (*init_func)(WARSHP *, INT, INT);		/* class initialization hook */
	void (*tick_func)(WARSHP *, INT);		/* class per-tick hook       */
	void (*kill_func)(WARSHP *, INT, WARSHP *);	/* class death hook          */
	void (*won_func)(WARSHP *, INT, WARSHP *);	/* class victory hook        */
} SHIP;

struct cmd {
	CHAR *command;			/* command keyword                         */
	void (*func)(void);		/* command handler                         */
	byte cando;			/* 0 = payers only, 1 = all players        */
};

typedef struct _shiptab {
	SHORT shipno;			/* target ship slot                        */
	CHAR letter;			/* scan letter                             */
	CHAR flag;			/* visibility/state flag                   */
	DOUBLE dist;			/* distance                                */
	SHORT bearing;			/* bearing                                 */
	SHORT heading;			/* heading                                 */
	DOUBLE speed;			/* speed                                   */
} SHIPTAB;

#define NOSCANTAB 15		/* how many ships can be tracked by scanners at once */

typedef struct _scantab {
	SHIPTAB ship[NOSCANTAB];
} SCANTAB;

typedef struct _entrytab {
	byte active;			/* entry-message throttle active           */
	byte ticks;			/* ticks since entry state began           */
} ENTRYTAB;

typedef struct _s00 {
	CHAR *name;			/* predefined object name                  */
	CHAR *owner;			/* predefined owner userid                 */
	SHORT type;			/* predefined object type                  */
	DOUBLE xcoord;			/* x coordinate within sector 0,0          */
	DOUBLE ycoord;			/* y coordinate within sector 0,0          */
	SHORT env;			/* environment setting                     */
	SHORT res;			/* resource setting                        */
} S00;

typedef struct _team {
	LONG teamcode;			/* team identifier                         */
	CHAR teamname[31];		/* team name                               */
	USHORT teamcount;		/* current member count                    */
	USHORT teamdeldate;	/* deletion date                           */
	CHAR password[11];		/* join password                           */
	CHAR secret[11];		/* team secret                             */
	SHORT flag;			/* unused                                  */
} TEAM;


#define FUNC EXPORT
