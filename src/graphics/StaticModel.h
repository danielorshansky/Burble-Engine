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

class StaticModel {
public:
	StaticModel(const char *raw_path, bool gamma = false) : gamma(gamma) {
		Assimp::Importer importer;
		std::string path(raw_path);
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals | aiProcess_PreTransformVertices);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "Assimp Error: " << importer.GetErrorString() << std::endl;
			return;
		}
		std::replace(path.begin(), path.end(), '\\', '/');
		directory = path.substr(0, path.find_last_of('/'));
		Load(scene, scene->mRootNode);
	}

	void Render(Shader &shader, glm::mat4 &transformation) {
		for (Mesh &mesh : meshes)
			mesh.Render(shader, transformation);
	}

	void Clean() {
		for (Mesh &mesh : meshes)
			mesh.Clean();
	}

private:
	bool gamma;
	std::string directory;
	std::vector<Mesh> meshes;

	void Load(const aiScene *scene, aiNode *node) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
			meshes.push_back(ProcessMesh(scene, scene->mMeshes[node->mMeshes[i]]));
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			Load(scene, node->mChildren[i]);
	}

	Mesh ProcessMesh(const aiScene *scene, aiMesh *mesh) {
		aiColor4D raw_color;
		glm::vec4 color;
		bool colored = false;
		Material material;
		aiMaterial *raw_material = scene->mMaterials[mesh->mMaterialIndex];
		if (aiGetMaterialColor(raw_material, AI_MATKEY_COLOR_DIFFUSE, &raw_color) == AI_SUCCESS) {
			colored = true;
			color = glm::vec4(raw_color.r, raw_color.g, raw_color.b, raw_color.a);
		}
		std::vector<Texture> textures;
		std::vector<std::string> paths;
		std::vector<glm::vec3> positions;
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			if (mesh->HasTextureCoords(0))
				material.tex_coords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
			if (colored)
				material.colors.push_back(color);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		for (unsigned int i = 0; i < raw_material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString path;
			if (raw_material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS) {
				std::string full_path = directory + "/" + path.data;
				std::vector<std::string>::iterator iterator = std::find(paths.begin(), paths.end(), full_path);
				if (iterator != paths.end())
					textures.push_back(textures[iterator - paths.begin()]);
				else {
					textures.push_back(Texture(full_path.c_str()));
					paths.push_back(full_path);
				}
			}
		}
		material.textures = textures;
		return Mesh(positions, material, indices);
	}
};
