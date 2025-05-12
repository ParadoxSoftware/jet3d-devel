/****************************************************************************************/
/*  APROJECT.H                                                                          */
/*                                                                                      */
/*  Author:                                                                             */
/*  Description:                                                                        */
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
/*
  AProject.h -- Actor Studio Project file API

  Copyright � 1999, Eclipse Entertainment

  The Actor Studio project file is separated into several sections:
	Version info
	Path Settings
	Output file section
	Body section
	Materials Section
	Additional Textures
	Motion section

  The Apj API allows access to the individual sections and portions thereof.
*/
#ifndef AProject_H
#define AProject_H

#ifdef __cplusplus
//	extern "C" {
#endif


#include "jet.h"

//typedef struct tag_AProject	AProject;
class AProject;
typedef std::shared_ptr<AProject>				AProjectPtr;

typedef struct _ApjOutput
{
	std::string Filename;
	ApjOutputFormat Fmt;
} ApjOutput;

typedef struct _ApjPaths
{
	jeBoolean ForceRelative;
	std::string Materials;
	std::string TempFiles;
} ApjSearchPaths;

typedef struct _ApjBody
{
	std::string Filename;
	ApjBodyFormat Fmt;
} ApjBody;

// Entry in Materials section
typedef struct
{
	std::string Name;			// Material name
	ApjMaterialFormat Fmt;  // type
	std::string Filename;		// texture filename (may be NULL)
	JE_RGBA Color;		//
} ApjMaterialEntry;

typedef std::vector<ApjMaterialEntry>					ApjMaterials;

// Entry in motions section
typedef struct
{
	std::string Name;				// motion name
	ApjMotionFormat Fmt;	// motion file format
	std::string Filename;			// file that contains the motion
	jeBoolean OptFlag;		// optimization flag
	int OptLevel;			// motion optimization level
	std::string Bone;				// name of root bone to grab
} ApjMotionEntry;

typedef std::vector<ApjMotionEntry>						ApjMotions;

class AProject
{
public:
	AProject();
	AProject(const std::string& OutputName);
	AProject(jeVFile *FS);
	virtual ~AProject();

private:
	ApjOutput		Output;
	ApjSearchPaths	Paths;
	ApjBody			Body;
	ApjMaterials	Materials;
	ApjMotions		Motions;

public:
	bool Create (const std::string& OutputName);

	// file i/o
	bool CreateFromFile (jeVFile *FS);
	bool CreateFromFilename (const std::string& Filename);

	bool WriteToFile (jeVFile *FS);
	bool WriteToFilename (const std::string& Filename);

	// Paths section
	bool GetForceRelativePaths ();
	bool SetForceRelativePaths (const jeBoolean Flag);

	std::string GetMaterialsPath ();
	bool SetMaterialsPath (const std::string& Path);

	//Need a directory for 'temporary' or 'obj' files:
	std::string GetObjPath ();
	bool SetObjPath (const std::string& Path);
};

// Create and destroy
AProjectPtr AProject_Create (const std::string& OutputName);
//void AProject_Destroy (AProject **ppProject);

// file i/o
AProjectPtr AProject_CreateFromFile (jeVFile *FS);
AProjectPtr AProject_CreateFromFilename (const std::string& Filename);

jeBoolean AProject_WriteToFile (AProjectPtr pProject, jeVFile *FS);
jeBoolean AProject_WriteToFilename (AProjectPtr pProject, const std::string& Filename);

// Paths section
jeBoolean AProject_GetForceRelativePaths (AProjectPtr pProject);
jeBoolean AProject_SetForceRelativePaths (AProjectPtr pProject, const jeBoolean Flag);

std::string AProject_GetMaterialsPath (AProjectPtr pProject);
jeBoolean AProject_SetMaterialsPath (AProjectPtr pProject, const std::string& Path);

//Need a directory for 'temporary' or 'obj' files:
std::string AProject_GetObjPath (AProjectPtr pProject);
jeBoolean AProject_SetObjPath (AProjectPtr pProject, const std::string& Path);
		
// Output file section
typedef enum
{
	ApjOutput_Text = 0,
	ApjOutput_Binary = 1
} ApjOutputFormat;

std::string AProject_GetOutputFilename (AProjectPtr pProject);
jeBoolean AProject_SetOutputFilename (AProjectPtr pProject, const std::string& Filename);

