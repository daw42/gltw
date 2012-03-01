#ifndef __gltw_shader_hpp
#define __gltw_shader_hpp

namespace gltw {
	enum Shader {
        SHADER_FLAT = 0, 
        SHADER_PER_VERT_COLOR, 
        SHADER_DEFAULT_LIGHT, 
        SHADER_POINT_LIGHT, 
        SHADER_NONE
    };

	enum Attributes {
		GLTW_ATTRIB_POSITION = 0,
		GLTW_ATTRIB_COLOR,
		GLTW_ATTRIB_NORMAL
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
        
    void useStockShader( gltw::Shader );
    void setUniform4fv( GLuint, const char *, GLfloat * );
	void setUniformMatrix4( GLuint, const char *, GLfloat *);
    bool compileAndLoadShaderPair( gltw::Shader );
    bool checkCompilationStatus( GLuint );
    bool checkLinkStatus( GLuint );
    void initUniforms();
    
	/**
	 * This is docs for setModelViewMatrix.
	 */
    void setModelViewMatrix( GLfloat * );
    void setProjectionMatrix( GLfloat * );
    void setColor( GLfloat * );
}

#include "gltw_shader.inl"

#endif