
namespace gltw {

	inline VertexBatch::VertexBatch( GLenum mode, GLuint numVerts, int attribs, GLenum hint ) :
		drawMode(mode), nVerts(numVerts), bufferUsage(hint), buffersReady(false), attributes(attribs), vaID(0)
	{
		for( int i = 0; i < NUM_BUFFERS; i++) bufIDs[i] = 0;
		if( attributes & ATTRIB_POSITION == 0 ) {
			cerr << "Error in VertexBatch constructor:  VertexBatch must include the position attribute." << endl;
			exit(1);
		}
	}

	inline VertexBatch::~VertexBatch() {
		// TODO delete buffers and clean-up
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
		if( attributes & ATTRIB_NORMAL == 0 ) {
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
		if( attributes & ATTRIB_COLOR == 0 ) {
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
		return  ((attributes & ATTRIB_POSITION != 0) && (bufIDs[POSITION] != 0 ) ) &&
			((attributes & ATTRIB_COLOR != 0) && (bufIDs[COLOR] != 0 ) ) &&
			((attributes & ATTRIB_NORMAL != 0) && (bufIDs[NORMAL] != 0 ) );
	}

	inline void VertexBatch::buildVertexArray() {
		glGenVertexArrays( 1, &vaID );
		glBindVertexArray(vaID);
			
		if( (attributes & ATTRIB_POSITION != 0 ) && bufIDs[POSITION] != 0 ) {
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[POSITION] );
			glVertexAttribPointer( GLTW_ATTRIB_IDX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(GLTW_ATTRIB_IDX_POSITION);
		}

		if( (attributes & ATTRIB_COLOR != 0 ) && bufIDs[COLOR] != 0 ) {
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[COLOR] );
			glVertexAttribPointer( GLTW_ATTRIB_IDX_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(GLTW_ATTRIB_IDX_COLOR);
		}

		if( (attributes & ATTRIB_NORMAL != 0 ) && bufIDs[NORMAL] != 0 ) {
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[NORMAL] );
			glVertexAttribPointer( GLTW_ATTRIB_IDX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(GLTW_ATTRIB_IDX_NORMAL);
		}
	}

	inline void VertexBatch::draw() {
		if( ! isReady() ) {
			cerr << "VertexBatch is not ready to draw.  Missing some vertex data." << endl;
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
	}

	inline Mesh * buildSphere() {}
}