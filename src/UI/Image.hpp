#ifndef UI_IMAGE_HPP
#define UI_IMAGE_HPP

#include "../Util/Defs.hpp"
#include "../Util/Util.hpp"
#include <GL/gl.h>

class Image
{
	const unsigned char* m_data;
	std::size_t m_size;

	// GL data
	GLuint m_texture;
	Vec2i m_dim;

public:
	Image(const unsigned char* image, std::size_t size)
	{
		m_data = image;
		m_size = size;
	}
	~Image()
	{ }

	Vec2i GetSize() const
	{
		return m_dim;
	}

	const GLuint GetTexture() const
	{
		return m_texture;
	}

	void Load();
};

#endif // UI_IMAGE_HPP
