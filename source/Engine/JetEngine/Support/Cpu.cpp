/****************************************************************************************/
/*  CPU.C                                                                               */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description:                                                                        */
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
/*****

CPU info gathering module

by Ken Baird, January 1999
with a little manicure by cbloom

----------

CB : I cut the MHZ time way down and put some smart
rounding in.  We need to make sure this is accurate enough

*******/

#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include <string>

#include "Errorlog.h"
#include "Dcommon.h"
#include "Cpu.h"
#include "Log.h"
#include "ThreadLog.h"

#define MHZ_MILLIS	(50)	// 0.05 seconds seems accurate enough

/*{**** Externs *********************/

uint32	jeCPU_Features = 0;
float	jeCPU_PerformanceFreq = 1.0f;
float	jeCPU_SecondsPerClock = 1.0f;
uint32	jeCPU_MHZ = 0;

/*}{**** Functions ; related to GetInfo *********************/

//=====================================================================================
//	CPU ID Stuff
//=====================================================================================
static uint32	MaxCPUIDVal;
static char	ProcVendorString[16];
static char	ProcName[48];
static uint32	ProcType;
static uint32	ProcFamily;
static uint32	ProcModel;
static uint32	ProcStepping;

#ifndef JET64
//CPU Identification routines
static uint32	GetCPUIDEAX(uint32 funcNum)
{
	uint32	retval;

	__try
	{
		_asm
		{
			mov	eax,funcNum
			CPUID
			mov	retval,eax
		}
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
      retval = 0;
	}

	return	retval;
}

static uint32 GetCPUIDEBX(uint32 funcNum)
{
   uint32 retval;
   
   __try
   {
      _asm
      {
         mov	eax,funcNum
         CPUID
         mov	retval,ebx
      }
   }__except(EXCEPTION_EXECUTE_HANDLER)
   {
      retval = 0;
   }
   
	return retval;
}

static uint32	GetCPUIDEDX(uint32 funcNum)
{
	uint32	retval;

	__try
	{
		_asm
		{
			mov	eax,funcNum
			CPUID
			mov	retval,edx
		}
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
		retval	=0;
	}

	return	retval;
}

static uint32	GetCPUIDString(uint32 funcNum, char *szId)
{
	uint32	retval;

	__try
	{
		_asm
		{
			mov	eax,funcNum
			CPUID
			mov	retval,eax
			mov	eax,szId
			mov	dword ptr[eax],ebx
			mov	dword ptr[eax+4],edx
			mov	dword ptr[eax+8],ecx
		}
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
		retval	=0;
	}

	return	retval;
}

static void	GetCPUIDStringAMD(uint32 funcNum, char *szId)
{
	uint32	retval;
	
	__try
	{
		_asm
		{
			mov eax,funcNum
			CPUID
			mov	retval,eax
			mov	eax,szId
			mov	dword ptr[eax+4],ebx
			mov	dword ptr[eax+8],ecx
			mov	ebx,retval
			mov	dword ptr[eax+12],edx
			mov	dword ptr[eax],ebx
		}
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
		retval	=0;
	}

   printf("AMDSTRING\n\n\n\n\n");
}
#endif
// For out of order processors, the cpuid does serliaization
// On all processors, additional overhead is added
static __int64	GetRDTSC(void)
{
	__int64	clock;

#ifndef JET64
	
	_asm
	{
		push	ebx
		push	ecx
		xor		eax,eax
		CPUID
		RDTSC
		mov		dword ptr clock,eax
		mov		dword ptr clock+4,edx
		xor		eax,eax
		CPUID
		pop		ecx
		pop		ebx
	}

#else
	clock = __rdtsc();
#endif

	return	clock;
}


static uint32	GetRDTSCOverHead(void)
{
	uint32		elap, MinOverhead	=0xffffff;
	__int64	start;
	int		x;

	for(x=0;x < 50;x++)
	{
		start		=GetRDTSC();
		elap		=(uint32)(GetRDTSC() - start);
		MinOverhead =min(MinOverhead, elap);
	}
	return	MinOverhead;
}

#if 0 

