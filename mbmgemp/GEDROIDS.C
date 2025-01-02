
/***************************************************************************
 *                                                                         *
 *   GEDROIDS.C                                                            *
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


#define  GEDROIDS   1

#include "geglobal.h"

/* LOCAL GLOBAL DEFS ****************************************************/

char	droidname[UIDSIZ];
double	dr_topspeed;

/**************************************************************************
** Droid Init Function                                                   **
**************************************************************************/

void  FUNC droid_init(ptr, usrn, class)
WARSHP *ptr;
int   usrn;
int   class;

{

WARSHP *wptr;

int zothusn;
double ddist;

if (usrn < 0 || usrn >= nships)
	{
	logthis(spr("DROID_INIT:bad usrn [%d]",usrn));
	return;
	}

strncpy(droidname,"@Droid-",UIDSIZ);/* Bj Added name here */
sprintf(&droidname[7],"%d",usrn);

waruptr = warusroff(usrn);
warsptr = warshpoff(usrn);

logthis(spr("GE:INF:Adding %s user",droidname));

initusr(droidname);

memcpy(waruptr,&tmpusr,sizeof(WARUSR));  /* make it the current user */

/* make me a Ship */
logthis(spr("GE:INF:Adding %s ship - %d",ptr->userid,class));

initshp(droidname,class);

memcpy(ptr,&tmpshp,sizeof(WARSHP));  /* make is the current ship */
sprintf(ptr->shipname,"%s%u\0",shipclass[class].shipname,usrn*usrn+gernd()%(2*usrn+1));

ptr->coord.xcoord    = rndm((double)univmax*2.0)-(double)univmax;
ptr->coord.ycoord    = rndm((double)univmax*2.0)-(double)univmax;

if(shipclass[ptr->shpclass].loadout == 2)  /* Murdonian Transport */
	{
	ptr->items[I_MISSILE] = (gernd()%20)+10;
	ptr->items[I_TORPEDO] = (gernd()%20)+10;
	ptr->items[I_IONCANNON] = (gernd()%25)+5;
	ptr->items[I_FLUXPOD] = (gernd()%30)+10;
	ptr->items[I_DECOYS] = (gernd()%75)+10;
	ptr->items[I_JAMMERS] = (gernd()%75)+10;
	ptr->items[I_MINE] = (gernd()%75)+10;
	ptr->items[I_GOLD] = (gernd()%500)+100;
	}
else
if(shipclass[ptr->shpclass].loadout == 6)  /* Galactic Command Freighter */
	{
	ptr->items[I_MISSILE] = (gernd()%50)+10;
	ptr->items[I_TORPEDO] = (gernd()%50)+10;
	ptr->items[I_IONCANNON] = (gernd()%20)+10;
	ptr->items[I_FIGHTER] = (gernd()%100)+20;
	ptr->items[I_FLUXPOD] = (gernd()%40)+10;
	ptr->items[I_DECOYS] = (gernd()%100)+10;
	ptr->items[I_JAMMERS] = (gernd()%100)+10;
	ptr->items[I_ZIPPERS] = (gernd()%100)+10;
	ptr->items[I_MINE] = (gernd()%100)+10;
	ptr->items[I_GOLD] = (gernd()%cyb_gold)+1000;
	}
else
	{
	ptr->items[I_FLUXPOD] = (gernd()%10)+10;
	if (shipclass[ptr->shpclass].max_torps)
		ptr->items[I_TORPEDO] = (gernd()%20)+10;
	if (shipclass[ptr->shpclass].max_missl)
		ptr->items[I_MISSILE] = (gernd()%20)+10;
	if (shipclass[ptr->shpclass].has_decoy)
		ptr->items[I_DECOYS] = (gernd()%20)+10;
	if (shipclass[ptr->shpclass].has_jam)
		ptr->items[I_JAMMERS] = (gernd()%20)+10;
	if (shipclass[ptr->shpclass].has_mine)
		ptr->items[I_MINE] = (gernd()%20)+10;
	if (shipclass[ptr->shpclass].has_zip)
		ptr->items[I_ZIPPERS] = (gernd()%20)+10;
	if (shipclass[ptr->shpclass].loadout == 4)  /* Sarten Civil Transport */
		ptr->items[I_GOLD] = (gernd()%200)+100;
	}


cyb_cruise(ptr);
ptr->holdcourse = 0;
ptr->status = GESTAT_AUTO;
ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;

if (shipclass[class].max_phasr > 1)
	ptr->phasrtype = (gernd()%shipclass[class].max_phasr)+1;
else
	ptr->phasrtype = shipclass[class].max_phasr;
if (shipclass[class].max_shlds > 1)
	ptr->shieldtype = (gernd()%shipclass[class].max_shlds)+1;
else
	ptr->shieldtype = shipclass[class].max_shlds;

ptr->cybmine = (byte)255;

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
			prfmsg(DROIDNEW,bearing);
			outprfge(FILTER,zothusn);
			}
		else
			{
			setsect(ptr);
			prfmsg(DROIDNW2,xsect,ysect);
			outprfge(FILTER,zothusn);
			}
		}
	}
}

