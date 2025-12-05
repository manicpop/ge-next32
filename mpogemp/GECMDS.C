

/***************************************************************************
 *                                                                         *
 *   GECMDS.C                                                              *
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
#include "portable.h"
#include "dosface.h"

#endif

#include "math.h"
#include "majorbbs.h"

#include "gemain.h"

#define GECMDS 1

#include "geglobal.h"


/* LOCAL GLOBAL DEFS *****************************************************/


char *kwrd[NUMITEMS] = {						"men",
									"mis",
									"tor",
									"ion",
									"flu",
									"foo",
									"fig",
									"dec",
									"tro",
									"zip",
									"jam",
									"min",
									"gol",
									"sp"};

char *item_name[NUMITEMS] = {						"men",
									"missiles",
									"torpedoes",
									"ion cannons",
									"flux pods",
									"food cases",
									"fighters",
									"decoys",
									"troops",
									"zippers",
									"jammers",
									"mines",
									"gold",
									"spies"};


void	cmd_gehelp(), cmd_cloak(), cmd_impulse(), cmd_phas(), cmd_report(),
	cmd_rotate(), cmd_send(), cmd_scan(), cmd_shields(), cmd_warp(),
	cmd_torp(), cmd_missl(), cmd_decoy(), cmd_flux(), cmd_set(), cmd_orbit(),
	cmd_transfer(), cmd_admin(), cmd_attack(), cmd_geroster(), cmd_buy(),
	cmd_price(), cmd_planet(), cmd_maint(), cmd_new(), cmd_sell(), cmd_sysop(),
	cmd_rename(), cmd_destruct(), cmd_abort(), cmd_jammer(), cmd_mine(),
	cmd_abandon(), cmd_zipper(), cmd_lock(), cmd_navigate(), cmd_who(),
	cmd_displ(), cmd_freq(), cmd_cls(), cmd_data(), cmd_team(), cmd_spy(),
	cmd_jettison(), cmd_stop();

#define GECMDSIZ (sizeof(gecmds)/sizeof(struct cmd))

struct	cmd	gecmds[]={
		/*	command	function	0=payers only */
		/*	-------	----------- 	------------- */
			{"?",	cmd_gehelp,	1},
			{"aba",	cmd_abandon,	0},
			{"abo",	cmd_abort,	1},
			{"adm",	cmd_admin,	0},
			{"att",	cmd_attack,	0},
			{"buy",	cmd_buy,	0},
			{"clo",	cmd_cloak,	1},
			{"cls",	cmd_clear,	0},
			{"dat",	cmd_data,	0},
			{"dec",	cmd_decoy,	1},
			{"des",	cmd_destruct,	1},
			{"flu",	cmd_flux,	1},
			{"fre",	cmd_freq,	0},
			{"hel",	cmd_gehelp,	1},
			{"imp",	cmd_impulse,	1},
			{"jam",	cmd_jammer,	1},
			{"jet",	cmd_jettison,	1},
			{"loc",	cmd_lock,	1},
			{"mai",	cmd_maint,	1},
			{"min",	cmd_mine,	1},
			{"mis",	cmd_missl,	1},
			{"nav",	cmd_navigate,	1},
			{"new",	cmd_new,	1},
			{"orb",	cmd_orbit,	1},
			{"pha",	cmd_phas,	1},
			{"pla",	cmd_planet,	0},
			{"pri",	cmd_price,	0},
			{"ren",	cmd_rename,	0},
			{"rep",	cmd_report,	1},
			{"ros",	cmd_geroster,	1},
			{"rot",	cmd_rotate,	1},
			{"sca",	cmd_scan,	1},
			{"sel", cmd_sell,	1},
			{"sen",	cmd_send,	0},
			{"set",	cmd_set,	1},
			{"shi",	cmd_shields,	1},
			{"spy",	cmd_spy,	1},
			{"sto", cmd_stop,	1},
			{"sys",	cmd_sysop,	1},
			{"tea", cmd_team,	1},
			{"tor",	cmd_torp,	1},
			{"tra",	cmd_transfer,	0},
			{"war",	cmd_warp,	1},
			{"who",	cmd_who,	1},
			{"zip",	cmd_zipper,	1}
};


/* If you wish to add your own help commands to the list simply add the command
	name to the list and the name of the message in the MSG file. Also add the
	command to the list in the message HLPINDEX                Mike           */

struct hlpcmd	{
		char *command;
		int helptxt;
		};

struct hlpcmd gehlp[] = {
		{"abandon",			HLPABA},
		{"abort",			HLPABO},
		{"admin",			HLPADM},
		{"attack",			HLPATT},
		{"buy",				HLPBUY},
		{"cloak",			HLPCLO},
		{"cls",				HLPCLS},
		{"decoy",			HLPDEC},
		{"destruct",			HLPDES},
		{"freq",			HLPFRE},
		{"help",			HLPHEL},
		{"hyper",			HLPHYP},
		{"impulse",			HLPIMP},
		{"jammer",			HLPJAM},
		{"jettison",			HLPJET},
		{"lock",			HLPLOC},
		{"maintenance",			HLPMAI},
		{"mine",			HLPMIN},
		{"missile",			HLPMIS},
		{"navigate",			HLPNAV},
		{"new",				HLPNEW},
		{"newprice",			HLPNEW2},
		{"orbit",			HLPORB},
		{"phaser",			HLPPHA},
		{"planet",			HLPPLA},
		{"price",			HLPPRI},
		{"rename",			HLPREN},
		{"report",			HLPREP},
		{"roster",			HLPROS},
		{"rotate",			HLPROT},
		{"scan",			HLPSCA},
		{"sell",			HLPSEL},
		{"send",			HLPSEN},
		{"set",				HLPSET},
		{"shield",			HLPSHI},
		{"spy",				HLPSPY},
		{"stop",			HLPSTO},
		{"sys",				HLPSYS},
		{"team",			HLPTEA},
		{"torpedo",			HLPTOR},
		{"transfer",			HLPTRA},
		{"warp",			HLPWAR},
		{"who",				HLPWHO},
		{"x",				HLPEXT},
		{"zipper",			HLPZIP},

/* FYI: The above are commands and below are topics....       Mike       */

		{"battle",			HLPBATTL},
		{"battle2",			HLPBATT2},
		{"battle3",			HLPBATT3},
		{"class",			HLPCLS1},
		{"communicate",			HLPCOMMU},
		{"cybertrons",			HLPCYBER},
		{"distress",			HLPDIST},
		{"flux",			HLPFLU},
		{"galaxy",			HLPGALXY},
		{"lydorians",			HLPLYDO},
		{"murdonians",			HLPMURD},
		{"moving",			HLPNAVIG},
		{"planets",			HLPPLANT},
		{"planets2",			HLPPLAN2},
		{"planets3",			HLPPLAN3},
		{"sartens",			HLPSART},
		{"scoring",			HLPSCORE},
		{"starting",			HLPSTART},
		{"strategy",			HLPSTRAT},
		{"tryklons",			HLPTRYK},
		{"vakories",			HLPVAKO},
		{"wormholes",			HLPWORM},
		{"zygorians",			HLPZYGO},

		{NULL,				0}
};

struct	cmd * FUNC gesearch(ptr,tab,len)
char	*ptr;
struct	cmd tab[];
int	len;

{

	int c;
	struct cmd *lo,*md,*hi;

	lo = &tab[0];
	hi = &tab[len-1];

	while (lo <= hi)
		{
#pragma warn -sig
		md = lo + ((hi - lo)/2L);
#pragma warn +sig
		if ((c = strncmp(ptr,md->command,3)) < 0)
			hi = md - 1;
		else if (c > 0)
			lo = md + 1;
		else return(md);
		}
	return(NULL);
}

void FUNC gwar()
{
struct	cmd *cmdptr;
char	*mv0ptr;

if (margc == 0)
	{
	warnop();
	}
else
	{
	logthis(spr("Input Ch# %d Margc %d",usrnum,margc));

	if (margc > 0)
		{
		sprintf(gechrbuf,"Margv[0] [%s]",margv[0]);
		logthis(gechrbuf);
		}
	if (margc > 1)
		{
		sprintf(gechrbuf,"Margv[1] [%s]",margv[1]);
		logthis(gechrbuf);
		}
	if (margc > 2)
		{
		sprintf(gechrbuf,"Margv[2] [%s]",margv[2]);
		logthis(gechrbuf);
		}
	if (margc > 3)
		{
		sprintf(gechrbuf,"Margv[3] [%s]",margv[3]);
		logthis(gechrbuf);
		}
	if (margc > 4)
		{
		sprintf(gechrbuf,"Margv[4] [%s]",margv[4]);
		logthis(gechrbuf);
		}

	for (mv0ptr = margv[0]; *mv0ptr != '\0'; mv0ptr++)
		{
		*mv0ptr = tolower(*mv0ptr);
		}
	if ((cmdptr = gesearch(margv[0], gecmds, GECMDSIZ)) != NULL)
		{
#ifdef PHARLAP
		if (!hasmkey(PLAYKEY) && cmdptr->cando == 0)
#else
		if (usrptr->class < PAYING && cmdptr->cando == 0)
#endif
			{
			prfmsg(NOCANDO);
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			(*(cmdptr->func))();
			}
		}
	else
		{
		prfmsg(INVCMD);
		outprfge(ALWAYS,usrnum);
		}
	}
}


/**************************************************************************
** Blank line was input response                                         **
**************************************************************************/

void FUNC warnop()
{
prfmsg(FORHELP);
outprfge(ALWAYS,usrnum);
}



