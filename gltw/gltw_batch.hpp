#ifndef __gltw_batch_hpp
#define __gltw_batch_hpp

namespace gltw {

	// Inheriting from this should disallow copying via copy constructor or
	// assignment operator.
	class NonCopyable {
	private:
		NonCopyable(const NonCopyable& other );
		NonCopyable & operator = (const NonCopyable & other );
	public:
		NonCopyable() { }
	};

	class VertexBatch : public NonCopyable {
	public:
		enum Buffer { POSITION, NORMAL, COLOR, TEXCOORD, ELEMENT, NUM_BUFFERS };
		enum Attribute { ATTRIB_POSITION = 0x01, ATTRIB_NORMAL = 0x02, ATTRIB_COLOR = 0x04, ATTRIB_TEXCOORD = 0x08 };

		VertexBatch( GLenum primitiveType, GLuint numVerts, int attributes = ATTRIB_POSITION, GLenum usage = GL_DYNAMIC_DRAW );
		~VertexBatch();

		bool isReady();

		void copyPositionData( GLfloat * );
		void copyNormalData( GLfloat * );
		void copyColorData( GLfloat * );
		void copyTexCoordData( GLfloat * );

		virtual void draw();

	protected:
		void buildVertexArray();
		bool attribEnabled( Attribute attrib );

		int attributes;
		unsigned int nVerts;
		GLenum bufferUsage, drawMode;
		GLuint vaID;
		GLuint bufIDs[NUM_BUFFERS];
	};

	class TriangleMesh : public VertexBatch {
	public:

		TriangleMesh( GLuint numVerts, GLuint numElements, int attributes = ATTRIB_POSITION | ATTRIB_NORMAL, GLenum usage = GL_STATIC_DRAW );
		~TriangleMesh();

		void copyElementData( GLuint * );

		virtual void draw();

	private:
		GLuint nElements;
	};

	TriangleMesh* buildSphere( );
}

#include "gltw_batch.inl"

#endif