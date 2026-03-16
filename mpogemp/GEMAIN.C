
/***************************************************************************
 *                                                                         *
 *   GEMAIN.C                                                              *
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

/* if FASTPLANET defined updates happen every 3 seconds */

/* #define FASTPLANET 1 */

#ifdef PHARLAP

#include "gcomm.h"
#include "string.h"
#else

#include "stdio.h"
#include "ctype.h"
#include "mem.h"
#include "dos.h"
#include "usracc.h"
#include "btvstf.h"
#include "stdlib.h"
#include "portable.h"
#include "dosface.h"

#endif
#include "math.h"
#include "stdlib.h"
#include "majorbbs.h"

#include "gemain.h"


#define GEMAIN 1

#include "geglobal.h"

/* LOCAL GLOBAL DEFS *****************************************************/


#ifdef PHARLAP

#else

int dfsthn();

#endif

int gestt;


struct module mpoge={		/* module interface block			*/
	"",			/*	description for main menu		*/
	gelogon,		/*	user logon supplemental routine		*/
	galemp,			/*	input routine if selected		*/
	stshdlr,		/*	status-input routine if selected	*/
	NULL,			/*	"injoth" routine for this module	*/
	pwarlof,		/*	user logoff supplemental routine	*/
	pwarhup,		/*	hangup (lost carrier) routine		*/
	pgemidnight,		/*	midnight cleanup routine		*/
	pgedelete,		/*	delete-account routine			*/
	pclswar			/*	finish-up (sys shutdown) routine	*/
};


BTVFILE			*gebb1,	/* GEship.dat */
			*gebb2,	/* GEplanet.dat */
			*gebb4,	/* GEmail.dat */
			*gebb5;	/* GEuser.dat */

FILE *gemb, *gehlpmb, *geshmb;


static	char	*geuser,
		*geship,
		*geplnt,
		*gemail,
		*geshipcl;

static	char	*endmark;

int	numwar=0;		/* number of users in game  */
int	decpass=0;		/* decoy expiry batching pass counter */

WARSHP	tmpshp;			/* used to temporarly set up a new ship */

WARSHP	*warshp,*warsptr;

WARUSR	*warusr,*waruptr;

int	warusr_ecl,warshp_ecl;

WARUSR	tmpusr;

GALSECT	sector,*sptr1;

GALPLNT	planet;

GALWORM	worm;

PLANETAB	*ptab;

char map[MAXY][MAXX+1];		/* global scan map array */
char mapc[MAXY][MAXX+1];	/* global scan map array color map */

MINE		*mines;		/* place to stuff mines */

MAIL		mail;
MAILSTAT	tmpstat;

BEACONTAB	*beacon;

struct message	*gemsg;


/***********************************************************************/
/* Global variables                                                    */
/***********************************************************************/

int				nships;
int				heading;
unsigned			speed;
int				game_day;

/* do not touch the next two definitions !!! */
int				xsect,ysect;
unsigned			xcord,ycord;

PKEY				pkey;
unsigned			distance;
double				ddistance;
int				bearing;
unsigned			energy;
unsigned			damage;
char				*gechrbuf,*gechrbuf2,*gechrbuf3;
char				*warpbuf;
int				plnum;
GALPLNT				*plptr;
ITEM				*titems;
TEAM				*teamtab;

int				su;
int				tmp_usrnum;

long				max_plrec,
				teambonus,
				pltvcash,
				pltvdiv,
				startcash;

unsigned			plantime;

unsigned long			nebseed;

int				gemaxplrs,
				gefreebies,
				gemaxlist,
				maxships,
				se100dam,
				profon,
				showopt,
				syscmds,
				sysonly,
				max_plnts,
				trans_opt,
				numships,
				univmax,
				plodds,
				wormodds,
				nebodds,
				hpfirdst,
				hpdammax,
				pfirdist,
				pdammax,
				jamtime,
				maildays,
				torpsped,
				mislsped,
				decodds,
				nummines,
				usermines,
				cyb_class,
				dr_class,
				clenguse,
				logflag,
				optmenu,
				cyb_gold,
				tot_classes,
				team_max,
				fse_state,
				phatowrp,
				score_bonus,
				score_f1,
				score_f2,
				chgloser,
				univwrap,
				maxplanets,
				meneat,
				showdoc,
				cattkd,
				gcnum,
				planupd,
				passnum;

char				*opttxt,
				optchr;

long				*opttbl;

double				tor_fact,
				tdammax,
				mdammax,
				mis_fact,
				idammax,
				minedammax,
				repairrate,
				tooclose,
				hyperdist1,
				hyperdist2,
				plattrf1,
				plattrf2,
				plattrf3,
				plattrt1,
				plattrt2,
				plattrt3;

SHPKEY				shpkey;
MAILKEY				mailkey;

SHIP				*shipclass;

S00				*s00;
int				s00plnum;

SCANTAB				*scantab;

long				shieldprice[TOPSHIELD];
long				phaserprice[TOPPHASOR];
unsigned			baseprice[NUMITEMS];

/* Maximum items a planet can hold (adjusted later)*/

double maxpl[NUMITEMS];

/* the net weight of each item */

long weight[NUMITEMS];

/* the net score value of each item on the planet */

long value[NUMITEMS];

/* the number of items produced by 1000 men a day */

long manhours[NUMITEMS];

#define MENU struct _menu
MENU {
	int	substt;
	int	(*func)();
	};

#define MENUNUM (sizeof(menu)/sizeof(MENU))

MENU	menu[] = {
	{	0,		mnu_main,},
	{	1,		mnu_main_ans},
	{	FIGHTSUB,	mnu_fightsub},
	{	ADMENU1,	mnu_admenu1},
	{	ADMENU1A,	mnu_admenu1a},
	{	ADMENU2,	mnu_admenu2},
	{	ADMENU2B,	mnu_admenu2b},
	{	ADMENU2E,	mnu_admenu2e},
	{	ADMEN2F1,	mnu_admenu2f1},
	{	ADMEN2F2,	mnu_admenu2f2},
	{	ADMEN2F3,	mnu_admenu2f3},
	{	ADMEN2F4,	mnu_admenu2f4},
	{	ADMENU2H,	mnu_admenu2h},
	{	ADMENU2I,	mnu_admenu2i},
	{	ADMENU2J,	mnu_admenu2j},
	{	CHOOSESH,	mnu_choosesh},
	{	MENUG,		mnu_menug},
	{	MENUG1,		mnu_menug1},
	{	MENUG2,		mnu_menug2}

};


/**************************************************************************
** System start up function                                              **
**************************************************************************/

#ifdef PHARLAP
void EXPORT init__galemp(void)

{
#ifdef GETRAINER
stzcpy(mpoge.descrp,gmdnam("MPOG2.MDF"),MNMSIZ);
#else
stzcpy(mpoge.descrp,gmdnam("MPOGEMP.MDF"),MNMSIZ);
#endif

iniwara();
gestt=register_module(&mpoge);
return;
}
#else
int FUNC iniwar(void)
{
iniwara();
return(0);
}
#endif

void FUNC dummy(void)
{
}

