#ifndef DEVICE_H
#define DEVICE_H

#include <SFML/Graphics.hpp>
#include "Render.h"

class Device{
public:
	int Width;
	int Height;

	Device(int width, int height);
	~Device( );
	void Display(sf::RenderWindow &window, const Camera &camera, const std::vector<Mesh> &meshes);
private:
	Renderer *render;
};

#endif


