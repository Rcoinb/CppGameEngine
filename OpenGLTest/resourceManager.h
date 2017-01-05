#ifndef RESOURCEMANAGER_INCLUDED_H
#define RESOURCEMANAGER_INCLUDED_H

#include <string>
#include <vector>
#include "mesh.h"
#include "texture.h"
#include "material.h"

class ResourceManager
{
public:
	ResourceManager() {}

	template <typename T>
	inline T* LoadResourceFromFile(const std::string& fileName)
	{
		
		if (typeid(T) == typeid(Mesh))
		{
			Mesh* mesh = new Mesh(fileName);
			meshResources.push_back(mesh);
			return (T*) mesh;
		}

		if (typeid(T) == typeid(Texture))
		{
			Texture* texture = new Texture(fileName);
			textureResources.push_back(texture);
			return (T*) texture;
		}

		return nullptr;
	}

	template <typename T>
	void AddResource(T* recource)
	{
		if (typeid(T) == typeid(Mesh))
		{
			meshResources.push_back((Mesh*) recource);
			return;
		}

		if (typeid(T) == typeid(Texture))
		{
			textureResources.push_back((Texture*) recource);
			return;
		}

		if (typeid(T) == typeid(Material))
		{
			materialResources.push_back((Material*)recource);
			return;
		}

	}

	virtual ~ResourceManager()
	{
		for (auto i : meshResources)
		{
			delete i;
		}
		meshResources.clear();

		for (auto i : materialResources)
		{
			delete i;
		}
		materialResources.clear();

		for (auto i : textureResources)
		{
			delete i;
		}
		textureResources.clear();
	}
private:
	std::vector<Mesh*> meshResources;
	std::vector<Material*> materialResources;
	std::vector<Texture*> textureResources;
};

#endif