void FUNC iniwara(void)
{
int	i,n,type,classbase;
int	j;

int	class_tab[50];

gemb	= opnmsg(GEMSG);
endmark	= stgopt(ENDMARK);
if (!sameas(endmark,"ENDMARK"))
	{
	catastro("GE:ERR:MPOGEMSG.MCV Corrupted");
	}


geuser		= stgopt(GEUSER);
geship		= stgopt(GESHIP);
geplnt		= stgopt(GEPLNT);
gemail		= stgopt(GEMAIL);
geshipcl	= stgopt(GESHIPCL);

gemaxplrs	= numopt(MAXPLRS,1,256);
gefreebies	= numopt(FREEBIES,0,1);
gemaxlist	= numopt(MAXLIST,3,50);
maxships	= numopt(MAXSHIPS,1,60);
se100dam	= numopt(SE100DAM,1,101);
showopt		= numopt(SHOWOPT,0,5);
trans_opt	= ynopt(TRANSOPT);
syscmds		= ynopt(SYSCMDS);
sysonly		= ynopt(SYSONLY);
max_plnts	= numopt(MAXPLNTS,1,256);
planupd		= numopt(PLANUPD,1,15);
plodds		= numopt(PLODDS,1,20);
wormodds	= numopt(WORMODDS,1,100);
nebodds		= numopt(NEBODDS,0,10);
univmax		= numopt(UNIVMAX,10,32767);
univwrap	= ynopt(UNIVWRAP);
s00plnum	= numopt(S00PLNUM,3,9);
maxplanets	= numopt(MAXPLSE,1,9);
teambonus	= numopt(TEAMBONU,0,32000)*100L;
team_max	= numopt(TEAMMAX,0,32000);
meneat		= ynopt(MENEAT);
showdoc		= numopt(SHOWDOC,0,10);
cattkd		= numopt(CATTKD,0,10);
gcnum		= numopt(GCNUM,0,8);

profon		= ynopt(PROFON);
logflag		= ynopt(LOGFLG);

if (logflag)
geshocst(0,"GE:Ext Trace Logging ON!");
#ifdef FASTPLANET
geshocst(0,"GE:FASTPLANET ON!");
#endif

hpfirdst	= numopt(HPFIRDST,1,20);
hpdammax	= numopt(HPDAMMAX,1,200);
pfirdist	= numopt(PFIRDST,1,20);
pdammax		= numopt(PDAMMAX,1,200);

jamtime		= numopt(JAMTIME,1,10);
maildays	= numopt(MAILDAYS,1,7);
torpsped	= numopt(TORPSPED,1,10000);
mislsped	= numopt(MISLSPED,1,10000);

nummines	= numopt(NUMMINES,1,200);
usermines	= numopt(USRMINES,1,200);

decodds		= numopt(DECODDS,1,20);

tor_fact	= (double)numopt(TORFACT,1,50);
tor_fact	= tor_fact/10.0;
tdammax		= (double)numopt(TDAMMAX,1,100);
mis_fact	= (double)numopt(MISFACT,1,50);
mis_fact	= mis_fact/10.0;
mdammax		= (double)numopt(MDAMMAX,1,100);
idammax		= (double)numopt(IDAMMAX,1,100);
minedammax	= (double)numopt(MNDAMMAX,1,200);
repairrate	= (double)numopt(REPAIRRT,1,50);
repairrate	= repairrate/100.0;

tooclose	= (double)numopt(TOOCLOSE,1,32000);

clenguse	= numopt(CLENGUSE,1,32000);

startcash	= (long)numopt(STRTCASH,1,32000);
startcash	*=1000L;

max_plrec	= (long)numopt(MAXPLREC,10,32767);
max_plrec	*=2;

cyb_gold	= numopt(CYBGOLD,0,32000);

hyperdist1	= (double)numopt(HYPDST1,1,32000);
hyperdist2	= (double)numopt(HYPDST2,1,32000);

plattrf1	= (double)(numopt(PLATTRF1,5,1000));
plattrf1	= plattrf1/100.0;

plattrf2	= (double)(numopt(PLATTRF2,5,1000));
plattrf2	= plattrf2/100.0;


plattrf3	= (double)(numopt(PLATTRF3,5,1000));
plattrf3	= plattrf3/100.0;


plattrt1	= (double)(numopt(PLATTRT1,5,1000));
plattrt1	= plattrt1/100.0;


plattrt2	= (double)(numopt(PLATTRT2,5,1000));
plattrt2	= plattrt2/100.0;



/* load the planet maximum table */
for (i=0;i<NUMITEMS;++i)
	{
	logthis(spr("Item %s",item_name[i]));

	maxpl[i] = (double) (lngopt(ITMPL01+i,0L,201228378L));
	logthis(spr("Itm #%d maxpl=%ld",i,(long)maxpl[i]));

	weight[i] = lngopt(ITMWT01+i,0L,201228378L);
	logthis(spr("Itm #%d weight=%ld",i,weight[i]));

	value[i] = lngopt(ITMVAL01+i,0L,201228378L);
	logthis(spr("Itm #%d value=%ld",i,value[i]));

	manhours[i] = lngopt(ITMMH01+i,0L,201228378L);
	logthis(spr("Itm #%d manhours=%ld",i,manhours[i]));

	baseprice[i] = numopt(ITMPR01+i,1,32000);
	logthis(spr("Itm #%d baseprice=%d",i,baseprice[i]));

	}

/* load the shieldprice table */
for (i=0;i<TOPSHIELD;++i)
	{
	logthis(spr("shieldtype %d",i));

	shieldprice[i] = lngopt(SHLDPR01+i,0L,201228378L);
	logthis(spr("Shld #%d Price=%ld",i,shieldprice[i]));
	}

/* load the phaserprice table */
for (i=0;i<TOPPHASOR;++i)
	{
	logthis(spr("phasertype %d",i));

	phaserprice[i] = lngopt(PHSRPR01+i,0L,201228378L);
	logthis(spr("Phaser #%d Price=%ld",i,phaserprice[i]));
	}


pltvcash = lngopt(PLTVCASH,0L,201228378L);
logthis(spr("pltvcash=%ld",pltvcash));

pltvdiv = lngopt(PLTVDIV,0L,201228378L);

phatowrp = numopt(PHATOWRP,0,100);

score_bonus = numopt(SCRBONUS,0,32700);
score_f2 = numopt(SCRFACT,0,32700);

chgloser = numopt(CHGLOSER,0,100);

optmenu		= ynopt(OPTMENU);
optchr		= chropt(OPTCHR);
optchr		= toupper(optchr);
opttxt		= stgopt(OPTTXT);

opttbl		= (long *)alcmem(n=nterms*sizeof(long));
setmem(opttbl,n,0);

gebb1=opnbtv(geship,sizeof(WARSHP));

gebb4=opnbtv(gemail,sizeof(struct message)+GEMSGSIZ);

gebb2=opnbtv(geplnt,sizeof(GALSECT));

nebseed		= 1L;
pkey.xsect	= 0;
pkey.ysect	= 0;
pkey.plnum	= 1;
if (gesdb(GEGET,&pkey,(GALSECT *)&planet))
	{
	if (planet.type == PLTYPE_PLNT && planet.nebseed != 0L)
		nebseed = planet.nebseed;
	}
else
	{
	nebseed = (((unsigned long)gernd()) << 16) | ((unsigned long)gernd());
	if (nebseed == 0L)
		nebseed = 1L;
	}

game_day = cofdat(today());

cyb_class = 0;
dr_class = 0;

/* load the ship class table */
geshmb = opnmsg(geshipcl);
setmbk(geshmb);

#define NCL 28

/* first audit the table */

/* does the table have all the elements? */
n = (SXXEND - S01TYPE);
i = n/NCL;
if ((i*NCL) != n)
		catastro("GE:ERR:Ship Class Tbl Corrupted");

/* this is how many inactive and active classes we have	*/
/* since we only load active classes we must go figure	*/
/* out how many that really is.				*/

geshocst(1,spr("GE:INF:Fnd %d class slots",i));
tot_classes = i;

n = 0;

for (i=0; i<tot_classes; ++i)
	{
	classbase = S01TYPE+(i*NCL);
	type = tokopt(classbase,"USER","CYBORG","DROID","<NONE>",NULL);

	class_tab[i] = classbase;

	if (type != CLASSTYPE_NONE)
		{
		++n;
		}
	}

geshocst(1,spr("GE:INF:Fnd %d defined classes",n));

/* allocate memory for ship class table*/

shipclass = (SHIP *)alcmem(n=tot_classes*sizeof(SHIP));
setmem(shipclass,n,0);
geshocst(1,spr("GE:INF:Ship Class Mem: %d",n));

/* read in the ship classes */

i = 0;
for (n=0; n<tot_classes; ++n)
	{
	classbase = class_tab[i];
	shipclass[i].max_type = tokopt(classbase,"USER","CYBORG","DROID","<NONE>",NULL);
	shipclass[i].typename = stgopt(++classbase);
	logthis(spr("Loaded class %d - %s",i,shipclass[i].typename));

	shipclass[i].npcprefx = stgopt(++classbase);
	logthis(spr("  NPC prefix %s",shipclass[i].npcprefx));

	shipclass[i].max_shlds = numopt(++classbase,0,19);
	shipclass[i].max_phasr = numopt(++classbase,0,19);
	shipclass[i].max_torps = numopt(++classbase,0,3);
	shipclass[i].max_missl = numopt(++classbase,0,3);
	shipclass[i].has_decoy = ynopt(++classbase);
	shipclass[i].has_jam = ynopt(++classbase);
	shipclass[i].has_zip = ynopt(++classbase);
	shipclass[i].has_mine = ynopt(++classbase);
	shipclass[i].max_attk = ynopt(++classbase);
	shipclass[i].max_cloak = ynopt(++classbase);
	shipclass[i].max_accel = numopt(++classbase,0,32767);
	shipclass[i].max_warp = numopt(++classbase,0,255);
	shipclass[i].max_tons = lngopt(++classbase,1,2000000000L);
	shipclass[i].max_price = lngopt(++classbase,1,2000000000L);
	shipclass[i].max_points = numopt(++classbase,1,32767);
	shipclass[i].scanrange = lngopt(++classbase,1,9999999L);
	shipclass[i].cybs_can_att = ynopt(++classbase);
	shipclass[i].noclaim = numopt(++classbase,0,255);
	shipclass[i].lowest_to_attk = numopt(++classbase,0,255);
	shipclass[i].tot_to_create = numopt(++classbase,0,255);
	shipclass[i].tough_factor = numopt(++classbase,0,4);
	shipclass[i].damfact = numopt(++classbase,0,32767);
	shipclass[i].faction = numopt(++classbase,0,32767);
	shipclass[i].loadout = numopt(++classbase,0,32767);

	shipclass[i].hlpmsg = ++classbase;

	shipclass[i].init_func	= NULL;
	shipclass[i].tick_func	= NULL;
	shipclass[i].kill_func	= NULL;
	shipclass[i].won_func	= NULL;

	if (shipclass[i].max_type == CLASSTYPE_CYBORG ||
		shipclass[i].max_type == CLASSTYPE_DROID)
		{
		numships += shipclass[i].tot_to_create;
		}

	if (shipclass[i].max_type == CLASSTYPE_CYBORG) /* CYBORG */
		{
		if (cyb_class == 0)	/* set up base cybertron class */
			cyb_class = i;
		shipclass[i].init_func	= cyb_init;
		shipclass[i].tick_func	= cyb_lives;
		shipclass[i].kill_func	= cyb_died;
		shipclass[i].won_func	= cyb_won;
		}
	else
	if (shipclass[i].max_type == CLASSTYPE_DROID) /* DROID */
		{
		if (dr_class == 0)	/* set up base droid class */
			dr_class = i;
		shipclass[i].init_func	= droid_init;
		shipclass[i].tick_func	= droid_lives;
		shipclass[i].kill_func	= droid_died;
		shipclass[i].won_func	= droid_won;
		}
	geshocst(1,spr("GE:INF:Init Class %s",shipclass[i].typename));

	++i; /* index the next table entry */
	}

gebb5=opnbtv(geuser,sizeof(WARUSR));

gehlpmb =opnmsg(GEHELP);

/* ships in game is at least number of terminal channels */
nships = nterms + numships;
if (nships > 125)
	{
	geshocst(0,spr("GE:ERR:Too many ships defined"));
	nships = 125;
	}

/* allocate memory for user data table */

warusr_ecl=pltile(nships*(long)sizeof(WARUSR),0,sizeof(WARUSR),sizeof(WARUSR));
warusr=MK_FP(warusr_ecl,0);

for (j=0 ; j < nships ; j++)
	{
	setmem((void *)warusroff(j),sizeof(WARUSR),0);
	}
geshocst(1,spr("GE:INF:User Mem: %ld",nships*sizeof(WARUSR)));

/* allocate memory for ship data table */
warshp_ecl=pltile(nships*(long)sizeof(WARSHP),0,sizeof(WARSHP),sizeof(WARSHP));
warshp=MK_FP(warshp_ecl,0);

for (j=0 ; j < nships ; j++)
	{
	setmem(((void *)warshpoff(j)),sizeof(WARSHP),0);
	warshpoff(j)->status = GESTAT_AVAIL;
	}
geshocst(1,spr("GE:INF:Ship Mem: %ld",nships*sizeof(WARSHP)));

/* allocate memory for planet table */
ptab=(PLANETAB	*)alcmem(n=(nships*sizeof(PLANETAB))+1);
setmem(ptab,n,0);
geshocst(1,spr("GE:INF:Planet Table Mem: %d",n));

/* allocate memory for a temporary item table */
titems=(ITEM  *)alcmem(n=nships*sizeof(ITEM));
setmem(titems,n,0);
geshocst(1,spr("GE:INF:Temp Items Mem: %d",n));

/* allocate memory for a team table */
teamtab=(TEAM  *)alcmem(n=MAXTEAMS*sizeof(TEAM));
geshocst(1,spr("GE:INF:Team Tab Mem: %d",n));

/* allocate memory for scan table */
scantab=(SCANTAB *)alcmem(n=nships*sizeof(SCANTAB));
setmem(scantab,n,0);
geshocst(1,spr("GE:INF:Scantab Mem: %d",n));

/* allocate memory for S00 table */
s00=(S00 *)alcmem(n=s00plnum*sizeof(S00));
setmem(s00,n,0);
geshocst(1,spr("GE:INF:S00 Mem: %d",n));

/* allocate memory for beacon table */
beacon=(BEACONTAB *)alcmem(n=nships*sizeof(BEACONTAB));
setmem(beacon,n,0);
geshocst(1,spr("GE:INF:Beacontab Mem: %d",n));

/* allocate memory for the mail message table*/
gemsg=(struct message *)alcmem(sizeof(struct message)+GEMSGSIZ);

/* allocate memory for mine table */
mines=(MINE *)alcmem(n=nummines*sizeof(MINE));
setmem(mines,n,0);
geshocst(1,spr("GE:INF:Mines Mem: %d",n));

/* allocate memory for garbage bucket */

gechrbuf = (char *)alcmem(255);
gechrbuf2 = (char *)alcmem(20);
gechrbuf3 = (char *)alcmem(20);
warpbuf = (char *)alcmem(40);


/* init empty mine field */

for (n=0; n < nummines; ++n)
	mines[n].channel = 255;

/* init sector, planet, and worm table to bad values */
sector.xsect = 32767;
sector.ysect = 32767;
sector.plnum = 32767;

planet.xsect = 32767;
planet.ysect = 32767;
planet.plnum = 32767;

worm.xsect = 32767;
worm.ysect = 32767;
worm.plnum = 32767;

setmbk(gemb);

/* number of parameters per planet entry */
#define NPL 8

/* read in the neutral planets */

for (i=0; i<s00plnum; ++i)
	{
	if ((S00P1RES+(i*NPL))-(S00P1DEF+(i*NPL)) != (NPL-1))
		catastro(spr("GE:ERR:Sect00 Table Error %d Msg # %d",i+1,S00P1DEF+(i*NPL)));
	classbase = S00P1DEF+(i*NPL);
	n					= ynopt(classbase);
	if (!n)
		catastro(spr("GE:ERR:Sect00 Table Error %d Msg # %d",i+1,S00P1DEF+(i*NPL)));

	s00[i].name	= stgopt(++classbase);
	s00[i].owner	= stgopt(++classbase);
	s00[i].type	= numopt(++classbase,0,3);

	s00[i].xcoord	= (double)(numopt(++classbase,100,9900));
	s00[i].xcoord	= s00[i].xcoord/10000.0;
	s00[i].ycoord	= (double)(numopt(++classbase,100,9900));
	s00[i].ycoord	= s00[i].ycoord/10000.0;

	s00[i].env	= numopt(++classbase,0,3);
	s00[i].res	= numopt(++classbase,0,3);

	geshocst(1,spr("GE:INF:I/S00 %d %s",s00[i].type,s00[i].name));
	}

/* Load the team table from disk */
load_team_tab();

/* tell everyone that we are up */
geshocst(0,spr("Galactic Empire %s",VERSION));
geshocst(0,spr("Registration # %s",stgopt(REGNO)));

#ifdef PHARLAP
rtkick(TICKTIME,pwarrti);
rtkick(TICKTIME2,pwarrti2);
rtkick(60,pwarrti3);
rtkick(30,pplarti);
rtkick(1,pautorti);

#else

rtkick(TICKTIME,warrti);
rtkick(TICKTIME2,warrti2);
rtkick(60,warrti3);
rtkick(10,plarti);
rtkick(1,autorti);

#endif



/* find the module number (state) of the FSE for later use */
fse_state=-1;
for(i=0;i<nmods;i++)
	{
	if((sameas((char *)(module[i]->descrp),"Editor")) == TRUE)
		fse_state=i;
	}
}