void FUNC cmd_gehelp()
{
int	ndx,i,syshelp;

char	gechrbuf4[20], gechrbuf5[20], gechrbuf6[20], gechrbuf7[20];

#ifdef PHARLAP
if ((!syscmds) || (sysonly && !(hasmkey(SYSKEY))))
#else
if ((!syscmds) || (sysonly && !(usrptr->flags&ISYSOP)))
#endif
	syshelp = FALSE;
else
	syshelp = TRUE;

setmbk(gehlpmb);

if (margc < 2 || margc > 3)
	{
	prfmsg(HLPINDEX);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (genearas(margv[1],"version"))
	{
	prf(VERSION);
	prf("\r\r");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (genearas(margv[1],"sys") && syshelp == FALSE)
	{
	prfmsg(HLPINDEX);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (genearas(margv[1],"class"))
	{
	if (margc == 2)
		{
		prfmsg(HLPCLS1);
		for (i=0;i<cyb_class;++i)
			{
			if (shipclass[i].max_type == CLASSTYPE_USER)
				{
				if (shipclass[i].max_tons >999999L)
					sprintf(gechrbuf,"%ldm",shipclass[i].max_tons/1000000L);
				else
				if (shipclass[i].max_tons >999)
					sprintf(gechrbuf,"%ldk",shipclass[i].max_tons/1000);
				else
					sprintf(gechrbuf,"%ld",shipclass[i].max_tons);

				if (shipclass[i].max_price >999999L)
					sprintf(gechrbuf2,"%ldm",shipclass[i].max_price/1000000L);
				else
				if (shipclass[i].max_price >999)
					sprintf(gechrbuf2,"%ldk",shipclass[i].max_price/1000);
				else
					sprintf(gechrbuf2,"%ld",shipclass[i].max_price);

				if (shipclass[i].scanrange >999999L)
					sprintf(gechrbuf3,"%ldm",shipclass[i].scanrange/1000000L);
				else
				if (shipclass[i].scanrange >999)
					sprintf(gechrbuf3,"%ldk",shipclass[i].scanrange/1000);
				else
					sprintf(gechrbuf3,"%ld",shipclass[i].scanrange);

				if (shipclass[i].max_accel >999)
					sprintf(gechrbuf4,"%dk",shipclass[i].max_accel/1000);
				else
					sprintf(gechrbuf4,"%d",shipclass[i].max_accel);

				if (shipclass[i].max_warp == 0)
					sprintf(gechrbuf5,"%s N",CLR_RED1);
				else
					sprintf(gechrbuf5,"%d",shipclass[i].max_warp);

				if (shipclass[i].max_shlds == 0)
					sprintf(gechrbuf6,"%s N",CLR_RED1);
				else
					sprintf(gechrbuf6,"%d",shipclass[i].max_shlds);

				if (shipclass[i].max_phasr == 0)
					sprintf(gechrbuf7,"%s N",CLR_RED1);
				else
					sprintf(gechrbuf7,"%d",shipclass[i].max_phasr);

				prf("%s%2d %s%-24s %s%5s %s%2s %2s %2s %s %s %s %s %s %s %s %s %s%3s %4s %4s %5d\r",
					CLR_CYAN2, i+1,
					CLR_CYAN1, shipclass[i].typename,
					CLR_YELLOW1, gechrbuf2,
					CLR_WHITE2, gechrbuf5,
					gechrbuf6,
					gechrbuf7,
					shipclass[i].max_torps ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].max_missl ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].has_mine ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].max_attk ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].has_decoy ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].has_jam ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].has_zip ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					shipclass[i].max_cloak ? CLR_GREEN2 "Y" : CLR_RED1 "N",
					CLR_WHITE2, gechrbuf4,
					gechrbuf3,
					gechrbuf,
					shipclass[i].max_points
					);
				}
			}
		prfmsg(HLPCLS2);
		outprfge(ALWAYS,usrnum);
		return;
		}
	else
	if (margc == 3)
		{
		i = atoi(margv[2])-1;
		if (i >= 0 && i < cyb_class && shipclass[i].max_type == CLASSTYPE_USER)
			{
			setmbk(geshmb);
			prfmsg(shipclass[i].hlpmsg);
			outprfge(ALWAYS,usrnum);
			return;
			}
		else
			{
			prfmsg(HLPCLS3);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	else
		{
		prfmsg(HLPINDEX);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}

ndx = 0;
while (gehlp[ndx].command != NULL)
	{
	if (genearas(margv[1], gehlp[ndx].command))
		{
		prfmsg(gehlp[ndx].helptxt);
		outprfge(ALWAYS,usrnum);
		return;
		}
	++ndx;
	}

prfmsg(HLPINDEX);
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Fire engines on impulse                                               **
**************************************************************************/

void FUNC cmd_impulse()
{
unsigned deg;

if (margc < 2 || margc > 3)
	{
	prfmsg(FORMAT,"IMPULSE");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(IMPULSE1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDNAVG);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 3)
	strcpy(gechrbuf,margv[2]);
else
	strcpy(gechrbuf,"0");
strcpy(gechrbuf2,gechrbuf);
if (*gechrbuf == '@')
	{
	*gechrbuf = '+';
	deg = atoi(gechrbuf);
	if (deg > 359)
		{
		prfmsg(NUMOOR,0,359);
		outprfge(ALWAYS, usrnum);
		return;
		}
	}

if (valpcnt(margv[1],0,99))
	{
	if (warsptr->helm == 0)
		{
		if (*gechrbuf2 != '@')
			if (!valdegree(gechrbuf))
				return;
		if (warsptr->where >= 10)
			{
			refresh(warsptr,usrnum);
			prfmsg(LEAVEORB);
			warsptr->where = 0;
			warsptr->repair = 0;
			}
		if (*gechrbuf2 != '@')
			{
			if (warsptr->degrees != 0)
				deg = (unsigned)normal(warsptr->heading + (double)warsptr->degrees);
			else
				deg = warsptr->head2b;
			}
		prfmsg(ENGFIRE,deg);
		outprfge(ALWAYS,usrnum);
		warsptr->speed2b = 1000.0 * ((double)warsptr->percent/100.0);
		if (deg != warsptr->head2b)
			warsptr->head2b = (double)deg;
		}
	else
		{
		prfmsg(HLBROKE);
		if (warsptr->speed != 0)
			prfmsg(HLBROKE2);
		outprfge(ALWAYS,usrnum);
		}
	}
}


/**************************************************************************
** Fire engines on warp drive                                            **
**************************************************************************/

void FUNC cmd_warp()
{
unsigned deg;
int	speed,topspeed,cap;

if (shipclass[warsptr->shpclass].max_warp == 0 && atoi(margv[1]) != 0)
	{
	prfmsg(WARP01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->topspeed == 0 && atoi(margv[1]) != 0)
	{
	prfmsg(WARPSPD2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDNAVG);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc < 2 || margc > 3)
	{
	prfmsg(FORMAT,"WARP");
	outprfge(ALWAYS,usrnum);
	}
else
	{
	speed = atoi(margv[1]);
	topspeed = warsptr->topspeed;
	cap = topspeed+(topspeed/2);
	if (speed < 0)
		{
		prfmsg(FORMAT,"WARP");
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* if requsted speed is over 150% cruising speed and over current speed */
	if (speed > cap && speed > warsptr->speed/1000)
		{
		/* if current speed is over 100% cruising speed */
		if (warsptr->speed/1000 > warsptr->topspeed)
			{
			/* if current speed is over 150% cruising speed, don't change */
			if (warsptr->speed/1000 > cap)
				{
				prfmsg(WARPSPD3);
				outprfge(ALWAYS,usrnum);
				return;
				}
			/* if not, change to 150% cruising speed */
			else
				{
				speed = cap;
				prfmsg(WARPSPD4,speed);
				outprfge(ALWAYS,usrnum);
				}
			}
		else
			{
			prfmsg(WARP03);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}

	if (margc == 3)
		strcpy(gechrbuf,margv[2]);
	else
		strcpy(gechrbuf,"0");
	strcpy(gechrbuf2,gechrbuf);
	if (*gechrbuf == '@')
		{
		*gechrbuf = '+';
		deg = atoi(gechrbuf);
		if (deg > 359)
			{
			prfmsg(NUMOOR,0,359);
			outprfge(ALWAYS, usrnum);
			return;
			}
		}
	if (warsptr->helm == 0)
		{
		if (*gechrbuf2 != '@')
			if (!valdegree(gechrbuf))
				return;
		if (speed > topspeed && warsptr->speed/1000 <= topspeed)
			{
			prfmsg(WARP04,topspeed);
			outprfge(ALWAYS,usrnum);
			}

		if (warsptr->where >= 10)
			{
			refresh(warsptr,usrnum);
			prfmsg(LEAVEORB);
			warsptr->where = 0;
			warsptr->repair = 0;
			}

		if (*gechrbuf2 != '@')
			{
			if (warsptr->degrees != 0)
				deg = (unsigned)normal(warsptr->heading + (double)warsptr->degrees);
			else
				deg = warsptr->head2b;
			}
		prfmsg(ENGFIRE,deg);
		outprfge(ALWAYS,usrnum);
		if (shipclass[warsptr->shpclass].max_accel >= 1000 && warsptr->speed < 1000.0 && speed != 0)
			warsptr->speed = 0.0;	/* no fractional warp speeds */
		warsptr->speed2b = 1000.0 * (float)speed;
		if (deg != warsptr->head2b)
			warsptr->head2b = (double)deg;
		}
	else
		{
		prfmsg(HLBROKE);
		if (warsptr->speed != 0)
			prfmsg(HLBROKE2);
		outprfge(ALWAYS,usrnum);
		}
	}
}

/**************************************************************************
** Emergency stop                                                        **
**************************************************************************/

void FUNC cmd_stop()
{
if (warsptr->helm < 0 && warsptr->speed > 0)
	prfmsg(EMERSTOP);
else
	prfmsg(ENGFIRE,(unsigned)warsptr->head2b);

outprfge(ALWAYS,usrnum);
warsptr->speed2b = 0;
}

/**************************************************************************
** Rotate ship                                                           **
**************************************************************************/

void FUNC cmd_rotate()
{
unsigned deg;


if (margc != 2)
	{
	prfmsg(FORMAT,"ROTATE");
	outprfge(ALWAYS,usrnum);
	}
else
if (*margv[1] == '@') /* turn absolute */
	{
	if (warsptr->helm == 0)
		{
		*margv[1] = '+';
		deg = atoi(margv[1]);
		if (deg < 360)
			{
			if (deg == warsptr->heading)
				{
				prfmsg(NOWALRD,deg);
				}
			else
				{
				prfmsg(NOWTURN,deg);
				warsptr->head2b = (double)deg;
				}
			outprfge(ALWAYS,usrnum);
			}
		else
			{
			prfmsg(NUMOOR,0,359);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	else
		{
		prfmsg(HLBROKE);
		if (warsptr->speed != 0)
			prfmsg(HLBROKE2);
		outprfge(ALWAYS,usrnum);
		}
	}
else
if (valdegree(margv[1]))
	{
	if (warsptr->helm == 0)
		{
		if (warsptr->degrees == 0)
			{
			prfmsg(NOWALRD,(int)(warsptr->heading+.5));
			}
		else
			{
			deg = (unsigned)normal(warsptr->heading + (double)warsptr->degrees);
			prfmsg(NOWTURN,deg);
			warsptr->head2b = (double)deg;
			}
		outprfge(ALWAYS,usrnum);
		}
	else
		{
		prfmsg(HLBROKE);
		if (warsptr->speed != 0)
			prfmsg(HLBROKE2);
		outprfge(ALWAYS,usrnum);
		}
	}
}



/**************************************************************************
** Recharge engines                                                      **
**************************************************************************/

void FUNC cmd_flux()
{
fluxstat(warsptr,usrnum,65535U);
}

/**************************************************************************
** Orbit a planet                                                        **
**************************************************************************/

void FUNC cmd_orbit()

{
int got_plt;

if (margc != 2)
	{
	prfmsg(FORMAT,"ORBIT");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(ORBIT4);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where >= 10)
	{
	prfmsg(ORBIT3);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = (atoi(margv[1]));

if (plnum <= MAXPLANETS && plnum > 0)
	{
	got_plt = getplanetdat(usrnum);
	if (got_plt)
		{
		if (plptr->type == PLTYPE_WORM)
			{
			prfmsg(ORBIT0);
			outprfge(ALWAYS,usrnum);
			return;
			}
		distance = (unsigned)(cdistance(&warsptr->coord,&plptr->coord)*10000);
		if (distance <= 250)
			{
			if (strlen(plptr->name) == 0)
				{
				prfmsg(ORBIT1N,plnum);
				}
			else
				{
				prfmsg(ORBIT1,plnum,plptr->name);
				}
			warsptr->where = 10 + plnum;
			warsptr->speed = 0;
			warsptr->speed2b = 0;
			}
		else
			{
			prfmsg(ORBIT2);
			}
		}
	else
		{
		prfmsg(NOPLNT);
		}
	}
else
	{
	prfmsg(NOPLNT);
	}
outprfge(ALWAYS,usrnum);
}




/**************************************************************************
** Fire phasers                                                          **
**************************************************************************/

void FUNC cmd_phas()

{

if (shipclass[warsptr->shpclass].max_phasr == 0)
	{
	prfmsg(PHASER0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->phasr < 0)
	{
	prfmsg(PHBROKE);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	if (margc == 2)
		{
		if (valdegree(margv[1]))
			{
			if (warsptr->hypha == 0)
				{
				firehp(warsptr,usrnum);
				}
			else
				{
				prfmsg(HPWAIT);
				outprfge(ALWAYS,usrnum);
				}
			}
		}
	else
		{
		prfmsg(FORMAT,"HYPER");
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	if (margc == 2)
		{
		if (warsptr->phasr >=PMINFIRE)
			{
			if (valdegree(margv[1]))
				{
				warsptr->percent = 1;
				firep(warsptr,usrnum);
				}
			}
		else
			{
			prfmsg(PHANONE);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
	if (margc == 3)
		{
		if (warsptr->phasr >=PMINFIRE)
			{
			if (valpcnt(margv[2],0,5) && valdegree(margv[1]))
				{
				firep(warsptr,usrnum);
				}
			}
		else
			{
			prfmsg(PHANONE);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
		{
		prfmsg(FORMAT,"PHASER");
		outprfge(ALWAYS,usrnum);
		}
	}
}

void FUNC firep(ptr,usrn)
WARSHP	*ptr;
int	usrn;
{
WARSHP	*wptr;
WARUSR	*wuptr;

unsigned deg;
double factor;

int hitone;

hitone = FALSE;

if (ptr->cloak > 0 )
	{
	prfmsg(PCLOKUP,"The phasers are");
	outprfge(ALWAYS,usrn);
	return;
	}

if (ptr->damage >= 100) /* no firing in the brief period between going over 100 and blowing up */
	{
	prfmsg(RNDPHSR);
	outprfge(ALWAYS,usrn);
	return;
	}

if (ptr->shieldstat == SHIELDUP)
	{
	shielddn(ptr,usrn);
	}

if (ptr->phasr >=PMINFIRE)
	{
	if (neutral(&ptr->coord))
		{
		zaphim(ptr,usrn);
		prfmsg(FRCTER);
		outprfge(ALWAYS,usrn);
		return;
		}
	deg = (unsigned)(normal(ptr->heading + (double)ptr->degrees) + 0.5);
	prfmsg(PFIRED,(int)ptr->phasr,ptr->percent);
	outprfge(FILTER,usrn);
	for (othusn=0 ; othusn < nships ; othusn++)
		{
		wptr=warshpoff(othusn);
		ddistance = cdistance(&ptr->coord,&wptr->coord)*10000;
		if (ingegame(othusn) && (wptr->where != 1 || ptr->phasrtype >= phatowrp) && ddistance < 100000.0)
			{
			if (othusn != usrn && (shipclass[wptr->shpclass].faction != shipclass[ptr->shpclass].faction
				|| shipclass[ptr->shpclass].faction == 0) && (wptr->distress == 255 || wptr->distress == usrn || ptr->lock == othusn))
				{
				heading = (unsigned)(vector(&ptr->coord,&wptr->coord) + 0.5);
				if (smallest(heading,deg) < ptr->percent+PHABIAS)
					{
					factor = pdamage(ptr,ddistance,ptr->percent);

					/* sprintf(gechrbuf3,"%f",factor);
					prf("pdamage returns: %s\r",gechrbuf3); */

					factor *= 0.5 + (double)ptr->phasrtype / 2.0;

					/* sprintf(gechrbuf3,"%f",factor);
					prf("after phasrtype adjustment: %s\r",gechrbuf3); */

					factor = ton_fact(wptr,factor);

					/* sprintf(gechrbuf3,"%f",factor);
					prf("after ton_fact adjustment: %s\r",gechrbuf3);
					outprfge(ALWAYS,usrn); */

					/* lower it for hyper */
					if (wptr->where == 1)
						factor = factor /2.0;

					if (factor > 0.0)
						{
						if (neutral(&wptr->coord))
							{
							prfmsg(PDEFNEUT,username(wptr));
							outprfge(ALWAYS,usrn);
							}
						else
							{
							if (factor < 1.0)	/* hit, but no damage */
								factor = 0.0;
							hitone = TRUE;
							/* prioritize user hits over npcs so users get credit */
							if (wptr->damage < 100 || (wptr->lastfired < nships && warshpoff(wptr->lastfired)->status == GESTAT_AUTO && ptr->status == GESTAT_USER))
								wptr->lastfired = usrn;
							wptr->cantexit = FIRETICKS;
							ptr->cantexit = FIRETICKS;
							if (wptr->status == GESTAT_AUTO)	/* if npc... */
								{
								wptr->cybmine = usrn;	/* engage user */
								wptr->tick = 2;		/* do it fast */
								wptr->npcmsg = 255;	/* reset annoy msg tracking */
								}
							if (wptr->shieldstat != SHIELDUP)
								{
								damstr((int)factor);
								if (wptr->status == GESTAT_AUTO)
									prfmsg(PHITNPC,gechrbuf,username(wptr));
								else
									prfmsg(PHITHIM,gechrbuf,username(wptr));
								outprfge(ALWAYS,usrn);
								if (ptr->status == GESTAT_AUTO)
									prfmsg(PNPCHIT,username(ptr),gechrbuf);
								else
									prfmsg(PHITYOU,username(ptr),gechrbuf);
								outprfge(ALWAYS,othusn);
								/* cap npc-on-npc phasers so big ships don't get one shot kills */
								if (ptr->status == GESTAT_AUTO && wptr->status == GESTAT_AUTO &&
									factor >= (double)((shipclass[ptr->shpclass].tough_factor+1)*5+5))
									wptr->damage += (double)((shipclass[ptr->shpclass].tough_factor+1)*5+(gernd()%5)+1);
								else
									wptr->damage += factor;
								wuptr = warusroff(usrn);
								set_dislike(wuptr,shipclass[wptr->shpclass].faction,(int)factor);
								}
							else
								{
								shieldhit(wptr,othusn,(int)factor); /* modify the damage */
								wuptr = warusroff(usrn);
								set_dislike(wuptr,shipclass[wptr->shpclass].faction,2);
								if (wptr->status == GESTAT_AUTO)
									prfmsg(PDEFLNPC,username(wptr));
								else
									prfmsg(PDEFLECT,username(wptr));
								outprfge(ALWAYS,usrn);
								if (ptr->status == GESTAT_AUTO)
									prfmsg(PNPCDEF,username(ptr),(int)factor);
								else
									prfmsg(PHITDEF,username(ptr),(int)factor);
								outprfge(ALWAYS,othusn);
								}
							randamage(wptr,othusn); /*assess any random damage */
							}
						}
					}
				}
			}
		}

	if (hitone == TRUE || ptr->status == GESTAT_USER)	/* if NPC, don't actually fire unless doing damage */
		ptr->phasr = 0;
	}
else
	{
	prfmsg(PHANONE);
	outprfge(ALWAYS,usrn);
	}
}


void FUNC firehp(ptr,usrn)
WARSHP	*ptr;
int	usrn;
{
WARSHP	*wptr;
WARUSR	*wuptr;
unsigned deg;
double factor;


if (ptr->damage >= 100)
	{
	prfmsg(RNDPHSR);
	outprfge(ALWAYS,usrn);
	return;
	}

if (fluxstat(ptr,usrn,HPFIRAMT) == 1)
	{
	if (neutral(&ptr->coord))
		{
		zaphim(ptr,usrn);
		prfmsg(FRCTER);
		outprfge(ALWAYS,usrn);
		return;
		}
	deg = (unsigned)normal(ptr->heading + (double)ptr->degrees);
	prfmsg(HPFIRED,deg);
	outprfge(FILTER,usrn);
	ptr->energy -= HPFIRAMT;
	ptr->hypha = 1;
	for (othusn=0 ; othusn < nships ; othusn++)
		{
		wptr=warshpoff(othusn);
		ddistance = cdistance(&ptr->coord,&wptr->coord)*10000;
		if (ingegame(othusn) && wptr->where == 1 && ddistance < 100000.0)
			{
			if (othusn != usrn && (shipclass[wptr->shpclass].faction != shipclass[ptr->shpclass].faction
				|| shipclass[ptr->shpclass].faction == 0) && (wptr->distress == 255 || wptr->distress == usrn || ptr->lock == othusn))
				{
				heading = (unsigned)vector(&ptr->coord,&wptr->coord);
				if (smallest(heading,deg) < HPBEAMW)
					{
					if (ddistance < (double)shipclass[ptr->shpclass].scanrange)
						{
						factor = pdamage(ptr,ddistance,0);

						/* sprintf(gechrbuf3,"%f",factor);
						prf("pdamage returns: %s\r",gechrbuf3); */

						factor *= 0.5 + (double)ptr->phasrtype / 2.0;

						/* sprintf(gechrbuf3,"%f",factor);
						prf("after phasrtype adjustment: %s\r",gechrbuf3); */

						factor = ton_fact(wptr,factor);

						/* sprintf(gechrbuf3,"%f",factor);
						prf("after ton_fact adjustment: %s\r",gechrbuf3);
						outprfge(ALWAYS,usrn); */

						if (factor > 0.0)
							{
							if (neutral(&wptr->coord))
								{
								prfmsg(PDEFNEUT,username(wptr));
								outprfge(ALWAYS,usrn);
								}
							else
								{
								if (factor < 1.0)	/* hit, but no damage */
									factor = 0.0;
								damstr((int)factor);

								if (wptr->status == GESTAT_AUTO)
									prfmsg(HPHITN,gechrbuf,username(wptr));
								else
									prfmsg(HPHITM,gechrbuf,username(wptr));
								outprfge(ALWAYS,usrn);
								if (ptr->status == GESTAT_AUTO)
									prfmsg(HPNHITU,username(ptr),gechrbuf);
								else
									prfmsg(HPHITU,username(ptr),gechrbuf);
								outprfge(ALWAYS,othusn);
								/* prioritize user hits over npcs so users get credit */
								if (wptr->damage < 100 || (wptr->lastfired < nships && warshpoff(wptr->lastfired)->status == GESTAT_AUTO && ptr->status == GESTAT_USER))
									wptr->lastfired = usrn;
								/* cap npc-on-npc phasers so big ships don't get one shot kills */
								if (ptr->status == GESTAT_AUTO && wptr->status == GESTAT_AUTO &&
									factor >= (double)((shipclass[ptr->shpclass].tough_factor+1)*5+5))
									wptr->damage += (double)((shipclass[ptr->shpclass].tough_factor+1)*5+(gernd()%5)+1);
								else
									wptr->damage += factor;
								wuptr = warusroff(usrn);
								set_dislike(wuptr,shipclass[wptr->shpclass].faction,(int)factor);
								if (wptr->status == GESTAT_AUTO)	/* if npc... */
									{
									wptr->cybmine = usrn;	/* engage user */
									wptr->tick = 2;		/* do it fast */
									wptr->npcmsg = 255;	/* reset annoy msg tracking */
									}
								wptr->cantexit = FIRETICKS;
								ptr->cantexit = FIRETICKS;
								randamage(wptr,othusn); /*assess any random damage */
								}
							}
						}
					}
				}
			}
		}
	}
else
	{
	prfmsg(HNOFIRP);
	outprfge(ALWAYS,usrn);
	}
}


/**************************************************************************
** Fire torpedoes                                                        **
**************************************************************************/

void FUNC cmd_torp()

{

int shpnum;

lockwarn = TRUE;

if (shipclass[warsptr->shpclass].max_torps == 0)
	{
	prfmsg(TORP3);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(TORP2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP,"The torpedo launcher is");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shielddn(warsptr,usrnum);
	}

if (warsptr->items[I_TORPEDO] == 0)
	{
	prfmsg(NOTORPS);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margv[1] == NULL)
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc < 2)
	{
	prfmsg(FORMAT,"TORPEDO");
	outprfge(ALWAYS,usrnum);
	return;
	}

shpnum = findshp(margv[1],1);

if (shpnum == usrnum)
	{
	prfmsg(FRCTER);
	outprfge(ALWAYS,usrnum);
	}
else
if ( shpnum >= 0)
	{
	if (neutral(&warsptr->coord))
		{
		zaphim(warsptr,usrnum);
		prfmsg(FRCTER);
		outprfge(ALWAYS,usrnum);
		return;
		}
	torp(warsptr,usrnum,shpnum);
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shieldup(warsptr,usrnum);
	}
}

int FUNC torp(ptr,usrn,shpnum)
WARSHP	*ptr;
int	usrn;
int	shpnum;

{
WARSHP	*wptr;

int	i;

if (ptr->damage >= 100)
	{
	prfmsg(RNDFCNT);
	outprfge(ALWAYS,usrn);
	return(0);
	}

if (lockon(ptr,0,shpnum,usrn) == 1)
	{
	for (i=0; i<MAXTORPS;++i)
		{
		wptr = warshpoff(shpnum);
		if (wptr->ltorps[i].distance == 0)
			{
			prfmsg(TFIRE1);
			outprfge(FILTER,usrn);
			--ptr->items[I_TORPEDO];
			prfmsg(TFIRE2,shpltr(shpnum,usrn));
			outprfge(FILTER,shpnum);
			wptr->ltorps[i].distance = (unsigned)(cdistance(&ptr->coord,&(wptr->coord))*10000);
			wptr->ltorps[i].distance += 20;
			wptr->ltorps[i].channel = (unsigned char)usrn;
			wptr->cantexit = FIRETICKS;
			ptr->cantexit = FIRETICKS;
			return(1);
			}
		}
	prfmsg(TORMANY,MAXTORPS);
	outprfge(FILTER,usrn);
	return(0);
	}
return(0);
}

/**************************************************************************
** Fire missile                                                          **
**************************************************************************/

void FUNC cmd_missl()

{

int shpnum;
unsigned energy;

lockwarn = TRUE;

if (shipclass[warsptr->shpclass].max_missl == 0)
	{
	prfmsg(MISS01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0)
	{
	prfmsg(PCLOKUP,"The missile launcher is");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->shieldstat == SHIELDUP)
	shielddn(warsptr,usrnum);

if (warsptr->items[I_MISSILE] == 0)
	{
	prfmsg(NOMISSL);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margv[1] == NULL && margc == 3)
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc < 2)
	{
	prfmsg(FORMAT,"MISSILE");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 2)
	energy = 5000;
else
	{
	if (atol(margv[2]) > 20000L)
		energy = 20000;
	else
	if (atol(margv[2]) < 2000L)
		energy = 2000;
	else
		energy = (unsigned)(atol(margv[2]));
	}

if (fluxstat(warsptr,usrnum,energy) == 0)
	{
	prfmsg(MISSHRT);
	outprfge(ALWAYS,usrnum);
	return;
	}

shpnum = findshp(margv[1],1);

if (shpnum == usrnum)
	{
	prfmsg(FRCTER);
	outprfge(ALWAYS,usrnum);
	}
else
if (shpnum >= 0)
	{
	if (neutral(&warsptr->coord))
		{
		zaphim(warsptr,usrnum);
		prfmsg(FRCTER);
		outprfge(ALWAYS,usrnum);
		return;
		}
	misl(warsptr,usrnum,shpnum,energy,energy);
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}

if (warsptr->shieldstat == SHIELDUP)
	{
	shieldup(warsptr,usrnum);
	}
}

int FUNC misl(ptr,usrnum,shpnum,energy,eng_flu)
WARSHP	*ptr;
int	usrnum, shpnum;
unsigned energy, eng_flu;

{
WARSHP *wptr;
int i;

if (ptr->damage >= 100)
	{
	prfmsg(RNDFCNT);
	outprfge(ALWAYS,usrnum);
	return(0);
	}

if (lockon(ptr,1,shpnum,usrnum) == 1)
	{
	for (i=0; i<MAXMISSL;++i)
		{
		wptr=warshpoff(shpnum);
		if (wptr->lmissl[i].distance == 0)
			{
			prfmsg(MFIRE1,energy);
			outprfge(FILTER,usrnum);
			--(ptr->items[I_MISSILE]);
			ptr->energy -= eng_flu;
			prfmsg(MFIRE2,shpltr(shpnum,usrnum));
			outprfge(FILTER,shpnum);
			wptr->lmissl[i].distance = (unsigned)(cdistance(&ptr->coord,&(wptr->coord))*10000);
			wptr->lmissl[i].distance += 20;
			wptr->lmissl[i].channel = (unsigned char)usrnum;
			wptr->lmissl[i].energy = energy;
			wptr->cantexit = FIRETICKS;
			ptr->cantexit = FIRETICKS;
			return(1);
			}
		}
	prfmsg(MISMANY,MAXMISSL);
	outprfge(FILTER,usrnum);
	return(0);
	}
return(0);
}

int FUNC lockon(ptr,type,ship,usrn)
WARSHP	*ptr;
int	type,ship,usrn;
{
WARSHP	*wptr;

double dist,speed,fact=0.0;

if (ptr->firecntl > 0)
	{
	prfmsg(FCBROKE);
	outprfge(ALWAYS,usrn);
	return(0);
	}

if (warsptr->jam_sev > (byte)2)
	{
	prfmsg(JAMMER4W);
	outprfge(ALWAYS,usrn);
	return(0);
	}

wptr= warshpoff(ship);

if (neutral(&(wptr->coord)))
	{
	prfmsg(FCNONO);
	outprfge(ALWAYS,usrn);
	return(0);
	}

dist = cdistance(&ptr->coord,&(wptr->coord));
if (wptr->cloak < 10 && (dist*10000.0) < (double)shipclass[warsptr->shpclass].scanrange)
	{
	speed = ptr->speed + wptr->speed;

	if (type == 0) /* torpedo */
		{
		if (wptr->speed > 999)
			{
			fact = 0.0;
			}
		else
			{
			fact = (1.2-(speed/5000));
			fact *= ((5.0-dist)/tor_fact);
			}
		}

	if (type == 1) /* missile */
		fact = ((5.0-dist)/mis_fact);


	if (fact > .7)
		{
		if (lockwarn == TRUE)
			{
			prfmsg(LOCK2,shpltr(ship,usrn));
			outprfge(FILTER,ship);
			}
		lockwarn = TRUE;
		if (wptr->status == GESTAT_AUTO)	/* if npc... */
			{
			wptr->cybmine = usrn;	/* engage user */
			wptr->tick = 2;		/* do it fast */
			wptr->npcmsg = 255;	/* reset annoy msg tracking */
			}
		return(1);
		}
	else
		{
		if (lockwarn == TRUE)
			{
			prfmsg(LOCK3,shpltr(usrn,ship));
			outprfge(FILTER,usrn);
			if (warshpoff(usrn)->status == GESTAT_USER)
				{
				prfmsg(LOCK4,shpltr(ship,usrn));
				outprfge(FILTER,ship);
				}
			}
		lockwarn = TRUE;
		return(0);
		}
	}
else
	{
	prfmsg(LOCK5,shpltr(usrn,ship));
	outprfge(ALWAYS,usrn);
	return(0);
	}
}

int FUNC findshp(ptr,type)
char	*ptr;
int	type; /* 0 = this sector only, 1 = everywhere */
{
char	letter;
int	shpnum,i;
double	dist;
WARSHP	*wptr;
shpnum = -1;
if (ptr[0] == '@')
	{
	if (warsptr->lock < 0 || warsptr->lock >= nships)
		{
		warsptr->lock = -1;
		prfmsg(NOLOCK);
		return(-1);
		}
	else
		{
		shpnum = warsptr->lock;
		if (!ingegame(shpnum))
			{
			warsptr->lock = -1;
			prfmsg(NOLOCK);
			return(-1);
			}
		wptr=warshpoff(shpnum);
		dist = cdistance(&warsptr->coord,&(wptr->coord));
		if ((dist*10000) > (double)shipclass[warsptr->shpclass].scanrange)
			{
			warsptr->lock = -1;
			prfmsg(NOLOCK);
			shpnum = -1;
			}
		else
			{
			shpnum = warsptr->lock;
			}
		}
	}
else
	{
	letter = toupper(*ptr);
	for(i=0;i<NOSCANTAB;++i)
		{
		if (scantab[usrnum].ship[i].letter == letter)
			{
			shpnum = scantab[usrnum].ship[i].shipno;
			break;
			}
		}

	if (i>=NOSCANTAB)
		return(-1);

	}

if (!ingegame(shpnum))
	{
	return(-1);
	}

wptr=warshpoff(shpnum);

if (type == 0)
	{
	if (samesect(&(wptr->coord), &warsptr->coord)
		&& wptr->cloak < 10)
		{
		return (shpnum);
		}
	else
		{
		return (-1);
		}
	}
else
if (type == 1)
	{
	if (wptr->cloak < 10)
		{
		return (shpnum);
		}
	else
		{
		return (-1);
		}
	}
return(-1);
}

/* firing in sector NEUTRAL is a big no-no */

void FUNC zaphim(ptr,usrn)
WARSHP *ptr;
int usrn;
{
damstr(se100dam);
prfmsg(ZAPHIM1,gechrbuf);
outprfge(ALWAYS,usrn);
ptr->damage += se100dam;
}

/**************************************************************************
** Fire decoys                                                           **
**************************************************************************/

void FUNC cmd_decoy()

{

int i;


if (!shipclass[warsptr->shpclass].has_decoy)
	{
	prfmsg(DECOY0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(DECOY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP,"The decoy launcher is");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDDECY);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_DECOYS] == 0)
	{
	prfmsg(NODECOYS);
	outprfge(ALWAYS,usrnum);
	return;
	}

for (i=0; i<10;++i)
	{
	if (warsptr->decout[i] == 0)
		{
		--warsptr->items[I_DECOYS];
		warsptr->decout[i] = DECOYTIME;
		prfmsg(DECFIRE);
		outprfge(FILTER,usrnum);
		return;
		}
	}
prfmsg(DECMANY);
outprfge(FILTER,usrnum);
}


/**************************************************************************
** Launch Jammer                                                         **
**************************************************************************/

void FUNC cmd_jammer()

{

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP,"The jammer launcher is");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (!shipclass[warsptr->shpclass].has_jam)
	{
	prfmsg(JAMMER0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_JAMMERS] == 0)
	{
	prfmsg(JAMMER1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDJAMR);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->jamload > 0 )
	{
	prfmsg(JAMMER6);
	outprfge(ALWAYS,usrnum);
	return;
	}

prfmsg(JAMMER2);
outprfge(FILTER,usrnum);
jam(warsptr,usrnum);
}

void FUNC jam(ptr,usrn)
WARSHP	*ptr;
int	usrn;
{
WARSHP	*wptr;
int	zothusn, sev, scaled;
double	ddist;

for (zothusn=0; zothusn < nships ; zothusn++)
	{
	wptr=warshpoff(zothusn);
	if (!ingegame(zothusn))
		continue;

	ddist = cdistance(&ptr->coord,&wptr->coord);
	ddist *= 10000;

	if (ddist > 50000.0)
		continue;

	sev = 10 - (int)(ddist / 5000.0);
	if (sev < 1)
		sev = 1;
	if (sev > 10)
		sev = 10;

	if (wptr->jam_sev < (byte)sev)
		{
		/* closer jammer, raise severity and reset timer */
		wptr->jam_sev  = (byte)sev;
		wptr->jam_time = (byte)jamtime;
		}
	else
		{
		/* farther/equal jammer, small refresh */
		if (wptr->jam_sev > 0)
			{
			scaled = (jamtime * sev + wptr->jam_sev/2) / wptr->jam_sev;
			if (scaled < 1)
				scaled = 1;
			if (wptr->jam_time < scaled)
				wptr->jam_time = (byte)scaled;
			}
		}

	if (wptr->jam_time > 0 && usrn != zothusn)
		{
		prfmsg(JAMMER3);
		outprfge(FILTER, zothusn);
		}
	}

--ptr->items[I_JAMMERS];
ptr->cantexit = FIRETICKS;
ptr->jamload = 1;
}


/**************************************************************************
** Launch Zipper                                                         **
**************************************************************************/

void FUNC cmd_zipper()

{

if (!shipclass[warsptr->shpclass].has_zip)
	{
	prfmsg(ZIPPER0);
	outprfge(ALWAYS,usrnum);
	return;
	}


if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP,"The zipper launcher is");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDZIPR);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_ZIPPERS] == 0)
	{
	prfmsg(ZIPPER1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->zipload > 0 )
	{
	prfmsg(ZIPPER4);
	outprfge(ALWAYS,usrnum);
	return;
	}

prfmsg(ZIPPER2);
outprfge(FILTER,usrnum);
zip(warsptr,usrnum);
}

void FUNC zip(ptr,usrn)
WARSHP	*ptr;
int	usrn;
{
MINE	*mptr;
int	i;
double	ddist;

usrn = usrn;

for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	if (mptr->channel != 255)
		{
		ddist = cdistance(&ptr->coord,&mptr->coord);
		ddist *= 10000;
		if (ddist < (double)shipclass[warsptr->shpclass].scanrange)
			{
			mptr->timer = 1; /* set mine to explode next tick */
			}
		}
	}
if (ptr->status == GESTAT_AUTO)
	prfmsg(ZIPPER3N,ptr->shipname);
else
	prfmsg(ZIPPER3,ptr->shipname);
outrange(FILTER,&ptr->coord);
--ptr->items[I_ZIPPERS];
ptr->cantexit = FIRETICKS;
ptr->zipload = 1;
}

/**************************************************************************
** Launch Mine                                                           **
**************************************************************************/

void FUNC cmd_mine()

{
int i;

if (!shipclass[warsptr->shpclass].has_mine)
	{
	prfmsg(MINE0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord))
	{
	prfmsg(MINE7);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 )
	{
	prfmsg(PCLOKUP,"The mine launcher is");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDMINE);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_MINE] <= 0)
	{
	warsptr->items[I_MINE] = 0;
	prfmsg(MINE1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc != 2 )
	{
	prfmsg(FORMAT,"MINE");
	outprfge(ALWAYS,usrnum);
	return;
	}

i = atoi(margv[1]);

if (i < 1 || i > 50)
	{
	prfmsg(FORMAT,"MINE");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (laymine(warsptr,usrnum,i) == 1)
	{
	prfmsg(MINE3,i);
	outprfge(FILTER,usrnum);
	return;
	}

prfmsg(MINE2,usermines);
outprfge(FILTER,usrnum);
}

/* split out so that cyb's can lay mines too */

int FUNC laymine(ptr,usrn,timer)
WARSHP	*ptr;
int	usrn;
int	timer;
{
int i,cnt;

/* count up the number of mines this player has layed */

cnt = 0;

for (i=0; i<nummines;++i)
	if (mines[i].channel == (byte)usrn)
		cnt++;

if (cnt >= usermines)
	{
	return(0);
	}

for (i=0; i<nummines;++i)
	{
	if (mines[i].channel == 255)
		{
		ptr->cantexit = FIRETICKS;
		mines[i].channel = (byte)usrn;
		mines[i].timer = timer;
		mines[i].coord.xcoord = ptr->coord.xcoord;
		mines[i].coord.ycoord = ptr->coord.ycoord;
		--ptr->items[I_MINE];
		return(1);
		}
	}
return(0);
}

/**************************************************************************
** Send a message to all                                                 **
**************************************************************************/

void FUNC cmd_send()
{
char letter;

if (margc > 2)
	{
	letter = toupper(*margv[1]);
	if (letter >= 'A' && letter <= 'C')
		{
		/* CHGD:MBM22e */
		if (pfnlvl >= 2 && profon)
			{
			prfmsg(MSGPRF);
			}
		else
			{
			if (warsptr->freq[letter-'A'] == 0)
				{
				rstrin();
				prfmsg(MSGSNT1,warsptr->shipname,letter,margv[2]);
				outwar(FILTER,usrnum,0);
				prfmsg(MSGSNT2);
				}
			else
			if (warsptr->freq[letter-'A'] < 20000)
				{
				rstrin();
				prfmsg(MSGSNT3,warsptr->shipname,letter,margv[2]);
				outsect(ALWAYS,&warsptr->coord,usrnum,warsptr->freq[letter-'A']);
				prfmsg(MSGSNT4,warsptr->freq[letter - 'A']);
				}
			else
			if (warsptr->freq[letter-'A'] >= 20000)
				{
				rstrin();
				prfmsg(MSGSNT5,warsptr->shipname,letter,margv[2]);
				outwar(ALWAYS,usrnum,warsptr->freq[letter-'A']);
				prfmsg(MSGSNT6,warsptr->freq[letter - 'A']);
				}
			}
		}
	else
		{
		prfmsg(BADCOM);
		}

	}
else
	{
	prfmsg(FORMAT,"SEND");
	}
outprfge(FILTER,usrnum);
}



/**************************************************************************
** Set Com Freq                                                          **
**************************************************************************/

void FUNC cmd_freq()
{
unsigned freq;
char letter;

if (margc < 3)
	{
	prfmsg(FORMAT,"FREQ");
	outprfge(ALWAYS,usrnum);
	return;
	}

letter = toupper(*margv[1]);

if (letter < 'A' || letter > 'C')
	{
	prfmsg(FORMAT,"FREQ");
	outprfge(ALWAYS,usrnum);
	return;
	}


if (sameas(margv[2],"hail"))
	{
	warsptr->freq[letter - 'A'] = 0;
	prfmsg(RADSET1,letter);
	outprfge(ALWAYS,usrnum);
	return;
	}

freq = atoi(margv[2]);

if (freq == 0)
	{
	prfmsg(FORMAT,"FREQ");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (freq < 20000)
	{
	warsptr->freq[letter - 'A'] = freq;
	prfmsg(RADSET2,letter,freq);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (freq >= 20000)
	{
	warsptr->freq[letter - 'A'] = freq;
	prfmsg(RADSET3,letter,freq);
	outprfge(ALWAYS,usrnum);
	return;
	}

}

/**************************************************************************
** Generate ships report                                                 **
**************************************************************************/

void FUNC cmd_report()
{

WARSHP	*ptr;
int	max,pcnt,i,none,zothusn;
double	ddist;

if (margc != 2 || (!sameas(margv[1],"nav") && !sameas(margv[1],"sys") && !sameas(margv[1],"inv") && !sameas(margv[1],"acc") && !sameas(margv[1],"ord")))
	{
	prfmsg(FORMAT,"REPORT");
	outprfge(ALWAYS,usrnum);
	return;
	}

energy	= (unsigned)warsptr->energy +.5;
damage	= (unsigned)warsptr->damage +.5;
speed	= ((unsigned)warsptr->speed  +.5);
heading	= (int)(warsptr->heading+.5);

prfmsg(REP01,shipclass[warsptr->shpclass].typename,warsptr->shipname);
prfmsg(DASHES);

if (sameas(margv[1],"nav"))
	{
	prfmsg(REP35);
	if (warsptr->where == 1)
		{
		setsect(warsptr);
		prfmsg(REP02,xsect, ysect);
		prfmsg(REP03, showarp(warsptr->speed));
		prfmsg(REP04, heading);
		}
	else
	if (warsptr->where == 0)
		{
		setsect(warsptr);
		prfmsg(REP05,xsect,ysect);
		prfmsg(REP06, showarp(warsptr->speed));
		prfmsg(REP07, heading);
		}
	else
	if (warsptr->where >= 10)
		{
		setsect(warsptr);
		prfmsg(REP08,warsptr->where - 10,xsect,ysect);
		}

	setsect(warsptr);

	prfmsg(REP32,xsect,ysect,xcord,ycord);
	if (warsptr->status == GESTAT_AUTO)
		{
		prfmsg(REP40,warsptr->status,warsptr->cybmine,warsptr->cybupdate);
		}
	}
else
if (sameas(margv[1],"sys"))
	{
	prfmsg(REP36);
	prfmsg(REP09,energy);
	if (shipclass[warsptr->shpclass].max_shlds != 0)
		{
		if (warsptr->shieldstat == SHIELDUP)
			{
			if (warsptr->shieldtype < 10)
				{
				prfmsg(REP10,warsptr->shieldtype);
				}
			else
				{
				prfmsg(REP10S,warsptr->shieldtype);
				}
			charge(warsptr,&max,&pcnt);
			prfmsg(REP11B,pcnt);
			}
		else
		if (warsptr->shieldstat == SHIELDDN || warsptr->shieldstat == SHIELDDM)
			{
			if (warsptr->shieldtype < 10)
				{
				prfmsg(REP11,warsptr->shieldtype);
				}
			else
				{
				prfmsg(REP11S,warsptr->shieldtype);
				}
			}
		}
	if (shipclass[warsptr->shpclass].max_phasr != 0)
		{
		if (warsptr->phasr >= PMINFIRE)
			{
			if (warsptr->phasrtype < 10)
				prfmsg(REP23,warsptr->phasrtype);
			else
				prfmsg(REP23S,warsptr->phasrtype);
			}
		else
		if (warsptr->phasr >= 0)
			{
			if (warsptr->phasrtype < 10)
				prfmsg(REP29,warsptr->phasrtype);
			else
				prfmsg(REP29S,warsptr->phasrtype);
			}
		else
			{
			if (warsptr->phasrtype < 10)
				prfmsg(REP24,warsptr->phasrtype);
			else
				prfmsg(REP24S,warsptr->phasrtype);
			}
		}

	prfmsg(REP24A,warsptr->freq[0],warsptr->freq[1],warsptr->freq[2]);



	if (shipclass[warsptr->shpclass].max_cloak == 1)
		{
		if (warsptr->cloak > 0)
			prfmsg(REP12);
		else
			prfmsg(REP13);
		}

	damage = (unsigned)(warsptr->damage+.5);
	damstr(damage);

	prfmsg(REP14,gechrbuf);
	if (warsptr->shieldstat == SHIELDDM)
		prfmsg(REP15);
	if (warsptr->helm < 0 )
		prfmsg(REP16);
	if (warsptr->cloak < 0 )
		prfmsg(REP17);
	if (warsptr->tactical < 0 )
		prfmsg(REP18);
	if (warsptr->phasr < 0 )
		prfmsg(REP21);
	if (warsptr->firecntl > 0)
		prfmsg(REP22);

	if (shipclass[warsptr->shpclass].max_warp != 0)
		{
		if (warsptr->topspeed == 0)
			prfmsg(REP19);
		else
		if (warsptr->speed/1000 > warsptr->topspeed)
			prfmsg(REP20A);
		else
		if (warsptr->overspeed > 0)
			prfmsg(REP20,warsptr->topspeed);
		}

	if (warsptr->repair > 0)
		prfmsg(REP18A,warsptr->repair);

	if (waruptr->rospos != 0)
		prfmsg(REP39,waruptr->rospos);

	}
else
if (sameas(margv[1],"inv"))
	{
	prfmsg(REP38);

	for (i=0; i<NUMITEMS; ++i)
		{
		if (warsptr->items[i] > 0)
			{
			sprintf(gechrbuf,"%s%s%16lu",item_name[i],gedots(22-strlen(item_name[i])),warsptr->items[i]);
			gechrbuf[0] = toupper(gechrbuf[0]);
			prf("%s\r",gechrbuf);
			}
		}

	sprintf(gechrbuf2,"%lu",calcweight(warsptr));
	prfmsg(REP37,gechrbuf2);
	}
else
if (sameas(margv[1],"acc"))
	{
	prfmsg(REP25);

	if (waruptr->planets == 0)
		prfmsg(REP26);
	else
		prfmsg(REP27,waruptr->planets);


	sprintf(gechrbuf,"%lu",waruptr->cash);
	prfmsg(REP28,gechrbuf);

	if (waruptr->score <= 0)
		sprintf(gechrbuf,"0");
	else
		sprintf(gechrbuf,"%lu",waruptr->score);

	prfmsg(REP30,gechrbuf);

	prfmsg(REP31,waruptr->kills);

	if (waruptr->teamcode > 0)
		{
		prfmsg(REP31A,teamname(waruptr));
		}
	}
else
if (sameas(margv[1],"ord"))
	{
	prfmsg(REP41);
	none = TRUE;
	prfmsg(REP42);
	for (i=0;i<MAXTORPS;++i)
		{
		if (warsptr->ltorps[i].distance != 0)
			{
			none = FALSE;
			if (warsptr->ltorps[i].channel < nships)
				{
				ptr=warshpoff(warsptr->ltorps[i].channel);
				prf("\r ");
				if (warsptr->lock == warsptr->ltorps[i].channel)
					prf(" %s*%s%s%s*%s  ",CLR_RED1,CLR_BLUE2,username(ptr),CLR_RED1,CLR_WHITE2);
				else
					prf("  %s%s%s   ",CLR_BLUE2,username(ptr),CLR_WHITE2);
				}
			if (warsptr->ltorps[i].channel == 255)
				prf("\r  (destroyed)   ");
			if (warsptr->jam_sev <= (byte)2)
				prf("Dist: %u",warsptr->ltorps[i].distance);
			else
				prf("Dist: ?????");
			}
		}
	if (none == TRUE)
		prf("none\r");
	else
		prf("\r");
	none = TRUE;
	prfmsg(REP43);
	for (i=0;i<MAXMISSL;++i)
		{
		if (warsptr->lmissl[i].distance != 0)
			{
			none = FALSE;
			if (warsptr->lmissl[i].channel < nships)
				{
				ptr=warshpoff(warsptr->lmissl[i].channel);
				prf("\r ");
				if (warsptr->lock == warsptr->lmissl[i].channel)
					prf(" %s*%s%s%s*%s  ",CLR_RED1,CLR_BLUE2,username(ptr),CLR_RED1,CLR_WHITE2);
				else
					prf("  %s%s%s   ",CLR_BLUE2,username(ptr),CLR_WHITE2);
				}
			if (warsptr->lmissl[i].channel == 255)
				prf("\r  (destroyed)   ");
			if (warsptr->jam_sev <= (byte)2)
				prf("Dist: %u",warsptr->lmissl[i].distance);
			else
				prf("Dist: ?????");
			}
		}
	if (none == TRUE)
		prf("none\r");
	else
		prf("\r");
	if (shipclass[warsptr->shpclass].max_torps != 0)
		{
		none = TRUE;
		prfmsg(REP44);
		for (zothusn = 0; zothusn < nships; zothusn++)
			{
			if (ingegame(zothusn))
				{
				ptr=warshpoff(zothusn);
				for (i=0;i<MAXTORPS;++i)
					{
					if (ptr->ltorps[i].distance != 0 && ptr->ltorps[i].channel == usrnum)
						{
						none = FALSE;
						prf("\r ");
						if (warsptr->lock == zothusn)
							prf(" %s*%s%s%s*%s  ",CLR_RED1,CLR_BLUE2,username(ptr),CLR_RED1,CLR_WHITE2);
						else
							prf("  %s%s%s   ",CLR_BLUE2,username(ptr),CLR_WHITE2);
						if (warsptr->jam_sev <= (byte)2)
							prf("Dist: %u",ptr->ltorps[i].distance);
						else
							prf("Dist: ?????");
						}
					}
				}
			}
		if (none == TRUE)
			prf("none\r");
		else
			prf("\r");
		}
	if (shipclass[warsptr->shpclass].max_missl != 0)
		{
		none = TRUE;
		prfmsg(REP45);
		for (zothusn = 0; zothusn < nships; zothusn++)
			{
			if (ingegame(zothusn))
				{
				ptr=warshpoff(zothusn);
				for (i=0;i<MAXMISSL;++i)
					if (ptr->lmissl[i].distance != 0 && ptr->lmissl[i].channel == usrnum)
						{
						none = FALSE;
						prf("\r ");
						if (warsptr->lock == zothusn)
							prf(" %s*%s%s%s*%s  ",CLR_RED1,CLR_BLUE2,username(ptr),CLR_RED1,CLR_WHITE2);
						else
							prf("  %s%s%s   ",CLR_BLUE2,username(ptr),CLR_WHITE2);
						if (warsptr->jam_sev <= (byte)2)
							prf("Dist: %u",ptr->lmissl[i].distance);
						else
							prf("Dist: ?????");

						}
				}
			}
		if (none == TRUE)
			prf("none\r");
		else
			prf("\r");
		}
	if (shipclass[warsptr->shpclass].has_mine != 0)
		{
		none = TRUE;
		prfmsg(REP46);
		for (i=0; i<nummines; ++i)
			if (mines[i].channel == (byte)usrnum)
				{
				none = FALSE;
				prf("\r ");
				ddist = cdistance(&warsptr->coord,&mines[i].coord);
				ddist *= 10000;
				bearing = (int)(cbearing(&warsptr->coord,&mines[i].coord,warsptr->heading)+.5);
				if (warsptr->jam_sev <= (byte)2)
					prf("%d %d  T:%2d  Br:%4d  Dist: %s",
						(int)mines[i].coord.xcoord,(int)mines[i].coord.ycoord,mines[i].timer,bearing,spr("%ld",(long)ddist));
				else
					prf("%d %d  T:%2d  Br:????  Dist: ?????",
						(int)mines[i].coord.xcoord,(int)mines[i].coord.ycoord,mines[i].timer);
				}
		if (none == TRUE)
			prf("none\r");
		else
			prf("\r");
		}
	if (shipclass[warsptr->shpclass].has_decoy != 0)
		{
		none = 0;
		for (i=0; i<10;++i)
			if (warsptr->decout[i] != 0)
				++none;
		prfmsg(REP47);
		if (none == 0)
			prf("none\r");
		else
			prf("%d\r",none);
		}
	}
prfmsg(DASHES);
outprfge(ALWAYS,usrnum);
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

/**************************************************************************
** Static functions for scans                                            **
**************************************************************************/

static void map_planets()
{
int i;
unsigned x,y;
getsector(&warsptr->coord);
for (i=0;i < sector.numplan;++i)
	{
	if (sector.ptab[i].coord.xcoord != 0)
		{
		x = coord2(sector.ptab[i].coord.xcoord)+25;
		y = coord2(sector.ptab[i].coord.ycoord)+25;
		if (map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] == '*')
			mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '7';
		map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '1' + i;
		if (mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] != '7')
			{
			if (sector.ptab[i].type == PLTYPE_WORM)
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '3';
			else
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '5';
			}
		}
	}
}

static void clearmap()
{
int i,j;

for (i=0; i < MAXY; ++i)
	{
	for (j=0; j < MAXX; ++j)
		{
		map[i][j]=' ';
		mapc[i][j]=' ';
		}
	map[i][MAXX] = 0;
	mapc[i][MAXX] = 0;
	}
}

/**************************************************************************
** Scan Command                                                          **
**************************************************************************/
void FUNC cmd_scan()
{

if (warsptr->tactical != 0)
	{
	prfmsg(TABROKE);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDTACT);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc > 1)
	{
	if (genearas("sh",margv[1]))
		scan_sh();
	else
	if (genearas("pl",margv[1]))
		scan_pl();
	else
	if (genearas("ra",margv[1]))
		scan_ra();
	else
	if (genearas("se",margv[1]))
		scan_se();
	else
	if (genearas("lo",margv[1]))
		scan_lo();
	else
		{
		prfmsg(FORMAT,"SCAN");
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(FORMAT,"SCAN");
	outprfge(ALWAYS,usrnum);
	}
}


/* SCAN SHIP FUNCTION */

void FUNC scan_sh()

{
int	shpnum,gheading;
WARSHP	*wptr;
WARUSR	*wuptr;
char	ltr;
unsigned int rseed = gernd();
long	scandist;	/* update_scantab uses ddistance, so we need a local here */

if (margc != 3)
	{
	prfmsg(FORMAT,"SCAN");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->jam_sev > (byte)7 || (warsptr->jam_sev > (byte)2 && rseed%(9 - (int)warsptr->jam_sev) == 0))
	{
	prfmsg(JAMMER4);
	outprfge(ALWAYS,usrnum);
	return;
	}

shpnum = findshp(margv[2],1);

if (shpnum == usrnum)
	{
	prfmsg(SCANER);
	outprfge(ALWAYS,usrnum);
	}
else
if (shpnum >= 0)
	{
	wptr = warshpoff(shpnum);
	wuptr = warusroff(shpnum);
	scandist = cdistance(&warsptr->coord,&wptr->coord)*10000;
	if (scandist < shipclass[warsptr->shpclass].scanrange)
		{
		bearing = (int)(cbearing(&warsptr->coord,&wptr->coord,warsptr->heading)+.5);
		heading = (int) (cbearing(&wptr->coord,&warsptr->coord,wptr->heading)+.5);
		gheading = (int) (wptr->heading+.5);

		speed = ((unsigned)(wptr->speed+.5));

		sprintf(gechrbuf,"%s",wptr->shipname);

		if (warsptr->jam_sev > 2)
			jam_scramble(gechrbuf, warsptr->jam_sev, &rseed);
		if (wptr->status == GESTAT_AUTO)
			prfmsg(SCAN01N,gechrbuf);
		else
			prfmsg(SCAN01,gechrbuf);
		prfmsg(DASHES);
		if (warsptr->jam_sev < (byte)3)
			prfmsg(SCAN01A,shipclass[wptr->shpclass].typename);
		if (wptr->status == GESTAT_USER && warsptr->jam_sev < (byte)3)
			{
			prfmsg(SCAN02,username(wptr));
			if (warusroff(shpnum)->teamcode >0)
				prfmsg(SCAN02A,teamname(wuptr));
			}
		memset(gechrbuf, 0, 255);
		sprintf(gechrbuf,"%d",bearing);
		sprintf(gechrbuf2,"%d",heading);
		sprintf(gechrbuf3,"%ld",(long)scandist);
		if (warsptr->jam_sev > (byte)2)
			{
			jam_scramble(gechrbuf, warsptr->jam_sev, &rseed);
			jam_scramble(gechrbuf2, warsptr->jam_sev, &rseed);
			jam_scramble(gechrbuf3, warsptr->jam_sev, &rseed);
			}
		prfmsg(SCAN03,gechrbuf,gechrbuf2,gechrbuf3);
		setsect(wptr);
		sprintf(gechrbuf,"%d",gheading);
		sprintf(gechrbuf2,"%d %d",xsect,ysect);
		sprintf(gechrbuf3,"%s",showarp(wptr->speed));
		if (warsptr->jam_sev > 2)
			{
			jam_scramble(gechrbuf, warsptr->jam_sev, &rseed);
			jam_scramble(gechrbuf2, warsptr->jam_sev, &rseed);
			jam_scramble(gechrbuf3, warsptr->jam_sev, &rseed);
			}
		prfmsg(SCAN03A,gechrbuf,gechrbuf2);
		prfmsg(SCAN04,gechrbuf3);

		if (warsptr->where != 1 && wptr->where != 1 && warsptr->jam_sev < (byte)3)
			{
			damage = (unsigned) (wptr->damage+.5);
			damstr(damage);
			prfmsg(SCAN05,gechrbuf);

			if (wptr->shieldstat == SHIELDUP)
				prfmsg(SCAN06);
			else
				prfmsg(SCAN07);

			prfmsg(SCAN07A,wuptr->kills);
			}

		prfmsg(DASHES);
		outprfge(ALWAYS,usrnum);

		/* if beyond the "scanned" ships range disply this msg */
		if (warsptr->jam_sev < (byte)3)
			{
			if ((long)scandist > shipclass[wptr->shpclass].scanrange)
				{
				bearing = (int)(cbearing(&wptr->coord,&warsptr->coord,wptr->heading)+.5);
				prfmsg(SCAN2,bearing);
				}
			else
				{
				/* all else get this */
				ltr = shpltr(shpnum,usrnum);
				prfmsg(SCAN1,ltr,warsptr->shipname);
				}
			outprfge(FILTER,shpnum);
			}
		}
	else
		{
		prfmsg(NOSHIP);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}
}

void FUNC scan_pl()

{
unsigned i;
unsigned int rseed = gernd();

/* SCAN PLANET FUNCTION */

if (margc != 3)
	{
	prfmsg(FORMAT,"SCAN");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->jam_sev > (byte)7 || (warsptr->jam_sev > (byte)2 && rseed%(9 - (int)warsptr->jam_sev) == 0))
	{
	prfmsg(JAMMER4);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = atoi(margv[2]);

if (plnum <= MAXPLANETS && plnum > 0)
	{
	getplanetdat(usrnum);
	refresh(warsptr,usrnum);
	if (plnum > sector.numplan)
		{
		prfmsg(NOPLNT);
		outprfge(ALWAYS,usrnum);
		}
	else
	if (plptr->type == PLTYPE_PLNT)
		{
		bearing = (int)(cbearing(&warsptr->coord,&plptr->coord,warsptr->heading)+.5);
		ddistance = cdistance(&warsptr->coord,&plptr->coord)*10000;
		memset(gechrbuf, 0, 255);
		sprintf(gechrbuf,"%s",plptr->name);
		if (warsptr->jam_sev > (byte)2 && warsptr->where - 10 != plnum)
			jam_scramble(gechrbuf, warsptr->jam_sev, &rseed);
		prfmsg(SCAN08,plnum,gechrbuf);
		prfmsg(DASHES);

		if (plptr->userid[0] != 0 && (warsptr->jam_sev < (byte)3 || warsptr->where - 10 == plnum))
			prfmsg(SCAN09,plptr->userid);

		memset(gechrbuf2, 0, 20);
		memset(gechrbuf3, 0, 20);
		sprintf(gechrbuf2,"%d",bearing);
		sprintf(gechrbuf3,"%ld",(long)ddistance);
		if (warsptr->jam_sev > (byte)2 && warsptr->where - 10 != plnum)
			{
			jam_scramble(gechrbuf2, warsptr->jam_sev, &rseed);
			jam_scramble(gechrbuf3, warsptr->jam_sev, &rseed);
			}
		prfmsg(SCAN10,gechrbuf2,gechrbuf3);

		if (warsptr->where != 1 && (warsptr->jam_sev < (byte)3 || warsptr->where - 10 == plnum))
			{
			prfmsg(SCAN11);
			if (plptr->enviorn == 0)
				prfmsg(SCAN12);
			else
			if (plptr->enviorn == 1)
				prfmsg(SCAN13);
			else
			if (plptr->enviorn == 2)
				prfmsg(SCAN14);
			else
			if (plptr->enviorn == 3)
				prfmsg(SCAN15);

			prfmsg(SCAN16);
			if (plptr->resource == 0)
				prfmsg(SCAN12);
			else
			if (plptr->resource == 1)
				prfmsg(SCAN13);
			else
			if (plptr->resource == 2)
				prfmsg(SCAN14);
			else
			if (plptr->resource == 3)
				prfmsg(SCAN15);
			/*DEBUG
			prf("plptr->userid=%s\rwarsptr->userid=%s\r",plptr->userid,warsptr->userid);*/

			if (sameas(plptr->userid,warsptr->userid) || (plptr->userid[0] == 0 && warsptr->where - 10 == plnum))
				{
				for (i=0; i<NUMITEMS; ++i)
					{
					if (plptr->items[i].qty > 0)
						{
						sprintf(gechrbuf,"%s%s%12lu",item_name[i],gedots(26-strlen(item_name[i])),plptr->items[i].qty);
						gechrbuf[0] = toupper(gechrbuf[0]);
						prf("%s\r",gechrbuf);
						}
					}
				}
			else
				{
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty == 0)
					strcpy(gechrbuf,"None");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 100)
					strcpy(gechrbuf,"Less than 100");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 1000)
					strcpy(gechrbuf,"Hundreds");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 10000)
					strcpy(gechrbuf,"Thousands");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 100000L)
					strcpy(gechrbuf,"Tens of thousands");
				else
				if (plptr->items[I_MEN].qty + plptr->items[I_TROOPS].qty < 1000000L)
					strcpy(gechrbuf,"Hundreds of thousands");
				else
					strcpy(gechrbuf,"Millions");
				prfmsg(SCAN28,gechrbuf);

				if (plptr->items[I_MISSILE].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_MISSILE].qty < 250)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_MISSILE].qty < 1000)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN29,gechrbuf);

				if (plptr->items[I_TORPEDO].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_TORPEDO].qty < 250)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_TORPEDO].qty < 1000)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN30,gechrbuf);

				if (plptr->items[I_FLUXPOD].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_FLUXPOD].qty < 250)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_FLUXPOD].qty < 1000)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN33,gechrbuf);

				if (plptr->items[I_MINE].qty == 0)
					strcpy(gechrbuf,"No");
				else
				if (plptr->items[I_MINE].qty < 250)
					strcpy(gechrbuf,"Small");
				else
				if (plptr->items[I_MINE].qty < 1000)
					strcpy(gechrbuf,"Moderate");
				else
					strcpy(gechrbuf,"Large");
				prfmsg(SCAN34,gechrbuf);

				if (plptr->items[I_FIGHTER].qty == 0)
					prfmsg(SCAN31);
				else
					prfmsg(SCAN32);

				}
			}
		prfmsg(DASHES);
		outprfge(ALWAYS,usrnum);
		}
	else
	if (plptr->type == PLTYPE_WORM)
		{
		memcpy(&worm,plptr,sizeof(GALWORM));
		bearing = (int)(cbearing(&warsptr->coord,&worm.coord,warsptr->heading)+.5);
		ddistance = cdistance(&warsptr->coord,&worm.coord)*10000;
		prfmsg(SCANWRM,plnum,worm.name);
		prfmsg(DASHES);

		memset(gechrbuf2, 0, 20);
		memset(gechrbuf3, 0, 20);
		sprintf(gechrbuf2,"%d",bearing);
		sprintf(gechrbuf3,"%ld",(long)ddistance);
		if (warsptr->jam_sev > (byte)2)
			{
			jam_scramble(gechrbuf2, warsptr->jam_sev, &rseed);
			jam_scramble(gechrbuf3, warsptr->jam_sev, &rseed);
			}
		prfmsg(SCAN10,gechrbuf2,gechrbuf3);
		prfmsg(DASHES);
		outprfge(ALWAYS,usrnum);
		}

		else
		{
		prfmsg(NOPLNT);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(NOPLNT);
	outprfge(ALWAYS,usrnum);
	}
}

