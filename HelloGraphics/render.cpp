#include "Render.h"
#define EnablePhongShading 0
const vec3 LightPos(0.0f, 10.0f, 10.0f);

Renderer::Renderer(int width, int height){
	PixelWidth = width;
	PixelHeight = height;
	BufferSize = 4 * PixelWidth * PixelHeight;
	backBuffer = new byte[BufferSize]();
	depthBuffer = new float[PixelWidth * PixelHeight];
}

Renderer::~Renderer(){
	delete[ ] backBuffer;
	delete[ ] depthBuffer;
}

void Renderer::SetPixel(int x, int y, Color color, ColorCompent colorComp){
	int index = y * PixelWidth + x;

	switch (colorComp)
	{
	case RGBA: {
				   backBuffer[index] = (byte)(color.R * 255);
				   backBuffer[index + 1] = (byte)(color.G * 255);
				   backBuffer[index + 2] = (byte)(color.B * 255);
				   backBuffer[index + 3] = (byte)(color.A * 255);
				   break;
	}
	case BGRA: {
				   backBuffer[index] = (byte)(color.B * 255);
				   backBuffer[index + 1] = (byte)(color.G * 255);
				   backBuffer[index + 2] = (byte)(color.R * 255);
				   backBuffer[index + 3] = (byte)(color.A * 255);
				   break;
	}
	default:
		break;
	}
}

void Renderer::SetPixel(int x, int y, float z, Color color, ColorCompent colorComp){
	int indexZ = y * PixelWidth + x;
	int index = 4 * indexZ;

	if (z >depthBuffer[indexZ]){
		return;
	}
	depthBuffer[indexZ] = z;

	switch (colorComp)
	{
	case RGBA: {
				   backBuffer[index] = (byte)(color.R * 255);
				   backBuffer[index + 1] = (byte)(color.G * 255);
				   backBuffer[index + 2] = (byte)(color.B * 255);
				   backBuffer[index + 3] = (byte)(color.A * 255);
				   break;
	}
	case BGRA: {
				   backBuffer[index] = (byte)(color.B * 255);
				   backBuffer[index + 1] = (byte)(color.G * 255);
				   backBuffer[index + 2] = (byte)(color.R * 255);
				   backBuffer[index + 3] = (byte)(color.A * 255);
				   break;
	}
	default:
		break;
	}
}

Vertex Renderer::VertexShader(mat4 transform, mat4 worldM, Vertex coord){
	Vertex vertex;
	vec4 worldPos = worldM * makevec4_from_vec3(coord.Position);
	auto vectorClip = transform * makevec4_from_vec3(coord.Position);
	auto vector = DivideByW(vectorClip);
	vec3 vec(vector[0], vector[1], 1.0f);
	auto mat = Transform::GetViewPortMatrix(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, PixelWidth, PixelHeight, 0.0f);
	vec = mat * vec;

	mat4 normalMTemp = gmtl::makeTranspose(gmtl::makeInvert(worldM));
	mat3 normalM;
	normalM.set(normalMTemp[0][0], normalMTemp[0][1], normalMTemp[0][2],
		                 normalMTemp[1][0], normalMTemp[1][1], normalMTemp[1][2], 
		                 normalMTemp[2][0], normalMTemp[2][1], normalMTemp[2][2]);
	auto normal = normalM * coord.Normal;

	Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
	
	vertex.Position = vec3(vec[0], vec[1], vector[2]);
	vertex.worldPosition = vec3(worldPos[0], worldPos[1], worldPos[2]);
	vertex.Normal = normal;
	vertex.Texture2DCoord = coord.Texture2DCoord;

	if (EnablePhongShading){
		vertex.color = color;
		return vertex;
	}else{
		auto scale = ComputeNDotL(vertex.worldPosition, vertex.Normal, LightPos);
		vertex.color = color * scale;
		return vertex;
	}
}