/**************************************************************************
** User loged in routine                                                 **
**************************************************************************/


int FUNC gelogon(void)
{

int i;
int other_ge_present = FALSE;

for (i = 0; i < nmods; ++i)
	{
	if (module[i] == &mpoge)	/* we see ourself */
		continue;

	if (sameas((char *)module[i]->descrp, "Galactic Empire"))	/* hello there */
		{
		other_ge_present = TRUE;
		break;
		}
	}

setmbk(gemb);

if (!hasmkey(PLAYKEY) || other_ge_present == TRUE)
	return(0);

 /* BJ CHANGED FROM USRACC */
if (!geudb(GELOOKUP,usaptr->userid, warusroff(usrnum)))
	{
	prfmsg(GECALLS);
	outprf(usrnum);
	}
else
if (gernd()%10 == 1)
	{
	prfmsg(GECALLS2);
	outprf(usrnum);
	}
return(0);
}

/**************************************************************************
** User deleted routine                                                  **
**************************************************************************/

#ifdef PHARLAP
void FUNC pgedelete(uid)
char	*uid;
{
gedeletea(uid);
return;
}
#else
int gedelete(uid)
char	*uid;
{
gedeletea(uid);
return(0);
}
#endif

void FUNC gedeletea(uid)
char	*uid;
{

if (geudb(GELOOKUP,uid, &tmpusr))
	{
	geudb(GEGET,uid, &tmpusr);
	while (gepdb(GELOOKUPNAME,uid,0,&tmpshp))
		{
		gcrbtv(&tmpshp,0);
		gepdb(GEDELETE,tmpshp.userid,tmpshp.shipno,&tmpshp);
		logthis(spr("GE:Deleted %s ship %d",tmpshp.userid,tmpshp.shipno));
		}
	geudb(GEDELETE,tmpusr.userid,&tmpusr);
	logthis(spr("GE:Deleted %s user",tmpusr.userid));
	return;
	}
geshocst(1,spr("GE:User %s not in DB",uid));
return;
}


/**************************************************************************
** Midnight cleanup routine                                              **
**************************************************************************/

#ifdef PHARLAP
void FUNC pgemidnight(void)
{
gemidnighta();
return;
}
#else
int FUNC gemidnight(void)
{
gemidnighta();
return(0);
}
#endif

void FUNC gemidnighta(void)
{
int	i;
int	foundit;
int	intkey = PLTYPE_PLNT;
long	scr;

setmbk(gemb);

geshocst(0,spr("GE:INF:Begin Cleanup"));

/* clear out planet counter */
geshocst(1,spr("GE:INF:Cleanup Phase-1"));
setbtv(gebb5);
if (qlobtv(0))
	{
	do
		{
		gcrbtv(&tmpusr,0);
		tmpusr.planets = 0;
		tmpusr.score = 0;
		tmpusr.plscore = 0;
		tmpusr.population = 0;
		for (i=0; i<8; ++i)
			if (tmpusr.factions[i] > 40)
				tmpusr.factions[i] = tmpusr.factions[i]-40;
			else
				tmpusr.factions[i] = 0;
		updbtv(&tmpusr);
		gcrbtv(&tmpusr,0);	/* thank you BTRIEVE 5.00b */
		} while (qnxbtv());
	}

geshocst(1,spr("GE:INF:Cleanup Phase-2"));

setbtv(gebb2);

/* if we didn't finish one full planet pass, recalc plantime tomorrow */
if (passnum <= 1)
	{
	geshocst(1,spr("GE:INF:first planet cycle didn't complete, recalc tomorrow"));
	if (agebtv(&planet, &intkey, 2))
		{
		planet.plantimesave = 0;
		gesdb(GEUPDATE, (PKEY *)&planet, (GALSECT *)&planet);
		}
	}

if (qlobtv(0))
	{
	do
		{
		gcrbtv(&planet,0);
		setbtv(gebb5);
		if (planet.type == PLTYPE_PLNT)
			{
			if (planet.userid[0] != 0)
				{
				if (qeqbtv(planet.userid,0))
					{
					gcrbtv(&tmpusr,0);
					plptr = &planet;
					calc_networth();
					++tmpusr.planets;
					tmpusr.population += (plptr->items[I_MEN].qty/10000L);
					updbtv(&tmpusr);
					gcrbtv(&tmpusr,0);

					/* now go create the Status Record */

					strncpy(tmpstat.userid,tmpusr.userid,UIDSIZ);
					tmpstat.class = MAIL_CLASS_PRODRPT;
					tmpstat.type = MESG20;
					tmpstat.stamp = cofdat(today());
					sprintf(tmpstat.dtime,"%s - %.5s",ncedat(today()),nctime(now()));
					strcpy(tmpstat.name1,planet.name);
					tmpstat.int1 = planet.xsect;
					tmpstat.int2 = planet.ysect;
					tmpstat.cash = planet.cash;
					tmpstat.timestamp = planet.timestamp;
					tmpstat.tax = planet.tax;
					for (i=0;i<NUMITEMS;++i)
						tmpstat.itemqty[i] = planet.items[i].qty;

/*HERE*/

					memcpy(&mail,&tmpstat,sizeof(MAILSTAT));
					mailit(0);
					}
				}
			}
		setbtv(gebb2);
		} while (qnxbtv());
	}

sprintf(gechrbuf,"%ld",(cntrbtv()/2L));
geshocst(0,spr("GE:INF:Plnt DB Size %sk",gechrbuf));

if (cntrbtv() >= max_plrec)
	geshocst(0,"GE:INF:Max Sect Reached");

geshocst(1,spr("GE:INF:Cleanup Phase-3"));

/* purge mail older than 7 days */

setbtv(gebb4);

i = cofdat(today());
i -= maildays; /* back up 1 week */
if (qlobtv(0))
	{
	do
		{
		gcrbtv(gemsg,0);
		if (gemsg->nreply < i) /* we robbed nreply for the stamp */
			delbtv();
		if (gemsg->userto[0] == '*') /* non-live player */
			delbtv();

		} while (qnxbtv());
	}

geshocst(1,spr("GE:INF:Cleanup Phase-4"));
setbtv(gebb5);

/* zero out the team count and score */
for (i=0;i<MAXTEAMS;++i)
	{
	teamtab[i].teamcount = 0;
	teamtab[i].teamscore = 0;
	}

/* first count up the members of a team */

if (qlobtv(0))
	{
	do
		{
		gcrbtv(&tmpusr,0);
		foundit = FALSE;
		if (tmpusr.teamcode > 0)
			{
			for (i=0;i<MAXTEAMS;++i)
				{
				if (teamtab[i].teamcode == tmpusr.teamcode)
					{
					foundit=TRUE;
					break;
					}
				}
			if (foundit == FALSE)
				{
				tmpusr.teamcode = 0;
				logthis(spr("Reset Teamcode to 0 [%s]",tmpusr.userid));
				}
			else
				{
				++teamtab[i].teamcount;
				sprintf(gechrbuf,"++Teamcnt %ld %s",tmpusr.teamcode,tmpusr.userid);
				logthis(gechrbuf);
				}
			}
		if (foundit == FALSE)
			updbtv(&tmpusr);
		gcrbtv(&tmpusr,0);
		} while (qnxbtv());
	}

/* update player and team scores */

if (qlobtv(0))
	{
	do
		{
		gcrbtv(&tmpusr,0);
		tmpusr.score = tmpusr.plscore + tmpusr.klscore;

		if (tmpusr.teamcode > 0)
			{
			for (i=0;i<MAXTEAMS;++i)
				{
				if (teamtab[i].teamcode == tmpusr.teamcode)
					{
					break;
					}
				}

			/* just incase things get messed up */
			if (teamtab[i].teamcount == 0)
			teamtab[i].teamcount = 1;

			teamtab[i].teamscore += teambonus;

			sprintf(gechrbuf,"++Teamscr=%ld+(%ld/%d) %s",
				teamtab[i].teamscore,
				tmpusr.score,
				teamtab[i].teamcount,
				tmpusr.userid);
			logthis(gechrbuf);

			scr = tmpusr.score/(long)teamtab[i].teamcount;
			teamtab[i].teamscore += scr;
			}
		updbtv(&tmpusr);
		gcrbtv(&tmpusr,0);
		} while (qnxbtv());
	}

/* remove any teams with no players */

for (i=0;i<MAXTEAMS;++i)
	{
	if (teamtab[i].teamcode > 0 && teamtab[i].teamcount == 0)
		{
		teamtab[i].teamcode = -1;
		geshocst(0,spr("GE:INF:Removed Team %s",teamtab[i].teamname));
		}
	}
/* update the team scores on disk */

update_team_tab();

geshocst(0,spr("GE:INF:End Cleanup"));
return;
}


