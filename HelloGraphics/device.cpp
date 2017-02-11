#include "device.h"

Device::Device(int width, int height){
	Width = width;
	Height = height;
	render = new Renderer(width, height);
}

Device::~Device( ){
	delete render;
}

void Device::Display(sf::RenderWindow &window, const Camera &camera, const std::vector<Mesh> &meshes){
	Color color{ 0.0f, 0.0f, 0.0f, 1.0f };
	render->Clear(color, RGBA);

	render->Render(camera, meshes);
	sf::Uint8* pixels = (sf::Uint8*)render->Getbuffer();

	sf::Texture texture;
	texture.create(Width, Height);
	sf::Sprite sprite(texture); // needed to draw the texture on screen
	texture.update(pixels);

	window.draw(sprite);
}