/**************************************************************************
** Droid Lives Function                                                  **
**************************************************************************/

void  FUNC droid_lives(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

if (usrn < 0 || usrn >= nships)
	{
	logthis(spr("DROID_LIVES:bad usrn [%d]",usrn));
	return;
	}

sprintf(&droidname[7],"%d",usrn+1);

/* DEBUG
logthis(spr("GE:%s Lives",droidname)); */

/* reset the ticker to 255 to cause it to recalc */
ptr->tick = 255;

if (ptr->holdcourse > 0)
        --(ptr->holdcourse);

/* save off the topspeed in 1000's */
/* if no warp, top speed is impulse 99 */
if (ptr->topspeed == 0 && shipclass[ptr->shpclass].max_accel > 0)
        dr_topspeed = 990;
else
        dr_topspeed = (double)ptr->topspeed*1000.0;

if(shipclass[ptr->shpclass].loadout == 1)
	droid_act_1(ptr,usrn);    /* Lydorian Garbage Scow */
else
if(shipclass[ptr->shpclass].loadout == 2)
	droid_act_2(ptr,usrn);    /* Murdonian Transport */
else
if(shipclass[ptr->shpclass].loadout == 3)
	droid_act_3(ptr,usrn);    /* Vakory Survey Drone */
else
if(shipclass[ptr->shpclass].loadout == 4)
	droid_act_4(ptr,usrn);    /* Sarten Civil Transport */
else
if(shipclass[ptr->shpclass].loadout == 5)
	droid_act_5(ptr,usrn);    /* Sarten Attack Drone */
else
if(shipclass[ptr->shpclass].loadout == 6)
	droid_act_6(ptr,usrn);    /* Galactic Command Freighter */

ptr->energy = 50000L;


if (ptr->tick == 255)
        {
        if (ptr->cybmine == 255)        /* if just cruising around don't get back to me for some time */
                ptr->tick = (CYBTICKTIME + gernd()%CYBTICKTIME)*5;
        else
        if (ptr->cybmine >= nterms)     /* if going after a fellow NPC, medium speed */
                ptr->tick = (CYBTICKTIME + gernd()%CYBTICKTIME)*3 - shipclass[ptr->shpclass].tough_factor;
        else
                ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME - shipclass[ptr->shpclass].tough_factor;
        }
}


/* ptr to sender , usrn = reciever */
void  FUNC droid_annoy(ptr,usrn,rnd,first,last)
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
	base = DRBASEM;
	base += (ptr->shpclass - dr_class)*16;

	first = first+base;
	last = last+base;
	sel = first+gernd()%(last-first+1);

	if (sel < DRLASTM)
		{
		prfmsg(sel,ptr->shipname);
		outprfge(FILTER,usrn);
		}
        sprintf(gechrbuf,"dr_ann shnm=<%s> usrn=%d base=%d frst=%d lst=%d sel=%d",ptr->shipname,usrn, base, first, last, sel);
        logthis(gechrbuf);
	}
}


/**************************************************************************
** Lydorian Garbage Scow                                                 **
** Impulse only, no phasers, no projectiles                              **
**************************************************************************/

