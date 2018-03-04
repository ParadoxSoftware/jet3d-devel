/****************************************************************************************/
/*  EXTBOX.C                                                                            */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description: Axial aligned bounding box support                                     */
/*                                                                                      */
/*  The contents of this file are subject to the Jet3D Public License                   */
/*  Version 1.02 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.jet3d.com                                                                */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Jet3D, released December 12, 1999.                             */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#include "ExtBox.h"
#include <assert.h>
#include <intrin.h>

#define MAX(aa,bb)   ( ((aa)>(bb))?(aa):(bb) )
#define MIN(aa,bb)   ( ((aa)<(bb))?(aa):(bb) )

// Added by Icestorm
JETAPI jeBoolean JETCC jeExtBox_IsPoint(  const jeExtBox *B )
{
	assert (B != NULL);
	if (!JE_FLOATS_EQUAL(B->Min.X,B->Max.X))
		return JE_FALSE;
    if (!JE_FLOATS_EQUAL(B->Min.Y,B->Max.Y))
		return JE_FALSE;
	if (!JE_FLOATS_EQUAL(B->Min.Z,B->Max.Z))
		return JE_FALSE;
	else
		return JE_TRUE;
}


JETAPI jeBoolean JETCC jeExtBox_IsValid(  const jeExtBox *B )
{
	if (B == NULL) return JE_FALSE;
	
	if (jeVec3d_IsValid(&(B->Min)) == JE_FALSE)
		return JE_FALSE;
	if (jeVec3d_IsValid(&(B->Max)) == JE_FALSE)
		return JE_FALSE;


	if (    (B->Min.X <= B->Max.X) &&
			(B->Min.Y <= B->Max.Y) &&
			(B->Min.Z <= B->Max.Z)   	)
		return JE_TRUE;
	else
		return JE_FALSE;
}

JETAPI void JETCC jeExtBox_Set(  jeExtBox *B,
					jeFloat X1,jeFloat Y1,jeFloat Z1,
					jeFloat X2,jeFloat Y2,jeFloat Z2)
{
	assert (B != NULL);

	//jeVec3d_Set	(&B->Min, MIN (x1, x2),	MIN (y1, y2),MIN (z1, z2));
	//jeVec3d_Set (&B->Max, MAX (x1, x2),	MAX (y1, y2),MAX (z1, z2));

	if ( X1 > X2 )
		{	B->Max.X = X1;	B->Min.X = X2;	}
	else
		{	B->Max.X = X2;  B->Min.X = X1;  }
	
	if ( Y1 > Y2 )
		{	B->Max.Y = Y1;	B->Min.Y = Y2;	}
	else
		{	B->Max.Y = Y2;  B->Min.Y = Y1;  }
	
	if ( Z1 > Z2 )
		{	B->Max.Z = Z1;	B->Min.Z = Z2;	}
	else
		{	B->Max.Z = Z2;  B->Min.Z = Z1;  }

	assert( jeVec3d_IsValid(&(B->Min)) != JE_FALSE );
	assert( jeVec3d_IsValid(&(B->Max)) != JE_FALSE );

}

// Set box Min and Max to the passed point
JETAPI void JETCC jeExtBox_SetToPoint ( jeExtBox *B, const jeVec3d *Point )
{
	assert( B     != NULL );
	assert( Point != NULL );
	assert( jeVec3d_IsValid(Point) != JE_FALSE );

	
	B->Max = *Point;
	B->Min = *Point;
}

// Extend a box to encompass the passed point
JETAPI void JETCC jeExtBox_ExtendToEnclose( jeExtBox *B, const jeVec3d *Point )
{
	assert ( jeExtBox_IsValid(B) != JE_FALSE );
	assert( Point != NULL );
	assert( jeVec3d_IsValid(Point) != JE_FALSE );

	if (Point->X > B->Max.X ) B->Max.X = Point->X;
	if (Point->Y > B->Max.Y ) B->Max.Y = Point->Y;
	if (Point->Z > B->Max.Z ) B->Max.Z = Point->Z;

	if (Point->X < B->Min.X ) B->Min.X = Point->X;
	if (Point->Y < B->Min.Y ) B->Min.Y = Point->Y;
	if (Point->Z < B->Min.Z ) B->Min.Z = Point->Z;

}

static jeBoolean JETCC jeExtBox_Intersects(  const jeExtBox *B1,  const jeExtBox *B2 )
{
	assert ( jeExtBox_IsValid (B1) != JE_FALSE );
	assert ( jeExtBox_IsValid (B2) != JE_FALSE );

	if ((B1->Min.X > B2->Max.X) || (B1->Max.X < B2->Min.X)) return JE_FALSE;
	if ((B1->Min.Y > B2->Max.Y) || (B1->Max.Y < B2->Min.Y)) return JE_FALSE;
	if ((B1->Min.Z > B2->Max.Z) || (B1->Max.Z < B2->Min.Z)) return JE_FALSE;
	return JE_TRUE;
}


	
JETAPI jeBoolean JETCC jeExtBox_Intersection( const jeExtBox *B1, const jeExtBox *B2, jeExtBox *Result )
{
	jeBoolean rslt;

	assert ( jeExtBox_IsValid (B1) != JE_FALSE );
	assert ( jeExtBox_IsValid (B2) != JE_FALSE );

	rslt = jeExtBox_Intersects (B1, B2);
	if ( (rslt != JE_FALSE) && (Result != NULL))
		{
			jeExtBox_Set ( Result,
						MAX (B1->Min.X, B2->Min.X),
						MAX (B1->Min.Y, B2->Min.Y),
						MAX (B1->Min.Z, B2->Min.Z),
						MIN (B1->Max.X, B2->Max.X),
						MIN (B1->Max.Y, B2->Max.Y),
						MIN (B1->Max.Z, B2->Max.Z) );
		}
	return rslt;
}

