/*!
	@file OglDrv.h
	@author Anthony Rufrano (paradoxnj)
	@brief OpenGL Driver Main Header
*/
#ifndef OGLDRV_H
#define OGLDRV_H

#include <windows.h>
#include <gl/gl.h>

#include "glext.h"
#include "wglext.h"

#include "DCommon.h"
#include "jeFileLogger.h"

#define DRIVERAPI				extern "C" _declspec(dllexport)

extern HWND						hWnd;
extern HDC						hDC;
extern HGLRC					hglRC;

extern bool						Has_Stencil;
extern bool						Has_MultiTexture;

//extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
//extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;

extern DRV_Window				ClientWindow;
extern DRV_Driver				OGLDRV;

extern jet3d::jeFileLoggerPtr	ogllog;

// Private Driver Functions
void							BuildRGBGammaTables(float Gamma);

// Public Driver Functions
jeBoolean						DRIVERCC OGLDrv_EnumSubDrivers(DRV_ENUM_DRV_CB *Cb, void *Context);
jeBoolean						DRIVERCC OGLDrv_EnumModes(S32 Driver, char *DriverName, DRV_ENUM_MODES_CB *Cb, void *Context);
jeBoolean						DRIVERCC OGLDrv_EnumPixelFormats(DRV_ENUM_PFORMAT_CB *Cb, void *Context);

jeBoolean						DRIVERCC OGLDrv_Init(DRV_DriverHook *Hook);
jeBoolean						DRIVERCC OGLDrv_Shutdown();
jeBoolean						DRIVERCC OGLDrv_UpdateWindow();
jeBoolean						DRIVERCC OGLDrv_SetActive(jeBoolean Active);

jeBoolean						DRIVERCC OGLDrv_BeginScene(jeBoolean Clear, jeBoolean ClearZ, RECT *WorldRect, jeBoolean WireFrame);
jeBoolean						DRIVERCC OGLDrv_EndScene();
jeBoolean						DRIVERCC OGLDrv_BeginBatch();
jeBoolean						DRIVERCC OGLDrv_EndBatch();

jeBoolean						DRIVERCC OGLDrv_RenderGouraudPoly(jeTLVertex *Pnts, int32 NumPoints, uint32 Flags);
jeBoolean						DRIVERCC OGLDrv_RenderWorldPoly(jeTLVertex *Pnts, int32 NumPoints, jeRDriver_Layer *Layers, int32 NumLayers, void *LMapCBContext, uint32 Flags);
jeBoolean						DRIVERCC OGLDrv_RenderMiscTexturePoly(jeTLVertex *Pnts, int32 NumPoints, jeRDriver_Layer *Layers, int32 NumLayers, uint32 Flags);

jeBoolean						DRIVERCC OGLDrv_DrawDecal(jeTexture *THandle, RECT *SrcRect, int32 x, int32 y);
jeBoolean						DRIVERCC OGLDrv_Screenshot(const char *filename);

jeBoolean						DRIVERCC OGLDrv_GetGamma(float *Gamma);
jeBoolean						DRIVERCC OGLDrv_SetGamma(float Gamma);

jeBoolean						DRIVERCC OGLDrv_SetMatrix(uint32 Type, jeXForm3d *Matrix);
jeBoolean						DRIVERCC OGLDrv_GetMatrix(uint32 Type, jeXForm3d *Matrix);

jeBoolean						DRIVERCC OGLDrv_SetCamera(jeCamera *Camera);
jeBoolean						DRIVERCC OGLDrv_SetRenderState(uint32 state, uint32 value);

typedef struct RGB_LUT
{
	uint32						r[256];
	uint32						g[256];
	uint32						b[256];
	uint32						a[256];
} RGB_LUT;

extern RGB_LUT					Lut1;

namespace jet3d {
	class OGLDriver : public jeDriver
	{
	public:
		OGLDriver();
		~OGLDriver() {}

	private:
		HWND hWnd;
		HDC	hDC;
		HGLRC hglRC;

		bool Has_Stencil;
		bool Has_MultiTexture;
		DRV_Window				ClientWindow;
		DRV_RENDER_MODE					RenderMode = RENDER_NONE;
		GLuint							Render_HardwareFlags;

		GLuint							boundTexture;
		GLuint							boundTexture2;

		GLuint							decalTexObj;

		GLfloat							CurrentGamma;

		int32							LastError;
		std::string						LastErrorStr;

		DRV_EngineSettings				EngineSettings;

		GLuint							AlphaFunc = GL_LEQUAL;
		GLuint							StencilFunc = GL_LEQUAL;

		GLfloat							AlphaRef;
		GLfloat							StencilRef;

	public:
		uint32 AddRef();
		uint32 Release();

		const std::string& getName() const;
		const int32 getVersionMajor() const;
		const int32 getVersionMinor() const;

		const int32 getLastError() const;
		const std::string& getLastErrorString() const;

		jeBoolean enumSubDrivers(DRV_ENUM_DRV_CB* Cb, void* Context);
		jeBoolean enumModes(int32 Driver, const std::string& DriverName, DRV_ENUM_MODES_CB* Cb, void* Context);
		jeBoolean enumPixelFormats(DRV_ENUM_PFORMAT_CB* Cb, void* Context);

		jeBoolean initialize(DRV_DriverHook* Hook);
		jeBoolean shutdown();
		jeBoolean reset();
		jeBoolean updateWindow();
		jeBoolean setActive(const jeBoolean& active);

		jeTexture* createTexture(int32 Width, int32 Height, int32 NumMipLevels, const jeRDriver_PixelFormat* PixelFormat);
		jeTexture* createTextureFromFile(jeVFile* File);

		jeBoolean beginScene(jeBoolean Clear, jeBoolean ClearZ, RECT* WorldRect, jeBoolean Wireframe);
		jeBoolean endScene();
		jeBoolean beginBatch();
		jeBoolean endBatch();

		jeBoolean renderGouraudPoly(jeTLVertex* Pnts, int32 NumPoints, uint32 Flags);
		jeBoolean renderWorldPoly(jeTLVertex* Pnts, int32 NumPoints, jeRDriver_Layer* Layers, int32 NumLayers, void* LMapCBContext, uint32 Flags);
		jeBoolean renderMiscTexturePoly(jeTLVertex* Pnts, int32 NumPoints, jeRDriver_Layer* Layers, int32 NumLayers, uint32 Flags);

		jeBoolean drawDecal(jeTexture* THandle, RECT* SRect, int32 x, int32 y);

		jeBoolean screenShot(const std::string& filename);
		jeBoolean drawText(const std::string& text, int x, int y, uint32 color);
		jeBoolean setFog(float r, float g, float b, float start, float endi, jeBoolean enable);

		jeBoolean setGamma(const float& gamma);
		float getGamma();

		jeFont* createFont(int32 Height, int32 Width, uint32 Weight, jeBoolean Italic, const std::string& facename);

		jeBoolean setRenderState(uint32 state, uint32 value);

	public:
		static std::string strName;
	};
}
#endif


