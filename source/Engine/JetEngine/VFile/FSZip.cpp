/*!
	@file FSZip.cpp

	@author Anthony Rufrano (paradoxnj)
	@brief Zip file interface

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
#include <stdio.h>
#include <string.h>
#include <string>

#include "BaseType.h"
#include "Ram.h"
#include "VFile.h"
#include "VFile._h"
#include "FSZip.h"

#include "zzip/lib.h"

typedef struct _ZipFile
{
	std::string strFileName;

	ZZIP_DIR *pZipFile;
	ZZIP_FILE *pFile;
} ZipFile;

static void * JETCC FSZip_FinderCreate(jeVFile *FS, void *Handle, const char *	FileSpec)
{
	return NULL;
}

static jeBoolean JETCC FSZip_FinderGetNextFile(void *Handle)
{
	return JE_FALSE;
}

static jeBoolean JETCC FSZip_FinderGetProperties(void *Handle, jeVFile_Properties *Props)
{
	memset(Props, 0, sizeof(jeVFile_Properties));
	return JE_FALSE;
}

static void JETCC FSZip_FinderDestroy(void *Handle)
{
}

static void * JETCC FSZip_OpenNewSystem(jeVFile *FS, const char *Name, void *Context, unsigned int OpenModeFlags)
{
	ZipFile *pFile = nullptr;

	assert(FS == NULL);
	assert(Name != NULL);
	assert(OpenModeFlags & JE_VFILE_OPEN_DIRECTORY);

	pFile = static_cast<ZipFile*>(jeRam_AllocateClear(sizeof(ZipFile)));
	if (!pFile)
		return NULL;

	zzip_error_t zzipError;
	pFile->pZipFile = zzip_dir_open(Name, &zzipError);
	if (!pFile->pZipFile)
	{
		jeRam_Free(pFile);
		pFile = nullptr;

		return nullptr;
	}

	pFile->strFileName = Name;
	return pFile;
}

static void * JETCC FSZip_Open(jeVFile *FS, void *Handle, const char *Name, void *Context, unsigned int OpenModeFlags)
{
	ZipFile *pFile = static_cast<ZipFile*>(Handle);
	ZipFile *pNewFile = nullptr;

	assert(Name != NULL);

	pNewFile = static_cast<ZipFile*>(jeRam_AllocateClear(sizeof(ZipFile)));
	if (!pNewFile)
		return nullptr;

	if (OpenModeFlags & JE_VFILE_OPEN_DIRECTORY)
	{
		assert(FS == NULL);

		zzip_error_t zzipError;
		pFile->pZipFile = zzip_dir_open(Name, &zzipError);
		if (!pFile->pZipFile)
		{
			jeRam_Free(pFile);
			pFile = nullptr;

			return nullptr;
		}
	}
	else
	{
		assert(FS != NULL);
		ZipFile *pFile = static_cast<ZipFile*>(Handle);

		pNewFile->pZipFile = nullptr;
		pNewFile->pFile = zzip_file_open(pFile->pZipFile, Name, ZZIP_ONLYZIP | ZZIP_CASELESS);
		if (!pNewFile->pFile)
		{
			jeRam_Free(pNewFile);
			pNewFile = nullptr;

			return nullptr;
		}
	}

	pNewFile->strFileName = Name;
	return pNewFile;
}

static jeBoolean JETCC FSZip_UpdateContext(jeVFile *FS, void *Handle, void *Context, int ContextSize)
{
	return JE_FALSE;
}

static jeBoolean JETCC FSZip_Close(void *Handle)
{
	assert(Handle != NULL);

	ZipFile *pFile = static_cast<ZipFile*>(Handle);
	if (pFile->pZipFile)
	{
		zzip_closedir(pFile->pZipFile);
		pFile->pZipFile = nullptr;
	}
	else if (pFile->pFile)
	{
		zzip_file_close(pFile->pFile);
		pFile->pFile = nullptr;
	}
	else
	{
		return JE_FALSE;
	}

	jeRam_Free(pFile);
	pFile = nullptr;

	return JE_TRUE;
}

static jeBoolean JETCC FSZip_GetS(void *Handle, void *Buff, int MaxLen)
{
	assert(Handle != NULL);

	ZipFile *pFile = static_cast<ZipFile*>(Handle);
	assert(pFile->pZipFile == nullptr);

	zzip_size_t bytesread = zzip_fread(Buff, 1, MaxLen, pFile->pFile);
	if (bytesread == 0)
		return JE_FALSE;

	char *end = (char*)Buff + bytesread;
	char *p = (char*)Buff;

	while (p < end)
	{
		/*
		This code will terminate a line on one of three conditions:
		\r	Character changed to \n, next char set to 0
		\n	Next char set to 0
		\r\n	First \r changed to \n.  \n changed to 0.
		*/
		if (*p == '\r')
		{
			int Skip = 0;

			*p = '\n';		// set end of line
			p++;			// and skip to next char
			// If the next char is a newline, then skip it too  (\r\n case)
			if (*p == '\n')
			{
				Skip = 1;
			}
			*p = '\0';
			// Set the file pointer back a bit since we probably overran
			//SetFilePointer(File->FileHandle, -(int)(BytesRead - ((p + Skip) - (char *)Buff)), NULL, FILE_CURRENT);
			zzip_seek(pFile->pFile, -(int)(bytesread - ((p + Skip) - (char*)Buff)), SEEK_CUR);
			assert(p - (char *)Buff <= MaxLen);
			return JE_TRUE;
		}
		else if (*p == '\n')
		{
			// Set the file pointer back a bit since we probably overran
			p++;
			//SetFilePointer(File->FileHandle, -(int)(BytesRead - (p - (char *)Buff)), NULL, FILE_CURRENT);
			zzip_seek(pFile->pFile, -(int)(bytesread - (p - (char*)Buff)), SEEK_CUR);
			*p = '\0';
			assert(p - (char *)Buff <= MaxLen);
			return JE_TRUE;
		}
		p++;
	}

	return JE_TRUE;
}