JETAPI void JETCC jeExtBox_Union( const jeExtBox *B1, const jeExtBox *B2, jeExtBox *Result )
{
	assert ( jeExtBox_IsValid (B1) != JE_FALSE );
	assert ( jeExtBox_IsValid (B2) != JE_FALSE );
	assert (Result != NULL);

	jeExtBox_Set (	Result,
				MIN (B1->Min.X, B2->Min.X),
				MIN (B1->Min.Y, B2->Min.Y),
				MIN (B1->Min.Z, B2->Min.Z),
				MAX (B1->Max.X, B2->Max.X),
				MAX (B1->Max.Y, B2->Max.Y),
				MAX (B1->Max.Z, B2->Max.Z) );
}

JETAPI jeBoolean JETCC jeExtBox_ContainsPoint(  const jeExtBox *B,  const jeVec3d *Point )
{
	assert (jeExtBox_IsValid (B) != JE_FALSE);
	assert( jeVec3d_IsValid(Point) != JE_FALSE );

	if (    (Point->X >= B->Min.X) && (Point->X <= B->Max.X) &&
			(Point->Y >= B->Min.Y) && (Point->Y <= B->Max.Y) &&
			(Point->Z >= B->Min.Z) && (Point->Z <= B->Max.Z)     )
		{
			return JE_TRUE;
		}
	else
		{
			return JE_FALSE;
		}
}


JETAPI void JETCC jeExtBox_GetTranslation( const jeExtBox *B, jeVec3d *pCenter )
{
	assert (jeExtBox_IsValid (B) != JE_FALSE);
	assert (pCenter != NULL);

	jeVec3d_Set( pCenter,
				(B->Min.X + B->Max.X)/2.0f,
				(B->Min.Y + B->Max.Y)/2.0f,
				(B->Min.Z + B->Max.Z)/2.0f );
}

JETAPI void JETCC jeExtBox_Translate(  jeExtBox *B,  jeFloat DX,  jeFloat DY,  jeFloat DZ	)
{
	jeVec3d VecDelta;

	assert (jeExtBox_IsValid (B) != JE_FALSE);

	jeVec3d_Set (&VecDelta, DX, DY, DZ);
		assert( jeVec3d_IsValid(&VecDelta) != JE_FALSE );
	jeVec3d_Add (&B->Min, &VecDelta, &B->Min);
	jeVec3d_Add (&B->Max, &VecDelta, &B->Max);
}

JETAPI void JETCC jeExtBox_SetTranslation( jeExtBox *B, const jeVec3d *pCenter )
{
	jeVec3d Center,Translation;

	assert (jeExtBox_IsValid (B) != JE_FALSE);
	assert (pCenter != NULL);
	assert( jeVec3d_IsValid(pCenter) != JE_FALSE );

	jeExtBox_GetTranslation( B, &Center );
	jeVec3d_Subtract( pCenter, &Center, &Translation);

	jeExtBox_Translate( B, Translation.X, Translation.Y, Translation.Z );
}

// Icestorm Begin
JETAPI void JETCC jeExtBox_SetNewOrigin( jeExtBox *B, const jeVec3d *pOrigin)
{
	assert (jeExtBox_IsValid (B) != JE_FALSE);
	assert (pOrigin != NULL);
	assert( jeVec3d_IsValid(pOrigin) != JE_FALSE );
	jeVec3d_Subtract(&B->Min, pOrigin, &B->Min);
	jeVec3d_Subtract(&B->Max, pOrigin, &B->Max);
}

JETAPI void JETCC jeExtBox_MoveToOrigin( jeExtBox *B, jeVec3d *OldCenter )
{
	jeFloat DX,DY,DZ;
	assert (jeExtBox_IsValid (B) != JE_FALSE);

	DX=(B->Min.X+B->Max.X)*0.5f;
	B->Min.X-=DX;B->Max.X-=DX;

	DY=(B->Min.Y+B->Max.Y)*0.5f;
	B->Min.Y-=DY;B->Max.Y-=DY;

	DZ=(B->Min.Z+B->Max.Z)*0.5f;
	B->Min.Z-=DZ;B->Max.Z-=DZ;
	if (OldCenter)
		jeVec3d_Set(OldCenter, DX, DY, DZ);
}

JETAPI void JETCC jeExtBox_TranslateAndMoveToOrigin( jeExtBox *B, const jeVec3d *vMove, jeVec3d *MovedCenter )
{
	jeFloat DX,DY,DZ;
	assert (jeExtBox_IsValid (B) != JE_FALSE);

	DX=(B->Min.X+B->Max.X)*0.5f;
	B->Min.X-=DX;B->Max.X-=DX;

	DY=(B->Min.Y+B->Max.Y)*0.5f;
	B->Min.Y-=DY;B->Max.Y-=DY;

	DZ=(B->Min.Z+B->Max.Z)*0.5f;
	B->Min.Z-=DZ;B->Max.Z-=DZ;
	if (MovedCenter)
	{ 
		MovedCenter->X=DX+vMove->X;
		MovedCenter->Y=DY+vMove->Y;
		MovedCenter->Z=DZ+vMove->Z;
	}
}
// Icestorm End

JETAPI void JETCC jeExtBox_GetScaling( const jeExtBox *B, jeVec3d *pScale )
{
	assert (jeExtBox_IsValid (B) != JE_FALSE );
	assert (pScale != NULL);

	jeVec3d_Subtract( &(B->Max), &(B->Min), pScale );
}

