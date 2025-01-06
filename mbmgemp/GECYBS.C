
/***************************************************************************
 *                                                                         *
 *   GECYBS.C                                                              *
 *                                                                         *
 *   Copyright (C) 1988, 89, 90, 91, 92 Michael B. Murdock                 *
 *                                                                         *
 *   This is the source for the Galactic Empire game module                *
 *                                                                         *
 *                                         M. Murdock     03/17/92         *
 *                                                                         *
 *   ge-next                                                               *
 *                                                                         *
 *   Copyright (C) 2024-2025 Anthony Schmidt, anthony@manicpop.org         *
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

#else

#include "stdio.h"
#include "ctype.h"
#include "dos.h"
#include "usracc.h"
#include "btvstf.h"
#include "stdlib.h"
#include "math.h"
#include "portable.h"
#include "dosface.h"
#endif

#include "majorbbs.h"
#include "message.h"

#include "gemain.h"


#define  GECYBS   1

#include "geglobal.h"

/* LOCAL GLOBAL DEFS ****************************************************/

char	cybname[UIDSIZ]; /*BJ REMOVED CYBORG- name*/
int	cybhaltflg = 0;


double	d_topspeed;

/**************************************************************************
** Cyborg functions                                                      **
**************************************************************************/

void  FUNC cyb_init(ptr, usrn, class)
WARSHP *ptr;
int	usrn;
int	class;