static uint32	GetMHZ(void)
{
	__int64		StartClock, ElapClock;
	uint32		StartTime, ElapTime, MHZ;
	
	StartClock = GetRDTSC();
	StartTime  = timeGetTime();

	// this loop should take MHZ_MILLIS milliseconds
	do {
		ElapTime = timeGetTime() - StartTime;
	} while( ElapTime < MHZ_MILLIS );

	ElapClock = GetRDTSC() - StartClock;
	MHZ	= (uint32)(ElapClock/(1000*ElapTime));

	// check for common errors
	{
	uint32 Z50,Z83;
		Z50 = ((MHZ + 25)/50)*50; // round to the nearest fifty
		Z83 = (int)((MHZ + 41)*12/1000)*1000/12; // round to the nearest 83.333 == 1000/12
		if ( abs(Z50 - MHZ) < 7 )
			MHZ = Z50;
		else if ( abs(Z83 - MHZ) < 7 )
			MHZ = Z83;
		else
		{
			MHZ = ((MHZ + 5)/10)*10; // round to the nearest ten
		}
	}

	return MHZ;
}

#else

static __int64 StartClock;

static uint32 StartTime;

static void StartGetMHZ(void)
{
	StartClock = GetRDTSC();
	StartTime  = timeGetTime();	
}

static uint32	GetMHZ(void)
{
	__int64		ElapClock;
	uint32		ElapTime, MHZ;
	
	do {
		ElapTime = timeGetTime() - StartTime;
	} while( ElapTime < MHZ_MILLIS );

	ElapClock = GetRDTSC() - StartClock;
	MHZ	= (uint32)(ElapClock/(1000*ElapTime));

	// check for common errors
	{
	uint32 Z50,Z83;
		Z50 = ((MHZ + 25)/50)*50; // round to the nearest fifty
		Z83 = (int)((MHZ + 41)*12/1000)*1000/12; // round to the nearest 83.333 == 1000/12
		//	by trilobite	Jan. 2011
		//if ( abs(Z50 - MHZ) < 7 )
		if ( abs((int)(Z50 - MHZ)) < 7 )
		//
			MHZ = Z50;
		//	by trilobite	Jan. 2011
		//else if ( abs(Z83 - MHZ) < 7 )
		else if ( abs((int)(Z83 - MHZ)) < 7 )
			//
			MHZ = Z83;
		else
		{
			MHZ = ((MHZ + 5)/10)*10; // round to the nearest ten
		}
	}

	return MHZ;
}
#endif

#ifdef WIN32
static jeBoolean GetPerformanceFreq(void)
{
	LARGE_INTEGER Freq;

	if (!QueryPerformanceFrequency(&Freq))
	{
		jeErrorLog_Add(JE_ERR_NO_PERF_FREQ, NULL);
		return JE_FALSE;
	}

	jeCPU_PerformanceFreq = (float) Freq.LowPart;

	return JE_TRUE;
}
#endif

#ifdef JET64
#include <intrin.h>

//  Misc.
bool HW_MMX;
bool HW_x64;
bool HW_ABM;      // Advanced Bit Manipulation
bool HW_RDRAND;
bool HW_BMI1;
bool HW_BMI2;
bool HW_ADX;
bool HW_PREFETCHWT1;

//  SIMD: 128-bit
bool HW_SSE;
bool HW_SSE2;
bool HW_SSE3;
bool HW_SSSE3;
bool HW_SSE41;
bool HW_SSE42;
bool HW_SSE4a;
bool HW_AES;
bool HW_SHA;

//  SIMD: 256-bit
bool HW_AVX;
bool HW_XOP;
bool HW_FMA3;
bool HW_FMA4;
bool HW_AVX2;

//  SIMD: 512-bit
bool HW_AVX512F;    //  AVX512 Foundation
bool HW_AVX512CD;   //  AVX512 Conflict Detection
bool HW_AVX512PF;   //  AVX512 Prefetch
bool HW_AVX512ER;   //  AVX512 Exponential + Reciprocal
bool HW_AVX512VL;   //  AVX512 Vector Length Extensions
bool HW_AVX512BW;   //  AVX512 Byte + Word
bool HW_AVX512DQ;   //  AVX512 Doubleword + Quadword
bool HW_AVX512IFMA; //  AVX512 Integer 52-bit Fused Multiply-Add
bool HW_AVX512VBMI; //  AVX512 Vector Byte Manipulation Instructions

