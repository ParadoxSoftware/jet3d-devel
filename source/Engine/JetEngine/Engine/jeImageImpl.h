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

#include "FreeImage.h"
#include "jeImage.h"

class jeImageImpl : public jeImage
{
public:
	jeImageImpl();
	virtual ~jeImageImpl();

protected:
	uint32 m_iRefCount;
	FIBITMAP *m_pBitmap;
	jeTexture *m_pTexture;

public:
	uint32 AddRef();
	uint32 Release();

	jeBoolean Create(int32 Width, int32 Height, int32 BPP);
	jeBoolean CreateFromFile(jeVFile *File);
	void Destroy();

	const int32 GetWidth() const;
	const int32 GetHeight() const;
	const int32 GetBPP() const;
	uint8 * GetBits();

	void SetTextureHandle(jeTexture *Texture);
	jeTexture * GetTextureHandle();
};

#endif