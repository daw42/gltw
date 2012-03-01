#ifndef __gltw_batch_hpp
#define __gltw_batch_hpp

namespace gltw {

	class VertexBatch {
	public:
		enum Buffer { POSITION, NORMAL, COLOR, TEXCOORD, NUM_BUFFERS };

		VertexBatch( GLenum primitiveType, GLuint numVerts, GLenum usage = GL_DYNAMIC_DRAW );
		~VertexBatch();

		void copyPositionData( GLfloat * );
		void copyNormalData( GLfloat * );
		void copyColorData( GLfloat * );

		void draw();

	private:
		unsigned int nVerts;
		GLenum bufferUsage, drawMode;
		bool buffersReady;
		GLuint vaID;
		GLuint bufIDs[NUM_BUFFERS];
	};

	void buildSphere( );
}

#include "gltw_batch.inl"

#endif