JETAPI void JETCC jeExtBox_Scale( jeExtBox *B, jeFloat ScaleX, jeFloat ScaleY, jeFloat ScaleZ )
{
	jeVec3d Center;
	jeVec3d Scale;
	jeFloat DX,DY,DZ;

	assert (jeExtBox_IsValid (B) != JE_FALSE );
	assert (ScaleX >= 0.0f );
	assert (ScaleY >= 0.0f );
	assert (ScaleZ >= 0.0f );
	assert (ScaleX * ScaleX >= 0.0f );		// check for NANS
	assert (ScaleY * ScaleY >= 0.0f );
	assert (ScaleZ * ScaleZ >= 0.0f );

	jeExtBox_GetTranslation( B, &Center );
	jeExtBox_GetScaling    ( B, &Scale  );
	
	DX = ScaleX * Scale.X * 0.5f;
	DY = ScaleY * Scale.Y * 0.5f;
	DZ = ScaleZ * Scale.Z * 0.5f;

	B->Min.X = Center.X - DX;
	B->Min.Y = Center.Y - DY;
	B->Min.Z = Center.Z - DZ;
	
	B->Max.X = Center.X + DX;
	B->Max.Y = Center.Y + DY;
	B->Max.Z = Center.Z + DZ;
	
	assert (jeExtBox_IsValid (B) != JE_FALSE);
}

JETAPI void JETCC jeExtBox_SetScaling( jeExtBox *B, const jeVec3d *pScale )
{
	jeVec3d Center;
	jeFloat DX,DY,DZ;

	assert (jeExtBox_IsValid (B) != JE_FALSE );
	assert (pScale != NULL );
	assert (jeVec3d_IsValid( pScale )!= JE_FALSE);
	assert (pScale->X >= 0.0f );
	assert (pScale->Y >= 0.0f );
	assert (pScale->Z >= 0.0f );

	jeExtBox_GetTranslation( B, &Center );

	DX = pScale->X / 2.0f;
	DY = pScale->Y / 2.0f;
	DZ = pScale->Z / 2.0f;

	B->Min.X = Center.X - DX;
	B->Min.Y = Center.Y - DY;
	B->Min.Z = Center.Z - DZ;
	
	B->Max.X = Center.X + DX;
	B->Max.Y = Center.Y + DY;
	B->Max.Z = Center.Z + DZ;
}

JETAPI void JETCC jeExtBox_LinearSweep(	const jeExtBox *BoxToSweep, 
						const jeVec3d *StartPoint, 
						const jeVec3d *EndPoint, 
						jeExtBox *EnclosingBox )
{

	assert (jeExtBox_IsValid (BoxToSweep) != JE_FALSE );
	assert (StartPoint   != NULL );
	assert (EndPoint     != NULL );
	assert (jeVec3d_IsValid( StartPoint )!= JE_FALSE);
	assert (jeVec3d_IsValid( EndPoint   )!= JE_FALSE);
	assert (EnclosingBox != NULL );

	*EnclosingBox = *BoxToSweep;

	if (EndPoint->X > StartPoint->X)
		{
			EnclosingBox->Min.X += StartPoint->X; 
			EnclosingBox->Max.X += EndPoint->X; 
		}
	else
		{
			EnclosingBox->Min.X += EndPoint->X; 
			EnclosingBox->Max.X += StartPoint->X; 
		}

	if (EndPoint->Y > StartPoint->Y)
		{
			EnclosingBox->Min.Y += StartPoint->Y; 
			EnclosingBox->Max.Y += EndPoint->Y; 
		}
	else
		{
			EnclosingBox->Min.Y += EndPoint->Y; 
			EnclosingBox->Max.Y += StartPoint->Y; 
		}

	if (EndPoint->Z > StartPoint->Z)
		{
			EnclosingBox->Min.Z += StartPoint->Z; 
			EnclosingBox->Max.Z += EndPoint->Z; 
		}
	else
		{
			EnclosingBox->Min.Z += EndPoint->Z; 
			EnclosingBox->Max.Z += StartPoint->Z; 
		}
	assert (jeExtBox_IsValid (EnclosingBox) != JE_FALSE );
}

static jeBoolean JETCC jeExtBox_XFaceDist(  const jeVec3d *Start, 
												const jeVec3d *Delta, const jeExtBox *B, jeFloat *T, jeFloat X)
{
	jeFloat t;
	jeFloat Y,Z;
	assert( Start != NULL );
	assert( Delta != NULL );
	assert( B     != NULL );
	assert( T     != NULL );

	//if ( (Start->X <= X) && (X <= Delta->X + Start->X) )
		{
			t = (X - Start->X)/Delta->X;
			Y  = Start->Y + Delta->Y * t;
			if ( ( B->Min.Y <= Y) && (Y <= B->Max.Y) )
				{
					Z = Start->Z + Delta->Z * t;
					if ( ( B->Min.Z <= Z) && (Z <= B->Max.Z) )
						{
							*T = t;
							return JE_TRUE;
						}
				}
		}
	return JE_FALSE;
}

static jeBoolean JETCC jeExtBox_YFaceDist(  const jeVec3d *Start, const jeVec3d *Delta, const jeExtBox *B, jeFloat *T, jeFloat Y)
{
	jeFloat t;
	jeFloat X,Z;
	assert( Start != NULL );
	assert( Delta != NULL );
	assert( B     != NULL );
	assert( T     != NULL );

	//if ( (Start->Y <= Y) && (Y <= Delta->Y + Start->Y) )
		{
			t = (Y - Start->Y)/Delta->Y;
			Z  = Start->Z + Delta->Z * t;
			if ( ( B->Min.Z <= Z) && (Z <= B->Max.Z) )
				{
					X = Start->X + Delta->X * t;
					if ( ( B->Min.X <= X) && (X <= B->Max.X) )
						{
							*T = t;
							return JE_TRUE;
						}
				}
		}
	return JE_FALSE;
}


