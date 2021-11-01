////////////////////////////////////////////////////////////////////////////////////////
//	Bitmaplist struct
////////////////////////////////////////////////////////////////////////////////////////
typedef struct BitmapList
{
	int		Total;
	char	**Name;
	int		*Width;
	int		*Height;
	int		*NumericSizes;
	char	**StringSizes;
	int		SizesListSize;

} BitmapList;
static BitmapList		*Bitmaps;
static char				*NoSelection = "< none >";

// Added by Incarnadine
// This function gets the correct translation for a collision/render box.  The problem was,
// boxes are positioned by their center, while ActorObj may be positioned anywhere.  This
// takes that into account and returns where the box should be positioned.
void GetBoxTranslation(const jeExtBox *Box, const jeActor *Actor, jeVec3d *Translation)
{
	jeVec3d BoxCenterToActor = { 0 };
	jeVec3d BoxCenter = { 0 };
	jeVec3d TranslationMod = { 0 };
	jeVec3d ActorPos = { 0 }, Zero = { 0 };
	ActorObj *Object = nullptr;

	assert(Box != nullptr);
	assert(Translation != nullptr);
	assert(Actor != nullptr);
	assert(Actor->Object != nullptr);	
	
	Object = Actor->Object;
	jeVec3d_Set(&Zero,0,0,0);
	ActorPos = Actor->Xf.Translation;	
	jeExtBox_GetTranslation(Box,&BoxCenter);
	jeVec3d_Subtract(&BoxCenter, &Zero,&BoxCenterToActor);
	BoxCenterToActor.X = 0.0f;//*= Object->ScaleX;
	BoxCenterToActor.Y *= Object->ScaleX;
	BoxCenterToActor.Z = 0.0f;//*= Object->ScaleZ;	
	jeVec3d_Subtract(&BoxCenterToActor,&BoxCenter,&TranslationMod);
	jeVec3d_Add(&TranslationMod,&ActorPos,Translation);
}

