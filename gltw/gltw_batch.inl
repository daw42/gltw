
namespace gltw {

	inline VertexBatch::VertexBatch( GLenum mode, GLuint numVerts, int attribs, GLenum hint ) :
		attributes(attribs), nVerts(numVerts),  bufferUsage(hint), drawMode(mode), vaID(0)
	{
		for( int i = 0; i < NUM_BUFFERS; i++) bufIDs[i] = 0;
		if( !attribEnabled(ATTRIB_POSITION) ) {
			cerr << "Error in VertexBatch constructor:  VertexBatch must include the position attribute." << endl;
			exit(1);
		}
	}

	inline VertexBatch::~VertexBatch() {
		// Delete buffers/vertex arrays safely ignores 0s 
		glDeleteBuffers(NUM_BUFFERS, bufIDs);
		glDeleteVertexArrays(1, &vaID);
	}

	inline bool VertexBatch::attribEnabled( Attribute attrib ) {
		return (attrib & attributes) != 0;
	}

	inline void VertexBatch::copyPositionData( GLfloat * data ) {
		if( bufIDs[ POSITION ] == 0 ) {
			glGenBuffers(1, &bufIDs[POSITION] );
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[POSITION] );
			glBufferData( GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * nVerts, NULL, bufferUsage);			
		}
		glBindBuffer( GL_ARRAY_BUFFER, bufIDs[POSITION]);
		glBufferSubData( GL_ARRAY_BUFFER, 0, 3 * sizeof(GLfloat)*nVerts, data); 
	}

	inline void VertexBatch::copyNormalData( GLfloat *data ) {
		if( !attribEnabled(ATTRIB_NORMAL) ) {
			cerr << "Error in VertexBatch.copyNormalData: the normal attribute was not selected for this VertexBatch." << endl;
			return;
		}

		if( bufIDs[ NORMAL ] == 0 ) {
			glGenBuffers(1, &bufIDs[NORMAL] );
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[NORMAL] );
			glBufferData( GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * nVerts, NULL, bufferUsage);			
		}
		glBindBuffer( GL_ARRAY_BUFFER, bufIDs[NORMAL]);
		glBufferSubData( GL_ARRAY_BUFFER, 0, 3 * sizeof(GLfloat)*nVerts, data); 
	}

	inline void VertexBatch::copyColorData( GLfloat * data ) {
		if( !attribEnabled(ATTRIB_COLOR) ) {
			cerr << "Error in VertexBatch.copyColorData: the color attribute was not selected for this VertexBatch." << endl;
			return;
		}
		if( bufIDs[ COLOR ] == 0 ) {
			glGenBuffers(1, &bufIDs[COLOR] );
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[COLOR] );
			glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * nVerts, NULL, bufferUsage);			
		}
		glBindBuffer( GL_ARRAY_BUFFER, bufIDs[COLOR]);
		glBufferSubData( GL_ARRAY_BUFFER, 0, 4 * sizeof(GLfloat)*nVerts, data); 
	}

	inline bool VertexBatch::isReady() {
		return !(
			( attribEnabled(ATTRIB_POSITION) && bufIDs[POSITION] == 0 ) ||
			( attribEnabled(ATTRIB_COLOR) && bufIDs[COLOR] == 0 ) ||
			( attribEnabled(ATTRIB_NORMAL) && bufIDs[NORMAL] == 0 )
			);
	}

	inline void VertexBatch::buildVertexArray() {
		glGenVertexArrays( 1, &vaID );
		glBindVertexArray(vaID);
			
		if( attribEnabled(ATTRIB_POSITION) && bufIDs[POSITION] != 0 ) {
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[POSITION] );
			glVertexAttribPointer( GLTW_ATTRIB_IDX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(GLTW_ATTRIB_IDX_POSITION);
		}

		if( attribEnabled(ATTRIB_COLOR) && bufIDs[COLOR] != 0 ) {
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[COLOR] );
			glVertexAttribPointer( GLTW_ATTRIB_IDX_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(GLTW_ATTRIB_IDX_COLOR);
		}

		if( attribEnabled(ATTRIB_NORMAL) && bufIDs[NORMAL] != 0 ) {
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[NORMAL] );
			glVertexAttribPointer( GLTW_ATTRIB_IDX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(GLTW_ATTRIB_IDX_NORMAL);
		}
		glBindVertexArray(0);
	}

	inline void VertexBatch::draw() {
		if( ! isReady() ) {
			cerr << "VertexBatch is not ready to draw.  Missing some vertex data." << endl;
			return;
		}

		if( vaID == 0 ) {
			if( bufIDs[POSITION] == 0 ) {
				cerr << "No position data available in VertexBatch!" << endl;
				return;
			}
			buildVertexArray();
		}

		glBindVertexArray(vaID);
		glDrawArrays( drawMode, 0, nVerts );
		glBindVertexArray(0);
	}

	inline TriangleMesh::TriangleMesh(GLuint numVerts, GLuint numElements, int attributes, GLenum usage) :
		VertexBatch(GL_TRIANGLES, numVerts, attributes, usage), nElements(numElements) 
	{
		if( !attribEnabled(ATTRIB_NORMAL) ) {
			cerr << "Error in TriangleMesh constructor:  TriangleMesh must include the normal attribute." << endl;
			exit(1);
		}
	}

	inline void TriangleMesh::copyElementData( GLuint * data )
	{
		if( bufIDs[ ELEMENT ] == 0 ) {
			glGenBuffers(1, &bufIDs[ELEMENT] );
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[ELEMENT] );
			glBufferData( GL_ARRAY_BUFFER, sizeof(GLuint) * nElements, NULL, bufferUsage);			
		}
		glBindBuffer( GL_ARRAY_BUFFER, bufIDs[ELEMENT]);
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(GLuint)*nElements, data);
	}

	inline void TriangleMesh::buildVertexArray() {
		VertexBatch::buildVertexArray();

		glBindVertexArray(vaID);
		if( bufIDs[ELEMENT] != 0 ) {
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufIDs[ELEMENT] );
		}
		glBindVertexArray(0);
	}

	inline void TriangleMesh::draw()
	{
		if( ! isReady() ) {
			cerr << "TriangleMesh is not ready to draw.  Missing some vertex data." << endl;
			return;
		}

		if( vaID == 0 ) {
			if( bufIDs[POSITION] == 0 ) {
				cerr << "No position data available in VertexBatch!" << endl;
				return;
			}
			buildVertexArray();
		}

		glBindVertexArray(vaID);
		glDrawElements(drawMode, nElements, GL_UNSIGNED_INT, 0 );
		glBindVertexArray(0);
	}

	inline TriangleMesh * buildTorus( GLfloat outerRadius, GLfloat innerRadius, GLint nSides, GLint nRings ) {
		GLint nVerts = nSides * (nRings+1);
		GLint faces = nSides * nRings;
		
		float ringFactor  = (float)(2.0 * GLTW_PI / nRings);
		float sideFactor = (float)(2.0 * GLTW_PI / nSides);
		int idx = 0;

		GLfloat *verts = new GLfloat[ nVerts * 3 ];
		GLfloat *norms = new GLfloat[ nVerts * 3 ];
		GLuint *el = new GLuint[faces * 6];

		for( int ring = 0; ring <= nRings; ring++ ) {
			float u = ring * ringFactor;
			float cu = cos(u);
			float su = sin(u);
			for( int side = 0; side < nSides; side++ ) {
				float v = side * sideFactor;
				float cv = cos(v);
				float sv = sin(v);
				float r = (outerRadius + innerRadius * cv);
				verts[idx] = r * cu;
				verts[idx + 1] = r * su;
				verts[idx + 2] = innerRadius * sv;
				norms[idx] = cv * cu * r;
				norms[idx + 1] = cv * su * r;
				norms[idx + 2] = sv * r;
				// Normalize
				float len = sqrt( norms[idx] * norms[idx] +
					norms[idx+1] * norms[idx+1] +
					norms[idx+2] * norms[idx+2] );
				norms[idx] /= len;
				norms[idx+1] /= len;
				norms[idx+2] /= len;
				idx += 3;
			}
		}

		idx = 0;
		for( int ring = 0; ring < nRings; ring++ ) {
			int ringStart = ring * nSides;
			int nextRingStart = (ring + 1) * nSides;
			for( int side = 0; side < nSides; side++ ) {
				int nextSide = (side+1) % nSides;
				// The quad
				el[idx] = (ringStart + side);
				el[idx+1] = (nextRingStart + side);
				el[idx+2] = (nextRingStart + nextSide);
				el[idx+3] = ringStart + side;
				el[idx+4] = nextRingStart + nextSide;
				el[idx+5] = (ringStart + nextSide);
				idx += 6;
			}
		}

		TriangleMesh *mesh = new TriangleMesh(nVerts, 6 * faces);
		mesh->copyPositionData(verts);
		mesh->copyNormalData(norms);
		mesh->copyElementData(el);

		delete [] verts;
		delete [] norms;
		delete [] el;

		return mesh;
	}

	inline TriangleMesh * buildCylinder( float base, float top, float height, int slices, int stacks )
	{
		GLuint elements = slices * stacks * 6;
		GLuint nVerts = slices * (stacks + 1);

		// Allocate temporary space for the vertex data
		GLfloat * verts;
		GLfloat * normals;
		GLuint * el;
		verts = new GLfloat[ nVerts * 3 ];
		normals = new GLfloat[ nVerts * 3 ];
		el = new GLuint[ elements ];

		// Generate the points
		GLuint vIdx = 0;
		GLfloat x, y, z, nx, ny, nlen;
		float sliceFac = 2.0 * GLTW_PI / slices;
		float stackFac = height / stacks;
		float angle = 0.0f, alpha = 0.0f, r = 0.0f;
		float normZ = (base - top) / height;
		for( int i = 0; i <= stacks ; i++ ) {
			z = i * stackFac;
			alpha = z / height;
			r = (1 - alpha) * base + alpha * top;
			for( int j = 0; j < slices; j++ ) {
				angle = sliceFac * j;
				x = nx = cosf(angle);
				y = ny = sinf(angle);
				nlen = sqrt(nx * nx + ny * ny + normZ * normZ);
				x *= r;
				y *= r;
				verts[vIdx] = x;
				verts[vIdx+1] = y;
				verts[vIdx+2] = z;
				normals[vIdx] = nx / nlen;
				normals[vIdx+1] = ny / nlen;
				normals[vIdx+2] = normZ / nlen;
				vIdx += 3;
			}
		}

		// Generate the element indexes for triangles
		GLuint elIdx = 0, stackStart, nextStackStart;
		for( int i = 0; i < stacks; i++ ) {
			stackStart = i * slices;
			nextStackStart = (i+1) * slices;
			for( int j = 0; j < slices; j++ ) {
				// Triangle one
				el[elIdx] = stackStart + j;
				el[elIdx+1] = nextStackStart + j;
				el[elIdx+2] = nextStackStart + ((j+1) % slices);
				// Triangle 2
				el[elIdx+3] = stackStart + j;
				el[elIdx+4] = nextStackStart + ((j+1) % slices);
				el[elIdx+5] = stackStart + ((j+1) % slices);
				elIdx += 6;
			}
		}

		TriangleMesh *mesh = new TriangleMesh(nVerts, elements);
		mesh->copyPositionData(verts);
		mesh->copyNormalData(normals);
		mesh->copyElementData(el);

		// Delete the local arrays, we don't need them anymore.
		delete [] verts;
		delete [] normals;
		delete [] el;

		return mesh;
	}

	inline TriangleMesh * buildCube() 
	{
		float side = 1.0f;
		float side2 = side / 2.0f;

		float v[] = {
			// Front
			-side2, -side2, side2, side2, -side2, side2, side2,  side2, side2, -side2,  side2, side2,
			// Right
			side2, -side2, side2, side2, -side2, -side2, side2,  side2, -side2, side2,  side2, side2,
			// Back
			-side2, -side2, -side2, -side2,  side2, -side2, side2,  side2, -side2, side2, -side2, -side2,
			// Left
			-side2, -side2, side2, -side2,  side2, side2, -side2,  side2, -side2, -side2, -side2, -side2,
			// Bottom
			-side2, -side2, side2, -side2, -side2, -side2, side2, -side2, -side2, side2, -side2, side2,
			// Top
			-side2,  side2, side2, side2,  side2, side2, side2,  side2, -side2, -side2,  side2, -side2
		};

		float n[] = {
			// Front
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			// Right
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			// Back
			0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
			// Left
			-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			// Bottom
			0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			// Top
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
		};

		GLuint el[] = {
			0,1,2,0,2,3,
			4,5,6,4,6,7,
			8,9,10,8,10,11,
			12,13,14,12,14,15,
			16,17,18,16,18,19,
			20,21,22,20,22,23
		};

		TriangleMesh *mesh = new TriangleMesh(24, 36);
		mesh->copyPositionData(v);
		mesh->copyNormalData(n);
		mesh->copyElementData(el);

		return mesh;
	}

	inline TriangleMesh * buildSphere(GLfloat radius, int slices, int stacks)
	{
		GLuint nVerts = slices * (stacks + 1);
		GLuint elements = (slices * 2 * (stacks-1) ) * 3;

		// Allocate the temporary arrays for the vertex data
		GLfloat *verts = new GLfloat[nVerts*3];
		GLfloat *norms = new GLfloat[nVerts*3];
		GLuint * el = new GLuint[elements];

		// Generate positions and normals
		GLfloat theta, phi;
		GLfloat thetaFac = (2.0 * GLTW_PI) / slices;
		GLfloat phiFac = GLTW_PI / stacks;
		GLfloat nx, ny, nz;
		GLuint idx = 0;
		for( int i = 0; i < slices; i++ ) {
			theta = i * thetaFac;
			for( int j = 0; j <= stacks; j++ ) {
				phi = j * phiFac;
				nx = sinf(phi) * cosf(theta);
				ny = sinf(phi) * sinf(theta);
				nz = cosf(phi);
				verts[idx] = radius * nx; verts[idx+1] = radius * ny; verts[idx+2] = radius * nz;
				norms[idx] = nx; norms[idx+1] = ny; norms[idx+2] = nz;
				idx += 3;
			}
		}

		// Generate the element list
		idx = 0;
		for( int i = 0; i < slices; i++ ) {
			GLuint stackStart = i * (stacks + 1);
			GLuint nextStackStart = ((i+1) % slices) * (stacks+1);
			for( int j = 0; j < stacks; j++ ) {
				if( j == 0 ) {
					el[idx] = stackStart;
					el[idx+1] = stackStart + 1;
					el[idx+2] = nextStackStart + 1;
					idx += 3;
				} else if( j == stacks - 1) {
					el[idx] = stackStart + j;
					el[idx+1] = stackStart + j + 1;
					el[idx+2] = nextStackStart + j;
					idx += 3;
				} else {
					el[idx] = stackStart + j;
					el[idx+1] = stackStart + j + 1;
					el[idx+2] = nextStackStart + j + 1;
					el[idx+3] = nextStackStart + j;
					el[idx+4] = stackStart + j;
					el[idx+5] = nextStackStart + j + 1;
					idx += 6;
				}
			}
		}


		TriangleMesh * mesh = new TriangleMesh(nVerts,elements);
		mesh->copyPositionData(verts);
		mesh->copyNormalData(norms);
		mesh->copyElementData(el);

		// Delete our vertex data, we don't need it anymore
		delete [] verts;
		delete [] norms;
		delete [] el;

		return mesh;
	}

	inline TriangleMesh * buildPlane(float xsize, float zsize, int xdivs, int zdivs)
	{
		if( xdivs < 1 ) xdivs = 1;
		if( zdivs < 1 ) zdivs = 1;

		float * v = new float[3 * (xdivs + 1) * (zdivs + 1)];
		float * n = new float[3 * (xdivs + 1) * (zdivs + 1)];
		GLuint * el = new GLuint[6 * xdivs * zdivs];

		float x2 = xsize / 2.0f;
		float z2 = zsize / 2.0f;
		float iFactor = (float)zsize / zdivs;
		float jFactor = (float)xsize / xdivs;
		float x, z;
		int vidx = 0;
		for( int i = 0; i <= zdivs; i++ ) {
			z = iFactor * i - z2;
			for( int j = 0; j <= xdivs; j++ ) {
				x = jFactor * j - x2;
				v[vidx] = x;
				v[vidx+1] = 0.0f;
				v[vidx+2] = z;
				n[vidx] = 0.0f;
				n[vidx+1] = 1.0f;
				n[vidx+2] = 0.0f;
				vidx += 3;
			}
		}

		GLuint rowStart, nextRowStart;
		int idx = 0;
		for( int i = 0; i < zdivs; i++ ) {
			rowStart = i * (xdivs+1);
			nextRowStart = (i+1) * (xdivs+1);
			for( int j = 0; j < xdivs; j++ ) {
				el[idx] = rowStart + j;
				el[idx+1] = nextRowStart + j;
				el[idx+2] = nextRowStart + j + 1;
				el[idx+3] = rowStart + j;
				el[idx+4] = nextRowStart + j + 1;
				el[idx+5] = rowStart + j + 1;
				idx += 6;
			}
		}

		int verts = (xdivs+1) * (zdivs+1);
		int elements = 6 * xdivs * zdivs;
		TriangleMesh * mesh = new TriangleMesh(verts, elements);

		mesh->copyPositionData(v);
		mesh->copyNormalData(n);
		mesh->copyElementData(el);

		delete [] v;
		delete [] n;
		delete [] el;

		return mesh;
	}
}