static jeBoolean JETCC jeExtBox_ZFaceDist(  const jeVec3d *Start, const jeVec3d *Delta, const jeExtBox *B, jeFloat *T, jeFloat Z)
{
	jeFloat t;
	jeFloat X,Y;
	assert( Start != NULL );
	assert( Delta != NULL );
	assert( B     != NULL );
	assert( T     != NULL );

	//if ( (Start->Z <= Z) && (Z <= Delta->Z + Start->Z) )
		{
			t = (Z - Start->Z)/Delta->Z;
			X  = Start->X + Delta->X * t;
			if ( ( B->Min.X <= X) && (X <= B->Max.X) )
				{
					Y = Start->Y + Delta->Y * t;
					if ( ( B->Min.Y <= Y) && (Y <= B->Max.Y) )
						{
							*T = t;
							return JE_TRUE;
						}
				}
		}
	return JE_FALSE;
}



JETAPI jeBoolean JETCC jeExtBox_RayCollision( const jeExtBox *B, const jeVec3d *Start, const jeVec3d *End, 
								jeFloat *T, jeVec3d *Normal )
{
	// only detects rays going 'in' to the box
	jeFloat t;
	jeVec3d Delta;
	jeVec3d LocalNormal;
	jeFloat LocalT;

	assert( B != NULL );
	assert( Start != NULL );
	assert( End != NULL );
	assert (jeVec3d_IsValid( Start )!= JE_FALSE);
	assert (jeVec3d_IsValid( End   )!= JE_FALSE);
	assert (jeExtBox_IsValid( B )!= JE_FALSE );

	jeVec3d_Subtract(End,Start,&Delta);
	
	if (Normal == NULL)
		Normal = &LocalNormal;
	if (T == NULL)
		T = &LocalT;
	
	// test x end of box, facing away from ray direction.
	if (Delta.X > 0.0f)
		{
			if ( (Start->X <= B->Min.X) && (B->Min.X <= End->X) &&
				 (jeExtBox_XFaceDist(  Start ,&Delta, B, &t, B->Min.X ) != JE_FALSE) )
					{
						jeVec3d_Set( Normal,  -1.0f, 0.0f, 0.0f );
						*T = t;
						return JE_TRUE;
					}
		}
	else if (Delta.X < 0.0f)
		{
			if ( (End->X <= B->Max.X) && (B->Max.X <= Start->X) &&
				 (jeExtBox_XFaceDist(  Start ,&Delta, B, &t, B->Max.X ) != JE_FALSE) )
					{
						jeVec3d_Set( Normal,  1.0f, 0.0f, 0.0f );
						*T = t;
						return JE_TRUE;
					}
		}
	
	// test y end of box, facing away from ray direction.
	if (Delta.Y > 0.0f)
		{	
			if ( (Start->Y <= B->Min.Y) && (B->Min.Y <= End->Y) &&
				 (jeExtBox_YFaceDist(  Start ,&Delta, B, &t, B->Min.Y ) != JE_FALSE) )
				{
					jeVec3d_Set( Normal,  0.0f, -1.0f, 0.0f );
					*T = t;
					return JE_TRUE;
				}
		}
	else if (Delta.Y < 0.0f)
		{
			if ( (End->Y <= B->Max.Y) && (B->Max.Y <= Start->Y) &&
				 (jeExtBox_YFaceDist(  Start ,&Delta, B, &t, B->Max.Y ) != JE_FALSE) )
				{
					jeVec3d_Set( Normal,  0.0f, 1.0f, 0.0f );
					*T = t;
					return JE_TRUE;
				}
		}
	
	// test z end of box, facing away from ray direction.
	if (Delta.Z > 0.0f)
		{	
			if ( (Start->Z <= B->Min.Z) && (B->Min.Z <= End->Z) &&
			     (jeExtBox_ZFaceDist(  Start ,&Delta, B, &t, B->Min.Z ) != JE_FALSE) )
				{
					jeVec3d_Set( Normal,  0.0f, 0.0f, -1.0f );
					*T = t;
					return JE_TRUE;
				}
		}
	else if (Delta.Z < 0.0f)
		{			
			if ( (End->Z <= B->Max.Z) && (B->Max.Z <= Start->Z) &&
				 (jeExtBox_ZFaceDist(  Start ,&Delta, B, &t, B->Max.Z ) != JE_FALSE) )
				{
					jeVec3d_Set( Normal,  0.0f, 0.0f, 1.0f );
					*T = t;
					return JE_TRUE;
				}
		}
	return JE_FALSE;	
}

JETAPI void JETCC jeExtBox_GetPoint( const jeExtBox *B, const int iPoint, jeVec3d *vPoint)
{
	assert(vPoint != NULL);

	switch(iPoint)
	{
	case 0:
		jeVec3d_Set(vPoint, B->Min.X, B->Min.Y, B->Min.Z);
		break;
	case 1:
		jeVec3d_Set(vPoint, B->Max.X, B->Min.Y, B->Min.Z);
		break;
	case 2:
		jeVec3d_Set(vPoint, B->Min.X, B->Max.Y, B->Min.Z);
		break;
	case 3:
		jeVec3d_Set(vPoint, B->Max.X, B->Max.Y, B->Min.Z);
		break;
	case 4:
		jeVec3d_Set(vPoint, B->Min.X, B->Min.Y, B->Max.Z);
		break;
	case 5:
		jeVec3d_Set(vPoint, B->Max.X, B->Min.Y, B->Max.Z);
		break;
	case 6:
		jeVec3d_Set(vPoint, B->Min.X, B->Max.Y, B->Max.Z);
		break;
	case 7:
		jeVec3d_Set(vPoint, B->Max.X, B->Max.Y, B->Max.Z);
		break;
	}
}

