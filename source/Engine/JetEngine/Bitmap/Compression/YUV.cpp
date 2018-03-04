/****************************************************************************************/
/*  Yuv                                                                                 */
/*                                                                                      */
/*  Author: Charles Bloom                                                               */
/*  Description:  YUV <-> RGB code                                                      */
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
#include <assert.h>

#include "YUV.h"
//#include "Utility.h"
#include "Cpu.h"

#pragma warning(disable : 4244)	// int -> uint8 conversions abound

#pragma warning(disable : 4799)	// I know we've got no emms; it's done in wavelet.c

/*}{******* RGB <-> YUV in C ***********/

void RGBb_to_YUVb(const uint8 *RGB,uint8 *YUV)
{
int R = RGB[0], G = RGB[1], B = RGB[2];

	YUV[0] = Y_RGB(R,G,B);
	YUV[1] = U_RGB(R,G,B) + 127;
	YUV[2] = V_RGB(R,G,B) + 127;
}

void YUVb_to_RGBb(const uint8 *YUV,uint8 *RGB)
{
int y,u,v,r,g,b;

	y = YUV[0];
	u = YUV[1] - 127;
	v = YUV[2] - 127;

	r = R_YUV(y,u,v);
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);

	RGB[0] = JE_MINMAX(r,0,255);	// we could get negative ones and whatnot
	RGB[1] = JE_MINMAX(g,0,255);	//	because the y,u,v are not really 24 bits;
	RGB[2] = JE_MINMAX(b,0,255);	//	there are regions of YUV space that will never be reached by RGBb_to_YUVb
}


void RGBb_to_YUVb_line(const uint8 *RGB,uint8 *YUV,int len)
{
int R,G,B;

	while(len--)
	{
		R = *RGB++;
		G = *RGB++;
		B = *RGB++;
		*YUV++ = Y_RGB(R,G,B);
		*YUV++ = U_RGB(R,G,B) + 127;
		*YUV++ = V_RGB(R,G,B) + 127;
	}
}

void YUVb_to_RGBb_line(const uint8 *YUV,uint8 *RGB,int len)
{
int y,u,v,r,g,b;

	while(len--)
	{
		y = (*YUV++);
		u = (*YUV++) - 127;
		v = (*YUV++) - 127;

		r = R_YUV(y,u,v);
		g = G_YUV(y,u,v);
		b = B_YUV(y,u,v);

		*RGB++ = JE_MINMAX(r,0,255);	// we could get negative ones and whatnot
		*RGB++ = JE_MINMAX(g,0,255);	//	because the y,u,v are not really 24 bits;
		*RGB++ = JE_MINMAX(b,0,255);	//	there are regions of YUV space that will never be reached by RGBb_to_YUVb
	}
}


void RGBb_to_YUVi(const uint8 *RGB,int *Y,int *U,int *V)
{
int R = RGB[0], G = RGB[1], B = RGB[2];

	*Y = Y_RGB(R,G,B);
	*U = U_RGB(R,G,B) + 127;
	*V = V_RGB(R,G,B) + 127;

	assert( JE_ISINRANGE(*Y,0,255) );
	assert( JE_ISINRANGE(*U,0,255) );
	assert( JE_ISINRANGE(*V,0,255) );
}

void YUVi_to_RGBb(int y,int u,int v,uint8 *RGB)
{
int r,g,b;

// yuv can be kicked out of 0,255 by the wavelet
//	assert( JE_ISINRANGE(y,0,255) );
//	assert( JE_ISINRANGE(u,0,255) );
//	assert( JE_ISINRANGE(v,0,255) );

	u -= 127;
	v -= 127;
	r = R_YUV(y,u,v); // this is just like a matrix multiply
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);
	RGB[0] = JE_MINMAX(r,0,255);	// we could get negative ones and whatnot
	RGB[1] = JE_MINMAX(g,0,255);	//	because the y,u,v are not really 24 bits;
	RGB[2] = JE_MINMAX(b,0,255);	//	there are regions of YUV space that will never be reached by RGBb_to_YUVb
}

