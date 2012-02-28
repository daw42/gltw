#ifndef __gltw_hpp
#define __gltw_hpp

// OpenGL includes.  You may need to fix this up to match your OpenGL setup.
#ifdef WIN32
#include <glload/gl_3_3.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

namespace gltw { }

#include "gltw_shader.hpp"
#include "gltw_batch.hpp"

#endif
