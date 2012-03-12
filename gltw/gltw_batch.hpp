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
	 * <p>The VertexBatch is a class that manages a set of buffer object containing
	 * vertex data.  A VertexBatch contains one buffer for each attribute.  The
	 * available attributes must be selected via the constructor.  Before drawing
	 * the VertexBatch, one must copy the vertex data into the buffers by using one
	 * or more of the copy*Data functions.</p>
	 *
	 * <p>An example of the use of this class for drawing a triangle follows.</p>
	 *
	 * <p><code>
	 *    // Initialization (need only be done once)<br />
	 *    GLfloat verts[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };<br />
	 *    VertexBatch myBatch(GL_TRIANGLES, 3);<br />
	 *    myBatch.copyVertexData(verts);<br />
	 *    <br />
	 *    // Within the draw function<br />
	 *    myBatch.draw();<br />
	 *    </code></p>
	 *  
	 *  <p> Data can be copied into the VertexBatch at any time, but must be done at
	 *   least once before drawing.</p>
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
		 *        entries in the ::Attribute enum.  Multiple attributes can be specified using a bitwise
		 *        OR operator (|).
		 * @param usage the buffer usage specifer to be used, defaults to GL_DYNAMIC_DRAW.
		 */
		VertexBatch( GLenum primitiveType, GLuint numVerts, int attributes = ATTRIB_POSITION, GLenum usage = GL_DYNAMIC_DRAW );

		/** Deletes the OpenGL buffer objects for this VertexBatch */
		virtual ~VertexBatch();

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
		virtual void buildVertexArray();
		bool attribEnabled( Attribute attrib );

		int attributes;
		/** The number of vertices in this VertexBatch */
		unsigned int nVerts;
		GLenum bufferUsage, drawMode;
		GLuint vaID;
		GLuint bufIDs[NUM_BUFFERS];
	};

	/**
	 * Implements a mesh of triangles using vertex buffers.  This class extends
	 * VertexBatch by including element arrays, and allowing only GL_TRIANGLES.
	 * This is intended to be used for meshes that do not change over time.
	 */
	class TriangleMesh : public VertexBatch {
	public:
		/**
		 * Construct a TriangleMesh object.   This constructor makes no OpenGL calls.
		 *
		 * @param numVerts the number of verticies in the mesh.  Once constructed, this is fixed and
		 *         cannot be changed.
		 * @param numElements the number of element indexes in the mesh.  Once constructed, this is fixed
		 *          and cannot be changed.
		 * @param attributes the attributes that will be part of this batch, this can be any of the
		 *        entries in the ::Attribute enum.  Multiple attributes can be specified using a bitwise
		 *        OR operator (|).
		 * @param usage the buffer usage specifer to be used, defaults to GL_STATIC_DRAW.
		 */
		TriangleMesh( GLuint numVerts, GLuint numElements, int attributes = ATTRIB_POSITION | ATTRIB_NORMAL, GLenum usage = GL_STATIC_DRAW );

		/**
		 * Copy the array of element index data to the buffer contained within this TriangleMesh,
		 * creating the buffer if needed.  The element index data is assumed to be one GLuint
		 * per index.
		 *
		 * @param data a pointer to nElements values, where nElements is the number of elements
		 *        provided at the time this object was constructed.
		 */
		void copyElementData( GLuint * data );

		/** Draw this TriangleMesh.  This will do nothing and print an error message if
		 * the buffers are not ready.  Make sure to fill the buffers via one of the
		 * copy*Data methods prior to drawing.
		 */
		virtual void draw();

	private:
		virtual void buildVertexArray();

		GLuint nElements;
	};

	/// @defgroup 3Dshapes Functions for building 3D shapes
	/// @{
	/** 
	 * Create a TriangleMesh that describes a torus shape.  The torus is defined centered
	 * at the origin in the x-y plane.  It is the caller's responsibility to delete the TriangleMesh
	 * object when finished. 
	 *
	 * @param outerRadius the radius from the origin to the center of the "ring"
	 * @param innerRadius the internal radius of the "ring" of the donut
	 * @param nSides the number of sides per ring 
	 * @param nRings the number of rings around the donut
	 */
	TriangleMesh* buildTorus( GLfloat outerRadius, GLfloat innerRadius, GLint nSides, GLint nRings );
	
	/**
	 * Create a TriangleMesh that describes a cube.  The cube is centered at the origin,
	 * with a side length of 1.  It is the caller's responsibility to delete the TriangleMesh
	 * object when finished.
	 */
	TriangleMesh* buildCube();

	/**
	 * Create a TriangleMesh that describes a cylinder.  The cylinder is aligned along the z
	 * axis and may have a different radius at each end.  If one of the radii is zero, the 
	 * result is a cone.  
	 *
	 * @param base the radius of the base of the cylinder (at z = 0)
     * @param top the radius of the top of the cylinder (at z = height)
     * @param height the height of the cylinder (extent in the z direction)
     * @param slices the number of subdivisions along the z axis.  This must be
     *               greater than or equal to one.
     * @param stacks the number of subdivisions around the z axis.  This must be
     *                 greater than or equal to three.
	 */
	TriangleMesh * buildCylinder( float base, float top, float height, int slices, int stacks );

	/**
      * Create a TriangleMesh describing a Sphere.  The
      * sphere is oriented along the z axis.  The poles are at z = radius
      * and z = -radius.
      *
      * @param radius the radius of the sphere
      * @param slices the number of subdivisions along the z axis (like lines of
      *               longitude).  This must be greater than or equal to three.
      * @param stacks the number of subdivisions around the z axis (like lines of
      *               latitude).  This must be greater than or equal to three.
      */
	TriangleMesh * buildSphere(GLfloat radius, int slices, int stacks);
	/// @}
}

#include "gltw_batch.inl"

#endif