{

WARSHP *wptr;

int	i, goldwin, goldspin, goldtry, zothusn;
double	ddist;

logthis(spr("@Cyb_init usrn=%d,class=%d",usrn,class));


if (usrn < 0 || usrn >= nships)
	{
	logthis(spr("CYB_INIT:bad usrn [%d]",usrn));
	return;
	}

strncpy(cybname,"@Cybrg-",UIDSIZ);/* Bj Added name here */
sprintf(&cybname[7],"%d",usrn);

if (!(geudb(GELOOKUP,cybname, &tmpusr)))
	{
	initusr(cybname);
	geudb(GEADD,tmpusr.userid,&tmpusr);
	logthis(spr("GE:INF:Adding %s user",tmpusr.userid));
	}

waruptr = warusroff(usrn);
warsptr = warshpoff(usrn);

if (geudb(GELOOKUP,cybname, waruptr))
	{
	geudb(GEGET,cybname, waruptr);

	logthis(spr("GE:INF:Load %s user",waruptr->userid));

	if (gepdb(GELOOKUPNAME,cybname,0,ptr))
		{
		gcrbtv(ptr,0);
		logthis(spr("GE:INF:Load %s ship",ptr->userid));

		ptr->status = GESTAT_AUTO;
		ptr->shield = 40 + (ptr->shieldtype*10);
		ptr->phasr = 100;
		ptr->cybmine = (byte)255;
		cyb_cruise(ptr);
		ptr->cybupdate = 100 + gernd()%20;
		ptr->holdcourse = 0;
		ptr->tick = CYBTICKTIME + gernd()%(CYBTICKTIME*5);

		/* SANITY CHECK */
		if (shipclass[ptr->shpclass].max_type != CLASSTYPE_CYBORG)
			{
			geshocst(0,spr("GE:ERR:NOTCYBCLS %d",ptr->shpclass));
			}
		}
	else
		{
		/* make me a Cybertron */
		logthis(spr("GE:INF:Adding %s ship - %d",ptr->userid,class));

		initshp(cybname,class);
		gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
		memcpy(ptr,&tmpshp,sizeof(WARSHP));  /* make is the current ship */

		logthis(spr("GE:INF:Add shp,cls=%d/%d",class,ptr->shpclass));
		sprintf(ptr->shipname,"%s%u\0",shipclass[class].shipname,usrn*usrn+gernd()%(2*usrn+1));
		logthis(spr("  Named: %s",ptr->shipname));

		ptr->coord.xcoord    = rndm((double)univmax*2.0)-(double)univmax;
		ptr->coord.ycoord    = rndm((double)univmax*2.0)-(double)univmax;

		if (shipclass[class].max_phasr > 1)
		        ptr->phasrtype = (gernd()%shipclass[class].max_phasr)+1;
		else
		        ptr->phasrtype = shipclass[class].max_phasr;
		if (shipclass[class].max_shlds > 1)
		        ptr->shieldtype = (gernd()%shipclass[class].max_shlds)+1;
		else
		        ptr->shieldtype = shipclass[class].max_shlds;

		ptr->cybmine = (byte)255;
		ptr->shield = 40 + (ptr->shieldtype*10);
		ptr->phasr = 100;

		ptr->items[I_FLUXPOD] = (gernd()%20)+10;
		if (shipclass[ptr->shpclass].has_decoy)
			ptr->items[I_DECOYS] = (gernd()%20)+10;
		if (shipclass[ptr->shpclass].max_missl)
			ptr->items[I_MISSILE] = (gernd()%10)+20;
		if (shipclass[ptr->shpclass].max_torps)
			ptr->items[I_TORPEDO] = (gernd()%20)+20;
		if (shipclass[ptr->shpclass].has_mine)
			ptr->items[I_MINE] = (gernd()%40)+10;
		if (shipclass[ptr->shpclass].has_jam)
			ptr->items[I_JAMMERS] = (gernd()%20)+10;
		if (shipclass[ptr->shpclass].has_zip)
			ptr->items[I_ZIPPERS] = (gernd()%5)+5;

		/* favor higher gold amounts for tougher cybertrons */
		/* level 2 gets one spin of the wheel, other levels get mulitple */
		/* higher levels get the best outcome, lowest the worst */

		if (cyb_gold > 0)
			{
			goldwin = gernd()%cyb_gold;
			goldtry = abs(shipclass[ptr->shpclass].tough_factor - 2);

			for (i = 0; i < goldtry; i++)
				{
				goldspin = gernd()%cyb_gold;
				if (shipclass[ptr->shpclass].tough_factor < 2 && goldspin < goldwin)
					goldwin = goldspin;
				if (shipclass[ptr->shpclass].tough_factor > 2 && goldspin > goldwin)
					goldwin = goldspin;
				}
			ptr->items[I_GOLD] = goldwin;
			}

		ptr->holdcourse = 0;

		ptr->status = GESTAT_AUTO;
		ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;

		ptr->cybupdate = 1;

		gepdb(GEUPDATE,ptr->userid,ptr->shipno,ptr);
		/* show users sector of new Cyb if in scan range */
		/* show bearing if far away */
		/* thanks Dave Walton for the idea */
		for (zothusn=0; zothusn<nterms; zothusn++)
			{
			wptr=warshpoff(zothusn);
			if (ingegame(zothusn) && wptr->jammer == 0)
				{
				ddist = cdistance(&ptr->coord,&wptr->coord);
				ddist *= 10000;
				if (ddist > shipclass[wptr->shpclass].scanrange)
					{
					bearing = (int)(cbearing(&wptr->coord,&ptr->coord,wptr->heading)+.5);
					prfmsg(CYBNEW,bearing);
					outprfge(FILTER,zothusn);
					}
				else
					{
					setsect(ptr);
					prfmsg(CYBNEW2,xsect,ysect);
					outprfge(FILTER,zothusn);
					}
				}
			}
		}
	}
else
	{
	/* DEBUG */
	geshocst(0,spr("GE:ERR:NO FIND %s user",cybname));
	}
}


void  FUNC cyb_lives(ptr,usrn)

