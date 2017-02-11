#ifndef RENDER_H
#define RENDER_H

#include "vector.h"
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"
#include <limits>
#include <ppl.h>

enum ColorCompent{
	RGBA,
	BGRA
};

struct Fragment{
	float z;
	vec2 pixelCoordinate;
	Color inColor;
	Color outColor;
	vec3 normal;
	vec3 worldPos;
	vec3 lightPos;
	vec2 textureCoordinate;
	const Texture *texture;
};

class Renderer{
public:
	int PixelWidth;
	int PixelHeight;
	int BufferSize;

	//Renderer( );
	Renderer(int pixelWidth, int pixelHeight);
	~Renderer();
	void Render(const Camera &camera, const std::vector<Mesh> &meshes);
	byte *Getbuffer( ) const;
	void Renderer::Clear(Color color, ColorCompent colorComp);

private:
	byte *backBuffer;
	float *depthBuffer;

	void Renderer::SetPixel(int x, int y, Color color, ColorCompent colorComp);
	void Renderer::SetPixel(int x, int y, float z, Color color, ColorCompent colorComp);
	void DrawPoint(vec2 point, Color color, ColorCompent colorComp);
	void DrawPoint(vec2 point, float z, Color color, ColorCompent colorComp);
	void DrawLine_Bresenham(vec3 point1, vec3 point2, Color color, ColorCompent colorComp);
	void ScanLineXRasterization(int y, Vertex v1, Vertex v2, Vertex v3, Vertex v4, const Texture &texture, ColorCompent colorComp);
	void TriangleRasterization(Vertex v1, Vertex v2, Vertex v3, const Texture &texture, ColorCompent colorComp);
	Vertex VertexShader(mat4 transform, mat4 worldM, Vertex coord);
	void FragmentShader(Fragment &frag);
	bool FloatEqual(float a, float b);

	float Clamb(float value, float min = 0.0f, float max = 1.0f);
	float Interpolate(float x1, float y1, float x2, float y2, float value);
	Color Interpolate(float x1, Color color1, float x2, Color color2, float value);
	vec2 Interpolate(float x1, vec2 uv1, float x2, vec2 uv2, float value);
	vec4 DivideByW(vec4 vec);
	float ComputeNDotL(vec3 vertex, vec3 normal, vec3 light);
};

#endif