void RGBi_to_YUVi(int R,int G,int B,int *Y,int *U,int *V)
{
	assert( JE_ISINRANGE(R,0,255) );
	assert( JE_ISINRANGE(G,0,255) );
	assert( JE_ISINRANGE(B,0,255) );

	*Y = Y_RGB(R,G,B);
	*U = U_RGB(R,G,B) + 127;
	*V = V_RGB(R,G,B) + 127;

	assert( JE_ISINRANGE(*Y,0,255) );
	assert( JE_ISINRANGE(*U,0,255) );
	assert( JE_ISINRANGE(*V,0,255) );
}

void YUVi_to_RGBi(int y,int u,int v,int *R,int *G,int *B)
{
int r,g,b;

// yuv can be kicked out of 0,255 by the wavelet
//	assert( JE_ISINRANGE(y,0,255) );
//	assert( JE_ISINRANGE(u,0,255) );
//	assert( JE_ISINRANGE(v,0,255) );

	u -= 127;
	v -= 127;
	r = R_YUV(y,u,v); // this is just like a matrix multiply
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);

	*R = JE_MINMAX(r,0,255);	// we could get negative ones and whatnot
	*G = JE_MINMAX(g,0,255);	//	because the y,u,v are not really 24 bits;
	*B = JE_MINMAX(b,0,255);	//	there are regions of YUV space that will never be reached by RGBb_to_YUVb
}

void YUVi_to_RGBi_line(int *line1,int *line2,int *line3,int len)
{
int y,u,v,r,g,b;

	// <> use MMX

	/*cachetouch_w(line1,len>>3);
	cachetouch_w(line2,len>>3);
	cachetouch_w(line3,len>>3);*/
	while(len--)
	{
		y = *line1;
		u = *line2 - 127;
		v = *line3 - 127;

		r = R_YUV(y,u,v);
		g = G_YUV(y,u,v);
		b = B_YUV(y,u,v);

		r = JE_MINMAX(r,0,255);
		g = JE_MINMAX(g,0,255);
		b = JE_MINMAX(b,0,255);

		*line1++ = r;
		*line2++ = g;
		*line3++ = b;
	}
}

void YUVi_to_BGRb_line_c(int *iline1,int *iline2,int *iline3,uint8 * ibline,int ilen)
{
int y,u,v,r,g,b,len;
int *line1,*line2,*line3;
uint8 * bline;

	line1 = iline1;
	line2 = iline2;
	line3 = iline3;
	bline = ibline;
	len = ilen;

	/*cachetouch_r(line1,len>>3);
	cachetouch_r(line2,len>>3);
	cachetouch_r(line3,len>>3);
	cachetouch_w(bline,(len*3)>>5);*/
	
	while(len--)
	{
		y = (*line1++);
		u = (*line2++) - 127;
		v = (*line3++) - 127;

		r = R_YUV(y,u,v);
		g = G_YUV(y,u,v);
		b = B_YUV(y,u,v);

		r = JE_MINMAX(r,0,255);
		g = JE_MINMAX(g,0,255);
		b = JE_MINMAX(b,0,255);

		bline[0] = b;
		bline[1] = g;
		bline[2] = r;
		bline+=3;
	}
}

void YUVi_to_BGRb_lines_c(int w,int h,int **Ylines,int **Ulines,int **Vlines,uint8 * BGRptr,int BGRstride)
{
int yz;
	for(yz=0;yz<h;yz++)
	{	
	int y,u,v,r,g,b,len;
	int *line1,*line2,*line3;
	uint8 * bline;

		line1 = Ylines[yz];
		line2 = Ulines[yz];
		line3 = Vlines[yz];
		bline = BGRptr;
		len = w;

		/*cachetouch_r(line1,len>>3);
		cachetouch_r(line2,len>>3);
		cachetouch_r(line3,len>>3);
		cachetouch_w(bline,(len*3)>>5);*/
		
		while(len--)
		{
			y = (*line1++);
			u = (*line2++) - 127;
			v = (*line3++) - 127;

			r = R_YUV(y,u,v);
			g = G_YUV(y,u,v);
			b = B_YUV(y,u,v);

			r = JE_MINMAX(r,0,255);
			g = JE_MINMAX(g,0,255);
			b = JE_MINMAX(b,0,255);

			bline[0] = b;
			bline[1] = g;
			bline[2] = r;
			bline+=3;
		}

		BGRptr += BGRstride;
	}
}