// Added by Icestorm:
// __inline functions & defines for an
// fast collision routine ;)
// NOTE:	Added EPSILON (slip through edge/corner bug)
//			Added more asm code (prevents unnecessary fdiv)
//			Flipped normals into right direction ;)
//			Fixed FPU-stack-overflow-bug
static  jeFloat			JE_EXTBOX_FC_EPSILON = 0.00001f;
#define FSIZE			4
#define VSIZE			4*FSIZE
#define _X				+0*FSIZE]
#define _Y				+1*FSIZE]
#define _Z				+2*FSIZE]
#define _MIN			+0*VSIZE
#define _MAX			+1*VSIZE
#define _VPATH		dword ptr [esi
#define _VPATH2		dword ptr [ecx
#define _B			dword ptr [edi
#define _XMOVINGBOX dword ptr [ebx

#define _ASM_TEST_A(_P)																   \
__asm	fld   _VPATH _P				/* |   vPath->_P								*/ \
__asm	fmul  t						/* | t*vPath->_P								*/ \
__asm	fld   _B _MAX _P 			/* | t*vPath->_P | B->Max._P					*/ \
__asm	fsub  _XMOVINGBOX _MIN _P  	/* | t*vPath->_P | B->Max._P-xMovingBox->Min._P */ \
__asm   fadd  JE_EXTBOX_FC_EPSILON  /*   IMPROTANT!!								*/ \
__asm	fcomp						/* | t*vPath->_P								*/ \
__asm	fnstsw ax					/*												*/ \
__asm	test  ah,1h					/*												*/ \
__asm	je   TestCollision##_P		/* !(xMovingBox->Min._P+t*vPath->_P<=B->Max._P) */ \
__asm	fcomp st					/*	Clean up stack								*/ \
__asm	jmp  short NoCollision		/*												*/

#define _ASM_TEST_B(_P)																   \
__asm	fld   _B _MIN _P			/* | t*vPath->_P | B->Min._P					*/ \
__asm	fsub  _XMOVINGBOX _MAX _P	/* | t*vPath->_P | B->Min._P-xMovingBox->Max._P */ \
__asm   fsub  JE_EXTBOX_FC_EPSILON  /*   IMPROTANT!!								*/ \
__asm	fcompp						/*												*/ \
__asm	fnstsw ax					/*												*/ \
__asm	test  ah,41h				/*												*/ \
__asm	je    NoCollision			/* !(B->Min._P<=t*vPath->_P+xMovingBox->Max._P) */

static __inline jeBoolean jeExtBox_asmCollisionTestX_(jeFloat t, jeVec3d *vPath, const jeExtBox *B, jeExtBox *xMovingBox)
{
#ifndef JET64
	__asm
	{
		mov   esi,vPath
		mov   edi,B
		mov   ebx,xMovingBox
		_ASM_TEST_A(_Y)
TestCollision_Y:
		_ASM_TEST_B(_Y)
		_ASM_TEST_A(_Z)		
TestCollision_Z:
		_ASM_TEST_B(_Z)
		mov   eax,1
		jmp short End
NoCollision:
		xor   eax,eax
End:
	}
#else
	return JE_FALSE;
#endif
}

static __inline jeBoolean jeExtBox_asmCollisionTestY_(jeFloat t, jeVec3d *vPath, const jeExtBox *B, jeExtBox *xMovingBox)
{
#ifndef JET64
	_asm
	{
		mov   esi,vPath
		mov   edi,B
		mov   ebx,xMovingBox
		_ASM_TEST_A(_X)
TestCollision_X:
		_ASM_TEST_B(_X)
		_ASM_TEST_A(_Z)		
TestCollision_Z:
		_ASM_TEST_B(_Z)		
		mov   eax,1
		jmp short End
NoCollision:
		xor   eax,eax
End:
	}
#else
	return JE_FALSE;
#endif
}

static __inline jeBoolean jeExtBox_asmCollisionTestZ_(jeFloat t, jeVec3d *vPath, const jeExtBox *B, jeExtBox *xMovingBox)
{
#ifndef JET64
	_asm
	{
		mov   esi,vPath
		mov   edi,B
		mov   ebx,xMovingBox
		_ASM_TEST_A(_X)
TestCollision_X:
		_ASM_TEST_B(_X)
		_ASM_TEST_A(_Y)		
TestCollision_Y:
		_ASM_TEST_B(_Y)
		mov   eax,1
		jmp short End
NoCollision:
		xor   eax,eax
End:
	}
#else
	return JE_FALSE;
#endif
}

