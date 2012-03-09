
namespace gltw {

	inline void checkForOpenGLError(const char * file, int line) {
		GLenum glErr;
		glErr = glGetError();
		while (glErr != GL_NO_ERROR)
		{
			cerr << "glError in file " << file << " @ line " << line <<": ";

			switch( glErr )
			{
			case GL_INVALID_ENUM:
				cerr << "Invalid enum";
				break;
			case GL_INVALID_VALUE:
				cerr << "Invalid value";
				break;
			case GL_INVALID_OPERATION:
				cerr << "Invalid operation";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				cerr << "Invalid framebuffer operation";
				break;
			case GL_OUT_OF_MEMORY:
				cerr << "Out of memory";
				break;
			default:
				cerr << "Unknown error";
			}
			cerr << endl;
			glErr = glGetError();
		}
	}

}