void YUVi_to_XRGB_line_c(int *iline1,int *iline2,int *iline3,uint8 * ibline,int ilen)
{
int y,u,v,r,g,b,len;
int *line1,*line2,*line3;
uint8 * bline;

	line1 = iline1;
	line2 = iline2;
	line3 = iline3;
	bline = ibline;
	len = ilen;

	/*cachetouch_r(line1,len>>3);
	cachetouch_r(line2,len>>3);
	cachetouch_r(line3,len>>3);
	cachetouch_w(bline,len>>3);*/
	
	while(len--)
	{
		y = (*line1++);
		u = (*line2++) - 127;
		v = (*line3++) - 127;

		r = R_YUV(y,u,v);
		g = G_YUV(y,u,v);
		b = B_YUV(y,u,v);

		r = JE_MINMAX(r,0,255);
		g = JE_MINMAX(g,0,255);
		b = JE_MINMAX(b,0,255);

		bline[0] = b;
		bline[1] = g;
		bline[2] = r;
		bline += 4;
	}
}

/*}{******* MMX YUV -> BGR blitters ***********/

static const __int64 Const_V_16 = 0x0000A6462DB50000;
static const __int64 Const_U_16 = 0x00000000E9FA7168;

/*void YUVi_to_BGRb_lines_mmx(int w,int h,int **Ylines,int **Ulines,int **Vlines,uint8 * BGRptr,int BGRstride)
{
int yz;

	for(yz=0;yz<h;yz++)
	{
	int *line1,*line2,*line3;
	uint8 * bline;

		line1 = Ylines[yz];
		line2 = Ulines[yz];
		line3 = Vlines[yz];
		bline = BGRptr;
		BGRptr += BGRstride;

		assert(w > 1 && h > 1 );

		//cachetouch_r(line1,w>>3);
		//cachetouch_r(line2,w>>3);
		//cachetouch_r(line3,w>>3);
		//cachetouch_w(bline,(w*3)>>5);
			
		__asm
		{
		mov ecx,w
		sub ecx,1
		mov edi,bline

		movq mm3,Const_V_16
		movq mm4,Const_U_16

		More:		

			//**
			//*
			//*	ecx is width
			//*	edi is BGRptr
			//*
			//*	eax is (V<<2)-509
			//*	ebx is (U<<2)-509
			//*	edx is Y
			//*
			//*	the multiply coefficients are in 14 bits, then we rshr 16 via mulhw
			//*
			//*	mm0 is four V int16's, multiplied by their coefficients (mm3)
			//*	mm1 is four U int16's, multiplied by their coefficients (mm4)
			//*	mm2 is four Y int16's
			//*
			//*	XRGB = mm0 + mm1 + mm2
			//*
			//*	we're taking about 45 clocks
			//*	my manual count indicates we could take about 37 if we were perfect
			

			//*
			//*
			//* MMX optimization notes:
			//*	1. there is only one MMX pack/unpack unit
			//*	2. there is only one MMX multiply unit
			//*	3. MMX instructions that use memory or integers use port 0 only
			//*	4. all MMX instructions are 1 clock except multiply, which is 3
			//

			mov			eax,line3	// V
			mov			eax,[eax]	// eax = v; hard stall on eax, inevitable
			add			line3,4		// no stall on line3

			shl			eax,2		// V<<=2
			
			mov			ebx,line2	// U

			sub			eax,509		// do ((V<<2)-510) instead of ((V-127)<<2)

			mov			ebx,[ebx]	// ebx = u
			add			line2,4

			movd		mm0,eax		// mm0 = [0][v]
			
			shl			ebx,2

			punpckldq	mm0,mm0		// mm0 = [v][v]
			
			sub			ebx,509

			packssdw	mm0,mm0		// mm0 = [v][v][v][v]
			
			movd		mm1,ebx		// mm1 = [0][u]

			mov			edx,line1	// Y

			pmulhw		mm0,mm3		// keep only high words; same as multiplying in 32 bits and doing >>16
			
			// put some non-dependent stuff after the multiply:

			mov			edx,[edx]	// edx = y			

			punpckldq	mm1,mm1		// mm1 = [u][u]

			movd		mm2,edx		// mm2 = [0][y]

			packssdw	mm1,mm1		// mm1 = [u][u][u][u]			

			// these two packs cannot pair!

			punpckldq	mm2,mm2		// mm2 = [y][y]
			
			pmulhw		mm1,mm4

			// put some stuff after the multiply:

			add			line1,4
			packssdw	mm2,mm2		// mm2 = [y][y][y][y]

			// now XRGB = mm0 + mm1 + mm2

			paddsw		mm0,mm1

			paddsw		mm0,mm2		// hard stall on mm0, inevitable ; no stall on mm2

			// convert the four int16s to eight bytes; also do a clamp(0,255) for free!

			packuswb	mm0,mm0		// hard stall on mm0, inevitable

			movd		[edi],mm0	// hard stall on mm0, then unaligned write! bad!
			add			edi,3		// no stall on edi

		dec ecx
		jnz More

			//{		one last one that doesn't write 4->3
			mov eax,line3			// V
			mov eax,[eax]
			add line3,4

			shl			eax,2
			sub			eax,509
			movd		mm0,eax		// mm0 = [0][x]
			punpckldq	mm0,mm0		// mm0 = [x][x]
			packssdw	mm0,mm0		// mm0 = [x][x][x][x]
			pmulhw		mm0,mm3
			
			mov ebx,line2			// U
			mov ebx,[ebx]
			add line2,4

			shl			ebx,2
			sub			ebx,509
			movd		mm1,ebx		// mm0 = [0][x]
			punpckldq	mm1,mm1		// mm0 = [x][x]
			packssdw	mm1,mm1		// mm0 = [x][x][x][x]
			pmulhw		mm1,mm4

			mov edx,line1			// Y
			mov edx,[edx]
			add line1,4

			movd		mm2,edx		// mm0 = [0][x]
			punpckldq	mm2,mm2		// mm0 = [x][x]
			packssdw	mm2,mm2		// mm0 = [x][x][x][x]

			paddsw		mm0,mm1
			paddsw		mm0,mm2

			packuswb	mm0,mm0

			movd		eax,mm0		// eax is XRGB
			mov			[edi],ax
			shr			eax,16
			mov			[edi+2],al
			//}
		}
	}

	//__asm { emms }	
}*/

