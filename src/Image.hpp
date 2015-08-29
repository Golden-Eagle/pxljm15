#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <exception>

#include <gecom/Uncopyable.hpp>

#include "stb_image.h"

//TODO put mesh stuff in here?
namespace pxljm {

	class Image;
	using image_ptr = std::shared_ptr<Image>;

	class Image : gecom::Uncopyable {
		int x, y, n;
		unsigned char *m_data;
	public:
		Image(const std::string &file) {
			load(file);
		}
		~Image() {
			unload();
		}

		int channel_count() {
			return n;
		}

		unsigned glType() {
			if(n == 4)
				return GL_RGBA;

			// #rashassumptions
			return GL_RGB;
		}

	private:
		void load(const std::string& filename) {
			m_data = stbi_load(filename.c_str(), &x, &y, &n, 0);

			if(m_data == NULL) {
				std::cerr << "Error reading " << filename << std::endl;
				throw std::runtime_error("Error :: could not open file.");
			}
		}

		void unload() {
			stbi_image_free(m_data);
		}
	};
}