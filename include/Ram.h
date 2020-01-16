/****************************************************************************************/
/*  RAM.H                                                                               */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description: Replacement for malloc, realloc and free                               */
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
// RAM memory manager

#ifndef JE_RAM_H
#define JE_RAM_H

#include "BaseType.h"

//////////////////////////////////////////////////////////////////////////
// BlackIce 3/26/2009 completely redesigned
// New memory manager is based on separate Win32 Heap, so all internal objects
// will be allocated from our own heap
// Heap is created on module startup and destroyed on shutdown
// NOTE! Do not call release version of API in debug build, use macros instead

JETAPI void * JETCC jeRam_Allocate( uint32 iSize );        // allocate memory
JETAPI void * JETCC jeRam_AllocateClear( uint32 iSize );   // allocate the ram & clear it. (calloc)
JETAPI void * JETCC jeRam_Realloc( void *pPtr, uint32 iNewSize );
JETAPI void   JETCC jeRam_Free( void *pPtr );              // Free an allocated memory block

JETAPI void * JETCC jeRam_DebugAllocate( uint32 iSize, const char* pFile, uint32 iLine );
JETAPI void * JETCC jeRam_DebugAllocateClear( uint32 iSize, const char* pFile, uint32 iLine );
JETAPI void * JETCC jeRam_DebugRealloc( void* pPtr, uint32 iSize, const char* pFile, uint32 iLine );
JETAPI void   JETCC jeRam_DebugFree( void *pPtr, const char* pFile, uint32 iLine );

JETAPI void   JETCC jeRam_ShowStats( void *fpFile );
JETAPI jeBoolean JETCC jeRam_IsValidPtr( void *pPtr );

#ifdef _DEBUG // debug version

    #define JE_RAM_ALLOCATE(size)                           jeRam_DebugAllocate(size, __FILE__, __LINE__)
    #define JE_RAM_REALLOC(ptr,size)                        jeRam_DebugRealloc(ptr, size, __FILE__, __LINE__)
    #define JE_RAM_ALLOCATE_CLEAR(size)                     jeRam_DebugAllocateClear(size, __FILE__, __LINE__)
    #define JE_RAM_ALLOCATE_STRUCT(type)            (type *)jeRam_DebugAllocate(sizeof(type), __FILE__, __LINE__)
    #define JE_RAM_ALLOCATE_STRUCT_CLEAR(type)      (type *)jeRam_DebugAllocateClear(sizeof(type), __FILE__, __LINE__)
    #define JE_RAM_ALLOCATE_ARRAY(type,count)       (type *)jeRam_DebugAllocate(sizeof(type)*(count), __FILE__, __LINE__)
    #define JE_RAM_ALLOCATE_ARRAY_CLEAR(type,count) (type *)jeRam_DebugAllocateClear(sizeof(type)*(count), __FILE__, __LINE__)
    #define JE_RAM_REALLOC_ARRAY(ptr,type,count)    (type *)jeRam_DebugRealloc((ptr), sizeof(type)*(count), __FILE__, __LINE__)
    #define JE_RAM_FREE(pPtr)                              {jeRam_DebugFree(pPtr, __FILE__, __LINE__ ); pPtr = NULL; }

#else // for release

    #define JE_RAM_ALLOCATE(size)                           jeRam_Allocate(size)
    #define JE_RAM_REALLOC(ptr,size)                        jeRam_Realloc(ptr, size)
    #define JE_RAM_ALLOCATE_CLEAR(size)                     jeRam_AllocateClear(size)
    #define JE_RAM_ALLOCATE_STRUCT(type)            (type *)jeRam_Allocate(sizeof (type))
    #define JE_RAM_ALLOCATE_STRUCT_CLEAR(type)      (type *)jeRam_AllocateClear(sizeof (type))
    #define JE_RAM_ALLOCATE_ARRAY(type,count)       (type *)jeRam_Allocate(sizeof (type) * (count))
    #define JE_RAM_ALLOCATE_ARRAY_CLEAR(type,count) (type *)jeRam_AllocateClear(sizeof (type) * (count))
    #define JE_RAM_REALLOC_ARRAY(ptr,type,count)    (type *)jeRam_Realloc((ptr), sizeof(type) * (count))
    #define JE_RAM_FREE(pPtr)                              {jeRam_Free(pPtr); pPtr = NULL;}

#endif

#endif // JE_RAM_H
