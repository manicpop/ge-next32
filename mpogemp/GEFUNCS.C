
/***************************************************************************
 *                                                                         *
 *   GEFUNCS.C                                                             *
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

#ifdef PHARLAP

#include "gcomm.h"
#include "string.h"
#include "stdio.h"

#else
#include "stdio.h"
#include "ctype.h"
#include "dos.h"
#include "usracc.h"
#include "btvstf.h"
#include "stdlib.h"
#include "portable.h"
#include "dosface.h"

#endif
#include "math.h"
#include "majorbbs.h"

#include "gemain.h"

#define GEFUNCS 1

#include "geglobal.h"

/* LOCAL GLOBAL DEFS *****************************************************/



/**************************************************************************
** Look up the ships this player has                                     **
**************************************************************************/

void FUNC lookupshp()

{
int	noships = 0;

/* get the user record from GEuser.dat */

if (!(geudb(GELOOKUP,usaptr->userid, waruptr)))
	{
	/* Not found.... Better make up something */
	initusr(usaptr->userid); /* create his account */
	geudb(GEADD,tmpusr.userid,&tmpusr);
	memcpy(waruptr,&tmpusr,sizeof(WARUSR));	/* make it the current user */
	}
else
	{
	/* Got it! ... Dang are we lucky */
	geudb(GEGET,usaptr->userid, waruptr);
	}

setbtv(gebb1);

/* don't count if no ships at all, or no ships for this user */
if (qlobtv(0) && gepdb(GELOOKUPNAME, usaptr->userid, 0, warsptr))
	{
	/* get a total for user ship count */
	do
		{
		gcrbtv(warsptr,0);
		if (!sameas(usaptr->userid, warsptr->userid))
			break;
		noships++;
		} while (qnxbtv());

	waruptr->noships = noships;
	gepdb(GELOOKUPNAME, usaptr->userid, 0, warsptr);
	}

if (noships == 0)
	{
	initshp(usaptr->userid,0); /* give the dude a class 1 ship */
	gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
	memcpy(warsptr,&tmpshp,sizeof(WARSHP));	/* make is the current ship */
	waruptr->noships = 1;
	prfmsg(FIRSTIME);
	outprfge(ALWAYS,usrnum);
	}
else
if (noships > 1)
	{
	findships(0, 0);
	prfmsg(FLEET3);
	usrptr->substt = CHOOSESH;
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (noships == 1)
	{
	setbtv(gebb1);
	findships(0,1);

	if (gepdb(GEGET,usaptr->userid,scantab[usrnum].ship[0].shipno,warsptr))
		{
		tossingegame(); /* into the game you go bud! */
		return;
		}
	else
		{
		/* somehow lost the ship... make one anyway */
		geshocst(0,spr("GE:DBG:Ship Load Err %s",usaptr->userid));
		initshp(usaptr->userid,0); /* give the dude a class 1 ship */
		gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
		memcpy(warsptr,&tmpshp,sizeof(WARSHP));	/* make is the current ship */
		prfmsg(FIRSTIME);
		outprfge(ALWAYS,usrnum);
		}
	}
tossingegame();
}


/**************************************************************************
** Toss this yokel into the arena - God rest his soul                    **
**************************************************************************/

void FUNC tossingegame()
{

prfmsg(ANNOUN,shipclass[warsptr->shpclass].typename, warsptr->shipname, waruptr->userid);
outwar(FILTER,usrnum,0);

prfmsg(ENTSHP,waruptr->userid);
outprfge(ALWAYS,usrnum);

update_scantab(warshpoff(usrnum),usrnum);

if (warsptr->cloak != 10)
	{
	prfmsg(ENTWAR, warsptr->shipname);
	outsect(FILTER,&warsptr->coord,usrnum,0);
	}

btupmt(usrnum,'>');
prfmsg(WELCOM,waruptr->userid);
outprfge(ALWAYS,usrnum);
usrptr->substt = FIGHTSUB;
warsptr->status = GESTAT_USER;
assign_cybs(usrnum,0);
}

/**************************************************************************
** Initialize all the ship data                                          **
** NOTE: waruptr MUST be set to this channel first                       **
**************************************************************************/

int FUNC initshp(userid,type)

char	*userid;
int	type;
{
double	ddistance;
int	i,flag;

logthis(spr("GE:DBG:initship %d",type));
logthis(spr("%s",userid));
strncpy(tmpshp.userid,userid,UIDSIZ);
tmpshp.shpclass		= type;

if (shipclass[type].max_type == CLASSTYPE_USER)
	{
	strncpy(tmpshp.shipname," <NO NAME> ",20);

	tmpshp.coord.xcoord = NEUTRAL_X + rndm(.9999);
	tmpshp.coord.ycoord = NEUTRAL_Y + rndm(.9999);
	getsector(&tmpshp.coord);
	flag = 1;

	while (flag == 1)
		{
		tmpshp.coord.xcoord = NEUTRAL_X + rndm(.9999);
		tmpshp.coord.ycoord = NEUTRAL_Y + rndm(.9999);
		flag = 0;
		for (i=0;i<sector.numplan;++i)
			{
			if (sector.ptab[i].coord.xcoord != 0)
				{
				ddistance = cdistance(&tmpshp.coord,&sector.ptab[i].coord)*10000;
				if (ddistance < 1000)
					flag = 1;
				}
			}
		}

	tmpshp.phasrtype	= 1;
	tmpshp.shieldtype	= 1;
	tmpshp.items[I_FLUXPOD]	= 3;
	}
else
	{
	tmpshp.shipname[0]	= '\0';
	tmpshp.coord.xcoord	= 0.0;
	tmpshp.coord.ycoord	= 0.0;
	tmpshp.phasrtype	= 0;
	tmpshp.shieldtype	= 0;
	tmpshp.items[I_FLUXPOD]	= 0;
	}

tmpshp.heading		= gernd()%360;
tmpshp.head2b		= tmpshp.heading;
tmpshp.speed		= 0;
tmpshp.phasr		= 100;
tmpshp.speed2b		= 0;
tmpshp.damage		= 0.0;
tmpshp.lastfired	= -1;
tmpshp.energy		= 50000L;
tmpshp.kills		= 0;
tmpshp.tactical		= 0;
tmpshp.helm		= 0;
tmpshp.cloak		= 0;
tmpshp.shieldstat	= SHIELDDN;
tmpshp.shield		= 0;
tmpshp.degrees		= 0;
tmpshp.percent		= 0;
tmpshp.train		= 0;
tmpshp.where		= 0;
tmpshp.jam_sev		= 0;
tmpshp.jam_time		= 0;
for (i=0;i<3;++i)
	tmpshp.freq[i] = 0;
tmpshp.titem		= 0;
tmpshp.hostile		= 0;
tmpshp.repair		= 0;
tmpshp.hypha		= 0;
tmpshp.torpcntl		= 0;
tmpshp.mislcntl		= 0;
tmpshp.cantexit		= 0;
tmpshp.items[I_TORPEDO]	= 0;
tmpshp.items[I_MISSILE]	= 0;
tmpshp.items[I_FOOD]	= 0;
tmpshp.items[I_DECOYS]	= 0;
tmpshp.items[I_FIGHTER]	= 0;
tmpshp.items[I_MEN]	= 0;
tmpshp.items[I_IONCANNON]	= 0;
tmpshp.items[I_TROOPS]	= 0;
tmpshp.items[I_ZIPPERS]	= 0;
tmpshp.items[I_JAMMERS]	= 0;
tmpshp.items[I_MINE]	= 0;
tmpshp.items[I_GOLD]	= 0;

tmpshp.destruct		= 0;
tmpshp.status		= 0;
tmpshp.cybmine		= 0;
tmpshp.upgrade		= 0;	/*UNUSED ATM*/
tmpshp.cybupdate	= 0;
tmpshp.tick		= 0;
tmpshp.distress		= 255;
tmpshp.minesnear	= 0;
tmpshp.lock		= -1;
tmpshp.holdcourse	= 0;
tmpshp.overspeed	= 0L;
tmpshp.ukills		= 0;

tmpshp.zipload		= 0;
tmpshp.jamload		= 0;
tmpshp.decload		= 0;
tmpshp.mineload		= 0;
tmpshp.torps_fired	= 0;
tmpshp.missl_fired	= 0;

tmpshp.shipno = waruptr->topshipno+1;

++waruptr->topshipno;
++waruptr->noships;

for (i=0;i<MAXTORPS;++i)
	tmpshp.ltorps[i].distance = 0;

for (i=0;i<MAXMISSL;++i)
	tmpshp.lmissl[i].distance = 0;

for (i=0;i<MAXDECOY;++i)
	tmpshp.decout[i] = 0;

tmpshp.topspeed = shipclass[tmpshp.shpclass].max_warp;
logthis(spr("Created ship - topspeed = %d",tmpshp.topspeed));
return(0);
}

int FUNC initusr(userid)
char	*userid;
{

setmem(&tmpusr,sizeof(WARUSR),0);
strncpy(tmpusr.userid,userid,UIDSIZ); /* BJ CHANGED TO UIDSIZ */
tmpusr.cash		= startcash;
tmpusr.options[0]	= FULLNAMES; /* set scan default */

return(0);
}

/**************************************************************************
** find and list all the ships a single user has                         **
**************************************************************************/

int FUNC findships(int direction, int quiet)
{
int	found = 0;
int	i, j, step, thispage, lastpage;
int	first_no = 0;
int	last_no = 0;
int	before = 0;
int	first_shipno = 0;
SCANTAB *sptr;

setbtv(gebb1);
sptr = &scantab[usrnum];

/* if we're paging, grab current page’s known first/last ship numbers from scantab */
if (direction != 0)
	{
	for (i = 0; i < NOSCANTAB; ++i)
		{
		if (sptr->ship[i].shipno != 0)
			{
			first_no = sptr->ship[0].shipno;
			/* find last non-zero */
			for (j = NOSCANTAB-1; j >= 0; --j)
				if (sptr->ship[j].shipno != 0)
					{
					last_no = sptr->ship[j].shipno;
					break;
					}
			break;
			}
		}
	}
else
	/* make sure we're at beginning */
	gepdb(GELOOKUPNAME, usaptr->userid, 0, warsptr);


/* page navigation */
if (direction > 0 && last_no != 0)
	{
	/* last ship of current page, plus one */
	if (gepdb(GEGET, usaptr->userid, last_no, warsptr))
		{
		if (!qnxbtv())	/* no next page */
			return 0;
		}
	}
else
if (direction < 0 && first_no != 0)
	{
	/* first ship of current page, then back NOSCANTAB, plus one */
	if (gepdb(GEGET, usaptr->userid, first_no, warsptr))
		{
		step = 0;
		while (step < NOSCANTAB && qprbtv())
			step++;
		}
	}

/* clear the page buffer to avoid stale shipnos */
for (i = 0; i < NOSCANTAB; ++i)
	sptr->ship[i].shipno = 0;

/* print header and one page */
if (!quiet)
	prf("%s    Class                Name                 Sector         Status\r",CLR_CYAN2);
do
	{
	gcrbtv(warsptr,0);

	if (!sameas(usaptr->userid, warsptr->userid))
		break;

	setsect(warsptr);
	if (!quiet)
		{
		prf("%s%2d  %-20s %-20s %6d %6d  ", CLR_WHITE2, found+1,
			shipclass[warsptr->shpclass].typename, warsptr->shipname, xsect, ysect);

		if (warsptr->energy < 5000 && warsptr->items[I_FLUXPOD] == 0)
			prf("%sflux depleted%s",CLR_RED1,CLR_WHITE2);
		else
		if (warsptr->damage > 75.5)
			prf("%ssevere%s damage",CLR_RED1,CLR_WHITE2);
		else
		if (warsptr->damage > 50.5)
			prf("%sheavy%s damage",CLR_RED1,CLR_WHITE2);
		else
		if (warsptr->cloak > 0)
			prf("cloak %sON%s",CLR_GREEN2,CLR_WHITE1);
		else
		if (warsptr->items[I_GOLD] >= 500)
			{
			sprintf(gechrbuf,"%lu",warsptr->items[I_GOLD]);
			prf("%s gold",gechrbuf);
			}
		else
		if (warsptr->where > 10)
			prf("orbiting planet %s%d%s",CLR_BLUE2,warsptr->where-10,CLR_WHITE2);

		prf("\r");
		prf(CLR_WHITE2);
	}

	sptr->ship[found].shipno = warsptr->shipno;
	++found;
	} while (qnxbtv() && found < NOSCANTAB);

/* display page X of Y if needed */
if (!quiet && waruptr->noships > NOSCANTAB)
	{
	first_shipno = sptr->ship[0].shipno;
	before = 0;

	if (first_shipno != 0 && gepdb(GEGET, usaptr->userid, first_shipno, warsptr))
		{
		while (qprbtv())
			{
			gcrbtv(warsptr,0);
			if (!sameas(usaptr->userid, warsptr->userid))
				break;
			++before;
			}

		/* restore cursor to this page's first ship */
		gepdb(GEGET, usaptr->userid, first_shipno, warsptr);
	        }

	thispage = (before / NOSCANTAB) + 1;
	lastpage = (waruptr->noships + NOSCANTAB - 1) / NOSCANTAB;
	prf("%sPage %d of %d, ", CLR_CYAN2, thispage, lastpage);
	if (thispage == 1)
		prf("\"n\" for next page.\r");
	else
	if (thispage == lastpage)
		prf("\"p\" for previous page.\r");
	else
		prf("\"p\" for previous page, \"n\" for next page.\r");
	}
warsptr->status = 0;
outprfge(ALWAYS,usrnum);

return (found);
}

/**************************************************************************
** Select the ship to board from the list                                **
**************************************************************************/

void FUNC selectship()
{
int	selection;
int	shpno;
int	page_count;

/* exit back */
if ((sameas(margv[0],"x")) || (sameas(margv[0],"X")))
	{
	disp_main_menu();
	outprfge(ALWAYS,usrnum);
	usrptr->substt = 1;
	return;
	}

/* numeric selection on current page */
selection = (atoi(margv[0])) - 1;
if (selection >= 0 && selection < NOSCANTAB && scantab[usrnum].ship[selection].shipno != 0)
	{
	shpno = scantab[usrnum].ship[selection].shipno;
	setbtv(gebb1);
	if (gepdb(GEGET,usaptr->userid,shpno,warsptr))
		{
		tossingegame(); /* into the game you go bud! */
		return;
		}
	}

/* paging */
if (sameas(margv[0], "N") || sameas(margv[0], "n"))
	{
	page_count = findships(1, 0);
	if (page_count == 0)
		{
		prfmsg(FLEET4);
		page_count = findships(0, 0);
		}
	prfmsg(FLEET3);
	usrptr->substt = CHOOSESH;
	outprfge(ALWAYS, usrnum);
	return;
	}

if (sameas(margv[0], "P") || sameas(margv[0], "p"))
	{
	page_count = findships(-1, 0);
	if (page_count == 0)
		{
		prfmsg(FLEET4);
		page_count = findships(0, 0);
		}
	prfmsg(FLEET3);
	usrptr->substt = CHOOSESH;
	outprfge(ALWAYS, usrnum);
	return;
	}

/* anything else, show error and first page again */
prfmsg(FLEET4);
page_count = findships(0, 0);
prfmsg(FLEET3);
usrptr->substt = CHOOSESH;
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Repair the ship                                                       **
**************************************************************************/

void FUNC repairship(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
if (ptr->repair > 0)
	{
	if (ptr->cantexit > 0 && ptr->repair != 255)	/* allow for sys maint */
		{
		prfmsg(MAINT10);
		ptr->repair = 0;
		outprfge(ALWAYS,usrn);
		return;
		}

	if (ptr->damage > 3.0)
		ptr->damage -= 3.0;
	else
		ptr->damage = 0.0;

	ptr->repair = (int)(ptr->damage/3.0);
	if (ptr->repair <= 1)
		{
		ptr->repair = 0;
		ptr->damage = 0.0;
		if (ptr->phasr < 1)
			ptr->phasr = 0;
		ptr->tactical = 0;
		ptr->helm = 0;
		if (ptr->cloak < 1)	/* this is for oliver */
			ptr->cloak = 0;
		ptr->torpcntl = 0;
		ptr->mislcntl = 0;
		ptr->zipload = 0;
		ptr->jamload = 0;
		ptr->decload = 0;
		ptr->mineload = 0;
		ptr->torps_fired = 0;
		ptr->missl_fired = 0;

		if (ptr->shieldstat == SHIELDDM)
			ptr->shieldstat = SHIELDDN;
		if (ptr->shield < 1)
			ptr->shield = 0;
		ptr->topspeed = shipclass[ptr->shpclass].max_warp;
		ptr->overspeed = 0;
		prfmsg(MAINT7);
		outprfge(ALWAYS,usrn);
		}
	}
}


/**************************************************************************
** Rotate the ship                                                       **
**************************************************************************/

void FUNC rotateship(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
int	angle;
double	rotamt;

rotamt = (double)(shipclass[ptr->shpclass].max_accel/10.0);

if (ptr->heading != ptr->head2b)
	{
	if (fabs(normal(ptr->heading - ptr->head2b)) >= (360.0-rotamt) ||
		fabs(normal(ptr->heading - ptr->head2b)) <= rotamt)
		{
		ptr->heading = ptr->head2b;
		angle = (int)ptr->heading;
		prfmsg(NOWTHER,angle);
		outprfge(FILTER,usrn);
		}
	else
		{
		angle = (int)normal(ptr->heading - ptr->head2b);
		if (angle < 180)	/* rotate left */
			ptr->heading = normal(ptr->heading - rotamt);
		else			/* rotate right */
			ptr->heading = normal(ptr->heading + rotamt);
		angle = (int)ptr->heading;
		prfmsg(NOWTRNP,angle);
		outprfge(FILTER,usrn);
		}
	}
}


/**************************************************************************
** Accelerate the ship                                                   **
**************************************************************************/

void FUNC accel(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
int	usage;
int	newwarp;
int	need;
double	accelrate,decelrate;

if (ptr->speed < ptr->speed2b)
	{
	accelrate = (double)shipclass[ptr->shpclass].max_accel;
	if (ptr->speed < 1000 && ptr->speed2b < 1000)
		usage = 0;
	else
		usage = ACCENGAMT;
	if (fabs(ptr->speed - ptr->speed2b) <= accelrate)
		{
		need = usage;
		if (ptr->speed/1000 < 1 && ptr->speed2b >= 1000)
			{
			newwarp = (int)(ptr->speed2b/1000.0);
			need += (newwarp + 10);
			}
		if (fluxstat(ptr,usrn,need) == 1)
			{
			if ((ptr->speed2b >= 1000) && (ptr->speed/1000 < 1))
				hyperspace(ptr,usrn,1);
			ptr->speed = ptr->speed2b;
			ptr->energy -= usage;

			prfmsg(SPEEDIS, showarp(ptr->speed));
			outprfge(FILTER,usrn);
			}
		else
			{
			if (ptr->speed2b >= 1000 && ptr->energy < (ACCENGAMT + 10 + 1))
				prfmsg(MOVE5);
			else
				prfmsg(NOACCEL,(int)(ptr->speed/1000.0));
			outprfge(ALWAYS,usrn);
			decelrate = (double)shipclass[ptr->shpclass].max_accel * 2.0;
			if ((ptr->speed/1000 >=1) && ((ptr->speed-decelrate)/1000 <1))
				hyperspace(ptr,usrn,0);
			if (fabs(ptr->speed) <= decelrate)
				ptr->speed = 0;
			else
				ptr->speed -= decelrate;
			ptr->speed2b = 0;
			}
		}
	else
		{
		need = usage;
		if (ptr->speed/1000 < 1 && (ptr->speed+accelrate) >= 1000)
			{
			newwarp = (int)((ptr->speed+accelrate)/1000.0);
			need += (newwarp + 10);
			}
		if (fluxstat(ptr,usrn,need) == 1)
			{
			if ((ptr->speed2b >= 1000) && (ptr->speed/1000 < 1) && ((ptr->speed+accelrate)/1000 >=1))
				hyperspace(ptr,usrn,1);
			if ((int)(ptr->speed/accelrate) != (int)((ptr->speed + accelrate)/accelrate))
				{
				sprintf(gechrbuf,"%.2f",(ptr->speed + accelrate)/1000.0);
				prfmsg(WARP,gechrbuf);
				outprfge(FILTER,usrn);
				}
			ptr->speed += accelrate;
			ptr->energy -= usage;
			}
		else
			{
			if (ptr->speed2b >= 1000 && ptr->energy < (ACCENGAMT + 10 + 1))
				prfmsg(MOVE5);
			else
				prfmsg(NOACCEL,(int)(ptr->speed/1000.0));
			outprfge(ALWAYS,usrn);
			decelrate = (double)shipclass[ptr->shpclass].max_accel * 2.0;
			if ((ptr->speed/1000 >=1) && ((ptr->speed-decelrate)/1000 <1))
				hyperspace(ptr,usrn,0);
			if (fabs(ptr->speed) <= decelrate)
				ptr->speed = 0;
			else
				ptr->speed -= decelrate;
			ptr->speed2b = 0;
			}
		}
	}
else
if (ptr->speed > ptr->speed2b)
	{
	decelrate = (double)shipclass[ptr->shpclass].max_accel * 2.0;
	if ((ptr->speed2b < 1000) && (ptr->speed/1000 >=1) && ((ptr->speed-decelrate)/1000 <1))
		hyperspace(ptr,usrn,0);

	if (fabs(ptr->speed - ptr->speed2b) <= decelrate)
		{
		ptr->speed = ptr->speed2b;
		if (ptr->speed > 0)
			{
			prfmsg(SPEEDIS,showarp(ptr->speed));
			outprfge(FILTER,usrn);
			}
		else
			{
			prfmsg(DEADSTOP);
			outprfge(FILTER,usrn);
			}
		}
	else
		{
		if ((int)(ptr->speed/decelrate) != (int)((ptr->speed - decelrate)/decelrate))
			{
			if (ptr->speed > 0 )
				{
				sprintf(gechrbuf,"%.2f",(ptr->speed - decelrate)/1000.0);
				prfmsg(WARP,gechrbuf);
				outprfge(FILTER,usrn);
				}
			else
				{
				prfmsg(DEADSTOP);
				outprfge(FILTER,usrn);
				}
			}
		if (ptr->speed/1000 > ptr->topspeed && (ptr->speed - decelrate)/1000 <= ptr->topspeed &&
			ptr->topspeed < shipclass[ptr->shpclass].max_warp)
			prfmsg(WARPSPD,ptr->topspeed);
		ptr->speed -= decelrate;
		}
	}
}


/**************************************************************************
** Make the jump to or from hyperspace                                   **
**************************************************************************/

void FUNC hyperspace(ptr,usrn,flag)

WARSHP	*ptr;
int	usrn;
{

int i;

if (flag == 1)
	{
	if (ptr->shieldstat == SHIELDUP)
		{
		prfmsg(SHLDDN);
		ptr->shieldstat = SHIELDDN;
		}
	if (ptr->cloak > 0 && ptr->cloak != 3)
		{
		prfmsg(CLOKOFF);
		ptr->cloak = 3;
		}
	prfmsg(HYPERIN,ptr->shipname);
	outprfge(FILTER,usrn);

	ptr->where = 1;

	if (ptr->status == GESTAT_AUTO)
		prfmsg(HYPERINN,ptr->shipname);
	else
		prfmsg(HYPERIN2,ptr->shipname);
	outsect(FILTER,&(warshpoff(usrn)->coord),usrn,0);

	for(i=0;i<MAXTORPS;++i)
		{
		if (ptr->ltorps[i].distance > 0)
			{
			if (flag == 1 && ingegame(ptr->ltorps[i].channel) && ptr->ltorps[i].channel < nterms)
				{
				prfmsg(TORMISS2,shpltr(ptr->ltorps[i].channel,usrn));
				outprfge(FILTER,ptr->ltorps[i].channel);
				}
			flag = 0;
			}
		ptr->ltorps[i].distance = 0;
		}
	for(i=0;i<MAXDECOY;++i)
		ptr->decout[i] = 0;
	}
else
	{
	prfmsg(HYPEROUT,ptr->shipname);
	outprfge(FILTER,usrn);

	ptr->where = 0;

	if (ptr->status == GESTAT_AUTO)
		{
		/* tough cybs raise shields immediately */
		if (shipclass[ptr->shpclass].tough_factor > 1 && ptr->shieldstat == SHIELDDN)
			shieldup(ptr,usrn);
		prfmsg(HYPEROU2,ptr->shipname);
		}
	else
		prfmsg(HYPEROUN,ptr->shipname);
	outsect(FILTER,&(warshpoff(usrn)->coord),usrn,0);
	}
}

/**************************************************************************
** Move the ship                                                         **
**************************************************************************/

void FUNC moveship(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

WARSHP	*wptr;
COORD	oldsect,newsect,neutsect;
int	overamt,intspeed,zothusn,movenergy;
double	ddist;
float	newtop;
byte	ptr_neb,oth_neb;

neutsect.xcoord = 0.50001;
neutsect.ycoord = 0.50001;

if (ptr->speed > 0)
	{
	if (ptr->status == GESTAT_USER && ptr->speed <= ptr->speed2b)
		{
		intspeed = ptr->speed/1000.0;
		if (intspeed < 1)
			movenergy = 1;
		else
			movenergy = intspeed + 10;
		if (fluxstat(ptr,usrn,movenergy) == 0)
			{
			ptr->speed2b = 0;
			if (intspeed >= 1 && ptr->energy < (ACCENGAMT + 10 + 1))
				prfmsg(MOVE5);
			else
				prfmsg(MOVE4);
			outprfge(FILTER,usrn);
			if ((ptr->speed/1000 >=1) && ((ptr->speed-((double)shipclass[ptr->shpclass].max_accel * 2.0))/1000 <1))
				hyperspace(ptr,usrn,0);
			if (fabs(ptr->speed) <= ((double)shipclass[ptr->shpclass].max_accel * 2.0))
				ptr->speed = 0;
			else
				ptr->speed -= ((double)shipclass[ptr->shpclass].max_accel * 2.0);
			return;
			}
		else
			ptr->energy -= movenergy;
		}

	movecoord(&oldsect, &ptr->coord);
	ptr->coord.xcoord = ptr->coord.xcoord + ((ptr->speed * sin(degtorad(ptr->heading)))/65000.0);
	ptr->coord.ycoord = ptr->coord.ycoord - ((ptr->speed * cos(degtorad(ptr->heading)))/65000.0);

	if (ptr->where <= 1)
		{
		if (ptr->coord.xcoord > univmax+1)
			{
			if (univwrap)
				{
				ptr->coord.xcoord -= (double)((univmax*2)+1);
				}
			else
				{
				ptr->coord.xcoord = (double)(univmax-2-(int)(ptr->speed/1000));
				if (ptr->coord.ycoord <= univmax+1 && ptr->coord.ycoord >= (univmax*-1)) /* avoid double bounce */
					{
					ptr->head2b = normal(vector(&(ptr->coord),&neutsect));
					ptr->heading = ptr->head2b;
					telezip(ptr,usrn);
					}
				}
			}
		else
		if (ptr->coord.xcoord < (univmax*-1))
			{
			if (univwrap)
				{
				ptr->coord.xcoord += (double)((univmax*2)+1);
				}
			else
				{
				ptr->coord.xcoord = (double)((univmax-2-(int)(ptr->speed/1000))*-1);
				if (ptr->coord.ycoord <= univmax+1 && ptr->coord.ycoord >= (univmax*-1))
					{
					ptr->head2b = normal(vector(&(ptr->coord),&neutsect));
					ptr->heading = ptr->head2b;
					telezip(ptr,usrn);
					}
				}
			}


		if (ptr->coord.ycoord > univmax+1)
			{
			if (univwrap)
				{
				ptr->coord.ycoord -= (double)((univmax*2)+1);
				}
			else
				{
				ptr->coord.ycoord = (double)(univmax-2-(int)(ptr->speed/1000));
				ptr->head2b = normal(vector(&(ptr->coord),&neutsect));
				ptr->heading = ptr->head2b;
				telezip(ptr,usrn);
				}
			}
		else
		if (ptr->coord.ycoord < (univmax*-1))
			{
			if (univwrap)
				{
				ptr->coord.ycoord += (double)((univmax*2)+1);
				}
			else
				{
				ptr->coord.ycoord = (double)((univmax-2-(int)(ptr->speed/1000))*-1);
				ptr->head2b = normal(vector(&(ptr->coord),&neutsect));
				ptr->heading = ptr->head2b;
				telezip(ptr,usrn);
				}
			}
		}

	movecoord(&newsect, &ptr->coord);

	if (!samesect(&oldsect, &newsect))
		{
		prfmsg(MOVE1,
			(innebula(coord1(oldsect.xcoord),coord1(oldsect.ycoord)) ? CLR_GREEN2 "nebula" : "sector"),
			coord1(oldsect.xcoord),coord1(oldsect.ycoord),
			(innebula(coord1(newsect.xcoord),coord1(newsect.ycoord)) ? CLR_GREEN2 "nebula" : "sector"),
			coord1(newsect.xcoord),coord1(newsect.ycoord));
		outprfge(FILTER,usrn);
		if (ptr->cloak != 10)
			{
			if (ptr->speed < 21000.0)
				{
				if (ptr->status == GESTAT_AUTO)
					prfmsg(MOVE2N,ptr->shipname);
				else
					prfmsg(MOVE2,ptr->shipname);
				outsect(FILTER,&oldsect,usrn,0);
				}
			if (ptr->speed < 21000.0)
				{
				if (ptr->status == GESTAT_AUTO)
					prfmsg(MOVE3N,ptr->shipname);
				else
					prfmsg(MOVE3,ptr->shipname);
				outsect(FILTER,&newsect,usrn,0);
				}
			}
		ptr->hostile = 0;
		if (ptr->destruct > 0 && neutral(&newsect))
			{
			prfmsg(SELFD4);
			outprfge(ALWAYS,usrn);
			ptr->destruct = 0;
			}
		}

	/* if I am cloaked tell the closer ones */
	if (ptr->cloak == 10)
		{
		unsigned int r = gernd();
		if (ptr->speed2b > (double)(((r >> 5) % 200) + 10) && (((r >> 8) % 25) == 0))
			{
			ptr_neb = (byte)innebula(coord1(ptr->coord.xcoord),coord1(ptr->coord.ycoord));
			for (zothusn=0 ; zothusn < nterms ; zothusn++)
				{
				wptr=warshpoff(zothusn);
				if (ingegame(zothusn) && zothusn != usrn)
					{
					ddist = cdistance(&warsptr->coord,&wptr->coord);
					ddist *= 10000;
					oth_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));
					if ((ptr_neb || oth_neb) && !(ptr_neb && oth_neb && ddist < (double)NEBRNG))
						continue;

					if (ddist < (shipclass[wptr->shpclass].scanrange)
						&& ddist < 20000 && wptr->jam_sev <= (byte)2)
						{
						bearing = cbearing(&wptr->coord,&ptr->coord,wptr->heading);
						/* slop it up +- 10 degrees on either side */
						bearing += ((r >> 11) % 20) - 10;
						prfmsg(CLOK3,bearing);
						outprfge(ALWAYS,zothusn);
						}
					}
				}
			}
		}
	if (ptr->speed > 0.0 && ptr->status == GESTAT_USER)
		{
		unsigned int r = gernd();
		intspeed = ptr->speed/1000.0;

		/* if this ship is exceeding top cruising speed and not in the process of going under it */
		if (intspeed > ptr->topspeed && (int)(ptr->speed2b/1000.0) > ptr->topspeed)
			{
			newtop = shipclass[ptr->shpclass].max_warp * (1.0f - (float)ptr->overspeed / 10000.0f);
			if (newtop < 1.0f)
				{
				prfmsg(WARPBRK);
				outprfge(ALWAYS,usrn);
				ptr->topspeed = 0;
				ptr->speed2b = 0;
				ptr->damage += r%20;
				}
			else
				{
				if (ptr->topspeed != newtop && ptr->topspeed != 0)
					ptr->topspeed = (int)newtop;

				/* for every 10% over cruising speed, increase potential random damage */
				overamt = (intspeed * 100 / newtop) - 100;
				ptr->overspeed += (r%(overamt+1));

				/* if over twice new cruising speed, blow up the engines */
				if ((overamt >= 110 || ptr->overspeed > 4000000000UL) && ptr->topspeed != 0)
					{
					prfmsg(WARPBRK);
					outprfge(ALWAYS,usrn);
					ptr->topspeed = 0;
					ptr->speed2b = 0;
					ptr->damage += r%20;
					}
				else
				if (overamt != ptr->npcmsg)
					{
					if (overamt >= 60 && overamt/10 != ptr->npcmsg/10)
						{
						prfmsg(WARPFAST+(int)((overamt/10)-6));
						outprfge(FILTER,usrn);
						}
					ptr->npcmsg = overamt;
					}
				}
			}

		}
	/* Cybertrons ignore gravity */
	if (ptr->where == 0 && ptr->status == GESTAT_USER)
		gravity(ptr,usrn);

	if (ptr->hostile > 0)
		checkdist(ptr,usrn);
	}
else
	{
	if (ptr->where == 1)	/* fix stuck users */
		ptr->where = 0;
	}



/* if there is a beacon message and same sector display it */
if (samesect(&beacon[usrn].coord,&ptr->coord))
	{
	if (beacon[usrn].beacon[0] != 0 && gernd()%10 == 0)
		{
		prfmsg(BEAC01,beacon[usrn].plnum,beacon[usrn].beacon);
		outprfge(FILTER,usrn);
		}
	}
}

