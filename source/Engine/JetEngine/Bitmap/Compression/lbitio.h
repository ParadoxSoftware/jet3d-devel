/****************************************************************************************/
/*  LBITIO.H                                                                            */
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
/*
 *
 * Totally changed 6-12-95
 *
 * we now properly write bits left->right
 * 
 * all previous programs must be modified to be accomodate this
 *
 * new 6-13 : after FlushWrite,	you must read the return value of FlushWrite
 *
 */

#ifndef L_BIT_IO_H
#define L_BIT_IO_H

#include "Utility.h"

struct LBitIOInfo
  {
  uint32 BitBuffer;
  int BitsToGo;
  uint8 *BitArray;
  uint8 *BitArrayPtr;
  };

//externs in LBitIO.c :

extern uint8 * LBitIO_GetArray(struct LBitIOInfo * BII);
extern  struct LBitIOInfo * LBitIO_Init(uint8 *Array);
extern  void LBitIO_CleanUp(struct LBitIOInfo * BII);
extern  long LBitIO_FlushWrite(struct LBitIOInfo * BII); //returns length of data
extern  void LBitIO_ResetArray(struct LBitIOInfo * BII,uint8 *Array);             
extern  long LBitIO_GetPos(struct LBitIOInfo * BII);
extern  long LBitIO_GetPosD(struct LBitIOInfo * BII);

//macros:

/*

BII = (struct LBitIOInfo * BII)

void LBitIO_StepArray(struct LBitIOInfo * BII,int step);
void LBitIO_WriteZeroBits(BII,long BitStrgLen);
void LBitIO_WriteBits(BII,uint32 BitStrg,long BitStrgLen);
void LBitIO_WriteBit(BII,uint32 Bit);
void LBitIO_WriteZeroBit(BII);
void LBitIO_InitRead(BII);
void LBitIO_ReadBit(BII,jeBoolean Bit); //fills out Bit
void LBitIO_ReadBits(BII,uint32 BitStrg,long BitStrgLen); //fills out BitStrg
void LBitIO_PeekBits(BII,uint32 BitStrg,long BitStrgLen); //fills out BitStrg
void LBitIO_SkipBits(BII,long BitStrgLen);

*/

#define LBitIO_StepArray(BII,step)	BII->BitArrayPtr += step;

/* write BitBuffer to BitArray */
#define LBitIO_WriteBuf(BII)	do { 					\
*(BII->BitArrayPtr)++ = BII->BitBuffer>>24;				\
*(BII->BitArrayPtr)++ = (BII->BitBuffer>>16)&0xFF;		\
*(BII->BitArrayPtr)++ = (BII->BitBuffer>>8)&0xFF;		\
*(BII->BitArrayPtr)++ = BII->BitBuffer&0xFF; } while(0)

/* read BitBuffer from BitArray */
#define LBitIO_ReadBuf(BII)	do {								\
BII->BitBuffer  = *(BII->BitArrayPtr)++; BII->BitBuffer<<=8;	\
BII->BitBuffer += *(BII->BitArrayPtr)++; BII->BitBuffer<<=8;	\
BII->BitBuffer += *(BII->BitArrayPtr)++; BII->BitBuffer<<=8;	\
BII->BitBuffer += *(BII->BitArrayPtr)++; } while(0)


/*
 *  Write <BitStrgLen> number of zero bits into the BII
 *  params: struct LBitIOInfo * BII,long BitStrgLen
 *
 */
#define LBitIO_WriteZeroBits(BII,BitStrgLen) do {	\
if ( BII->BitsToGo < BitStrgLen   )               \
  {                                               \
	BII->BitBuffer <<= BII->BitsToGo;								\
	BII->BitsToGo = BitStrgLen - BII->BitsToGo;			\
	LBitIO_WriteBuf(BII);            								\
  BII->BitBuffer = 0;                             \
  BII->BitsToGo = 32 - BII->BitsToGo;							\
  }                                               \
else                                              \
  {                                               \
	BII->BitBuffer <<= BitStrgLen;                  \
  BII->BitsToGo -= BitStrgLen;                    \
                                                  \
  if ( BII->BitsToGo == 0 )                       \
    {                                             \
	LBitIO_WriteBuf(BII);							\
    BII->BitsToGo = 32;                           \
    BII->BitBuffer = 0;                           \
    }                                             \
  }                                           } while(0)