// Added by Icestorm (fast, rewritten version of Incarnadine's one)
// (ca. 7-12 times faster)
// ----------------------------------------
// Collides a moving box (or ray) against a stationary box.  The moving box
// must be relative to the path and move from Start to End.
//   Only returns a ray/box hitting the outside of the box.  
//     on success, JE_TRUE is returned, and 
//       if T is non-NULL, T is returned as 0..1 where 0 is a collision at Start, and 1 is a collision at End
//       if Normal is non-NULL, Normal is the surface normal of the box where the collision occured.
JETAPI jeBoolean JETCC jeExtBox_Collision(	const jeExtBox *B, const jeExtBox *MovingBox,
											const jeVec3d *Start, const jeVec3d *End, 
											jeFloat *T, jeVec3d *Normal )
{
#ifndef JET64
	jeFloat t;
	jeExtBox xSweepBox,xMovingBox,*xMovingBoxPtr=&xMovingBox;
	jeVec3d vPath,*vPathPtr=&vPath;
	jeBoolean TestB;

	assert(B != NULL);

	// If there's no moving box, do a ray collision
	if(MovingBox == NULL)
		return jeExtBox_RayCollision(B,Start,End,T,Normal);

	// If the boxes already overlap, we have to report no collision
	// to be consistent with the rest of the engine collision calls.
	xMovingBox = *MovingBox;  // Used later as well.
	jeExtBox_Translate(&xMovingBox, Start->X, Start->Y, Start->Z);
	if(jeExtBox_Intersection(B, &xMovingBox, NULL)) return JE_FALSE;	

	// Verify the sweepbox intersects this box
	jeExtBox_LinearSweep(MovingBox, Start, End, &xSweepBox);
	if(!jeExtBox_Intersection(B, &xSweepBox, NULL)) return JE_FALSE;

	jeVec3d_Subtract(End,Start,&vPath);

	// CollisionTest X-Front
	if (vPath.X<0.0f)
	{
		//t=(B->Max.X-xMovingBox.Min.X)/vPath.X;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xMovingBoxPtr
			fld   _B _MAX _X
			fsub  _XMOVINGBOX _MIN _X
			fldz
			fcom
			fnstsw ax
			test  ah,1h
			jne    NoCollisionX1
			fadd   _VPATH _X
			fcom
			fnstsw ax
			test  ah,41h
			je   NoCollisionX1
			fdivp st(1),st
			fstp  t
			jmp   short CTEndX1
NoCollisionX1:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndX1:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xMovingBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xMovingBox.Max,&vPath,t,&vPoint2);
			//if (vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)		
			if (jeExtBox_asmCollisionTestX_(t, &vPath, B, &xMovingBox) )
			{ 
				if (Normal) jeVec3d_Set(Normal,1.0f,0.0f,0.0f);
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	} 
	// CollisionTest X-Back
	else if (vPath.X>0.0f)
	{
		//t=(B->Min.X-xMovingBox.Max.X)/vPath.X;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xMovingBoxPtr
			fld   _B _MIN _X
			fsub  _XMOVINGBOX _MAX _X
			fldz
			fcom
			fnstsw ax
			test  ah,41h
			je    NoCollisionX2
			fadd  _VPATH _X
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionX2
			fdivp st(1),st
			fstp  t
			jmp   short CTEndX2
NoCollisionX2:
			fcompp
			mov   TestB,0
CTEndX2:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath,t,&vPoint2);
			//if (vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)
			if (jeExtBox_asmCollisionTestX_(t, &vPath, B, &xMovingBox) )
			{
				if (Normal) jeVec3d_Set(Normal,-1.0f,0.0f,0.0f);
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Y-Front
	if (vPath.Y<0.0f)
	{
		//t=(B->Max.Y-xMovingBox.Min.Y)/vPath.Y;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xMovingBoxPtr
			fld   _B _MAX _Y
			fsub  _XMOVINGBOX _MIN _Y
			fldz
			fcom
			fnstsw ax
			test  ah,1h
			jne    NoCollisionY1
			fadd  _VPATH _Y
			fcom
			fnstsw ax
			test  ah,41h
			je   NoCollisionY1
			fdivp st(1),st
			fstp  t
			jmp   short CTEndY1
NoCollisionY1:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndY1:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,bT,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath,bT,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)
			if (jeExtBox_asmCollisionTestY_(t, &vPath, B, &xMovingBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,+1.0f,0.0f);
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Y-Back
	else if (vPath.Y>0.0f)
	{
		//t=(B->Min.Y-xMovingBox.Max.Y)/vPath.Y;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xMovingBoxPtr
			fld   _B _MIN _Y
			fsub  _XMOVINGBOX _MAX _Y
			fldz
			fcom
			fnstsw ax
			test  ah,41h
			je    NoCollisionY2
			fadd   _VPATH _Y
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionY2
			fdivp st(1),st
			fstp  t
			jmp   short CTEndY2
NoCollisionY2:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndY2:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath,t,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)
			if (jeExtBox_asmCollisionTestY_(t, &vPath, B, &xMovingBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,-1.0f,0.0f);
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Z-Front
	if (vPath.Z<0.0f)
	{
		//t=(B->Max.Z-xMovingBox.Min.Z)/vPath.Z;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xMovingBoxPtr
			fld   _B _MAX _Z
			fsub  _XMOVINGBOX _MIN _Z
			fldz
			fcom
			fnstsw ax
			test  ah,1h
			jne    NoCollisionZ1
			fadd   _VPATH _Z
			fcom
			fnstsw ax
			test  ah,41h
			je   NoCollisionZ1
			fdivp st(1),st
			fstp  t
			jmp   short CTEndZ1
NoCollisionZ1:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndZ1:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath,t,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y)
			if (jeExtBox_asmCollisionTestZ_(t, &vPath, B, &xMovingBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,0.0f,+1.0f);
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Z-Back
	else if (vPath.Z>0.0f)
	{
		//t=(B->Min.Z-xMovingBox.Max.Z)/vPath.Z;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xMovingBoxPtr
			fld   _B _MIN _Z
			fsub  _XMOVINGBOX _MAX _Z
			fldz
			fcom
			fnstsw ax
			test  ah,41h
			je    NoCollisionZ2
			fadd  _VPATH _Z
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionZ2
			fdivp st(1),st
			fstp  t
			jmp   short CTEndZ2
NoCollisionZ2:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndZ2:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath,t,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y)
			if (jeExtBox_asmCollisionTestZ_(t, &vPath, B, &xMovingBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,0.0f,-1.0f);
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	}
#endif

	return JE_FALSE;
}

#ifndef JET64
#define _ASM_TEST2_(_P)																   \
__asm	fld   _VPATH _P				/* |   vPath->_P								*/ \
__asm	fmul  t						/* | t*vPath->_P								*/ \
__asm	fld   _B _MAX _P 			/* | t*vPath->_P | B->Max._P					*/ \
__asm	fsub  _XMOVINGBOX _MIN _P  	/* | t*vPath->_P | B->Max._P-xMovingBox->Min._P */ \
__asm   fadd  JE_EXTBOX_FC_EPSILON  /*   IMPROTANT!!								*/ \
__asm	fcompp						/*												*/ \
__asm	fnstsw ax					/*												*/ \
__asm	test  ah,1h					/*												*/ \
__asm	jne   NoCollision			/* !(xMovingBox->Min._P+t*vPath->_P<=B->Max._P) */ \
									/*												*/ \
__asm	fld   _VPATH2 _P			/* |   vPath2->_P								*/ \
__asm	fmul  t						/* | t*vPath2->_P								*/ \
__asm	fld   _B _MIN _P			/* | t*vPath2->_P | B->Min._P					*/ \
__asm	fsub  _XMOVINGBOX _MAX _P	/* | t*vPath2->_P | B->Min._P-xMovingBox->Max._P*/ \
__asm   fsub  JE_EXTBOX_FC_EPSILON  /*   IMPROTANT!!								*/ \
__asm	fcompp						/*												*/ \
__asm	fnstsw ax					/*												*/ \
__asm	test  ah,41h				/*												*/ \
__asm	je    NoCollision			/* !(B->Min._P<=t*vPath2->_P+xMovingBox->Max._P)*/
#endif
#ifndef JET64
static __inline jeBoolean jeExtBox_asmCollisionTestX2_(jeFloat t, jeVec3d *vPath, jeVec3d *vPath2,
													   const jeExtBox *B, jeExtBox *xMovingBox)
{
	__asm
	{
		mov   esi,vPath
		mov   edi,B
		mov   ebx,xMovingBox
		mov   ecx,vPath2
		_ASM_TEST2_(_Y)
		_ASM_TEST2_(_Z)
		mov   eax,1
		jmp short End
NoCollision:
		xor   eax,eax
End:
	}
}

static __inline jeBoolean jeExtBox_asmCollisionTestY2_(jeFloat t, jeVec3d *vPath, jeVec3d *vPath2,
													   const jeExtBox *B, jeExtBox *xMovingBox)
{
	_asm
	{
		mov   esi,vPath
		mov   edi,B
		mov   ebx,xMovingBox
		mov	  ecx,vPath2
		_ASM_TEST2_(_X)
		_ASM_TEST2_(_Z)		
		mov   eax,1
		jmp short End
NoCollision:
		xor   eax,eax
End:
	}
}

static __inline jeBoolean jeExtBox_asmCollisionTestZ2_(jeFloat t, jeVec3d *vPath, jeVec3d *vPath2,
													   const jeExtBox *B, jeExtBox *xMovingBox)
{
	_asm
	{
		mov   esi,vPath
		mov   edi,B
		mov   ebx,xMovingBox
		mov   ecx,vPath2
		_ASM_TEST2_(_X)
		_ASM_TEST2_(_Y)		
		mov   eax,1
		jmp short End
NoCollision:
		xor   eax,eax
End:
	}
}
#endif

// Added by Icestorm
// ----------------------------------------
// Collides a changing box against a stationary box.  The changing box
// must be relative to Pos.
//   Only returns a box hitting the outside of the box.  
//     on success, JE_TRUE is returned, and 
//       if T is non-NULL, T is returned as 0..1 where 0 is a collision at Start, and 1 is a collision at End
//       if Normal is non-NULL, Normal is the surfacenormal of the box where the collision occured.
//       if Point is non-NULL, Point is a point of the surface where the collision occured.
JETAPI jeBoolean JETCC jeExtBox_ChangeBoxCollision(	const jeExtBox *B, const jeVec3d *Pos,
													const jeExtBox *StartBox, const jeExtBox *EndBox,
													jeFloat *T, jeVec3d *Normal, jeVec3d *Point )
{
#ifndef JET64
	jeFloat t;
	jeExtBox xStartBox,xChangeBox,*xStartBoxPtr=&xStartBox;
	jeVec3d vPath,vPath2,*vPathPtr=&vPath,*vPathPtr2=&vPath2;
	jeBoolean TestB;

	assert(B != NULL);
	assert(StartBox != NULL);
	assert(EndBox != NULL);

	// If the boxes already overlap, we have to report no collision
	// to be consistent with the rest of the engine collision calls.
	xStartBox = *StartBox;  // Used later as well.
	jeExtBox_Translate(&xStartBox, Pos->X, Pos->Y, Pos->Z);
	if(jeExtBox_Intersection(B, &xStartBox, NULL)) return JE_FALSE;	

	// Verify the sweepbox intersects this box
	jeExtBox_Union(StartBox, EndBox, &xChangeBox);
	if(!jeExtBox_Intersection(B, &xChangeBox, NULL)) return JE_FALSE;

	jeVec3d_Subtract(&(EndBox->Min),&(StartBox->Min),&vPath);
	jeVec3d_Subtract(&(EndBox->Max),&(StartBox->Max),&vPath2);

	// CollisionTest X-Front
	if (vPath.X<0.0f)
	{
		//t=(B->Max.X-xStartBox.Min.X)/vPath.X;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xStartBoxPtr
			fld   _B _MAX _X
			fsub  _XMOVINGBOX _MIN _X
			fldz
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionX1
			fadd  _VPATH _X
			fcom
			fnstsw ax
			test  ah,41h
			je   NoCollisionX1
			fdivp st(1),st
			fstp  t
			jmp   short CTEndX1
NoCollisionX1:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndX1:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath2,t,&vPoint2);
			//if (vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)		
			if (jeExtBox_asmCollisionTestX2_(t, &vPath, &vPath2, B, &xStartBox) )
			{ 
				if (Normal) jeVec3d_Set(Normal,1.0f,0.0f,0.0f);
				if (Point) *Point=B->Max;
				if (T) *T=t;
				return JE_TRUE;
			}
		}
	} 
	// CollisionTest X-Back
	if (vPath2.X>0.0f)
	{
		//t=(B->Min.X-xStartBox.Max.X)/vPath2.X;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr2
			mov   ebx,xStartBoxPtr
			fld   _B _MIN _X
			fsub  _XMOVINGBOX _MAX _X
			fldz
			fcom
			fnstsw ax
			test  ah,41h
			je    NoCollisionX2
			fadd  _VPATH _X
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionX2
			fdivp st(1),st
			fstp  t
			jmp   short CTEndX2
NoCollisionX2:
			fcompp
			mov   TestB,0
CTEndX2:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath2,t,&vPoint2);
			//if (vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)
			if (jeExtBox_asmCollisionTestX2_(t, &vPath, &vPath2, B, &xStartBox) )
			{
				if (Normal) jeVec3d_Set(Normal,-1.0f,0.0f,0.0f);
				if (T) *T=t;
				if (Point) *Point=B->Min;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Y-Front
	if (vPath.Y<0.0f)
	{
		//t=(B->Max.Y-xStartBox.Min.Y)/vPath.Y;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xStartBoxPtr
			fld   _B _MAX _Y
			fsub  _XMOVINGBOX _MIN _Y
			fldz
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionY1
			fadd  _VPATH _Y
			fcom
			fnstsw ax
			test  ah,41h
			je   NoCollisionY1
			fdivp st(1),st
			fstp  t
			jmp   short CTEndY1
NoCollisionY1:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndY1:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,bT,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath2,bT,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)
			if (jeExtBox_asmCollisionTestY2_(t, &vPath, &vPath2, B, &xStartBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,+1.0f,0.0f);
				if (T) *T=t;
				if (Point) *Point=B->Max;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Y-Back
	if (vPath2.Y>0.0f)
	{
		//t=(B->Min.Y-xStartBox.Max.Y)/vPath2.Y;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr2
			mov   ebx,xStartBoxPtr
			fld   _B _MIN _Y
			fsub  _XMOVINGBOX _MAX _Y
			fldz
			fcom
			fnstsw ax
			test  ah,41h
			je    NoCollisionY2
			fadd  _VPATH _Y
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionY2
			fdivp st(1),st
			fstp  t
			jmp   short CTEndY2
NoCollisionY2:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndY2:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath2,t,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Z<=B->Max.Z && B->Min.Z<=vPoint2.Z)
			if (jeExtBox_asmCollisionTestY2_(t, &vPath, &vPath2, B, &xStartBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,-1.0f,0.0f);
				if (T) *T=t;
				if (Point) *Point=B->Min;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Z-Front
	if (vPath.Z<0.0f)
	{
		//t=(B->Max.Z-xStartBox.Min.Z)/vPath.Z;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr
			mov   ebx,xStartBoxPtr
			fld   _B _MAX _Z
			fsub  _XMOVINGBOX _MIN _Z
			fldz
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionZ1
			fadd  _VPATH _Z
			fcom
			fnstsw ax
			test  ah,41h
			je   NoCollisionZ1
			fdivp st(1),st
			fstp  t
			jmp   short CTEndZ1
NoCollisionZ1:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndZ1:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath2,t,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y)
			if (jeExtBox_asmCollisionTestZ2_(t, &vPath, &vPath2, B, &xStartBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,0.0f,+1.0f);
				if (T) *T=t;
				if (Point) *Point=B->Max;
				return JE_TRUE;
			}
		}
	}
	// CollisionTest Z-Back
	if (vPath2.Z>0.0f)
	{
		//t=(B->Min.Z-xStartBox.Max.Z)/vPath2.Z;
		//if (t>=0 && t<=1)
		TestB=JE_TRUE;
		__asm 
		{
			mov   edi,B
			mov   esi,vPathPtr2
			mov   ebx,xStartBoxPtr
			fld   _B _MIN _Z
			fsub  _XMOVINGBOX _MAX _Z
			fldz
			fcom
			fnstsw ax
			test  ah,41h
			je    NoCollisionZ2
			fadd  _VPATH _Z
			fcom
			fnstsw ax
			test  ah,1h
			jne   NoCollisionZ2
			fdivp st(1),st
			fstp  t
			jmp   short CTEndZ2
NoCollisionZ2:
			fcompp
			xor   eax,eax
			mov   TestB,0
CTEndZ2:
		}
		if(TestB)
		{
			//jeVec3d_AddScaled(&xStartBox.Min,&vPath,t,&vPoint);
			//jeVec3d_AddScaled(&xStartBox.Max,&vPath2,t,&vPoint2);
			//if (vPoint.X<=B->Max.X && B->Min.X<=vPoint2.X &&
			//	vPoint.Y<=B->Max.Y && B->Min.Y<=vPoint2.Y)
			if (jeExtBox_asmCollisionTestZ2_(t, &vPath, &vPath2, B, &xStartBox) )
			{
				if (Normal) jeVec3d_Set(Normal,0.0f,0.0f,-1.0f);
				if (T) *T=t;
				if (Point) *Point=B->Min;
				return JE_TRUE;
			}
		}
	}
#endif

	return JE_FALSE;
}