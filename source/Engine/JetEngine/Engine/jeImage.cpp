/*!
	@file jeImage.cpp
	
	@author Anthony Rufrano (paradoxnj)
	@brief Replacement for jeBitmap using FreeImage

	@par Licence
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
#include <assert.h>
#include "jeImageImpl.h"

extern "C" {
#include "FreeImage.h"
}

#include "ErrorLog.h"
#include "debug_new.h"

static unsigned free_image_read(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	jeVFile					*f = (jeVFile*)handle;
	long					pos, newpos;

	jeVFile_Tell(f, &pos);
	jeVFile_Read(f, buffer, size * count);
	jeVFile_Tell(f, &newpos);

	return newpos - pos;
}

static unsigned free_image_write(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	jeVFile					*f = (jeVFile*)handle;
	long					pos, newpos;

	jeVFile_Tell(f, &pos);
	jeVFile_Write(f, buffer, size * count);
	jeVFile_Tell(f, &newpos);

	return 1;
}

static int free_image_seek(fi_handle handle, long offset, int origin)
{
	jeVFile					*f = (jeVFile*)handle;
	jeVFile_Whence			whence;

	switch (origin)
	{
	case SEEK_SET:
		{
			whence = JE_VFILE_SEEKSET;
			break;
		}
	case SEEK_CUR:
		{
			whence = JE_VFILE_SEEKCUR;
			break;
		}
	case SEEK_END:
		{
			whence = JE_VFILE_SEEKEND;
			break;
		}
	default:
		return -1;
	}

	return jeVFile_Seek(f, offset, whence);
}

static long free_image_tell(fi_handle handle)
{
	jeVFile						*f = (jeVFile*)handle;
	long						pos;

	jeVFile_Tell(f, &pos);
	return pos;
}

jeImageImpl::jeImageImpl()
{
	m_pBitmap = NULL;
	m_pTexture = NULL;

	m_iRefCount = 1;
}

jeImageImpl::~jeImageImpl()
{
	Destroy();
}

uint32 jeImageImpl::AddRef()
{
	m_iRefCount++;
	return m_iRefCount;
}

uint32 jeImageImpl::Release()
{
	m_iRefCount--;
	if (m_iRefCount <= 0)
	{
		delete this;
		return 0;
	}

	return m_iRefCount;
}

jeBoolean jeImageImpl::Create(int32 Width, int32 Height, int32 BPP)
{
	if (m_pBitmap != NULL)
		return JE_FALSE;

	m_pBitmap = FreeImage_Allocate(Width, Height, BPP);
	if (!m_pBitmap)
		return JE_FALSE;

	return JE_TRUE;
}

jeBoolean jeImageImpl::CreateFromFile(jeVFile *File)
{
	FreeImageIO io;
	FREE_IMAGE_FORMAT fif;
	char fileName[_MAX_PATH];

	memset(&io, 0, sizeof(FreeImageIO));

	jeVFile_GetName(File, fileName, _MAX_PATH);
	jeErrorLog_AddString(-1, fileName, NULL);

	io.read_proc = (FI_ReadProc)free_image_read;
	io.seek_proc = (FI_SeekProc)free_image_seek;
	io.tell_proc = (FI_TellProc)free_image_tell;
	io.write_proc = (FI_WriteProc)free_image_write;

	fif = FreeImage_GetFileTypeFromHandle(&io, (fi_handle)File);
	if (fif == FIF_UNKNOWN)
		return NULL;

	m_pBitmap = FreeImage_LoadFromHandle(fif, &io, (fi_handle)File);
	if (!m_pBitmap)
		return JE_FALSE;

	// No more 8-Bit or 16-Bit bitmaps
	if (FreeImage_GetBPP(m_pBitmap) < 24)
	{
		FIBITMAP *conv = NULL;

		conv = FreeImage_ConvertTo32Bits(m_pBitmap);
		FreeImage_Unload(m_pBitmap);
		m_pBitmap = conv;
	}

	FreeImage_FlipVertical(m_pBitmap);
	
	return JE_TRUE;
}

void jeImageImpl::Destroy()
{
	m_pTexture = NULL;
	FreeImage_Unload(m_pBitmap);
}

const int32 jeImageImpl::GetWidth() const
{
	assert(m_pBitmap);

	return (int32)FreeImage_GetWidth(m_pBitmap);
}

const int32 jeImageImpl::GetHeight() const
{
	assert(m_pBitmap);

	return (int32)FreeImage_GetHeight(m_pBitmap);
}

const int32 jeImageImpl::GetBPP() const
{
	assert(m_pBitmap);

	return (int32)FreeImage_GetBPP(m_pBitmap);
}

uint8 * jeImageImpl::GetBits()
{
	assert(m_pBitmap);

	return (uint8*)FreeImage_GetBits(m_pBitmap);
}

void jeImageImpl::SetTextureHandle(jeTexture *Texture)
{
	m_pTexture = Texture;
}

jeTexture * jeImageImpl::GetTextureHandle()
{
	return m_pTexture;
}
