#ifndef __gltw_shader_hpp
#define __gltw_shader_hpp

namespace gltw {
	enum Shader {
        SHADER_FLAT = 0, 
        SHADER_PER_VERT_COLOR, 
        SHADER_LIGHT, 
        SHADER_POINT_LIGHT, 
        SHADER_NONE
    };

	class GltwState {
	private:
		GltwState();

	public:
		GLuint shaderIDs[ gltw::SHADER_NONE ];
		Shader activeShader;
		
		const char * source[gltw::SHADER_NONE][2];
		static GltwState& state();
	};
        
    void useStockShader( gltw::Shader );
    void setUniform4fv( GLuint, const char *, GLfloat * );
	void setUniformMatrix4( GLuint, const char *, GLfloat *);
    bool compileAndLoadShaderPair( gltw::Shader );
    bool checkCompilationStatus( GLuint );
    bool checkLinkStatus( GLuint );
    void initUniforms();
    
    void setModelViewMatrix( GLfloat * );
    void setProjectionMatrix( GLfloat * );
    void setColor( GLfloat * );
}

#include "gltw_shader.inl"

#endif