void FUNC scan_ra()
{
int i, x, y;
double xf, yf, x1, y1, range;
double minx, miny;
double xfactor, yfactor;
double cell_center_x, cell_center_y;
double cell_half_x, cell_half_y;
double gal_min, gal_max;
double shiftx = 0.0, shifty = 0.0;

WARSHP *wptr;
MINE   *mptr;

if (margc < 2 || margc > 3)
	{
	prfmsg(FORMAT, "SCAN");
	outprfge(ALWAYS, usrnum);
	return;
	}

setsect(warsptr);

if (genearas("h",margv[2]))
	range = (double)((shipclass[warsptr->shpclass].scanrange) / 2);
else
if (genearas("q",margv[2]))
	range = (double)((shipclass[warsptr->shpclass].scanrange) / 4);
else
	{
	x = atoi(margv[2]);
	if (x < 1 || x > 9 || margc == 2)
		x = 9;

	range = (double)((shipclass[warsptr->shpclass].scanrange) / ((10 - x) * (10 - x)));
	}

prfmsg(SCAN24, spr("%ld", (long)range), xsect, ysect);

clearmap();
update_scantab(warsptr, usrnum);

x1 = warsptr->coord.xcoord;
y1 = warsptr->coord.ycoord;

xfactor = range / 5000.0 / (double)MAXX;
yfactor = range / 5000.0 / (double)MAXY;

minx = x1 - range / 10000.0;
miny = y1 - range / 10000.0;

cell_half_x = xfactor / 2.0;
cell_half_y = yfactor / 2.0;

gal_min = -((double)univmax);
gal_max = (double)univmax + 0.99;

/* determine shift for X if player would fall one left of center */
if ((int)((x1 - minx) / xfactor) == (MAXX/2) - 1)
	shiftx = xfactor;

/* determine shift for Y if player would fall one above center */
if ((int)((y1 - miny) / yfactor) == (MAXY/2) - 1)
	shifty = yfactor;

/* mark out-of-bounds areas */
for (y = 0; y < MAXY; y++)
	{
	for (x = 0; x < MAXX; x++)
		{
		cell_center_x = minx + x * xfactor + shiftx;
		cell_center_y = miny + y * yfactor + shifty;

		if ((cell_center_x + cell_half_x < gal_min) || (cell_center_x - cell_half_x > gal_max) ||
			(cell_center_y + cell_half_y < gal_min) || (cell_center_y - cell_half_y > gal_max))
			{
			map[y][x] = '.';
			mapc[y][x] = '4';
			}
		}
	}

/* plot mines */
for (i = 0, mptr = mines; i < nummines; ++mptr, ++i)
	{
	if (mptr->channel != 255)
		{
		xf = ((mptr->coord.xcoord - x1) / xfactor) + ((double)MAXX)/2.0;
		yf = ((mptr->coord.ycoord - y1) / yfactor) + ((double)MAXY)/2.0;

		xf += shiftx / xfactor;
		yf += shifty / yfactor;

		if (xf >= 0.0 && xf < (double)MAXX && yf >= 0.0 && yf < (double)MAXY)
			{
			x = (int)xf;
			y = (int)yf;
			map[y][x] = '.';
			mapc[y][x] = '0';
			}
		}
	}

/* plot ships from scantab */
for (i = 0; i < NOSCANTAB; i++)
	{
	othusn = scantab[usrnum].ship[i].shipno;
	if (scantab[usrnum].ship[i].flag == 1)
		{
		wptr = warshpoff(othusn);

		xf = ((wptr->coord.xcoord - x1) / xfactor) + ((double)MAXX)/2.0 + shiftx / xfactor;
		yf = ((wptr->coord.ycoord - y1) / yfactor) + ((double)MAXY)/2.0 + shifty / yfactor;

		if (xf >= 0.0 && xf < (double)MAXX && yf >= 0.0 && yf < (double)MAXY)
			{
			x = (int)xf;
			y = (int)yf;
			/* don't replace locked ship on map */
			if (mapc[y][x] != '6')
				{
				map[y][x] = scantab[usrnum].ship[i].letter;

				if (warsptr->lock == othusn)
					mapc[y][x] = '6';
				else
				if (shipclass[wptr->shpclass].max_type == CLASSTYPE_CYBORG)
					mapc[y][x] = '1';
				else
				if (shipclass[wptr->shpclass].max_type == CLASSTYPE_DROID)
					mapc[y][x] = '4';
				else
					mapc[y][x] = '2';
				}
			}
		}
	}

/* plot player at center */
map[MAXY/2][MAXX/2] = '*';
mapc[MAXY/2][MAXX/2] = '0';

if (waruptr->options[SCANOPTS] == FULL)
	printmap(RANGEFULL,(long)range);
else
if (waruptr->options[SCANOPTS] == FULLNAMES)
	printmap(RANGENAMES,(long)range);
else
if (waruptr->options[SCANOPTS] == FULLEXTRA)
	printmap(RANGEEXTRA,(long)range);
else
if (waruptr->options[SCANOPTS] == NOMAP)
	printmap(RANGENOMAP,(long)range);
else
	printmap(RANGE,0L);

outprfge(ALWAYS, usrnum);
}