/* determine the net worth of a planet */

void FUNC calc_networth()
{
unsigned long	v;
v = value_pl();
tmpusr.plscore += v;
}


unsigned FUNC long value_pl()
{
unsigned long	v;
int		i;

v = (plptr->cash+plptr->tax)/(1000000L/pltvcash);

for (i=0;i<NUMITEMS;++i)
	{
	v += (value[i] * ((long)plptr->items[i].qty/pltvdiv));
	}

return (v);
}

/**************************************************************************
** User logged off                                                       **
**************************************************************************/

int FUNC pwarlof(void)
{
warsptr=warshpoff(usrnum);
waruptr=warusroff(usrnum);

logthis(spr("WARLOF called 4 %s",waruptr->userid));
return(0);
}




/**************************************************************************
** User hungup routine                                                   **
**************************************************************************/
#ifdef PHARLAP
void FUNC pwarhup(void)
{
warhupa();
return;
}

#else
int FUNC warhup(void)
{
warhupa();
return(0);
}
#endif

void FUNC warhupa(void)
{
setbtv(gebb1);
setmbk(gemb);

warsptr=warshpoff(usrnum);
waruptr=warusroff(usrnum);

logthis(spr("WARHUP called 4 %s",waruptr->userid));

if (warsptr->status == GESTAT_USER)
	{
	if (ingegame(usrnum))
		{
		/* if modem hangup */
		logthis(spr("User Hungup Status = %d",status));
		if (warsptr->cantexit > 0)
#ifndef MBBSEMU
			{
			/* if we're in cleanup mode, don't killem */
			if (status == RING && rsmodes[usrnum] != NORMRS)
				{
				cleartm(usrnum);
				clearitm(usrnum);
				gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
				geudb(GEUPDATE,waruptr->userid,waruptr);
				}
			else
#endif
				{
				killem(warsptr,usrnum);
				warsptr->where = -1;
				}
#ifndef MBBSEMU
			}
#endif
		else
			{
			prfmsg(PEACEOUT,waruptr->userid,warsptr->shipname);
			outwar(ALWAYS,usrnum,0);
			prfmsg(EXIWAR2,warsptr->shipname);
			outsect(ALWAYS,&warsptr->coord,usrnum,0);
			cleartm(usrnum);
			gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);

			geudb(GEUPDATE,waruptr->userid,waruptr);
			}
		--numwar;
		}
	}

warsptr->status = GESTAT_AVAIL;
return;
}

/**************************************************************************
** System shutdown message                                               **
**************************************************************************/

#ifdef PHARLAP
void FUNC pclswar(void)
{
clswara();
return;
}
#else
int FUNC clswar(void)
{
clswara();
return(0);
}
#endif

void FUNC clswara(void)
{
if (gemb != NULL)
	{
	clsmsg(gemb);
	gemb=NULL;
	}

clsbtv(gebb1);
clsbtv(gebb4);
clsbtv(gebb2);
clsbtv(gebb5);

logthis("***GALACTIC EMPIRE SHUTDOWN***");
return;
}

/**************************************************************************
** Main input loop                                                       **
**************************************************************************/

int FUNC galemp()
{
int	i,rtn;

setbtv(gebb1);
setmbk(gemb);
warsptr=warshpoff(usrnum);
waruptr=warusroff(usrnum);

for (i=0;i<MENUNUM;++i)
	{
	if (menu[i].substt == usrptr->substt)
		{
		rtn = (*(menu[i].func))();
		clrprf();
		return(rtn);
		}
	}
return(1);
}

/**************************************************************************
** Send message to all ships                                             **
**************************************************************************/

void FUNC outwar(int filter,unsigned exclude,unsigned freq)
{
int	i;

int	zothusn;
for (zothusn=0; zothusn < nships; zothusn++)
	{
	if (zothusn != exclude && ingegame(zothusn))
		{
		if (freq == 0)
			{
			outprfge(filter,zothusn);
			}
		else
			{
			for (i=0; i<3; ++i)
				{
				if (freq == warshpoff(zothusn)->freq[i])
					{
					outprfge(filter,zothusn);
					}
				}
			}
		}
	}
clrprf();
}

/**************************************************************************
** Player/ship Database functions                                        **
**************************************************************************/

int FUNC gepdb(func, usrname, shipnum, geptr)
int	func;
char	*usrname;
int	shipnum;
WARSHP	*geptr;
{
int	rtn;

setbtv(gebb1);
rtn = 0;

strncpy(shpkey.userid,usrname,UIDSIZ);
shpkey.shipno = shipnum;
logthis(spr("GEPDB called: F=%d,%s,%d,%s",func,usrname,shipnum,geptr->userid));
switch (func)
	{

	case	GELOOKUP	:
		if (qeqbtv(&shpkey,1))
			rtn = 1;
		break;

	case	GEADD		:
#ifdef PHARLAP
		if (!dinsbtv(geptr))
#else
		if (!insbtv(geptr))
#endif
			geshocst(0,spr("GE:ERR:Ship ins Fail %s",usrname));

		break;

	case	GEDELETE	:
		if (acqbtv(NULL,&shpkey,1))
			{
			delbtv();
			rtn = 1;
			}
		else
			{
			geshocst(0,spr("GE:ERR:Ship Del Fail %s",usrname));
			}
		break;

	case	GEUPDATE	:

		if (acqbtv(NULL,&shpkey,1))
			{
			updbtv(geptr);
			rtn = 1;
			}
		else
			{
			geshocst(0,spr("GE:ERR:Ship Upd Fail %s",usrname));
			}
		break;

	case	GEGET		:
		if (acqbtv(geptr,&shpkey,1))
			rtn = 1;
		break;

	case	GENEXT		:
		if (qnxbtv())
			rtn = 1;
		break;

	case	GELOOKUPNAME	:
		if (qeqbtv(usrname,0))
			rtn = 1;
		break;

	default:
		rtn = 0;
	}
return(rtn);
}

/**************************************************************************
** User Database functions                                               **
**************************************************************************/

int FUNC geudb(int func, char *usrname, WARUSR *geptr)
{
int	rtn;

setbtv(gebb5);
rtn = 0;

logthis(spr("GEUDB called: F=%d,%s,%s",func,usrname,geptr->userid));
switch (func)
	{

	case	GELOOKUP	:
		if (qeqbtv(usrname,0))
			rtn = 1;
		logthis(spr("GE: lookup *%s* f:%d",usrname,rtn));
		break;

	case	GEADD		:

#ifdef PHARLAP
		if (!dinsbtv(geptr))
#else
		if (!insbtv(geptr))
#endif
			geshocst(0,spr("GE:ERR:User ins Fail %s",usrname));

		break;

	case	GEDELETE	:
		if (acqbtv(NULL,usrname,0))
			{
			delbtv();
			rtn = 1;
			}
		else
			{
			geshocst(0,spr("GE:ERR:User Del Fail %s",usrname));
			}
		break;

	case	GEUPDATE	:
		logthis(spr("DEBUG <%s> <%s> update",usrname,geptr->userid));
		if (acqbtv(NULL,usrname,0))
			{
			updbtv(geptr);
			rtn = 1;
			}
		else
			{
			geshocst(0,spr("GE:ERR:User Upd Fail %s",usrname));
			}
		break;

	case	GEGET		:
		if (acqbtv(geptr,usrname,0))
			{
			rtn = 1;
			}
		else
			{
			geshocst(0,spr("GE:ERR:User Get Fail %s",usrname));
			}
		break;

	default:
		rtn = 0;
	}
return(rtn);
}


/**************************************************************************
** sector Database functions                                             **
**************************************************************************/

int FUNC gesdb(func, sect, geptr)
int	func;
PKEY	*sect;
GALSECT	*geptr;
{
int	rtn;

logthis(spr("Func GESDB, func = %d, sect*= %ld,geptr*=%ld",func,(long)sect,(long)geptr));
logthis(spr("            xsect %d, ysect %d, plnum %d",sect->xsect,sect->ysect,sect->plnum));

setbtv(gebb2);
rtn = 0;

switch (func)
	{

	case	GELOOKUP	:
		if (!qeqbtv(sect,0))
			rtn = 1;
		break;

	case	GEUPDATE	:
		if (acqbtv(NULL,sect,0))
			{
			updbtv(geptr);
			rtn = 1;
			}
		else
			{
			geshocst(0,spr("GE:ERR:Plt Upd Fail x%d,y%d,p%d",sect->xsect,
				sect->ysect,sect->plnum));
			}
		break;

	case	GEADD		:
		logthis(spr("GE:DBG:Ins Sect %d %d %d",geptr->xsect,geptr->ysect,geptr->plnum));

#ifdef PHARLAP
		if (!dinsbtv(geptr))
#else
		if (!insbtv(geptr))
#endif
			geshocst(0,"GE:ERR:Sect/plt ins Fail");
		else
			logthis("GE:DBG:Ins Sect suceeded");
		break;

	case	GEGET		:
		if  ((geptr->xsect != sect->xsect)
				||(geptr->ysect != sect->ysect)
				||(geptr->plnum != sect->plnum))
			{
			if (acqbtv(geptr,sect,0))
				{
				logthis("gesdb GEGET acqbtv found record");
				rtn = 1;
				}
			}
		else
			{
			logthis("gesdb GEGET record already in memory");
			rtn = 1;
			}
		break;

	case	GEGETNOW	:
		if (acqbtv(geptr,sect,0))
			rtn = 1;
		break;

	default:
		rtn = 0;
	}
return(rtn);
}

int FUNC getplanetdat(usrn)          /* plnum MUST be set before this is called */
int	usrn;
{
int	i,bbad;

if (plnum > 0 && plnum <= MAXPLANETS)
	{
	getsector(&(warshpoff(usrn)->coord));

/* DEBUG Is numplan set right or is it 9*/
	if (plnum <= sector.numplan)
		{
		logthis (spr("Getsectdat:plnum = %d, numplan = %d",plnum,sector.numplan));
		getplanet(&(warshpoff(usrn)->coord),plnum);
/* If this fails what happens */
		plptr = &planet;
		if (plptr->type == PLTYPE_WORM)
			{
			memcpy(&worm,&planet,sizeof(GALWORM));	/* make it the current user */
			}
		else
			{
			if (plptr->beacon[0] != 0)
				{
				bbad = FALSE;

				for (i=0;i<BEACONMSGSZ;++i)
					{
					if (plptr->beacon[i] < ' ' || plptr->beacon[i] > '~')
						{
						plptr->beacon[0] = 0;
						bbad = TRUE;
						break;
						}
					}
				if (!bbad)
					{
					movecoord(&beacon[usrn].coord,&plptr->coord);
					beacon[usrn].plnum = plnum;
					strncpy(beacon[usrn].beacon,plptr->beacon,BEACONMSGSZ);
					}
				}
			}
		}
	else
		{
		return(FALSE);
		}
	}
return(TRUE);
}