void FUNC telezip(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
/*
ptr->coord.xcoord       = (rndm((double)(univmax-2)))+1;
ptr->coord.ycoord       = (rndm((double)(univmax-2)))+1;
*/
ptr->speed2b = 0.0;
ptr->speed = ptr->speed2b;
ptr->where = 0;
if (ptr->status == GESTAT_USER)
	{
	ptr->damage += TELEDAM;
	damstr(TELEDAM);
	prfmsg(TELEPORT,gechrbuf);
	prfmsg(NOWTHER,(int)ptr->heading);	/* show now pointing towards 0 0 */
	outprfge(ALWAYS,usrn);
	}
else
	{
	if (ptr->topspeed == 0)
		ptr->speed2b = 990;
	else
		ptr->speed2b = (double)ptr->topspeed*1000.0;	/* head toward 0 0 for the moment */
	ptr->cybupdate = 20 + gernd()%5;	/* save and pick new heading after a while */
	}
}


void FUNC gravity(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
int	i;
unsigned dist;

refresh(ptr, usrn);

/* check distances to plantets */

for (i=0; i<MAXPLANETS;++i)
	{
	if (ptab[usrn].planets[i].type != 0)
		{
		dist = (unsigned)(cdistance(&ptr->coord,&ptab[usrn].planets[i].coord)*10000);
	/*      prf("dist to planet %u is %d\r",i,dist);
		outprfge(ALWAYS,usrn);*/
		if (dist < 250 && ptr->damage < 101.0)	/* no addl msgs after crash */
			{
			if (dist >= 50)
				{
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					prfmsg(GRAVITY1,i+1);
				else
					prfmsg(GRAVWRM1,i+1);

				outprfge(ALWAYS,usrn);
				}
			else
			if (dist >= 25)
				{
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					prfmsg(GRAVITY2,i+1);
				else
					prfmsg(GRAVWRM2,i+1);

				outprfge(ALWAYS,usrn);
				}
			else
			if (dist < 25 )
				{
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					prfmsg(GRAVITY3,i+1);
				else
					prfmsg(GRAVWRM3,i+1);

				outprfge(ALWAYS,usrn);
				if (ptab[usrn].planets[i].type == PLTYPE_PLNT)
					{
					ptr->damage = 101.0;
					ptr->cantexit = FIRETICKS; /* no exiting after crashing */
					}
				else
					{
					setsect(ptr); /* build PKEY */
					pkey.plnum = i+1;
					gesdb(GEGETNOW,&pkey,(GALSECT *)&worm);
					ptr->coord.xcoord = worm.destination.xcoord;
					ptr->coord.ycoord = worm.destination.ycoord;
					prfmsg(MOVE1,
						(innebula(xsect,ysect) ? CLR_GREEN2 "nebula" : "sector"),
						xsect,ysect,
						(innebula(coord1(worm.destination.xcoord),coord1(worm.destination.ycoord)) ? CLR_GREEN2 "nebula" : "sector"),
						coord1(worm.destination.xcoord),coord1(worm.destination.ycoord));
					ptr->damage+= 5.5;
					outprfge(ALWAYS,usrn);
					clearitm(usrn);	 /* clear the tors and missiles */
					ptr->jam_time = (byte)0;
					ptr->jam_sev = (byte)0;
					assign_cybs(usrn,0); /* clear current cyb pursuits and pick closest new ones */
					}
				}
			}
		}
	}
}

