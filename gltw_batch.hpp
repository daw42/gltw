#ifndef __gltw_batch_hpp
#define __gltw_batch_hpp

namespace gltw {

	class VertexBatch {
	public:
		enum Buffer { POSITION, NORMAL, COLOR, TEXCOORD, ELEMENT, NUM_BUFFERS };
		enum Attribs { ATTRIB_POSITION = 0x01, ATTRIB_NORMAL = 0x02, ATTRIB_COLOR = 0x04, ATTRIB_TEXCOORD = 0x08 };

		VertexBatch( GLenum primitiveType, GLuint numVerts, int attributes = ATTRIB_POSITION, GLenum usage = GL_DYNAMIC_DRAW );
		~VertexBatch();

		void copyPositionData( GLfloat * );
		void copyNormalData( GLfloat * );
		void copyColorData( GLfloat * );
		void copyTexCoordData( GLfloat * );

		bool isReady();

		virtual void draw();

	protected:
		void buildVertexArray();

		int attributes;
		unsigned int nVerts;
		GLenum bufferUsage, drawMode;
		bool buffersReady;
		GLuint vaID;
		GLuint bufIDs[NUM_BUFFERS];
	};

	class Mesh : public VertexBatch {
	public:

		Mesh( GLenum drawMode, GLuint numVerts, GLuint numElements, int attributes = ATTRIB_POSITION, GLenum usage = GL_STATIC_DRAW );
		~Mesh();

		void copyElementData( GLuint * );

		virtual void draw();

	private:
		GLuint nElements;
	};

	Mesh* buildSphere( );
}

#include "gltw_batch.inl"

#endif