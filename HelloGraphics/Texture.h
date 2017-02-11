#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "typedef.h"
#include <SFML/Graphics/Image.hpp>

class Texture{
public:
	Texture( );
	Texture(std::string filename);
	~Texture( );
	Color Map(float u, float v) const;
	bool IsEnable( ) const;
private:
	bool isEnable;
	int Width;
	int Height;
	int BufferSize;
	std::vector<byte> Buffer;
	void LoadImageFromFile(std::string filename);
};

#endif