std::string strCPUName;

uint32 regs[4];

jeBoolean jeCPU_GetInfo(void)
{
	int nIds = 0;

	__cpuid((int*)regs, 0);
	nIds = regs[0];

	__cpuid((int*)regs, 0x80000000);
	unsigned nExIds = regs[0];

	if (nIds >= 0x00000001)
	{
		__cpuid((int*)regs, 0x00000001);
		HW_MMX = (regs[3] & ((int)1 << 23)) != 0;
		HW_SSE = (regs[3] & ((int)1 << 25)) != 0;
		HW_SSE2 = (regs[3] & ((int)1 << 26)) != 0;
		HW_SSE3 = (regs[2] & ((int)1 << 0)) != 0;

		HW_SSSE3 = (regs[2] & ((int)1 << 9)) != 0;
		HW_SSE41 = (regs[2] & ((int)1 << 19)) != 0;
		HW_SSE42 = (regs[2] & ((int)1 << 20)) != 0;
		HW_AES = (regs[2] & ((int)1 << 25)) != 0;

		HW_AVX = (regs[2] & ((int)1 << 28)) != 0;
		HW_FMA3 = (regs[2] & ((int)1 << 12)) != 0;

		HW_RDRAND = (regs[2] & ((int)1 << 30)) != 0;
	}

	if (nIds >= 0x00000007)
	{
		HW_AVX2 = (regs[1] & ((int)1 << 5)) != 0;

		HW_BMI1 = (regs[1] & ((int)1 << 3)) != 0;
		HW_BMI2 = (regs[1] & ((int)1 << 8)) != 0;
		HW_ADX = (regs[1] & ((int)1 << 19)) != 0;
		HW_SHA = (regs[1] & ((int)1 << 29)) != 0;
		HW_PREFETCHWT1 = (regs[2] & ((int)1 << 0)) != 0;

		HW_AVX512F = (regs[1] & ((int)1 << 16)) != 0;
		HW_AVX512CD = (regs[1] & ((int)1 << 28)) != 0;
		HW_AVX512PF = (regs[1] & ((int)1 << 26)) != 0;
		HW_AVX512ER = (regs[1] & ((int)1 << 27)) != 0;
		HW_AVX512VL = (regs[1] & ((int)1 << 31)) != 0;
		HW_AVX512BW = (regs[1] & ((int)1 << 30)) != 0;
		HW_AVX512DQ = (regs[1] & ((int)1 << 17)) != 0;
		HW_AVX512IFMA = (regs[1] & ((int)1 << 21)) != 0;
		HW_AVX512VBMI = (regs[2] & ((int)1 << 1)) != 0;
	}

	if (nExIds >= 0x80000001)
	{
		__cpuid((int*)regs, 0x80000001);
		HW_x64 = (regs[3] & ((int)1 << 29)) != 0;
		HW_ABM = (regs[2] & ((int)1 << 5)) != 0;
		HW_SSE4a = (regs[2] & ((int)1 << 6)) != 0;
		HW_FMA4 = (regs[2] & ((int)1 << 16)) != 0;
		HW_XOP = (regs[2] & ((int)1 << 11)) != 0;
	}

	jeCPU_Features = 0;

	if (HW_MMX) jeCPU_Features |= JE_CPU_HAS_MMX;
	if (HW_RDRAND) jeCPU_Features |= JE_CPU_HAS_RDTSC;
	if (HW_SSE) jeCPU_Features |= JE_CPU_HAS_KATMAI;
	if (HW_SSE2) jeCPU_Features |= JE_CPU_HAS_SSE2;

	return JE_TRUE;
}

const uint32 &jeCPU_GetEAX()
{
	return regs[0];
}

const uint32 &jeCPU_GetEBX()
{
	return regs[1];
}

const uint32 &jeCPU_GetECX()
{
	return regs[2];
}