void FUNC scan_se()

{
unsigned i,x,y;
WARSHP	*wptr;
MINE	*mptr;

refresh(warsptr,usrnum);

setsect(warsptr);
prfmsg(SCAN25,xsect,ysect);
clearmap();

update_scantab(warsptr,usrnum);

for (i=0,mptr = mines; i<nummines;++mptr,++i)
	{
	x = coord1(mptr->coord.xcoord);
	y = coord1(mptr->coord.ycoord);

	if (mptr->channel != 255 && (x==xsect && y==ysect))	/* if a live mine */
		{
		x = coord2(mptr->coord.xcoord) +50;
		y = coord2(mptr->coord.ycoord) +50;
		map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '.';
		mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '0';
		}
	}

for (i=0 ; i< NOSCANTAB; i++)
	{
	othusn = scantab[usrnum].ship[i].shipno;
	if (scantab[usrnum].ship[i].flag == 1)
		{
		wptr = warshpoff(othusn);
		if (samesect(&wptr->coord,&warsptr->coord))
			{
			x = coord2(wptr->coord.xcoord) +50;
			y = coord2(wptr->coord.ycoord) +50;
			map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = scantab[usrnum].ship[i].letter;
			if (warsptr->lock == othusn)
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '6';
			else
			if (shipclass[wptr->shpclass].max_type == CLASSTYPE_CYBORG)
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '1';
			else
			if (shipclass[wptr->shpclass].max_type == CLASSTYPE_DROID)
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '4';
			else
				mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '2';
			}
		}
	}
x = coord2(warsptr->coord.xcoord) +50;
y = coord2(warsptr->coord.ycoord) +50;
map[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '*';
mapc[y/(SSMAX/(MAXY-1))][x/(SSMAX/(MAXX-1))] = '0';

map_planets();
if (waruptr->options[SCANOPTS] == NOMAP)
	printmap(SECTORNOMAP,0L);
else
if (waruptr->options[SCANOPTS] == SIMPLE)
	printmap(SECTOR,0L);
else
	printmap(SECTORFULL,0L);
outprfge(ALWAYS,usrnum);
}

void FUNC scan_lo()
{
int x, y, fullgal;
double xf, yf, x1, y1, range;
double minx, miny;
double xfactor, yfactor;
double sx, sy;
double cell_center_x, cell_center_y;
double cell_half_x, cell_half_y;
double gal_min, gal_max;
double shiftx = 0.0;

WARSHP *wptr;

if (margc < 2 || margc > 3)
	{
	prfmsg(FORMAT,"SCAN");
	outprfge(ALWAYS,usrnum);
	return;
	}

setsect(warsptr);

if (margc == 3 && genearas("h",margv[2]))
	range = (double)(shipclass[warsptr->shpclass].scanrange) * 5.0;
else
if (margc == 3 && genearas("q",margv[2]))
	range = (double)(shipclass[warsptr->shpclass].scanrange) * 2.5;
else
	range = (double)(shipclass[warsptr->shpclass].scanrange) * 10.0;

x1 = warsptr->coord.xcoord * 10000.0;
y1 = warsptr->coord.ycoord * 10000.0;

gal_min = -(double)univmax * 10000.0;
gal_max = ((double)univmax * 10000.0) + 9999.0;

/* will this scan touch all four corners of the galaxy? */
fullgal = (range >= (x1 - gal_min) && range >= (gal_max - x1) &&
	range >= (y1 - gal_min) && range >= (gal_max - y1));

if (fullgal)
	prfmsg(SCAN24G);
else
	prfmsg(SCAN24, spr("%ld",(long)range), xsect, ysect);

clearmap();

if (fullgal)
	{
	minx = gal_min;
	miny = gal_min;
	xfactor = (gal_max - gal_min) / (double)MAXX;
	yfactor = (gal_max - gal_min) / (double)MAXY;
	}
else
	{
	minx = x1 - range;
	miny = y1 - range;
	xfactor = (2.0 * range) / (double)MAXX;
	yfactor = (2.0 * range) / (double)MAXY;

	/* compute visual shift so player ends up at MAXX/2 */
	shiftx = ((double)MAXX / 2.0) - 0.5 - ((x1 - minx) / xfactor);
	shiftx *= xfactor;  /* convert from cells to units */
	}

cell_half_x = xfactor / 2.0;
cell_half_y = yfactor / 2.0;

/* mark outside of galaxy */
for (y = 0; y < MAXY; y++)
	{
	for (x = 0; x < MAXX; x++)
		{
		cell_center_x = minx + (x + 0.5) * xfactor + shiftx;
		cell_center_y = miny + (y + 0.5) * yfactor;

		/* if the entire cell lies outside galaxy bounds */
		if ((cell_center_x + cell_half_x < gal_min) || (cell_center_x - cell_half_x > gal_max) ||
			(cell_center_y + cell_half_y < gal_min) || (cell_center_y - cell_half_y > gal_max))
			{
			map[y][x]  = '.';
			mapc[y][x] = '4';
			}
		}
	}

/* place all ships except the player scanning */
for (othusn = 0; othusn < nships; othusn++)
	{
	if (ingegame(othusn) && othusn != usrnum)
		{
		wptr = warshpoff(othusn);

		sx = (wptr->coord.xcoord * 10000.0 - minx + shiftx) / xfactor;
		sy = (wptr->coord.ycoord * 10000.0 - miny) / yfactor;

		if (sx >= 0.0 && sy >= 0.0)
			{
			x = (int)sx;
			y = (int)sy;

			if (x >= 0 && y >= 0 && x < MAXX && y < MAXY)
				{
				if (wptr->status == GESTAT_AUTO)
					{
					if (map[y][x] != '=')   /* user ships take precedence */
						{
						map[y][x] = '+';
						mapc[y][x] = '0';
						}
					}
				else
					{
					map[y][x] = '=';
					mapc[y][x] = '0';
					}
				}
			}
		}
	}

/* place player */
if (fullgal)
	{
	xf = (x1 - minx) / xfactor;
	yf = (y1 - miny) / yfactor;

	if (xf >= 0.0 && yf >= 0.0)
		{
		x = (int)xf;
		y = (int)yf;

		if (x >= MAXX) x = MAXX - 1;
		if (y >= MAXY) y = MAXY - 1;

		if (x >= 0 && y >= 0 && x < MAXX && y < MAXY)
			{
			map[y][x] = '*';
			mapc[y][x] = '0';
			}
		}
	}
else
	{
	map[MAXY/2][MAXX/2] = '*';
	mapc[MAXY/2][MAXX/2] = '0';
	}

printmap(LONG,0L);
outprfge(ALWAYS,usrnum);
}


void FUNC update_scantab(ptr, usrn)
WARSHP	*ptr;
int	usrn;
{
int	i,j;
char	l;
WARSHP	*wptr;
SCANTAB	tmp;

char	lettab[300];

setmem(&lettab[0],sizeof(char)*300,0);

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
		tmp.ship[i].bearing = (int)(cbearing(&ptr->coord,&(wptr->coord),ptr->heading)+.5);
		tmp.ship[i].heading = (int)(cbearing(&(wptr->coord),&ptr->coord,wptr->heading)+.5);
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

/**************************************************************************
** Print the map                                                         **
**************************************************************************/

void FUNC printmap(int maptype, long dist_filter)
{
SCANTAB *sptr = &scantab[usrnum];
int i, shp = 0, ff = 0;

print_map_header(maptype);
outprfge(ALWAYS, usrnum);

for (i = 0; i < MAXY + 1; ++i)
	{
	if (maptype == SECTORNOMAP || maptype == RANGENOMAP)
		continue;

	if (i == MAXY)
		prfmsg(PLUSDASH);
	else
		print_map_row(i, maptype);

	if (maptype == SECTORFULL)
		{
		while (shp < MAXPLANETS && ptab[usrnum].planets[shp].type != 0 && print_planet_line(shp) == FALSE)
			shp++;  /* skip jammed planet and immediately try next one */

		shp++;
		}

	if (maptype == RANGENAMES || maptype == RANGEEXTRA)
		{
		while (ff == 0 && warsptr->jam_sev > (byte)7 && gernd()%2 == 0)
			shp++;
		shp += print_range_line(sptr, shp, &ff, dist_filter);
		}

	if (maptype == RANGEFULL)
		{
		while (ff == 0 && warsptr->jam_sev > (byte)7 && gernd()%2 == 0)
			shp++;
		shp += print_fullrange_line(sptr, shp, dist_filter);
		}

	prf("\r");
	}

if (maptype == RANGEEXTRA || maptype == RANGENOMAP)
	print_range_summary(sptr, shp, dist_filter, maptype);

if (maptype == SECTORNOMAP)
	print_planet_summary(shp);

prf("\r");
prf(CLR_WHITE2);
}
unsigned FUNC coord2(dcoord)
double	dcoord;
{
double	d1,d2;
int	d3;

d2 =modf(1+modf(dcoord, &d1),&d1);
d3 = (d2 * SSMAX);

return ((unsigned)d3);

}


int FUNC coord1(dcoord)
double dcoord;
{

return ((int)floor(dcoord));

}

/**************************************************************************
** Take the shields up or down                                           **
**************************************************************************/

void FUNC cmd_shields()

{

if (shipclass[warsptr->shpclass].max_shlds == 0)
	{
	prfmsg(SHIELD0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(SHLD1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDSHLD);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc != 2)
	{
	prfmsg(FORMAT,"SHIELD");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"up"))
	{
	if (fluxstat(warsptr,usrnum,SHENGUSE * warsptr->shieldtype) == 1)
		{
		if (warsptr->shieldstat == SHIELDDM)
			{
			prfmsg(SHNORPR);
			outprfge(ALWAYS,usrnum);
			return;
			}
		else
			{
			shieldup(warsptr,usrnum);
			return;
			}
		}
	else
		{
		prfmsg(SHNOPWR);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
else
if (sameas(margv[1],"down"))
	{
	shielddn(warsptr,usrnum);
	return;
	}
else
	{
	prfmsg(FORMAT,"SHIELD");
	outprfge(ALWAYS,usrnum);
	return;
	}
}


/**************************************************************************
** Turn cloaking on and off                                              **
**************************************************************************/

void FUNC cmd_cloak()

{

if (shipclass[warsptr->shpclass].max_cloak == 0)
	{
	prfmsg(CLOK01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc != 2)
	{
	prfmsg(FORMAT,"CLOAK");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where == 1)
	{
	prfmsg(CLOK1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->damage >= 100)
	{
	prfmsg(RNDCLOK);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"on"))
	{
	if (warsptr->cloak > 0)
		{
		prfmsg(CLOKCOM);
		outprfge(FILTER,usrnum);
		}
	else
	if (warsptr->cloak == 0)
		{
		if (fluxstat(warsptr,usrnum,clenguse) == 1)
			{
			warsptr->cloak = 1;
			prfmsg(CLOKON);
			outprfge(FILTER,usrnum);
			}
		else
			{
			prfmsg(CLOKPWR);
			outprfge(ALWAYS,usrnum);
			}
		}
	else
	if (warsptr->cloak < 0)
		{
		prfmsg(CLOKDAM);
		outprfge(ALWAYS,usrnum);
		}
	return;
	}
else
if (sameas(margv[1],"off"))
	{
	if (warsptr->cloak <= 0)
		{
		prfmsg(CLOKDWN);
		outprfge(FILTER,usrnum);
		}
	else
	if (warsptr->cloak > 0)
		{
		warsptr->cloak = 0;
		assign_cybs(usrnum,1);	/* don't pull far away cybs if close ones around */
		prfmsg(CLOKOFF);
		outprfge(FILTER,usrnum);
		prfmsg(CLOK2);
		outrange(FILTER,&warsptr->coord);
		}
	return;
	}
prfmsg(FORMAT,"CLOAK");
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Transfer goods down                                                   **
**************************************************************************/

void FUNC cmd_transfer()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(TRANSFR3);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 4)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[3]))
			{
			if (genearas("u",margv[1]))
				trans_up(i);
			else
			if (genearas("d",margv[1]))
				trans_down(i);
			else
				prfmsg(FORMAT,"TRANSFER");
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}

prfmsg(FORMAT,"TRANSFER");
outprfge(ALWAYS,usrnum);
}

void trans_down(item)
int	item;

{
long	amt;

if (neutral(&warsptr->coord))
	{
	prfmsg(TRANSFR2);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (trans_opt || sameas(plptr->userid,warsptr->userid))
	{
	if (sameas("ALL",margv[2]))
		{
		amt = warsptr->items[item];
		if (amt == 0L)
			{
			/* user wants all down but there are none */
			sprintf(gechrbuf,"%lu",amt);
			prfmsg(TRANSFR5,gechrbuf,item_name[item]);
			return;
			}
		}
	else
		amt = atol(margv[2]);

	if (amt == 0L)
		{
		/* user specified 0 or non-number */
		prfmsg(FORMAT,"TRANSFER");
		return;
		}

	if (amt < 0L)
		{
		/* user wants all but specified amount */
		amt = warsptr->items[item] + amt;
		if (amt < 0L)
			{
			/* the amount to withhold is more than the total */
			prfmsg(TRANSFR1);
			return;
			}
		}

	if (warsptr->items[item] >= amt)
		{
		warsptr->items[item] -= amt;
		plptr->items[item].qty += amt;
		sprintf(gechrbuf,"%lu",amt);
		prfmsg(TRANSFR5,gechrbuf,item_name[item]);
		setsect(warsptr); /* build PKEY */
		pkey.plnum = plnum;
		gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
		gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
		return;
		}
	else
		{
		/* not enough on board */
		prfmsg(TRANSFR1);
		}
	}
else
	{
	/* not owner */
	prfmsg(TRANSFR4);
	}
}

void trans_up(item)
int	item;

{
long	amt;

plnum = warsptr->where - 10;

getplanetdat(usrnum);

/* you must own this planet or NOBODY must own it to xfer up */

if (sameas(plptr->userid,warsptr->userid) || plptr->userid[0] == 0)
	{
	if (sameas("MAX",margv[2]))
		{
		amt = (shipclass[warsptr->shpclass].max_tons - calcweight(warsptr))/((double)weight[item]/100.0);
		if (amt <= 0L)
			{
			/* user wants max but not even one will fit */
			prfmsg(TRANSUP6);
			return;
			}
		}
	else
	if (sameas("ALL",margv[2]))
		{
		amt = plptr->items[item].qty;
		if (amt == 0L)
			{
			/* user wants all up but there are none */
			sprintf(gechrbuf,"%lu",amt);
			prfmsg(TRANSUP5,gechrbuf,item_name[item]);
			return;
			}
		}
	else
		amt = atol(margv[2]);

	if (amt == 0L)
		{
		/* user specified 0 or non-number */
		prfmsg(FORMAT,"TRANSFER");
		return;
		}

	if (amt < 0)
		{
		/* user wants all but specified amount */
		amt = plptr->items[item].qty + amt;
		if (amt < 0)
			{
			/* the amount to withhold is more than the total */
			prfmsg(TRANSUP1);
			return;
			}
		}

	if (chkweight(warsptr,item,amt))
		{
		if (plptr->items[item].qty >= amt)
			{
			plptr->items[item].qty -= amt;
			warsptr->items[item] += amt;
			sprintf(gechrbuf,"%lu",amt);
			prfmsg(TRANSUP5,gechrbuf,item_name[item]);
			setsect(warsptr); /* load PKEY */
			pkey.plnum = plnum;
			gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
			gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
			return;
			}
		else
			{
			/* not enough on planet */
			prfmsg(TRANSUP1);
			}
		}
	else
		{
		/* not enough space */
		prfmsg(TRANSUP6);
		}
	}
else
	{
	/* not owner */
	prfmsg(TRANSUP4);
	}
}




/**************************************************************************
** abandon a colony                                                      **
**************************************************************************/

void FUNC cmd_abandon()

{

if (warsptr->where < 10)
	{
	prfmsg(ABAN01);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (sameas(plptr->userid,warsptr->userid))
	{
	plptr->userid[0] = 0;
	if(--waruptr->planets <0)
		waruptr->planets = 0;

	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);

	prfmsg(ABAN02);
	outprfge(ALWAYS,usrnum);

	}
else
	{
	prfmsg(ADMIN2);
	outprfge(ALWAYS,usrnum);
	}
}



/**************************************************************************
** establish a colony or administer it                                   **
**************************************************************************/

void FUNC cmd_admin()

{

if (warsptr->where < 10)
	{
	prfmsg(ADMIN1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (plptr->userid[0] == 0)
	{
	if (waruptr->planets >= max_plnts)
		{
		prfmsg(ADMIN4,max_plnts);
		outprfge(ALWAYS,usrnum);
		return;
		}
	prfmsg(ADMENU1);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU1;
	}
else
#ifdef PHARLAP
if (sameas(plptr->userid,warsptr->userid) || (syscmds && !sysonly) || (sysonly && (hasmkey(SYSKEY))))
#else
if (sameas(plptr->userid,warsptr->userid) || (syscmds && !sysonly) || (sysonly && (usrptr->flags&ISYSOP))))
#endif

	{
	prfmsg(ADMENU2);
	outprfge(ALWAYS,usrnum);
	usrptr->substt = ADMENU2;
	}
else
	{
	prfmsg(ADMIN2);
	outprfge(ALWAYS,usrnum);
	}
}

/**************************************************************************
** Attack Command                                                        **
**************************************************************************/

void FUNC cmd_attack()

{

int won;
unsigned long num;

if (warsptr->where < 10)
	{
	prfmsg(ADMIN1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (sameas(plptr->userid,warsptr->userid))
	{
	prfmsg(ATTACK0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord))
	{
	zaphim(warsptr,usrnum);
	prfmsg(ATTKER);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 3)
	{
	if (genearas("tro",margv[2]))
		{
		if (sameas("ALL",margv[1]))
			num = warsptr->items[I_TROOPS];
		else
			num = atol(margv[1]);
		if (num > 0L && num <= warsptr->items[I_TROOPS])
			{
			warsptr->hostile = warsptr->where;
			warsptr->cantexit = FIRETICKS;
			won = attack_men(num);
			if (won == 1)
				{
				prfmsg(ATTACK8);
				outprfge(ALWAYS,usrnum);
				}
			else
				{
				prfmsg(ATTACK9);
				outprfge(ALWAYS,usrnum);
				}
			}
		else
			{
			prfmsg(ATTACKM0);
			outprfge(ALWAYS,usrnum);
			}
		return;
		}
	else
	if (genearas("fig",margv[2]))
		{
		if (shipclass[warsptr->shpclass].max_attk > 0)
			{
			if (sameas("ALL",margv[1]))
				num = warsptr->items[I_FIGHTER];
			else
				num = atol(margv[1]);
			if (num > 0 && num <= warsptr->items[I_FIGHTER])
				{
				warsptr->hostile = warsptr->where;
				warsptr->cantexit = FIRETICKS;
				won = attack_fig(num);
				if (won == 1)
					{
					prfmsg(ATTACK8);
					outprfge(ALWAYS,usrnum);
					}
				else
					{
					prfmsg(ATTACK9);
					outprfge(ALWAYS,usrnum);
					}
				}
			else
				{
				prfmsg(ATTACKF0);
				outprfge(ALWAYS,usrnum);
				}
			return;
			}
		else
			{
			prfmsg(ATTACK0A);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	prfmsg(FORMAT,"ATTACK");
	}
else
	{
	prfmsg(FORMAT,"ATTACK");
	}
outprfge(ALWAYS,usrnum);
}

int FUNC attack_men(num)
unsigned long num;
{

double	r;
int	won = 0;
int	ii;
unsigned long i, j, left1, left2, kill1, kill2,	ratio;

/* take troops off ship */
warsptr->items[I_TROOPS] -= num;
sprintf(gechrbuf,"%ld",num);

/* tell him there gone*/
prfmsg(ATTACKM1,gechrbuf);
outprfge(ALWAYS,usrnum);

left1 = num;
left2 = plptr->items[I_TROOPS].qty;

kill1 = 0;
kill2 = 0;

/* figure out the proportion of this attack*/

if (left2 > 0)
	ratio = (left1*100UL)/left2;
else
	ratio = 0;

if (plptr->items[I_FIGHTER].qty > 1)
	{
	prfmsg(ATTACKM7);
	i = ((unsigned long)(gernd()%35))+9L;
	kill1 = i * (plptr->items[I_FIGHTER].qty);
	if (kill1 > left1)
		kill1 = left1;
	sprintf(gechrbuf,"%ld",kill1);
	prfmsg(ATTACKM8,gechrbuf);
	}

/* this specifies the number of troops killed by ground troops */
r = rndm(plattrt1)+.25;	/*.766*/
kill1 += (unsigned long)((double)left2 * r);

if (ratio > 2L)
	{
	r = rndm(plattrt2)+.1;	/* .344 */
	kill2 = (unsigned long)((double)left1 * r);
	}
else
	{
	kill2 = 0L;
	}

if (kill1 > left1)
	kill1 = left1;

if (kill2 > left2)
	kill2 = left2;

sprintf(gechrbuf,"%ld",kill1);
sprintf(gechrbuf2,"%ld",kill2);

prfmsg(ATTACKM2,gechrbuf2,gechrbuf);

left1 -= kill1;
left2 -= kill2;

if (left2 > 0 && left2 < (left1/4))
	{
	sprintf(gechrbuf,"%ld",left2);
	prfmsg(ATTACKM3,gechrbuf);
	won = 1;
	}

if (left1 > 0 && left1 < (left2/4))
	{
	sprintf(gechrbuf,"%ld",left1);
	prfmsg(ATTACKM4,gechrbuf);
	plptr->items[I_TROOPS].qty += left1;
	left1 = 0;
	}

/* randomly trash a few of the planets items */

if (ratio > 2 && left1 > (left2/2))
	{
	for(ii=1;ii<NUMITEMS;++ii)
		{
		j = (unsigned long)gernd()%15;

		if (j > plptr->items[ii].qty)
			j = plptr->items[ii].qty;

		if (j >= 1)
			{
			sprintf(gechrbuf,"%ld",j);
			prfmsg(ATTACKM5,gechrbuf,item_name[ii]);
			plptr->items[ii].qty -= j;
			}
		}
	}

if (left1 > 0)
	{
	warsptr->items[I_TROOPS] += left1;
	sprintf(gechrbuf,"%lu",left1);
	prfmsg(ATTACKM6,gechrbuf);
	}

if (left1 > 0L && (left2 <= 0L && plptr->items[(int)I_FIGHTER].qty <= 0L))
	{
	won = 1;
	}

plptr->items[I_TROOPS].qty = left2;

outprfge(ALWAYS,usrnum);
clrprf();

/* inform the player if he is not in game */

if (ratio > 5L) /* big enough to let spy report on it */
	call_4_help(TRUE,won);
else
if (ratio > 1L)
	call_4_help(FALSE,won);


/* dont mail him unless its significant*/
if (ratio > 1L)
	{
	mail.type = MESG02;
	strncpy(mail.userid,plptr->userid,UIDSIZ);

	if (won == 1)
		{
		wonplnt();
		mail.type = MESG03;
		}

	strcpy(mail.name1,plptr->name);
	mail.class = MAIL_CLASS_DISTRESS;
	mail.int1 = sector.xsect;
	mail.int2 = sector.ysect;
	mail.long1 = num;
	sprintf(mail.name2,"%s",warsptr->shipname);
	sprintf(mail.string1,"%s",warsptr->userid);

	mailit(1);
	}

setsect(warsptr);
pkey.plnum = plnum;
gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
return(won);
}

int FUNC attack_fig(num)
unsigned long num;
{

double	r;
int	won = 0;
unsigned long j, left1, left2, kill1, kill2, ratio;

float	fl1, fl2, fl3;

int	ii;

warsptr->items[I_FIGHTER] -= num;
sprintf(gechrbuf,"%ld",num);
prfmsg(ATTACKF1,gechrbuf);
outprfge(ALWAYS,usrnum);

left1 = num;
left2 = plptr->items[I_FIGHTER].qty;

kill1 = 0;
kill2 = 0;

/* figure out the proportion of this attack*/

/* there is a bug here */

if (left2 > 0)
	{
	fl1 = left1;
	fl2 = left2;
	fl3 = (fl1/fl2)*100.0;
	ratio = (unsigned long)fl3;
	}
else
	ratio = 0;


/* If there are more than 500 troops - shoot down some fighters */
if (left1 > 0 && plptr->items[I_TROOPS].qty > 500 && (gernd()%5-1) > 0)
	{
	prfmsg(ATTACKF8);
	r = rndm(plattrf1)+.05; /*.188) */
	kill1 += (unsigned long)((double)left1 * r);
	}

/* if there are fighters on the planet then they will shoot down some of
   the attackers */

if (left2 > 0L)
	{
	/* this specifies the planet fighters kill ratio -
	   at least 20% and as much as 120.0% */
	r = rndm(plattrf2)+.2;	/* 1.0 */
	kill1 += (unsigned long)((double)left2 * r);

	/* if the ratio of attacker to attackee is at least 1 to 2 (2%)
	   then kill off some of the planets fighters - this makes it harder
		for the little marauders to eat away at your fighters */

	if (ratio > 1)
		{
		r = rndm(plattrf3)+.2;	/* .551 */
		kill2 = (unsigned long)((double)left1 * r);
		}
	else
		{
		kill2 = 0L;
		}

	if (kill1 > left1)
		kill1 = left1;

	if (kill2 > left2)
		kill2 = left2;

	sprintf(gechrbuf,"%ld",kill2);
	sprintf(gechrbuf2,"%ld",kill1);

	prfmsg(ATTACKF2,gechrbuf,gechrbuf2);
	left1 -= kill1;
	left2 -= kill2;
	}

/* must be at least 5% to do damage to items on the planet */

if (ratio > 5)
	{
	for(ii=0;ii<NUMITEMS;++ii)
		{
		j = (unsigned long)gernd()%15;

		if (j > plptr->items[ii].qty)
			j = plptr->items[ii].qty;

		if (j > 0 && ii != I_FIGHTER)
			{
			sprintf(gechrbuf,"%ld",j);
			prfmsg(ATTACKF5,gechrbuf,item_name[ii]);
			plptr->items[ii].qty -= j;
			}
		}
	}

if (left1 > 0L && left2 <= 0L && plptr->items[I_TROOPS].qty < 5L)
	{
	won = 1;
	prfmsg(ATTACKF7);
	}

if (left1 > 0L)
	{
	warsptr->items[I_FIGHTER] +=left1;
	sprintf(gechrbuf,"%ld",left1);
	prfmsg(ATTACKF6,gechrbuf);
	}

outprfge(ALWAYS,usrnum);
clrprf();

if (ratio > 5) /* big enough to let spy report on it */
	call_4_help(TRUE,won);
else
if (ratio > 1)
	call_4_help(FALSE,won);

plptr->items[I_FIGHTER].qty = left2;

if (ratio > 2 || won == 1)
	{
	mail.class = MAIL_CLASS_DISTRESS;
	mail.type = MESG04;
	strncpy(mail.userid,plptr->userid,UIDSIZ);

	if (won == 1)
		{
		wonplnt();
		mail.type = MESG05;
		}

	strcpy(mail.name1,plptr->name);
	mail.class = MAIL_CLASS_DISTRESS;
	mail.int1 = sector.xsect;
	mail.int2 = sector.ysect;
	mail.long1 = num;
	sprintf(mail.name2,"%s",warsptr->shipname);
	sprintf(mail.string1,"%s",warsptr->userid);

	mailit(1);
	}

setsect(warsptr);
pkey.plnum = plnum;
gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
return (won);
}

void FUNC call_4_help(int send_spy_mail, int won)

{
if (instat(plptr->userid,gestt) && othusp->substt >= FIGHTSUB)
	{
	prfmsg(ATTACK6,plptr->name,xsect,ysect,warsptr->userid,warsptr->shipname);
	outprf(othusn);
	prfmsg(ATTACK7);
	outprfge(ALWAYS,usrnum);
	clrprf();
	}
else
if (onsys(plptr->userid) && user[othusn].state != fse_state)
	{
	prfmsg(ATTACK6A);
	injoth();
	prfmsg(ATTACK7);
	outprfge(ALWAYS,usrnum);
	clrprf();
	}
else
if (won == 0
	&& send_spy_mail
	&& gernd()%6 == 0
	&& plptr->spyowner[0] != 0)
	{
	prfmsg(SPYM3,plptr->name,xsect,ysect,warsptr->userid);
	strcpy(mail.userid,plptr->spyowner);
	strcpy(mail.topic,"Intelligence Report");
	sendit();
	clrprf();
	}
else
if (won == 1
	&& plptr->spyowner[0] != 0)
	{
	prfmsg(SPYM4,plptr->name,xsect,ysect,warsptr->userid);
	strcpy(mail.userid,plptr->spyowner);
	strcpy(mail.topic,"Intelligence Report");
	sendit();
	clrprf();
	}
}

void FUNC wonplnt()

{
strncpy(plptr->userid,warsptr->userid,UIDSIZ);
warsptr->hostile = 0;
++waruptr->planets;
}

/**************************************************************************
** Roster Command                                                        **
**************************************************************************/

void FUNC cmd_geroster()

{

int i = 0;
int j;

setbtv(gebb5);

j = gemaxlist;

if (margc == 2 && sameas(margv[1],"all"))
	{
	prfmsg(ROSTER1);
	outprfge(ALWAYS,usrnum);
	j = 200;
	}
else
	{
	prfmsg(ROSTER2,gemaxlist);
	outprfge(ALWAYS,usrnum);
	}

if (qhibtv(1))
	{
	do
		{
		gcrbtv(&tmpusr,1);
		logthis(spr("ROS:Got %s Score %lu",tmpusr.userid,tmpusr.score));
		if ((tmpusr.score > 0 || j == 200) && tmpusr.userid[0] != '@')
			{
			++i;
			sprintf(gechrbuf,"%11lu",tmpusr.score);
			sprintf(gechrbuf2," %10.2fm",((float)tmpusr.population)/100.0);
			prf("%-29s%s%6d%4d%s\r",tmpusr.userid,gechrbuf,tmpusr.kills,tmpusr.planets,gechrbuf2);
			outprfge(ALWAYS,usrnum);
			}
		} while (qprbtv() && i < j);
	}
else
	{
	logthis("No one in roster yet");
	}
}

/**************************************************************************
** Planet command                                                        **
**************************************************************************/

void FUNC cmd_planet()

{
setbtv(gebb2);

if (qlobtv(0))
	{
	if (qeqbtv(warsptr->userid,1))
		{
		prfmsg(PLAMSG1);
		do
			{
			gcrbtv(&planet,1);
			if (sameas(planet.userid,warsptr->userid))
				{
				prf("%-24s %6d %6d  %6d\r",planet.name,planet.xsect,planet.ysect,planet.plnum);
				outprfge(ALWAYS,usrnum);
				}
			else
				break;
			} while (qnxbtv());
		}
	else
		{
		prfmsg(PLAMSG2);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(PLAMSG2);
	outprfge(ALWAYS,usrnum);
	}
}


/**************************************************************************
** Sell goods                                                            **
**************************************************************************/

void FUNC cmd_sell()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(SELL1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (!neutral(&warsptr->coord))
	{
	prfmsg(SELL1);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (waruptr->factions[gcnum] > 100)	/* don't buy from jerks */
	{
	prfmsg(NOZYG,(int)((waruptr->factions[gcnum]-61)/40));
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (neutral(&warsptr->coord) && plnum == 1) /*must be Zygor-3*/
	{
	if (margc == 3)
		{
		for (i=0; i < NUMITEMS; ++i)
			{
			if (genearas(kwrd[i],margv[2]))
				{
				sell(i);
				outprfge(ALWAYS,usrnum);
				return;
				}
			}
		}
	}

prfmsg(FORMAT,"SELL");
outprfge(ALWAYS,usrnum);
}

void FUNC sell(item)
int	item;

{
unsigned long amt;
long	doll,fee;

if ((amt = atol(margv[1])) > 0L || sameas("ALL",margv[1]))
	{
	if (sameas("ALL",margv[1]))
		if (warsptr->items[item] == 0L)
			{
			prfmsg(SELL5,item_name[item]);
			return;
			}
		else
			{
			amt = warsptr->items[item];
			}
	if (warsptr->items[item] >= amt)
		{
		if (amt > SLCAP / baseprice[item])
			{
			prfmsg(TOOMUCH);
			return;
			}
		else
			{
			doll = (long)baseprice[item]*amt;

			fee = 1L + (doll/1000L);

			if (waruptr->cash > ULCAP - (doll - fee))
				{
				sprintf(gechrbuf,"%lu",ULCAP);
				prfmsg(TOORICH,gechrbuf);
				return;
				}
			else
				{
				warsptr->items[item] -= amt;
				/* if there is more tax than profit then tax = profit */
				if ((doll-fee) < 0)
					fee = doll;

				waruptr->cash += (doll-fee);

				gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
				geudb(GEUPDATE,waruptr->userid,waruptr);

				sprintf(gechrbuf,"%lu",amt);
				sprintf(gechrbuf2,"%ld",(doll-fee));
				sprintf(gechrbuf3,"%ld",fee);

				prfmsg(SELL2,gechrbuf3,gechrbuf2,gechrbuf,item_name[item]);
				return;
				}
			}
		}
	else
		{
		prfmsg(SELL3,item_name[item]);
		}
	}
else
	{
	prfmsg(FORMAT,"SELL");
	}
}




/**************************************************************************
** Buy goods                                                             **
**************************************************************************/

void FUNC cmd_buy()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(BUY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (neutral(&warsptr->coord) && plnum == 1 && waruptr->factions[gcnum] > 100)	/* if Zygor, don't sell to jerks */
	{
	prfmsg(NOZYG,(int)((waruptr->factions[gcnum]-61)/40));
	outprfge(ALWAYS,usrnum);
	return;
	}

/* two problems:
   1 - trading player has no team affiliation and planet was designated
	    TEAM while the owner had no team affiliation...hence they both are
		 on the same team. Need to find out what the teamcode is when no team
		 is set... 0 I suspect.
	2 - players can just specify the password "team" and trade anyway
*/


if (sameas(plptr->password,"team")
	&& plptr->teamcode > 0
	&& plptr->teamcode != waruptr->teamcode)
	{
	prfmsg(BUYPAS3);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas(plptr->password,"team")
	&& plptr->teamcode > 0
	&& plptr->teamcode == waruptr->teamcode)
	{
	prfmsg(BUYPAS4);
	outprfge(ALWAYS,usrnum);
	}
else
if (!sameas(plptr->password,"none") && margc > 1 && margc < 4)
	{
	prfmsg(BUYPAS1);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (!sameas(plptr->password,"none")
	&& !sameas(plptr->password,margv[3]))
	{
	prfmsg(BUYPAS2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc > 2)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[2]))
			{
			buy(i);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}
prfmsg(FORMAT,"BUY");
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Price goods                                                           **
**************************************************************************/

void FUNC cmd_price()
{

int i;

if (warsptr->where < 10)
	{
	prfmsg(BUY1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (neutral(&warsptr->coord) && plnum == 1 && waruptr->factions[gcnum] > 100)	/* if Zygor, don't price to jerks */
	{
	prfmsg(NOZYG,(int)((waruptr->factions[gcnum]-61)/40));
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margc == 3)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[2]))
			{
			buy(i);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}
prfmsg(FORMAT,"PRICE");
outprfge(ALWAYS,usrnum);
}

void FUNC buy(item)
int	item;
{
unsigned long amt, avail, tot, ptot;

if (plptr->userid[0] != 0)
	{
	if ((amt = atol(margv[1])) > 0 || sameas("MAX",margv[1]) || sameas("ALL",margv[1]))
		{
		if (sameas(plptr->userid,warsptr->userid) || plptr->items[item].sell == 'Y')
			{
			if (sameas("MAX",margv[1]))
				amt = (shipclass[warsptr->shpclass].max_tons - calcweight(warsptr))/((double)weight[item]/100.0);
			if (sameas("ALL",margv[1]))
				amt = amt4sale(item);
			if ((sameas(plptr->userid,warsptr->userid) && amt > SLCAP / baseprice[item])
				|| (!sameas(plptr->userid,warsptr->userid) && amt > SLCAP / (long)plptr->items[item].markup2a))
				{
				prfmsg(TOOMUCH);
				return;
				}
			if (chkweight(warsptr,item,amt))
				{
				avail = amt4sale(item);
				if (avail > 0 && avail >= amt)
					{
					if ((tot = price(item,amt)) <= waruptr->cash)
						{
						if (strncmp(margv[0],"buy",3) == 0)
							{
							if (!neutral(&warsptr->coord))
								{
								plptr->items[item].qty -= amt;
								plptr->items[item].sold2a += amt;
								if (plptr->cash > ULCAP - tot)
									{
									ptot = ULCAP - plptr->cash;
									plptr->cash +=ptot;
									}
								else
									{
									plptr->cash +=tot;
									}
								setsect(warsptr);
								pkey.plnum = plnum;
								gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);
								}
							warsptr->items[item] += amt;
							waruptr->cash -= tot;
							sprintf(gechrbuf,"%lu",tot);

							if (sameas(plptr->userid, warsptr->userid))
								{
								sprintf(gechrbuf2,"%lu",amt);
								prfmsg(BUY9,gechrbuf2,item_name[item],baseprice[item],gechrbuf);
								}
							else
								{
								sprintf(gechrbuf2,"%lu",amt);
								prfmsg(BUY9,gechrbuf2,item_name[item],plptr->items[item].markup2a,gechrbuf);
								}
							return;
							}
						else
							{
							sprintf(gechrbuf,"%lu",tot);
							sprintf(gechrbuf2,"%lu",amt);
							if (sameas(plptr->userid, warsptr->userid))
								{
								prfmsg(PRICE1,gechrbuf2,item_name[item],baseprice[item],gechrbuf);
								}
							else
								{
								prfmsg(PRICE1,gechrbuf2,item_name[item],plptr->items[item].markup2a,gechrbuf);
								}
							}
						}
					else
						{
						sprintf(gechrbuf,"%lu",tot);
						sprintf(gechrbuf2,"%lu",amt);
						prfmsg(BUY2,gechrbuf,gechrbuf2,item_name[item]);
						}
					}
				else
					{
					sprintf(gechrbuf,"%lu",avail);
					prfmsg(BUY3,gechrbuf,item_name[item]);
					}
				}
			else
				{
				if (strncmp(margv[0],"buy",3) == 0)
					prfmsg(BUY8);
				else
					{
					sprintf(gechrbuf2,"%lu",amt);
					sprintf(gechrbuf,"%lu",price(item,amt));
					if (sameas(plptr->userid, warsptr->userid))
						{
						prfmsg(PRICE2,gechrbuf2,item_name[item],baseprice[item],gechrbuf);
						}
					else
						{
						prfmsg(PRICE2,gechrbuf2,item_name[item],plptr->items[item].markup2a,gechrbuf);
						}
					}
				}
			}
		else
			{
			prfmsg(BUY4,item_name[item]);
			}
		}
	else
		{
		prfmsg(FORMAT,"BUY");
		}
	}
else
	{
	prfmsg(BUY7);
	}
}

unsigned long FUNC amt4sale(item)
int item;

{
unsigned long forsale = 0;

if (sameas(plptr->userid, warsptr->userid))
	forsale = plptr->items[item].qty;
else
	if (plptr->items[item].qty > plptr->items[item].reserve && plptr->items[item].sell == 'Y')
		forsale = plptr->items[item].qty - plptr->items[item].reserve;

if (item == I_GOLD)
	{
	plnum = warsptr->where - 10;
	getplanetdat(usrnum);
	if (neutral(&warsptr->coord) && plnum == 1)
		{
		forsale = waruptr->cash;
		}
	}

return (forsale);
}

long FUNC price(item,amt)
unsigned item;
unsigned long amt;

{
long tot;

if (sameas(plptr->userid, warsptr->userid))
	tot = ((long)baseprice[item])*amt;
else
	tot = ((long)plptr->items[item].markup2a)*amt;

return(tot);
}



/**************************************************************************
** Maintenance and repair                                                **
**************************************************************************/

void FUNC cmd_maint()
{

unsigned price;

if (warsptr->where < 10)
	{
	prfmsg(MAINT1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;


getplanetdat(usrnum);

if (!sameas(plptr->password,"none") && margc < 2)
	{
	prfmsg(MAINT2);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (!sameas(plptr->password,"none")
	&& !sameas(plptr->password,margv[1]))
	{
	prfmsg(MAINT3);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (plptr->userid[0] == 0 || plptr->items[I_MEN].qty < 25000L)
	{
	prfmsg(MAINT8);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord) && chkitm(usrnum))
	warsptr->cantexit = 0;

if (warsptr->cantexit > 0)
	{
	prfmsg(MAINT9);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->cloak > 0 && !sameas(plptr->userid,warsptr->userid))
	{
	prfmsg(MAINT12);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->repair > 0)
	{
	prfmsg(MAINT11);
	outprfge(ALWAYS,usrnum);
	return;
	}

price = 200;

if (neutral(&warsptr->coord))
	{
	if (plnum == 1 || plnum == 2)
		{
		price = 2500;
		}
	else
		{
		prfmsg(MAINT4);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}

if (waruptr->cash >= price)
	{
	warsptr->repair = (unsigned)(warsptr->damage/3.0)+1;
	waruptr->cash -= price;
	prfmsg(MAINT5,warsptr->repair);
	}
else
	{
	prfmsg(MAINT6);
	}

outprfge(ALWAYS,usrnum);

}


/**************************************************************************
** New ship or goods command                                             **
**************************************************************************/

void FUNC cmd_new()
{

int	type,ctype;
long	delta,credit,fee;

if (margc != 3)
	{
	prfmsg(FORMAT,"NEW");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->where < 10)
	{
	prfmsg(NEW1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (neutral(&warsptr->coord) && plnum == 1) /*must be Zygor-3*/
	{
	if (waruptr->factions[gcnum] > 100)	/* don't sell to jerks */
		{
		prfmsg(NOZYG,(int)((waruptr->factions[gcnum]-61)/40));
		outprfge(ALWAYS,usrnum);
		return;
		}
	if (sameas(margv[1],"ship"))
		{
		type = atoi(margv[2])-1;
/* FIX THIS - Change to use full name instead of number */
		if (type >= 0 && type < cyb_class && shipclass[type].max_type == CLASSTYPE_USER)
			{
			if (waruptr->noships < maxships)
				{
				if (shipclass[type].max_price <= waruptr->cash)
					{
					waruptr->cash -= shipclass[type].max_price;
					prfmsg(NEW3,shipclass[type].typename);
					initshp(waruptr->userid,type);
					gepdb(GEADD,tmpshp.userid,tmpshp.shipno,&tmpshp);
					}
				else
					{
					prfmsg(NEW4,shipclass[type].typename);
					}
				}
			else
				{
				prfmsg(NEW16,waruptr->noships);
				}
			}
		else
			{
			prfmsg(NEW2);
			}
		}
	else
	if (sameas(margv[1],"shield"))
		{
		type = atoi(margv[2]);
		if (type <= 0 || type >TOPSHIELD)
			{
			prfmsg(NEW12);
			}
		else
		if (warsptr->shieldtype == type)
			{
			prfmsg(NEW15S,type);
			}
		else
		if (shipclass[warsptr->shpclass].max_shlds >= type)
			{
			ctype = warsptr->shieldtype;
			delta = (shieldprice[ctype-1]-(shieldprice[ctype-1]/3L));

			if (delta > 0)
				{
				prfmsg(NEW19,l2as(delta));
				outprfge(ALWAYS,usrnum);
				}
			else
				delta = 0;

			delta = shieldprice[type-1]-delta;
			credit = 0;

			if (delta < 0)
				{
				credit = (delta*-1);
				fee = credit/50L;
				credit = credit-fee;
				if (credit < 0)
					credit = 0;
				if (waruptr->cash > ULCAP - credit)
					{
					credit = ULCAP - waruptr->cash;
					sprintf(gechrbuf,"%lu",ULCAP);
					prfmsg(TOORICH,gechrbuf);
					}
				prfmsg(NEW18,l2as(fee),l2as(credit));
				outprfge(ALWAYS,usrnum);
				delta = 0;
				}


			if (delta < 1000 && delta > 0)
				{
				prfmsg(NEW17);
				outprfge(ALWAYS,usrnum);
				delta = 1000;
				}

			if (delta <= waruptr->cash)
				{
				waruptr->cash -= delta;
				waruptr->cash += credit;
				warsptr->shieldtype = type;
				if (delta == 0)
					{
					prfmsg(NEW13,type);
					}
				else
					{
					prfmsg(NEW7,l2as(delta),type);
					}
				}
			else
				{
				prfmsg(NEW8,type);
				}
			}
		else
			{
			prfmsg(NEW6,type);
			}
		}
	else
	if (sameas(margv[1],"phaser"))
		{
		type = atoi(margv[2]);
		if (type <= 0 || type >TOPPHASOR)
			{
			prfmsg(NEW12);
			}
		else
		if (warsptr->phasrtype == type)
			{
			prfmsg(NEW15P,type);
			}
		else
		if (shipclass[warsptr->shpclass].max_phasr >= type)
			{
			ctype = warsptr->phasrtype;
			delta = (phaserprice[ctype-1]-(phaserprice[ctype-1]/3L));

			if (delta > 0)
				{
				prfmsg(NEW29,l2as(delta));
				outprfge(ALWAYS,usrnum);
				}
			else
				delta = 0;

			delta = phaserprice[type-1]-delta;
			credit = 0;

			if (delta < 0)
				{
				credit = (delta*-1);
				fee = credit/50L;
				credit = credit-fee;
				if (credit < 0)
					credit = 0;
				if (waruptr->cash > ULCAP - credit)
					{
					credit = ULCAP - waruptr->cash;
					sprintf(gechrbuf,"%lu",ULCAP);
					prfmsg(TOORICH,gechrbuf);
					}
				prfmsg(NEW28,l2as(fee),l2as(credit));
				outprfge(ALWAYS,usrnum);
				delta = 0;
				}

			if (delta < 1000 && delta > 0)
				{
				prfmsg(NEW17);
				outprfge(ALWAYS,usrnum);
				delta = 1000;
				}

			if (delta <= waruptr->cash)
				{
				waruptr->cash -= delta;
				waruptr->cash += credit;
				warsptr->phasrtype = type;
				if (delta == 0)
					{
					prfmsg(NEW14,type);
					}
				else
					{
					prfmsg(NEW10,l2as(delta),type);
					}
				}
			else
				{
				prfmsg(NEW11,type);
				}
			}
		else
			{
			prfmsg(NEW9,type);
			}
		}
	else
		{
		prfmsg(FORMAT,"NEW");
		}
	}
else
	{
	prfmsg(NEW5);
	}
outprfge(ALWAYS,usrnum);
}


/**************************************************************************
** SYSOP commands                                                        **
**************************************************************************/


void FUNC cmd_sysop()
{

int	i,j;
long	amt;
int	gotone;

WARSHP	*ptr;

#ifdef PHARLAP
if ((!syscmds) || (sysonly && !(hasmkey(SYSKEY))))
#else
if ((!syscmds) || (sysonly && !(usrptr->flags&ISYSOP)))
#endif
	{
	prfmsg(INVCMD);
	outprfge(ALWAYS,usrnum);
	return;
	}
if (sameas("factions",margv[1]) && margc == 2)
	{
	for (i=0;i<8;++i)
		{
		prfmsg(FACNAME0+i);
		prf("... %d\r",waruptr->factions[i]);
		}
	outprfge(ALWAYS,usrnum);
	return;
	}
if (sameas("help",margv[1]) && margc == 2)
	{
	setmbk(gehlpmb);
	prfmsg(gehlp[37].helptxt);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("get",margv[1]) && margc == 4)
	{
	if (margc == 4)
		{
		for (i=0; i < NUMITEMS; ++i)
			{
			if (genearas(kwrd[i],margv[3]))
				{
				if ((amt = atol(margv[2])) > 0)
					{
					warsptr->items[i] += amt;
					sprintf(gechrbuf,"%ld",amt);
					prfmsg(SYSGET,gechrbuf,item_name[i]);
					outprfge(ALWAYS,usrnum);
					return;
					}
				}
			}
		}
	}
else
if (sameas("kill",margv[1]) && margc == 3)
	{
	rstrin();
	gotone = FALSE;
	for (othusn=0; othusn < nships ; othusn++)
		{
		if (genearas(margv[2],warshpoff(othusn)->userid))
			{
			warshpoff(othusn)->damage = 101;
			prfmsg(SYSKILL,warshpoff(othusn)->userid);
			outprfge(ALWAYS,usrnum);
			gotone = TRUE;
			}
		}
	if (!gotone)
		{
		prfmsg(SYSKILN);
		outprfge(ALWAYS,usrnum);
		}
	return;
	}
else
if (sameas("cash",margv[1]) && margc == 3)
	{
	waruptr->cash += atol(margv[2]);
	sprintf(gechrbuf,"%lu",atol(margv[2]));
	prfmsg(SYSCASH,gechrbuf);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("midnight",margv[1]) && margc == 2)
	{
	gemidnighta();
	return;
	}
else
if (sameas("goto",margv[1]) && margc == 4)
	{
	if (margc == 4)
		{
		i = atoi(margv[2]);
		j = atoi(margv[3]);
		if (abs(i) <= univmax && abs(j) <= univmax)
			{
			setsect(warsptr);
			if (warsptr->where != 1)
				warsptr->where = 0;
			warsptr->hostile = 0;
			warsptr->coord.xcoord = (double)i + .5;
			warsptr->coord.ycoord = (double)j + .5;

			plnum = 0;
			getplanetdat(usrnum);
			prfmsg(MOVE1,xsect,ysect,i,j);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	}
else
if (sameas("class",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) <= tot_classes && atoi(margv[2]) > 0)
		{
		warsptr->shpclass = atoi(margv[2])-1;
		warsptr->topspeed = shipclass[warsptr->shpclass].max_warp;
		prfmsg(SYSCLS,warsptr->shipname,shipclass[warsptr->shpclass].typename);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
else
if (sameas("shieldtype",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) < 255)
		{
		warsptr->shieldtype = atoi(margv[2]);
		prfmsg(NEW7,"0",warsptr->shieldtype);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}

else
if (sameas("phasertype",margv[1]) && margc == 3)
	{
	if (atoi(margv[2]) < 255)
		{
		warsptr->phasrtype = atoi(margv[2]);
		prfmsg(NEW10,"0",warsptr->phasrtype);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
else
if (sameas("maint",margv[1]))
	{
	if (sameas("now",margv[2]))
		{
		warsptr->damage = 0.0;
		warsptr->repair = 255;
		}
	else
		{
		if (warsptr->cantexit > 0)
			{
			prfmsg(MAINT9);
			outprfge(ALWAYS,usrnum);
			return;
			}
		warsptr->repair = (unsigned)(warsptr->damage/3.0)+1;
		}
	if (warsptr->repair == 255)
		prfmsg(MAINT5,0);
	else
		prfmsg(MAINT5,warsptr->repair);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("unjam",margv[1]))
	{
	warsptr->jam_sev = (byte)0;
	warsptr->jam_time = (byte)0;
	prfmsg(JAMMER5);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("list",margv[1]))
	{
	if (margc == 2)
		i = 0;
	else
		i = atoi(margv[2]);

	prfmsg(SYSLST);
	if (nships>i+50)
		j = i+50;
	else
		j = nships;

	for(;i<j;++i)
		{
		ptr = warshpoff(i);
		if (ptr->status != GESTAT_AVAIL)
			{
			setsect(ptr);
			prf("%3d %-20s %5d %5d %6d %4d %7d %5d %5d %4d\r",i,username(ptr),xsect,ysect,(int)(ptr->damage),(int)(ptr->tick),(int)(ptr->cybmine),(int)(ptr->holdcourse),(int)(ptr->cybupdate),(int)(ptr->lastfired));
			}
		}
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("classlist",margv[1]))
	{
	prfmsg(SYSCLL);
	for(i=0;i<tot_classes;++i)
		{
		if (shipclass[i].max_type != CLASSTYPE_NONE)
			{
			prf("%3d %-34s %5d %10d \r",i+1,
				shipclass[i].typename,
				shipclass[i].cybs_can_att,
				shipclass[i].noclaim);
			}
		}
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("cybpause",margv[1])&& margc == 3)
	{
	i = atoi(margv[2]);
	prfmsg(SYSCYB,i);
	outprfge(ALWAYS,usrnum);
	cybhaltflg = i;
	return;
	}
else
if (sameas("multiply",margv[1]) && (margc > 1 && margc < 4))
	{
	if (warsptr->where < 10)
		{
		prfmsg(ADMIN1);
		outprfge(ALWAYS,usrnum);
		return;
		}
	plnum = warsptr->where - 10;
	getplanetdat(usrnum);
	if (plptr->items[0].qty > 0 && plptr->userid[0] != 0)
		{
		if (margc == 3)
			j = atoi(margv[2]);
		else
			j = 1;
		if (j > 50)
			j = 50;			/* be nice to system resources */
		for (i=0;i<j;++i)
			{
		multiply(TRUE);
			}
		gesdb(GEUPDATE,(PKEY *)&planet,(GALSECT *)&planet);
		prfmsg(SYSPOP,j);
		outprfge(ALWAYS,usrnum);
		return;
		}
	prfmsg(SYSNP);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas("orbit",margv[1]) && (margc == 3))
	{
	if (warsptr->where >= 10)
		{
		prfmsg(ORBIT3);
		outprfge(ALWAYS,usrnum);
		return;
		}

	plnum = (atoi(margv[2]));

	if (plnum <= MAXPLANETS && plnum > 0)
		{
		i = getplanetdat(usrnum);
		if (i)
			{
			if (plptr->type == PLTYPE_WORM)
				{
				prfmsg(ORBIT0);
				outprfge(ALWAYS,usrnum);
				return;
				}
			if (strlen(plptr->name) == 0)
				{
				prfmsg(ORBIT1N,plnum);
				}
			else
				{
				prfmsg(ORBIT1,plnum,plptr->name);
				}
			outprfge(ALWAYS,usrnum);
			sprintf(gechrbuf,"%lu (%d)",plptr->timestamp >> 4,(int)plptr->timestamp & 0xF);
			prf("Planet timestamp: %s\r",gechrbuf);
			outprfge(ALWAYS,usrnum);
			warsptr->where = 10 + plnum;
			warsptr->speed = 0;
			warsptr->speed2b = 0;
			return;
			}
		else
			{
			prfmsg(NOPLNT);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	else
		{
		prfmsg(NOPLNT);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
else
if (sameas("assigncybs",margv[1]) && margc == 2)
	{
	assign_cybs(usrnum,0);
	prfmsg(SYSACY);
	outprfge(ALWAYS,usrnum);
	return;
	}
else

prfmsg(FORMAT,"SYS");
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Rename the ship command..                                             **
**************************************************************************/

void FUNC cmd_rename()
{

if (margc >= 2)
	{
	rstrin();
	strncpy(warsptr->shipname,margv[1],19);
	warsptr->shipname[19] = 0;
	prfmsg(RENAME1,warsptr->shipname);
	}
else
	{
	prfmsg(RENAME3);
	}
outprfge(ALWAYS,usrnum);
}



/**************************************************************************
** Self Destruct                                                         **
**************************************************************************/

void FUNC cmd_destruct()
{
if (!neutral(&warsptr->coord))
	{
	prfmsg(SELFD1);
	outprfge(ALWAYS,usrnum);
	warsptr->destruct = (byte)COUNTDOWN;
	return;
	}
prfmsg(SELFD1A);
outprfge(ALWAYS,usrnum);
}



/**************************************************************************
** Abort Self Destruct                                                   **
**************************************************************************/

void FUNC cmd_abort()
{
if (warsptr->destruct > (byte)0)
	{
	if (warsptr->destruct < 10)
		{
		prfmsg(SELFD4A,warsptr->shipname);
		outrange(ALWAYS,&warsptr->coord);
		}
	prfmsg(SELFD4);
	warsptr->destruct = (byte)0;
	}
else
	{
	prfmsg(SELFD5);
	}
outprfge(ALWAYS,usrnum);
}


/**************************************************************************
** Lock command...                                                       **
**************************************************************************/

void FUNC cmd_lock()
{

int	shpnum;

if (margc == 1)
	{
	warsptr->lock = -1;
	prfmsg(LOCK01);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (margv[1] == NULL)
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	return;
	}

shpnum = findshp(margv[1],1);

if (shpnum >= 0)
	{
	warsptr->lock = shpnum;
	if (warshpoff(shpnum)->status == GESTAT_USER)
		prfmsg(LOCK02, warshpoff(shpnum)->shipname,username(warshpoff(shpnum)));
	else
		prfmsg(LOCK02N, warshpoff(shpnum)->shipname,username(warshpoff(shpnum)));
	outprfge(FILTER,usrnum);
	}
else
	{
	prfmsg(NOSHIP);
	outprfge(ALWAYS,usrnum);
	}
}



/**************************************************************************
** navigate command...                                                   **
**************************************************************************/

void FUNC cmd_navigate()
{
COORD	tmp;
int	x,y;
double	bear;

if (margc == 3)
	{
	x = atoi(margv[1]);
	y = atoi(margv[2]);
	}
else
if (margc == 1)
	{
	x = 0;
	y = 0;
	}
else
	{
	prfmsg(FORMAT,"NAVIGATE");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (abs(x) > univmax || abs(y) > univmax)
	{
	prfmsg(FORMAT,"NAVIGATE");
	outprfge(ALWAYS,usrnum);
	return;
	}

tmp.xcoord = x;
tmp.ycoord = y;

tmp.xcoord +=.50001;
tmp.ycoord +=.50001;


ddistance = cdistance(&(warsptr->coord),&tmp)*10000;

bear = cbearing(&(warsptr->coord),&tmp,warsptr->heading)+.5;
bearing = (int)(cbearing(&(warsptr->coord),&tmp,warsptr->heading));

sprintf(gechrbuf,"NAV from X:%f Y:%f",warsptr->coord.xcoord,warsptr->coord.ycoord);
logthis(gechrbuf);

sprintf(gechrbuf,"NAV to X:%f Y:%f",tmp.xcoord,tmp.ycoord);
logthis(gechrbuf);

sprintf(gechrbuf,"Dist: %f, Bearing: %f",ddistance,bear);
logthis(gechrbuf);

prfmsg(NAV01,x,y,bearing,spr("%ld",(long)ddistance));
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** who command...                                                        **
**************************************************************************/

void FUNC cmd_who()
{
int zothusn;

WARSHP *wptr;

for (zothusn=0; zothusn < nterms; zothusn++)
	if (ingegame(zothusn))
		{
		wptr=warshpoff(zothusn);
		if (wptr->status == GESTAT_USER)
			prf("%s ",username(wptr));
		}
prf("\r");
outprfge(ALWAYS,usrnum);
}


/**************************************************************************
** Set Command                                                           **
**************************************************************************/

void FUNC cmd_set()

{

/* this is temporary until i decide what options i'm putting here */

int invalid = FALSE;

if (margc < 2 || margc > 3)
	{
	prfmsg(FORMAT,"SET");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"scan"))
	{
	if (sameas(margv[2],"simple"))
		waruptr->options[SCANOPTS] = SIMPLE;
	else
	if (sameas(margv[2],"full"))
		waruptr->options[SCANOPTS] = FULL;
	else
	if (sameas(margv[2],"fullnames"))
		waruptr->options[SCANOPTS] = FULLNAMES;
	else
	if (sameas(margv[2],"fullextra"))
		waruptr->options[SCANOPTS] = FULLEXTRA;
	else
	if (sameas(margv[2],"nomap"))
		waruptr->options[SCANOPTS] = NOMAP;
	else
		invalid = TRUE;
	}
else
if (sameas(margv[1],"filter"))
	{
	if (sameas(margv[2],"on"))
		waruptr->options[MSG_FILTER] = TRUE;
	else
	if (sameas(margv[2],"off"))
		waruptr->options[MSG_FILTER] = FALSE;
	else
		invalid = TRUE;
	}
else
if (sameas(margv[1],"?"))
	{
	invalid = 2;
	prf("Option scan set to ");
	if (waruptr->options[SCANOPTS] == SIMPLE)
		prf("simple.\r");
	else
	if (waruptr->options[SCANOPTS] == FULL)
		prf("full.\r");
	else
	if (waruptr->options[SCANOPTS] == FULLNAMES)
		prf("fullnames.\r");
	else
	if (waruptr->options[SCANOPTS] == FULLEXTRA)
		prf("fullextra.\r");
	else
	if (waruptr->options[SCANOPTS] == NOMAP)
		prf("nomap.\r");
	else
		prf("undefined.\r");
	prf("Option filter set to ");
	if (waruptr->options[MSG_FILTER] == TRUE)
		prf("true.\r");
	else
	if (waruptr->options[MSG_FILTER] == FALSE)
		prf("false.\r");
	else
		prf("undefined.\r");
	}
else
	invalid = TRUE;

if (invalid == TRUE)
	prfmsg(FORMAT,"SET");
if (invalid == FALSE)
	prf("Option %s set to %s\r",margv[1],margv[2]);
outprfge(ALWAYS,usrnum);

}

/**************************************************************************
** Team Command                                                          **
**   Team join nnnnnn pppppppppp                                         **
**   Team score                                                          **
**   Team unjoin                                                         **
**   Team start nnnnnn ssssssssss pppppppppp <team name>                 **
**        nnnnnn is teamcode                                             **
**        ssssssssss is the secret password for the founder              **
**        pppppppppp is the public password for team members             **
**   Team members                                                        **
**   Team kick ssssssssss userid                                         **
**   Team newpass ssssssssss pppppppppp                                  **
**   Team newname ssssssssss <team name>                                 **
**                                                                       **
**   Add logic to compare if the team member got kicked off and tell     **
**   him in a mail message. nice                                         **
**                                                                       **
**   Add logic to remove a teamcode that has no members at cleanup       **
**************************************************************************/

void FUNC cmd_team()


{
int	i,j,numteams,next;

long	highscore;
int	highpos;

TEAM	tmp;

int	temptab[MAXTEAMS];


if (margc < 2)
	{
	prfmsg(FORMAT,"TEAM");
	outprfge(ALWAYS,usrnum);
	return;
	}

if (sameas(margv[1],"join"))
	{
	/* got enough parameters */
	if (margc != 4)
		{
		prfmsg(FORMAT,"TEAM");
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* verify that the teamcode is valid */

	strcpy(gechrbuf,margv[2]);

	if (strlen(gechrbuf) != 5)
		{
		prfmsg(FORMAT,"TEAM");
		outprfge(ALWAYS,usrnum);
		return;
		}
	/* are they all digits */
	for (i=0;i<5;++i)
		{
		if (gechrbuf[i] < '0' || gechrbuf[i] > '9')
			{
			prfmsg(FORMAT,"TEAM");
			outprfge(ALWAYS,usrnum);
			return;
			}
		}

	tmp.teamcode = atol(gechrbuf);

	/* verify that this is an actual team */

	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;
		if (tmp.teamcode == teamtab[i].teamcode)
			{
			break;
			}
		}

	if (i >= MAXTEAMS)
		{
		prfmsg(TEAMBAD);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* verify that the password is correct */
	if (!sameas(teamtab[i].password,margv[3]))
		{
		prfmsg(TEAMBADP);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* verify that the team has room */
	if (teamtab[i].teamcount >= team_max)
		{
		prfmsg(TEAMBIG);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* add the teamcode to this users record */

	waruptr->teamcode = tmp.teamcode;


	/* update the team count */
	teamtab[i].teamcount++;

	/* update the disk copy of team database */

	update_team_tab();

	/* tell user that team has been created */

	prfmsg(TEAMJOIN,teamtab[i].teamname);
	outprfge(ALWAYS,usrnum);

	return;
	}
else
if (sameas(margv[1],"score"))
	{
	/*sort the table*/
	highpos = 0;

	for (i=0;i < MAXTEAMS; ++i)
		{
		teamtab[i].flag = 0; /* flag the records for sorting*/
		}

	/* sort the records*/

	for (i=0;i < MAXTEAMS; ++i)
		{
		highscore = 0;
		highpos = 0;
		for (j=0;j < MAXTEAMS;++j)
			{
			if (teamtab[j].teamscore >= highscore && teamtab[j].flag != 1)
				{
				highscore = teamtab[j].teamscore;
				highpos = j;
				}
			}
		teamtab[highpos].flag = 1; /* take it out of the running */
		temptab[i]=highpos;
		}

	prfmsg(TEAMHDR);
	for (i=0;i<MAXTEAMS;++i)
		{
		j = temptab[i];
		if (teamtab[j].teamcode != 0)
			{
			prf("%-6s %-30s %-5d %s\r",
				spr("%ld",teamtab[j].teamcode),
				teamtab[j].teamname,
				teamtab[j].teamcount,
				spr("%ld",teamtab[j].teamscore));
			outprfge(ALWAYS,usrnum);
			}
		}
	prfmsg(TEAMTLR);
	outprfge(ALWAYS,usrnum);
	return;
	}
else
if (sameas(margv[1],"unjoin"))
	{
	if (waruptr->teamcode >0)
		{
		/* verify that this is still a good team */

		for (i=0;i<MAXTEAMS;++i)
			{
			if (teamtab[i].teamcode == 0)
				break;
			if (waruptr->teamcode == teamtab[i].teamcode)
				{
				prfmsg(TEAMUNJN,teamname(waruptr));
				outprfge(ALWAYS,usrnum);
				waruptr->teamcode = 0;
				geudb(GEUPDATE,waruptr->userid,waruptr);

				/* update the team count */

				teamtab[i].teamcount--;
				if (teamtab[i].teamcount > 65000U) /* roll over */
					teamtab[i].teamcount = 0;

				/* update the disk copy of team database */

				update_team_tab();

				return;
				}
			}
		prfmsg(TEAMNOT);
		outprfge(ALWAYS,usrnum);
		}
	else
		{
		prfmsg(TEAMNOT);
		outprfge(ALWAYS,usrnum);
		return;
		}
	}
else
if (sameas(margv[1],"start"))
	{
	if (margc < 6)
		{
		prfmsg(FORMAT,"TEAM");
		outprfge(ALWAYS,usrnum);
		return;
		}
	/* see how many teams are already created */
	numteams = 0;
	for (next=0;next<MAXTEAMS;++next)
		{
		if (teamtab[next].teamcode == 0)
			break;
		numteams++;
		}
	if (numteams >= MAXTEAMS)
		{
		prfmsg(TOOMANY,MAXTEAMS);
		outprf(usrnum);
		return;
		}
	/* verify that the teamcode is valid */
	strcpy(gechrbuf,margv[2]);
	if (strlen(gechrbuf) != 5)
		{
		prfmsg(TEAMBAD);
		outprfge(ALWAYS,usrnum);
		return;
		}
	/* are they all digits */
	for (i=0;i<5;++i)
		{
		if (gechrbuf[i] < '0' || gechrbuf[i] > '9')
			{
			prfmsg(TEAMBAD);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}
	tmp.teamcode = atol(gechrbuf);

	if (tmp.teamcode <= 0)
		{
		prfmsg(TEAMBAD);
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* save off the passwords before we rstrin */
	strncpy(tmp.secret,margv[3],10);
	strncpy(tmp.password,margv[4],10);

	rstrin();
	strncpy(tmp.teamname, margv[5], 30);
	tmp.teamcount = 1;

	/* check to see that this team does not already exist */
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;
		if (tmp.teamcode == teamtab[i].teamcode)
			{
			prfmsg(TEAMEXST);
			outprfge(ALWAYS,usrnum);
			return;
			}
		if (sameas(tmp.teamname,teamtab[i].teamname))
			{
			prfmsg(TEAMEXST);
			outprfge(ALWAYS,usrnum);
			return;
			}
		}

	/* add the team to the team database */

	teamtab[next].teamcode = tmp.teamcode;
	strncpy(teamtab[next].teamname, tmp.teamname, 30);
	teamtab[next].teamcount = tmp.teamcount;
	strncpy(teamtab[next].password, tmp.password, 10);
	strncpy(teamtab[next].secret, tmp.secret, 10);

	/* add the teamcode to this users record */

	waruptr->teamcode = tmp.teamcode;

	/* update the disk copy of team database */

	update_team_tab();

	/* tell user that team has been created */

	prfmsg(TEAMCRT,tmp.teamname,gechrbuf,tmp.password,tmp.secret);
	outprfge(ALWAYS,usrnum);

	return;
	}
else
if (sameas(margv[1],"members"))
	{

	if (waruptr->teamcode == 0)
		{
		prfmsg(TEAMNOT);
		outprfge(ALWAYS,usrnum);
		return;
		}

	setbtv(gebb5);

	j = team_max;
	i = 0;

	if (qeqbtv(&waruptr->teamcode,2))
		{
		prfmsg(TEAMMHDR,gemaxlist);

		do
			{
			gcrbtv(&tmpusr,2);
			if (tmpusr.teamcode == waruptr->teamcode)
				{
				if (i == 0)
					prf("%s",tmpusr.userid);
				else
					prf(", %s",tmpusr.userid);

				outprfge(ALWAYS,usrnum);
				++i;
				}
			else
				{
				break;
				}
			} while (qnxbtv() && i < j);
		prf("\r");
		outprfge(ALWAYS,usrnum);
		return;
		}
	else
		{
		logthis("No one in team yet");
		}
	}
else
if (sameas(margv[1],"kick"))
	{
	if (margc < 4)
		{
		prfmsg(FORMAT,"TEAM");
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* locate this players team in the list*/
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;

		if (waruptr->teamcode == teamtab[i].teamcode)
			{
			/* check to see that the passwords match */
			if (sameas(margv[2],teamtab[i].secret))
				{
				/* check to see if its a valid userid */
				rstrin();
				strncpy(tmpusr.userid,margv[3],UIDSIZ);
				makhdl(tmpusr.userid);
				if (geudb(GELOOKUP,tmpusr.userid,&tmpusr))
					{
					gcrbtv(&tmpusr,0);
					/* check if the player is currently on this team */
					if (tmpusr.teamcode == teamtab[i].teamcode)
						{
						/* reset the teamcode */
						tmpusr.teamcode = 0;
						/* re-write the users record */
						geudb(GEUPDATE,tmpusr.userid,&tmpusr);

						/* send the guy mail telling him he got kicked off the team */
						clrprf();
						prfmsg(TEAMKYOU,teamtab[i].teamname,warsptr->userid);
						strcpy(mail.userid,tmpusr.userid);
						strcpy(mail.topic,"Team Membership Revoked");
						sendit();
						clrprf();

						/* tell this user it is done */
						prfmsg(TEAMKICK,tmpusr.userid);
						outprfge(ALWAYS,usrnum);
						return;
						}
					else
						{
						prfmsg(TEAMNTM);
						outprfge(ALWAYS,usrnum);
						}

					}
				else
					{
					prfmsg(TEAMNFND);
					outprfge(ALWAYS,usrnum);
					}
				}
			else
				{
				prfmsg(TEAMBDSC);
				outprfge(ALWAYS,usrnum);
				}
			return;
			}
		}
	prfmsg(TEAMNOT);
	outprfge(ALWAYS,usrnum);
	}
else
if (sameas(margv[1],"newpass"))
	{

	if (margc < 4)
		{
		prfmsg(FORMAT,"TEAM");
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* locate this players team in the list*/
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;

		if (waruptr->teamcode == teamtab[i].teamcode)
			{
			/* check to see that the passwords match */
			if (sameas(margv[2],teamtab[i].secret))
				{
				/* get the password - make sure it is less than 10 char */
				if (strlen(margv[3]) > 10)
					{
					prfmsg(TEAMBPSS);
					outprfge(ALWAYS,usrnum);
					return;
					}
				strcpy(teamtab[i].password,margv[3]);
				prfmsg(TEAMNPSS,teamtab[i].password);
				outprfge(ALWAYS,usrnum);
				update_team_tab();
				return;
				}
			else
				{
				prfmsg(TEAMBDSC);
				outprfge(ALWAYS,usrnum);
				}
			return;
			}
		}
	prfmsg(TEAMNOT);
	outprfge(ALWAYS,usrnum);
	}
else
if (sameas(margv[1],"newname"))
	{
	if (margc < 4)
		{
		prfmsg(FORMAT,"TEAM");
		outprfge(ALWAYS,usrnum);
		return;
		}

	/* locate this players team in the list*/
	for (i=0;i<MAXTEAMS;++i)
		{
		if (teamtab[i].teamcode == 0)
			break;

		if (waruptr->teamcode == teamtab[i].teamcode)
			{
			/* check to see that the passwords match */
			if (sameas(margv[2],teamtab[i].secret))
				{
				/* get the new teamname - make sure it is at least 5 char long */
				rstrin();
				if (strlen(margv[3]) < 5)
					{
					prfmsg(TEAMBNAM);
					outprfge(ALWAYS,usrnum);
					return;
					}
				strncpy(teamtab[i].teamname,margv[3],30);
				prfmsg(TEAMNNAM,teamtab[i].teamname);
				outprfge(ALWAYS,usrnum);
				update_team_tab();
				return;
				}
			else
				{
				prfmsg(TEAMBDSC);
				outprfge(ALWAYS,usrnum);
				}
			return;
			}
		}
	prfmsg(TEAMNOT);
	outprfge(ALWAYS,usrnum);
	}
else
if (sameas(margv[1],"dumpitout"))
	{
	for (i=0;i<MAXTEAMS;++i)
		{
		prf("code name count score\r\n");
		sprintf(gechrbuf,"|%5ld|%-30s|%5d|%10ld|%-10s|%-10s|\r\n",
			teamtab[i].teamcode,
			teamtab[i].teamname,
			teamtab[i].teamcount,
			teamtab[i].teamscore,
			teamtab[i].password,
			teamtab[i].secret);
		prf(gechrbuf);
		outprfge(ALWAYS,usrnum);
		}
	}
prfmsg(FORMAT,"TEAM");
outprfge(ALWAYS,usrnum);
}

char	* FUNC teamname(WARUSR *ptr)
{
int i;
static	char	badteamname[]={"Invalid Team Code"};

for (i=0;i<MAXTEAMS;++i)
	{
	if (teamtab[i].teamcode == 0)
		return(&badteamname[0]);
	if (ptr->teamcode == teamtab[i].teamcode)
		{
		return(teamtab[i].teamname);
		}
	}
return(&badteamname[0]);
}

void FUNC cmd_clear()

{
prf("\33[2J\33[0;0H");
outprfge(ALWAYS,usrnum);
}

void FUNC cmd_data()
{

#ifdef DATACMD

int i,j;
if (margc != 3)
	{
	prfmsg(INVCMD);
	outprfge(ALWAYS,usrnum);
	return;
	};

if (!sameas(margv[1],"qazwsx"))
	{
	prfmsg(INVCMD);
	outprfge(ALWAYS,usrnum);
	return;
	};


if (sameas(margv[2],"report"))
	{
	prf("UD1:%s,%d,%d,%d,%d*\r",
		waruptr->userid,
		waruptr->noships,
		waruptr->kills,
		waruptr->rospos,
		waruptr->planets);
	sprintf(gechrbuf,"%ld",waruptr->score);
	sprintf(gechrbuf2,"%lu",waruptr->cash);
	sprintf(gechrbuf3,"%ld",waruptr->population);
	prf("UD2:%s,%s,%s*\r",gechrbuf,gechrbuf2,gechrbuf3);
	outprfge(ALWAYS,usrnum);

	setsect(warsptr);

	prf("SD1:%s,%d*\r",
		warsptr->shipname,
		warsptr->shpclass);

	prf("SD2:%s,%s,%d,%d,%d,%d,%s,%s*\r",
		spr("%ld",(long)warsptr->heading),
		spr("%ld",(long)warsptr->speed),
		xsect,ysect,xcord,ycord,
		spr("%ld",(long)warsptr->damage),
		spr("%ld",(long)warsptr->energy));

	prf("SD3:%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d*\r",
		spr("%ld",(long)warsptr->phasr),
		warsptr->phasrtype,
		warsptr->kills,
		warsptr->lastfired,
		warsptr->shieldtype,
		warsptr->shieldstat,
		warsptr->shield,
		warsptr->cloak,
		warsptr->tactical,
		warsptr->helm,
		warsptr->train,
		warsptr->where);

	prf("SD4:");
	for (i=0;i<MAXTORPS;++i)
		prf("T%d:%u-%u,",i,warsptr->ltorps[i].channel,warsptr->ltorps[i].distance);
	prf("*\r");

	prf("SD5:");
	for (i=0;i<MAXMISSL;++i)
		prf("M%d:%u-%u,",i,warsptr->lmissl[i].channel,warsptr->lmissl[i].distance);
	prf("*\r");

	prf("SD6:");
	for (i=0;i<NUMITEMS;++i)
		prf("I%d:%s,",i,spr("%ld",warsptr->items[i]));
	prf("*\r");

	j=0;
	for(i=0;i<MAXDECOY;++i)
		if (warsptr->decout[i] > 0)
			++j;

	prf("SD7:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d*\r",
		j,
		(int)warsptr->jam_time,
		warsptr->kills,
		warsptr->freq[0],
		warsptr->freq[1],
		warsptr->freq[2],
		warsptr->hostile,
		warsptr->cantexit,
		warsptr->repair,
		warsptr->hypha,
		warsptr->firecntl,
		warsptr->destruct,
		warsptr->status);

	outprfge(ALWAYS,usrnum);
	return;
	}
if (sameas(margv[2],"scan"))
	{
	scan_data1();
	return;
	}
if (sameas(margv[2],"sector"))
	{
	scan_data2();
	return;
	}

prfmsg(INVCMD);
outprfge(ALWAYS,usrnum);
}


void FUNC scan_data1()

{
SCANTAB	*sptr;
WARSHP	*wptr;
int	i,j;

char	mask[] = {" %c %d %d %d %d %s %d %d %s %d/%s\r"};


prf("DataScan: Range: %s\r",spr("%6ld",shipclass[warsptr->shpclass].scanrange));

update_scantab(warsptr,usrnum);

sptr = &scantab[usrnum];

prf("Shp Xsect Ysect Xcoord Ycoord Distance Bearing Heading Speed Class\r");

setsect(warsptr);

prf(mask,'*',xsect,ysect,xcord,ycord,"0",0,
	(int)warsptr->heading,showarp(warsptr->speed),
	warsptr->shpclass,shipclass[warsptr->shpclass].typename);

if (warsptr->jam_sev > (byte)2)
	{
	prf("** Jammed **\r");
	outprfge(ALWAYS,usrnum);
	return;
	}

for(i=0; i<NOSCANTAB;++i)
	{
	if (sptr->ship[i].flag == 1)
		{
		wptr = warshpoff(sptr->ship[i].shipno);

		setsect(wptr);

		j = wptr->shpclass;
		prf(mask,sptr->ship[i].letter,xsect,ysect,xcord,ycord,spr("%ld",(long)(sptr->ship[i].dist)),
			sptr->ship[i].bearing,sptr->ship[i].heading,showarp(sptr->ship[i].speed),
			j,shipclass[j].typename);
		}
	}

outprfge(ALWAYS,usrnum);
}

void FUNC scan_data2()

{
unsigned i,x,y;

refresh(warsptr,usrnum);

setsect(warsptr);
prf("Datascan: Sector X:%u Y:%u\r",xsect,ysect);

getsector(&warsptr->coord);

prf("NPlnts = %d\r",sector.numplan);
for (i=0;i < sector.numplan;++i)
	{
	if (sector.ptab[i].coord.xcoord != 0)
		{
		x = coord2(sector.ptab[i].coord.xcoord);
		y = coord2(sector.ptab[i].coord.ycoord);
		prf("Pl#%d: Xcoord:%d, Ycoord:%d, Type:%d\r",i+1,x,y,sector.ptab[i].type);
		}
	}

#else

prfmsg(INVCMD,usrnum);

#endif

outprfge(ALWAYS,usrnum);

}

char	* FUNC gedots(int numdots)
{
int	i;
static char	dotbuf[41];

if (numdots >40)
	numdots = 40;

for (i=0;i<numdots;++i)
	dotbuf[i]='.';
dotbuf[numdots]=0;
return (dotbuf);
}


/**************************************************************************
** Spy Command                                                           **
**************************************************************************/

void FUNC cmd_spy()

{

if (warsptr->where < 10)
	{
	prfmsg(ADMIN1);
	outprfge(ALWAYS,usrnum);
	return;
	}

plnum = warsptr->where - 10;

getplanetdat(usrnum);

if (sameas(plptr->userid,warsptr->userid))
	{
	prfmsg(SPY0);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (neutral(&warsptr->coord))
	{
	prfmsg(SPY0C);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (warsptr->items[I_SPY] > 0)
	{
	warsptr->items[I_SPY]--;
	prfmsg(SPYM1);
	outprfge(ALWAYS,usrnum);
	strcpy(plptr->spyowner,warsptr->userid);

	setsect(warsptr); /* build PKEY */
	pkey.plnum = plnum;
	gesdb(GEUPDATE,&pkey,(GALSECT *)&planet);

	return;
	}
else
	{
	prfmsg(SPYM0);
	}
outprfge(ALWAYS,usrnum);
}

/**************************************************************************
** Jettison Command                                                      **
**************************************************************************/

void FUNC cmd_jettison()

{
int i;

if (margc == 3)
	{
	for (i=0; i < NUMITEMS; ++i)
		{
		if (genearas(kwrd[i],margv[2]))
			{
			jettison(i);
			return;
			}
		}
	}

prfmsg(FORMAT,"JETTISON");
outprfge(ALWAYS,usrnum);
}

void FUNC jettison(item)
int	item;

{
unsigned long amt;
int	defuse = FALSE;

if (item == I_MEN || item == I_TROOPS || item == I_SPY)
	{
	prfmsg(JETT2,item_name[item]);
	outprfge(ALWAYS,usrnum);
	return;
	}

if (item == I_TORPEDO || item == I_MINE)
	defuse = TRUE;

if (sameas("ALL",margv[1]) > 0L)
	{
	amt = warsptr->items[item];
	warsptr->items[item] = 0;
	sprintf(gechrbuf,"%ld",amt);
	if (defuse == TRUE)
		prfmsg(JETT3D,gechrbuf,item_name[item]);
	else
		prfmsg(JETT3,gechrbuf,item_name[item]);
	outprfge(ALWAYS,usrnum);
	gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
	return;
	}
else
if ((amt = atol(margv[1])) > 0L)
	{
	if (warsptr->items[item] >= amt)
		{
		warsptr->items[item] -= amt;
		sprintf(gechrbuf,"%ld",amt);
		if (defuse == TRUE)
			prfmsg(JETT3D,gechrbuf,item_name[item]);
		else
			prfmsg(JETT3,gechrbuf,item_name[item]);
		outprfge(ALWAYS,usrnum);
		gepdb(GEUPDATE,warsptr->userid,warsptr->shipno,warsptr);
		return;
		}
	else
		{
		prfmsg(JETT1);
		outprfge(ALWAYS,usrnum);
		}
	}
else
	{
	prfmsg(FORMAT,"JETTISON");
	outprfge(ALWAYS,usrnum);
	}
}