/*void YUVi_to_BGRb_line_mmx2(int *line1,int *line2,int *line3,uint8 * bline,int len)
{
	assert(len > 1 );

	len --;

	//cachetouch_r(line1,len>>3);
	//cachetouch_r(line2,len>>3);
	//cachetouch_r(line3,len>>3);
	//cachetouch_w(bline,(len*3)>>5);
	
	__asm
	{
	
	mov ecx,len
	mov edi,bline

	movq mm3,Const_V_16
	movq mm4,Const_U_16

	YUVi_to_BGRb_line_mmx2_More:		

		mov			eax,line3			// V
		mov			eax,[eax]			// hard stall on eax, inevitable
		add			line3,4				// no stall on line3

		shl			eax,2
		
		mov			ebx,line2			// U

		sub			eax,510

		mov			ebx,[ebx]
		add			line2,4

		movd		mm0,eax		// mm0 = [0][x]
		
		shl			ebx,2

		punpckldq	mm0,mm0		// mm0 = [x][x]
		
		sub			ebx,510

		packssdw	mm0,mm0		// mm0 = [x][x][x][x]
		
		movd		mm1,ebx		// mm0 = [0][x]

		pmulhw		mm0,mm3
		mov			edx,line1			// Y
		
		punpckldq	mm1,mm1		// mm0 = [x][x]

		mov			edx,[edx]

		packssdw	mm1,mm1		// mm0 = [x][x][x][x]
				
		movd		mm2,edx		// mm0 = [0][x]

		add			line1,4

		punpckldq	mm2,mm2		// mm0 = [x][x]
		
		pmulhw		mm1,mm4

		packssdw	mm2,mm2		// mm0 = [x][x][x][x]

		paddsw		mm0,mm1

		paddsw		mm0,mm2		// hard stall on mm0, inevitable ; no stall on mm2

		packuswb	mm0,mm0		// hard stall on mm0, inevitable

		movd		[edi],mm0	// unaligned write! bad!
		add			edi,3		// no stall on edi

	dec ecx
	jnz YUVi_to_BGRb_line_mmx2_More

	mov bline,edi

	//emms
	}

	{
	int y,u,v,r,g,b;	
	y = (*line1);
	u = (*line2) - 127;
	v = (*line3) - 127;

	r = R_YUV(y,u,v);
	g = G_YUV(y,u,v);
	b = B_YUV(y,u,v);

	r = JE_MINMAX(r,0,255);
	g = JE_MINMAX(g,0,255);
	b = JE_MINMAX(b,0,255);

	bline[0] = b;
	bline[1] = g;
	bline[2] = r;
	}
}*/