////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_StrDup()
//
////////////////////////////////////////////////////////////////////////////////////////
static char * Util_StrDup(
	const char	*const String )	// string to copy
{

	// locals
	char	*NewString = nullptr;

	// ensure valid data
	assert( String != nullptr );

	// copy string
	NewString = static_cast<char*>(JE_RAM_ALLOCATE( strlen( String ) + 1 ));
	if ( NewString ) 
	{
		strcpy( NewString, String );
	}
	else
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
	}

	// return string
	return NewString;

} // Util_StrDup()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_ResetActorMaterialToDefault()
//
//	Resets an actors material to its default one.
//
////////////////////////////////////////////////////////////////////////////////////////
static jeBoolean Util_ResetActorMaterialToDefault(
	jeActor		*Actor,				// actor to reset
	const jeActor_Def	*ActorDef,			// actor def from which to get default info from
	int			MaterialIndex )		// material index
{

	// locals
	jeBody		*Body = nullptr;
	jeBoolean	Result = JE_FALSE;
	const char	*MaterialName = nullptr;
	jeMaterialSpec	*Bitmap = nullptr;
	jeFloat		Red = 0, Green = 0, Blue = 0;
	jeUVMapper	Mapper = { 0 };

	// ensure valid data
	assert( Actor != nullptr );
	assert( ActorDef != nullptr );
	assert( MaterialIndex >= 0 );

	// get actor def body
	Body = jeActor_GetBody( ActorDef );
	assert ( Body != nullptr );

	// get default material
	Result = jeBody_GetMaterial(	Body, MaterialIndex, &MaterialName,
									&Bitmap, &Red, &Green, &Blue,
									&Mapper );
	if ( Result == JE_FALSE )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, "Failed to reset actor material to default" );
		return JE_FALSE;
	}

	// reset actor matet
	return jeActor_SetMaterial( Actor, MaterialIndex, Bitmap, Red, Green, Blue, Mapper );

} // Util_ResetActorMaterialToDefault()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_CreateBitmapFromFileName()
//
//	Create a bitmap from a file.
//
////////////////////////////////////////////////////////////////////////////////////////
static jeMaterialSpec * Util_CreateBitmapFromFileName(
	jeVFile		*File,			// file system to use
	const char	*Name,			// name of the file
	const char	*AlphaName,		// name of the alpha file
	jet3d::jeResourceMgr* ResourceMgr)	
{

	// locals
	jeVFile		*BmpFile = nullptr;
	jeBitmap	*Bmp = nullptr;
	jeMaterialSpec *MatSpec = nullptr;
	jeBoolean	Result = JE_FALSE;

	// ensure valid data
	assert( Name != nullptr );
	assert(ResourceMgr != nullptr);

	// open the bitmap
	if ( File == nullptr )
	{
		BmpFile = jeVFile_OpenNewSystem( nullptr, JE_VFILE_TYPE_DOS, Name, nullptr, JE_VFILE_OPEN_READONLY );
	}
	else
	{
		BmpFile = jeVFile_Open( File, Name, JE_VFILE_OPEN_READONLY );
	}
	if ( BmpFile == nullptr )
	{
		jeErrorLog_Add( JE_ERR_FILEIO_OPEN, nullptr );
		return nullptr;
	}

	// create the bitmap
	Bmp = jeBitmap_CreateFromFile( BmpFile );
	
	//MatSpec = jeMaterialSpec_Create(jeResourceMgr_GetEngine(ResourceMgr), ResourceMgr);
	MatSpec = jeMaterialSpec_Create(ResourceMgr->getEngine());
	jeVFile_Close( BmpFile );
	if ( Bmp == nullptr )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
		return nullptr;
	}

	// add alpha if required...
	if ( AlphaName != nullptr )
	{

		// locals
		jeBitmap	*AlphaBmp = nullptr;
		jeVFile		*AlphaFile = nullptr;

		// open alpha file
		if ( File == nullptr )
		{
			AlphaFile = jeVFile_OpenNewSystem( nullptr, JE_VFILE_TYPE_DOS, AlphaName, nullptr, JE_VFILE_OPEN_READONLY );
		}
		else
		{
			AlphaFile = jeVFile_Open( File, AlphaName, JE_VFILE_OPEN_READONLY );
		}
		if( AlphaFile == nullptr )
		{
			jeErrorLog_Add( JE_ERR_FILEIO_OPEN, nullptr );
			jeBitmap_Destroy( &Bmp );
			return nullptr;
		}

		// create alpha bitmap
		AlphaBmp = jeBitmap_CreateFromFile( AlphaFile );
		jeVFile_Close( AlphaFile );
		if ( AlphaBmp == nullptr )
		{
			jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
			jeBitmap_Destroy( &Bmp );
			return nullptr;
		}

		// fail if alpha isn't same size as main bitmap
		if (	( jeBitmap_Width( Bmp ) != jeBitmap_Width( AlphaBmp ) ) ||
				( jeBitmap_Height( Bmp ) != jeBitmap_Height( AlphaBmp ) ) )
		{
			jeErrorLog_Add( JE_ERR_BAD_PARAMETER, nullptr );
			jeBitmap_Destroy( &AlphaBmp );
			jeBitmap_Destroy( &Bmp );
			return nullptr;
		}

		// set its alpha
		Result = jeBitmap_SetAlpha( Bmp, AlphaBmp );
		if ( Result == JE_FALSE )
		{
			jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
			jeBitmap_Destroy( &AlphaBmp );
			jeBitmap_Destroy( &Bmp );
			return nullptr;
		}

		// don't need the alpha anymore
		jeBitmap_Destroy( &AlphaBmp );
	}
	// ...or just set the color key
	else
	{
		Result = jeBitmap_SetColorKey( Bmp, JE_TRUE, 255, JE_FALSE );
		assert( Result );
	}

