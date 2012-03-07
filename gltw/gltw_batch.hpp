#ifndef __gltw_batch_hpp
#define __gltw_batch_hpp

namespace gltw {

	/** Inheriting from this should disallow copying via copy constructor or
	 assignment operator. */
	class NonCopyable {
	private:
		NonCopyable(const NonCopyable& other );
		NonCopyable & operator = (const NonCopyable & other );
	public:
		NonCopyable() { }
	};

	/**
	 * The VertexBatch is a class that manages a set of buffer object containing
	 * vertex data.  A VertexBatch contains one buffer for each attribute.  The
	 * available attributes must be selected via the constructor.
	 */
	class VertexBatch : public NonCopyable {
	public:
		/**
		 * Constructs a new VertexBatch object.  This constructor does not call any 
		 * OpenGL functions.
		 *
		 * @param primitiveType the OpenGL primitive type (GL_TRIANGLES, GL_LINES, GL_TRIANGLE_FAN, etc.)
		 * @param numVerts the number of verticies in the batch.  Once constructed, this is fixed and
		 *         cannot be changed.
		 * @param attributes the attributes that will be part of this batch, this can be any of the
		 *        entries in the ::Attribute enum.  Multiple attributes can be specified using a binary
		 *        OR operator (|).
		 * @param usage the buffer usage specifer to be used, defaults to GL_DYNAMIC_DRAW.
		 */
		VertexBatch( GLenum primitiveType, GLuint numVerts, int attributes = ATTRIB_POSITION, GLenum usage = GL_DYNAMIC_DRAW );

		/** Deletes the OpenGL buffer objects for this VertexBatch */
		~VertexBatch();

		/** Returns whether or not this VertexBatch object
		 * is ready to be drawn.  If this returns false, a call to the
		 * draw() function will fail to draw anything. 
		 * 
		 * @return whether or not this object is ready for drawing
		 */
		bool isReady();

		/**
		 * Copy the array of position data to the buffer contained within this VertexBatch,
		 * creating the buffer if needed.  The position data is assumed to be 3 coordinates
		 * per vertex (x,y,z).
		 *
		 * @param data a pointer to 3 * nVerts values, where nVerts is the number of vertices
		 */
		void copyPositionData( GLfloat * data );
		/**
		 * Copy the array of normal data to the buffer contained within this VertexBatch,
		 * creating the buffer if needed.  The normal data is assumed to be 3 coordinates
		 * per vertex (x,y,z).
		 *
		 * @param data a pointer to 3 * nVerts values, where nVerts is the number of vertices 
		 */
		void copyNormalData( GLfloat * data );
		/**
		 * Copy the array of color data to the buffer contained within this VertexBatch,
		 * creating the buffer if needed.  The color data is assumed to be 4 values
		 * per vertex (r,g,b,a).
		 *
		 * @param data a pointer to 4 * nVerts values, where nVerts is the number of vertices 
		 */
		void copyColorData( GLfloat * data );
		/**
		 * Not implemented yet.
		 */
		void copyTexCoordData( GLfloat * data );

		/** Draw this VertexBatch.  This will do nothing and print an error message if
		 * the buffers are not ready.  Make sure to fill the buffers via one of the
		 * copy*Data methods prior to drawing.
		 */
		virtual void draw();

	protected:
		enum Buffer { POSITION, NORMAL, COLOR, TEXCOORD, ELEMENT, NUM_BUFFERS };
		void buildVertexArray();
		bool attribEnabled( Attribute attrib );

		int attributes;
		/** The number of vertices in this VertexBatch */
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

	/// @defgroup 3Dshapes Functions for building 3D shapes
	/// @{
	/** Not implemented yet */
	TriangleMesh* buildSphere( );
	/// @}
}

#include "gltw_batch.inl"

#endif