
#include "../sys/inc.h"
#include "ms3d.h"

//int m->m_frame = 0;

void ms3dinit(ms3d *m)
{
	m->m_numMeshes = 0;
	m->m_pMeshes = NULL;
	m->m_numMaterials = 0;
	m->m_pMaterials = NULL;
	m->m_numTriangles = 0;
	m->m_pTriangles = NULL;
	m->m_numVertices = 0;
	m->m_pVertices = NULL;
	m->m_numJoints = 0;
	m->m_pJoints = NULL;
}

void ms3dfree(ms3d *m)
{
	ms3ddestroy(m);
}

void ms3ddestroy(ms3d *m)
{
	int i;
	for ( i = 0; i < m->m_numMeshes; i++ )
		free( m->m_pMeshes[i].m_pTriangleIndices);
	for ( i = 0; i < m->m_numMaterials; i++ )
		free( m->m_pMaterials[i].m_pTextureFilename);

	m->m_numMeshes = 0;
	if ( m->m_pMeshes != NULL )
	{
		free( m->m_pMeshes);
		m->m_pMeshes = NULL;
	}

	m->m_numMaterials = 0;
	if ( m->m_pMaterials != NULL )
	{
		free( m->m_pMaterials);
		m->m_pMaterials = NULL;
	}

	m->m_numTriangles = 0;
	if ( m->m_pTriangles != NULL )
	{
		free( m->m_pTriangles);
		m->m_pTriangles = NULL;
	}

	m->m_numVertices = 0;
	if ( m->m_pVertices != NULL )
	{
		free( m->m_pVertices);
		m->m_pVertices = NULL;
	}

	m->m_numJoints = 0;
	if(m->m_pJoints != NULL)
	{
		free( m->m_pJoints);
		m->m_pJoints = NULL;
	}

	free(m->mconv);
	m->mconv = NULL;
}

