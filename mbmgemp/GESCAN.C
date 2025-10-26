/***************************************************************************
 *                                                                         *
 *   GESCAN.C                                                              *
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
#include "message.h"

#include "gemain.h"
#include "geglobal.h"

#define GESCAN 1

/* out of range mask for printmap */
const int scan_side_blocks[15] =
	{7, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1, 3, 4, 7};

/**************************************************************************
** Functions for printmap()                                              **
**************************************************************************/

void FUNC set_map_color(int color)
{
switch (color)
	{
	case 1: prf(CLR_RED2); break;
	case 2: prf(CLR_GREEN2); break;
	case 3: prf(CLR_YELLOW2); break;
	case 4: prf(CLR_BLUE2); break;
	case 5: prf(CLR_BLUE1); break;
	case 6: prf(CLR_RED1); break;
	default: prf(CLR_WHITE2); break;
	}
}

void FUNC print_ship_letter(int othusn, char letter)
{
int type = shipclass[warshpoff(othusn)->shpclass].max_type;

if (waruptr->options[JAMTEST] > 2 && (gernd()%(20 / waruptr->options[JAMTEST])) == 0)
	letter = '?';

if (type == CLASSTYPE_CYBORG)
	prf("%s%c%s", CLR_RED2, letter, CLR_WHITE2);
else
if (type == CLASSTYPE_DROID)
	prf("%s%c%s", CLR_BLUE2, letter, CLR_WHITE2);
else
	prf("%s%c%s", CLR_GREEN2, letter, CLR_WHITE2);
}

void FUNC print_ship_data(long dist, int bearing, int heading, double speed)
{
prf("%s    %4d    %4d%9s", spr("%6ld", dist), bearing, heading, showarp(speed));
}

void FUNC print_ship_name(int othusn)
{
if (warsptr->lock == othusn)
	prf("%s*%s%s%s*", CLR_RED1, CLR_CYAN1, username(warshpoff(othusn)), CLR_RED1);
else
if (warshpoff(othusn)->distress != 255)
	prf("%s*%s%s%s*", CLR_GREEN2, CLR_CYAN1, username(warshpoff(othusn)), CLR_GREEN2);
else
	prf(" %s%s", CLR_CYAN1, username(warshpoff(othusn)));
}

int FUNC print_planet_line(int shp)
{
unsigned int rseed = gernd();
int jam_digits, len, k;

if (ptab[usrnum].planets[shp].type == PLTYPE_WORM)
	prf(CLR_YELLOW2);
else
	prf(CLR_BLUE1);

if (rseed%(waruptr->options[JAMTEST]+1) > 6)
	return(FALSE);
if (rseed%(waruptr->options[JAMTEST]+1) > 3)
	sprintf(gechrbuf,"%c",'?');
else
	sprintf(gechrbuf,"%d",shp + 1);

sprintf(gechrbuf2, "%d", (int)(cdistance(&warsptr->coord, &ptab[usrnum].planets[shp].coord) * 10000));
sprintf(gechrbuf3, "%d", (int)(cbearing(&warsptr->coord, &ptab[usrnum].planets[shp].coord, warsptr->heading) + .5));

/* if jammed, mess up the numbers */
if (waruptr->options[JAMTEST] > 2)
	{
	jam_digits = rseed % (((waruptr->options[JAMTEST] < 6) ?
		(waruptr->options[JAMTEST] / 2) : (waruptr->options[JAMTEST] - 2)) + 1);
	len = strlen(gechrbuf2);
	if (jam_digits > len)
		jam_digits = len;
	for (k = len - jam_digits; k < len; k++)
		gechrbuf2[k] = '?';

	jam_digits = (rseed >> 4) % (((waruptr->options[JAMTEST] < 6) ?
		(waruptr->options[JAMTEST] / 2) : (waruptr->options[JAMTEST] - 2)) + 1);
	len = strlen(gechrbuf3);
	if (jam_digits > len)
		jam_digits = len;
	for (k = len - jam_digits; k < len; k++)
		if (gechrbuf3[k] != '-')	/* always keep negative symbol */
			gechrbuf3[k] = '?';
	}

prf("     %s%s %8s %7s", gechrbuf, CLR_WHITE2, gechrbuf2, gechrbuf3);

if (warsptr->where - 11 == shp)
	prf("   (orbiting)");
return(TRUE);
}