/* End LBitIO_WriteZeroBits */

/*
 *  Write <BitStrgLen> number of bits from BitStrg into the BII
 *  params: struct LBitIOInfo * BII,uint32 BitStrg,long BitStrgLen
 *
 */
#define LBitIO_WriteBits(BII,BitStrg,BitStrgLen) do { \
if ( BII->BitsToGo < (BitStrgLen) )               \
  {                                               \
	BII->BitBuffer <<= BII->BitsToGo;					\
	BII->BitsToGo = (BitStrgLen) - BII->BitsToGo;			\
	BII->BitBuffer += (BitStrg) >> BII->BitsToGo;			\
	LBitIO_WriteBuf(BII);									\
  BII->BitBuffer = (BitStrg) & ( ( 1 << BII->BitsToGo ) - 1 );\
  BII->BitsToGo = 32 - BII->BitsToGo;							\
  }                                               \
else                                              \
  {                                               \
	BII->BitBuffer <<= BitStrgLen;                  \
	BII->BitBuffer += (BitStrg);                      \
  BII->BitsToGo -= (BitStrgLen)  ;                  \
                                                  \
  if ( BII->BitsToGo == 0 )                       \
    {                                             \
		LBitIO_WriteBuf(BII);					\
    BII->BitsToGo = 32;                           \
    BII->BitBuffer = 0;                           \
    }                                             \
  }                                              } while(0)
/* End LBitIO_WriteBits */

/*
 *  Write a Bit to the BII - Bit may be True or False (need not be 0 or 1)
 *  params: struct LBitIOInfo * BII,uint32 Bit
 *
 */
#define LBitIO_WriteBit(BII,Bit)   			do {\
BII->BitBuffer = (BII->BitBuffer<<1) + (Bit);	\
BII->BitsToGo--;                                \
if ( ! BII->BitsToGo )               			\
  {                                             \
	LBitIO_WriteBuf(BII);						\
  BII->BitsToGo = 32;                           \
  BII->BitBuffer = 0;                           \
  }                                        } while(0)
/* End LBitIO_WriteBit */

#define LBitIO_WriteZeroBit(BII)   		do {	\
BII->BitBuffer <<= 1;							\
BII->BitsToGo--;                                \
if ( ! BII->BitsToGo )               		    \
  {                                             \
	LBitIO_WriteBuf(BII);						\
  BII->BitsToGo = 32;                           \
  BII->BitBuffer = 0;                           \
  }                                     } while(0)
/* End LBitIO_WriteZeroBit */


/*
 *  Reads bits into BitBuffer from BitArray
 *  must be done before any _Read commands
 *
 */
#define LBitIO_InitRead(BII) do { LBitIO_ReadBuf(BII);    \
BII->BitsToGo = 32; } while(0)
/* End LBitIO_InitRead */

extern const uint32 LBitIOReadBitMask;

/*
 *  Read a bit from BII into (jeBoolean Bit)
 *
 */
#define LBitIO_ReadBit(BII,Bit)  do {                \
if ( BII->BitBuffer & LBitIOReadBitMask )        				\
  Bit = 1;                                        \
else                                              \
  Bit = 0;                                        \
BII->BitBuffer <<= 1;															\
BII->BitsToGo --;                                 \
if ( BII->BitsToGo == 0 )                         \
  {                                               \
	LBitIO_ReadBuf(BII);														\
  BII->BitsToGo = 32;                             \
  }                                 } while(0)
/* End LBitIO_ReadBit */



/*
 *  Read <BitStrgLen> number of bits into BitStrg from the BII
 *  params: struct LBitIOInfo * BII,uint32 BitStrg,long BitStrgLen
 *
 *  fills out BitStrg given BitStrgLen
 *  BitStrgLen must be less than 32
 *
 */
#define LBitIO_ReadBits(BII,BitStrg,BitStrgLen)    do {		\
if ( BII->BitsToGo < BitStrgLen )                                     \
  {                                                                   \
  BitStrg = BII->BitBuffer >> ( 32 - BitStrgLen );										\
                                                                      \
	LBitIO_ReadBuf(BII);																								\
                                                                      \
  BII->BitsToGo = 32 - BitStrgLen + BII->BitsToGo;                    \
  BitStrg += BII->BitBuffer >> BII->BitsToGo;                         \
	BII->BitBuffer <<= (32 - BII->BitsToGo);                            \
  }                                                                   \
