#ifndef __gltw_shader_hpp
#define __gltw_shader_hpp

namespace gltw {
	enum Shader {
        SHADER_IDENTITY = 0, SHADER_FLAT, SHADER_SHADED, SHADER_LIGHT, SHADER_POINT_LIGHT, 
        NUM_SHADERS
    };

	class GltwState {
	private:
		GltwState();

	public:
		GLuint shaderIDs[ gltw::NUM_SHADERS ];
		const char * source[gltw::NUM_SHADERS][2];
		static GltwState& state();
	};
        
    void useStockShader( gltw::Shader , ... );
    void setUniform4fv( GLuint, const char *, GLfloat * );
	void setUniformMatrix4( GLuint, const char *, GLfloat *);
    bool compileAndLoadShaderPair( gltw::Shader );
    bool checkCompilationStatus( GLuint );
    bool checkLinkStatus( GLuint );
}

#include "gltw_shader.inl"

#endif