void Renderer::DrawPoint(vec2 point, Color color, ColorCompent colorComp){
	if (point[0] >= 0 && point[0] < PixelWidth && point[1] >= 0 && point[1] < PixelHeight){
		SetPixel((int)point[0], (int)point[1], color, colorComp);
	}
}

void Renderer::DrawPoint(vec2 point, float z, Color color, ColorCompent colorComp){
	if (point[0] >= 0 && point[0] < PixelWidth && point[1] >= 0 && point[1] < PixelHeight){
		SetPixel((int)point[0], (int)point[1], z, color, colorComp);
	}
}

/*********************************Bresenham Line Algorithm*************************************
given two point (x0, y0), (x1, y1) considering slope 0 < m <= 1 in a window coordinate, draw a line
Line : f(x, y) = (y0 - y1)x + (x1 - x0)y + x0y1 -x1y0           f(x, y) = Ax + By + C
essentially, by computing f(mid_point) continuously, and it has an incremental version
f(x + 1, y) = f(x, y) + A
f(x + 1, y + 1) = f(x, y) + A + B

pseudocode:
y = y0
d = f(x0 + 1, y0 + 0.5)
for x = x0 to x1 do
    draw(x, y)
    if d < 0 then
        y = y + 1
        d = d + (x1 - x0 ) + (y0 - y1)
    else
        d = d + (y0 - y1 )
***********************************my own generic implemention****************************************************************/
void Renderer::DrawLine_Bresenham(vec3 point1, vec3 point2, Color color, ColorCompent colorComp){
	int x0 = (int)point1[0];
	int y0 = (int)point1[1];
	int x1 = (int)point2[0];
	int y1 = (int)point2[1];
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int A = y0 - y1;
	int B = x1 - x0;
	int x = x0;
	int y = y0;
	int d = 2 * A * sx + B * sy; //initially, d = f(x0 + 1, y0 + 0.5) = f(x0 + 1, y0 + 0.5) - f(x0, y0) = A + 0.5 * B , and multiplied by 2
	bool swap = (std::abs(B) < std::abs(A)) ? true : false;

	if (!swap){
		while (true){
			DrawPoint(vec2(x, y), color, colorComp);
			if (x == x1){
				break;
			}

			x += sx;
			if (sx * sy *d < 0){
				y += sy;
				d = d + 2 * (A * sx + B * sy);
			}
			else{
				d += 2 * A * sx;
			}
		}
	}
	else{
		while (true){
			DrawPoint(vec2(x, y), color, colorComp);
			if (y == y1){
				break;
			}

			y += sy;
			if (sx * sy * d < 0){
				d += 2 * B * sy;
			}
			else{
				x += sx;
				d = d + 2 * (A * sx + B * sy);
			}
		}
	}
}

//if point is left about line, return true else false
//bool Renderer::PointInLineLeft(vec3 point, vec3 linePoint1, vec3 linePoint2){
//	float x0 = linePoint1[0];
//	float y0 = linePoint1[1];
//	float x1 = linePoint2[0];
//	float y1 = linePoint2[1];
//	                                         
//	bool value = ((y0 - y1) * point[0] + (x1 - x0) * point[1] + x0 * y1 - x1 * y0) > 0.0f;
//	if (x1 < x0){
//		value = !value;
//	}
//	return value;
//}

