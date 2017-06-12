/****************************************************************************************/
/*  RAM.C                                                                               */
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
// RAM Memory manager

//////////////////////////////////////////////////////////////////////////
// BlackIce 3/26/2009 completely redesigned
// The main main idea is to make all allocations for internal objects from separate heap
// and also keeping track of information about where memory block was allocated in crt heap.
// I'm not sure that engine doesn't overwrites the memory, that is why 2 heaps are used.

//#include "stdafx.h"
#include <Windows.h>
#include <assert.h>
#include <map>
#include "Ram.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
#define RAM_BLK_MAX_FILENAME   64

//////////////////////////////////////////////////////////////////////////
// helper class to store information about allocated blocks
class CMemBlockInfo
{
    char   m_szFileName[RAM_BLK_MAX_FILENAME]; // just to prevent small rapid memory allocation
    int32  m_iLine;
    int32  m_iSize;
    DWORD  m_dwAddress;

public:
    CMemBlockInfo( const char* lpszFile, int32 iLine, int32 iSize, DWORD dwAddress )
    {
        size_t iLen = strlen( lpszFile );
        if( iLen > RAM_BLK_MAX_FILENAME-1 )
            lpszFile += iLen - RAM_BLK_MAX_FILENAME-1; // truncate it a little
        strncpy( m_szFileName, lpszFile, RAM_BLK_MAX_FILENAME-1 );
        m_szFileName[RAM_BLK_MAX_FILENAME-1] = 0;
        m_iLine = iLine;
        m_iSize = iSize;
        m_dwAddress = dwAddress;
    }

    CMemBlockInfo( const CMemBlockInfo& copy )
    {
        strcpy( m_szFileName, copy.m_szFileName );
        m_iLine = copy.m_iLine;
        m_iSize = copy.m_iSize;
        m_dwAddress = copy.m_dwAddress;
    }

    CMemBlockInfo& operator= ( const CMemBlockInfo& copy )
    {
        if( &copy == this )
            *this;
        strcpy( m_szFileName, copy.m_szFileName );
        m_iLine = copy.m_iLine;
        m_iSize = copy.m_iSize;
        m_dwAddress = copy.m_dwAddress;
        return *this;
    }

    ~CMemBlockInfo() { ; }

    inline const char* GetFileName() const { return m_szFileName; }
    inline int32       GetLine() const { return m_iLine; }
    inline int32       GetSize() const { return m_iSize; }
    inline DWORD       GetAddress() const { return m_dwAddress; }
};


//////////////////////////////////////////////////////////////////////////
// Actually does memory management
class CJetRam
{
    typedef map<DWORD, CMemBlockInfo> MEMBLKINFO_MAP;

    MEMBLKINFO_MAP      m_mapInfo;              // contains records about allocated memory blocks
    HANDLE              m_hHeap;                // our internal heap
    DWORD               m_dwNumAllocations;     // total number of allocation per session
    CRITICAL_SECTION    m_CS;                   // MT protector

    inline void Lock()   { EnterCriticalSection( &m_CS ); }
    inline void Unlock() { LeaveCriticalSection( &m_CS ); }

    class CAutoLocker // helper for debug version of API
    {
        CJetRam *m_pOwner;
    public:
        CAutoLocker( CJetRam *pOwner ) { m_pOwner = pOwner; m_pOwner->Lock(); }
        ~CAutoLocker() { m_pOwner->Unlock(); }
    };

    friend class CAutoLocker; // for locking/unlocking only

public:
    CJetRam()
    {
        m_hHeap = HeapCreate( 0, 256*256, 0 ); // let's start with the size same as in crt
        if( !m_hHeap )
        {
            throw( "HeapCreate -> NULL!" ); // FATAL!
        }

        InitializeCriticalSection( &m_CS );
        m_dwNumAllocations = 0;
    }

    ~CJetRam()
    {
        HeapDestroy( m_hHeap );
        Dump();
        m_mapInfo.clear();
        DeleteCriticalSection( &m_CS );
    }

    //////////////////////////////////////////////////////////////////////////
    // Release version doesn't do runtime checks!!!, also not need to lock here, since serialization is ON
    inline void* Allocate( uint32 iSize )
    {
        return HeapAlloc( m_hHeap, 0, iSize );
    }

    inline void* AllocateClear( uint32 iSize )
    {
        return HeapAlloc( m_hHeap, HEAP_ZERO_MEMORY, iSize );
    }

    inline void* Realloc( void *pPtr, uint32 iNewSize )
    {
        return HeapReAlloc( m_hHeap, 0, pPtr, iNewSize );
    }

    inline void  Free( void *pPtr )
    {
        HeapFree( m_hHeap, 0, pPtr );
    }

    //////////////////////////////////////////////////////////////////////////
    // Debug version
    void* DebugAllocate( uint32 iSize, const char* pFile, uint32 iLine )
    {
        CAutoLocker lock( this );

        void *p = Allocate( iSize );

        if( p )
        {
            m_dwNumAllocations++;
            m_mapInfo.insert( MEMBLKINFO_MAP::value_type( DWORD(p), CMemBlockInfo( pFile, iLine, iSize, DWORD(p) ) ) );
        }

        return p;
    }

    void* DebugAllocateClear( uint32 iSize, const char* pFile, uint32 iLine )
    {
        CAutoLocker lock( this );

        void *p = AllocateClear( iSize );

        if( p )
        {
            m_dwNumAllocations++;
            m_mapInfo.insert( MEMBLKINFO_MAP::value_type( DWORD(p), CMemBlockInfo( pFile, iLine, iSize, DWORD(p) ) ) );
        }

        return p;
    }