else                                                                  \
  {                                                                   \
  BitStrg = BII->BitBuffer >> ( 32 - BitStrgLen );										\
	BII->BitBuffer <<= BitStrgLen;                                      \
  BII->BitsToGo -= BitStrgLen;                                        \
                                                                      \
  if ( BII->BitsToGo == 0 )                                           \
    {                                                                 \
		LBitIO_ReadBuf(BII);																							\
    BII->BitsToGo = 32;                                               \
    }                                                                 \
  }                                                          } while(0)
/* End LBitIO_ReadBits */

/*
 *  Read <BitStrgLen> number of bits into BitStrg from the BII
 *  params: struct LBitIOInfo * BII,uint32 BitStrg,long BitStrgLen
 *
 *  fills out BitStrg given BitStrgLen
 *  BitStrgLen must be less than 32
 *
 *  PeekBits is the same as ReadBits except that the BII
 *		is not advanced
 *
 */
#define LBitIO_PeekBits(BII,BitStrg,BitStrgLen)         do {		\
BitStrg = BII->BitBuffer >> ( 32 - BitStrgLen );										  \
if ( BII->BitsToGo < BitStrgLen )                                     \
	{																																		\
	uint32 PeekBitBuf = (BII->BitArrayPtr[0]<<24)+(BII->BitArrayPtr[1]<<16)+(BII->BitArrayPtr[2]<<8)+(BII->BitArrayPtr[3]);	\
  BitStrg += ( PeekBitBuf >> ( 32 - BitStrgLen + BII->BitsToGo ));		} } while(0)
/* End LBitIO_PeekBits */

/*
 *  Skip past <BitStrgLen> number of bits into BitStrg from the BII
 *  params: struct LBitIOInfo * BII,long BitStrgLen
 *  BitStrgLen must be less than 32
 *
 */
#define LBitIO_SkipBits(BII,BitStrgLen)             	do {			\
if ( BII->BitsToGo < BitStrgLen )                                     \
  {                                                                   \
	LBitIO_ReadBuf(BII);																								\
  BII->BitsToGo += 32 - BitStrgLen;                                   \
	BII->BitBuffer <<= (32 - BII->BitsToGo);                            \
  }                                                                   \
else                                                                  \
  {                                                                   \
	BII->BitBuffer <<= BitStrgLen;                                      \
  BII->BitsToGo -= BitStrgLen;                                        \
                                                                      \
  if ( BII->BitsToGo == 0 )                                           \
    {                                                                 \
		LBitIO_ReadBuf(BII);																							\
    BII->BitsToGo = 32;                                               \
    }                                                                 \
  }                                                        } while(0)
/* End LBitIO_SkipBits */


/************ now LOCAL versions ***********/

#define LocalLBitIO_Variables()					\
uint32 LocalBitBuffer; int LocalBitsToGo;		\
uint8 *LocalBitArray,*LocalBitArrayPtr;			/***/

#define LocalLBitIO_PutState(BII) do {				\
BII->BitBuffer 		= LocalBitBuffer;				\
BII->BitsToGo 		= LocalBitsToGo;				\
BII->BitArray 		= LocalBitArray;				\
BII->BitArrayPtr 	= LocalBitArrayPtr;	} while(0) /***/


#define LocalLBitIO_GetState(BII) do {				\
LocalBitBuffer 		= BII->BitBuffer;				\
LocalBitsToGo 		= BII->BitsToGo;				\
LocalBitArray 		= BII->BitArray;				\
LocalBitArrayPtr 	= BII->BitArrayPtr;	} while(0) 	/***/

/* write BitBuffer to BitArray */
#define LocalLBitIO_WriteBuf() do {						\
*LocalBitArrayPtr++ = LocalBitBuffer>>24;				\
*LocalBitArrayPtr++ = (LocalBitBuffer>>16)&0xFF;		\
*LocalBitArrayPtr++ = (LocalBitBuffer>>8)&0xFF;			\
*LocalBitArrayPtr++ = LocalBitBuffer&0xFF;			} while(0) 			/* */

