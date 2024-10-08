/****************************************************************************************/
/*  GSpan.cpp                                                                           */
/*                                                                                      */
/*  Author: John Pollard                                                                */
/*  Description: Front to back span code                                                */
/*                                                                                      */
/*  The contents of this file are subject to the Jet3D Public License                   */
/*  Version 1.01 (the "License"); you may not use this file except in                   */
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
#include <Windows.h>
#include <Assert.h>

#include "D3DDrv.h"
#include "d3d_GSpan.h"

#ifdef USE_SPANS //{

#pragma message ("Here be statics and globals")
/*
		SPAN		GSpan_SpanLines[MAX_SPAN_LINES];

static	SPAN_MINMAX	SMinMax[MAX_SPAN_LINES];			// Linked list of spans for each scanline...
static	SLIST		ScanHash[MAX_SPANS];				// hash table for SList

static	BOOL		PolyVisible = FALSE;

		S32			GSpan_NumWorldPixels = 0;
static	S32			NumSpans = 0;
static	S32			NumSpanPixels[MAX_SPAN_LINES];
static	S32			PolysRendered = 0;

static	S32			CurrentSList = 0;

static	const S32	CEIL_FRACT = ( ( 1 << 16)-1);
*/
static SPAN			SpanLines[MAX_SPAN_LINES];

static SPAN_MINMAX	SMinMax[MAX_SPAN_LINES];			// Linked list of spans for each scanline...
static SLIST		ScanHash[MAX_SPANS];				// hash table for SList

static BOOL			PolyVisible = FALSE;

static int32		NumWorldPixels = 0;
static int32		NumSpans = 0;
static int32		NumSpanPixels[MAX_SPAN_LINES];
static int32		PolysRendered = 0;

static int32		CurrentSList = 0;

static const int32	CEIL_FRACT = ( ( 1 << 16)-1);

void	GSpan_InitStaticsAndGlobals(void)
{
	memset(SpanLines, 0, sizeof(SpanLines));
	memset(SMinMax, 0, sizeof(SMinMax));
	memset(ScanHash, 0, sizeof(ScanHash));
	PolyVisible = FALSE;
	NumWorldPixels = 0;
	NumSpans = 0;
	memset(NumSpanPixels, 0, sizeof(NumSpanPixels));
	PolysRendered = 0;
	CurrentSList = 0;
}



void DRIVERCC EdgeOutNoUV (int32 x1, int32 y1, int32 x2, int32 y2)
{
	int32		Ctmp;
	int32		y;
	int32		x,m;

	int32		ydelta;
	int32		Dir;
	int32		Cx1, Cx2, Cy1, Cy2;
	SPAN	*pSpans;

	Cx1 = x1;
	Cx2 = x2;
	Cy1 = y1;
	Cy2 = y2;

	if (Cy2 != Cy1)								// This isn't a horizontal line 
	{								
		Dir =0;									// Left side

		if (Cy2 < Cy1)							// Make sure y2 is greater than y1
		{
			Dir =1;								// Right side

			Ctmp = Cx1;
			Cx1 = Cx2;
			Cx2 = Ctmp;

			Ctmp = Cy1;
			Cy1 = Cy2;
			Cy2 = Ctmp;

		}

		ydelta = (Cy2 - Cy1);

		x = (Cx1 << 16) + CEIL_FRACT;            // Allign on int amounts
		m = (((Cx2 - Cx1))<<16) / ydelta;        // How much to increase x each iteration

		pSpans = &SpanLines[Cy1];

		if (!Dir)
		{
			for (y = Cy1; y <= Cy2; y++, pSpans++)       
			{
				pSpans->x1 = (x>>16);
				x += m;								// Add our constant to x
			}
		}
		else
		{
			for (y = Cy1; y <= Cy2; y++, pSpans++)       
			{
				pSpans->x2 = (x>>16);
				x += m;								// Add our constant to x
			}
		}   
	} 
}

