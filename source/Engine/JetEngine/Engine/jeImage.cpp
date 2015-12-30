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
#include "jeImage.h"
#include "FreeImage.h"
#include "DCommon.h"
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

typedef struct jeImage
{
	FIBITMAP *Bmp;
	jeTexture *pDriverTexture;
} jeImage;

JETAPI jeImage * JETCC jeImage_Create(int32 Width, int32 Height, int32 BPP)
{
	jeImage *pImg = new jeImage;
	memset(pImg, 0, sizeof(jeImage));

	pImg->Bmp = FreeImage_Allocate(Width, Height, BPP);
	if (!pImg->Bmp)
	{
		JE_SAFE_DELETE(pImg);
		return NULL;
	}

	return pImg;
}

JETAPI jeImage * JETCC jeImage_CreateFromFile(jeVFile *File)
{
	FreeImageIO io;
	FREE_IMAGE_FORMAT fif;
	jeImage *pImg = NULL;

	memset(&io, 0, sizeof(FreeImageIO));

	io.read_proc = (FI_ReadProc)free_image_read;
	io.seek_proc = (FI_SeekProc)free_image_seek;
	io.tell_proc = (FI_TellProc)free_image_tell;
	io.write_proc = (FI_WriteProc)free_image_write;

	fif = FreeImage_GetFileTypeFromHandle(&io, (fi_handle)File);
	if (fif == FIF_UNKNOWN)
		return NULL;

	pImg = new jeImage;
	pImg->Bmp = FreeImage_LoadFromHandle(fif, &io, (fi_handle)File);
	if (!pImg->Bmp)
	{
		JE_SAFE_DELETE(pImg);
		return NULL;
	}

	// No more 8-Bit or 16-Bit bitmaps
	if (FreeImage_GetBPP(pImg->Bmp) < 24)
	{
		FIBITMAP *conv = NULL;

		conv = FreeImage_ConvertTo32Bits(pImg->Bmp);
		FreeImage_Unload(pImg->Bmp);
		pImg->Bmp = conv;
	}

	FreeImage_FlipVertical(pImg->Bmp);
	pImg->pDriverTexture = NULL;

	return pImg;
}

JETAPI void JETCC jeImage_Destroy(jeImage **pImage)
{
	FreeImage_Unload((*pImage)->Bmp);
	
	(*pImage)->Bmp = NULL;
	(*pImage)->pDriverTexture = NULL;

	JE_SAFE_DELETE((*pImage));
}

JETAPI int32 JETCC jeImage_GetWidth(jeImage *pImage)
{
	assert(pImage);

	return (int32)FreeImage_GetWidth(pImage->Bmp);
}

JETAPI int32 JETCC jeImage_GetHeight(jeImage *pImage)
{
	assert(pImage);

	return (int32)FreeImage_GetHeight(pImage->Bmp);
}

JETAPI int32 JETCC jeImage_GetBPP(jeImage *pImage)
{
	assert(pImage);

	return (int32)FreeImage_GetBPP(pImage->Bmp);
}

JETAPI uint8 *jeImage_GetBits(jeImage *pImage)
{
	assert(pImage);
	assert(pImage->Bmp);

	return (uint8*)FreeImage_GetBits(pImage->Bmp);
}
