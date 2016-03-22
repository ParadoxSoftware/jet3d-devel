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
#pragma once

#include <string>
#include <list>
#include "Basetype.h"
#include "jeVFile.h"

namespace jet3d {

class jeResource : virtual public jeUnknown
{
protected:
	virtual ~jeResource(){}

public:
	virtual const std::string &getType() = 0;
	virtual const std::string &getName() = 0;

	virtual bool load() = 0;
	virtual bool unload() = 0;

	virtual bool isLoaded() = 0;
	virtual bool isDirty() = 0;
};

typedef std::list<jeResource*>				jeResourceList;
typedef jeResourceList::iterator			jeResourceListItr;

class jeResourceManager : virtual public jeUnknown
{
protected:
	virtual ~jeResourceManager(){}

public:
	virtual bool initialize() = 0;
	virtual bool shutdown() = 0;

	virtual jeResource *create(IVFile *pFile, const std::string &strName) = 0; 
};

} // namespace jet3d