/**************************************************************************
** Team Table Database functions                                         **
**************************************************************************/

void FUNC load_team_tab()

{

char	buffer[256];
FILE	*mzfp;
int	i;

/* clear out the memory team table */

for (i=0;i<MAXTEAMS;++i)
	{
	teamtab[i].teamcode = 0;
	teamtab[i].teamname[0] = 0;
	teamtab[i].teamcount = 0;
	teamtab[i].teamscore = 0;
	teamtab[i].password[0] = 0;
	teamtab[i].secret[0] = 0;
	}

logthis("Loading Team Table");

if ((mzfp=fopen("MPOGETEA.DAT","r")) != NULL)
	{
	i = 0;
	while(fgets(buffer,sizeof(buffer),mzfp) != NULL)
		{
		if (sameto("TEAM|",buffer)&& buffer[80] == '|')
			{
/*			logthis(spr("Loading TT Elmt # %d",i));*/
/*			logthis(spr("Buffer = [%s]",buffer));*/
			strncpy(gechrbuf,&buffer[5],5);
			gechrbuf[5]=0;
			teamtab[i].teamcode = atol(gechrbuf);
			logthis(spr(" Team Code [%s]",gechrbuf));

			strncpy(teamtab[i].teamname,&buffer[11],30);
			stripb(teamtab[i].teamname);
			logthis(spr(" Team Name [%s]",teamtab[i].teamname));

			strncpy(gechrbuf,&buffer[42],5);
			gechrbuf[5]=0;
			teamtab[i].teamcount = atoi(gechrbuf);
			logthis(spr(" Team Cnt [%s]",gechrbuf));

			strncpy(gechrbuf,&buffer[48],10);
			gechrbuf[10]=0;
			teamtab[i].teamscore = atol(gechrbuf);
			logthis(spr(" Team Score [%s]",gechrbuf));

			strncpy(teamtab[i].password,&buffer[59],10);
			stripb(teamtab[i].password);
			strncpy(teamtab[i].secret,&buffer[70],10);
			stripb(teamtab[i].secret);

			i++;
			if (i >= MAXTEAMS)
				break;
			}
		else
			{
			geshocst(0,"GE:ERR Bad Team Rcd - Ignored");
			}
		}
	fclose(mzfp);
	}
}

void FUNC update_team_tab()

{

FILE	*hdl;
int	i;

hdl = fopen("mpogetea.dat","wt");

if(hdl != (FILE *)0)
	{
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode != -1)
			{
			fprintf(hdl,"TEAM|%5ld|%-30s|%5d|%10ld|%-10s|%-10s|\n",
				teamtab[i].teamcode,
				teamtab[i].teamname,
				teamtab[i].teamcount,
				teamtab[i].teamscore,
				teamtab[i].password,
				teamtab[i].secret);
			}
		}
	fclose(hdl);
	}

return;
}
/**************************************************************************
** Planet economic processing                                            **
**************************************************************************/
#ifdef PHARLAP
void FUNC pplarti(void)
{
plartia();
return;
}
#else
int FUNC plarti(void)
{
plartia();
return(0);
}
#endif

void FUNC plartia(void)
{
static int foundpl = TRUE;
static long fpos = 0;
static long tocks = 0;
double ftocktime,ftockfact;
int i, tic, multnum;
int intkey = PLTYPE_PLNT;
static unsigned int plown = 0;
static unsigned int plnob = 0;
static unsigned int plemt = 0;
long numrecs;

#define SECSADAY 82800L		/* 23 hours, for breathing room */
#define MAXTIC 25		/* max planets to scan in a go without finding one to update */

setbtv(gebb2);

numrecs=cntrbtv();	/* how many total records? sectors, planets, wormholes */

sprintf(gechrbuf,"%ld",numrecs);
logthis(spr("plartia entered, numrecs %s, passnum %d, plantime %u",gechrbuf,passnum,plantime));

++tocks;	/* how many times routine has run this passnum */

if (passnum > planupd)	/* we've run all the day's updates */
	{
	agebtv(&planet, &intkey, 2);
	planet.plantimesave = plantime;
	planet.timestamp = ((unsigned long)game_day << 4) | passnum;
	gesdb(GEUPDATE, (PKEY *)&planet, (GALSECT *)&planet);
	geshocst(1,spr("GE:INF:all planets updated for day %d",game_day));
	return;
	}

if (passnum == 0)	/* fresh boot */
	{
	if (!agebtv(&planet, &intkey, 2))
		{
		plantime = 10;
		logthis(spr("no planet records, wait %u seconds",plantime));
		}
	else
		{
		if (planet.xsect == 0 && planet.ysect == 0 && planet.plnum == 1)
			{
			logthis(spr("Checking Zygor timestamp, game day %d",game_day));

#ifdef FASTPLANET
			plantime = 3;
#else
			/* if no plantime saved, how fast do we want to start out? */
			if (planet.plantimesave <= 3)
				{
				if (numrecs < 50)
					numrecs = 50;
				else
				if (numrecs > 5000)
					numrecs = 5000;
				plantime = (unsigned int)((3997L * (5000L - numrecs)) / 4950L + 3);
				plantime /= planupd;
				logthis(spr("estimated plantime set to %u",plantime));
				}
			else
				plantime = planet.plantimesave;
#endif
			if (plantime < 3)
				plantime = 3;

			if ((planet.timestamp >> 4) == game_day)	/* game has already been up today */
				{
				passnum = (int)(planet.timestamp & 0xF);
				logthis(spr("resuming from planupd %d",planupd));
				}
			else
				passnum = 1;	/* different day than last time */
			}
		else
			catastro("GE:ERR:First planet record is not Zygor");
		}
	}
else
	{
	if (foundpl == TRUE)
		{
		if (fpos == 0)
			{
			agebtv(&planet, &intkey, 2);	/* acquire first planet record */
			fpos = absbtv();		/* save position for next time through */
			}
		else
			gabbtv(&planet, fpos, 2);	/* get the planet we found last time */

		sprintf(gechrbuf,"%lu",fpos);

		if (planet.xsect == 0 && planet.ysect == 0 && planet.plnum == 1)
			{
			logthis(spr("updating Zygor (%s)",gechrbuf));
			plptr = &planet;
			update_plan_1();
			planet.plantimesave = plantime;
			}
		else
		if (planet.xsect == 0 && planet.ysect == 0 && planet.plnum == 2)
			{
			logthis(spr("updating T-Station (%s)",gechrbuf));
			plptr = &planet;
			update_plan_2();
			}
		else
		if (planet.xsect == 0 && planet.ysect == 0 && planet.plnum == 3)
			{
			logthis(spr("updating Enforcer Planet (%s)",gechrbuf));
			plptr = &planet;
			update_plan_3();
			}
		else
			{
			logthis(spr("updating Planet %s (%s, %s)...",gechrbuf,planet.name,planet.userid));
			/* updated in the last 7 days? catch up */
			if ((planet.timestamp >> 4) >= game_day - 7 && (planet.timestamp >> 4) < game_day)
				{
				multnum = ((game_day - (int)(planet.timestamp >> 4) - 1) * planupd)
					+ (planupd - (int)(planet.timestamp & 0xF)) + passnum;
				}
			else
			/* updated today but missed a pass? unlikely, but whatev */
			if ((planet.timestamp >> 4) == game_day)
				{
				multnum = passnum - (int)(planet.timestamp & 0xF);
				}
			else
			/* otherwise update once */
				multnum = 1;
			if (multnum < 0)
				multnum = 0;
			logthis(spr("...%d times",multnum));
			plptr = &planet;
			for (i = 0; i < multnum-1; ++i)
				multiply(FALSE);	/* don't send mail for multiples */
			if (multnum > 0)
				multiply(TRUE);
			logthis("calling checkspy");
			check_spy();
			logthis("back from checkspy");
			setbtv(gebb2);
			setmbk(gemb);
			}

		planet.timestamp = (((unsigned long)game_day) << 4) | passnum;
		gesdb(GEUPDATE, (PKEY *)&planet, (GALSECT *)&planet);
		}

		/* mbm was right, see GEREADME 02/04/90 */
		/* at this point, we either just finished a planet update or are coming into this routine fresh */
		/* either way, we need to set the key and cursor again for query next to work */
		gabbtv(&planet, fpos, 2);

		foundpl = FALSE;
		tic = 0;

		do
			{
			tic++;

			if (!qnxbtv() || (int)(gebb2->key[0]) != PLTYPE_PLNT)	/* hit a wormhole or no wormholes somehow? passnum done */
				{
				sprintf(gechrbuf,"%ld",tocks);
				logthis(spr("tocks %s. planets updated %u, empty %u, unowned %u"),gechrbuf,plown,plemt,plnob);

				ftocktime = ((double)(tocks * plantime))+1.0;
				ftockfact = ((double)(SECSADAY/planupd)/ftocktime);
				ftocktime = ((double)plantime*ftockfact);

				if (ftocktime < 3.0)
					{
					geshocst(1,"GE:INF:plarti:recalb tic forced to 3");
					plantime = 3;
					}
				else
#ifdef FASTPLANET
					{
					plantime = 3;
					}
#else
					{
					plantime = (int)ftocktime;
					}
#endif
				++passnum;
				foundpl = TRUE;
				fpos = 0;
				tocks = 0;
				plown = 0;
				plnob = 0;
				plemt = 0;
				break;
				}

			gcrbtv(&planet, 2);
			if (planet.userid[0] != '\0' && ((planet.items[I_MEN].qty > 0 || planet.items[I_TROOPS].qty > 0)
				|| (planet.xsect == 0 && planet.ysect == 0)))	/* owned and populated or in neut */
				{
				fpos = absbtv();
				foundpl = TRUE;
				sprintf(gechrbuf,"%lu",fpos);
				++plown;
				logthis(spr("plartia: found next owned planet at %s", gechrbuf));
				break;
				}
			else
				{
				fpos = absbtv();
				sprintf(gechrbuf,"%lu",fpos);
				if (planet.userid[0] != '\0')
					++plemt;
				else
					++plnob;
				}
			} while (tic < MAXTIC);
	}

if (foundpl == FALSE)
	logthis("plartia: no planet this tock");

#ifdef PHARLAP
rtkick(plantime, pplarti);
#else
rtkick(plantime, plarti);
#endif

return;
}

/**************************************************************************
** Real time kick routine                                                **
**************************************************************************/

#ifdef PHARLAP
void FUNC pwarrti(void)
{
warrtia();
return;
}
#else
int FUNC warrti(void)
{
warrtia();
return(0);
}
#endif