void Renderer::ScanLineXRasterization(int y, Vertex v1, Vertex v2, Vertex v3, Vertex v4, const Texture &texture, ColorCompent colorComp){
	auto point1 = v1.Position;
	auto point2 = v2.Position;
	auto point3 = v3.Position;
	auto point4 = v4.Position;

	int scanx1 = (int)Interpolate(point1[1], point1[0], point2[1], point2[0], y);
	int scanx2 = (int)Interpolate(point3[1], point3[0], point4[1], point4[0], y);
	Color color1 = Interpolate(point1[1], v1.color, point2[1], v2.color, y);
	Color color2 = Interpolate(point3[1], v3.color, point4[1], v4.color, y);
	float z1 = Interpolate(point1[1], point1[2], point2[1], point2[2], y);
	float z2 = Interpolate(point3[1], point3[2], point4[1], point4[2], y);
	vec2 uv1 = Interpolate(point1[1], v1.Texture2DCoord, point2[1], v2.Texture2DCoord, y);
	vec2 uv2 = Interpolate(point3[1], v3.Texture2DCoord, point4[1], v4.Texture2DCoord, y);

	int symbol = (scanx1 < scanx2) ? 1 : -1;
	for (int x = scanx1; x != scanx2 + symbol; x += symbol){
		float z = Interpolate(scanx1, z1, scanx2, z2, x);
		float u = Interpolate(scanx1, uv1[0], scanx2, uv2[0], x);
		float v = Interpolate(scanx1, uv1[1], scanx2, uv2[1], x);
		Color color = Interpolate(scanx1, color1, scanx2, color2, x);

		Fragment frag;
		frag.inColor = color;
		frag.z = z;
		frag.lightPos = LightPos;
		frag.pixelCoordinate = vec2(x, y);
		frag.texture = &texture;
		frag.textureCoordinate = vec2(u, v);
		FragmentShader(frag);
		
		DrawPoint(frag.pixelCoordinate, frag.z, frag.outColor, colorComp);
	}
}

void Renderer::FragmentShader(Fragment &frag){
	Color textureColor;
	if (frag.texture->IsEnable( )){
		textureColor = frag.texture->Map(frag.textureCoordinate[0], frag.textureCoordinate[1]);
	}else{
		textureColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	}

	//for simple, only use diffuse light
	if (EnablePhongShading){
		vec3 fragDir = frag.lightPos - frag.worldPos;
		float scale = ComputeNDotL(frag.worldPos, frag.normal, frag.lightPos);
		frag.outColor = frag.inColor * scale * textureColor;
	}else{
		frag.outColor = frag.inColor * textureColor;
	}
}

void Renderer::TriangleRasterization(Vertex v1, Vertex v2, Vertex v3, const Texture &texture, ColorCompent colorComp){
	if (v1.Position[1] > v2.Position[1]){
		auto temp = v1;
		v1 = v2;
		v2 = temp;
	}
	if (v1.Position[1] > v3.Position[1]){
		auto temp = v1;
		v1 = v3;
		v3 = temp;
	}
	if (v2.Position[1] > v3.Position[1]){
		auto temp = v2;
		v2 = v3;
		v3 = temp;
	}

	for (int y = (int)v1.Position[1]; y <= (int)v3.Position[1]; y++){
		if (y < v2.Position[1]){
			ScanLineXRasterization(y, v1, v2, v1, v3, texture, colorComp);
		}else{
			ScanLineXRasterization(y, v2, v3, v1, v3, texture, colorComp);
		}
	}
}