const uint32 &jeCPU_GetEDX()
{
	return regs[3];
}

#else

jeBoolean jeCPU_GetInfo(void)
{
	int BrandIdx;
	static jeBoolean GotInfo = JE_FALSE;
	char * CPUName = NULL;
	uint32	TypeFlags;

	if ( GotInfo )
		return JE_TRUE;
	GotInfo = JE_TRUE;

	StartGetMHZ();

	MaxCPUIDVal = GetCPUIDString(0, ProcVendorString);
	ProcVendorString[13]=0;

	if(strncmp(ProcVendorString, "GenuineIntel", 12)==0)
	{
		TypeFlags = GetCPUIDEDX(0x1);

		CPUName = "Intel ";
	
		jeCPU_Features	=(TypeFlags & (1<<23))? JE_CPU_HAS_MMX : 0;
		jeCPU_Features	=(TypeFlags & (1<<4))? jeCPU_Features | JE_CPU_HAS_RDTSC : jeCPU_Features;
		jeCPU_Features	=(TypeFlags & (1<<3))? jeCPU_Features | JE_CPU_HAS_CMOV : jeCPU_Features;
		jeCPU_Features	=((TypeFlags & ((1<<15) | 1))==((1<<15) | 1))? jeCPU_Features | JE_CPU_HAS_FCMOV : jeCPU_Features;
		jeCPU_Features	=(TypeFlags & (1<<8))? jeCPU_Features | JE_CPU_HAS_CMPXCHG8B : jeCPU_Features;
		jeCPU_Features	=(TypeFlags & (1<<25))? jeCPU_Features | JE_CPU_HAS_KATMAI : jeCPU_Features;
		jeCPU_Features	=(TypeFlags & (1<<25))? jeCPU_Features | JE_CPU_HAS_SSE2 : jeCPU_Features;

		TypeFlags		=GetCPUIDEAX(1);
		ProcType		=(TypeFlags>>12)&0x3;
		ProcFamily		=(TypeFlags>>8)&0xf;
		ProcModel		=(TypeFlags>>4)&0xf;
		ProcStepping	=(TypeFlags)&0x7;
	}
	else if (	strncmp(ProcVendorString, "AuthenticAMD", 12) == 0 )
	{
		CPUName = "AMD ";

		TypeFlags = GetCPUIDEDX(0x1);

		jeCPU_Features	=(TypeFlags & (1<<23))? JE_CPU_HAS_MMX : 0;
      	jeCPU_Features	=(TypeFlags & (1<<4))? jeCPU_Features | JE_CPU_HAS_RDTSC : jeCPU_Features;
      	jeCPU_Features	=(TypeFlags & (1<<3))? jeCPU_Features | JE_CPU_HAS_CMOV : jeCPU_Features;
      	jeCPU_Features	=((TypeFlags & (1<<15 | 1))==(1<<15 | 1))? jeCPU_Features | JE_CPU_HAS_FCMOV : jeCPU_Features;
      	jeCPU_Features	=(TypeFlags & (1<<8))? jeCPU_Features | JE_CPU_HAS_CMPXCHG8B : jeCPU_Features;
      	jeCPU_Features	=(TypeFlags & (1<<25))? jeCPU_Features | JE_CPU_HAS_KATMAI : jeCPU_Features;

		TypeFlags = GetCPUIDEAX(0x80000000);
		if ( TypeFlags )	//extended functions supported
		{
			TypeFlags = GetCPUIDEDX(0x80000001);
			GetCPUIDStringAMD(0x80000002, ProcName);
			GetCPUIDStringAMD(0x80000003, ProcName+16);
			GetCPUIDStringAMD(0x80000004, ProcName+32);

			jeCPU_Features	=(TypeFlags & (1<<23))? JE_CPU_HAS_MMX : 0;
			jeCPU_Features	=(TypeFlags & (1<<4))? jeCPU_Features | JE_CPU_HAS_RDTSC : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<31))? jeCPU_Features | JE_CPU_HAS_3DNOW : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<15))? jeCPU_Features | JE_CPU_HAS_CMOV : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<16))? jeCPU_Features | JE_CPU_HAS_FCMOV : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<8))? jeCPU_Features | JE_CPU_HAS_CMPXCHG8B : jeCPU_Features;
		}
		else
		{
			TypeFlags = GetCPUIDEDX(0x1);

			jeCPU_Features	=(TypeFlags & (1<<23))? JE_CPU_HAS_MMX : 0;
			jeCPU_Features	=(TypeFlags & (1<<4))? jeCPU_Features | JE_CPU_HAS_RDTSC : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<3))? jeCPU_Features | JE_CPU_HAS_CMOV : jeCPU_Features;
			jeCPU_Features	=((TypeFlags & (1<<15 | 1))==(1<<15 | 1))? jeCPU_Features | JE_CPU_HAS_FCMOV : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<8))? jeCPU_Features | JE_CPU_HAS_CMPXCHG8B : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<25))? jeCPU_Features | JE_CPU_HAS_KATMAI : jeCPU_Features;
		}

		TypeFlags		=GetCPUIDEAX(1);
		ProcType		=(TypeFlags>>12)&0x3;
		ProcFamily		=(TypeFlags>>8)&0xf;
		ProcModel		=(TypeFlags>>4)&0xf;
        ProcStepping=(TypeFlags)&0x7;

      	TypeFlags   =GetCPUIDEBX(1);
      	BrandIdx = (TypeFlags & 0x000000FF);
      	switch (BrandIdx) {
      		case 0 : strcpy(ProcName, "Unknown");break;
      		case 1 : strcpy(ProcName, "Pentium 3"); break;
      		case 2 : strcpy(ProcName, "Pentium 3 Xeon"); break;
      		case 8 : strcpy(ProcName, "Pentium 4"); break;
      	}
	}
	else // Cyrix and Centaur are basically like K6
	{
		TypeFlags = GetCPUIDEAX(0x80000000);

		if ( strncmp(ProcVendorString, "CentaurHauls", 12) == 0 ) // <> CB questionable : Centaur is just like AMD ?
			CPUName = "Centaur ";
		else
			CPUName = "Unknown ";

		if(TypeFlags)	//extended functions supported
		{
			TypeFlags = GetCPUIDEDX(0x80000001);
			GetCPUIDStringAMD(0x80000002, ProcName);
			GetCPUIDStringAMD(0x80000003, ProcName+16);
			GetCPUIDStringAMD(0x80000004, ProcName+32);

			jeCPU_Features	=(TypeFlags & (1<<23))? JE_CPU_HAS_MMX : 0;
			jeCPU_Features	=(TypeFlags & (1<<4))? jeCPU_Features | JE_CPU_HAS_RDTSC : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<31))? jeCPU_Features | JE_CPU_HAS_3DNOW : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<15))? jeCPU_Features | JE_CPU_HAS_CMOV : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<16))? jeCPU_Features | JE_CPU_HAS_FCMOV : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<8))? jeCPU_Features | JE_CPU_HAS_CMPXCHG8B : jeCPU_Features;
		}
		else
		{
			TypeFlags = GetCPUIDEDX(0x1);

			jeCPU_Features	=(TypeFlags & (1<<23))? JE_CPU_HAS_MMX : 0;
			jeCPU_Features	=(TypeFlags & (1<<4))? jeCPU_Features | JE_CPU_HAS_RDTSC : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<3))? jeCPU_Features | JE_CPU_HAS_CMOV : jeCPU_Features;
			jeCPU_Features	=((TypeFlags & (1<<15 | 1))==(1<<15 | 1))? jeCPU_Features | JE_CPU_HAS_FCMOV : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<8))? jeCPU_Features | JE_CPU_HAS_CMPXCHG8B : jeCPU_Features;
			jeCPU_Features	=(TypeFlags & (1<<25))? jeCPU_Features | JE_CPU_HAS_KATMAI : jeCPU_Features;
		}

		TypeFlags		=GetCPUIDEAX(1);
		ProcType		=(TypeFlags>>12)&0x3;
		ProcFamily		=(TypeFlags>>8)&0xf;
		ProcModel		=(TypeFlags>>4)&0xf;
		ProcStepping	=(TypeFlags)&0x7;
	}