/* If player is far from the planet they were attacking then they are not
	being hostile */

void FUNC checkdist(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
int	i;
unsigned dist;

refresh(ptr, usrn);

i = ptr->hostile - 11;
if (ptab[usrn].planets[i].type != 0)
	{
	dist = (unsigned)(cdistance(&ptr->coord,&ptab[usrn].planets[i].coord)*10000);
	if (dist > 1000)
		{
		ptr->hostile = 0;
		}
	}
}

void FUNC refresh(ptr, usrn)

WARSHP	*ptr;
int	usrn;
{
int	i;

COORD	ss,tmpcoord;

movecoord(&ss,&ptr->coord);

/* need to refresh planet coords? */

tmpcoord.xcoord = 32767.000;
tmpcoord.ycoord = 32767.000;

for (i = 0; i < MAXPLANETS; ++i)
	{
	if (ptab[usrn].planets[i].type != 0)
		{
		movecoord(&tmpcoord,&ptab[usrn].planets[i].coord);
		break;
		}
	}

if (!samesect(&tmpcoord, &ss))
	{
	logthis(spr("GEFUNCS:refreshing sector for usrn %d",usrn));
	getsector(&ss);
	memcpy(&ptab[usrn],&sector.ptab,sizeof(PLANETAB));
	}
}

/**************************************************************************
** Check the damage and repair any - Also service weapons                **
**************************************************************************/

