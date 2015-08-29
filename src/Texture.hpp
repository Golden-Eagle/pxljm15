#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <gecom/GL.hpp>
#include <gecom/Initial3D.hpp>
#include <stb_image.h>


namespace pxljm {

	class Texture;
	using texture_ptr = std::shared_ptr<Texture>;

	class Texture {
	private:
		GLuint m_textureID = 0;
		std::vector<unsigned char> m_data;
		int m_w; // Width
		int m_h; // Height
		int m_n; // Number of channels


	public:
		Texture(unsigned char *, int, int, int); //stb constructor
		~Texture();

		static texture_ptr fromFile(const std::string &);

		void bind(GLuint);

		int width();
		int height();
		int channels();
	};

}