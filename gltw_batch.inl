
namespace gltw {

	inline VertexBatch::VertexBatch( GLenum mode, GLuint numVerts, GLenum hint ) :
		drawMode(mode), nVerts(numVerts), bufferUsage(hint), buffersReady(false), vaID(0)
	{
		for( int i = 0; i < NUM_BUFFERS; i++) bufIDs[i] = 0;
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
		if( bufIDs[ POSITION ] == 0 ) {
			glGenBuffers(1, &bufIDs[NORMAL] );
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[NORMAL] );
			glBufferData( GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * nVerts, NULL, bufferUsage);			
		}
		glBindBuffer( GL_ARRAY_BUFFER, bufIDs[NORMAL]);
		glBufferSubData( GL_ARRAY_BUFFER, 0, 3 * sizeof(GLfloat)*nVerts, data); 
	}

	inline void VertexBatch::copyColorData( GLfloat * ) {
		if( bufIDs[ COLOR ] == 0 ) {
			glGenBuffers(1, &bufIDs[COLOR] );
			glBindBuffer( GL_ARRAY_BUFFER, bufIDs[COLOR] );
			glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * nVerts, NULL, bufferUsage);			
		}
		glBindBuffer( GL_ARRAY_BUFFER, bufIDs[COLOR]);
		glBufferSubData( GL_ARRAY_BUFFER, 0, 4 * sizeof(GLfloat)*nVerts, data); 
	}

	inline void VertexBatch::draw() {
	
	}

	inline void buildSphere() {}
}