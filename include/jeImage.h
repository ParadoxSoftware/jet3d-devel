/*!
	@file jeImage.h 
	
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
#ifndef __JE_IMAGE_H__
#define __JE_IMAGE_H__

#include "Basetype.h"
#include "VFILE.H"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct jeImage						jeImage;
typedef struct jeTexture					jeTexture;

JETAPI jeImage * JETCC jeImage_Create(int32 Width, int32 Height, int32 BPP);
JETAPI jeImage * JETCC jeImage_CreateFromFile(jeVFile *File);
JETAPI void JETCC jeImage_Destroy(jeImage **pImage);

JETAPI int32 JETCC jeImage_GetWidth(const jeImage *pImage);
JETAPI int32 JETCC jeImage_GetHeight(const jeImage *pImage);
JETAPI int32 JETCC jeImage_GetBPP(const jeImage *pImage);
JETAPI uint8 * JETCC jeImage_GetBits(const jeImage *pImage);

JETAPI void JETCC jeImage_SetTextureHandle(jeImage *Image, jeTexture *Texture);
JETAPI jeTexture * JETCC jeImage_GetTextureHandle(const jeImage *Image);

#ifdef __cplusplus
}
#endif

#endif
