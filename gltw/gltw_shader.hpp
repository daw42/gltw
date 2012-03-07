#ifndef __gltw_shader_hpp
#define __gltw_shader_hpp

namespace gltw {
	/** The supported shaders */
	enum Shader {
		/** The Flat Shader.  This gives all polygons a flat color, specified by
		  * the color defined by ::setColor.  It supports the ::ATTRIB_POSITION attribute 
		  * only.
		  */
        SHADER_FLAT = 0, 
		/** The per-vert color shader.  This allows a color to be associated with
		 * each vertex.  The polygon is shaded using color interpolation.  This shader
		 * supports the ::ATTRIB_POSITION and ::ATTRIB_COLOR attributes.
		 */
        SHADER_PER_VERT_COLOR, 
        
		/** The default light shader.  This shader will shade polygons using a diffuse
		 * shading equation, and assuming a light source located at the camera position.
		 * It supports the ::ATTRIB_POSITION and ::ATTRIB_NORMAL attributes. */
		SHADER_DEFAULT_LIGHT, 

		/** The point light shader.  This shader shades polygons using a diffuse shading
		 * equation, and assuming a point light that is positioned in eye coordinates via
		 * the ::setLightPosition function.  It supports the ::ATTRIB_POSITION and 
		 * ::ATTRIB_NORMAL attributes.
		 */
        SHADER_POINT_LIGHT,

		/** This represents a situation where there is no active shader.  Its value is equal
		 * to the number of supported shaders. */
        SHADER_NONE
    };

	/**
	 * The attributes that may be supported by the shaders.  See the documentation for
	 * the Shader enum for information about what attributes are supported by which shaders.
	 */
	enum Attribute { 
		/** The vertex position attribute */
		ATTRIB_POSITION = 0x01, 
		/** The vertex normal attribute */
		ATTRIB_NORMAL = 0x02, 
		/** The vertex color attribute */
		ATTRIB_COLOR = 0x04, 
		/** The vertex texture coordinate attribute */
		ATTRIB_TEXCOORD = 0x08 
	};

	/** The internal index of each attribute */
	enum AttributeIndex {
		/** The index of the position attribute */
		GLTW_ATTRIB_IDX_POSITION = 0,
		/** The index of the color attribute */
		GLTW_ATTRIB_IDX_COLOR,
		/** The index of the normal attribute */
		GLTW_ATTRIB_IDX_NORMAL
	};

	/** @internal */
	class ShaderState {
	private:
		ShaderState();

	public:
		/** The IDs of the shader programs */
		GLuint shaderIDs[ gltw::SHADER_NONE ];
		/** The currently active shader */
		Shader activeShader;
		/** Source code for the shaders */
		const char * source[gltw::SHADER_NONE][2];
		/** Retrieves the singleton object. */
		static ShaderState& state();
	};

    /// @privatesection
	void setUniform4fv( GLuint, const char *, GLfloat * );
	void setUniform3fv( GLuint progID, const char * name, GLfloat *value );
	void setUniformMatrix4( GLuint, const char *, GLfloat *);
    bool compileAndLoadShaderPair( gltw::Shader );
    bool checkCompilationStatus( GLuint );
    bool checkLinkStatus( GLuint );
    void initUniforms();
	/// @publicsection

	/**
	 * Compile and link (if necessary) the given shader and make it
	 * the "active" shader.  If any compiler
	 * errors occur, they will be displayed to standard error.
	 * 
	 * @param shader the shader to compile/load
	 */
    void useStockShader( gltw::Shader shader);
    
	/**
	 * Set the model-view matrix for the currently active shader.
	 * If no shader is active, this does nothing.
	 *
	 * @param m a pointer to the matrix, an array of 16 GLfloat values organized
	 *    in column-major order.
	 */
    void setModelViewMatrix( GLfloat * m );

	/**
	 * Set the projection matrix for the currently active shader.
	 * If no shader is active, this does nothing.
	 *
	 * @param m a pointer to the matrix, an array of 16 GLfloat values organized
	 *   in column-major order.
	 */
    void setProjectionMatrix( GLfloat * m );

	/**
	 * Set the color used by the currently active shader.
	 * If no shader is active, this does nothing.
	 *
	 * @param c a pointer an array of 4 GLfloat values organized
	 *    as {r, g, b, a}
	 */
    void setColor( GLfloat * c);

	/**
	 * Set the position of the light (in eye coordinates) used by the currently active shader.
	 * If no shader is active, or the shader does not support this uniform,
	 * this function does nothing.
	 *
	 * @param pos a pointer an array of 3 GLfloat values organized
	 *    as {x, y, z}
	 */
    void setLightPosition( GLfloat *pos );
}

#include "gltw_shader.inl"

#endif