#pragma message ("Krouer: change nullptr to something better next time")
	jeMaterialSpec_AddLayerFromBitmap(MatSpec, 0, Bmp, nullptr);

	// all done
	return MatSpec;

} // Util_CreateBitmapFromFileName()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_DestroyBitmapList()
//
////////////////////////////////////////////////////////////////////////////////////////
static void Util_DestroyBitmapList(
	BitmapList	**DeadList )	// list to destroy
{

	// locals
	BitmapList	*List = nullptr;
	int			i = 0;

	// ensure valid data
	assert( DeadList != nullptr );
	assert( *DeadList != nullptr );

	// get list pointer
	List = *DeadList;

	// destroy file list
	if ( List->Name != nullptr )
	{
		assert( List->Total > 0 );
		for ( i = 0; i < List->Total; i++ )
		{
			if ( List->Name[i] != nullptr )
			{
				JE_RAM_FREE( List->Name[i] );
			}
		}
		JE_RAM_FREE( List->Name );
	}

	// destroy width and height lists
	if ( List->Width != nullptr )
	{
		JE_RAM_FREE( List->Width );
	}
	if ( List->Height != nullptr )
	{
		JE_RAM_FREE( List->Height );
	}

	// destroy numeric sizes list
	if ( List->NumericSizes != nullptr )
	{
		JE_RAM_FREE( List->NumericSizes );
	}

	// destroy string sizes list
	if ( List->StringSizes != nullptr )
	{
		for ( i = 0; i < List->SizesListSize; i++ )
		{
			assert( List->StringSizes[i] != nullptr );
			JE_RAM_FREE( List->StringSizes[i] );
		}
	}

	// free bitmaplist struct
	JE_RAM_FREE( List );

	// zap pointer
	*DeadList = nullptr;

} // Util_DestroyBitmapList()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_CreateBitmapList()
//
////////////////////////////////////////////////////////////////////////////////////////
static BitmapList * Util_CreateBitmapList(
	jet3d::jeResourceMgr	*ResourceMgr,	// resource manager to use
	const std::string		&ResourceName,	// name of resource
	const std::string		&FileFilter )	// file filter
{

	// locals
	BitmapList		*Bmps = nullptr;
	jeVFile			*FileDir = nullptr;
	jeVFile_Finder	*Finder = nullptr;
	int				CurFile = 0;

	// ensure valid data
	assert( ResourceMgr != nullptr );
	assert( !ResourceName.empty());
	assert( !FileFilter.empty() );

	// allocate bitmaplist struct
	Bmps = static_cast<BitmapList*>(JE_RAM_ALLOCATE_CLEAR( sizeof( *Bmps ) ));
	if ( Bmps == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		return nullptr;
	}

	// get vfile dir
	//FileDir = jeResource_GetVFile( ResourceMgr, ResourceName );
	FileDir = ResourceMgr->getVFile(ResourceName);
	if ( FileDir == nullptr )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// create directory finder
	Finder = jeVFile_CreateFinder( FileDir, FileFilter.c_str() );
	if ( Finder == nullptr )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// determine how many files there are
	Bmps->Total = 1;
	while ( jeVFile_FinderGetNextFile( Finder ) == JE_TRUE )
	{
		Bmps->Total++;
	}

	// destroy finder
	jeVFile_DestroyFinder( Finder );
	Finder = nullptr;

	// allocate name list
	Bmps->Name = static_cast<char **>(JE_RAM_ALLOCATE_CLEAR( sizeof( char * ) * Bmps->Total ));
	if ( Bmps->Name == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// allocate width list
	Bmps->Width = static_cast<int *>(JE_RAM_ALLOCATE_CLEAR( sizeof( int * ) * Bmps->Total ));
	if ( Bmps->Width == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// allocate height list
	Bmps->Height = static_cast<int *>(JE_RAM_ALLOCATE_CLEAR( sizeof( int * ) * Bmps->Total ));
	if ( Bmps->Height == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// allocate numeric sizes list
	Bmps->NumericSizes = static_cast<int *>(JE_RAM_ALLOCATE_CLEAR( sizeof( int * ) * Bmps->Total ));
	if ( Bmps->NumericSizes == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// allocate string sizes list
	Bmps->StringSizes = static_cast<char **>(JE_RAM_ALLOCATE_CLEAR( sizeof( char * ) * Bmps->Total ));
	if ( Bmps->StringSizes == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// create directory finder
	Finder = jeVFile_CreateFinder( FileDir, FileFilter.c_str() );
	if ( Finder == nullptr )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
		goto ERROR_Util_BuildBitmapList;
	}

	// first entry is always the "no selection" slot
	CurFile = 0;
	Bmps->Name[CurFile++] = Util_StrDup( NoSelection );

	// build file list
	while ( jeVFile_FinderGetNextFile( Finder ) == JE_TRUE )
	{

		// locals
		jeVFile_Properties	Properties;
		//jeBitmap			*Bitmap;
		jeMaterialSpec		*MatSpec = nullptr;

		// get properties of current file
		if( jeVFile_FinderGetProperties( Finder, &Properties ) == JE_FALSE )
		{
			jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
			goto ERROR_Util_BuildBitmapList;
		}

		// save file name
		assert( CurFile < Bmps->Total );
		Bmps->Name[CurFile] = Util_StrDup( Properties.Name );
		if ( Bmps->Name[CurFile] == nullptr )
		{
			jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
			goto ERROR_Util_BuildBitmapList;
		}

		// save width and height
		MatSpec = Util_CreateBitmapFromFileName( FileDir, Bmps->Name[CurFile], nullptr, ResourceMgr );
		if ( MatSpec == nullptr )
		{
			jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
			goto ERROR_Util_BuildBitmapList;
		}
		Bmps->Width[CurFile] = jeMaterialSpec_Width( MatSpec );
		Bmps->Height[CurFile] = jeMaterialSpec_Height( MatSpec );
		jeMaterialSpec_Destroy( &MatSpec );

		// add sise to numeric sizes list
		{

			// locals
			jeBoolean	AddIt;
			int			i;

			// check if it needs to be added to numeric sizes list
			AddIt = JE_TRUE;
			for ( i = 0; i < Bmps->Total; i++ )
			{
				if ( Bmps->Width[CurFile] == Bmps->NumericSizes[i] )
				{
					AddIt = JE_FALSE;
					break;
				}
			}

			// add it if required
			if ( AddIt == JE_TRUE )
			{
				for ( i = 0; i < Bmps->Total; i++ )
				{
					if (	( Bmps->Width[CurFile] < Bmps->NumericSizes[i] ) ||
							( Bmps->NumericSizes[i] == 0 ) )
					{
						int	Hold1, Hold2;
						Hold1 = Bmps->Width[CurFile];
						for ( ; i < Bmps->Total; i++ )
						{
							Hold2 = Bmps->NumericSizes[i];
							Bmps->NumericSizes[i] = Hold1;
							Hold1 = Hold2;
						}
						AddIt = JE_FALSE;
						break;
					}
				}
			}
			assert( AddIt == JE_FALSE );
		}

		// adjust file counter
		CurFile++;
	}

	// destroy finder
	jeVFile_DestroyFinder( Finder );

	// close vfile dir
	//if ( jeResource_DeleteVFile( ResourceMgr, ResourceName ) == 0 )
	if (!ResourceMgr->removeVFile(ResourceName))
	{
		jeVFile_Close( FileDir );
	}

	// create string sizes list
	{

		// locals
		int		i;
		char	Buf[256];

		// build list
		Bmps->SizesListSize = Bmps->Total;
		for ( i = 0; i < Bmps->Total; i++ )
		{
			if ( Bmps->NumericSizes[i] == 0 )
			{
				Bmps->SizesListSize = i;
				break;
			}
			sprintf(Buf, "%i" , Bmps->NumericSizes[i]); //_itoa( Bmps->NumericSizes[i], Buf, 10 );
			Bmps->StringSizes[i] = Util_StrDup( Buf );
		}
	}

	// return bitmaplist struct
	return Bmps;

	// error handling
	ERROR_Util_BuildBitmapList:

	// destroy bitmap list
	assert( Bmps != nullptr );
	Util_DestroyBitmapList( &Bmps );

	// destroy finder
	if ( Finder != nullptr )
	{
		jeVFile_DestroyFinder( Finder );
	}

	// close vfile dir
	if ( FileDir != nullptr )
	{
		//if ( jeResource_DeleteVFile( ResourceMgr, ResourceName ) == 0 )
		if (!ResourceMgr->removeVFile(ResourceName))
		{
			jeVFile_Close( FileDir );
		}
	}

	// return failure
	return nullptr;

} // Util_CreateBitmapList()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_DrawPoly()
//
///////////////////////////////////////////////////////////////////////////////////////
static void Util_DrawPoly(
	jeWorld		*World,	// world in which to draw poly
	const jeLVertex	*V1,	// top left
	const jeLVertex	*V2,	// top right
	const jeLVertex	*V3,	// bottom right
	const jeLVertex	*V4 )	// bottom left
{

	// locals
	jeUserPoly	*Poly;

	// ensure valid data
	assert( World != nullptr );
	assert( V1 != nullptr );
	assert( V2 != nullptr );
	assert( V3 != nullptr );
	assert( V4 != nullptr );

	// draw poly
	Poly = jeUserPoly_CreateQuad( V1, V2, V3, V4, nullptr, JE_RENDER_FLAG_ALPHA );
	jeWorld_AddUserPoly( World, Poly, JE_TRUE );
	jeUserPoly_Destroy( &Poly );

} // Util_DrawPoly()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_DrawExtBox()
//
///////////////////////////////////////////////////////////////////////////////////////
static void Util_DrawExtBox(
	jeWorld		*World,		// world to draw it in
	const JE_RGBA		*Color,		// color to draw it in
	const jeExtBox	*ExtBox )	// extent box to draw
{

	// locals
	jeLVertex	Vertex[4];
	int			i = 0;

	// ensure valid data
	assert( World != nullptr );
	assert( Color != nullptr );
	assert( ExtBox != nullptr );

	// init vert struct
	for ( i = 0; i < 4; i++ )
	{
		Vertex[i].a = Color->a;
		Vertex[i].r = Color->r;
		Vertex[i].g = Color->g;
		Vertex[i].b = Color->b;
	}
	Vertex[0].u = 0.0f;
	Vertex[0].v = 0.0f;
	Vertex[1].u = 1.0f;
	Vertex[1].v = 0.0f;
	Vertex[2].u = 1.0f;
	Vertex[2].v = 1.0f;
	Vertex[3].u = 0.0f;
	Vertex[3].v = 1.0f;

	// side 1
	Vertex[0].X = ExtBox->Min.X;
	Vertex[0].Y = ExtBox->Max.Y;
	Vertex[0].Z = ExtBox->Min.Z;
	Vertex[1].X = ExtBox->Max.X;
	Vertex[1].Y = ExtBox->Max.Y;
	Vertex[1].Z = ExtBox->Min.Z;
	Vertex[2].X = ExtBox->Max.X;
	Vertex[2].Y = ExtBox->Min.Y;
	Vertex[2].Z = ExtBox->Min.Z;
	Vertex[3].X = ExtBox->Min.X;
	Vertex[3].Y = ExtBox->Min.Y;
	Vertex[3].Z = ExtBox->Min.Z;
	Util_DrawPoly( World, &( Vertex[0] ), &( Vertex[1] ), &( Vertex[2] ), &( Vertex[3] ) );

	// side 2
	Vertex[0].X = ExtBox->Min.X;
	Vertex[0].Y = ExtBox->Max.Y;
	Vertex[0].Z = ExtBox->Max.Z;
	Vertex[1].X = ExtBox->Max.X;
	Vertex[1].Y = ExtBox->Max.Y;
	Vertex[1].Z = ExtBox->Max.Z;
	Vertex[2].X = ExtBox->Max.X;
	Vertex[2].Y = ExtBox->Min.Y;
	Vertex[2].Z = ExtBox->Max.Z;
	Vertex[3].X = ExtBox->Min.X;
	Vertex[3].Y = ExtBox->Min.Y;
	Vertex[3].Z = ExtBox->Max.Z;
	Util_DrawPoly( World, &( Vertex[0] ), &( Vertex[1] ), &( Vertex[2] ), &( Vertex[3] ) );

	// side 3
	Vertex[0].X = ExtBox->Min.X;
	Vertex[0].Y = ExtBox->Max.Y;
	Vertex[0].Z = ExtBox->Min.Z;
	Vertex[1].X = ExtBox->Min.X;
	Vertex[1].Y = ExtBox->Max.Y;
	Vertex[1].Z = ExtBox->Max.Z;
	Vertex[2].X = ExtBox->Max.X;
	Vertex[2].Y = ExtBox->Max.Y;
	Vertex[2].Z = ExtBox->Max.Z;
	Vertex[3].X = ExtBox->Max.X;
	Vertex[3].Y = ExtBox->Max.Y;
	Vertex[3].Z = ExtBox->Min.Z;
	Util_DrawPoly( World, &( Vertex[0] ), &( Vertex[1] ), &( Vertex[2] ), &( Vertex[3] ) );

	// side 4
	Vertex[0].X = ExtBox->Max.X;
	Vertex[0].Y = ExtBox->Max.Y;
	Vertex[0].Z = ExtBox->Min.Z;
	Vertex[1].X = ExtBox->Max.X;
	Vertex[1].Y = ExtBox->Max.Y;
	Vertex[1].Z = ExtBox->Max.Z;
	Vertex[2].X = ExtBox->Max.X;
	Vertex[2].Y = ExtBox->Min.Y;
	Vertex[2].Z = ExtBox->Max.Z;
	Vertex[3].X = ExtBox->Max.X;
	Vertex[3].Y = ExtBox->Min.Y;
	Vertex[3].Z = ExtBox->Min.Z;
	Util_DrawPoly( World, &( Vertex[0] ), &( Vertex[1] ), &( Vertex[2] ), &( Vertex[3] ) );

	// side 5
	Vertex[0].X = ExtBox->Max.X;
	Vertex[0].Y = ExtBox->Min.Y;
	Vertex[0].Z = ExtBox->Min.Z;
	Vertex[1].X = ExtBox->Max.X;
	Vertex[1].Y = ExtBox->Min.Y;
	Vertex[1].Z = ExtBox->Max.Z;
	Vertex[2].X = ExtBox->Min.X;
	Vertex[2].Y = ExtBox->Min.Y;
	Vertex[2].Z = ExtBox->Max.Z;
	Vertex[3].X = ExtBox->Min.X;
	Vertex[3].Y = ExtBox->Min.Y;
	Vertex[3].Z = ExtBox->Min.Z;
	Util_DrawPoly( World, &( Vertex[0] ), &( Vertex[1] ), &( Vertex[2] ), &( Vertex[3] ) );

	// side 6
	Vertex[0].X = ExtBox->Min.X;
	Vertex[0].Y = ExtBox->Min.Y;
	Vertex[0].Z = ExtBox->Min.Z;
	Vertex[1].X = ExtBox->Min.X;
	Vertex[1].Y = ExtBox->Min.Y;
	Vertex[1].Z = ExtBox->Max.Z;
	Vertex[2].X = ExtBox->Min.X;
	Vertex[2].Y = ExtBox->Max.Y;
	Vertex[2].Z = ExtBox->Max.Z;
	Vertex[3].X = ExtBox->Min.X;
	Vertex[3].Y = ExtBox->Max.Y;
	Vertex[3].Z = ExtBox->Min.Z;
	Util_DrawPoly( World, &( Vertex[0] ), &( Vertex[1] ), &( Vertex[2] ), &( Vertex[3] ) );

} // Util_DrawExtBox()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_CreateEmptyList()
//
////////////////////////////////////////////////////////////////////////////////////////
static jeBoolean Util_CreateEmptyList(
	char	***List,		// where to save list pointer
	int		*ListSize )		// where to save list size
{

	// locals
	char	**NewList = nullptr;

	// ensure valid data
	assert( List != nullptr );
	assert( ListSize != nullptr );

	// reset passed in data
	*List = nullptr;
	*ListSize = 0;

	// allocate list
	NewList = static_cast<char**>(JE_RAM_ALLOCATE( sizeof( char * ) ));
	if ( NewList == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		return JE_FALSE;
	}

	// init data
	NewList[0] = Util_StrDup( NoSelection );
	*ListSize = 1;

	// all done
	*List = NewList;
	return JE_TRUE;

} // Util_CreateEmptyList()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_WriteString()
//
////////////////////////////////////////////////////////////////////////////////////////
static jeBoolean Util_WriteString(
	jeVFile	*File,		// file to write to
	const std::string	&String )	// string to write out
{

	// locals
	int			Size = 0;
	jeBoolean	Result = JE_TRUE;

	// ensure valid data
	assert( File != nullptr );
	assert( !String.empty() );

	// write out complete
	Size = String.length() + 1;
	assert( Size > 0 );
	Result &= jeVFile_Write( File, &Size, sizeof( Size ) );
	Result &= jeVFile_Write( File, String.c_str(), Size );

	// log errors
	if ( Result != JE_TRUE )
	{
		jeErrorLog_Add( JE_ERR_FILEIO_WRITE, nullptr );
	}

	// all done
	return Result;

} // Util_WriteString()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_DestroyFileList()
//
////////////////////////////////////////////////////////////////////////////////////////
static void Util_DestroyFileList(
	char	***DeadList,		// list to destroy
	const int		*DeadListSize )		// size of list
{

	// locals
	char	**List = nullptr;
	int		ListSize = 0;
	int		i = 0;

	// save pointers
	assert( DeadList != nullptr );
	List = *DeadList;
	assert( List != nullptr );
	assert( DeadListSize != nullptr );
	ListSize = *DeadListSize;
	assert( ListSize > 0 );

	// free all strings
	for ( i = 0; i < ListSize; i++ )
	{
		if ( List[i] != nullptr )
		{
			JE_RAM_FREE( List[i] );
			List[i] = nullptr;
		}
	}

	// free the list itself
	JE_RAM_FREE( List );

	// zap final data
	List = nullptr;
	ListSize = 0;

} // Util_DestroyFileList()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_BuildFileList()
//
////////////////////////////////////////////////////////////////////////////////////////
static char ** Util_BuildFileList(
	jet3d::jeResourceMgr	*ResourceMgr,
	const std::string		&ResourceName,
	const std::string		&FileFilter,
	int				*FileListSize )
{

	// locals
	jeVFile			*FileDir = nullptr;
	jeVFile_Finder	*Finder = nullptr;
	int				TotalFiles = 0;
	int				CurFile = 0;
	char			**FileList = nullptr;

	// ensure valid data
	assert( ResourceMgr != nullptr );
	assert( ResourceName != nullptr );
	assert( FileFilter != nullptr );
	assert( FileListSize );

	// get vfile dir
	//FileDir = jeResource_GetVFile( ResourceMgr, ResourceName );
	FileDir = ResourceMgr->getVFile(ResourceName);
	if ( FileDir == nullptr )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
		return nullptr;
	}

	// create directory finder
	Finder = jeVFile_CreateFinder( FileDir, FileFilter.c_str() );
	if ( Finder == nullptr )
	{
		goto ERROR_Util_BuildFileList;
	}

	// determine how many files there are
	TotalFiles = 1;
	while ( jeVFile_FinderGetNextFile( Finder ) == JE_TRUE )
	{
		TotalFiles++;
	}

	// destroy finder
	jeVFile_DestroyFinder( Finder );
	Finder = nullptr;

	// allocate file list
	FileList = static_cast<char**>(JE_RAM_ALLOCATE_CLEAR( sizeof( char * ) * TotalFiles ));
	if ( FileList == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		goto ERROR_Util_BuildFileList;
	}

	// create directory finder
	Finder = jeVFile_CreateFinder( FileDir, FileFilter.c_str() );
	if ( Finder == nullptr )
	{
		jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
		goto ERROR_Util_BuildFileList;
	}

	// first entry is always the "no selection" slot
	CurFile = 0;
	FileList[CurFile++] = Util_StrDup( NoSelection );

	// build file list
	while ( jeVFile_FinderGetNextFile( Finder ) == JE_TRUE )
	{

		// locals
		jeVFile_Properties	Properties;

		// get properties of current file
		if( jeVFile_FinderGetProperties( Finder, &Properties ) == JE_FALSE )
		{
			jeErrorLog_Add( JE_ERR_SUBSYSTEM_FAILURE, nullptr );
			continue;
		}

		// save file name
		assert( CurFile < TotalFiles );
		FileList[CurFile] = Util_StrDup( Properties.Name );
		if ( FileList[CurFile] != nullptr )
		{
			CurFile++;
		}
	}

	// destroy finder
	jeVFile_DestroyFinder( Finder );

	// close vfile dir
	//if ( jeResource_DeleteVFile( ResourceMgr, ResourceName ) == 0 )
	if (!ResourceMgr->removeVFile(ResourceName))
	{
		jeVFile_Close( FileDir );
	}

	// return file list
	*FileListSize = CurFile;
	return FileList;

	// error handling
	ERROR_Util_BuildFileList:

	// destroy file list
	if ( FileList != nullptr )
	{
		CurFile = 0;
		while ( CurFile < TotalFiles )
		{
			if ( FileList[CurFile] != nullptr )
			{
				JE_RAM_FREE( FileList[CurFile] );
			}
			CurFile++;
		}
		JE_RAM_FREE( FileList );
	}

	// destroy finder
	if ( Finder != nullptr )
	{
		jeVFile_DestroyFinder( Finder );
	}

	// close vfile dir
	if ( FileDir != nullptr )
	{
		//if ( jeResource_DeleteVFile( ResourceMgr, ResourceName ) == 0 )
		if (!ResourceMgr->removeVFile(ResourceName))
		{
			jeVFile_Close( FileDir );
		}
	}

	// zap file list size
	*FileListSize = 0;

	// return failure
	return nullptr;

} // Util_BuildFileList()



////////////////////////////////////////////////////////////////////////////////////////
//
//	Util_LoadLibraryString()
//
////////////////////////////////////////////////////////////////////////////////////////
static char * Util_LoadLibraryString(
	HINSTANCE		hInstance,
	unsigned int	ID )
{

	// locals
	#define		MAX_STRING_SIZE	255
	static char	StringBuf[MAX_STRING_SIZE];
	char		*NewString = nullptr;
	int			Size = 0;

	// ensure valid data
	assert( hInstance != nullptr );
	assert( ID >= 0 );

	// get resource string
	Size = LoadString( hInstance, ID, StringBuf, MAX_STRING_SIZE );
	if ( Size <= 0 )
	{
		jeErrorLog_Add( JE_ERR_WINDOWS_API_FAILURE, nullptr );
		return nullptr;
	}

	// copy resource string
	NewString = static_cast<char *>(JE_RAM_ALLOCATE( Size + 1 ));
	if ( NewString == nullptr )
	{
		jeErrorLog_Add( JE_ERR_MEMORY_RESOURCE, nullptr );
		return nullptr;
	}
	strcpy( NewString, StringBuf );

	// all done
	return NewString;

} // Util_LoadLibraryString()
