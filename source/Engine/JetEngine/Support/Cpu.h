/****************************************************************************************/
/*  CPU.H                                                                               */
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
#ifndef JE_CPU_H
#define JE_CPU_H

#include "BaseType.h"
#include <string>

//--------

jeBoolean jeCPU_GetInfo(void);
/*
void jeCPU_FloatControl_Push(void);
void jeCPU_FloatControl_Pop(void);
void jeCPU_FloatControl_RoundDown(void);
void jeCPU_FloatControl_RoundNearest(void);
void jeCPU_FloatControl_SinglePrecision(void);
void jeCPU_FloatControl_DoublePrecision(void);
*/
/*void jeCPU_EnterMMX(void);	// wrap MMX sections with these:
void jeCPU_LeaveMMX(void);

void jeCPU_PauseMMX(void);	// to temporarily used floats inside an MMX section:
void jeCPU_ResumeMMX(void);
*/
//-------- CPU Info:

#define	JE_CPU_HAS_RDTSC		0x0001
#define	JE_CPU_HAS_MMX  		0x0002
#define	JE_CPU_HAS_3DNOW		0x0004
#define	JE_CPU_HAS_CMOV 		0x0008
#define	JE_CPU_HAS_FCMOV		0x0010
#define	JE_CPU_HAS_KATMAI		0x0020
#define JE_CPU_HAS_CMPXCHG8B	0x0040
#define	JE_CPU_HAS_SSE2	        0x0080

extern uint32	jeCPU_Features;			// JE_CPU_HAS bitmasks
extern uint32	jeCPU_MHZ;
extern float	jeCPU_SecondsPerClock;	// == 1.0 / HZ
extern float	jeCPU_PerformanceFreq;	// Number of QueryPerformanceCounter Ticks Per Second

//--------

#ifdef JET64
//  Misc.
extern bool HW_MMX;
extern bool HW_x64;
extern bool HW_ABM;      // Advanced Bit Manipulation
extern bool HW_RDRAND;
extern bool HW_BMI1;
extern bool HW_BMI2;
extern bool HW_ADX;
extern bool HW_PREFETCHWT1;

//  SIMD: 128-bit
extern bool HW_SSE;
extern bool HW_SSE2;
extern bool HW_SSE3;
extern bool HW_SSSE3;
extern bool HW_SSE41;
extern bool HW_SSE42;
extern bool HW_SSE4a;
extern bool HW_AES;
extern bool HW_SHA;

//  SIMD: 256-bit
extern bool HW_AVX;
extern bool HW_XOP;
extern bool HW_FMA3;
extern bool HW_FMA4;
extern bool HW_AVX2;

//  SIMD: 512-bit
extern bool HW_AVX512F;    //  AVX512 Foundation
extern bool HW_AVX512CD;   //  AVX512 Conflict Detection
extern bool HW_AVX512PF;   //  AVX512 Prefetch
extern bool HW_AVX512ER;   //  AVX512 Exponential + Reciprocal
extern bool HW_AVX512VL;   //  AVX512 Vector Length Extensions
extern bool HW_AVX512BW;   //  AVX512 Byte + Word
extern bool HW_AVX512DQ;   //  AVX512 Doubleword + Quadword
extern bool HW_AVX512IFMA; //  AVX512 Integer 52-bit Fused Multiply-Add
extern bool HW_AVX512VBMI; //  AVX512 Vector Byte Manipulation Instructions

extern std::string strCPUName;
#endif

#endif // JE_CPU_H