WARSHP *ptr;
int           usrn;
{


WARSHP	*wptr;
int	zothusn;
int	i;

double	ddist;


if (!sameas(ptr->userid,warusroff(usrn)->userid))
	geshocst(0,"GE:ERR:Cyb Names !=");

i = usrn;

sprintf(&cybname[7],"%d",i);

logthis(spr("@cyb_lives %s",cybname));


/* reset the ticker to 255 to cause it to recalc */
ptr->tick = 255;

/* save off the topspeed in 1000's */
/* if no warp, top speed is impulse 99 */
if (ptr->topspeed == 0 && shipclass[ptr->shpclass].max_accel > 0)
	d_topspeed = 990;
else
	d_topspeed = (double)ptr->topspeed*1000.0;

/* countdown to database update */

db_update(ptr,usrn);

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	/* look at all the other ships */
	for (zothusn=0 ; zothusn < nships ; zothusn++)
		{
		wptr=warshpoff(zothusn);
		/* if in game, not cloaked, and not same faction, go getem */
		if (ingegame(zothusn) && wptr->cloak != 10 &&
			(shipclass[wptr->shpclass].faction != shipclass[ptr->shpclass].faction))
			{

			ddist = cdistance(&ptr->coord,&wptr->coord);
			ddist *= 10000;


			if (!neutral(&ptr->coord)
				&& ddist < (double)shipclass[ptr->shpclass].scanrange)
				{
				/* fire phasers (or maybe even missiles) at the fool */

				if (wptr->where == 1 && gebemean(ptr))
					{
					if (ddist < (tooclose+rndm(tooclose))
						|| shipclass[wptr->shpclass].cybs_can_att
						|| wptr->cantexit > 0
						|| ptr->cantexit > 0)
						{
						if (ddist < 30000.0)
							{
							cyb_annoy(ptr,zothusn,20,13,16);
							if (shipclass[ptr->shpclass].max_missl && (ptr->items[I_MISSILE] > 0) && (gernd()%10 == 0))
								{
								misl(ptr,usrn,zothusn,((shipclass[ptr->shpclass].tough_factor+1)*8000),0);
								}
							else
								{
								ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
								if (wptr->where == 1 && ptr->where == 1)
									firehp(ptr,usrn);
								else
									if (shipclass[ptr->shpclass].max_phasr >= phatowrp &&
										ptr->phasr >= PMINFIRE && ptr->where == 0)
										{
										ptr->percent = 2;
										firep(ptr,usrn);
										}
								}
							}
						}
					else
						{
						cyb_annoy(ptr,zothusn,30,1,4);
						}
					}
				else
				if (ptr->where == 0 && wptr->where != 1)
					{
					ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
					ptr->percent = 2;

					/* if the guy gets closer then 2000 or is not little guy or has fired */
					if (ddist < (tooclose + rndm(tooclose))
						|| shipclass[wptr->shpclass].cybs_can_att
						|| wptr->cantexit > 0)
						{
						cyb_attack(ptr,usrn,wptr,zothusn);
						cyb_annoy(ptr,zothusn,20,13,16);
						if (shipclass[ptr->shpclass].has_decoy && ptr->items[I_DECOYS] > 0)
							cyb_lay_decoys(ptr);
						}
					else
						{
						cyb_annoy(ptr,zothusn,20,9,12);
						}
					}
				}
			}
		}
	}
else
	{
	/* don't mine or move if immobile */
	if (shipclass[ptr->shpclass].max_accel > 0)
		{
	/* as long as they can't see ... the other player must be trying to get
		away.... might as well mine the area */
		if (shipclass[ptr->shpclass].has_mine
			&& ptr->items[I_MINE] > 0
			&& gernd()%5 == 0)
			laymine(ptr,usrn,10);

		ptr->speed2b = d_topspeed;
		ptr->holdcourse = gernd()%7 + 2;
		}
	}


if (shipclass[ptr->shpclass].max_accel > 0)
	{
	cyb_check_damage(ptr,usrn);
	cyb_check_lockon(ptr,usrn);
	}

ptr->energy = 50000L;

/*DEBUG
geshocst(0,spr("GE:cm %d",(int)ptr->cybmine)); */

/* check for jammer */
if (ptr->jammer > 0 && shipclass[ptr->shpclass].max_accel > 0)
	{
	ptr->speed2b = d_topspeed;
	ptr->holdcourse = gernd()%10 + 5;
	}

if (ptr->tick == 255)
	{
	if (ptr->cybmine == 255)	/* if just cruising around don't get back to me for some time */
		ptr->tick = (CYBTICKTIME + gernd()%CYBTICKTIME)*5;
	else
	if (ptr->cybmine >= nterms)	/* if going after a fellow NPC, medium speed */
		ptr->tick = (CYBTICKTIME + gernd()%CYBTICKTIME)*3 - shipclass[ptr->shpclass].tough_factor;
	else
		ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME - shipclass[ptr->shpclass].tough_factor;
	}
}

/* look if another cyborg (or cyborgs) has this ship claimed */

