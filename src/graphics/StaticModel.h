#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Texture.h"

class StaticModel {
public:
	StaticModel(const char *raw_path, bool gamma = false) : gamma(gamma) { // load model from path
		Assimp::Importer importer;
		std::string path(raw_path);
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals | aiProcess_PreTransformVertices); // initialize scene
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // check for errors
			std::cerr << "Assimp Error: " << importer.GetErrorString() << std::endl;
			return;
		}
		std::replace(path.begin(), path.end(), '\\', '/');
		directory = path.substr(0, path.find_last_of('/'));
		Load(scene, scene->mRootNode); // load
	}

	void Render(Shader &shader, glm::mat4 &model, Camera &camera, Window &window) { // render
		for (Mesh &mesh : meshes) // render each mesh
			mesh.Render(shader, model, camera, window);
	}

	void Clean() { // clean
		for (Mesh &mesh : meshes) // clean mesh
			mesh.Clean();
	}

private:
	bool gamma;
	std::string directory;
	std::vector<Mesh> meshes;

	void Load(const aiScene *scene, aiNode *node) { // load model
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
			meshes.push_back(ProcessMesh(scene, scene->mMeshes[node->mMeshes[i]])); // add mesh
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			Load(scene, node->mChildren[i]); // load each branch
	}

	Mesh ProcessMesh(const aiScene *scene, aiMesh *mesh) { // process each mesh
		aiColor4D raw_color;
		glm::vec4 color;
		bool colored = false;
		Material material;
		aiMaterial *raw_material = scene->mMaterials[mesh->mMaterialIndex];
		if (aiGetMaterialColor(raw_material, AI_MATKEY_COLOR_DIFFUSE, &raw_color) == AI_SUCCESS) { // check if colored
			colored = true;
			color = glm::vec4(raw_color.r, raw_color.g, raw_color.b, raw_color.a); // set color
		}
		std::vector<Texture> textures;
		std::vector<std::string> paths;
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z)); // add each vertex
			if (mesh->HasTextureCoords(0)) // if textured add texture coordinates
				material.tex_coords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			if (colored) // if colored add color
				material.colors.push_back(color);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) { // add all indices
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		for (unsigned int i = 0; i < raw_material->GetTextureCount(aiTextureType_DIFFUSE); i++) { // add all textures
			aiString path;
			if (raw_material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS) {
				std::string full_path = directory + '/' + path.data;
				std::vector<std::string>::iterator iterator = std::find(paths.begin(), paths.end(), full_path);
				if (iterator != paths.end())
					textures.push_back(textures[iterator - paths.begin()]);
				else {
					textures.push_back(Texture(full_path.c_str()));
					paths.push_back(full_path);
				}
			}
		}
		material.textures = textures; // set material textures
		return Mesh(positions, material, indices); // return mesh
	}
};
