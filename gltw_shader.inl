
namespace gltw {
	inline GltwState::GltwState() { 
		for( int i = 0; i < NUM_SHADERS; i++ ) shaderIDs[i] = 0; 
			
        source[SHADER_FLAT][0] = // gltw::SHADER_FLAT vertex shader
			"#version 150 \n"
			"in vec4 vPosition;"
			"uniform mat4 mv;"
			"uniform mat4 proj;
			"void main() {"
			"  gl_Position = proj * mv * vPosition;"
			"}";
		source[SHADER_FLAT][1] = // gltw::SHADER_FLAT fragment shader
			"#version 150 \n"
			"uniform vec4 color;"
			"out vec4 FragColor;"
			"void main() {"
			"   FragColor = color;"
			"}";
		source[SHADER_PER_VERT_COLOR][0] = 
		    "#version 150 \n"
			"in vec4 vPosition;"
			"in vec4 vColor;"
			"out vec4 color;"
			"uniform mat4 mv;"
			"uniform mat4 proj;
			"void main() {"
			"  color = vColor;"
			"  gl_Position = proj * mv * vPosition;"
			"}";
		source[SHADER_PER_VERT_COLOR][1] = 
		    "#version 150 \n"
			"in vec4 color;"
			"out vec4 FragColor;"
			"void main() {"
			"  FragColor = color;"
			"}";
		source[SHADER_LIGHT][0] = NULL;
		source[SHADER_LIGHT][1] = NULL;
		source[SHADER_POINT_LIGHT][0] = NULL;
		source[SHADER_POINT_LIGHT][1] = NULL;
		
		activeShader = SHADER_NONE;
	}

	inline GltwState& GltwState::state() {
		static GltwState *state = new GltwState();
		return *state;
	}

	inline void useStockShader( gltw::Shader shader )
    {
		GLuint &shaderID = GltwState::state().shaderIDs[ shader ];
        
        if( shaderID == 0 && shader != SHADER_NONE )
        {
            if( ! compileAndLoadShaderPair( shader ) )
                return;
        }
        
        if( shader == SHADER_NONE ) {
            glUseProgram(0);
        } else {
            glUseProgram(shaderID);
            initUniforms();
        }
        activeShader = shader;
    }
    
    inline void initUniforms() {
        if( activeShader != SHADER_NONE )
        {
            GLuint id = GltwState::state().shaderIDs[ activeShader ];
            GLfloat identity[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f };
            setUniformMatrix4( id, "mv", identity );
            setUniformMatrix4( id, "proj", identity );
        }
        
        if( activeShader == SHADER_FLAT )
        {
            GLuint id = GltwState::state().shaderIDs[ activeShader ];
            GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
            setUniform4fv( id, "color", white );
        }
    }
    
    inline void setUniform4fv( GLuint progID, const char * name, GLfloat *value ) {
        GLint location = glGetUniformLocation(progID, name);
        if( location != -1 ) { 
            glUniform4fv( location, 1, value );
        } else {
            cerr << "Unable to set uniform \"" << name << "\"" << endl;
        }
    }

	inline void setUniformMatrix4( GLuint progID, const char * name, GLfloat *value ) {
        GLint location = glGetUniformLocation(progID, name);
        if( location != -1 ) { 
            glUniformMatrix4fv( location, 1, GL_FALSE, value );
        } else {
            cerr << "Unable to set uniform \"" << name << "\"" << endl;
        }
    }
    
    inline bool compileAndLoadShaderPair( gltw::Shader shader )
    {
		GLuint &shaderID = GltwState::state().shaderIDs[ shader ];
        if( shaderID == 0 ) 
        {
            // Create the shader objects
            GLuint vert = glCreateShader(GL_VERTEX_SHADER);
            GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
            
            // Load the shader source
            glShaderSource(vert, 1, &GltwState::state().source[shader][0], NULL);
            glShaderSource(frag, 1, &GltwState::state().source[shader][1], NULL);
            
            // Compile
            glCompileShader(vert);
            if( ! checkCompilationStatus(vert) ) {
                glDeleteShader(vert);
                glDeleteShader(frag);
                return false;
            }
            glCompileShader(frag);
            if( ! checkCompilationStatus(frag) ) {
                glDeleteShader(vert);
                glDeleteShader(frag);
                return false;
            }
            
            // Create the program object
            shaderID = glCreateProgram();
            glAttachShader(shaderID, vert);
            glAttachShader(shaderID, frag);
            glLinkProgram( shaderID );
            if( ! checkLinkStatus(shaderID ) ) {
                glDeleteShader(vert);
                glDeleteShader(frag);
                glDeleteProgram(shaderID);
                shaderID = 0;
                return false;
            }
        }
        
        return true;
    }
    
    inline bool checkCompilationStatus( GLuint shaderID )
    {
        GLint status, logLen;
        GLchar *log;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        if( GL_TRUE == status ) return true;
        
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
        log = new GLchar[ logLen ];
        glGetShaderInfoLog(shaderID, logLen, NULL, log);
        
        cerr << "Failed to compile shader" << endl;
        cerr << log << endl;
        
        delete [] log;
        return false;
    }
    
    inline bool checkLinkStatus( GLuint programID )
    {
        GLint status, logLen;
        GLchar *log;
        glGetProgramiv(programID, GL_LINK_STATUS, &status);
        if( GL_TRUE == status ) return true;
        
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
        log = new GLchar[ logLen ];
        glGetProgramInfoLog(programID, logLen, NULL, log);
        
        cerr << "Failed to link shader" << endl;
        cerr << log << endl;
        
        delete [] log;
        return false;
    }
    
    inline void setModelViewMatrix( GLfloat *matrix )
    {        
        GltwState &state = GltwState::state();
        if( activeShader != SHADER_NONE ) {
            setUniformMatrix4( state.shaderIDs[activeShader], "mv", mv );
        }
    }
    
    inline void setColor( GLfloat *color )
    {
        if( activeShader == SHADER_FLAT ) {
            setUniform4fv( GltwState::state().shaderIDs[activeShader], "color", color);
        }
    }
}