void FUNC warrtia(void)
{
int	zothusn;		/* general purpose other-user channel number */
WARSHP	*wptr;
int	cntr;

logthis("TICK:Warrtia entered");

cntr = 0;

#ifdef LOG
	printf ("warrti kick\r");
#endif

checkmines();		/* check for mines */
	decpass = (decpass + 1) % 5;

for (zothusn=0 ; zothusn < nships ;zothusn++)
	{
	wptr=warshpoff(zothusn);
	if (ingegame(zothusn))
		{
		logthis(spr("Chk Shp Stat %s",wptr->userid));
		setbtv(gebb1);
		setmbk(gemb);
		if (wptr->status == GESTAT_USER)
			++cntr;
		repairship(wptr,zothusn);
		validate_lock(wptr,zothusn);
		if (wptr->damage < 100.0)
			{
			shieldstat(wptr,zothusn);
			cloakstat(wptr,zothusn);
			recharge(wptr,zothusn);
			}
		checktm(wptr,zothusn);		/* check torps, missl, and decoys */
		fireion(wptr,zothusn);
		checkdam(wptr,zothusn);
		}
	}
numwar = cntr;
#ifdef PHARLAP
rtkick(TICKTIME,pwarrti);
#else
rtkick(TICKTIME,warrti);
#endif
}

/**************************************************************************
** Real time kick routine for all automatons                             **
**************************************************************************/

#ifdef PHARLAP
void FUNC pautorti(void)
{
autortia();
return;
}
#else
int FUNC autorti(void)
{
autortia();
return(0);
}
#endif

void FUNC autortia(void)
{
int	zothusn;            /* general purpose other-user channel number */
WARSHP	*wptr;

static int	ticktock1 = 0;
static int	ticktock2 = 0;
int		count,class,clscnt,i;

logthis("TICK:autorti entered");

setmbk(gemb);
setbtv(gebb1);

/* 12/19/91 spread out disk I/O over more time */

if (ticktock1 == 0)
	ticktock1 = nterms;

++ ticktock2;

logthis(spr("ticktock1 = %d -- ticktock2 = %d",ticktock1,ticktock2));

if (ticktock2 >= 30 && ticktock1 < nships)
	{
	logthis("ticktock2 >=30 and ticktock1 < nships");
	wptr=warshpoff(ticktock1);
	zothusn=ticktock1;

	if (wptr->status == GESTAT_AVAIL)
		{
		/* determine class of current slot */
		clscnt = ticktock1 - nterms;
		class = -1;
		logthis("Chan Stat = GESTAT_AVAIL");
		for (i=0;i<tot_classes;++i)
			{
			if (shipclass[i].max_type == CLASSTYPE_CYBORG ||
				shipclass[i].max_type == CLASSTYPE_DROID)
				{
				/* is this slot within class i */
				if (clscnt < shipclass[i].tot_to_create)
					{
					class = i;
					break;
					}
				/* no... check next class */
				clscnt -= shipclass[i].tot_to_create;
				}
			}

		logthis(spr("picked class - %d",class));

		/* initialize the non-user ship areas */
		if (class > -1 && shipclass[class].init_func != NULL)
			{
			logthis(spr("Calling init_func 4 cls %d",class));
			logthis(spr("   Name: %s",shipclass[i].typename));

			(*(shipclass[class].init_func))(wptr,zothusn,class);
			}
		}

	if (++ticktock1 >= nships)
		ticktock1 = nterms;
	ticktock2 = 0;
	}

if (cybhaltflg <= 0)
	{
	cybhaltflg = 0;

	/* cybertron loop */
	for (count = nterms; count < nships; count++)
		{
		wptr=warshpoff(count);
		zothusn=count;

		if (wptr->status == GESTAT_AUTO)
			{
			if (wptr->tick == 0)
				{
				logthis(spr("Calling tick_func 4 usn %d",zothusn));
				logthis(spr("  Class %d",wptr->shpclass));

				if (shipclass[wptr->shpclass].tick_func != NULL)
					(*(shipclass[wptr->shpclass].tick_func))(wptr,zothusn);
				}
			else
				{
				--wptr->tick;
				}
			}
		}
	}
else
	{
	--cybhaltflg;
	}

logthis("Exiting AUTORTI");

#ifdef PHARLAP
rtkick(1,pautorti);
#else
rtkick(1,autorti);
#endif
return;
}

/**************************************************************************
** Real time kick routine #2                                             **
**************************************************************************/

#ifdef PHARLAP
void FUNC pwarrti2(void)
{
warrti2a();
return;
}

#else
int FUNC warrti2(void)
{
warrti2a();
return(0);
}
#endif

void FUNC warrti2a(void)
{
int	zothusn;		/* general purpose other-user channel number */
WARSHP 	*wptr;

static int	clicker = 0;

logthis("TICK:PWarrti2 entered");

zothusn = clicker;

while (zothusn < nships)
	{
	if (ingegame(zothusn))
		{
		wptr=warshpoff(zothusn);
		setbtv(gebb1);
		setmbk(gemb);
		if (!(wptr->status == GESTAT_AUTO && wptr->helm < 0))
			{
			rotateship(wptr,zothusn);
			accel(wptr,zothusn);
			}
		moveship(wptr,zothusn);
		destruct(wptr,zothusn);
		}
	zothusn +=3;
	}

clicker = (clicker+1)%3;

#ifdef PHARLAP
rtkick(TICKTIME2,pwarrti2);
#else
rtkick(TICKTIME2,warrti2);
#endif
return;
}

/**************************************************************************
** Real time kick routine #3                                             **
**************************************************************************/

#ifdef PHARLAP
void FUNC pwarrti3(void)
{
warrti3a();
return;
}

#else
int FUNC warrti3(void)
{
warrti3a();
return(0);
}
#endif

void FUNC warrti3a(void)
{
COORD	tmpcoord;

logthis("TICK:Warrti3a entered");

setmbk(gemb);
prfmsg(ZAPHIM2);

tmpcoord.xcoord = 0.0;
tmpcoord.ycoord = 0.0;
outsect(FILTER,&tmpcoord,99,0);
#ifdef PHARLAP
rtkick(120,pwarrti3);
#else
rtkick(120,warrti3);
#endif
return;
}


/*********************/
/* U T I L I T I E S */
/*********************/


/**************************************************************************
** OUTPRF special doesn't output if Automaton                            **
**************************************************************************/

void FUNC outprfge(class,shpno)
int	class;
int	shpno;

{
/* added 12/17/91 as a safty check */
if (shpno >= 0 && shpno < nterms)
	{
	if (user[shpno].state == gestt)
		{
		if (class == ALWAYS)
			{
			outprf(shpno);
			return;
			}
		else
		if (class == FILTER && (warusroff(shpno)->options[MSG_FILTER] == TRUE))
			{
			clrprf();
			return;
			}
		else
			{
			outprf(shpno);
			return;
			}
		}
	}
clrprf();
}


/**************************************************************************
** Send message to all ships in this sector                              **
**************************************************************************/

void FUNC outsect(filter,coordptr,exclude,freq)
int	filter;
COORD	*coordptr;
unsigned exclude,freq;
{
int	i;
int	zothusn;
double	ddist;
byte	src_neb,oth_neb;
WARSHP	*wptr;

src_neb = (byte)innebula(coord1(coordptr->xcoord),coord1(coordptr->ycoord));

for (zothusn=0; zothusn < nterms ; zothusn++)
	{
	if (ingegame(zothusn) && zothusn != exclude)
		{
		wptr=warshpoff(zothusn);
		if (samesect(&(wptr->coord),coordptr))
			{
			if (freq == 0)
				{
				oth_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));
				if (src_neb || oth_neb)
					{
					ddist = cdistance(coordptr,&wptr->coord);
					ddist *= 10000;
					if (!(src_neb && oth_neb && ddist < (double)NEBRNG))
						continue;
					}
				outprfge(filter,zothusn);
				}
			else
				{
				for (i=0; i<3; ++i)
					{
					if (freq == wptr->freq[i])
						{
						outprfge(filter,zothusn);
						}
					}
				}
			}
		}
	}
clrprf();
}

/**************************************************************************
** Send message to all ships in scanning range of this ship              **
**************************************************************************/

void FUNC outrange(filter,coordptr)
int	filter;
COORD	*coordptr;
{
double	ddist;
int	zothusn;
byte	src_neb,oth_neb;
WARSHP	*wptr;

src_neb = (byte)innebula(coord1(coordptr->xcoord),coord1(coordptr->ycoord));

for (zothusn=0 ; zothusn < nships ; zothusn++)
	{
	wptr=warshpoff(zothusn);
	if (ingegame(zothusn) && shipclass[wptr->shpclass].max_type == CLASSTYPE_USER)
		{
		ddist = cdistance(coordptr,&wptr->coord);
		ddist *= 10000;
		oth_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));
		if ((src_neb || oth_neb) && !(src_neb && oth_neb && ddist < (double)NEBRNG))
			continue;
		if (ddist > 1 && ddist < (double)shipclass[wptr->shpclass].scanrange)
			outprfge(filter,zothusn);
		}
	}
clrprf();
}


/**************************************************************************
** Check is user is in the game                                          **
**   Automatons are always in the game                                   **
**************************************************************************/

int FUNC ingegame(shpno)
int	shpno;
{


if (shpno >= nships || shpno < 0)
	return(FALSE);

if (shpno < nterms)
	if (user[shpno].state == gestt && user[shpno].substt >= FIGHTSUB)
		return(TRUE);

if (shpno >= nterms && warshpoff(shpno)->status == GESTAT_AUTO)
	return(TRUE);

return(FALSE);
}



/**************************************************************************
** SHOCST Replacement                                                    **
**************************************************************************/

void FUNC geshocst(opt,str)
int	opt;
char	*str;
{
char	tmpbuf[40];
/* kill warning */
str = str;

/* Always display */
if (opt == 0)
	{
	tmp_usrnum = usrnum;
	usrnum = -1;
#ifdef PHARLAP
	strncpy(tmpbuf,str,32);
	tmpbuf[31] = NULL;
	shocst(tmpbuf,str);
#else
	shocst(0,str);
#endif
	usrnum = tmp_usrnum;
	}
else
if (opt <= showopt)
	{
	tmp_usrnum = usrnum;
	usrnum = -1;
#ifdef PHARLAP
	strncpy(tmpbuf,str,32);
	tmpbuf[31] = NULL;
	shocst(tmpbuf,str);
#else
	shocst(0,str);
#endif
	usrnum = tmp_usrnum;
	}
if (logflag)
	logthis(spr("CON: %s",str));
}

/*****************************************************************************
** The following mnu functions are response handlers for input from the     **
** player while in a particular state. Each of these then typically results **
** in additional menus/messages being displayed to the player and the       **
** players state (substt) modified.                                         **
*****************************************************************************/

/* player selected GE from the main menu */

int FUNC mnu_main()
{
prfmsg(INTRO,VERSION);
disp_main_menu();
outprfge(ALWAYS,usrnum);
usrptr->substt = 1;
return(1);
}