    void* DebugRealloc( void* pPtr, uint32 iSize, const char* pFile, uint32 iLine )
    {
        CAutoLocker lock( this );

        if( pPtr )
        {
            MEMBLKINFO_MAP::iterator theIter = m_mapInfo.find( DWORD(pPtr) );
            if( theIter == m_mapInfo.end() )
            {
                DebugBreak(); // memory was allocated outside of this module - impossible to continue!!!
            }
            m_mapInfo.erase( theIter );
        }

        void *p = Realloc( pPtr, iSize );
        if( p )
        {
            m_dwNumAllocations++;
            m_mapInfo.insert( MEMBLKINFO_MAP::value_type( DWORD(p), CMemBlockInfo( pFile, iLine, iSize, DWORD(p) ) ) );
        }

        return p;
    }

    void  DebugFree( void *pPtr, const char* pFile, uint32 iLine )
    {
        CAutoLocker lock( this );

        MEMBLKINFO_MAP::iterator theIter = m_mapInfo.find( DWORD(pPtr) );
        if( theIter == m_mapInfo.end() )
        {
            DebugBreak(); // memory was allocated outside of this module - impossible to continue!!!
        }

        m_mapInfo.erase( theIter );
        Free( pPtr );
    }

    void Dump()
    {
        CAutoLocker lock( this );

        DWORD dwTotalLeakSize = 0, i;
        char szBuffer[MAX_PATH];

        MEMBLKINFO_MAP::iterator theIter = m_mapInfo.begin();
        for( i = 0; theIter != m_mapInfo.end(); theIter++, i++ )
        {
            const CMemBlockInfo& info = theIter->second;

            dwTotalLeakSize += info.GetSize();
            sprintf( szBuffer, "%d File: '%s', Line: %d, Size: %d, Ptr: 0x%08X\n", i+1, info.GetFileName() ,info.GetLine(), info.GetSize(), info.GetAddress() );
            
#ifdef _DEBUG
            OutputDebugString( szBuffer );
#else
            jeErrorLog_AddString( JE_ERR_SUBSYSTEM_FAILURE, szBuffer, NULL );
#endif
        }

        sprintf( szBuffer, "\nLeaks dump summary:\n\ttotal number of allocations: %d\n\ttotal leaks: %d\n\ttotal size: %d\n\n", m_dwNumAllocations, i, dwTotalLeakSize );

#ifdef _DEBUG
        OutputDebugString( szBuffer );
#else
        jeErrorLog_AddString( JE_ERR_SUBSYSTEM_FAILURE, szBuffer, NULL );
#endif
    }
};

static CJetRam g_Ram; // only 1 instance

#ifdef _DEBUG
    #define CHECK_RELEASE() \
        MessageBox( NULL, \
        "DO NOT USE RELEASE-MODE MEMORY ALLOCATION ROUTINES IN DEBUG BUILD!!!\n\nUSE JE_RAM_ MACROS INSTEAD!!!\n\n" \
        "REFER TO Ram.h\n\nTHIS IS MANDATORY RECOMENDATION!", \
        "JET3D DEVELOPMENT TEAM", MB_OK ); \
        DebugBreak();
#else
    #define CHECK_RELEASE   __noop
#endif

//////////////////////////////////////////////////////////////////////////
// Release version
JETAPI void * JETCC jeRam_Allocate( uint32 iSize )        // allocate memory
{
    CHECK_RELEASE();
    return g_Ram.Allocate( iSize );
}

JETAPI void * JETCC jeRam_AllocateClear( uint32 iSize )   // allocate the ram & clear it. (calloc)
{
    CHECK_RELEASE();
    return g_Ram.AllocateClear( iSize );
}

JETAPI void * JETCC jeRam_Realloc( void *pPtr, uint32 iNewSize )
{
    CHECK_RELEASE();
    if( !pPtr )
        return g_Ram.Allocate( iNewSize );
    return g_Ram.Realloc( pPtr, iNewSize );
}

JETAPI void   JETCC jeRam_Free( void *pPtr )              // Free an allocated memory block
{
    CHECK_RELEASE();
    g_Ram.Free( pPtr );
}

//////////////////////////////////////////////////////////////////////////
// Debug version

JETAPI void*  JETCC jeRam_DebugAllocate( uint32 iSize, const char* pFile, uint32 iLine )
{
    assert(iSize); assert(pFile); assert(iLine);

    return g_Ram.DebugAllocate( iSize, pFile, iLine );
}

JETAPI void*  JETCC jeRam_DebugAllocateClear( uint32 iSize, const char* pFile, uint32 iLine )
{
    assert(iSize); assert(pFile); assert(iLine);

    return g_Ram.DebugAllocateClear( iSize, pFile, iLine );
}

JETAPI void * JETCC jeRam_DebugRealloc( void* pPtr, uint32 iSize, const char* pFile, uint32 iLine )
{
    assert(iSize); assert(pFile); assert(iLine);
    if( !pPtr )
        return g_Ram.DebugAllocate( iSize, pFile, iLine );
    return g_Ram.DebugRealloc( pPtr, iSize, pFile, iLine );
}

JETAPI void   JETCC jeRam_DebugFree( void *pPtr, const char* pFile, uint32 iLine )
{
    assert(pFile); assert(iLine);
    if( pPtr )
        g_Ram.DebugFree( pPtr, pFile, iLine );
}

JETAPI void   JETCC jeRam_ShowStats( void *fpFile )
{
    g_Ram.Dump();
}

JETAPI jeBoolean JETCC jeRam_IsValidPtr( void *pPtr )
{
    if( !IsBadReadPtr( pPtr, 4 ) && !IsBadWritePtr( pPtr, 4 ) )
        return JE_TRUE;
    return JE_FALSE;
}
