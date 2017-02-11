#include "Texture.h"
#include <iostream>

void Texture::LoadImageFromFile(std::string filename){
	sf::Image image;
	if (!image.loadFromFile(filename)){
		std::cout << "load image error" << std::endl;
		return;
	}

	Width = image.getSize( ).x;
	Height = image.getSize( ).y;
	BufferSize = 4 * Width * Height;
	Buffer = std::vector<byte>(BufferSize);
	auto bufferPtr = image.getPixelsPtr( );
	for (int i = 0; i < BufferSize; i++){
		Buffer[i] = *(bufferPtr + i);
	}
}

Texture::Texture( ){
	isEnable = false;
}

Texture::Texture(std::string filename){
	LoadImageFromFile(filename);
	isEnable = true;
}

Texture::~Texture( ){
	
}

Color Texture::Map(float u, float v) const{
	if (Buffer.size( ) == 0){
		std::cout << "no image data" << std::endl;
		return Color{0.0f, 0.0f, 0.0f, 1.0f};
	}

	int U = std::abs(((int)std::round(u * Width)) % Width);
	int V = std::abs(((int)std::round(v * Height)) % Height);
	int pos = 4 * (V * Width + U);

	byte r = Buffer[pos];
	byte g = Buffer[pos + 1];
	byte b = Buffer[pos + 2];
	byte a = Buffer[pos + 3];

	return Color{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
}

bool Texture::IsEnable( ) const{
	return isEnable;
}