void  FUNC droid_act_1(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

WARSHP	*wptr;
int	zothusn;

double	ddist;

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	if (ptr->cybmine == 255)
		{
		/* look at user ships only */
		for (zothusn=0 ; zothusn < nterms ; zothusn++)
			{
			wptr=warshpoff(zothusn);
			/* hail users in area */
			if (ingegame(zothusn) && wptr->status == GESTAT_USER && wptr->cloak != 10)
				{
				ddist = cdistance(&ptr->coord,&wptr->coord);
				ddist *= 10000;
				if (ddist < (double)shipclass[ptr->shpclass].scanrange)
					{
					ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
					droid_annoy(ptr,zothusn,5,1,4);
					}
				}
			}
		}
	if (ptr->cybmine < nships)
		{
		zothusn = ptr->cybmine;
		wptr = warshpoff(zothusn);
		ddist = cdistance(&ptr->coord,&wptr->coord);
		ddist *= 10000;
		if ((ddist < (double)shipclass[ptr->shpclass].scanrange && wptr->cloak != 10) || ptr->cantexit > 0)
			{
			/* if still in range, flee */
			ptr->speed2b = dr_topspeed;
			ptr->head2b=(double)((int)(vector(&ptr->coord, &(wptr->coord)) + 180.0) % 360);
			droid_annoy(ptr,zothusn,20,9,12);
			}
		else
			{
			/* phew we're safe */
			ptr->cybmine = 255;
			cyb_cruise(ptr);
			}
		}
	else
		ptr->cybmine = 255;
	}
droid_check_state(ptr,usrn);
}

/**************************************************************************
** Murdonian Transport                                                   **
** Warp, phasers, no projectiles                                         **
**************************************************************************/

void  FUNC droid_act_2(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

WARSHP	*wptr;
int	zothusn;

double	ddist;

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	if (ptr->cybmine == 255)
		{
		/* look at user ships only */
		for (zothusn=0 ; zothusn < nterms ; zothusn++)
			{
			wptr=warshpoff(zothusn);
			/* hail users in area */
			if (ingegame(zothusn) && wptr->status == GESTAT_USER && wptr->cloak != 10)
				{
				ddist = cdistance(&ptr->coord,&wptr->coord);
				ddist *= 10000;
				if (ddist < (double)shipclass[ptr->shpclass].scanrange)
					{
					if (ptr->holdcourse == 0)
						{
						ptr->speed2b = ((gernd()%85)+15)*10;
						ptr->holdcourse = gernd()%50 + 10;
						}
					ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
					droid_annoy(ptr,zothusn,5,1,4);
					}
				}
			}
		}
	if (ptr->cybmine < nships)
		{
		ptr->holdcourse = 0;
		zothusn = ptr->cybmine;
		wptr = warshpoff(zothusn);
		ddist = cdistance(&ptr->coord,&wptr->coord);
		ddist *= 10000;
		if ((ddist < (double)shipclass[ptr->shpclass].scanrange && wptr->cloak != 10) || ptr->cantexit > 0)
			{
			/* if still in range, flee and attack */
			if (ptr->damage > 50)
				ptr->speed2b = dr_topspeed;
			else
				ptr->speed2b = 990.0;
			ptr->head2b = (double)((int)(vector(&ptr->coord, &(wptr->coord)) + 180.0 + (rand() % 51 - 25)) % 360);
			droid_annoy(ptr,zothusn,20,9,12);
			if (ddist < 30000)
				{
				ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
				if (wptr->where == 1 && ptr->where == 1)
					firehp(ptr,usrn);
				else
				if (ptr->where == 0 && (wptr->where == 0 || (wptr->where == 1 &&
					shipclass[ptr->shpclass].max_phasr >= phatowrp)) && ptr->phasr >= PMINFIRE)
					ptr->percent = 2;
					firep(ptr,usrn);
				}
			}
		else
			{
			/* phew we're safe */
			ptr->cybmine = 255;
			cyb_cruise(ptr);
			}
		}
	else
		ptr->cybmine = 255;
	}
else
	{
	ptr->speed2b = dr_topspeed;
	ptr->holdcourse = gernd()%20 + 10;
	}
droid_check_state(ptr,usrn);
}

/**************************************************************************
** Vakory Survey Drone                                                   **
** Warp, phasers, torps, jammers, mines                                  **
**************************************************************************/

void  FUNC droid_act_3(ptr,usrn)

