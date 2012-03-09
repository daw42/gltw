
namespace gltw {

	inline VertexBatch::VertexBatch( GLenum mode, GLuint numVerts, int attribs, GLenum hint ) :
		drawMode(mode), nVerts(numVerts), bufferUsage(hint), attributes(attribs), vaID(0)
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

	inline TriangleMesh::~TriangleMesh() { }


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
}