void FUNC checkdam(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

double	preload;

logthis(spr("GE:Chn %d checkdam %s",usrn,ptr->userid));

if (ptr->damage >= 100.0)
	{
	ptr->damage = 0.0;	/* reset damage so he can get back on */
	prfmsg(YOURDEAD);
	outprfge(ALWAYS,usrn);

/* DEBUG
	prf ("lastfired = %u\r",ptr->lastfired);
	outprfge(ALWAYS,usrn); */

	killem(ptr,usrn);

	/* only reset btupmt on "real" users */
	if (usrn < nterms)
		btupmt(usrn,'\0');

	if (ptr->status == GESTAT_AUTO || ptr->userid[0] == '@')
		ptr->status = GESTAT_AVAIL;

	if (ptr->status == GESTAT_USER)
		{
/*		user[usrn].state = 0;*/
		user[usrn].substt = 0;
		--numwar;
		ptr->where = -1;
		}
	return;
	}

/* repair ship, always */
if (ptr->damage > 0.0)
	ptr->damage = ptr->damage - repairrate;
else
	ptr->damage = 0.0;

/* charge phaser if not damaged */
if (ptr->phasr < 100 && ptr->phasr >= 0)
	{
	if (fluxstat(ptr,usrn,PENGUSE) == 1)
		{
		ptr->energy -= PENGUSE;
		/* If phasers get to minimum fire power tell captain */
		preload = (double)(ptr->phasrtype * PRELOAD);
		/* If phaser goes from under to 100 in one step, just show one msg */
		if (ptr->phasr < PMINFIRE && ptr->phasr + preload >= PMINFIRE && ptr->phasr + preload < 100)
			{
			prfmsg(PHSRUP);
			outprfge(ALWAYS,usrn);
			}

		ptr->phasr = ptr->phasr + preload;

		/* if phasers get to 100% tell captain, and set to 100% */
		if (ptr->phasr >= 100)
			{
			prfmsg(PHSRMAX);
			outprfge(ALWAYS,usrn);
			ptr->phasr = 100;
			}
		}
	}

/* only repair one system at a time (shields are separate), ranked by importance */

/* repair helm */
if (ptr->helm < 0)
	{
	++ptr->helm;
	if (ptr->helm == 0)
		{
		prfmsg(HLREPR);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair tactical display */
if (ptr->tactical < 0)
	{
	++ptr->tactical;
	if (ptr->tactical == 0)
		{
		prfmsg(TAREPR);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair phaser */
if (ptr->phasr < 0)
	{
	ptr->phasr += 1;
	if (ptr->phasr == 0)
		{
		prfmsg(PHREPR);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair torpedo launchers */
if (ptr->torpcntl > 0)
	{
	--ptr->torpcntl;
	if (ptr->torpcntl == 0)
		{
		prfmsg(FCREPRT);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair missile launchers */
if (ptr->mislcntl > 0)
	{
	--ptr->mislcntl;
	if (ptr->mislcntl == 0)
		{
		prfmsg(FCREPRM);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair cloak */
if (ptr->cloak < 0)
	{
	++ptr->cloak;
	if (ptr->cloak == 0)
		{
		prfmsg(CLREPR);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair jammer launcher */
if (ptr->jamload < 0)
	{
	++ptr->jamload;
	if (ptr->jamload == 0)
		{
		prfmsg(REPRJ);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair decoy launcher */
if (ptr->decload < 0)
	{
	++ptr->decload;
	if (ptr->decload == 0)
		{
		prfmsg(REPRD);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair zipper launcher */
if (ptr->zipload < 0)
	{
	++ptr->zipload;
	if (ptr->zipload == 0)
		{
		prfmsg(REPRZ);
		outprfge(ALWAYS,usrn);
		}
	return;
	}

/* repair mine launcher */
if (ptr->mineload < 0)
	{
	++ptr->mineload;
	if (ptr->mineload == 0)
		{
		prfmsg(REPRMN);
		outprfge(ALWAYS,usrn);
		}
	return;
	}
return;
}

void FUNC killem(ptr,usrn)
WARSHP	*ptr;
int	usrn;
{
WARSHP	*wptr;
WARUSR	*wuptr;
WARSHP	*disptr;
WARSHP	*nearptr;

unsigned i;
int who, comma, full, lospos, winpos, nearby;
long scr, amt, bonus1, bonus2, ded_amt;
double ddist;
unsigned int r = gernd();

/* 12/19/91 fix to prevent a player from being awarded points for killing */
/* himself */

waruptr=warusroff(usrn);

who = ptr->lastfired;

comma = FALSE;
full = FALSE;

if (who >= 0 && who < nships && who != usrn)
	{
	wptr= warshpoff(who);
	wuptr= warusroff(who);
	logthis(spr("Killed by %s",wptr->userid));
	if (wptr->status == GESTAT_AUTO)
		{
		if (shipclass[wptr->shpclass].won_func != NULL)
			shipclass[wptr->shpclass].won_func(wptr,who,ptr);
		}

	if (ptr->status == GESTAT_AUTO)
		prfmsg(KILLDNPC,username(ptr),username(wptr));
	else
		prfmsg(KILLEDBY,username(ptr),username(wptr));
	outwar(FILTER,usrn,0);

	++wuptr->kills;
	++wptr->kills;

	if (ptr->status == GESTAT_USER && wptr->status == GESTAT_USER)
		{
		++wuptr->ukills;
		++wptr->ukills;
		}

	if (ptr->status == GESTAT_AUTO)
		prfmsg(KILLGOTN,ptr->shipname);
	else
		prfmsg(KILLGOT1,ptr->shipname);

	if (shipclass[wptr->shpclass].max_tons <= calcweight(wptr))
		{
		full = TRUE;
		comma = TRUE;
		prf(" nothing");
		}
	else
		{
		/* get gold drop first, complete amount */
		amt = ptr->items[I_GOLD];
		if (amt > 0)
			{
			if (!chkweight(wptr,I_GOLD,amt))
				{
				amt = ((shipclass[wptr->shpclass].max_tons - calcweight(wptr))/((double)weight[I_GOLD]/100.0));
				full = TRUE;
				}
			if (amt > 0)
				{
				wptr->items[I_GOLD] += amt;
				sprintf(gechrbuf2,"%ld",amt);
				prf(" %s %s",gechrbuf2,item_name[I_GOLD]);
				comma = TRUE;
				}
			}
		/* get the rest except casualties, random amounts */
		for (i=1;i<NUMITEMS;++i)
			{
			if (full == TRUE)
				break;
			if (i != I_MEN && i != I_TROOPS && i != I_SPY && i != I_GOLD &&
				!(shipclass[ptr->shpclass].max_type == CLASSTYPE_CYBORG && i == I_FOOD))
				{
				amt = ptr->items[i] / (r%5 +1);
				/* only collect as much as we can hold */
				if (amt > 0)
					{
					if (!chkweight(wptr,i,amt))
						{
						amt = ((shipclass[wptr->shpclass].max_tons - calcweight(wptr))/((double)weight[i]/100.0));
						full = TRUE;
						}
					if (amt > 0)
						{
						wptr->items[i] += amt;
						sprintf(gechrbuf2,"%ld",amt);
						if (comma == TRUE)
							prf(", %s %s",gechrbuf2,item_name[i]);
						else
							{
							prf(" %s %s",gechrbuf2,item_name[i]);
							comma = TRUE;
							}
						}
					}
				}
			}
		}
	if (comma == FALSE)
		prf(" nothing.\r");
	else
		prf(".\r");

	if (full == TRUE)
		prfmsg(KILLFULL);

	outprfge(ALWAYS,who);

	/* grant points for the kill */
	scr = (long)shipclass[ptr->shpclass].max_points;
	bonus1 = 0;
	bonus2 = 0;

	if (ptr->status == GESTAT_USER && wptr->status == GESTAT_USER)
		{
		lospos = 0;
		winpos = 0;

		rospos(waruptr, wuptr, &lospos, &winpos);

		/* bonus for lower ranked taking out higher ranked */
		if (lospos != 0 && winpos > lospos && waruptr->score > wuptr->score)
			{
			bonus1 += ((winpos - lospos) * (long)score_bonus);
			bonus1 += ((waruptr->score - wuptr->score) / (long)score_bonus);
			}

		/* adjust bonus for taking out more/less powerful ship */
		if (shipclass[ptr->shpclass].damfact > (shipclass[wptr->shpclass].damfact + 50))
			bonus2 = scr / 2;
		else
		if (shipclass[ptr->shpclass].damfact < (shipclass[wptr->shpclass].damfact - 50))
			bonus2 = -((scr * 1L) / 3L);

		amt = scr + bonus1 + bonus2;
		if (amt < 0)
			amt = 0;

		ded_amt = (amt * score_f2)/100L;

		/* if loss exceeds total score, kill is worth nothing */
		if (ded_amt > waruptr->score)
			{
			ded_amt = 0;
			amt = 0;
			scr = 0;
			}
		}
	else
		{
		amt = scr;

		/* deduct less for losing to an NPC */
		ded_amt = (amt * score_f2)/1000L;
		}

	/* cap deduction to combat-earned score */
	if (ded_amt > waruptr->klscore)
		ded_amt = waruptr->klscore;

	waruptr->klscore -= ded_amt;
	waruptr->score -= ded_amt;

	if (ded_amt > 0)
		{
		sprintf(gechrbuf, "%lu", ded_amt);
		prfmsg(YRDEAD2, gechrbuf);
		outprfge(ALWAYS, usrn);
		}

	(wuptr->score) += amt;
	(wuptr->klscore) += amt;

	sprintf(gechrbuf,"%ld",scr);
	if (scr == 0)
		prfmsg(KILNOPTS);
	else
		{
		prfmsg(KILLPNTS,gechrbuf,shipclass[ptr->shpclass].typename);

		if (bonus2 > 0)
			{
			sprintf(gechrbuf,"%ld",bonus2);
			prfmsg(KILLBON1,gechrbuf);
			}
		else
		if (bonus2 < 0)
			{
			sprintf(gechrbuf,"%ld",(bonus2 * -1L));
			prfmsg(KILLBON2,gechrbuf);
			}

		if (bonus1 > 0)
			{
			sprintf(gechrbuf,"%ld",bonus1);
			prfmsg(KILLBON3,winpos,lospos,gechrbuf);
			}
		}

	outprfge(ALWAYS,who);

	if (scr > 0 && chgloser > 0
		&& ptr->status == GESTAT_USER
		&& wptr->status == GESTAT_USER)
		{
		amt = (waruptr->cash/100L)*(long)chgloser;
		if (amt > waruptr->cash)
			amt = waruptr->cash;


		if (amt > 0)
			{
			waruptr->cash -= amt;
			if (wuptr->cash > ULCAP - amt)
				{
				amt = ULCAP - wuptr->cash;
				sprintf(gechrbuf,"%lu",ULCAP);
				prfmsg(TOORICH,gechrbuf);
				outprfge(ALWAYS,who);
				}
			wuptr->cash += amt;
			sprintf(gechrbuf,"%ld",amt);
			prfmsg(CHGLSR1,gechrbuf);
			outprfge(ALWAYS,usrn);
			prfmsg(CHGLSR2,gechrbuf,ptr->userid);
			outprfge(ALWAYS,who);
			}

		}

	/* if the clown just killed was the last to fire on me clean out
		my last fired flag so as not to award him with any points should
		I end up getting killed */

	if (wptr->lastfired == usrn)
		wptr->lastfired = -1;

	/* distress handling */
	for (i=nterms;i<nships;++i)
		{
		if (ingegame(i))
			{
			disptr=warshpoff(i);
			if (disptr->distress == usrn)
				{
				amt = (shipclass[disptr->shpclass].max_points)*4;
				sprintf(gechrbuf,"%ld",amt);
				prfmsg(KILLDIS,gechrbuf,disptr->shipname);
				prfmsg(FACNAME0+shipclass[disptr->shpclass].faction);
				prf("\r");
				outprfge(ALWAYS,who);
				wuptr->score += amt;
				wuptr->klscore += amt;
				wuptr->factions[shipclass[disptr->shpclass].faction] = 0;
				disptr->distress = 255;
				}
			}
		}

	if (ptr->status == GESTAT_USER && wptr->status == GESTAT_USER
		&& showdoc != 0 && r%(11 - showdoc) == 0)
		{
		setbtv(gebb2);

		if (qlobtv(0) && qeqbtv(ptr->userid,1))
			{
			int bits = 1;
			if (waruptr->planets > 100)	/* if lots of planets, increase rnd threshold */
				bits = 4;
			else
			if (waruptr->planets > 50)
				bits = 2;
			prfmsg(CAPTDOC);
			prfmsg(PLAMSG1);
			i = 0;
			do
				{
				gcrbtv(&planet,1);
				if (sameas(planet.userid,ptr->userid))
					{
					if ((bits == 1 && (r & 1) == 1) ||	/* different random list each time */
						(bits == 2 && (r & 3) == 3) ||	/* over 50 planets, 1 in 4 chance to be included */
						(bits == 4 && (r & 7) == 7))	/* read 3 bits (1 in 8 chance) even though we shift 4 */
						{
						prf("%-24s %6d %6d  %6d\r",planet.name,planet.xsect,planet.ysect,planet.plnum);
						++i;
						if (i % 5 == 0)		/* cat five lines then print */
							outprfge(ALWAYS,who);
						}
					}
				else
					break;
				r >>= bits;
				if (r == 0)
					r = gernd();
				} while (qnxbtv() && (i < 20));
			if (i == 0)	/* oops, we didn't pick any planets, so print final planet */
				{
				qprbtv();
				gcrbtv(&planet,1);
				prf("%-24s %6d %6d  %6d\r",planet.name,planet.xsect,planet.ysect,planet.plnum);
				}
			if (i % 5 != 0)	/* if we're not on a multiple of 5, we still have to print the remainder */
				outprfge(ALWAYS,who);
			}
		}

	nearby = FALSE;
	for (i=0;i<nships;++i)
		{
		if (i != who && i != usrn && ingegame((int)i))
			{
			nearptr = warshpoff((int)i);
			if (nearptr->damage < 100.0)
				{
				ddist = cdistance(&wptr->coord,&nearptr->coord);
				ddist *= 10000.0;
				if (ddist < 30000.0)
					{
					nearby = TRUE;
					break;
					}
				}
			}
		}
	if (!nearby && wptr->cantexit > (FIRETICKS/4))
		wptr->cantexit = FIRETICKS/4;

	if (shipclass[wptr->shpclass].max_type != CLASSTYPE_DROID)
		geudb(GEUPDATE,wuptr->userid,wuptr);

	if (shipclass[ptr->shpclass].kill_func != NULL)
		shipclass[ptr->shpclass].kill_func(ptr,usrn,wptr);
	}
else
	{
	if (shipclass[ptr->shpclass].max_type != CLASSTYPE_USER)
		prfmsg(DIEDNPC,ptr->shipname);
	else
		prfmsg(DIED,ptr->shipname,username(ptr));
	outwar(ALWAYS,usrn,0);
	if (shipclass[ptr->shpclass].kill_func != NULL)
		shipclass[ptr->shpclass].kill_func(ptr,usrn,NULL);
	}


cleartm(usrn);	/* change destroyed user's torps and mis to 'no user' */

--(waruptr->noships);
/* fix any wrap problem */
if (waruptr->noships == 65535U)
	waruptr->noships = 0;

if (shipclass[ptr->shpclass].max_type != CLASSTYPE_DROID)
	{
	gepdb(GEDELETE,ptr->userid,ptr->shipno,ptr);
	geudb(GEUPDATE,waruptr->userid,waruptr);
	}

logthis(spr("GE:INF:%s died!",waruptr->userid));
}

/**************************************************************************
** Recharge energy pool                                                  **
**************************************************************************/

void FUNC recharge(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
	usrn = usrn; /* avoid the warning */
	if (ptr->energy < ENGYMAX)
		ptr->energy = ptr->energy + ENGRECHG;
	else
		ptr->energy = ENGYMAX;
}


/**************************************************************************
** Check flux status                                                     **
**************************************************************************/

int FUNC fluxstat(ptr,usrn,energy)

WARSHP	*ptr;
int	usrn;
unsigned energy;
{

if (ptr->energy < energy)
	{
	if (ptr->items[I_FLUXPOD] > 0)
		{
		ptr->energy = ENGYMAX;
		--ptr->items[I_FLUXPOD];
		prfmsg(FLUXLOAD);
		if (ptr->items[I_FLUXPOD] == 0)
			prfmsg(LASTFLUX);
		outprfge(ALWAYS,usrn);
		return(1);
		}
	else
		{
		prfmsg(NOFLUX);
		outprfge(ALWAYS,usrn);
		return(0);
		}
	}
else
return(1);
}


/**************************************************************************
** Check shield status                                                   **
**************************************************************************/

void FUNC shieldstat(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

if (ptr->shieldstat == SHIELDUP)
	{
	if (fluxstat(ptr,usrn,SHENGUSE * ptr->shieldtype) == 0)
		{
		ptr->shieldstat = SHIELDDN;
		ptr->shield = 0;
		prfmsg(SHDNNOP);
		outprfge(ALWAYS,usrn);
		}
	 else
		{
		shieldchg(ptr,usrn);
		}
	}
else
if (ptr->shieldstat == SHIELDDM)
	{
	shieldrep(ptr,usrn);
	}
}


/**************************************************************************
** Check cloak status                                                    **
**************************************************************************/

void FUNC cloakstat(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

if (ptr->cloak > 0 && ptr->cloak != 3)
	{
	if (fluxstat(ptr,usrn,clenguse) == 0)
		{
		ptr->cloak = 3;
		prfmsg(CLOKNOP);
		outprfge(ALWAYS,usrn);
		prfmsg(CLOK2);
		outrange(FILTER,&ptr->coord);
		}
	 else
		ptr->energy -= clenguse;
	 }
}

int FUNC isvisible(ptr,wptr)

WARSHP	*ptr;
WARSHP	*wptr;

{
double	ddist;
byte	ptr_neb,oth_neb;

if (wptr->cloak == 10)
	return(FALSE);

ptr_neb = (byte)innebula(coord1(ptr->coord.xcoord),coord1(ptr->coord.ycoord));
oth_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));

if (!(ptr_neb || oth_neb))
	return(TRUE);

ddist = cdistance(&ptr->coord,&wptr->coord) * 10000.0;
if (ptr_neb && oth_neb && ddist < (double)NEBRNG)
	return(TRUE);

return(FALSE);
}

/**************************************************************************
** Check mine status                                                     **
**************************************************************************/

void FUNC checkmines()
{
int	i;
int	zothusn;	/* general purpose other-user channel number */
WARSHP	*wptr;
WARUSR	*wuptr;
double	ddist, damfact;
unsigned udist;
byte	mine_neb,ship_neb;
MINE	*mptr;
setmbk(gemb);

/* reset per-ship mine proximity status each tick */
for (zothusn=0 ; zothusn < nships ; zothusn++)
	{
	wptr = warshpoff(zothusn);
	if (ingegame(zothusn))
		wptr->minesnear = FALSE;
	}

for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	if (mptr->channel != 255)	/* if a live mine */
		{
		--mptr->timer;
		if (mptr->timer%5 == 0)
			{
			mine_neb = (byte)innebula(coord1(mptr->coord.xcoord),coord1(mptr->coord.ycoord));
			for (zothusn=0 ; zothusn < nships ; zothusn++)
				{
				wptr=warshpoff(zothusn);
				if (ingegame(zothusn))
					{
					ddist = cdistance(&mptr->coord,&wptr->coord);
					ddist *= 10000;
					bearing = cbearing(&wptr->coord,&mptr->coord,wptr->heading);
					setsect(wptr);
					if (ddist < ((double)MINERANGE) && !neutral(&wptr->coord))
						{
						udist = (unsigned)ddist;
						if (mptr->timer == 0)
							{
							ddist = 1.0-(ddist/((double)MINERANGE));
							if (ddist < 0)
								ddist = 0;
							ddist = ddist*ddist;
							if (wptr->shieldstat == SHIELDUP)
								{
								damfact = (double)(ddist*minedammax);
								damfact = ton_fact(wptr,damfact); /* adjust for weight */
								damfact = damfact/(gernd()%5+wptr->shieldtype);
								damstr((int)damfact);
								prfmsg(MINE4,bearing,udist,gechrbuf);
								outprfge(FILTER,zothusn);
								shieldhit(wptr,zothusn,(int)damfact+20);
								}
							else
								{
								damfact = (double)(ddist*minedammax);
								damfact = ton_fact(wptr,damfact); /* adjust for weight */

								damstr((int)damfact);
								prfmsg(MINE4,bearing,udist,gechrbuf);
								outprfge(FILTER,zothusn);
								}
							wptr->damage += damfact;
							randamage(wptr,zothusn,damfact);
							/* don't set lastfired if NPC blows up its own kind or user blows up self */
							if ((shipclass[wptr->shpclass].faction != shipclass[warshpoff((int)mptr->channel)->shpclass].faction ||
								shipclass[wptr->shpclass].faction == 0 || shipclass[warshpoff((int)mptr->channel)->shpclass].faction == 0) &&
								zothusn != (int)mptr->channel)
								wptr->lastfired = (int)mptr->channel;
							wuptr = warusroff((int)mptr->channel);
							set_dislike(wuptr,shipclass[wptr->shpclass].faction,(int)damfact);
							wptr->minesnear = FALSE;
							/*DEBUG
							prf("MINE: chn # %d gets credit\r",wptr->lastfired);
							outprfge(ALWAYS,zothusn);*/
							}
						else
							{
							if (wptr->jam_sev <= (byte)2)
								{
								ship_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));
								if (!(mine_neb || ship_neb) || (mine_neb && ship_neb && ddist < (double)NEBRNG))
									{
									prfmsg(MINE6,bearing,udist);
									outprfge(FILTER,zothusn);
									}
								}
							wptr->minesnear = TRUE;
							}
						}
					else
					if (mptr->timer == 0 && wptr->jam_sev <= (byte)2)
						{
						ship_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));
						if (!(mine_neb || ship_neb) || (mine_neb && ship_neb && ddist < (double)NEBRNG))
							{
							prfmsg(MINE5,bearing);
							outprfge(FILTER,zothusn);
							}
						}
					}
				}
			}
		}
	if (mptr->timer == 0)
		mptr->channel = 255; /* stomp on it - HARD */
	}
}

/**************************************************************************
** Check for incoming torpedoes or missiles & track decoys               **
**************************************************************************/

void FUNC checktm(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
int	i,j,k,power,shotdown;
byte	track_cnt,lost_cnt,acc_used;
byte	acc_cnt[MAXTORPS],acc_chan[MAXTORPS];
byte	sh_cnt,un_cnt;
double	sh_dam,un_dam;

WARUSR				*wuptr;
MISSILE				*mptr;
TORPEDO				*tptr;
unsigned			*dptr;
double				damfact;
WARSHP				*sptr;
byte				ptr_neb,src_neb;
double				ndist;

/* flag hyper-phasers ready again */
if (ptr->hypha > 0)
	--(ptr->hypha);

if (ptr->jamload > 0)
	--(ptr->jamload);

if (ptr->zipload > 0)
	--(ptr->zipload);

if (ptr->mineload > 0)
	--(ptr->mineload);

if (ptr->decload > 0)
	--(ptr->decload);

ptr->torps_fired = 0;
ptr->missl_fired = 0;

/* knock down battle lock ticks */
if (ptr->cantexit > 0)
	--(ptr->cantexit);

ptr_neb = (byte)innebula(coord1(ptr->coord.xcoord),coord1(ptr->coord.ycoord));

/* torpedoes first */

shotdown = 0;
track_cnt = 0;
acc_used = 0;
sh_cnt = un_cnt = 0;
sh_dam = un_dam = 0.0;
for (i=0,tptr=ptr->ltorps;i<MAXTORPS;++i,++tptr)
	{
	if (tptr->distance > 0)
		{
		ptr->cantexit = FIRETICKS;
		if (tptr->distance >= 5000 && (int)tptr->channel < nships && ingegame((int)tptr->channel))
			{
			sptr = warshpoff((int)tptr->channel);
			src_neb = (byte)innebula(coord1(sptr->coord.xcoord),coord1(sptr->coord.ycoord));
			if (ptr_neb || src_neb)
				{
				ndist = cdistance(&ptr->coord,&sptr->coord) * 10000.0;
				if (!(ptr_neb && src_neb && ndist < (double)NEBRNG))
					{
					tptr->distance = 0;
					if ((int)tptr->channel < nterms)
						{
						prfmsg(TORMISS,shpltr(tptr->channel,usrn));
						outprfge(FILTER,tptr->channel);
						}
					continue;
					}
				}
			}
		if (neutral(&ptr->coord) && tptr->distance < 5000)
			{
			tptr->distance = 0;
			++shotdown;
			prfmsg(TORMISS,shpltr(tptr->channel,usrn));
			outprfge(FILTER,tptr->channel);
			}
		else
		if (tptr->distance <= torpsped)
			{
			tptr->distance = 0;
			if (ptr->shieldstat == SHIELDUP)
				{
				damfact = tdammax * rndm(.5);
				damfact = ton_fact(ptr,damfact); /* adjust for weight */

				ptr->damage += damfact;
				if ((int)tptr->channel < nships)
					{
					wuptr = warusroff((int)tptr->channel);
					set_dislike(wuptr,shipclass[ptr->shpclass].faction,(int)damfact);
					}
				if (++sh_cnt == 1)
					sh_dam = damfact;
				else
					sh_dam += damfact;
				for (k=0;k<acc_used;++k)
					if (acc_chan[k] == tptr->channel)
						break;
				if (k < acc_used)
					++acc_cnt[k];
				else
					{
					acc_chan[acc_used] = tptr->channel;
					acc_cnt[acc_used] = 1;
					++acc_used;
					}
				shieldhit(ptr,usrn,(gernd()%20)+10);
				}
			else
				{
				damfact = rndm(.5)+.5;
				damfact = tdammax * damfact;

				damfact = ton_fact(ptr,damfact); /* adjust for weight */

				ptr->damage += damfact;
				if ((int)tptr->channel < nships)
					{
					wuptr = warusroff((int)tptr->channel);
					set_dislike(wuptr,shipclass[ptr->shpclass].faction,(int)damfact);
					}
				if (++un_cnt == 1)
					un_dam = damfact;
				else
					un_dam += damfact;
				for (k=0;k<acc_used;++k)
					if (acc_chan[k] == tptr->channel)
						break;
				if (k < acc_used)
					++acc_cnt[k];
				else
					{
					acc_chan[acc_used] = tptr->channel;
					acc_cnt[acc_used] = 1;
					++acc_used;
					}
				}
			}
		else	/* still flying */
			{
			for (j=0,dptr=ptr->decout; j<MAXDECOY; ++j)
				{
				if (dptr[j] > 0)
					{
					if (tptr->distance < 5000 && (gernd()%decodds == 0))
						{
						prfmsg(TORDEST);
						outprfge(FILTER,usrn);
						if (tptr->channel < nterms)
							{
							prfmsg(TORDEST2);
							outprfge(FILTER,tptr->channel);
							}
						dptr[j] = 0;
						tptr->distance = 0;
						break;
						}
					}
				}
			if (tptr->distance > 0)	/* torp still here? */
				{
				tptr->distance -= torpsped;
				if (tptr->distance > 0)
					++track_cnt;
				}
			}
		}
	}

if (sh_cnt > 0)
	{
	damstr((int)sh_dam);
	if (sh_cnt == 1)
		prfmsg(THIT1,gechrbuf);
	else
		prfmsg(THIT3,sh_cnt,gechrbuf);
	outprfge(ALWAYS,usrn);
	}

if (un_cnt > 0)
	{
	damstr((int)un_dam);
	if (un_cnt == 1)
		prfmsg(THIT2,gechrbuf);
	else
		prfmsg(THIT4,un_cnt,gechrbuf);
	outprfge(ALWAYS,usrn);
	}

if ((sh_dam + un_dam) > 0.0)
	randamage(ptr,usrn,sh_dam + un_dam); /* combined torpedo random damage check */

if (shotdown > 0)
	{
	if (shotdown == 1)
		prfmsg(TORENF1);
	else
		prfmsg(TORENF,shotdown);
	outprfge(FILTER,usrn);
	}

if (track_cnt == 1)
	{
	prfmsg(TORP1);
	outprfge(FILTER,usrn);
	}
else
if (track_cnt > 1)
	{
	prfmsg(TORP4,track_cnt);
	outprfge(FILTER,usrn);
	}

for (k=0;k<acc_used;++k)
	acctm(ptr,usrn,0,acc_chan[k],acc_cnt[k]);

/* missiles second */
shotdown = 0;
track_cnt = 0;
lost_cnt = 0;
acc_used = 0;
sh_cnt = un_cnt = 0;
sh_dam = un_dam = 0.0;
for (i=0,mptr=ptr->lmissl;i<MAXMISSL;++i,++mptr)
	{
	if (mptr->distance > 0)
		{
		unsigned mstep;
		float menergy, mscale;

		ptr->cantexit = FIRETICKS;
		if (mptr->distance >= 5000 && (int)mptr->channel < nships && ingegame((int)mptr->channel))
			{
			sptr = warshpoff((int)mptr->channel);
			src_neb = (byte)innebula(coord1(sptr->coord.xcoord),coord1(sptr->coord.ycoord));
			if (ptr_neb || src_neb)
				{
				ndist = cdistance(&ptr->coord,&sptr->coord) * 10000.0;
				if (!(ptr_neb && src_neb && ndist < (double)NEBRNG))
					{
					mptr->distance = 0;
					++lost_cnt;
					if ((int)mptr->channel < nterms)
						{
						prfmsg(MISMISS,shpltr(mptr->channel,usrn));
						outprfge(FILTER,mptr->channel);
						}
					continue;
					}
				}
			}
		/* heavy missiles up to half speed, light missiles up to 2x speed */
		menergy = (float)mptr->energy + 300.0f;
		mscale = sqrt(5000.0f / menergy);
		if (mscale > 2.0f)
			mscale = 2.0f;
		if (mscale < 0.5f)
			mscale = 0.5f;
		mstep = (unsigned int)(mislsped * mscale);
		if (mstep == 0)
			mstep = 1;

		if (neutral(&ptr->coord) && mptr->distance < 5000)
			{
			mptr->distance = 0;
			++shotdown;
			prfmsg(MISMISS,shpltr(mptr->channel,usrn));
			outprfge(FILTER,mptr->channel);
			}
		else
		if (mptr->distance <= mstep)
			{
			mptr->distance = 0;
			/* reduce the energy by the damage factor of this ship */
			damfact = mptr->energy;
			damfact = ton_fact(ptr,damfact);
			mptr->energy = damfact;

			if (ptr->shieldstat == SHIELDUP)
				{
				damfact = damfact/20000.0;
				damfact = mdammax*(damfact * rndm(.1));
				ptr->damage += damfact;
				if (++sh_cnt == 1)
					sh_dam = damfact;
				else
					sh_dam += damfact;
				if ((int)mptr->channel < nships)
					{
					wuptr = warusroff((int)mptr->channel);
					set_dislike(wuptr,shipclass[ptr->shpclass].faction,(int)damfact);
					}
				for (k=0;k<acc_used;++k)
					if (acc_chan[k] == mptr->channel)
						break;
				if (k < acc_used)
					++acc_cnt[k];
				else
					{
					acc_chan[acc_used] = mptr->channel;
					acc_cnt[acc_used] = 1;
					++acc_used;
					}

				power = mptr->energy/999;
				power = power * (rndm(.5)+.5);
				shieldhit(ptr,usrn,power);
				}
			else
				{
				damfact = damfact/20000.0;
				damfact = mdammax*(damfact * (rndm(.5)+.5));
				ptr->damage += damfact;
				if (++un_cnt == 1)
					un_dam = damfact;
				else
					un_dam += damfact;
				if ((int)mptr->channel < nships)
					{
					wuptr = warusroff((int)mptr->channel);
					set_dislike(wuptr,shipclass[ptr->shpclass].faction,(int)damfact);
					}
				for (k=0;k<acc_used;++k)
					if (acc_chan[k] == mptr->channel)
						break;
				if (k < acc_used)
					++acc_cnt[k];
				else
					{
					acc_chan[acc_used] = mptr->channel;
					acc_cnt[acc_used] = 1;
					++acc_used;
					}
				}
			}
		else
			{
			for (j=0,dptr=ptr->decout; j<MAXDECOY; ++j)
				{
				if (dptr[j] > 0)
					{
					if (mptr->distance < 3000 && (gernd()%decodds == 0))
						{
						prfmsg(MISDEST);
						outprfge(FILTER,usrn);
						if (ingegame(mptr->channel) && mptr->channel < nterms)
							{
							prfmsg(MISDEST2);
							outprfge(FILTER,mptr->channel);
							}
						dptr[j] = 0;
						mptr->distance = 0;
						break;
						}
					}
				}
			if (mptr->distance > 0)	/* missl still here? */
				{
				mptr->distance -= mstep;
				if (ptr->speed > 100000.0 || mptr->distance > 50000U - (int)(ptr->speed/6.5) ||
					mptr->energy <= 500)
					{
					mptr->distance = 0;
					++lost_cnt;
					if (ingegame(mptr->channel) && mptr->channel < nterms)
						{
						prfmsg(MISMISS,shpltr(mptr->channel,usrn));
						outprfge(FILTER,mptr->channel);
						}
					}
				else
					{
					mptr->distance += (int)(ptr->speed/6.5);
					mptr->energy -= 500;	/* decrease energy over time */
					}
				if (mptr->distance > 0)
					++track_cnt;
				}
			}
		}
	}

if (lost_cnt == 1)
	{
	prfmsg(MISSL2);
	outprfge(FILTER,usrn);
	}
else
if (lost_cnt > 1)
	{
	prfmsg(MISSL4,lost_cnt);
	outprfge(FILTER,usrn);
	}

if (sh_cnt > 0)
	{
	damstr((int)sh_dam);
	if (sh_cnt == 1)
		prfmsg(MHIT1,gechrbuf);
	else
		prfmsg(MHIT3,sh_cnt,gechrbuf);
	outprfge(ALWAYS,usrn);
	}

if (un_cnt > 0)
	{
	damstr((int)un_dam);
	if (un_cnt == 1)
		prfmsg(MHIT2,gechrbuf);
	else
		prfmsg(MHIT4,un_cnt,gechrbuf);
	outprfge(ALWAYS,usrn);
	}

if ((sh_dam + un_dam) > 0.0)
	randamage(ptr,usrn,sh_dam + un_dam); /* combined missile random damage check */

for (k=0;k<acc_used;++k)
	acctm(ptr,usrn,1,acc_chan[k],acc_cnt[k]);

if (shotdown > 0)
	{
	if (shotdown == 1)
		prfmsg(MISENF1);
	else
		prfmsg(MISENF,shotdown);
	outprfge(FILTER,usrn);
	}

if (track_cnt == 1)
	{
	prfmsg(MISSL1);
	outprfge(FILTER,usrn);
	}
else
if (track_cnt > 1)
	{
	prfmsg(MISSL3,track_cnt);
	outprfge(FILTER,usrn);
	}

shotdown = 0;

/* finally decoys */

for (i=0,dptr=ptr->decout;i<MAXDECOY;++i)
	{
	if (dptr[i] > 0)
		{
		if (dptr[i] > 1)
			--dptr[i];
		else
		if (decpass == 0)
			{
			dptr[i] = 0;
			++shotdown;
			}
		}
	}
if (shotdown == 1)
	{
	prfmsg(DECGONE);
	outprfge(FILTER,usrn);
	}
else
if (shotdown > 1)
	{
	prfmsg(DECGONE2,shotdown);
	outprfge(FILTER,usrn);
	}
/* and Jammers too */
if (ptr->jam_time > (byte)0)
	{
	--ptr->jam_time;
	if (ptr->jam_time == (byte)0)
		{
		ptr->jam_sev = (byte)0;
		prfmsg(JAMMER5);
		outprfge(FILTER,usrn);
		}
	}
/* and cloak too */
if (ptr->cloak == 1)
	{
	ptr->cloak = 2;
	}
else
if (ptr->cloak == 2)
	{
	ptr->cloak = 10;
	prfmsg(CLOKUP,ptr->shipname);
	outprfge(ALWAYS,usrn);
	if (ptr->lock >= 0)
		{
		if (ptr->lock < nships && ingegame(ptr->lock))
			{
			prfmsg(LOCK04,shpltr(usrn,ptr->lock));
			outprfge(FILTER,ptr->lock);
			}
		ptr->lock = -1;
		prfmsg(LOCK01);
		outprfge(FILTER,usrn);
		}
	}
/* small weapon delay */
if (ptr->cloak == 3)
	{
	ptr->cloak = 0;
	prfmsg(CLOKW);
	outprfge(ALWAYS,usrn);
	}


/* clear lastfired if npc no longer exists */
if (ptr->lastfired >= 0 && ptr->lastfired < nships && warshpoff(ptr->lastfired)->status == GESTAT_AVAIL)
	ptr->lastfired = -1;

}

void FUNC validate_lock(ptr,usrn)
WARSHP	*ptr;
int	usrn;
{
WARSHP	*lptr;
double	dist;
int	lockee;
byte	locker_neb;
byte	lockee_neb;

if (ptr->lock < 0)
	return;

if (ptr->lock >= nships)
	{
	ptr->lock = -1;
	prfmsg(LOCK01);
	outprfge(FILTER,usrn);
	return;
	}

lockee = ptr->lock;
lptr = warshpoff(lockee);

if (!ingegame(lockee) || lptr->status == GESTAT_AVAIL)
	{
	ptr->lock = -1;
	return;
	}

if (lptr->cloak >= 10)
	{
	ptr->lock = -1;
	prfmsg(LOCK05,shpltr(lockee,usrn));
	outprfge(FILTER,usrn);
	prfmsg(LOCK04,shpltr(usrn,lockee));
	outprfge(FILTER,lockee);
	return;
	}

dist = cdistance(&ptr->coord,&lptr->coord) * 10000.0;
locker_neb = (byte)innebula(coord1(ptr->coord.xcoord),coord1(ptr->coord.ycoord));
lockee_neb = (byte)innebula(coord1(lptr->coord.xcoord),coord1(lptr->coord.ycoord));
if (locker_neb || lockee_neb)
	{
	if (!(locker_neb && lockee_neb && dist < (double)NEBRNG))
		{
		ptr->lock = -1;
		prfmsg(LOCK05,shpltr(lockee,usrn));
		outprfge(FILTER,usrn);
		prfmsg(LOCK04,shpltr(usrn,lockee));
		outprfge(FILTER,lockee);
		return;
		}
	}

if (dist > (double)shipclass[ptr->shpclass].scanrange)
	{
	ptr->lock = -1;
	prfmsg(LOCK05,shpltr(lockee,usrn));
	outprfge(FILTER,usrn);
	prfmsg(LOCK04,shpltr(usrn,lockee));
	outprfge(FILTER,lockee);
	return;
	}
}

void FUNC acctm(ptr,usrn,mt,channel,count)
WARSHP	*ptr;
int	usrn;
int	mt;
unsigned char channel;
int	count;

{

if (channel != 255)
	{
	if (count <= 1)
		{
		if (ptr->status == GESTAT_AUTO)
			prfmsg(MTACC1N+mt,shpltr(channel,usrn),ptr->shipname);
		else
			prfmsg(MTACC1+mt,shpltr(channel,usrn),ptr->shipname);
		}
	else
		{
		if (ptr->status == GESTAT_AUTO)
			prfmsg(MTACC3N+mt,count,shpltr(channel,usrn),ptr->shipname);
		else
			prfmsg(MTACC3+mt,count,shpltr(channel,usrn),ptr->shipname);
		}
	outprfge(ALWAYS,channel);
	ptr->lastfired = channel;
	}
else
	{
	ptr->lastfired = -1;
	}
}

int FUNC chkitm(usrn)
int	usrn;
{
WARSHP	*ptr;
int	i;

ptr=warshpoff(usrn);

for (i=0;i<MAXTORPS;++i)
	{
	if (ptr->ltorps[i].distance > 0)
		return(FALSE);
	}
for (i=0;i<MAXMISSL;++i)
	{
	if (ptr->lmissl[i].distance > 0)
		return(FALSE);
	}
return(TRUE);
}

void FUNC clearitm(usrn)
int	usrn;
{
WARSHP	*ptr;
int	i, first;

ptr=warshpoff(usrn);

first = TRUE;

for (i=0;i<MAXTORPS;++i)
	{
	if (ptr->ltorps[i].distance > 0)
		{
		ptr->ltorps[i].distance = 0;
		if (first == TRUE)
			{
			prfmsg(TORMISS2,shpltr(ptr->ltorps[i].channel,usrn));
			outprfge(FILTER,ptr->ltorps[i].channel);
			first = FALSE;
			}
		}
	}

first = TRUE;

for (i=0;i<MAXMISSL;++i)
	{
	if (ptr->lmissl[i].distance > 0)
		{
		ptr->lmissl[i].distance = 0;
		if (first == TRUE)
			{
			prfmsg(MISMISS2,shpltr(ptr->lmissl[i].channel,usrn));
			outprfge(FILTER,ptr->lmissl[i].channel);
			first = FALSE;
			}
		}
	}
}

void FUNC cleartm(channel)
int	channel;
{
WARSHP	*wptr;
int	j;
int	zothusn;

for (zothusn=0; zothusn < nships ; zothusn++)
	{
	if (zothusn != usrnum && ingegame(zothusn))
		{
		wptr=warshpoff(zothusn);
		for (j=0;j<MAXTORPS;++j)
			{
			if (wptr->ltorps[j].channel == (unsigned char)channel)
				{
				wptr->ltorps[j].channel = 255;
				}
			}
		for (j=0;j<MAXMISSL;++j)
			{
			if (wptr->lmissl[j].channel == (unsigned char)channel)
				{
				wptr->lmissl[j].channel = 255;
				}
			}
		}
	}
}


/**************************************************************************
** Check if the ION cannons need to be fired                             **
**************************************************************************/

void FUNC fireion(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

double	hitdam;

if (ptr->hostile > 1)
	{
	plnum = ptr->hostile - 10;
	getplanetdat(usrn);
	if (plptr->items[I_IONCANNON].qty > 0)
		{
		ptr->lastfired = -1;
		if (ptr->shieldstat == SHIELDUP)
			{
			hitdam = (idammax * rndm(.15));
			ptr->damage += hitdam;
			prfmsg(IHIT1);
			outprfge(ALWAYS,usrn);
			shieldhit(ptr,usrn,(gernd()%50)+40);
			}
		else
			{
			hitdam = (idammax * (rndm(.50) + .50));
			ptr->damage += hitdam;
			prfmsg(IHIT2);
			outprfge(ALWAYS,usrn);
			}
		randamage(ptr,usrn,hitdam);
		}
	}
}


/**************************************************************************
** Self Destruct countdown                                               **
**************************************************************************/

void FUNC destruct(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{
WARSHP	*wptr;
int	zothusn;
double	ddist;

if (ptr->destruct > (byte)0)
	{
	if (--ptr->destruct > (byte)0)
		{
		if (ptr->destruct==10)
			{
			prfmsg(SELFD2A,ptr->shipname);
			outrange(ALWAYS,&ptr->coord);
			}

		if (ptr->destruct==5)
			{
			prfmsg(SELFD2B,ptr->shipname);
			outrange(ALWAYS,&ptr->coord);
			}

		if (ptr->destruct==2)
			{
			prfmsg(SELFD2C,ptr->shipname);
			outrange(ALWAYS,&ptr->coord);
			}

		prfmsg(SELFD2,ptr->destruct);
		outprfge(ALWAYS,usrn);
		}
	else
		{
		prfmsg(SELFD3);
		ptr->damage = 101;
		outprfge(ALWAYS,usrn);
		prfmsg(SELFD3A,ptr->shipname);
		outrange(ALWAYS,&ptr->coord);
		if (ptr->shieldstat == SHIELDUP)
			{
			prfmsg(SELFD3S);
			outprfge(ALWAYS,usrn);
			}
		else
			for (zothusn=0 ; zothusn < nships ; zothusn++)
				{
				wptr=warshpoff(zothusn);
				if (ingegame(zothusn) && usrn != zothusn)
					{
					ddist = cdistance(&ptr->coord,&wptr->coord);
					ddist *= 10000;
					setsect(wptr);
					if (ddist < ((double)DESTRUCTRANGE) && !neutral(&wptr->coord))
						{
						ddist = 1.0-(ddist/((double)DESTRUCTRANGE));
						if (ddist < 0)
							ddist = 0;
						ddist = ddist*ddist*ddist;
						if (wptr->shieldstat == SHIELDUP)
							{
							damage = (unsigned)(ddist*minedammax);
							damage = damage*(shipclass[ptr->shpclass].damfact / 100);
							damage = damage/(gernd()%5+wptr->shieldtype);
							damstr(damage);
							prfmsg(SELFD6,gechrbuf);
							outprfge(ALWAYS,zothusn);
							shieldhit(wptr,zothusn,damage+20);
							}
						else
							{
							damage = (unsigned)(ddist*minedammax);
							damage = damage*(shipclass[ptr->shpclass].damfact / 100);
							damstr(damage);
							prfmsg(SELFD7,gechrbuf);
							outprfge(ALWAYS,zothusn);
							}
						wptr->damage += (double)damage;
						wptr->lastfired = -1;
						prfmsg(SELFD3N,gechrbuf,username(wptr));
						outprfge(ALWAYS,usrn);
						}
					}
				}
		}
	}
}

/**************************************************************************
** Verify percent for validaty                                           **
**************************************************************************/

int FUNC valpcnt(ptr,minnum,maxnum)
char	*ptr;
unsigned minnum,maxnum;
{
int	val;
char	*inpptr;

stripb(ptr);	/* BJ Changed */
if (inplen != 0)
	{
	for (inpptr = ptr; isdigit(*inpptr) ; inpptr++)
		{
		}
	if (*inpptr == '\0' || *inpptr == ' ')
		{
		if ((val=atoi(ptr)) >= minnum && val <= maxnum)
			{
			warsptr->percent = val;
			return(1);
			}
		}
	}
prfmsg(NUMOOR,minnum,maxnum);
outprfge(ALWAYS,usrnum);
return(0);
}

/**************************************************************************
** Verify degree for validity                                            **
**************************************************************************/

int FUNC valdegree(ptr)
char	*ptr;
{
int	val;

if (strlen(ptr) != 0)
	{
	val=atoi(ptr);
	if (val >= -180 && val <= 180)
		{
		warsptr->degrees = val;
		return(1);
		}
	}
prfmsg(NUMOOR,-180,180);
outprfge(ALWAYS,usrnum);
return(0);
}

/**************************************************************************
** Assess any random damage                                              **
**************************************************************************/

static int rd_item(WARSHP *ptr, unsigned int r, int itemnum, int damcomb)
{
unsigned int roll;
unsigned long qty, maxloss, have;
double frac;

have = ptr->items[itemnum];
if (!have)
	return 0;

frac = (double)damcomb;
frac = frac / (frac + (double)have / 100.0);
maxloss = (unsigned long)(have * frac);

if (maxloss > have)
	maxloss = have;

if (!maxloss)
	maxloss = 1;

roll = r % 100;
if (roll < 50)
	qty = 1 + (r % ((maxloss / 2) + 1));
else
if (roll < 80)
	qty = 1 + (r % (((3 * maxloss) / 4) + 1));
else
	qty = 1 + (r % maxloss);

if (have < 10 && qty >= have)
	qty = 1 + r % (2 + (have >> 1));

if (qty >= have)
	{
	if (have == 1)
		qty = 1;
	else
		qty = (have >> 1) + 1;
	}

if (qty > maxloss)
	qty = maxloss;

if (!qty)
	qty = 1;

ptr->items[itemnum] = have - qty;
return (qty > 32767UL) ? 32767 : (int)qty;
}

static void rd_append(char *buf, byte *comma, int qty, const char *sing, const char *plur)
{
if (qty <= 0)
	return;

if (*comma)
	sprintf(buf + strlen(buf), ", ");

sprintf(buf + strlen(buf), "%d %s", qty, qty == 1 ? sing : plur);
(*comma)++;
}

static int rd_add(WARSHP *ptr, unsigned int r, int itemnum, int damcomb,
	char *buf, byte *comma, const char *sing, const char *plur)
{
int qty = rd_item(ptr, r, itemnum, damcomb);

if (qty <= 0)
	return 0;

if (*comma)
	sprintf(buf + strlen(buf), ", ");

sprintf(buf + strlen(buf), "%d %s", qty, qty == 1 ? sing : plur);

(*comma)++;
return qty;
}

void FUNC randamage(ptr,usrn,hitdam)

WARSHP	*ptr;
int	usrn;
double	hitdam;
{
int	a, i, damcomb, qty, types, idx, item;
byte	comma = 0, doitems = 0, dosys = 0;
unsigned int r, r2;

gechrbuf[0] = '\0';

/* already dead */
if (ptr->damage >= 100.0)
	return;

damcomb = (int)(ptr->damage - hitdam);

/* hit must be over 5 and damage before hit must be over 20 */
if (hitdam < 5.0 || damcomb < 20)
	return;

/* weight toward big single hits */
damcomb = (int)((damcomb * 0.7) + (hitdam * (1.1 + hitdam * 0.025)));

/* cap to ensure that all 11 options are possible */
if (damcomb > 74)
	damcomb = 74;

r = gernd();
r2 = gernd();

doitems = r & 1;
dosys = (r >> 1) & 1;
if (!doitems && !dosys)
	{
	doitems = 1;
	dosys = 1;
	}

if (ptr->status == GESTAT_AUTO)
	{
	doitems = 0;
	if (!dosys)
		dosys = 1;
	}

a = r % (85 - damcomb);

if (a > 10)	/* no effect */
	return;

switch (a)
	{
	case 0: /* missiles */
		if (shipclass[ptr->shpclass].max_missl > 0)
			{
			if (dosys == 1 && ptr->mislcntl == 0)
				{
				ptr->mislcntl = 2 + r % (damcomb/3);
				prfmsg(RNDMISL);
				dosys = 2;
				}
			if (doitems == 1 && ptr->items[I_MISSILE] > 0)
				{
				qty = rd_item(ptr, r, I_MISSILE, damcomb);
				prfmsg(RNDITEM,qty,qty == 1 ? "missile was" : "missiles were");
				doitems = 2;
				}
			}
		break;

	case 1:	/* torpedoes */
		if (shipclass[ptr->shpclass].max_torps > 0)
			{
			if (dosys == 1 && ptr->torpcntl == 0)
				{
				ptr->torpcntl = 2 + r % (damcomb/3);
				prfmsg(RNDTORP);
				dosys = 2;
				}
			if (doitems == 1 && ptr->items[I_TORPEDO] > 0)
				{
				qty = rd_item(ptr, r, I_TORPEDO, damcomb);
				prfmsg(RNDITEM,qty,qty == 1 ? "torpedo was" : "torpedoes were");
				doitems = 2;
				}
			}
		break;

	case 2:	/* decoys */
		if (shipclass[ptr->shpclass].has_decoy > 0)
			{
			if (dosys == 1 && ptr->decload >= 0)
				{
				ptr->decload = -2 - r % (damcomb/3);
				prfmsg(RNDDECY);
				dosys = 2;
				}
			if (doitems == 1 && ptr->items[I_DECOYS] > 0)
				{
				qty = rd_item(ptr, r, I_DECOYS, damcomb);
				prfmsg(RNDITEM,qty,qty == 1 ? "decoy was" : "decoys were");
				doitems = 2;
				}
			}
		break;

	case 3:	/* zippers */
		if (shipclass[ptr->shpclass].has_zip > 0)
			{
			if (dosys == 1 && ptr->zipload >= 0)
				{
				ptr->zipload = -2 - r % (damcomb/3);
				prfmsg(RNDZIPR);
				dosys = 2;
				}
			if (doitems == 1 && ptr->items[I_ZIPPERS] > 0)
				{
				qty = rd_item(ptr, r, I_ZIPPERS, damcomb);
				prfmsg(RNDITEM,qty,qty == 1 ? "zipper was" : "zippers were");
				doitems = 2;
				}
			}
		break;

	case 4:	/* jammers */
		if (shipclass[ptr->shpclass].has_jam > 0)
			{
			if (dosys == 1 && ptr->jamload >= 0)
				{
				ptr->jamload = -2 - r % (damcomb/3);
				prfmsg(RNDJAMR);
				dosys = 2;
				}
			if (doitems == 1 && ptr->items[I_JAMMERS] > 0)
				{
				qty = rd_item(ptr, r, I_JAMMERS, damcomb);
				prfmsg(RNDITEM,qty,qty == 1 ? "jammer was" : "jammers were");
				doitems = 2;
				}
			}
		break;

	case 5:	/* mines */
		if (shipclass[ptr->shpclass].has_mine > 0)
			{
			if (dosys == 1 && ptr->mineload >= 0)
				{
				ptr->mineload = -2 - r % (damcomb/3);
				prfmsg(RNDMINE);
				dosys = 2;
				}
			if (doitems == 1 && ptr->items[I_MINE] > 0)
				{
				qty = rd_item(ptr, r, I_MINE, damcomb);
				prfmsg(RNDITEM,qty,qty == 1 ? "mine was" : "mines were");
				doitems = 2;
				}
			}
		break;

	case 6:	/* shields */
		if (shipclass[ptr->shpclass].max_shlds > 0 && ptr->shieldstat != SHIELDDM && dosys == 1)
			{
			prfmsg(SHDAMAG);
			outprfge(ALWAYS,usrn);
			ptr->shield = (int)(-2 - r % (damcomb/3));
			ptr->shieldstat = SHIELDDM;
			dosys = 2;
			}
		break;

	case 7:	/* phasers */
		if (shipclass[ptr->shpclass].max_phasr > 0 && ptr->phasr >= 0 && dosys == 1)
			{
			prfmsg(RNDPHSR);
			outprfge(ALWAYS,usrn);
			ptr->phasr = (int)(-2 - r % (damcomb/3));
			dosys = 2;
			}
		break;

	case 8:	/* cloak */
		if (shipclass[ptr->shpclass].max_cloak > 0 && ptr->cloak >= 0 && dosys == 1)
			{
			prfmsg(RNDCLOK);
			outprfge(ALWAYS,usrn);
			ptr->cloak = -2 - r % (damcomb/3);
			dosys = 2;
			}
		break;

	case 9:	/* scanners */
		if (ptr->tactical == 0 && dosys == 1)
			{
			prfmsg(RNDTACT);
			outprfge(ALWAYS,usrn);
			ptr->tactical = -2 - r % (damcomb/6);
			dosys = 2;
			}
		break;

	case 10: /* helm */
		if (ptr->helm == 0 && dosys == 1)
			{
			prfmsg(RNDNAVG);
			outprfge(ALWAYS,usrn);
			ptr->helm = -2 - r % (damcomb/9);
			dosys = 2;
			}
		break;

	default:
		break;
	}

/* if we didn't blow up a system or that system's items, and still need to do items */
if (doitems == 1 && dosys != 2)
	{
	a = r2 % 10;	/* 8 or 9 no effect */
	if (a == 4 || a == 5)	/* mess hall and head should happen less than the others */
		a = 0;
	if (a == 6 || a == 7)
		a = 1;

	switch (a)
		{
		case 0:	/* cargo bay */
			{
			byte allowed[NUMITEMS-4];
			int count = 0;

			prfmsg(RNDCRGO);

			for (i = 0; i < NUMITEMS; i++)
				{
				if (i == I_MEN || i == I_TROOPS || i == I_SPY || i == I_GOLD)
					continue;
				if (ptr->items[i] < 2)
					continue;
				allowed[count++] = (byte)i;
				}

			if (!count)		/* nothing to damage */
				break;

			types = 1 + (damcomb / 20);	/* how many types of items to damage */
			if (types > 4)
				types = 4;

			if (types > count)
				types = count;

			for (i = 0; i < types; ++i)
				{
				idx = (r2 >> (i*4)) % count; /* each 4 bits gives new entropy slice */
			        item = allowed[idx];

				/* swap-remove to prevent repeats without looping */
				allowed[idx] = allowed[--count];

				rd_add(ptr, r2 >> i, item, damcomb, gechrbuf, &comma, item_name[item], item_name[item]);
				}
			prfmsg(RNDITEM2, gechrbuf);
			}
			break;

		case 1:	/* living quarters */
			{
			int count = 0;

			prfmsg(RNDLVNG);

			if (ptr->items[I_MEN] > 0)
				count += rd_add(ptr, r2, I_MEN, damcomb, gechrbuf, &comma, "man", "men");
			if (ptr->items[I_TROOPS] > 0)
				count += rd_add(ptr, r2 >> 1, I_TROOPS, damcomb, gechrbuf, &comma, "troop", "troops");
			if (ptr->items[I_SPY] > 0)
				count += rd_add(ptr, r2 >> 2, I_SPY, damcomb, gechrbuf, &comma, "spy", "spies");

			if (!count)
				break;
			if (count == 1)
				sprintf(gechrbuf + strlen(gechrbuf), " was");
			else
				sprintf(gechrbuf + strlen(gechrbuf), " were");

			prfmsg(RNDITEM2,gechrbuf);
			}
			break;

		case 2:	/* head */
			{
			byte allowed[3];
			int count = 0;

			prfmsg(RNDHEAD);

			if (ptr->items[I_MEN] > 0)
				allowed[count++] = 0;
			if (ptr->items[I_TROOPS] > 0)
				allowed[count++] = 1;
			if (ptr->items[I_SPY] > 0)
				allowed[count++] = 2;

			if (count < 1)
				break;

			item = allowed[r2 % count];

			if (item == 0)
				{
				ptr->items[I_MEN]--;
				prfmsg(RNDITEM2,"1 man was");
				}
			if (item == 1)
				{
				ptr->items[I_TROOPS]--;
				prfmsg(RNDITEM2,"1 troop was");
				}
			if (item == 2)
				{
				ptr->items[I_SPY]--;
				prfmsg(RNDITEM2,"1 spy was");
				}
			}
			break;

		case 3:	/* mess hall */
			{
			byte allowed[3];
			int counter[3] = {0,0,0}, count = 0;
			prfmsg(RNDMESS);

			if (ptr->items[I_MEN] > 0)
				allowed[count++] = 0;
			if (ptr->items[I_TROOPS] > 0)
				allowed[count++] = 1;
			if (ptr->items[I_SPY] > 0)
				allowed[count++] = 2;

			if (count < 1)
				break;

			types = 1 + (r2 % 5);

			for (i = 0; i < types; ++i)
				{
				idx = (r2 >> (i * 3)) % count;  /* shift entropy slice a bit each time */
				item = allowed[idx];

				if (item == 0 && ptr->items[I_MEN] > 0)
					{
					ptr->items[I_MEN]--;
					counter[0]++;
					}
				else
				if (item == 1 && ptr->items[I_TROOPS] > 0)
					{
					ptr->items[I_TROOPS]--;
					counter[1]++;
					}
				else
				if (item == 2 && ptr->items[I_SPY] > 0)
					{
					ptr->items[I_SPY]--;
					counter[2]++;
					}
				}

			rd_append(gechrbuf, &comma, counter[0], "man", "men");
			rd_append(gechrbuf, &comma, counter[1], "troop", "troops");
			rd_append(gechrbuf, &comma, counter[2], "spy", "spies");

			qty = counter[0] + counter[1] + counter[2];

			if (!qty)
				break;
			if (qty == 1)
				sprintf(gechrbuf + strlen(gechrbuf), " was");
			else
				sprintf(gechrbuf + strlen(gechrbuf), " were");

			prfmsg(RNDITEM2,gechrbuf);
			}
			break;

		default:
			break;
		}
	}
outprfge(ALWAYS,usrn);
}

/**************************************************************************
** Determine the damage amount                                           **
**************************************************************************/

double FUNC pdamage(wptr,dist,foc)
WARSHP	*wptr;
double	dist;
int	foc;

{
double dd,fd,dp,factor,disfact,dam;

#ifdef MBBSEMU
int i;
double fractional;
#endif

if (wptr->where == 1)
	{
	factor = hpfirdst;
	dd = 1.0 - (dist / 40000.0);
	if (dd < 0.0)
		dd = 0.0;

#ifdef MBBSEMU
	dp = 1.0;
	if (factor > 0.0)
		{
		for (i = 0; i < (int)factor; ++i)
			dp *= dd;
		fractional = factor - (int)factor;
		if (fractional > 0.0 && dd > 0.0)
			dp *= 1.0 + fractional * (dd - 1.0);
		}
	else
		dp = 0.0;
#else
	dp = pow(dd,factor);
#endif
	dam = hpdammax * dp;
	}
else
	{
	factor = pfirdist;
	disfact = 20000.0 + ((double)wptr->phasrtype * 4000.0);
	dd = 1.0 - (dist/disfact);
	if (dd < 0.0)
		dd = 0.0;
	fd = 1.0 - ((double)foc / 11.0);

#ifdef MBBSEMU
	dp = 1.0;
	if (factor > 0.0)
		{
		for (i = 0; i < (int)factor; ++i)
			dp *= dd;
		fractional = factor - (int)factor;
		if (fractional > 0.0 && dd > 0.0)
			dp *= 1.0 + fractional * (dd - 1.0);
		}
	else
		{
		dp = 0.0;
		}
	dp *= (fd * fd) * (wptr->phasr / 100.0);
#else
	dp = (pow(dd,factor)) * (fd*fd) * (wptr->phasr/100.0);
#endif
	dam = pdammax * dp;
	}

logthis(spr("Pdamage %s %ld %d",wptr->userid,(long)dist,(int)dam));
return (dam);

}


/**************************************************************************
** set the xsect and ysect coordinates given a ship pntr                 **
**************************************************************************/

void FUNC setsect(ptr)
WARSHP	*ptr;
{
	xsect = coord1(ptr->coord.xcoord);
	ysect = coord1(ptr->coord.ycoord);
	xcord = coord2(ptr->coord.xcoord);
	ycord = coord2(ptr->coord.ycoord);
	pkey.xsect = xsect;
	pkey.ysect = ysect;
	pkey.plnum = 0;
}

/**************************************************************************
** move one coord to another (direction is <-- )                         **
**************************************************************************/

void FUNC movecoord(pointb, pointa)
COORD	*pointb, *pointa;
{

pointb->xcoord = pointa->xcoord;
pointb->ycoord = pointa->ycoord;

}

/**************************************************************************
** Compare two coords to determine if they are equal                     **
**************************************************************************/

int FUNC samesect(pointb, pointa)
COORD	*pointb, *pointa;
{
int	ax,ay,bx,by;
ax = coord1(pointa->xcoord);
ay = coord1(pointa->ycoord);
bx = coord1(pointb->xcoord);
by = coord1(pointb->ycoord);

return ((ax == bx) && (ay == by));
}


/**************************************************************************
** genearas function. Compare for length of element 1 only               **
**************************************************************************/

int FUNC genearas(str1,str2)
char	*str1,*str2;

{
return(sameto(str1,str2));
/*
return(!strnicmp(str1,str2,strlen(str1)));
 BJ CHANGED */
}


/**************************************************************************
** MAIL functions                                                        **
**************************************************************************/

int FUNC mailscan(userid,class)
char	*userid;
int	class;
{

strncpy(mailkey.userid,userid,UIDSIZ);
mailkey.class = class;

setbtv(gebb4);

/* if class = 0 scan if user has ANY mail */
if (class == 0)
	{
	if (qeqbtv(userid,0))
		{
		/* DEBUG
		prf("mail.userid=%s\rmail.class=%d\rmail.type=%d\r",mail.userid,mail.class,mail.type);*/
		return(TRUE);
		}
	}
else
/* otherwize see if he has this class of mail */
if (qeqbtv(&mailkey,1))
	{
	return(TRUE);
	}
return(FALSE);
}

int mailread(userid,class)
char	*userid;
{

strncpy(mailkey.userid,userid,UIDSIZ);
mailkey.class = class;
mailkey.msgno = 0;

setbtv(gebb4);

setmem(gemsg,FIXEDMSGSIZ,0);

if (qeqbtv(&mailkey,1))
	{
	gcrbtv(gemsg,1);
	prf("%s------------------------------------------------------------------------------%s\r",CLR_BLUE2,CLR_CYAN2);
	prf(gemsg->text);
	prf("%s------------------------------------------------------------------------------%s",CLR_BLUE2,CLR_WHITE2);
	outprfge(ALWAYS,usrnum);

	delbtv();

	return(TRUE);
	}


prfmsg(MAIL1);
outprfge(ALWAYS,usrnum);
return(FALSE);
}


void FUNC mailit(flag)
int	flag;
{
int	i;

setmbk(gemb);
clrprf();

if (flag == 1)
	{
	if (instat(mail.userid,gestt))
		{
		if (othusp->substt >= FIGHTSUB)
			{
			return;
			}
		}
	}

mail.stamp = cofdat(today());
sprintf(mail.dtime,"%s - %.5s",ncedat(today()),nctime(now()));

prfmsg(MAIL2,mail.dtime,mail.userid);

switch (mail.type)
	{
	case MESG02:
	case MESG03:
	case MESG04:
	case MESG05:
		strcpy(mail.topic,"Distress Message");
		sprintf(gechrbuf,"%ld",mail.long1);
		prfmsg(mail.type,mail.name1,mail.int1,mail.int2,mail.string1,mail.name2,gechrbuf);
		sendit();
		break;

	case MESG06:
	case MESG07:
		strcpy(mail.topic,"Distress Message");
		sprintf(gechrbuf,"%ld",mail.long1);
		prfmsg(mail.type,mail.name1,mail.int1,mail.int2,gechrbuf);
		sendit();
		break;

	case MESG08:
	case MESG09:
	case MESG10:
	case MESG11:
	case MESG12:
	case MESG13:
	case MESG14:
	case MESG15:
	case MESG16:
	case MESG17:
	case MESG18:
	case MESG19:
	case MESG19A:
	case MESG19B:
	case MESG30:
		strcpy(mail.topic,"Status Message");
		sprintf(gechrbuf,"%ld",mail.long1);
		prfmsg(mail.type,mail.name1,mail.int1,mail.int2,gechrbuf);
		sendit();
		break;

	case MESG20:

		strcpy(mail.topic,"Production Report");
		memcpy(&tmpstat,&mail,sizeof(MAILSTAT));
		prfmsg(tmpstat.type,tmpstat.name1,tmpstat.int1,tmpstat.int2);
		sprintf(gechrbuf2,"%lu",tmpstat.cash);
		prf("Cash %s  ",gechrbuf2);
		sprintf(gechrbuf2,"%lu",tmpstat.tax);
		prf("Tax %s  \r",gechrbuf2);
		for(i=0;i<NUMITEMS;++i)
			{
			setmem(gechrbuf,20,'.');
			gechrbuf[20-strlen(item_name[i])]=0;
			sprintf(gechrbuf2,"%ld",tmpstat.itemqty[i]);
			prf("%s%s%14s\r",item_name[i],gechrbuf,gechrbuf2);
			}
		sendit();
		break;

	default:
		prfmsg(MAIL3,mail.type);
		sendit();
		break;

	}

rstmbk();
}

int FUNC sendit()
{

/* don't send mail to non-live players */

if (mail.userid[0] == '*')
	return(FALSE);

setmem(gemsg,FIXEDMSGSIZ,0);

gemsg->msgno = 0;

strcpy(gemsg->userto,mail.userid);
strcpy(gemsg->from,"** Galactic Empire **");
strcpy(gemsg->to,mail.userid);
strcpy(gemsg->topic,mail.topic);
gemsg->auxtpc[0] = 0;

gemsg->flags = mail.class;

gemsg->crdate=today();
gemsg->crtime=now();

gemsg->nreply = cofdat(today());

prf2tx();

return(sendgemsg(gemsg,mail.userid));

}

void FUNC prf2tx(void)		/* xfer prfbuf contents to message text area */
{
char *cp;

stpans(prfbuf);
if (strlen(prfbuf) >= GEMSGSIZ)
	{
	prfbuf[GEMSGSIZ-1]='\0';
	}
for (cp=prfbuf ; *cp != '\0' ; cp++)
	{
	if (*cp == '\n')
		{
		*cp='\r';
		}
	}
strcpy(gemsg->text,prfbuf);
clrprf();
}


int FUNC sendgemsg(struct message *msgptr,char *usrid)

{

usrid=usrid;

setbtv(gebb4);

dinsbtv(msgptr);

rstbtv();

return(TRUE);

}

/**************************************************************************
** Shield functions                                                      **
**************************************************************************/

void FUNC shieldup(wptr,usrn)
WARSHP	*wptr;
int	usrn;
{
prfmsg(SHLDCHP);
outprfge(FILTER,usrn);
wptr->shieldstat = SHIELDUP;
}


void FUNC shielddn(wptr,usrn)
WARSHP	*wptr;
int	usrn;
{
usrn = usrn; /* avoid the warning */
prfmsg(SHLDDN);
outprfge(FILTER,usrn);
wptr->shieldstat = SHIELDDN;
}


void FUNC shieldhit(wptr,usrn,dam)
WARSHP	*wptr;
int	usrn;
int	dam;   /* 0% to 100% damage */
{
int	knock;

double	dmax, ddam;

dmax = (double)( 80 - ((int)wptr->shieldtype * SHIELD_FACTOR));

if (dmax < 0)
	dmax = 0;

ddam = dam;
ddam /=100;	/* make it a percentile */

knock = (int)(dmax * ddam);

wptr->shield -= knock;
if (wptr->shield <=2 )
	{
	prfmsg(SHDAMAG);
	outprfge(ALWAYS,usrn);
	wptr->shieldstat = SHIELDDM;
	wptr->shield -= (knock*3);
	}
else
if (wptr->shield < SHMINCHG )
	{
	prfmsg(SHKNKDN);
	outprfge(ALWAYS,usrn);
	}
}


void FUNC shieldrep(wptr,usrn)
WARSHP	*wptr;
int	usrn;
{
wptr->shield += (int)(wptr->shieldtype);

if (wptr->shield > 0)
	{
	wptr->shieldstat = SHIELDDN;
	wptr->shield = 0;
	prfmsg(SHREPR);
	outprfge(ALWAYS,usrn);
	}
}

void FUNC shieldchg(wptr,usrn)
WARSHP	*wptr;
int	usrn;
{
/*STATIC*/
static int 	maxcharge;
static int 	pcnt;
int		type;

type = wptr->shieldtype;

wptr->energy -=  (type*SHENGUSE);

charge(wptr,&maxcharge,&pcnt); /* go figure maxcharge and percent */

if (wptr->shield < maxcharge)
	{
	wptr->shield += (type*3);
	if (wptr->shield >= maxcharge)
		{
		wptr->shield = maxcharge;
		prfmsg(SHLDUP);
		}
	else
		{
		charge(wptr,&maxcharge,&pcnt); /* go figure maxcharge and percent */
		prfmsg(SHLDAT,pcnt);
		}
	outprfge(FILTER,usrn);
	}
}

/* calculate the relative charge the shields are at */

void FUNC charge(wptr,max,pct)
WARSHP	*wptr;
int	*max;
int	*pct;
{
*max = 40 + (wptr->shieldtype*10);
*pct = (wptr->shield*100)/(*max);
}

/* check if the goods to be added will cause wieght to be exceeded */

int FUNC chkweight(wptr,itm,amt)
WARSHP	*wptr;
int	itm;
long	amt;

{
int	i;
double	total = 0.0;

for (i=0; i<NUMITEMS; ++i)
	{
	total += ((double)wptr->items[i]*((double)weight[i]/100.0));
	}
total += ((double)amt*(double)weight[itm]/100);

return ((total <= (double)shipclass[wptr->shpclass].max_tons)
		&& (wptr->items[itm] <= ULCAP - amt));
}

/* tell the total weight on board */

unsigned long FUNC calcweight(wptr)
WARSHP	*wptr;
{
int	i;
double	totald = 0.0;
unsigned long total = 0;

for (i=0; i<NUMITEMS; ++i)
	{
	totald += (wptr->items[i]*((double)weight[i]/100L));
	}

total = (unsigned long)ceil(totald);
return (total);
}


/* Figure the ship letter for this user */

char FUNC shpltr(usrn,ship)
int	usrn,ship;
{
int	i;
SCANTAB	*sptr;

sptr = &scantab[usrn];

for (i=0; i<NOSCANTAB; ++i)
	{
	if (sptr->ship[i].shipno == ship)
		return(sptr->ship[i].letter);
	}

/* not found, update scantab */
update_scantab(warshpoff(usrn), usrn);

/* try again */
for (i=0; i<NOSCANTAB; ++i)
	{
	if (sptr->ship[i].shipno == ship)
		return(sptr->ship[i].letter);
	}

/* still not found, too many ships */
return('?');
}

/* return the proper name for this user given a pointer to the ship */

char * FUNC username(ptr)
WARSHP	*ptr;
{
if (shipclass[ptr->shpclass].max_type == CLASSTYPE_CYBORG)  /* CYBORG */
	return(ptr->shipname);
if (shipclass[ptr->shpclass].max_type == CLASSTYPE_DROID)  /* DROID */
	return(ptr->shipname);
return(ptr->userid);
}

/* data logger */

void FUNC logthis(str)
char	*str;

{

FILE	*hdl;
int	idate,itime;
char	*c_date,*c_time;



if (!logflag)
	return;

hdl = fopen("mpogeout.log","at+");

if(hdl != (FILE *)0)
	{
	idate = today();
	itime = now();
	c_date = ncdate(idate);
	c_time = nctime(itime);

	fprintf(hdl,"[%s %s] %s\r",c_date,c_time,str);
	fclose(hdl);
	}

return;
}

WARUSR	* FUNC warusroff(int	usrn)
{
if(usrn >= 0 && usrn < nships)
	return((WARUSR *)((long)(warusr_ecl+(usrn<<3))<<16));
else
	{
	geshocst(0,spr("GE:WARUSROFF:bad usrn [%d]",usrn));
	return((WARUSR *)((long)0));
	}
}

WARSHP	* FUNC warshpoff(int usrn)
{
if(usrn >= 0 && usrn < nships)
	return((WARSHP *)((long)(warshp_ecl+(usrn<<3))<<16));
else
	{
	geshocst(0,"GE:BAD WARSHPOFF CALL");
	logthis(spr("WARSHPOFF:bad usrn [%d]",usrn));
	return((WARSHP *)((long)0));
	}
}

double FUNC ton_fact(ptr,damfact)
WARSHP	*ptr;
double	damfact;

{

double	temp;

temp = damfact / ((double)(shipclass[ptr->shpclass].damfact)/100.0);

return(temp);
}

char * FUNC showarp(double speed)
{
if (speed == 0.0)
	sprintf(warpbuf,"0.00");
else
#ifdef MBBSEMU
if (fabs(speed - (long)(speed / FARSPEED) * FARSPEED) < 1e-6)
#else
if (fmod(speed, FARSPEED) == 0.0)
#endif
	sprintf(warpbuf,"??.??");
else
	sprintf(warpbuf,"%.2f",speed/1000.0);
#ifdef MBBSEMU
	/* MBBSemu doesn't currently honor %.2f */
	if (warpbuf[strlen(warpbuf) - 2] == '.')
		strcat(warpbuf, "0");
	else
	if (warpbuf[strlen(warpbuf) - 3] != '.')
		strcat(warpbuf, ".00");
#endif
return(warpbuf);
}

/* assign closest cybs to ship entering game or going through wormhole */

void FUNC assign_cybs(usrnum,call)
int usrnum, call;

{

WARSHP *wptr;

WARSHP *ptr;
int zothusn;

double	ddist;

double	low_dist = 999999999.0;
int	low_ship;
int	lta; /* lowest to attack */
int	i, cybpick, claims;

claims = 0;

/* call 0 = clear all current cyb pursuits */
/* call 1 = count all current cyb pursuits */
for (zothusn=nterms; zothusn < nships; ++zothusn)
	{
	ptr = warshpoff(zothusn);
	if (ptr->status == GESTAT_AUTO && shipclass[ptr->shpclass].max_type == CLASSTYPE_CYBORG && ptr->cybmine == usrnum)
		{
		if (call == 0)
			ptr->cybmine = 255;
		else
			++claims;
		}
	}

wptr = warshpoff(usrnum);
/* if we're not clearing, only claim enough to fill claims */
cybpick = shipclass[wptr->shpclass].noclaim-claims;

for (i=0; i < cybpick; ++i)
	{
	low_dist = 999999999.0;
	low_ship = -1;

	for (zothusn=nterms; zothusn < nships; ++zothusn)
		{
		ptr=warshpoff(zothusn);
		lta = shipclass[ptr->shpclass].lowest_to_attk-1;
		if (ingegame(zothusn) && ptr->status == GESTAT_AUTO && shipclass[ptr->shpclass].max_accel > 0 &&
			shipclass[ptr->shpclass].max_type == CLASSTYPE_CYBORG && lta <= wptr->shpclass && ptr->cybmine == 255)
			{
			ddist = cdistance(&ptr->coord,&wptr->coord);
			if (ddist < low_dist)
				{
				low_dist = ddist;
				low_ship = zothusn;
				}
			}
		}
		if (low_ship == -1)
			return;

		ptr=warshpoff(low_ship);
		ptr->cybmine = usrnum;
	}
}

/* is cyb in fast pursuit? */

int FUNC cyb_fast(ptr)
WARSHP *ptr;

{
return (ptr->speed != 0.0) &&
#ifdef MBBSEMU
	(fabs(ptr->speed - (long)(ptr->speed / FARSPEED) * FARSPEED) < 1e-6);
#else
	(fmod(ptr->speed, FARSPEED) == 0.0);
#endif
}

/* set faction dislike status */

void FUNC set_dislike(wuptr,facnum,dislike)
WARUSR *wuptr;
int facnum, dislike;

{
if (facnum < 0 || facnum > 7)
	{
	geshocst(0,spr("GE:set_dislike:bad facnum [%d]",facnum));
	return;
	}

if ((unsigned int)(wuptr->factions[facnum]) + dislike > 255)
	wuptr->factions[facnum] = 255;
else
	wuptr->factions[facnum] += dislike;
}

void FUNC rospos(WARUSR *losptr, WARUSR *winptr, int *lospos, int *winpos)
{
long ltarget = 0, wtarget = 0;
int i = 0;
int ranked;

*lospos = 0;
*winpos = 0;

setbtv(gebb5);

/* find user record for loser */
if (qeqbtv(losptr->userid, 0))
	ltarget = absbtv();

/* find user record for winner */
if (qeqbtv(winptr->userid, 0))
	wtarget = absbtv();

if (ltarget == 0 && wtarget == 0)
	return;	/* return both 0 */

/* start at top of roster (key 1 = score order) */
if (!qhibtv(1))
	return;

do
	{
	gcrbtv(&tmpusr, 1);

	ranked = (tmpusr.score > 0 && tmpusr.userid[0] != '@');

	if (ranked)
		++i;

	if (ranked && ltarget !=0 && absbtv() == ltarget)
		*lospos = i;
	else
	if (ranked && wtarget !=0 && absbtv() == wtarget)
		*winpos = i;

	if (*lospos && *winpos)
		break;	/* we're done here */

	} while (qprbtv());

if (*winpos == 0)	/* unranked winner */
	*winpos = i+1;

}

void FUNC damstr(damage)
int damage;

{
if (damage == 0)
	strcpy(gechrbuf,"no");
else
if (damage < 2)
	strcpy(gechrbuf,"negligible");
else
if (damage < 12)
	strcpy(gechrbuf,"very light");
else
if (damage < 25)
	strcpy(gechrbuf,"light");
else
if (damage < 50)
	strcpy(gechrbuf,"moderate");
else
if (damage < 75)
	strcpy(gechrbuf,"heavy");
else
	strcpy(gechrbuf,"severe");
}

void FUNC update_scantab(ptr, usrn)
WARSHP	*ptr;
int	usrn;
{
int	i,j;
char	l;
byte	ptr_neb,oth_neb;
WARSHP	*wptr;
SCANTAB	tmp;

char	lettab[300];

setmem(&lettab[0],sizeof(char)*300,0);

ptr_neb = (byte)innebula(coord1(ptr->coord.xcoord),coord1(ptr->coord.ycoord));

/* save off the old letters */
for (i=0;i<NOSCANTAB;++i)
	{
	j = scantab[usrn].ship[i].shipno;
	if (j>= 0 && j < 300)
		{
		/* only keep A through Z */
		l = scantab[usrn].ship[i].letter;
		lettab[j] = (l >= 'A' && l <= 'Z') ? l : 0;
		}
	}

/* clear the table */
for (i=0;i<NOSCANTAB;++i)
	tmp.ship[i].flag = 0;

for (othusn=0 ; othusn < nships ; othusn++)
	{
	if (othusn != usrn && ingegame(othusn))
		{

		wptr=warshpoff(othusn);
		ddistance = cdistance(&ptr->coord,&wptr->coord)*10000;
		oth_neb = (byte)innebula(coord1(wptr->coord.xcoord),coord1(wptr->coord.ycoord));
		if ((ptr_neb || oth_neb) && !(ptr_neb && oth_neb && ddistance < (double)NEBRNG))
			continue;
		if (ddistance < shipclass[ptr->shpclass].scanrange
			&& wptr->cloak < 10)
			{
			for (i=0;i<NOSCANTAB;++i)
				{
				/* entry blank - fill it with this one */
				if (tmp.ship[i].flag == 0)
					{
					tmp.ship[i].flag = 1;
					tmp.ship[i].shipno = othusn;
					tmp.ship[i].dist = ddistance;
					tmp.ship[i].letter = lettab[othusn];

					break;
					}
				/* is this ship closer */
				if (ddistance < tmp.ship[i].dist)
					{
					/* Yes - Push down the rest */
					for (j=NOSCANTAB-2;j>=i;j--)
						{
						if (tmp.ship[j].flag == 1)
							{
							memcpy(&tmp.ship[j+1],&tmp.ship[j],sizeof(SHIPTAB));
							}
						}
					/* fill the hole with the new ship */
					tmp.ship[i].flag = 1;
					tmp.ship[i].shipno = othusn;
					tmp.ship[i].dist = ddistance;
					tmp.ship[i].letter = lettab[othusn];
					break;
					}
				}
			}
		}
	}

/* go fill in the new ships without letters */

pick_letter(&tmp);

/* Now go fill in the rest of the table */

for (i=0;i<NOSCANTAB;++i)
	{
	/* clear out the empty entries */
	if (tmp.ship[i].flag == 0)
		{
		tmp.ship[i].letter = '?';
		tmp.ship[i].shipno = -1;
		}
	else
		{
		j = tmp.ship[i].shipno;
		wptr=warshpoff(j);
		tmp.ship[i].bearing = cbearing(&ptr->coord,&(wptr->coord),ptr->heading);
		tmp.ship[i].heading = cbearing(&(wptr->coord),&ptr->coord,wptr->heading);
		tmp.ship[i].speed = wptr->speed;
		}
	}

/* update the current users master record */
memcpy(&scantab[usrn],&tmp,sizeof(SCANTAB));

}

void FUNC pick_letter(ptr)
SCANTAB *ptr;
{
#define LETSIZE 26
char letters[LETSIZE] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                         'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char lettmp[LETSIZE];
int i,j;


/* init the temp tab */
memcpy(lettmp,letters,LETSIZE);

/* look at each ship in the table and punch out the letter from the
   list... when all done the letters remaining are available */

/* DEBUG - REMOVE THIS WHEN DONE
for (i=0;i<NOSCANTAB;++i)
	logthis(spr("PLTR-A:%d flg=%d shipno=%d letter=%d/%c",i,ptr->ship[i].flag,ptr->ship[i].shipno,ptr->ship[i].letter,ptr->ship[i].letter));*/

for (i=0;i<NOSCANTAB;++i)
	{
	if (ptr->ship[i].flag == 1 && ptr->ship[i].letter != 0)
		{
		for (j=0;j<LETSIZE;++j)
			{
			if (ptr->ship[i].letter == lettmp[j])
				{
				lettmp[j] = '@';
				break;
				}
			}
		}
	}

/* now go through and fix up the ? */

for (i=0;i<NOSCANTAB;++i)
	{
	if (ptr->ship[i].flag == 1 && ptr->ship[i].letter == 0)
		{
		for (j=0;j<LETSIZE;++j)
			{
			if (lettmp[j] != '@')
				{
				ptr->ship[i].letter = lettmp[j];
				lettmp[j] = '@';
				break;
				}
			}
		}
	}
/* DEBUG - REMOVE THIS WHEN DONE
for (i=0;i<NOSCANTAB;++i)
	logthis(spr("PLTR-Z:%d flg=%d shipno=%d letter=%d/%c",i,ptr->ship[i].flag,ptr->ship[i].shipno,ptr->ship[i].letter,ptr->ship[i].letter));*/

return;
}
