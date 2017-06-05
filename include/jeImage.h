/*!
	@file jeImage.h 
	
	@author Anthony Rufrano (paradoxnj)
	@brief Replacement for jeBitmap supporting more formats

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
#ifndef __JE_IMAGE_H__
#define __JE_IMAGE_H__

#include <string>
#include "Basetype.h"
#include "VFILE.H"
#include "jeResourceMgr.h"

typedef struct jeTexture					jeTexture;

class jeImage : virtual public jet3d::jeResource
{
protected:
	virtual ~jeImage(){}

public:
	virtual jeBoolean Create(int32 Width, int32 Height, int32 BPP) = 0;
	virtual void Destroy() = 0;

	virtual const int32 GetWidth() const = 0;
	virtual const int32 GetHeight() const = 0;
	virtual const int32 GetBPP() const = 0;
	virtual const int32 GetImageSize() const = 0;
	
	virtual void SetBits(uint8 *pBits) = 0;
	virtual uint8 * GetBits() = 0;

	virtual void SetTextureHandle(jeTexture *Texture) = 0;
	virtual jeTexture * GetTextureHandle() = 0;
};

#endif