WARSHP	*ptr;
int	usrn;
{

WARSHP	*wptr;
int	zothusn, i, j;

double	ddist;

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	if (ptr->cybmine == 255)
		{
		/* look at user ships only */
		for (zothusn=0 ; zothusn < nterms ; zothusn++)
			{
			wptr=warshpoff(zothusn);
			/* hail users in area */
			if (ingegame(zothusn) && wptr->status == GESTAT_USER && wptr->cloak != 10)
				{
				ddist = cdistance(&ptr->coord,&wptr->coord);
				ddist *= 10000;
				if (ddist < (double)shipclass[ptr->shpclass].scanrange)
					{
					ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
					droid_annoy(ptr,zothusn,20,1,4);
					}
				}
			}
		}
	if (ptr->cybmine < nships)
		{
		zothusn = ptr->cybmine;
		wptr = warshpoff(zothusn);
		ddist = cdistance(&ptr->coord,&wptr->coord);
		ddist *= 10000;
		if ((ddist < (double)shipclass[ptr->shpclass].scanrange && wptr->cloak != 10) || ptr->cantexit > 0)
			{
			if (ptr->holdcourse == 0)
				{
				if (ptr->damage < 67)
					ptr->speed2b = ((gernd()%99)+1)*10;
				else
					{
					if (ptr->items[I_MINE] > 0 && shipclass[ptr->shpclass].has_mine)
						laymine(ptr,usrn,10);
					if (ptr->items[I_JAMMERS] > 0 && shipclass[ptr->shpclass].has_jam)
						jam(ptr,usrn);
					ptr->speed2b = (double)(ptr->topspeed * 1000);
					ptr->head2b = (double)((int)(vector(&ptr->coord, &(wptr->coord)) + 180.0 + (rand() % 51 - 25)) % 360);
					}
				ptr->holdcourse = gernd()%30 + 20;
				}
			droid_annoy(ptr,zothusn,20,9,12);
			if (ddist < 30000)
				{
				ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
				if (wptr->where == 1 && ptr->where == 1)
					firehp(ptr,usrn);
				else
				if (ptr->where == 0 && (wptr->where == 0 || (wptr->where == 1 &&
					shipclass[ptr->shpclass].max_phasr >= phatowrp)) && ptr->phasr >= PMINFIRE)
					ptr->percent = 2;
					firep(ptr,usrn);
				if (ptr->where == 0 && wptr->where == 0 && shipclass[ptr->shpclass].max_torps && gernd()%10 == 0)
					{
					/* fire torpedoes at the fool */
					j = gernd()%(shipclass[ptr->shpclass].tough_factor+1);
					for (i=0;i<j;++i)
						{
						if (i>0)
							lockwarn = FALSE;
						if (ptr->items[I_TORPEDO] > 0)
							torp(ptr,usrn,zothusn);
						}
					}
				}
			}
		else
			{
			/* phew we're safe */
			ptr->cybmine = 255;
			cyb_cruise(ptr);
			}
		}
	else
		ptr->cybmine = 255;
	}
else
	{
	ptr->speed2b = dr_topspeed;
	ptr->holdcourse = gernd()%50 + 10;
	}
droid_check_state(ptr,usrn);
}

/**************************************************************************
** Sarten Civil Transport                                                **
** Warp, phasers, decoys, jammers                                        **
**************************************************************************/

void  FUNC droid_act_4(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

WARSHP   *wptr;
int      zothusn;

double   ddist;

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	if (ptr->cybmine == 255)
		{
		/* look at user ships only */
		for (zothusn=0 ; zothusn < nterms ; zothusn++)
			{
			wptr=warshpoff(zothusn);
			/* hail users in area */
			if (ingegame(zothusn) && wptr->status == GESTAT_USER && wptr->cloak != 10)
				{
				ddist = cdistance(&ptr->coord,&wptr->coord);
				ddist *= 10000;
				if (ddist < (double)shipclass[ptr->shpclass].scanrange)
					{
					ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
					droid_annoy(ptr,zothusn,5,1,4);
					}
				}
			}
		}
	if (ptr->cybmine < nships)
		{
		zothusn = ptr->cybmine;
		wptr = warshpoff(zothusn);
		ddist = cdistance(&ptr->coord,&wptr->coord);
		ddist *= 10000;
		if ((ddist < (double)shipclass[ptr->shpclass].scanrange && wptr->cloak != 10) || ptr->cantexit > 0)
			{
			/* if still in range, flee and attack */
			if (ptr->holdcourse == 0)
				{
				if (ptr->damage < 50)
					ptr->speed2b = dr_topspeed;
				else
					ptr->speed2b = 990.0;
				ptr->head2b = (double)((int)(vector(&ptr->coord, &(wptr->coord)) + 180.0 + (rand() % 51 - 25)) % 360);
				ptr->holdcourse = gernd()%7 + 2;
				}
			droid_annoy(ptr,zothusn,20,9,12);
			if (ddist < 30000)
				{
				ptr->degrees = (int)(cbearing(&ptr->coord,&wptr->coord,ptr->heading)+.5);
				if (wptr->where == 1 && ptr->where == 1)
					firehp(ptr,usrn);
				else
				if (ptr->where == 0 && (wptr->where == 0 || (wptr->where == 1 &&
					shipclass[ptr->shpclass].max_phasr >= phatowrp)) && ptr->phasr >= PMINFIRE)
					{
					ptr->percent = 2;
					firep(ptr,usrn);
					}
				}
			if (ptr->where == 0 && ptr->items[I_DECOYS] > 0 && shipclass[ptr->shpclass].has_decoy)
				cyb_lay_decoys(ptr);
			if (ptr->holdcourse == 1 && ptr->items[I_JAMMERS] > 0 && shipclass[ptr->shpclass].has_jam)
				jam(ptr,usrn);
			}
		else
			{
			/* phew we're safe */
			ptr->cybmine = 255;
			cyb_cruise(ptr);
			}
		}
	else
		ptr->cybmine = 255;
	}