void FUNC print_map_header(int maptype)
{
switch (maptype)
	{
	case SECTORNOMAP:
		/* if no planets in ptab, don't print header */
		{
		if (ptab[usrnum].planets[0].type != 0)
			prfmsg(NOMAPSE);
		break;
		}
	case RANGENOMAP:
		/* if no ships in scantab, don't print header */
		{
		if (scantab[usrnum].ship[0].flag == 1)
			prfmsg(NOMAPRA);
		break;
		}
	case SECTORFULL:
		{
		if (ptab[usrnum].planets[0].type != 0)
			prfmsg(PLUSSECT);
		else
			prfmsg(PLUSDASH);
		break;
		}
	case RANGEFULL:
	case RANGENAMES:
	case RANGEEXTRA: prfmsg(PLUSFULL); break;
	default:
		prfmsg(PLUSDASH);
		break;
	}
prf("\r");
}

void FUNC print_map_row(int i, int maptype)
{
int j = 0, start;
int prev_color = 6, cur_color;
unsigned int rseed = gernd();

prf("%s   |", CLR_RED1);
while (j < MAXX)
	{
	/* add red out of range highlighting */
	if (maptype >= RANGE && maptype <= RANGEEXTRA &&
		(j < scan_side_blocks[i] || j >= MAXX - scan_side_blocks[i]))
		{
		start = j;
		while (j < MAXX && (j < scan_side_blocks[i] || j >= MAXX - scan_side_blocks[i]))
		j++;

		if (prev_color != 6)
			{
			prf(CLR_RED1);
			prev_color = 6;
			}
		prf("%s", gedots(j - start));
		}
	else
	if (waruptr->options[JAMTEST] > 0)
		{
		if (!(map[i][j] == '.' && mapc[i][j] == '4') && map[i][j] != '*') /* don't overwrite out of bounds or player indicator */
			{
			/* cheap xorshift random, avoid a gazillion gernd calls */
			rseed ^= rseed << 7;
			rseed ^= rseed >> 9;
			rseed ^= rseed << 8;
			if (map[i][j] != ' ')
				{
				if (rseed%(waruptr->options[JAMTEST]) > 4) /* jam level increases chance of legit object blanked */
					map[i][j] = ' ';
				}
			if (map[i][j] == ' ') /* scramble empty spaces and blanked objects */
				{
				if (rseed%(80/waruptr->options[JAMTEST]) == 0)
					{
					map[i][j] = (byte)((rseed%94)+33); /* printable ascii range */
					if (maptype != LONG && rseed%6 == 0)	/* add color speckles if not sca lo */
						mapc[i][j] = (byte)((rseed%7)+48); /* ascii 0-6 */
					}
				}
			}
		}
	if (map[i][j] != ' ') /* if empty space, color code change not needed */
		{
		switch (mapc[i][j])
			{
			case '1': cur_color = 1; break;
			case '2': cur_color = 2; break;
			case '3': cur_color = 3; break;
			case '4': cur_color = 4; break;
			case '5': cur_color = 5; break;
			case '6': cur_color = 6; break;
			default: cur_color = 0; break;
			}
		if (prev_color != cur_color)
			{
			set_map_color(cur_color);
			prev_color = cur_color;
			}
		}

	prf("%c", map[i][j]);
	j++;
	}

if (prev_color != 6)
	prf(CLR_RED1);
prf("|");
}

