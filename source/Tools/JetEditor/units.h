/*
	@file units.h
	@author paradoxnj
	@brief Measurement conversions

	@par license
	The contents of this file are subject to the Jet3D Public License
	Version 1.02 (the "License"); you may not use this file except in
	compliance with the License. You may obtain a copy of the License at
	http://www.jet3d.com

	@par
	Software distributed under the License is distributed on an "AS IS"
	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
	the License for the specific language governing rights and limitations
	under the License.

	@par
	The Original Code is Jet3D, released December 12, 1999.
	Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved
*/
#pragma once

#include "basetype.h"
#include <math.h>
 
#ifndef M_PI
//#define	M_PI		((jeFloat)3.14159265358979323846f)
#endif

static const jeFloat M_PI = 3.14159265358979323846f;

#define PI2				((jeFloat)(2.0f * (M_PI)))
#define ONE_OVER_2PI	((jeFloat)(1.0f/(PI2)))

// some useful unit conversions
#define Units_DegreesToRadians(d) ((((jeFloat)(d)) * M_PI) / 180.0f)
#define Units_RadiansToDegrees(r) ((((jeFloat)(r)) * 180.0f) / M_PI)

#define UNITS_DEGREES_TO_RADIANS(d) Units_DegreesToRadians(d)
#define UNITS_RADIANS_TO_DEGREES(r) Units_RadiansToDegrees(r)

// Engine <--> Centimeter conversions
#define Units_CentimetersToEngine(c) (((float)(c)) / 2.54f)
#define Units_EngineToCentimeters(i) (((float)(i)) * 2.54f)

//#define CENTIMETERS_TO_ENGINE(c) Units_CentimetersToEngine(c)
//#define ENGINE_TO_CENTIMETERS(e) Units_EngineToCentimeters(e)

#define Units_Round(n) ((int)Units_FRound((n)))
#define Units_Trunc(n) ((int)(n))
#define Units_FRound(n)	((jeFloat)floor((n)+0.5f))
