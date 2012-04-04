#ifndef __gltw_shader_hpp
#define __gltw_shader_hpp

#include <string>
using std::string;

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
    bool compileAndLinkStockShader( gltw::Shader );
	bool getFileContents( const char * fileName, string &str /*out*/ );
    bool checkCompilationStatus( GLuint );
    bool checkLinkStatus( GLuint );
    void initUniforms();
	/// @publicsection

	/**
	 * Set a uniform vec4 variable
	 *
	 * @param prog the shader program ID
	 * @param name the name of the uniform variable
	 * @param value a pointer to 4 GLfloat values
	 */
	void setUniform4fv( GLuint prog, const char * name, GLfloat * value );
	/**
	 * Set a uniform vec3 variable
	 *
	 * @param prog the shader program ID
	 * @param name the name of the uniform variable
	 * @param value a pointer to 3 GLfloat values
	 */
	void setUniform3fv( GLuint prog, const char * name, GLfloat * value );
	/**
	 * Set a uniform vec4 variable
	 *
	 * @param prog the shader program ID
	 * @param name the name of the uniform variable
	 * @param x the x/r component
	 * @param y the y/g component
	 * @param z the z/b component
	 * @param w the w/a component
	 */
	void setUniform4f( GLuint prog, const char * name , GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	/**
	 * Set a uniform vec3 variable
	 *
	 * @param prog the shader program ID
	 * @param name the name of the uniform variable
	 * @param x the x/r component
	 * @param y the y/g component
	 * @param z the z/b component
	 */
	void setUniform3f( GLuint prog, const char * name, GLfloat x, GLfloat y, GLfloat z);
	/**
	 * Set a uniform mat4 variable
	 *
	 * @param prog the shader program ID
	 * @param name the name of the uniform variable
	 * @param value a pointer to 16 GLfloat values (column-major order)
	 */
	void setUniformMatrix4( GLuint prog, const char * name, GLfloat * value);

	/**
	 * Attempts to link the shader program
	 *
	 * @param id the id of the shader program (all shaders within the shader 
	 *   program must already be compiled and attached)
	 * @return true if the linking is successful.
	 */
	bool linkProgram( GLuint id );

	/**
	 * Compile and link the vertex and fragment shaders contained in the 
	 * strings provided.
	 *
	 * @param vertex the vertex shader code (null terminated)
	 * @param fragment the fragment shader code (null terminated)
	 * @return the ID of the shader program or 0 if the program failed to compile or link.
	 */
	GLuint compileAndLinkShaderPair( const char *vertex, const char *fragment );

	/**
	 * Compile and link the vertex and fragment shaders contained in the 
	 * files with the provided file names.
	 *
	 * @param vertexFileName the name of the file containing the vertex shader code
	 * @param fragmentFileName the name of the file containing the fragment shader code
	 * @return the ID of the shader program or 0 if the program failed to compile or link.
	 */
	GLuint compileAndLinkShaderPairFromFile( const char *vertexFileName, const char *fragmentFileName );

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
	 * Set the color used by the currently active shader.
	 * If no shader is active, this does nothing.
	 *
	 * @param red the red component of the color
	 * @param green the green component of the color
	 * @param blue the blue component of the color.
	 * @param alpha the alpha component of the color.
	 */
    void setColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	/**
	 * Set the position of the light (in eye coordinates) used by the currently active shader.
	 * If no shader is active, or the shader does not support this uniform,
	 * this function does nothing.
	 *
	 * @param pos a pointer an array of 3 GLfloat values organized
	 *    as {x, y, z}
	 */
    void setLightPosition( GLfloat *pos );

	/**
	 * Set the position of the light (in eye coordinates) used by the currently active shader.
	 * If no shader is active, or the shader does not support this uniform,
	 * this function does nothing.
	 *
	 * @param x the x coordinate of the position
	 * @param y the y coordinate of the position
	 * @param z the z coordinate of the position
	 */
    void setLightPosition( GLfloat x, GLfloat y, GLfloat z );
}

#include "gltw_shader.inl"

#endif