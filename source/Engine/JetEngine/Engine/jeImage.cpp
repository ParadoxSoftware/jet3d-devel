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
#include <string.h>
#include "Ram.h"
#include "jeImageImpl.h"
#include "ErrorLog.h"
//#include "debug_new.h"

/*jeImageImpl::jeImageImpl()
{
	m_pData = nullptr;
	m_pTexture = nullptr;
	m_Width = 0;
	m_Height = 0;
	m_ByteDepth = 0;
	m_ImageSize = 0;

	m_iRefCount = 1;
	m_strType = "Bitmap";
	m_strFileName = "BitmapFileName";
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

bool jeImageImpl::isDirty()
{
	return false;
}

bool jeImageImpl::isLoaded()
{
	return true;
}

jeBoolean jeImageImpl::Create(int32 Width, int32 Height, int32 BPP)
{
	if (m_pData != nullptr)
		JE_SAFE_DELETE_ARRAY(m_pData);

	m_Width = Width;
	m_Height = Height;
	m_ByteDepth = BPP;
	m_ImageSize = Width * Height * (BPP / 8);

	m_pData = new uint8[m_ImageSize];
	if (!m_pData)
		return JE_FALSE;

	return JE_TRUE;
}


void jeImageImpl::Destroy()
{
	m_pTexture = nullptr;
	JE_SAFE_DELETE_ARRAY(m_pData);
}

const int32 jeImageImpl::GetWidth() const
{
	return m_Width;
}

const int32 jeImageImpl::GetHeight() const
{
	return m_Height;
}

const int32 jeImageImpl::GetBPP() const
{
	return m_ByteDepth;
}

const int32 jeImageImpl::GetImageSize() const
{
	return m_ImageSize;
}

void jeImageImpl::SetBits(uint8 *pBits)
{
	memset(m_pData, 0, m_ImageSize);
	memcpy(m_pData, pBits, m_ImageSize);
}

uint8 * jeImageImpl::GetBits()
{
	return m_pData;
}

void jeImageImpl::SetTextureHandle(jeTexture *Texture)
{
	m_pTexture = Texture;
}

jeTexture * jeImageImpl::GetTextureHandle()
{
	return m_pTexture;
}

jet3d::jeResource *jeImage_BMP::Load(jeVFile *pFile)
{
	jeImage_BMP::BitmapFileHeader bfh;
	jeImage_BMP::BitmapInfoHeader bih;
	uint8 *data = nullptr;

	if (!pFile)
		return nullptr;

	// Read the bitmap file header
	jeVFile_Read(pFile, &bfh, sizeof(jeImage_BMP::BitmapFileHeader));

	// Make sure this is a bitmap
	if (bfh.bfType != 0x4D42)
		return nullptr;

	// Read the info header
	jeVFile_Read(pFile, &bih, sizeof(jeImage_BMP::BitmapInfoHeader));

	// Create the image shell
	jeImageImpl *pImage = new jeImageImpl();
	pImage->Create(bih.biWidth, bih.biHeight, bih.biBitCount / 8);

	// Allocate the bits
	data = new uint8[pImage->GetImageSize()];

	// Move to the bitmap bits
	jeVFile_Seek(pFile, bfh.bfOffBits, JE_VFILE_SEEKSET);

	// Read the bits
	jeVFile_Read(pFile, data, pImage->GetImageSize());

	// Set the image bits
	pImage->SetBits(data);
	JE_SAFE_DELETE_ARRAY(data);

	return pImage;
}

jeBoolean jeImage_BMP::Save(jeVFile *pFile, jet3d::jeResource *pResource)
{
	return JE_TRUE;
}

*/