int FUNC notclaimed(ptr,usrn)
WARSHP   *ptr;
int   usrn;
{
WARSHP   *wptr;
int zothusn,nc;

nc=0;
for (zothusn=nterms ; zothusn < nships ;zothusn++)
	{
	wptr=warshpoff(zothusn);
	if (wptr->status == GESTAT_AUTO && shipclass[wptr->shpclass].max_type == CLASSTYPE_CYBORG && wptr->cybmine == (byte)usrn)
		++nc;
	}
/*DEBUG
geshocst(0,spr("nc=%d",nc));*/

logthis(spr("notclaimed: nc = %d, class = %d, class.noclaim = %d",nc,ptr->shpclass,shipclass[ptr->shpclass].noclaim));
return (nc < shipclass[ptr->shpclass].noclaim);
}

/* ptr to sender , usrn = reciever */
void  FUNC cyb_annoy(ptr,usrn,rnd,first,last)
WARSHP   *ptr;
int      usrn;
int      rnd;
int      first;
int      last;
{

int base;
int sel;

if ((gernd()%rnd) == 1)
	{
	base = CYBBASEM;
	base += (ptr->shpclass - cyb_class)*16;

	first = first+base;
	last = last+base;
	sel = first+gernd()%(last-first+1);

	if (sel < CYBLASTM)
		{
		prfmsg(sel,ptr->shipname);

		outprfge(FILTER,usrn);
		}
	sprintf(gechrbuf,"cyb_ann shnm=<%s> usrn=%d base=%d frst=%d lst=%d sel=%d",ptr->shipname,usrn, base, first, last, sel);
	logthis(gechrbuf);
	}
}

int  FUNC gebemean(ptr)
WARSHP	*ptr;

{
WARSHP	*wptr;


if (ptr->cybmine >= nterms && ptr->cybmine < nships)
	{
	wptr = warshpoff(ptr->cybmine);

	if (wptr->status == GESTAT_AUTO)		/* always go easy on NPCs */
		if (gernd()%6 == 0)
			return(1);
		else
			return(0);
	}

if (gernd()%(5-shipclass[ptr->shpclass].tough_factor) == 0)
	return(1);

return(0);
}


/* countdown to database update */

void  FUNC db_update(ptr,usrn)

WARSHP   *ptr;
int      usrn;

{
WARUSR	*wuptr;

if (ptr->cybupdate > 1)
	{
	--ptr->cybupdate;
	return;
	}
if (ptr->cybupdate == 1 && ptr->cybmine == 255) /* if cruising around and about to update, change speed/direction */
	{
	cyb_cruise(ptr);
	--ptr->cybupdate;
	return;
	}
if (ptr->cybupdate == 0 && ptr->cybmine == 255)
	{
	wuptr= warusroff(usrn);
	logthis(spr("GE:DBG:Cyb UUpd %s",wuptr->userid));
	geudb(GEUPDATE,wuptr->userid,wuptr);
	logthis(spr("GE:DBG:Cyb PUpd %s",ptr->userid));
	gepdb(GEUPDATE,ptr->userid,ptr->shipno,ptr);
	ptr->cybupdate = 100 + gernd()%100;
	return;
	}
ptr->cybupdate = 20; /* if engaged with another ship, update later */
}

/**************************************************************************
** Attack the other player                                               **
**************************************************************************/

void  FUNC cyb_attack(ptr,usrn,wptr,zothusn)

WARSHP   *ptr;    /* ptr to cyb ship*/
int      usrn;    /* cybs ship number*/
WARSHP   *wptr;   /* ptr to users ship*/
int      zothusn;	/* users ship number*/

