/*!
	@file jeResourceManager.h 
	
	@author Anthony Rufrano
	@brief Manages resources

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
#ifndef __JE_RESOURCE_MANAGER_H__
#define __JE_RESOURCE_MANAGER_H__

#include <string>
#include <list>
#include "Basetype.h"
#include "jeVFile.h"

typedef struct jeEngine jeEngine;

/*! @name Resource kinds
@brief Possible value for Type parameter of jeResource_GetResource() */
/*@{*/
/*! @def JE_RESOURCE_ANY
@brief The resource is anything
*/
#define JE_RESOURCE_ANY		0x0000
/*! @def JE_RESOURCE_VFS
@brief The resource is a #jeVFile pointer
*/
#define JE_RESOURCE_VFS		0x0001
/*! @def JE_RESOURCE_BITMAP
@brief The resource is a #jeBitmap pointer
*/
#define JE_RESOURCE_BITMAP	0x0002
/*! @def JE_RESOURCE_SHADER
@brief The resource is a #jeShader pointer
*/
#define JE_RESOURCE_SHADER	0x0004
/*! @def JE_RESOURCE_SOUND
@brief The resource is a #jeSound pointer
*/
#define JE_RESOURCE_SOUND	0x0005
/*! @def JE_RESOURCE_ACTOR
@brief The resource is a #jeActor pointer
*/
#define JE_RESOURCE_ACTOR	0x0006
/*! @def JE_RESOURCE_MATERIAL
@brief The resource is a #jeMaterialSpec Material pointer build from JMAT file
*/
#define JE_RESOURCE_MATERIAL	0x0010
/*! @def JE_RESOURCE_TEXTURE
@brief The resource is a #jeTexture Texture pointer build from image file by driver
*/
#define JE_RESOURCE_TEXTURE		0x0011
/*@}*/

namespace jet3d {

class jeResourceNew : virtual public jeUnknown
{
protected:
	jeResourceNew() {}
	jeResourceNew(const jeResourceNew&) = delete;
	jeResourceNew(jeResourceNew&&) = delete;
	jeResourceNew& operator=(const jeResourceNew&) = delete;
	jeResourceNew& operator=(jeResourceNew&&) = delete;
	~jeResourceNew() {}

public:
	virtual const std::string &getType() = 0;
	virtual const std::string &getName() = 0;

	virtual bool isLoaded() = 0;
	virtual bool isDirty() = 0;
};

class jeResourceFactory : virtual public jeUnknown
{
protected:
	jeResourceFactory() {}
	jeResourceFactory(const jeResourceFactory&) = delete;
	jeResourceFactory(jeResourceFactory&&) = delete;
	jeResourceFactory& operator=(const jeResourceFactory&) = delete;
	jeResourceFactory& operator=(jeResourceFactory&&) = delete;
	~jeResourceFactory() {}

public:
	virtual jeResourceNew *load(jeVFile *pFile) = 0;
	virtual jeBoolean save(jeVFile *pFile, jeResourceNew *pResource) = 0;

	virtual const std::string &GetFileExtension() = 0;
};

typedef std::list<jeResourceNew*>				jeResourceList;
typedef jeResourceList::iterator				jeResourceListItr;

class jeResourceManager : virtual public jeUnknown
{
protected:
	jeResourceManager() {}
	jeResourceManager(const jeResourceManager&) = delete;
	jeResourceManager(jeResourceManager&&) = delete;
	jeResourceManager& operator=(const jeResourceManager&) = delete;
	jeResourceManager& operator=(jeResourceManager&&) = delete;
	~jeResourceManager() {}

public:
	virtual bool initialize() = 0;
	virtual bool shutdown() = 0;

	virtual jeResourceNew *create(IVFile *pFile, const std::string &strName) = 0; 
};

class jeResource : virtual public jeUnknown
{
protected:
	jeResource() {}
	jeResource(const jeResource&) = delete;
	jeResource(jeResource&&) = delete;
	jeResource& operator=(const jeResource&) = delete;
	jeResource& operator=(jeResource&&) = delete;
	~jeResource() {}

public:
	virtual const std::string &getName() = 0;
	virtual const uint32 getType() = 0;
	virtual void *getData() = 0;
	virtual jeBoolean isOpenDirectory() = 0;
};

class jeResourceMgr : virtual public jeUnknown
{
protected:
	jeResourceMgr() {};
	jeResourceMgr(const jeResourceMgr&) = delete;
	jeResourceMgr(jeResourceMgr&&) = delete;
	jeResourceMgr& operator=(const jeResourceMgr&) = delete;
	jeResourceMgr& operator=(jeResourceMgr&&) = delete;
	~jeResourceMgr(){}

public:
	virtual bool initializeWithDefaults() = 0;
	virtual void shutdown() = 0;

	virtual bool add(const std::string &strName, uint32 iType, void *pvData) = 0;
	virtual void *get(const std::string &strName) = 0;
	virtual bool remove(const std::string &strName) = 0;

	virtual jeEngine *getEngine() = 0;

	virtual bool addVFile(const std::string &strName, jeVFile *pFile) = 0;
	virtual jeVFile *getVFile(const std::string &strName) = 0;
	virtual bool removeVFile(const std::string &strName) = 0;

	virtual void *createResource(const std::string &strName, uint32 iType) = 0;
	virtual bool openDirectory(const std::string &strDirName, const std::string &strResourceName) = 0;
};

} // namespace jet3d

JETAPI jet3d::jeResourceMgr* JETCC jeResourceMgr_GetSingleton();

#endif