void DRIVERCC AddSpanNoUV(int32 x1, int32 x2, int32 y)
{
    int32		i, xx2;
    SLIST		*LineStart;
    SLIST		*Current;
	SPAN_MINMAX *pSList;

    assert(y >=0 && y < MAX_SPAN_LINES);
	
	if (NumSpanPixels[y] >= D3DDrv_ClientWindow.Width) 
		return;

    if (x1 > x2)									// Swap all the coordinates so x1 < x2 
    {
		i = x1; 
		x1 = x2; 
		x2 = i;
    }

    //if ( (x2 - x1) < 0) 
	//	return;										// Invalid line
    
    Current = SMinMax[y].First;
	
	LineStart = NULL;

	pSList = &SMinMax[y];

	// Check to see if there are spans
	// in the list yet...
	if (!pSList->First) 
	{       
		pSList->First = NewSList();     
        pSList->First->Last = NULL;
        pSList->First->Next = NULL;
        pSList->First->Min = x1;
        pSList->First->Max = x2;
    }
    else while (Current != NULL)
    {
		if (x1 >= Current->Min && x2 <= Current->Max)
			return;								// This line totally hidden...

		//if falls before the entire min, max
		if (LineStart == NULL) 
		{
			if (Current == pSList->First)
            if (x2 < Current->Min) 
			{
				SLIST *NewMinMax = NewSList();
                NewMinMax->Next = Current;
                NewMinMax->Last = NULL;
                Current->Last = NewMinMax;
                pSList->First = NewMinMax;
                NewMinMax->Min = x1;
                NewMinMax->Max = x2;
                goto WasNull;
            }
            // if falls in the middle (but not touching)
            if (Current->Next != NULL)
            if (x1 > Current->Max && x2 < (Current->Next)->Min) 
			{
                SLIST *NewMinMax = NewSList();
                NewMinMax->Next = Current->Next;
                NewMinMax->Last = Current;
                Current->Next->Last = NewMinMax;
                Current->Next = NewMinMax;
                NewMinMax->Min = x1;
                NewMinMax->Max = x2;
                goto WasNull;
            }
            // if it falls to the right of all spans
            if (Current->Next == NULL)
            if (x1 > Current->Max) 
			{
                SLIST *NewMinMax = NewSList();
                Current->Next = NewMinMax;
                NewMinMax->Next = NULL;
                NewMinMax->Last = Current;
                NewMinMax->Min = x1;
                NewMinMax->Max = x2;
                goto WasNull;
            }
        }
		//if we have already started crossing spans, and we find out
		// that we are in front of a span, then we can bail out...
        if (LineStart != NULL)
			if (x2 < Current->Min)
				goto WasNull;


        // We now know that we have not fallen into any empty holes.
        // We must now check to see what spans, we've crossed...

        // if split by a min/max
        if (x1 < Current->Min && x2 > Current->Max) 
		{
			xx2 = Current->Min-1;
            Current->Min = x1; 
			
			NumWorldPixels += xx2 - x1 + 1; 
			NumSpanPixels[y] += xx2 - x1 + 1;
			
			if (!PolyVisible) 
			{ 
				PolysRendered++;
				PolyVisible = 1;
			}
            
            x1 = Current->Max+1;
            Current->Max = x2;
            if (LineStart!=NULL) 
				LineStart->Max = x2;
            else
				LineStart = Current;
            goto next;
		}

		if (x1 <= Current->Max && x2 > Current->Max) 
		{
            x1 = Current->Max+1;
            Current->Max = x2;
            LineStart = Current;
            goto next;
		}
		if (x1 < Current->Min && x2 >= Current->Min) 
		{
            x2 = Current->Min-1;
            Current->Min = x1;
            if (LineStart!=NULL) 
				LineStart->Max = Current->Max;
            goto WasNull;
        }
		next:;
		Current = Current->Next;
    }
	WasNull:;

	if (!PolyVisible) 
	{ 
		PolysRendered++;
		PolyVisible = 1;
	}

	NumWorldPixels += x2 - x1 + 1;
	NumSpanPixels[y] += x2 - x1 + 1;
}	

void ResetSList(void)
{
	CurrentSList = 0;
	NumSpans = 0;
}

SLIST *NewSList(void)
{

	CurrentSList++;
	NumSpans++;

	assert(CurrentSList < MAX_SPANS);

	return &ScanHash[CurrentSList-1];

	return NULL;
}

void ResetSpans(int32 Rows)
{ 
	int32		i;

	for (i=0; i<Rows; i++) 
	{
		SMinMax[i].First = NULL;
		NumSpanPixels[i] = 0;
	}
	ResetSList();
	NumWorldPixels = 0;
}

#endif //}