/* read BitBuffer from BitArray */
#define LocalLBitIO_ReadBuf()	 do {						\
LocalBitBuffer  = *LocalBitArrayPtr++; LocalBitBuffer<<=8;	\
LocalBitBuffer += *LocalBitArrayPtr++; LocalBitBuffer<<=8;	\
LocalBitBuffer += *LocalBitArrayPtr++; LocalBitBuffer<<=8;	\
LocalBitBuffer += *LocalBitArrayPtr++; 	} while(0) 		/* */


/*
 *  Write <BitStrgLen> number of zero bits into the BII
 *  params: struct LocalLBitIOInfo * BII,long BitStrgLen
 *
 */
#define LocalLBitIO_WriteZeroBits(BitStrgLen)  do {	\
if ( LocalBitsToGo < BitStrgLen   )               \
  {                                               \
	LocalBitBuffer <<= LocalBitsToGo;				\
	LocalBitsToGo = BitStrgLen - LocalBitsToGo;		\
	LocalLBitIO_WriteBuf();            				\
  LocalBitBuffer = 0;                             \
  LocalBitsToGo = 32 - LocalBitsToGo;				\
  }                                               \
else                                              \
  {                                               \
	LocalBitBuffer <<= BitStrgLen;                \
  LocalBitsToGo -= BitStrgLen;                    \
                                                  \
  if ( LocalBitsToGo == 0 )                       \
    {                                             \
		LocalLBitIO_WriteBuf();						\
    LocalBitsToGo = 32;                           \
    LocalBitBuffer = 0;                           \
    }                                             \
  }                                	} while(0) 
/* End LocalLBitIO_WriteZeroBits */

/*
 *  Write <BitStrgLen> number of bits from BitStrg into the BII
 *  params: struct LocalLBitIOInfo * BII,uint32 BitStrg,long BitStrgLen
 *
 */
#define LocalLBitIO_WriteBits(BitStrg,BitStrgLen) do { \
if ( LocalBitsToGo < BitStrgLen   )               \
  {                                               \
	LocalBitBuffer <<= LocalBitsToGo;				\
	LocalBitsToGo = BitStrgLen - LocalBitsToGo;		\
	LocalBitBuffer += BitStrg >> LocalBitsToGo;		\
	LocalLBitIO_WriteBuf();							\
  LocalBitBuffer = BitStrg & ( ( 1 << LocalBitsToGo ) - 1 );\
  LocalBitsToGo = 32 - LocalBitsToGo;				\
  }                                               \
else                                              \
  {                                               \
	LocalBitBuffer <<= BitStrgLen;                  \
	LocalBitBuffer += BitStrg;                      \
  LocalBitsToGo -= BitStrgLen  ;                  \
                                                  \
  if ( LocalBitsToGo == 0 )                       \
    {                                             \
		LocalLBitIO_WriteBuf();						\
    LocalBitsToGo = 32;                           \
    LocalBitBuffer = 0;                           \
    }                                             \
  }                	} while(0)
/* End LocalLBitIO_WriteBits */

/*
 *  Write a Bit to the BII - Bit may be True or False (need not be 0 or 1)
 *  params: struct LocalLBitIOInfo * BII,uint32 Bit
 *
 */ 
#define LocalLBitIO_WriteBit(Bit)   			do { \
LocalBitBuffer = (LocalBitBuffer<<1) + Bit;		  \
LocalBitsToGo--;                                  \
if ( ! LocalBitsToGo )               			    \
  {                                             	\
	LocalLBitIO_WriteBuf();							\
	LocalBitsToGo = 32;                           	\
	LocalBitBuffer = 0;                           	\
  }                                 	} while(0) 
/* End LocalLBitIO_WriteBit */

#define LocalLBitIO_WriteZeroBit()   			do { \
LocalBitBuffer <<= 1;								\
LocalBitsToGo--;                                  	\
if ( ! LocalBitsToGo )               				\
  {                                               	\
	LocalLBitIO_WriteBuf();							\
  LocalBitsToGo = 32;                           	\
  LocalBitBuffer = 0;                           	\
  }                                  	} while(0) 
/* End LocalLBitIO_WriteZeroBit */


/*
 *  Reads bits into BitBuffer from BitArray
 *  must be done before any _Read commands
 *
 */
#define LocalLBitIO_InitRead() do { LocalLBitIO_ReadBuf(); LocalBitsToGo = 32;	} while(0)
/* End LocalLBitIO_InitRead */

extern const uint32 LBitIOReadBitMask;

