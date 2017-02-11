#include <SFML/Graphics.hpp>
#include "device.h"
#include "Model.h"

const unsigned int W = 800;
const unsigned int H = 600;

std::vector<Vertex> vertices{
	Vertex{ vec3(-1.0f, 1.0f, 1.0f), vec3(), vec3(-1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f) },
	Vertex{ vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f) },
	Vertex{ vec3(-1.0f, -1.0f, 1.0f), vec3(), vec3(-1.0f, -1.0f, 1.0f), vec2(0.0f, 1.0f) },
	Vertex{ vec3(1.0f, -1.0f, 1.0f), vec3(), vec3(1.0f, -1.0f, 1.0f), vec2(1.0f, 1.0f) },
	Vertex{ vec3(-1.0f, 1.0f, -1.0f), vec3(), vec3(-1.0f, 1.0f, -1.0f), vec2(1.0f, 0.0f) },
	Vertex{ vec3(1.0f, 1.0f, -1.0f), vec3(), vec3(1.0f, 1.0f, -1.0f), vec2(0.0f, 1.0f) },
	Vertex{ vec3(1.0f, -1.0f, -1.0f), vec3(), vec3(1.0f, -1.0f, -1.0f), vec2(0.0f, 0.0f) },
	Vertex{ vec3(-1.0f, -1.0f, -1.0f), vec3(), vec3(-1.0f, -1.0f, -1.0f), vec2(1.0f, 1.0f) },
};
std::vector<Face> faces{
	Face{ 0, 2, 1 },
	Face{ 1, 2, 3 },
	Face{ 1, 3, 6 },
	Face{ 1, 6, 5 },
	Face{ 0, 1, 4 },
	Face{ 1, 5, 4 },
	Face{ 2, 7, 3 },
	Face{ 3, 7, 6 },
	Face{ 0, 7, 2 },
	Face{ 0, 4, 7 },
	Face{ 4, 5, 6 },
	Face{ 4, 6, 7 },
};
Camera camera(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f));
Mesh mesh(vertices, faces, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f), "resources/container.jpg");
std::vector<Mesh> monkey = Model::LoadMeshesFromJsonFile("monkey.babylon");
//std::vector<Mesh> bunny = Model::LoadMeshesFromJsonFile("resources/bunny.babylon");

int main()
{
	Device device(W, H);
	// create the window
	sf::RenderWindow window(sf::VideoMode(W, H), "My window");

	sf::Clock clock;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		sf::Time time = clock.restart( );
		float fps = 1000.0f / time.asMilliseconds( );
		std::cout << "fps : " << fps << " / s" << std::endl;

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);
		
		//mesh.EulerAngleXYZ[0] += 0.2;
		//mesh.EulerAngleXYZ[1] += 0.2;
		//device.Display(window, camera, std::vector<Mesh>{mesh});

		for (auto &monkey_mesh : monkey){
			//monkey_mesh.EulerAngleXYZ[0] = 90.0f;
			monkey_mesh.EulerAngleXYZ[1] += 2.0f;
		}
		device.Display(window, camera, monkey);

		// end the current frame
		window.display();
	}

	return 0;
}