droid_check_state(ptr,usrn);
}

/**************************************************************************
** Sarten Attack Drone                                                   **
** Warp, phasers, torps, decoys, jammers, zippers                        **
**************************************************************************/

void  FUNC droid_act_5(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

WARSHP   *wptr;
int      zothusn;

double   ddist;

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	/* look at all the other ships */
	for (zothusn=0 ; zothusn < nterms ; zothusn++)
		{
		wptr=warshpoff(zothusn);
		/* if not me, and playing, and not cyborg, go getem */
		if (ingegame(zothusn) && wptr->status == GESTAT_USER)
			{
			ddist = cdistance(&ptr->coord,&wptr->coord);
			ddist *= 10000;
			if (ddist < (double)shipclass[ptr->shpclass].scanrange)
				{
				ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
				droid_annoy(ptr,zothusn,4,1,4);
				}
			}
		}
	}
else
	{
	ptr->speed2b = 999.9; /* has no warp capability */
	ptr->holdcourse = gernd()%50 + 10;
	}
droid_check_state(ptr,usrn);
}

/**************************************************************************
** Galactic Command Freighter                                            **
** Warp, phasers, torps, decoys, jammers, zippers                        **
**************************************************************************/

void  FUNC droid_act_6(ptr,usrn)

WARSHP *ptr;
int           usrn;
{

WARSHP   *wptr;
int      zothusn;

double   ddist;

/* am I being jammed ? */
if (ptr->jammer == 0)
	{
	/* look at all the other ships */
	for (zothusn=0 ; zothusn < nterms ; zothusn++)
		{
		wptr=warshpoff(zothusn);
		/* if not me, and playing, and not cyborg, go getem */
		if (ingegame(zothusn) && wptr->status == GESTAT_USER)
			{
			ddist = cdistance(&ptr->coord,&wptr->coord);
			ddist *= 10000;
			if (ddist < (double)shipclass[ptr->shpclass].scanrange)
				{
				ptr->tick = CYBTICKTIME + gernd()%CYBTICKTIME;
				droid_annoy(ptr,zothusn,4,1,4);
				}
			}
		}
	}
else
	{
	ptr->speed2b = 999.9; /* has no warp capability */
	ptr->holdcourse = gernd()%50 + 10;
	}
droid_check_state(ptr,usrn);
}


void   FUNC droid_won(ptr)
WARSHP   *ptr;

{
ptr->cybmine = 255;
cyb_cruise(ptr);
}


void   FUNC droid_died(ptr)
WARSHP   *ptr;

{
ptr->status = GESTAT_AVAIL;
logthis(spr("GE:INF:%s Died!",ptr->userid));
}

int   FUNC missl_attached(ptr,usrn)
WARSHP	*ptr;
int		usrn;

{
int		i;
MISSILE  *mptr;

usrn=usrn;

for (i=0,mptr=ptr->lmissl;i<MAXMISSL;++i,++mptr)
	{
	if (mptr->distance > 0)
		{
		return(TRUE);
		}
	}
return(FALSE);
}

void FUNC droid_check_state(ptr,usrn)
WARSHP *ptr;
int usrn;

{
if (ptr->speed < 1000.0)
	{
	ptr->where = 0;
	if (ptr->shieldstat != SHIELDDM)
		shieldup(ptr,usrn);
	}
else
	{
	ptr->where = 1;
	if (ptr->shieldstat == SHIELDUP)
		shielddn(ptr,usrn);
	}
}
