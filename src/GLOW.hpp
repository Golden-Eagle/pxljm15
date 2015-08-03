/*
 *
 * Skiro GL Main Header
 *
 */

#pragma once

#include <stdexcept>

#include <GLAER/glaer.h>
#include <GLFW/glfw3.h>

#include "Initial3D.hpp"
#include "Log.hpp"

// this is to enable multiple context support, defined in Window.cpp
namespace gecom {
	GlaerContext * getCurrentGlaerContext();
}

namespace gecom {

	// projection matrix creation ? - see the gl docs for gluPerspective

	// exception thrown by GL debug callback on error if GECOM_GL_NO_EXCEPTIONS is not defined
	class gl_error {
	public:
		//gl_error(const std::string &what_ = "GL error") : runtime_error(what_) { }
	};

	inline void draw_dummy(unsigned instances = 1) {
		static GLuint vao = 0;
		if (vao == 0) {
			glGenVertexArrays(1, &vao);
		}
		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_POINTS, 0, 1, instances);
		glBindVertexArray(0);
	}

	inline void checkGL() {
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			log("GL").error() << "GL error: " << err;
			throw std::runtime_error("BOOM!");
		}
	}

	inline void checkFB() {
		GLenum err = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (err != GL_FRAMEBUFFER_COMPLETE) {
			log("GL").error() << "Framebuffer status: " << err;
			log("GL").error() << "YOU BROKE THE FRAMEBUFFER!";
			throw std::runtime_error("OH NOES! THE FRAMEBUFFER IS BROKED");
		}
	}

	inline void checkExtension(const std::string &ext_name) {
		if (glfwExtensionSupported(ext_name.c_str())) {
			log("GL") << "Extension " << ext_name << " detected.";
		}
		else {
			log("GL").error() << "Extension " << ext_name << " not supported.";
			throw std::runtime_error("unsupported extension");
		}
	}

}