/* RANGEEXTRA / RANGENOMAP pairs */
void FUNC print_ship_pair(SCANTAB *sptr, int left, int right, long dist_filter)
{
int i, pad, othusn;

othusn = sptr->ship[left].shipno;
prf("   ");
print_ship_letter(othusn, sptr->ship[left].letter);
prf("   ");
print_ship_data(sptr->ship[left].dist, sptr->ship[left].bearing, sptr->ship[left].heading, sptr->ship[left].speed);

if (right < NOSCANTAB && sptr->ship[right].flag == 1 && (long)(sptr->ship[right].dist) < dist_filter)
	{
	othusn = sptr->ship[right].shipno;
	prf("  ");
	print_ship_letter(othusn, sptr->ship[right].letter);
	prf("   ");
	print_ship_data(sptr->ship[right].dist, sptr->ship[right].bearing,
		sptr->ship[right].heading, sptr->ship[right].speed);
	}
prf("\r");

othusn = sptr->ship[left].shipno;
pad = strlen(username(warshpoff(othusn)));
prf("   ");
print_ship_name(othusn);
for (i = 0; i < 36 - pad; ++i)
	prf(" ");

if (right < NOSCANTAB && sptr->ship[right].flag == 1 && (long)(sptr->ship[right].dist) < dist_filter)
	{
	othusn = sptr->ship[right].shipno;
	print_ship_name(othusn);
	}

prf("\r");
}

/* RANGENAMES (or first eight of RANGEEXTRA) */
int FUNC print_range_line(SCANTAB *sptr, int shp, int *ff, long dist_filter)
{
if (shp < NOSCANTAB && sptr->ship[shp].flag == 1 &&
	shp < (MAXY + 1) / 2 && (long)(sptr->ship[shp].dist) < dist_filter)
	{
	othusn = sptr->ship[shp].shipno;
	prf("     ");

	if (*ff == 0)
		{
		print_ship_letter(othusn, sptr->ship[shp].letter);
		prf("   ");
		print_ship_data(sptr->ship[shp].dist, sptr->ship[shp].bearing,
			sptr->ship[shp].heading, sptr->ship[shp].speed);
		*ff = 1;
		}
	else
		{
		print_ship_name(othusn);
		*ff = 0;
		return 1; /* move onto next ship */
		}
	}
return 0; /* first line or no ship, don't increment */
}

/* RANGEFULL */
int FUNC print_fullrange_line(SCANTAB *sptr, int shp, long dist_filter)
{
if (shp < NOSCANTAB && sptr->ship[shp].flag == 1 && (long)(sptr->ship[shp].dist) < dist_filter)
	{
	othusn = sptr->ship[shp].shipno;
	prf("     ");
	print_ship_letter(othusn, sptr->ship[shp].letter);
	prf("   ");
	print_ship_data(sptr->ship[shp].dist, sptr->ship[shp].bearing,
		sptr->ship[shp].heading, sptr->ship[shp].speed);
        return 1; /* move onto next ship */
	}
return 0; /* no ship, don't increment */
}

/* RANGEEXTRA / RANGENOMAP */
void FUNC print_range_summary(SCANTAB *sptr, int shp, long dist_filter, int maptype)
{
for (; shp < NOSCANTAB && sptr->ship[shp].flag == 1 &&
	(long)(sptr->ship[shp].dist) < dist_filter; shp += 2)
	{
	print_ship_pair(sptr, shp, shp + 1, dist_filter);
	}

if (maptype == RANGENOMAP && shp == 0) /* no ships */
	prfmsg(SCANNOSH);
}

/* SECTORNOMAP */
void FUNC print_planet_summary(int shp)
{
for (; shp < MAXPLANETS && ptab[usrnum].planets[shp].type != 0; ++shp)
	{
	if (print_planet_line(shp) == TRUE)
		prf("\r");
	}

if (shp == 0) /* no planets */
	prfmsg(SCANNOPL);
}
