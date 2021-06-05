#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace kaas;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() 
{
	for (GameObject* pGameObject : m_pObjects)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}
}

void Scene::Add(GameObject* object)
{
	m_pObjects.push_back(object);
}

void Scene::Update()
{
	for(auto& object : m_pObjects)
	{
		if (object->GetActive())
			object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		if (object->GetActive())
			object->Render();
	}
}