/* player selected somthing from the main menu */

int FUNC mnu_main_ans()
{
if (margc == 0 || margc > 1)
	{
	prfmsg(REPRMT);
	outprfge(ALWAYS,usrnum);
	return(1);
	}
else
if (margc == 1)
	{
	if (sameas(input,"P"))
		{
#ifdef PHARLAP
		if (!hasmkey(PLAYKEY))
			{
			prfmsg(FORPLAY);
			outprfge(ALWAYS,usrnum);
			prfmsg(REPRMT);
			outprfge(ALWAYS,usrnum);
			}
#else
		if (usrptr->class < PAYING && gefreebies == 0)
			{
			prfmsg(FORLIVE);
			outprfge(ALWAYS,usrnum);
			prfmsg(REPRMT);
			outprfge(ALWAYS,usrnum);
			}
#endif
		else
			{
			if (numwar < gemaxplrs)
				{
				lookupshp();
				clrprf();
				return(1);
				}
			else
				{
				prfmsg(NOSHPS);
				outprfge(ALWAYS,usrnum);
				prfmsg(REPRMT);
				outprfge(ALWAYS,usrnum);
				return(1);
				}
			}
		}
	else
	if (sameas(input,"G"))
		{
		prfmsg(EXPLAIN);
		outprfge(ALWAYS,usrnum);
		prfmsg(REPRMT);
		outprfge(ALWAYS,usrnum);
		return(1);
		}
	else
	if (sameas(input,"R"))
		{
		cmd_geroster();
		prfmsg(REPRMT);
		outprfge(ALWAYS,usrnum);
		return(1);
		}
	else
	if (sameas(input,"M"))
		{
		disp_menu_d();
		outprfge(ALWAYS,usrnum);
		return(1);
		}
	else
	if (sameas(input,"I"))
		{
		prfmsg(COINFO);
		outprfge(ALWAYS,usrnum);
		return(1);
		}
	else
	if (sameas(input,"?"))
		{
		disp_main_menu();
		outprfge(ALWAYS,usrnum);
		return(1);
		}
	else
	if (sameas(input,"X"))
		{
		prfmsg(EXIWAR);
		outprfge(ALWAYS,usrnum);
		btupmt(usrnum,0);
		return(0);
		}
	else
	if (optmenu)
		{
		input[0] = toupper(input[0]);
		if (input[0] == optchr)
			{
			optdisp();
			return(1);
			}
		}
	prfmsg(REPRMT);
	outprfge(ALWAYS,usrnum);
	return(1);
	}
return(1);
}

/* player is playing the game and entered a command */

int FUNC mnu_fightsub()
{
if (sameas(input,"x"))
	{
	if (warsptr->cantexit == 0 || (neutral(&warsptr->coord) && chkitm(usrnum)))
		{
		warsptr->cantexit = 0;
		cleartm(usrnum);
		gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
		geudb(GEUPDATE,waruptr->userid,waruptr);
		disp_main_menu();
		outprfge(ALWAYS,usrnum);
		prfmsg(PEACEOUT,waruptr->userid,warsptr->shipname);
		outwar(ALWAYS,usrnum,0);
		prfmsg(EXIWAR2,warsptr->shipname);
		outsect(ALWAYS,&warsptr->coord,usrnum,0);
		numwar = 0;
		usrptr->substt = 1;
		btupmt(usrnum,0);
		warsptr->status = GESTAT_AVAIL;
		}
	else
		{
		prfmsg(CANTEXT);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	if (margc > 0)
		gwar();
	else
		{
		prfmsg(FORHELP);
		outprfge(ALWAYS,usrnum);
		}
	}
return(1);
}

/* player has asked to admin a planet they do not own, and has been prompted
   to respond with yes or no to the question "do you wish to claim this
   planet". */

int FUNC mnu_admenu1()
{
int	i;
if (margc > 0)
	{
	if (genearas("y",margv[0]))
		{
		plnum = warsptr->where - 10;

		getplanetdat(usrnum);

		strncpy(plptr->userid,warsptr->userid,UIDSIZ);
		++waruptr->planets;
		geudb(GEUPDATE,waruptr->userid,waruptr);

		if (strlen(plptr->name) == 0)
			{
			for (i=0;i<NUMITEMS;++i)
				{
				plptr->items[i].rate = 0;
				plptr->items[i].markup2a = baseprice[i];
				}

			plptr->items[I_MEN].rate = 50;
			plptr->items[I_FOOD].rate = 50;
			}

		setsect(warsptr); /* build PKEY */
		gesdb(GEUPDATE,&pkey,&sector);

		pkey.plnum = plnum;
		gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);

		prfmsg(ADMENU1A);
		outprfge(ALWAYS,usrnum);
		usrptr->substt = ADMENU1A;
		}
	else
	if (genearas("n",margv[0]))
		{
		prfmsg(ADMIN3);
		outprfge(ALWAYS,usrnum);
		usrptr->substt = FIGHTSUB;
		}
	}
else
	{
	prfmsg(ADMENU1);
	outprfge(ALWAYS,usrnum);
	}
return(1);
}

/* player was asked to enter the name of the new planet and should have
   responded with a string.
*/

int mnu_admenu1a()
{
if (margc > 0)
	{
	plnum = warsptr->where - 10;

	getplanetdat(usrnum);
	rstrin();

	*margv[0] = toupper(*margv[0]);
	strncpy(plptr->name,margv[0],19);
	plptr->name[19] = 0;

	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);

	prfmsg(ADMENU1B,plnum,plptr->name,warsptr->userid,warsptr->shipname);

	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	}
else
	{
	prfmsg(ADMENU1A);
	outprfge(ALWAYS,usrnum);
	}
return(1);
}

/* player was displayed the admin main menu and should have selected an
   item from it.
*/

int FUNC mnu_admenu2()
{
int	i;
if (margc > 0)
	{
	if (*margv[0] >= '1' && *margv[0] <= '7')
		{
		plnum = warsptr->where - 10;

		getplanetdat(usrnum);

		switch (*margv[0])
			{
			case '1':
				prfmsg(ADMIN01,plptr->name);
				prfmsg(DASHESL);
				prfmsg(ADMIN02);
				for (i=0; i<NUMITEMS; ++i)
					{
					sprintf(gechrbuf,"%-11s %5u %10lu %5u %5u %5c %8lu",
							item_name[i],
							plptr->items[i].rate,
							plptr->items[i].qty,
							plptr->items[i].markup2a,
							plptr->items[i].reserve,
							plptr->items[i].sell,
							plptr->items[i].sold2a);
					prf("%s\r",gechrbuf);
					}
				prfmsg(DASHESL);
				sprintf(gechrbuf,"%lu",plptr->cash);
				prfmsg(ADMIN04,gechrbuf);
				sprintf(gechrbuf,"%lu",plptr->tax);
				prfmsg(ADMIN04A,gechrbuf);
				prfmsg(ADMIN04B,plptr->taxrate);
				prfmsg(ADMIN06,plptr->password);
				prfmsg(DASHESL);
				prfmsg(PRESSKEY);
				usrptr->substt = ADMENU2;
				break;

			case '2':
				prfmsg(ADMENU2B);
				usrptr->substt = ADMENU2B;
				break;

			case '3':
				prfmsg(ADMENU2E);
				usrptr->substt = ADMENU2E;
				break;

			case '4':
				prfmsg(ADMENU2G);
				usrptr->substt = ADMENU1A;
				break;

			case '5':
				prfmsg(ADMENU2H);
				usrptr->substt = ADMENU2H;
				break;

			case '6':
				prfmsg(ADMENU2I);
				usrptr->substt = ADMENU2I;
				break;

			case '7':
				prfmsg(ADMENU2J);
				usrptr->substt = ADMENU2J;
				break;

			}

		outprfge(ALWAYS,usrnum);
		}
	else
	if (sameas(input,"x"))
		{
		prfmsg(ADMIN3);
		outprfge(ALWAYS,usrnum);
		usrptr->substt = FIGHTSUB;
		}
	else
		{
		prfmsg(ADMENU2);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	}
return(1);
}

/* player asked to transfer cash from planet and has was prompted to enter
   the amount to transfer */

int FUNC mnu_admenu2b()
{
unsigned long amt;
plnum = warsptr->where - 10;

getplanetdat(usrnum);
amt = atol(margv[0]);

if (amt <= plptr->tax)
	{
	if (waruptr->cash > ULCAP - amt)
		{
		sprintf(gechrbuf,"%lu",ULCAP);
		prfmsg(TOORICH,gechrbuf);
		outprfge(ALWAYS,usrnum);
		}
	else
		{
		sprintf(gechrbuf,"%lu",amt);
		prfmsg(ADMENU2C,gechrbuf);
		outprfge(ALWAYS,usrnum);
		waruptr->cash += amt;
		plptr->tax -= amt;
		setsect(warsptr); /* build PKEY */
		pkey.plnum = plnum;
		gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);
		}
	}
else
	{
	prfmsg(ADMENU2D);
	}
prfmsg(ADMENU2);
outprfge(ALWAYS,usrnum);
usrptr->substt = ADMENU2;
return(1);
}

/* player asked to modify the parameters on an item, and was asked to enter
   the name of the selected item */