void Renderer::Render(const Camera &camera, const std::vector<Mesh> &meshes){
	mat4 viewM = camera.GetViewMatrix(); 
	mat4 projectionM = Transform::GetPerspectiveMatrix(45, (float)PixelWidth / (float)PixelHeight, 0.1f, 100.0f);

	for (auto const& mesh : meshes){
		mat4 worldM = mesh.GetWorldMatrixByEuler();
		mat4 matrix = projectionM * viewM * worldM;

		/*draw triangle*/
		int faceIndex = 0;
		size_t size = mesh.Faces.size( );
		//concurrency::parallel_for
		for (const auto &face : mesh.Faces){
			auto vertex1 = mesh.Vertices[face.index1];
			auto vertex2 = mesh.Vertices[face.index2];
			auto vertex3 = mesh.Vertices[face.index3];

			auto point1 = VertexShader(matrix, worldM, vertex1);
			auto point2 = VertexShader(matrix, worldM, vertex2);
			auto point3 = VertexShader(matrix, worldM, vertex3);

			//face culling
			vec3 pos = (point1.worldPosition+ point2.worldPosition + point3.worldPosition) / 3.0f;
			/*vec3 vector12 = point2.worldPosition - point1.worldPosition;
			vec3 vector23 = point3.worldPosition - point2.worldPosition;*/
			vec3 normal = (point1.Normal + point2.Normal + point3.Normal) / 3.0f;
			gmtl::normalize(normal);
			vec3 view = camera.Position - pos;
			gmtl::normalize(view);
			auto edotn = gmtl::dot(view, normal);
			if (!(edotn > 0.0f)){
				continue;
			}

			TriangleRasterization(point1, point2, point3, mesh.Texture2D, RGBA);
			faceIndex++;
			//auto colorv = 0.25f + (faceIndex % mesh.Faces.size( )) * 0.75f / mesh.Faces.size( );
			//float colorv = 1.0f;
			//Color color = { colorv, colorv, colorv , 1.0f};
			//Color color = { 0.4f, 0.2f, 1.0f, 1.0f };
		    
			/*DrawLine_Bresenham(point1, point2, color, RGBA);
			DrawLine_Bresenham(point2, point3, color, RGBA);
			DrawLine_Bresenham(point3, point1, color, RGBA);*/
		}
	}
}

float Renderer::Interpolate(float x1, float y1, float x2, float y2, float value){
	float t;
	if (FloatEqual(x1, x2)){
		t = 1;
	}
	else{
		t = (value - x1) / (x2 - x1);
		t = Clamb(t);
	}

	return (1 - t) * y1 + t * y2;
}

Color Renderer::Interpolate(float x1, Color color1, float x2, Color color2, float value){
	Color colorResult;
	colorResult.R = Interpolate(x1, color1.R, x2, color2.R, value);
	colorResult.G = Interpolate(x1, color1.G, x2, color2.G, value);
	colorResult.B = Interpolate(x1, color1.B, x2, color2.B, value);
	colorResult.A = Interpolate(x1, color1.A, x2, color2.A, value);

	return colorResult;
}

vec2 Renderer::Interpolate(float x1, vec2 uv1, float x2, vec2 uv2, float value){
	float u = Interpolate(x1, uv1[0], x2, uv2[0], value);
	float v = Interpolate(x1, uv1[1], x2, uv2[1], value);

	return vec2(u, v);
}

bool Renderer::FloatEqual(float a, float b){
	return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}

float Renderer::Clamb(float value, float min, float max){
	return std::max(min, std::min(value, max));
}

float Renderer::ComputeNDotL(vec3 vertex, vec3 normal, vec3 light){
	vec3 lightDir = light - vertex;
	gmtl::normalize(lightDir);
	gmtl::normalize(normal);
	return std::max(0.0f, gmtl::dot(lightDir, normal)); 
}

vec4 Renderer::DivideByW(vec4 vec){
	return vec4(vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3], 1.0f);
}

byte *Renderer::Getbuffer() const {
	return backBuffer;
}

void Renderer::Clear(Color color, ColorCompent colorComp)
{
	for (int index = 0; index < BufferSize; index += 4)
	{
		switch (colorComp)
		{
		case RGBA:{
					  backBuffer[index] = (byte)color.R * 255;
					  backBuffer[index + 1] = (byte)color.G * 255;
					  backBuffer[index + 2] = (byte)color.B * 255;
					  backBuffer[index + 3] = (byte)color.A * 255;
		}
		case BGRA:{
					  backBuffer[index] = (byte)color.B * 255;
					  backBuffer[index + 1] = (byte)color.G * 255;
					  backBuffer[index + 2] = (byte)color.R * 255;
					  backBuffer[index + 3] = (byte)color.A * 255;
		}
		default:
			break;
		}
	}

	for (int index = 0; index < BufferSize / 4; index++){
		depthBuffer[index] = std::numeric_limits<float>::infinity( );
	}
}