
/***************************************************************************
 *                                                                         *
 *   GELIB.C                                                               *
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


#endif

#include "math.h"
#include "majorbbs.h"

#include "gemain.h"
#include "geglobal.h"

/**************************************************************************
** Determine the smallest of two complementary angles                    **
**************************************************************************/

unsigned smallest(a1,a2)
unsigned a1,a2;

{
int a;

a = abs(a1-a2);

if (a > 180)
	return (360 - a);
else
	return (a);
}


/**************************************************************************
** Generate a random number                                              **
**************************************************************************/

double rndm(mod)
double mod;
{

static randmax = (double)RAND_MAX;

return(mod*(((double)((unsigned)rand()))/randmax));
}


unsigned int gernd()

{
return(rand());
}


double clamp_acos(v)
double v;
{
if (v > 1.0)
	return 1.0;
if (v < -1.0)
	return -1.0;
return v;
}

/**************************************************************************
** Calculate ship bearing between two objects                            **
**************************************************************************/

int	cbearing(ptr1, ptr2, heading)
COORD	*ptr1,*ptr2;
double	heading;

{
double b;

b = vector(ptr1, ptr2);
b = normal(360.0 - heading + b);

if (b > 180.0)
	b -= 360.0;
else
if (b < -180.0)
	b += 360.0;

/* do the rounding here so we can cast to int cleanly elsewhere */
if (b < 0.0)
        b -= 0.4999;
    else
        b += 0.4999;

/* never show 180 as negative */
if (b <= -180)
	b = 180;

return (int)b;
}

/**************************************************************************
** Calculate the distance between two ships                              **
**************************************************************************/

double	cdistance(ptr1,ptr2)
COORD	*ptr1,*ptr2;
{
double dx, dy;

dx = ptr1->xcoord - ptr2->xcoord;
dy = ptr1->ycoord - ptr2->ycoord;

return sqrt((dx * dx) + (dy * dy));
}

/**************************************************************************
** Calculate the angle from one ship to another                          **
**************************************************************************/

double	vector(ptr1,ptr2)
COORD	*ptr1, *ptr2;

{
double	da, dx, dy, raw;

/* handle exact alignment on an axis */
if (fabs(ptr1->ycoord - ptr2->ycoord) <= VEC_EPS)
	{
	if (ptr1->xcoord < ptr2->xcoord)
		return 90.0;
	else
	if (ptr1->xcoord > ptr2->xcoord)
		return 270.0;
	else
		return 0.0;
	}

if (fabs(ptr1->xcoord - ptr2->xcoord) <= VEC_EPS)
	{
	if (ptr1->ycoord < ptr2->ycoord)
		return 180.0;
	else
		return 0.0;
	}

dx = ptr2->xcoord - ptr1->xcoord;
dy = ptr2->ycoord - ptr1->ycoord;
da = sqrt((dx*dx) + (dy*dy));

raw = radtodeg(acos(clamp_acos((-dy) / da)));

if (dx >= 0.0)
	return raw;
else
	return 360.0 - raw;
}

/**************************************************************************
** Bring an angle back into the range 0 - 360                            **
**************************************************************************/

double	normal (angle)
double	angle;
{
while (angle < 0.0)
	angle += 360.0;
while (angle >= 360.0)
	angle -= 360.0;
return angle;
}

/**************************************************************************
** convert degrees to radiuns                                            **
**************************************************************************/

double degtorad(value)
double value;
{
return (value*(PI/180));
}

/**************************************************************************
** convert radiuns to degrees                                            **
**************************************************************************/

double radtodeg(value)
double value;
{
return (value*(180/PI));
}
