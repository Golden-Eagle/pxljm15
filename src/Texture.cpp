#include "Texture.hpp"


//stbi_image_free(data)

using namespace std;
using namespace pxljm;
using namespace i3d;


Texture::Texture(unsigned char *data, int w, int h, int n)
	: m_data(data, data+(w*h*n)), m_w(w), m_h(h), m_n(n) {

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	auto glFormat = [](int channels) {
		switch (channels) {
		case 1: return GL_R;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: break;
		}
	};

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, glFormat(m_n), m_w, m_h, 0, glFormat(m_n), GL_UNSIGNED_BYTE, &m_data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}


Texture::~Texture() { }


texture_ptr Texture::fromFile(const std::string & filename) {
	int x, y, n;
	unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 0);
	texture_ptr tex = make_shared<Texture>(data, x, y, n);
	stbi_image_free(data);
	return tex;
}

void Texture::bind(GLuint textureLocation) {
	glActiveTexture(textureLocation);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

int Texture::width() { return m_w; }
int Texture::height() { return m_h; }
int Texture::channels() { return m_n; }