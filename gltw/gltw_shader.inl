
namespace gltw {
	inline ShaderState::ShaderState() { 
		for( int i = 0; i < SHADER_NONE; i++ ) shaderIDs[i] = 0; 
			
        source[SHADER_FLAT][0] = 
			"#version 150 \n"
			"in vec4 vPosition;"
			"uniform mat4 mv;"
			"uniform mat4 proj;"
			"void main() {"
			"  gl_Position = proj * mv * vPosition;"
			"}";
		source[SHADER_FLAT][1] = 
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
			"uniform mat4 proj;"
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
		source[SHADER_DEFAULT_LIGHT][0] = 
			"#version 150 \n"
			"in vec3 vPosition;"
			"in vec3 vNormal;"
			"uniform mat4 mv;"
			"uniform mat4 proj;"
			"uniform vec4 color = vec4(0.9,0.9,0.9,1.0);"
			"out vec4 fColor;"
			"void main() {"
			"   mat3 normMatrix = mat3(mv[0].xyz, mv[1].xyz, mv[2].xyz);"
			"   vec3 n = normalize( normMatrix * vNormal );"
			"   vec3 light = vec3(0.0,0.0,1.0);"
			"   fColor = vec4( color.rgb * max(0.0, dot(n,light)), color.a );"
			"   gl_Position = proj * mv * vec4(vPosition,1.0);"
			"}";
		source[SHADER_DEFAULT_LIGHT][1] = 
			"#version 150 \n"
			"in vec4 fColor;"
			"out vec4 FragColor;"
			"void main() {"
			"   FragColor = fColor;"
			"}";
		source[SHADER_POINT_LIGHT][0] = 
			"#version 150 \n"
			"in vec3 vPosition;"
			"in vec3 vNormal;"
			"uniform mat4 mv;"
			"uniform mat4 proj;"
			"uniform vec4 color = vec4(0.9,0.9,0.9,1.0);"
			"uniform vec3 lightPos = vec3(0.0);"
			"out vec4 fColor;"
			"void main() {"
			"   mat3 normMatrix = mat3(mv[0].xyz, mv[1].xyz, mv[2].xyz);"
			"   vec3 n = normalize( normMatrix * vNormal );"
			"   vec4 ecPos = mv * vec4(vPosition,1.0);"
			"   vec3 light = normalize( lightPos - ecPos.xyz );"
			"   fColor = vec4( color.rgb * max(0.0, dot(n,light)), color.a );"
			"   gl_Position = proj * mv * vec4(vPosition,1.0);"
			"}";
		source[SHADER_POINT_LIGHT][1] = 
			"#version 150 \n"
			"in vec4 fColor;"
			"out vec4 FragColor;"
			"void main() {"
			"   FragColor = fColor;"
			"}";
		
		activeShader = SHADER_NONE;
	}

	inline ShaderState& ShaderState::state() {
		static ShaderState *state = new ShaderState();
		return *state;
	}

	inline void useStockShader( gltw::Shader shader )
    {
		GLuint &shaderID = ShaderState::state().shaderIDs[ shader ];
        
        if( shaderID == 0 && shader != SHADER_NONE )
        {
            if( ! compileAndLoadShaderPair( shader ) )
                exit(1);
        }
        
        if( shader == SHADER_NONE ) {
            glUseProgram(0);
        } else {
            glUseProgram(shaderID);
            initUniforms();
        }
		ShaderState::state().activeShader = shader;
    }
    
    inline void initUniforms() {
		gltw::Shader &activeShader = ShaderState::state().activeShader;

        if( activeShader != SHADER_NONE )
        {
            GLuint id = ShaderState::state().shaderIDs[ activeShader ];
            GLfloat identity[] = {1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f };
            setUniformMatrix4( id, "mv", identity );
            setUniformMatrix4( id, "proj", identity );
        }
        
        if( activeShader == SHADER_FLAT )
        {
            GLuint id = ShaderState::state().shaderIDs[ activeShader ];
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

	inline void setUniform4f( GLuint progID, const char * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w ) {
        GLint location = glGetUniformLocation(progID, name);
        if( location != -1 ) { 
            glUniform4f( location, x, y, z, w );
        } else {
            cerr << "Unable to set uniform \"" << name << "\"" << endl;
        }
    }

	inline void setUniform3fv( GLuint progID, const char * name, GLfloat *value ) {
        GLint location = glGetUniformLocation(progID, name);
        if( location != -1 ) { 
            glUniform3fv( location, 1, value );
        } else {
            cerr << "Unable to set uniform \"" << name << "\"" << endl;
        }
    }

	inline void setUniform3f( GLuint progID, const char * name, GLfloat x, GLfloat y, GLfloat z ) {
        GLint location = glGetUniformLocation(progID, name);
        if( location != -1 ) { 
            glUniform3f( location, x, y, z );
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
		GLuint &shaderID = ShaderState::state().shaderIDs[ shader ];
        if( shaderID == 0 ) 
        {
            // Create the shader objects
            GLuint vert = glCreateShader(GL_VERTEX_SHADER);
            GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
            
            // Load the shader source
            glShaderSource(vert, 1, &ShaderState::state().source[shader][0], NULL);
            glShaderSource(frag, 1, &ShaderState::state().source[shader][1], NULL);
            
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

			// Set up attribute locations
			glBindAttribLocation( shaderID, GLTW_ATTRIB_IDX_POSITION, "vPosition" );
			if( shader == SHADER_PER_VERT_COLOR )
			{
				glBindAttribLocation(shaderID, GLTW_ATTRIB_IDX_COLOR, "vColor" );
			}
			if( shader == SHADER_DEFAULT_LIGHT || shader == SHADER_POINT_LIGHT ) {
				glBindAttribLocation(shaderID, GLTW_ATTRIB_IDX_NORMAL, "vNormal" );
			}

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
        ShaderState &state = ShaderState::state();
        if( state.activeShader != SHADER_NONE ) {
            setUniformMatrix4( state.shaderIDs[state.activeShader], "mv", matrix );
        }
    }

	inline void setProjectionMatrix( GLfloat *matrix )
    {        
        ShaderState &state = ShaderState::state();
        if( state.activeShader != SHADER_NONE ) {
            setUniformMatrix4( state.shaderIDs[state.activeShader], "proj", matrix );
        }
    }
    
    inline void setColor( GLfloat *color )
    {
		ShaderState &state = ShaderState::state();
		if( state.activeShader == SHADER_FLAT || state.activeShader == SHADER_DEFAULT_LIGHT ||
			state.activeShader == SHADER_POINT_LIGHT ) {
            setUniform4fv( state.shaderIDs[state.activeShader], "color", color);
        }
    }
	
	inline void setColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		ShaderState &state = ShaderState::state();
		if( state.activeShader == SHADER_FLAT || state.activeShader == SHADER_DEFAULT_LIGHT ||
			state.activeShader == SHADER_POINT_LIGHT ) {
            setUniform4f( state.shaderIDs[state.activeShader], "color", red, green, blue, alpha);
        }
	}

	inline void setLightPosition( GLfloat *pos )
	{
		ShaderState &state = ShaderState::state();
		if( state.activeShader == SHADER_POINT_LIGHT ) {
            setUniform3fv( state.shaderIDs[state.activeShader], "lightPos", pos);
        }
	}

	inline void setLightPosition( GLfloat x, GLfloat y, GLfloat z )
	{
		ShaderState &state = ShaderState::state();
		if( state.activeShader == SHADER_POINT_LIGHT ) {
            setUniform3f( state.shaderIDs[state.activeShader], "lightPos", x, y, z);
        }
	}
}