{


MISSILE  *mptr;
int i,j;

wptr = wptr; /* eliminates warning*/

if (ptr->phasr >= PMINFIRE && gebemean(ptr))
	{
	ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
	ptr->percent = 2;
	firep(ptr,usrn);
	}

/* fire torpedoes or missiles at the fool */
j = gernd()%(shipclass[ptr->shpclass].tough_factor+2);

if (!gebemean(ptr))
	j = 0;

for (i=0;i<j;++i)
	{
	if (i>0)
		lockwarn = FALSE;
	if (gernd()%10 == 0 && shipclass[ptr->shpclass].max_missl && (ptr->items[I_MISSILE] > 0))
		{
		misl(ptr,usrn,zothusn,((shipclass[ptr->shpclass].tough_factor+1)*8000),0);
		}
	else
		{
		if (shipclass[ptr->shpclass].max_torps && (ptr->items[I_TORPEDO] > 0))
			torp(ptr,usrn,zothusn);
		}
	}

/* launch Zippers if needed */
if (gernd()%20 == 1 && shipclass[ptr->shpclass].has_zip && shipclass[ptr->shpclass].max_accel > 0)
	{
	if (ptr->minesnear == TRUE)
		{
		if (gernd()%3 == 1 && ptr->items[I_ZIPPERS] > 0)
			{
			zip(ptr,usrn);
			ptr->minesnear = FALSE;
			}
		/* get the hell out of here ...then come back */
		if (shipclass[ptr->shpclass].max_accel > 0)
			{
		ptr->speed2b = d_topspeed;
		ptr->head2b = rndm(359.9);
		ptr->holdcourse = gernd()%20 + 3;
			}
		}
	}

/* if we are in hyperspace and fighting and missiles detected
	get out of hyperspace and get shields up */

if (ptr->where == 1)
	{
	for (i=0,mptr=ptr->lmissl;i<MAXMISSL;++i,++mptr)
		{
		if (mptr->distance > 0)
			{
			ptr->speed2b = d_topspeed;
			ptr->holdcourse = gernd()%5 + 5;
			break;
			}
		}
	}
else
	{
	if (ptr->shieldstat != SHIELDDM)
		shieldup(ptr,usrn);
	}
}

/**************************************************************************
** Lay down some decoys                                                  **
**************************************************************************/

void  FUNC cyb_lay_decoys(ptr)
WARSHP   *ptr;

{

int   i;

/* send out a decoy */
for (i=0; i<5;++i)
	if (ptr->decout[i] == 0)
		ptr->decout[i] = DECOYTIME;
}

/**************************************************************************
** if hunting, and badly damaged dump mines, jam, and boogie             **
**************************************************************************/

void  FUNC cyb_check_damage(ptr,usrn)
WARSHP   *ptr;
int      usrn;

{

if (ptr->cybmine < 255 && ptr->damage > CYB_MINDAM && (gernd()%10 == 0))
	{
	if (shipclass[ptr->shpclass].has_mine
		&& ptr->items[I_MINE] > 0
		&& gernd()%5 == 0)
		laymine(ptr,usrn,10);

	if (shipclass[ptr->shpclass].has_jam
		&& ptr->items[I_JAMMERS] > 0
		&& gernd()%100 == 0)
		jam(ptr,usrn);

	ptr->speed2b = d_topspeed;
	ptr->head2b = rndm(359.9);
	ptr->holdcourse = gernd()%10 + 5;
	}
}

/**************************************************************************
** Check lockon status                                                   **
**************************************************************************/

void  FUNC cyb_check_lockon(ptr,usrn)
WARSHP   *ptr;
int      usrn;