/*void YUVi_to_XRGB_line_mmx(int *line1,int *line2,int *line3,uint8 * bline,int len)
{
	assert(len > 0 );

	//cachetouch_r(line1,len>>3);
	//cachetouch_r(line2,len>>3);
	//cachetouch_r(line3,len>>3);
	//cachetouch_w(bline,len>>3);

	__asm
	{
	
	mov ecx,len
	mov edi,bline

	movq mm3,Const_V_16
	movq mm4,Const_U_16

	More:		

		mov			eax,line3			// V
		mov			eax,[eax]			// hard stall on eax, inevitable
		add			line3,4				// no stall on line3

		shl			eax,2
		
		mov			ebx,line2			// U

		sub			eax,510

		mov			ebx,[ebx]
		add			line2,4

		movd		mm0,eax		// mm0 = [0][x]
		
		shl			ebx,2

		punpckldq	mm0,mm0		// mm0 = [x][x]
		
		sub			ebx,510

		packssdw	mm0,mm0		// mm0 = [x][x][x][x]
		
		movd		mm1,ebx		// mm0 = [0][x]

		pmulhw		mm0,mm3
		mov			edx,line1			// Y
		
		punpckldq	mm1,mm1		// mm0 = [x][x]

		mov			edx,[edx]

		packssdw	mm1,mm1		// mm0 = [x][x][x][x]
				
		movd		mm2,edx		// mm0 = [0][x]

		add			line1,4

		punpckldq	mm2,mm2		// mm0 = [x][x]
		
		pmulhw		mm1,mm4

		packssdw	mm2,mm2		// mm0 = [x][x][x][x]

		paddsw		mm0,mm1

		paddsw		mm0,mm2		// hard stall on mm0, inevitable ; no stall on mm2

		packuswb	mm0,mm0		// hard stall on mm0, inevitable

		movd		[edi],mm0
		add			edi,4		// no stall on edi

	dec ecx
	jnz More

	//emms
	}
}*/

/*}{******* CPU setup ***********/

void (*YUVi_to_XRGB_line)(int *line1,int *line2,int *line3,uint8 * bline,int len) = NULL;
void (*YUVi_to_BGRb_lines)(int w,int h,int **Ylines,int **Ulines,int **Vlines,uint8 * BGRptr,int BGRstride) = NULL;

void SetupYUV(void)
{
	jeCPU_GetInfo();

	/*if ( jeCPU_Features & JE_CPU_HAS_MMX )
	{
		// timed on hare512.bmp :
	//	YUVi_to_BGRb_line = YUVi_to_BGRb_line_mmx1;	// blit : 0.025 seconds = 47.2 clocks / pixel
	//	YUVi_to_BGRb_line = YUVi_to_BGRb_line_mmx2;	// blit : 0.025 seconds = 47.2 clocks / pixel
		YUVi_to_BGRb_line = YUVi_to_BGRb_line_c;	// blit : 0.034 seconds = 66.6 clocks / pixel
	//	YUVi_to_XRGB_line = YUVi_to_XRGB_line_mmx;
	//	YUVi_to_BGRb_lines = YUVi_to_BGRb_lines_mmx;// blit : 0.0245 seconds= 45.9 clocks / pixel
		YUVi_to_BGRb_lines = YUVi_to_BGRb_lines_c;// blit : 0.0245 seconds= 45.9 clocks / pixel
	}
	else*/
	{
	//	YUVi_to_BGRb_line = YUVi_to_BGRb_line_c;
		YUVi_to_XRGB_line = YUVi_to_XRGB_line_c;
		YUVi_to_BGRb_lines = YUVi_to_BGRb_lines_c;
	}
}

/*}******* EOF ***********/