#ifdef WIN32
	GetPerformanceFreq();
#endif

	jeCPU_MHZ = GetMHZ();
	jeCPU_SecondsPerClock = 1.0f / ( 1000000.0f * jeCPU_MHZ );

	ThreadLog_Initialize();
	Log_Printf("CPU : %d MHz ",jeCPU_MHZ);
	
	if ( CPUName ) Log_Printf(CPUName);
	if ( jeCPU_Features & JE_CPU_HAS_MMX ) Log_Printf("MMX ");
	if ( jeCPU_Features & JE_CPU_HAS_RDTSC )
	{
		Log_Printf("Pentium "); 
		if ( jeCPU_Features & JE_CPU_HAS_CMOV ) Log_Printf("Pro ");
	}
	else Log_Printf("486 ");
	Log_Printf("Class ");
	if ( jeCPU_Features & JE_CPU_HAS_3DNOW ) Log_Printf("with 3DNOW ");
//	if ( jeCPU_Features & JE_CPU_HAS_KATMAI ) Log_Printf("with Katmai ");
	Log_Printf("\n");
	jeCPU_Features &= ~JE_CPU_HAS_KATMAI;

	return JE_TRUE;
}
#endif

/*}{**** Functions : FloatControl related stuff *********************/

#define STACK_SIZE	(1024)
static uint16 ControlStack[STACK_SIZE];
static int ControlStackI = 0;
/*
void jeCPU_FloatControl_Push(void)
{
	uint16 control;
	assert(ControlStackI < STACK_SIZE);
	__asm
	{
		FNSTCW control
	}

	ControlStack[ControlStackI] = control;
	ControlStackI ++;
}


void jeCPU_FloatControl_Pop(void)
{
	uint16 control;
	assert(ControlStackI > 0 );
	ControlStackI --;
	control = ControlStack[ControlStackI];
	
	__asm
	{	
		FLDCW control
	}
	
}

void jeCPU_FloatControl_RoundDown(void)
{
	uint16 control;

	__asm
	{
		FNSTCW control
	}

	control &= ~(3<<10);
	control |=  (1<<10);

	__asm
	{	
		FLDCW control
	}
}

void jeCPU_FloatControl_RoundNearest(void)
{
	uint16 control;
	__asm
	{
		FNSTCW control
	}

	control &= ~(3<<10);
	control |=  (1<<10);

	__asm
	{	
		FLDCW control
	}
}

void jeCPU_FloatControl_SinglePrecision(void)
{
	uint16 control;

	__asm
	{
		FNSTCW control
	}

	control &= ~(3<<8);

	__asm
	{	
		FLDCW control
	}
}

void jeCPU_FloatControl_DoublePrecision(void)
{
	uint16 control;
	__asm
	{
		FNSTCW control
	}

	control &= ~(3<<8);
	control |=  (1<<8);

	__asm
	{	
		FLDCW control
	}
}
*/
/*}{**** Functions : MMX related stuff *********************/

/*static jeBoolean jeCPU_InMMX = JE_FALSE;
static jeBoolean jeCPU_WasInMMX = JE_FALSE;

void jeCPU_EnterMMX(void)
{
	if ( ! jeCPU_InMMX )
	{
		jeCPU_FloatControl_Push();
		jeCPU_InMMX = JE_TRUE;
		// as long as there's nothing on the floating point stack, you can enter MMX whenever you want!
	}
}

void jeCPU_LeaveMMX(void)
{
	if ( jeCPU_InMMX )
	{
		if ( jeCPU_Features & JE_CPU_HAS_MMX )
		{
			__asm { emms }
		}
		jeCPU_FloatControl_Pop();
		jeCPU_InMMX = JE_FALSE;
	}
}


void jeCPU_PauseMMX(void)
{	// to temporarily used floats inside an MMX section:
	jeCPU_WasInMMX = jeCPU_InMMX;
	jeCPU_LeaveMMX();
}

void jeCPU_ResumeMMX(void)
{
	if ( jeCPU_WasInMMX )
		jeCPU_EnterMMX();
}
*/