ApjOutputFormat AProject_GetOutputFormat (AProjectPtr pProject);
jeBoolean AProject_SetOutputFormat (AProjectPtr pProject, const ApjOutputFormat Fmt);


// Body section
typedef enum
{
	ApjBody_Invalid = 0,
	ApjBody_Max = 1,
	ApjBody_Nfo = 2,
	ApjBody_Bdy = 3,
	ApjBody_Act = 4
} ApjBodyFormat;

ApjBodyFormat AProject_GetBodyFormatFromFilename (const std::string& Name);

std::string AProject_GetBodyFilename (AProjectPtr pProject);
jeBoolean AProject_SetBodyFilename (AProjectPtr pProject, const std::string& Filename);

ApjBodyFormat AProject_GetBodyFormat (AProjectPtr pProject);
jeBoolean AProject_SetBodyFormat (AProjectPtr pProject, ApjBodyFormat Fmt);


// Materials section
typedef enum
{
	ApjMaterial_Color = 0,
	ApjMaterial_Texture = 1
} ApjMaterialFormat;

int AProject_GetMaterialsCount (AProjectPtr pProject);

jeBoolean AProject_AddMaterial
	(
	  AProjectPtr pProject,
	  const std::string& MaterialName,
	  const ApjMaterialFormat Fmt,
	  const std::string& TextureFilename,
	  const float Red, const float Green, const float Blue, const float Alpha,
	  int *pIndex		// returned index
	);

jeBoolean AProject_RemoveMaterial (AProjectPtr pProject, const int Index);

int AProject_GetMaterialIndex (AProjectPtr pProject, const std::string& MaterialName);

ApjMaterialFormat AProject_GetMaterialFormat (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMaterialFormat (AProjectPtr pProject, const int Index, const ApjMaterialFormat Fmt);

std::string AProject_GetMaterialName (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMaterialName (AProjectPtr pProject, const int Index, const std::string& MaterialName);

std::string AProject_GetMaterialTextureFilename (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMaterialTextureFilename (AProjectPtr pProject, const int Index, const std::string& TextureFilename);

JE_RGBA AProject_GetMaterialTextureColor (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMaterialTextureColor (AProjectPtr pProject, const int Index, 
	const float Red, const float Green, const float Blue, const float Alpha);


// Motions section
typedef enum
{
	ApjMotion_Invalid = 0,
	ApjMotion_Max = 1,
	ApjMotion_Key = 2,
	ApjMotion_Mot = 3,
// Actor motions not yet supported
//	ApjMotion_Act = 4,
	// enter new types before this line
	ApjMotion_TypeCount
} ApjMotionFormat;

ApjMotionFormat AProject_GetMotionFormatFromFilename (const std::string& Filename);

int AProject_GetMotionsCount (AProjectPtr pProject);

jeBoolean AProject_AddMotion
	(
	  AProjectPtr pProject,
	  const std::string& MotionName,
	  const std::string& Filename,
	  const ApjMotionFormat Fmt,
	  const jeBoolean OptFlag,
	  const int OptLevel,
	  const std::string& BoneName,
	  int *pIndex	// returned index
	);

jeBoolean AProject_RemoveMotion (AProjectPtr pProject, const int Index);

int AProject_GetMotionIndex (AProjectPtr pProject, const std::string& MotionName);

ApjMotionFormat AProject_GetMotionFormat (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMotionFormat (AProjectPtr *pProject, const int Index, const ApjMotionFormat Fmt);

std::string AProject_GetMotionName (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMotionName (AProjectPtr pProject, const int Index, const std::string& MotionName);

std::string AProject_GetMotionFilename (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMotionFilename (AProjectPtr pProject, const int Index, const std::string& Filename);

jeBoolean AProject_GetMotionOptimizationFlag (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMotionOptimizationFlag (AProjectPtr pProject, const int Index, const jeBoolean Flag);

int AProject_GetMotionOptimizationLevel (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMotionOptimizationLevel (AProjectPtr pProject, const int Index, const int OptLevel);

std::string AProject_GetMotionBone (AProjectPtr pProject, const int Index);
jeBoolean AProject_SetMotionBone (AProjectPtr pProject, const int Index, const std::string& BoneName);

#ifdef __cplusplus
//	}
#endif


#endif