int FUNC mnu_admenu2e()
{
int	i;
plnum = warsptr->where - 10;
getplanetdat(usrnum);

for (i=0; i<NUMITEMS; ++i) /* skip notused */
	{
	if (genearas(kwrd[i],margv[0]))
		{
		warsptr->titem = i;
		prfmsg(ADMEN2F1,item_name[warsptr->titem],plptr->items[warsptr->titem].rate);
		outprfge(ALWAYS,usrnum);
		usrptr->substt = ADMEN2F1;
		return(1);
		}
	}
usrptr->substt = ADMENU2;
prfmsg(ADMENU2);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player selected an item, was prompted to select the percent of effort
   on his selected item and should have responded with a percent.
*/

int FUNC mnu_admenu2f1()
{
unsigned	amt;
plnum = warsptr->where - 10;
getplanetdat(usrnum);
amt = atoi(margv[0]);

if (margc == 0)
	{
	titems[usrnum].rate = plptr->items[warsptr->titem].rate;
	prfmsg(ADMEN2F2,item_name[warsptr->titem],plptr->items[warsptr->titem].markup2a);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMEN2F2;
	return(1);
	}
if (margc == 1 && amt <=100)
	{
	titems[usrnum].rate = amt;
	prfmsg(ADMEN2F2,item_name[warsptr->titem],plptr->items[warsptr->titem].markup2a);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMEN2F2;
	return(1);
	}
prfmsg(ADMEN2F1,item_name[warsptr->titem],plptr->items[warsptr->titem].rate);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player was asked how much to charge for his selected item */

int FUNC mnu_admenu2f2()
{
unsigned	amt;
plnum = warsptr->where - 10;
getplanetdat(usrnum);
amt = atoi(margv[0]);

if (margc == 0)
	{
	titems[usrnum].markup2a = plptr->items[warsptr->titem].markup2a;
	prfmsg(ADMEN2F3,item_name[warsptr->titem],plptr->items[warsptr->titem].sell);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMEN2F3;
	return(1);
	}
if (margc == 1 && amt <=32000)
	{
	titems[usrnum].markup2a = amt;
	prfmsg(ADMEN2F3,item_name[warsptr->titem],plptr->items[warsptr->titem].sell);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMEN2F3;
	return(1);
	}

prfmsg(ADMEN2F2,item_name[warsptr->titem],plptr->items[warsptr->titem].markup2a);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player was asked if he wished to sell this item to other ships */

int FUNC mnu_admenu2f3()
{
plnum = warsptr->where - 10;
getplanetdat(usrnum);

if (margc == 0)
	{
	titems[usrnum].sell = plptr->items[warsptr->titem].sell;
	prfmsg(ADMEN2F4,item_name[warsptr->titem],plptr->items[warsptr->titem].reserve);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMEN2F4;
	return(1);
	}
if (margc == 1 && (genearas("y",margv[0]) || genearas("n",margv[0])))
	{
	titems[usrnum].sell = toupper(*margv[0]);
	prfmsg(ADMEN2F4,item_name[warsptr->titem],plptr->items[warsptr->titem].reserve);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMEN2F4;
	return(1);
	}
prfmsg(ADMEN2F3,item_name[warsptr->titem],plptr->items[warsptr->titem].sell);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player was asked hom much to reserve for stock piling */

int FUNC mnu_admenu2f4()
{
unsigned	amt;
plnum = warsptr->where - 10;
getplanetdat(usrnum);
amt = atoi(margv[0]);

if (margc == 0)
	{
	titems[usrnum].reserve = plptr->items[warsptr->titem].reserve;
	update_items();
	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	return(1);
	}
if (margc == 1 && amt <=32000)
	{
	titems[usrnum].reserve = amt;
	update_items();
	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	return(1);
	}
prfmsg(ADMEN2F4,item_name[warsptr->titem],plptr->items[warsptr->titem].reserve);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player was asked how much to set taxes at */

int FUNC mnu_admenu2h()
{
unsigned	amt;
amt = atoi(margv[0]);

if (margc == 1 && amt <=100)
	{
	plnum = warsptr->where - 10;
	getplanetdat(usrnum);

	plptr->taxrate = amt;

	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);

	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	return(1);
	}
prfmsg(ADMENU2H);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player was asked to specify the trade password for his planet */

int FUNC mnu_admenu2i()
{

if (margc == 1)
	{
	plnum = warsptr->where - 10;
	getplanetdat(usrnum);

	strncpy(plptr->password,margv[0],sizeof(plptr->password)-1);
	plptr->password[sizeof(plptr->password)-1] = 0;

	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);

	if (sameas(plptr->password,"none"))
		{
		prfmsg(ADMEN2I2);
		plptr->teamcode = 0;
		}
	else
	if (sameas(plptr->password,"team"))
		{
		if (waruptr->teamcode > 0)
			{
			plptr->teamcode = waruptr->teamcode;
			prfmsg(ADMEN2I3);
			}
		else
			{
			plptr->teamcode = 0;
			plptr->password[0] = 0;
			prfmsg(ADMEN2I4);
			}
		}
	else
		{
		prfmsg(ADMEN2I1,plptr->password);
		plptr->teamcode = 0;
		}

	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	return(1);
	}
prfmsg(ADMENU2I);
outprfge(ALWAYS,usrnum);
return(1);
}

/* player was asked to enter a beacon message should have
   responded with a string.
*/

int FUNC mnu_admenu2j()
{
plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (margc == 0)
	{
	plptr->beacon[0] = 0;
	prfmsg(ADMEN2J1);
	}
else
	{
	rstrin();
	strncpy(plptr->beacon,margv[0],BEACONMSGSZ);
	plptr->beacon[BEACONMSGSZ-1] = 0;
	prfmsg(ADMEN2J2);
	}

setsect(warsptr); /* build PKEY */
pkey.plnum = plnum;
gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);

prfmsg (ADMENU2);
outprfge(ALWAYS,usrnum);
usrptr->substt = ADMENU2;
return(1);
}


/* player selected item 1 from the main menu */

int FUNC mnu_choosesh()
{
selectship();
return(1);
}

/* player selected read messages from main menu, was displayed the mail
   sub-menu, and was asked to select an option */

int mnu_menug()
{
if (margc > 0)
	{
	switch (tolower(*margv[0]))
		{

	case '1':

		if (mailread(usaptr->userid,MAIL_CLASS_DISTRESS))
			{
			prfmsg(usrptr->substt = MENUG1);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			disp_menu_d();
			outprfge(ALWAYS,usrnum);
			}
		break;

	case '2':

		if (mailread(usaptr->userid,MAIL_CLASS_PRODRPT))
			{
			prfmsg(usrptr->substt = MENUG2);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			disp_menu_d();
			outprfge(ALWAYS,usrnum);
			}
		break;

	case 'x':

		disp_main_menu();
		outprfge(ALWAYS,usrnum);
		usrptr->substt = 1;
		break;

	default:

		disp_menu_d();
		outprfge(ALWAYS,usrnum);
		break;
		}
	}
else
	{
	disp_menu_d();
	outprfge(ALWAYS,usrnum);
	}
return(1);
}

/* player selected option 1 on the mail sub-menu and
   player was displayed the first message in the mail file and has been asked
   to press N for next or X to exit.
*/


int FUNC mnu_menug1()
{
if (margc > 0)
	{
	switch (tolower(*margv[0]))
		{

	case 'n':

		if (mailread(usaptr->userid,MAIL_CLASS_DISTRESS))
			{
			prfmsg(usrptr->substt = MENUG1);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			disp_menu_d();
			outprfge(ALWAYS,usrnum);
			}
		break;

	case 'x':

		disp_menu_d();
		outprfge(ALWAYS,usrnum);
		break;

	default:

		prfmsg(MENUG1);
		outprfge(ALWAYS,usrnum);
		break;
		}
	}
else
	{
	prfmsg(MENUG1);
	outprfge(ALWAYS,usrnum);
	}
return(1);
}

/* player selected option 2 on the mail sub-menu and
   player was displayed the first message in the mail file and has been asked
   to press N for next or X to exit.
*/

int FUNC mnu_menug2()
{
if (margc > 0)
	{
	switch (tolower(*margv[0]))
		{

	case 'n':

		if (mailread(usaptr->userid,MAIL_CLASS_PRODRPT))
			{
			prfmsg(usrptr->substt = MENUG2);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			disp_menu_d();
			outprfge(ALWAYS,usrnum);
			}
		break;

	case 'x':

		disp_menu_d();
		outprfge(ALWAYS,usrnum);
		break;

	default:

		prfmsg(MENUG2);
		outprfge(ALWAYS,usrnum);
		break;
		}
	}
else
	{
	prfmsg(MENUG2);
	outprfge(ALWAYS,usrnum);
	}
return(1);
}


/* re-displays the main menu */

void FUNC disp_main_menu()
{
prfmsg (MENUA);

if (mailscan(usaptr->userid,0))
	prfmsg(MENUB2);
else
	prfmsg(MENUB1);

if (optmenu)
	prf("\r   %c ... %s",optchr,opttxt);

prfmsg(MENUC);

}

/* re-displays the mail sub-menu */

void FUNC disp_menu_d()
{
prfmsg(MENUD);
if (mailscan(usaptr->userid,MAIL_CLASS_DISTRESS))
	prfmsg(MENUE2);
else
	prfmsg(MENUE1);

if (mailscan(usaptr->userid,MAIL_CLASS_PRODRPT))
	prfmsg(MENUF2);
else
	prfmsg(MENUF1);
prfmsg(usrptr->substt=MENUG);
}

void FUNC update_items()

{
int i,pcnt = 0;
plnum = warsptr->where - 10;

getplanetdat(usrnum);

for (i=0; i<NUMITEMS;++i)
	if (i != warsptr->titem)
		pcnt += plptr->items[i].rate;

if ((titems[usrnum].rate + pcnt) > 100)
	{
	titems[usrnum].rate = 100 - pcnt;
	if (titems[usrnum].rate > 100)
		titems[usrnum].rate = 0;

	prfmsg(ADMEN2FA,item_name[warsptr->titem],titems[usrnum].rate);
	outprfge(ALWAYS,usrnum);
	}
i = (titems[usrnum].rate + pcnt);
if (i < 100)
	{
	i = 100 - i;
	prfmsg(ADMEN2FB,i);
	outprfge(ALWAYS,usrnum);
	}
plptr->items[warsptr->titem].rate = titems[usrnum].rate;
plptr->items[warsptr->titem].sell = titems[usrnum].sell;
plptr->items[warsptr->titem].reserve = titems[usrnum].reserve;
plptr->items[warsptr->titem].markup2a = titems[usrnum].markup2a;
setsect(warsptr); /* build PKEY */
pkey.plnum = plnum;
gesdb(GEUPDATE,(PKEY *)&pkey,(GALSECT *)&planet);

}

void FUNC optdisp()
{
static FILE *hdl = (FILE *)0;

if (hdl == (FILE *)0)
	{
	hdl = fopen("mpogemnu.txt","rt");
	if (hdl == (FILE *)0)
		geshocst(0,"GE:ERR MPOGEMNU.TXT Open Failed");
	else
		logthis("optdisp: mpogemnu.txt opened");
	}

if (hdl != (FILE *)0)
	{
	if(fseek(hdl,opttbl[usrnum],0)==0)
		{
		if (fgets(gechrbuf,85,hdl)!=NULL)
			{
			logthis(gechrbuf);
			prf(gechrbuf);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			opttbl[usrnum] = 0;
			logthis("optdisp: hit eof - fpos set back to 0");
			usrptr->substt = OPTDISP2;
			btuinj(usrnum,CYCLE);
			fclose(hdl);		/* be kind rewind */
			hdl = (FILE *)0;
			return;
			}
		opttbl[usrnum]=ftell(hdl);
		usrptr->substt = OPTDISP;
		btuinj(usrnum,CYCLE);
		}
	else
		{
		logthis(spr("optdisp: seek error fpos = %ld",opttbl[usrnum]));
		prf("\r*** Seek Err in text file ***\r");
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prf("*** FILE MISSING - Notify Sysop!! ***");
	outprfge(ALWAYS,usrnum);
	}
return;
}

void FUNC stshdlr()
{
if (status == CYCLE)
	{
	switch (usrptr->substt)
		{
		case	OPTDISP:
			setmbk(gemb);
			if (btuoba(usrnum)>(OUTSIZ/2))
				{
				optdisp();
				}
			else
				{
				btuinj(usrnum,CYCLE);
				}

			break;

		case	OPTDISP2:

			setmbk(gemb);
			usrptr->substt = 1;
			disp_main_menu();
			outprfge(ALWAYS,usrnum);
			return;

		default:
			dfsthn();
			return;

		}
	}
else
	{
	dfsthn();
	}
}
