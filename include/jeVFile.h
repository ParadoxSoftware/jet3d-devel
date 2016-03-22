/*!
	@file jeVFile.h 
	
	@author Anthony Rufrano
	@brief The file system interface

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

	@note C++ conversion done by Anthony Rufrano (paradoxnj)
*/
#ifndef __JE_VFILE_H__
#define __JE_VFILE_H__

#include <string>
#include "Basetype.h"

namespace jet3d {

class IVFile : virtual public jeUnknown
{
protected:
	virtual ~IVFile(){}

public:
	typedef enum eSeek
	{
		FILE_SEEK_CUR,
		FILE_SEEK_END,
		FILE_SEEK_SET
	} eSeek;

	virtual const std::string &getName() = 0;
	virtual const std::string &getFullPath() = 0;

	virtual int32 read(void *pvBuffer, int32 iSize) = 0;
	virtual int32 write(const void *pvBuffer, int32 iSize) = 0;

	virtual uint32 length() = 0;
	virtual int32 tell() = 0;

	virtual void flush() = 0;
	virtual int32 seek(int32 lOffset, eSeek eOrigin);
	virtual void rewind() = 0;
};

class IVFileDirectory : virtual public jeUnknown
{
protected:
	virtual ~IVFileDirectory(){}

public:
	virtual IVFile *open(const std::string &strDirectoryName) = 0;

	virtual const std::string &getDirectoryType() = 0;
	virtual const std::string &getFullPath() = 0;
};

class IVFileDirectoryFactory : virtual public jeUnknown
{
protected:
	virtual ~IVFileDirectoryFactory(){}

public:
	virtual const std::string &getDirectoryTypeName() = 0;
	virtual IVFileDirectory *openDirectory(const std::string &strDirName) = 0;
};

class IVFileManager : virtual public jeUnknown
{
protected:
	virtual ~IVFileManager(){}

public:
	virtual bool registerDirectoryType(const std::string &strDirectoryTypeName, IVFileDirectoryFactory *pFactory) = 0;
	virtual IVFileDirectory *addPath(const std::string &strDirName, const std::string &strDirType) = 0;
};

} // namespace jet3d

#endif
