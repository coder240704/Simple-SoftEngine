#include "Model.h"

std::vector<Mesh> Model::LoadMeshesFromJsonFile(std::string file){
	picojson::value v;
	try{
		std::ifstream modelStream(file);
		modelStream >> v;
		modelStream.close( );
		std::string err = picojson::get_last_error( );
		if (!err.empty( )){
			std::cout << err << std::endl;
			exit(-1);
		}
	}catch (std::exception e){
		std::cout << "error : cannot read model file" << std::endl;
		exit(-1);
	}

	if (!v.is<object>( )){
		std::cout << "json file is not a object" << std::endl;
		exit(-1);
	}
	object &obj = v.get<object>( );
	std::map<std::string, Material> materialDic;
	auto &materials = obj["materials"];
	auto &materialArray = materials.get<array>( );
	for (auto material : materialArray){
		auto &material_obj = material.get<object>( );
		Material materialStruct;
		materialStruct.Name = material_obj["name"].get<std::string>( );
		materialStruct.ID = material_obj["id"].get<std::string>( );
		if (material_obj.count("diffuseTexture")){
			materialStruct.DiffuseTextureName = material_obj["diffuseTexture"].get<object>( )["name"].get<std::string>( );
		}
		materialDic[materialStruct.ID] = materialStruct;
	}

	auto &meshes = obj["meshes"];
	auto &meshArray = meshes.get<array>( );

	std::vector<Mesh> meshVector;
	for (picojson::value &mesh : meshArray){
		auto &mesh_obj = mesh.get<object>();
		auto &verticesArray = mesh_obj["vertices"].get<array>( );
		auto &indicesArray = mesh_obj["indices"].get<array>( );
		int uvCount = (int)mesh_obj["uvCount"].get<double>();

		int verticesStep = 1;
		switch (uvCount){
		    case 0:	 verticesStep = 6; break;
		    case 1:	 verticesStep = 8; break;
		    case 2:	 verticesStep = 10; break;
		}

		int verticesCount = verticesArray.size( ) / verticesStep;
		int facesCount = indicesArray.size( ) / 3;
		std::vector<Face> indices(facesCount);
		std::vector<Vertex> vertexProperty(verticesCount);

		for (int i = 0; i < verticesCount; i++){
			float x = (float)verticesArray[i * verticesStep].get<double>();
			float y = (float)verticesArray[i * verticesStep + 1].get<double>();
			float z = (float)verticesArray[i * verticesStep + 2].get<double>();

			float nx = (float)verticesArray[i * verticesStep + 3].get<double>();
			float ny = (float)verticesArray[i * verticesStep + 4].get<double>();
			float nz = (float)verticesArray[i * verticesStep + 5].get<double>();

			//model is exported in D3D Coordinate, and I use Opengl Coordinate, so add "-"
			vertexProperty[i].Position = vec3(x, y, -z);
			vertexProperty[i].Normal = vec3(nx, ny, -nz);

			if (uvCount > 0){
				float u = (float)verticesArray[i * verticesStep + 6].get<double>();
				float v = (float)verticesArray[i * verticesStep + 7].get<double>();
				vertexProperty[i].Texture2DCoord = vec2(u, v);
			}
		}
		
		for (int i = 0; i < facesCount; i++){
			int index1 = (float)indicesArray[3 * i].get<double>( );
			int index2 = (float)indicesArray[3 * i + 1].get<double>( );
			int index3 = (float)indicesArray[3 * i + 2].get<double>();
			indices[i] = Face{index1, index2, index3};
		}

		vec3 position;
		position[0] = mesh_obj["position"].get<array>( )[0].get<double>();
		position[1] = mesh_obj["position"].get<array>( )[1].get<double>();
		position[2] = mesh_obj["position"].get<array>( )[2].get<double>();

		Mesh mesh_class(vertexProperty, indices, position, vec3(0.0f, 0.0f, 0.0f));
		mesh_class.Name = mesh_obj["name"].get<std::string>( );

		if (uvCount > 0){
			auto id = mesh_obj["materialId"].get<std::string>( );
			auto meshTextureName = materialDic[id].DiffuseTextureName;
			mesh_class.Texture2D = Texture(meshTextureName);
		}

		meshVector.push_back(mesh_class);
	}

	return meshVector;
}