static jeBoolean JETCC FSZip_Tell(const void *Handle, long *Position)
{
	const ZipFile *File = static_cast<const ZipFile*>(Handle);

	assert(File->pZipFile == nullptr);
	*Position = (long)zzip_tell(File->pFile);

	return JE_TRUE;
}

static jeBoolean JETCC FSZip_BytesAvailable(void *Handle, long *Count)
{
	ZipFile *File = static_cast<ZipFile*>(Handle);

	assert(File->pZipFile == nullptr);

	zzip_size_t curpos = zzip_tell(File->pFile);
	zzip_seek(File->pFile, 0, SEEK_END);
	zzip_size_t endpos = zzip_tell(File->pFile);
	zzip_seek(File->pFile, curpos, SEEK_SET);

	*Count = (long)(endpos - curpos);
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_Read(void *Handle, void *Buff, uint32 Count)
{
	ZipFile *File = static_cast<ZipFile*>(Handle);

	assert(File->pZipFile == nullptr);

	zzip_size_t bytesread = zzip_fread(Buff, 1, Count, File->pFile);
	if (bytesread == 0)
		return JE_FALSE;

	return JE_TRUE;
}

static jeBoolean JETCC FSZip_Write(void *Handle, const void *Buff, int Count)
{
	// Not supported
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_Seek(void *Handle, int Where, jeVFile_Whence Whence)
{
	int w = SEEK_SET;
	ZipFile *File = static_cast<ZipFile*>(Handle);

	assert(File->pZipFile == nullptr);

	switch (Whence)
	{
	case JE_VFILE_SEEKSET:
		w = SEEK_SET;
		break;

	case JE_VFILE_SEEKEND:
		w = SEEK_END;
		break;

	case JE_VFILE_SEEKCUR:
		w = SEEK_CUR;
		break;

	default:
		return JE_FALSE;
	}

	zzip_seek(File->pFile, Where, w);
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_EOF(const void *Handle)
{
	const ZipFile *File = static_cast<const ZipFile*>(Handle);

	assert(File->pZipFile == nullptr);
	zzip_size_t curpos = zzip_tell(File->pFile);
	zzip_seek(File->pFile, 0, SEEK_END);
	zzip_size_t endpos = zzip_tell(File->pFile);
	zzip_seek(File->pFile, curpos, SEEK_SET);

	if (curpos == endpos)
		return JE_TRUE;

	return JE_FALSE;
}

static jeBoolean JETCC FSZip_Size(const void *Handle, long *Size)
{
	const ZipFile *File = static_cast<const ZipFile*>(Handle);
	
	assert(Size != nullptr);
	assert(File->pZipFile == nullptr);

	zzip_size_t curpos = zzip_tell(File->pFile);
	zzip_seek(File->pFile, 0, SEEK_END);
	*Size = zzip_tell(File->pFile);
	zzip_seek(File->pFile, curpos, SEEK_SET);

	return JE_TRUE;
}

static jeBoolean JETCC FSZip_GetProperties(const void *Handle, jeVFile_Properties *Properties)
{
	const ZipFile *File = static_cast<const ZipFile*>(Handle);

	memset(Properties, 0, sizeof(jeVFile_Properties));
	if (File->pZipFile)
		Properties->AttributeFlags = JE_VFILE_ATTRIB_DIRECTORY | JE_VFILE_ATTRIB_READONLY;
	else
		Properties->AttributeFlags = JE_VFILE_ATTRIB_READONLY;

	Properties->Time.Time1 = 0;
	Properties->Time.Time2 = 0;

	strcpy_s(Properties->Name, File->strFileName.c_str());
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_SetSize(void *Handle, long Size)
{
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_SetAttributes(void *Handle, jeVFile_Attributes Attributes)
{
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_SetTime(void *Handle, const jeVFile_Time *Time)
{
	return JE_TRUE;
}

static jeVFile * JETCC FSZip_GetHintsFile(void *Handle)
{
	return nullptr;
}

static jeBoolean JETCC FSZip_FileExists(jeVFile *FS, void *Handle, const char *Name)
{
	return JE_TRUE;
}

static jeBoolean JETCC FSZip_Disperse(jeVFile *FS, void *Handle, const char *Directory)
{
	return JE_FALSE;
}

static jeBoolean JETCC FSZip_DeleteFile(jeVFile *FS, void *Handle, const char *Name)
{
	return JE_FALSE;
}

static jeBoolean JETCC FSZip_RenameFile(jeVFile *FS, void *Handle, const char *Name, const char *NewName)
{
	return JE_FALSE;
}

static	jeVFile_SystemAPIs	FSZip_APIs =
{
	FSZip_FinderCreate,
	FSZip_FinderGetNextFile,
	FSZip_FinderGetProperties,
	FSZip_FinderDestroy,

	FSZip_OpenNewSystem,
	FSZip_UpdateContext,
	FSZip_Open,
	FSZip_DeleteFile,
	FSZip_RenameFile,
	FSZip_FileExists,
	FSZip_Disperse,
	FSZip_Close,

	FSZip_GetS,
	FSZip_BytesAvailable,
	FSZip_Read,
	FSZip_Write,
	FSZip_Seek,
	FSZip_EOF,
	FSZip_Tell,
	FSZip_Size,

	FSZip_GetProperties,

	FSZip_SetSize,
	FSZip_SetAttributes,
	FSZip_SetTime,

	FSZip_GetHintsFile,

};

const jeVFile_SystemAPIs *JETCC FSDos_GetAPIs(void)
{
	return &FSZip_APIs;
}
