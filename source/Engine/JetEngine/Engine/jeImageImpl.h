/*!
	@file jeImageImpl.h 
	
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
#ifndef __JE_IMAGE_IMPL_H__
#define __JE_IMAGE_IMPL_H__

#include "jeImage.h"

class jeImageImpl : public jeImage
{
public:
	jeImageImpl();
	virtual ~jeImageImpl();

protected:
	uint32 m_iRefCount;
	jeTexture *m_pTexture;

	uint8 *m_pData;
	int32 m_Width;
	int32 m_Height;
	int32 m_ByteDepth;
	int32 m_ImageSize;

	std::string m_strFileName;

public:
	uint32 AddRef();
	uint32 Release();

	jeBoolean IsDirty();
	const std::string &GetFileName();
	jet3d::jeResource *MakeCopy();

	jeBoolean Create(int32 Width, int32 Height, int32 BPP);
	void Destroy();

	const int32 GetWidth() const;
	const int32 GetHeight() const;
	const int32 GetBPP() const;
	const int32 GetImageSize() const;
	
	void SetBits(uint8 *pBits);
	uint8 * GetBits();
	
	void SetTextureHandle(jeTexture *Texture);
	jeTexture * GetTextureHandle();
};

class jeImage_BMP : public jet3d::jeResourceFactory
{
public:
	jeImage_BMP() { m_strFileExtension = "bmp"; }
	virtual ~jeImage_BMP() {}

protected:
	typedef struct _BitmapFileHeader
	{
		uint16 bfType;
		uint32 bfSize;
		uint16 bfReserved1;
		uint16 bfReserved2;
		uint32 bfOffBits;
	} BitmapFileHeader;

	typedef struct _BitmapInfoHeader
	{
		uint32 biSize;
		int32 biWidth;
		int32 biHeight;
		uint16 biPlanes;
		uint16 biBitCount;
		uint32 biCompression;
		uint32 biSizeImage;
		int32 biXPelsPerMeter;
		int32 biYPelsPerMeter;
		uint32 biClrUsed;
		uint32 biClrImportant;
	} BitmapInfoHeader;

	std::string m_strFileExtension;

public:
	jet3d::jeResource *Load(jeVFile *pFile);
	jeBoolean Save(jeVFile *pFile, jet3d::jeResource *pResource);

	const std::string &GetFileExtension() { return m_strFileExtension; }
};

#endif