/*
 *  Read a bit from BII into (jeBoolean Bit)
 *
 */
#define LocalLBitIO_ReadBit(Bit)    do {        \
if ( LocalBitBuffer & LBitIOReadBitMask ) 		\
  Bit = 1;                                        \
else                                              \
  Bit = 0;                                        \
LocalBitBuffer <<= 1;							\
LocalBitsToGo --;                                 \
if ( LocalBitsToGo == 0 )                         \
  {                                               \
	LocalLBitIO_ReadBuf();						\
  LocalBitsToGo = 32;                             \
  }                                       	} while(0) 
/* End LocalLBitIO_ReadBit */



/*
 *  Read <BitStrgLen> number of bits into BitStrg from the BII
 *  params: struct LocalLBitIOInfo * BII,uint32 BitStrg,long BitStrgLen
 *
 *  fills out BitStrg given BitStrgLen
 *  BitStrgLen must be less than 32
 *
 */
#define LocalLBitIO_ReadBits(BitStrg,BitStrgLen)   do {					\
if ( LocalBitsToGo < BitStrgLen )                                     \
  {                                                                   \
  BitStrg = LocalBitBuffer >> ( 32 - BitStrgLen );						\
                                                                      \
	LocalLBitIO_ReadBuf();												\
                                                                      \
  LocalBitsToGo = 32 - BitStrgLen + LocalBitsToGo;                    \
  BitStrg += LocalBitBuffer >> LocalBitsToGo;                         \
	LocalBitBuffer <<= (32 - LocalBitsToGo);                            \
  }                                                                   \
else                                                                  \
  {                                                                   \
  BitStrg = LocalBitBuffer >> ( 32 - BitStrgLen );						\
	LocalBitBuffer <<= BitStrgLen;                                      \
  LocalBitsToGo -= BitStrgLen;                                        \
                                                                      \
  if ( LocalBitsToGo == 0 )                                           \
    {                                                                 \
		LocalLBitIO_ReadBuf();											\
    LocalBitsToGo = 32;                                               \
    }                                                                 \
  }                               	} while(0) 
/* End LocalLBitIO_ReadBits */

/*
 *  Read <BitStrgLen> number of bits into BitStrg from the BII
 *  params: struct LocalLBitIOInfo * BII,uint32 BitStrg,long BitStrgLen
 *
 *  fills out BitStrg given BitStrgLen
 *  BitStrgLen must be less than 32
 *
 *  PeekBits is the same as ReadBits except that the BII
 *		is not advanced
 *
 */
#define LocalLBitIO_PeekBits(BitStrg,BitStrgLen)        do {		\
BitStrg = LocalBitBuffer >> ( 32 - BitStrgLen );					\
if ( LocalBitsToGo < BitStrgLen )                                   \
	{																\
	uint32 PeekBitBuf = (LocalBitArrayPtr[0]<<24)+(LocalBitArrayPtr[1]<<16)+(LocalBitArrayPtr[2]<<8)+(LocalBitArrayPtr[3]);	\
	BitStrg += ( PeekBitBuf >> ( 32 - BitStrgLen + LocalBitsToGo ));\
	}													} while(0) 
/* End LocalLBitIO_PeekBits */

/*
 *  Skip past <BitStrgLen> number of bits into BitStrg from the BII
 *  params: struct LocalLBitIOInfo * BII,long BitStrgLen
 *  BitStrgLen must be less than 32
 *
 */
#define LocalLBitIO_SkipBits(BitStrgLen)        do {					\
if ( LocalBitsToGo < BitStrgLen )                                     \
  {                                                                   \
	LocalLBitIO_ReadBuf();												\
  LocalBitsToGo += 32 - BitStrgLen;                                   \
	LocalBitBuffer <<= (32 - LocalBitsToGo);                            \
  }                                                                   \
else                                                                  \
  {                                                                   \
	LocalBitBuffer <<= BitStrgLen;                                      \
  LocalBitsToGo -= BitStrgLen;                                        \
                                                                      \
  if ( LocalBitsToGo == 0 )                                           \
    {                                                                 \
		LocalLBitIO_ReadBuf();											\
    LocalBitsToGo = 32;                                               \
    }                                                                 \
  }                                      		} while(0)
/* End LocalLBitIO_SkipBits */


#endif // BIT_IO_H