{

WARSHP   *wptr;
int      zothusn;

double	ddist;

double	low_dist = 999999999.0;
int	low_ship;
int	lta; /* lowest to attack */
int	usersin;

low_dist = 999999999.0;
low_ship = -1;
usersin = FALSE;

/* if cyborg not seeking - countdown */

zothusn = ptr->cybmine;

if (ptr->holdcourse > 0)
	{
	--(ptr->holdcourse);
	return;
	}

if (zothusn >= nships)
	{
	ptr->cybmine = (byte)255;
	}
else
	{
	if (!ingegame(zothusn))
		{
		ptr->cybmine = (byte)255;
		ptr->speed2b = rndm(ptr->topspeed)*1000; /* let them cruise */
		return;
		}

	wptr=warshpoff(zothusn);

	if (wptr->cloak == 10)
		{
		ptr->holdcourse=gernd()%5+5;
		ptr->speed2b = rndm(ptr->topspeed)*1000; /* let them cruise */

		/* if the guy is cloaked then give up after awhile */
		if(gernd()%10 == 0)
			ptr->cybmine = 255;

		return;
		}

	low_ship = zothusn;
	low_dist = cdistance(&ptr->coord,&(wptr->coord));
	}

/* don't pick new fights with NPCs if no users are playing */
for (zothusn=0; zothusn < nterms; zothusn++)
	{
	wptr=warshpoff(zothusn);
	if (ingegame(zothusn) && wptr->status == GESTAT_USER)
		{
		usersin = TRUE;
		break;
		}
	}


if (ptr->cybmine == (byte)255)
	{
	lta = shipclass[ptr->shpclass].lowest_to_attk-1;

	for (zothusn=0 ; zothusn < nships ; zothusn++)
		{
		wptr=warshpoff(zothusn);
		/* if playing, and not cloaked, and not same faction, go getem */
		if (ingegame(zothusn) && wptr->cloak != 10 &&
			(shipclass[wptr->shpclass].faction != shipclass[ptr->shpclass].faction))
			{
			/* if high enough class to attack, not already claimed, and user or attackable NPC */
			if (lta <= wptr->shpclass && notclaimed(wptr,zothusn)
				&& (wptr->status == GESTAT_USER ||
				(wptr->status == GESTAT_AUTO && shipclass[wptr->shpclass].cybs_can_att &&
					usersin == TRUE && gernd()%3000 == 0)))
				{
				/* figure out who is closest */
				ddist = cdistance(&ptr->coord,&wptr->coord);
				if (ddist < low_dist)
					{
					low_dist = ddist;
					low_ship = zothusn;
					}
				}
			}
		}
	}

if (low_ship == -1 || low_ship >= nships)
	{
	ptr->tick = 255; /* no one in game so cool it for awhile */
	ptr->cybmine = 255;
	}
else
	{
	ptr->cybmine = (byte)low_ship;
	wptr=warshpoff(low_ship);
	if (low_dist >= hyperdist1)
		{
		ptr->speed2b = ((int)(low_dist/hyperdist1))*FARSPEED;
		ptr->speed = ptr->speed2b;
		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		/* DEBUG
		prf("***\r%s, LONG, Sector %d %d, Speed: %s \rhyperdist1: %s, hyperdist2: %s, low_dist: %s\r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b),
			spr("%ld",(long)hyperdist1),spr("%ld",(long)hyperdist2),spr("%ld",(long)low_dist));
		outwar(ALWAYS,usrn,0); */
		}
	else
	if (low_dist >= hyperdist2)
		{
		if (cyb_fast(ptr))
			{
			ptr->speed2b = FARSPEED;
			ptr->speed = ptr->speed2b;
			}
		else
			ptr->speed2b = d_topspeed;
		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		/* DEBUG
		prf("***\r%s, MID, Sector %d %d, Speed: %s \rhyperdist1: %s, hyperdist2: %s, low_dist: %s\r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b),
			spr("%ld",(long)hyperdist1),spr("%ld",(long)hyperdist2),spr("%ld",(long)low_dist));
		outwar(ALWAYS,usrn,0); */
		cyb_annoy(ptr,low_ship,60,1,4);
		}
	else
	if (low_dist >= 3)
		{
		if (cyb_fast(ptr))
			{
			ptr->speed2b = d_topspeed;
			ptr->speed = ptr->speed2b;
			}
		else
			ptr->speed2b = d_topspeed;
		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		/* DEBUG
		prf("***\r%s, SHORT, Sector %d %d, Speed: %s \rhyperdist1: %s, hyperdist2: %s, low_dist: %s\r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b),
			spr("%ld",(long)hyperdist1),spr("%ld",(long)hyperdist2),spr("%ld",(long)low_dist));
		outwar(ALWAYS,usrn,0); */
		}
	else
	if (wptr->where == 1)
		{
		if (cyb_fast(ptr))
			{
			ptr->speed2b = d_topspeed;
			ptr->speed = ptr->speed2b;
			}
		if (wptr->speed * 1.25 >= d_topspeed)
		    ptr->speed2b = d_topspeed;
		else
		    ptr->speed2b = ((int)(wptr->speed * 1.25)/1000)*1000;
		ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		/* DEBUG
		prf("***\r%s, CLOSE, Sector %d %d, Speed: %s \rhyperdist1: %s, hyperdist2: %s, low_dist: %s\r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b),
			spr("%ld",(long)hyperdist1),spr("%ld",(long)hyperdist2),spr("%ld",(long)low_dist));
		outwar(ALWAYS,usrn,0); */
		}
	else
	if (shipclass[wptr->shpclass].cybs_can_att || wptr->cantexit > 0 || ptr->cantexit > 0)
		{
		if (low_dist > .5)
			{
			if (cyb_fast(ptr))
				{
				ptr->speed2b = 990.0;
				ptr->speed = ptr->speed2b;
				}
			else
				ptr->speed2b = 990.0;
			ptr->head2b = vector(&ptr->coord,&(wptr->coord));
			}
		else
			{
			if (cyb_fast(ptr))
				{
				ptr->speed2b = ((int)(rndm(350.0)+150.0)/10)*10;
				ptr->speed = ptr->speed2b;
				}
			else
				ptr->speed2b = ((int)(rndm(350.0)+150.0)/10)*10;
			ptr->head2b = rndm(359.9);
			}
		/* DEBUG
		prf("***\r%s, IMPULSE, Sector %d %d, Speed: %s \rhyperdist1: %s, hyperdist2: %s, low_dist: %s\r",cybname,(int)ptr->coord.xcoord,(int)ptr->coord.ycoord,spr("%ld",(long)ptr->speed2b),
			spr("%ld",(long)hyperdist1),spr("%ld",(long)hyperdist2),spr("%ld",(long)low_dist));
		outwar(ALWAYS,usrn,0); */
		cyb_annoy(ptr,low_ship,30,5,8);
		}
	else
		{
		/* back off if not going to shoot first */
		if (cyb_fast(ptr))
			{
			ptr->speed2b = ((int)(rndm(750.0)+150.0)/10)*10;
			ptr->speed = ptr->speed2b;
			}
		else
			ptr->speed2b = ((int)(rndm(750.0)+150.0)/10)*10;
		if (low_dist < 1.5)
			ptr->head2b=(double)((int)(vector(&ptr->coord, &(wptr->coord)) + 180.0) % 360);
		else
			ptr->head2b = vector(&ptr->coord,&(wptr->coord));
		cyb_annoy(ptr,low_ship,30,9,12);
		}
	/* make sure speed jumps won't leave us in a weird state */
	/* avoid fractional warp values */
	if (ptr->speed < 1000)
		{
		if (ptr->speed2b >= 1000 && shipclass[ptr->shpclass].max_accel >= 1000)
			{
			if (ptr->speed2b <= shipclass[ptr->shpclass].max_accel)
				ptr->speed = ptr->speed2b;
			else
				ptr->speed = shipclass[ptr->shpclass].max_accel;
			}
		else
			{
			ptr->where = 0;
			if (ptr->shieldstat != SHIELDDM)
				shieldup(ptr,usrn);
			}
		}
	if (ptr->speed >= 1000)
		{
		ptr->where = 1;
		if (ptr->shieldstat == SHIELDUP)
			shielddn(ptr,usrn);

		}
	}
}