dbool ms3dload(ms3d *m, const char *relative, unsigned int* diffm, unsigned int* specm, unsigned int* normm, unsigned int* ownm, dbool dontqueue)
{
	//char full[SPE_MAX_PATH+1];
	//FullPath(relative, full);
	
	FILE* fp = fopen(relative, "rb");

	//std::ifstream inputFile(relative, std::ios::in | std::ios::binary );
	//if ( inputFile.fail())
	if (!fp)
	{
		//Log("Couldn't show the model file %s ", relative);

		//char msg[SPE_MAX_PATH+1];
		//sprintf(msg, "Couldn't show the model file %s", relative);

		//ErrMess("Error", msg);
		printf("failed to load model\r\n");

		return dfalse;
	}

	//std::string reltemp = StripFile(relative);

	//if(strlen(reltemp.c_str()) == 0)
	//	reltemp += CORRECT_SLASH;

	//strcpy(m->m_relative, reltemp.c_str());
	strcpy(m->m_relative, relative);

	/*
	char pathTemp[SPE_MAX_PATH+1];
	int pathLength;
	for ( pathLength = strlen( filename ); --pathLength; )
	{
		if ( filename[pathLength] == '/' || filename[pathLength] == '\\' )
			break;
	}
	strncpy( pathTemp, filename, pathLength );

	int i;
	if ( pathLength > 0 )
	{
		pathTemp[pathLength++] = '/';
	}

	strncpy( m->m_filepath, filename, pathLength );
	*/

	//inputFile.seekg( 0, std::ios::end );
	fseek(fp, 0, SEEK_END);
	//long fileSize = inputFile.tellg();
	long fileSize = ftell(fp);
	//inputFile.seekg( 0, std::ios::beg );
	fseek(fp, 0, SEEK_SET);

	char *pBuffer = (char*)malloc(sizeof( char)*fileSize);
	//inputFile.read( pBuffer, fileSize );
	//inputFile.close();
	fread(pBuffer, 1, fileSize, fp);
	fclose(fp);

	char *pPtr = pBuffer;
	struct MS3DHeader *pHeader = ( struct MS3DHeader* )pPtr;
	pPtr += sizeof( struct MS3DHeader );

	if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
	{
		//Log("Not an MS3D file %s", relative);
		printf("not a model %s %d [%d] %d\r\n", pHeader->m_ID, sizeof(struct MS3DHeader), (int) pHeader->m_ID[4], fileSize);
		return dfalse;
    }

	if ( pHeader->m_version < 3 )
	{
		//Log("I know nothing about MS3D v1.2, %s" , relative);

		//char msg[SPE_MAX_PATH+1];
		//sprintf(msg, "Incompatible MS3D v1.2 ", relative);

		//ErrMess("Error", msg);
		printf("model is 1.2\r\n");

		return dfalse;
	}

	int nVertices = *( word* )pPtr;
	m->m_numVertices = nVertices;
	m->m_pVertices = (struct Vertex*)malloc(sizeof( struct Vertex)*nVertices);
	pPtr += sizeof( word );
	char* vstart = pPtr;

	m->mconv = (v3f*)malloc(sizeof( v3f)*nVertices);

	int i;
	for ( i = 0; i < nVertices; i++ )
	{
		m->mconv[i].x = m->mconv[i].y = m->mconv[i].z =9999999;
		struct MS3DVertex *pVertex = ( struct MS3DVertex* )pPtr;
		m->m_pVertices[i].m_boneID = pVertex->m_boneID;
		memcpy( m->m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
		memcpy(&m->mconv[i], m->m_pVertices[i].m_location, sizeof(float) * 3);
		pPtr += sizeof( struct MS3DVertex );
	}

	int nTriangles = *( word* )pPtr;
	m->m_numTriangles = nTriangles;
	m->m_pTriangles = (struct Triangle*)malloc(sizeof( struct Triangle)*nTriangles);
	pPtr += sizeof( word );

	for ( i = 0; i < nTriangles; i++ )
	{
		struct MS3DTriangle *pTriangle = ( struct MS3DTriangle* )pPtr;
		int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
		float t[3] = { 1.0f-pTriangle->m_t[0], 1.0f-pTriangle->m_t[1], 1.0f-pTriangle->m_t[2] };
		memcpy( m->m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
		memcpy( m->m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
		memcpy( m->m_pTriangles[i].m_t, t, sizeof( float )*3 );
		memcpy( m->m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
		pPtr += sizeof( struct MS3DTriangle );
	}

	int nGroups = *( word* )pPtr;
	m->m_numMeshes = nGroups;
	m->m_pMeshes = (struct Mesh*)malloc(sizeof( struct Mesh)*nGroups);
	pPtr += sizeof( word );
	for ( i = 0; i < nGroups; i++ )
	{
		pPtr += sizeof( byte );	// flags
		pPtr += 32;				// name

		word nTriangles = *( word* )pPtr;
		pPtr += sizeof( word );
		int *pTriangleIndices = (int*)malloc(sizeof(int)*nTriangles);
		for ( int j = 0; j < nTriangles; j++ )
		{
			pTriangleIndices[j] = *( word* )pPtr;
			pPtr += sizeof( word );
		}

		char materialIndex = *( char* )pPtr;
		pPtr += sizeof( char );

		m->m_pMeshes[i].m_materialIndex = materialIndex;
		m->m_pMeshes[i].m_numTriangles = nTriangles;
		m->m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
	}

	int nMaterials = *( word* )pPtr;
	m->m_numMaterials = nMaterials;
	m->m_pMaterials = (struct Material*)malloc(sizeof(struct Material)*nMaterials);
	pPtr += sizeof( word );
	for ( i = 0; i < nMaterials; i++ )
	{
		struct MS3DMaterial *pMaterial = ( struct MS3DMaterial* )pPtr;
		memcpy( m->m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
		memcpy( m->m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
		memcpy( m->m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
		memcpy( m->m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
		m->m_pMaterials[i].m_shininess = pMaterial->m_shininess;
		if ( strncmp( pMaterial->m_diffusem, ".\\", 2 ) == 0 ) {
			// MS3D 1.5.x relative path
			//StripPath(pMaterial->m_diffusem);
			//m->m_pMaterials[i].m_pTextureFilename = new char[ strlen(relativepath.c_str()) + strlen(pMaterial->m_diffusem) + 1 ];
			//strcpy( m->m_pMaterials[i].m_pTextureFilename, reltemp.c_str() );
			//sprintf(m->m_pMaterials[i].m_pTextureFilename, "%s%s", relativepath.c_str(), pMaterial->m_diffusem);
			m->m_pMaterials[i].m_pTextureFilename = (char*)malloc(sizeof( char)*(strlen( pMaterial->m_diffusem )+1));
			strcpy( m->m_pMaterials[i].m_pTextureFilename, pMaterial->m_diffusem );
		}
		else {
			// MS3D 1.4.x or earlier - absolute path
			m->m_pMaterials[i].m_pTextureFilename = (char*)malloc(sizeof(char)*(strlen(pMaterial->m_diffusem) + 1));
			strcpy( m->m_pMaterials[i].m_pTextureFilename, pMaterial->m_diffusem );
		}
		//StripPath(m->m_pMaterials[i].m_pTextureFilename);
		pPtr += sizeof( struct MS3DMaterial );
	}

	//loadtex(diffm, specm, normm, ownm, dontqueue);

	float animFPS = *( float* )pPtr;
	pPtr += sizeof( float );

	// skip currentTime
	pPtr += sizeof( float );

	m->m_totalFrames = *( int* )pPtr;
	pPtr += sizeof( int );

	m->m_totalTime = m->m_totalFrames*1000.0/animFPS;

	m->m_numJoints = *( word* )pPtr;
	pPtr += sizeof( word );

	m->m_pJoints = (struct Joint*)malloc(sizeof( struct Joint)*m->m_numJoints);

	struct JointNameListRec
	{
		int m_jointIndex;
		const char *m_pName;
	};

	const char *pTempPtr = pPtr;

	struct JointNameListRec *pNameList = (struct JointNameListRec*)malloc(sizeof( struct JointNameListRec)*m->m_numJoints);
	for ( i = 0; i < m->m_numJoints; i++ )
	{
		struct MS3DJoint *pJoint = ( struct MS3DJoint* )pTempPtr;
		pTempPtr += sizeof( struct MS3DJoint );
		pTempPtr += sizeof( struct MS3DKeyframe )*( pJoint->m_numRotationKeyframes+pJoint->m_numTranslationKeyframes );

		pNameList[i].m_jointIndex = i;
		pNameList[i].m_pName = pJoint->m_name;
	}

	for ( i = 0; i < m->m_numJoints; i++ )
	{
		struct MS3DJoint *pJoint = ( struct MS3DJoint* )pPtr;
		pPtr += sizeof( struct MS3DJoint );

		int j, parentIndex = -1;
		if ( strlen( pJoint->m_parentName ) > 0 )
		{
			for ( j = 0; j < m->m_numJoints; j++ )
			{
				if ( _stricmp( pNameList[j].m_pName, pJoint->m_parentName ) == 0 )
				{
					parentIndex = pNameList[j].m_jointIndex;
					break;
				}
			}
			if ( parentIndex == -1 ) {
				//Log("Unable to find parent bone in MS3D file");
				printf("failed to load model bone\r\n");
				return dfalse;
			}
		}

		memcpy( m->m_pJoints[i].m_localRotation, pJoint->m_rotation, sizeof( float )*3 );
		memcpy( m->m_pJoints[i].m_localTranslation, pJoint->m_translation, sizeof( float )*3 );
		m->m_pJoints[i].m_parent = parentIndex;
		m->m_pJoints[i].m_numRotationKeyframes = pJoint->m_numRotationKeyframes;
		m->m_pJoints[i].m_pRotationKeyframes = (struct Keyframe*)malloc(sizeof(struct Keyframe)*pJoint->m_numRotationKeyframes);
		m->m_pJoints[i].m_numTranslationKeyframes = pJoint->m_numTranslationKeyframes;
		m->m_pJoints[i].m_pTranslationKeyframes = (struct Keyframe*)malloc(sizeof(struct Keyframe)*pJoint->m_numTranslationKeyframes);

		for ( j = 0; j < pJoint->m_numRotationKeyframes; j++ )
		{
			struct MS3DKeyframe *pKeyframe = ( struct MS3DKeyframe* )pPtr;
			pPtr += sizeof( struct MS3DKeyframe );

			ms3dsetjointkf(m, i, j, pKeyframe->m_time*1000.0f, pKeyframe->m_parameter, dtrue );
		}

		for ( j = 0; j < pJoint->m_numTranslationKeyframes; j++ )
		{
			struct MS3DKeyframe *pKeyframe = ( struct MS3DKeyframe* )pPtr;
			pPtr += sizeof( struct MS3DKeyframe );

			ms3dsetjointkf(m, i, j, pKeyframe->m_time*1000.0f, pKeyframe->m_parameter, dfalse );
		}
	}

	free( pNameList);

	//setupjoints();

	free( pBuffer);

	//restart();

	//Log(relative);

	return dtrue;
}

void ms3dsetjointkf(ms3d *m, int jointIndex, int keyframeIndex, float time, float *parameter, dbool isRotation )
{
	struct Keyframe* keyframe = isRotation ? &m->m_pJoints[jointIndex].m_pRotationKeyframes[keyframeIndex] :
		&m->m_pJoints[jointIndex].m_pTranslationKeyframes[keyframeIndex];

	keyframe->m_jointIndex = jointIndex;
	keyframe->m_time = time;
	memcpy( keyframe->m_parameter, parameter, sizeof( float )*3 );
}

