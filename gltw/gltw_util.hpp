#ifndef __gltw_util_hpp
#define __gltw_util_hpp

namespace gltw {

	/** 
	 * Check for the presence of an OpenGL error by calling glGetError.
	 * If any errors are present, it will display the error to standard error.
	 * 
	 * You should call this function using the pre-processor definitions <code>__FILE__</code>
	 * and <code>__LINE__</code>.  For example:  
	 *
	 * <code>gltw::checkForOpenGLError(__FILE__, __LINE__);</code>
	 *
	 * @param fileName the name of the file from which this method was called.
	 * @param line the line number from which this method was called.
	 */
	void checkForOpenGLError(const char * fileName, int line);

}

#include "gltw_util.inl"

#endif