void   FUNC cyb_won(ptr,usrn,wptr)
WARSHP	*ptr;				/* ptr to Cyber who won */
int		usrn;				/* usernum of cyber who won */
WARSHP   *wptr;			/* ptr to ship cyber killed */

{
usrn = usrn;
wptr = wptr;
ptr->cybmine = (byte)255;
cyb_cruise(ptr);
ptr->cybupdate = 0;
}

void   FUNC cyb_died(ptr,usrn,wptr)
WARSHP	*ptr;				/* ptr to Cyber who died */
int		usrn;				/* usernum of cyber who died */
WARSHP   *wptr;			/* ptr to ship who killed cyber*/


{
usrn = usrn;
wptr = wptr;
ptr->status = GESTAT_AVAIL;
}

/**************************************************************************
** Set random speed and heading if cruising                              **
**************************************************************************/

void FUNC cyb_cruise(ptr)
WARSHP *ptr;

{
if (ptr->topspeed == 0)
	{
	if (shipclass[ptr->shpclass].max_accel > 0) /* impulse only */
		ptr->speed2b = ((gernd()%99)+1)*10;
	else
		ptr->speed2b = 0;
	}
else
	{
	ptr->speed2b = (gernd()%(ptr->topspeed)+1)*1000;
	}
if (shipclass[ptr->shpclass].max_accel > 0)	/* if ya can't move, ya can't rotate */
	ptr->head2b = rndm(359.9);
}
