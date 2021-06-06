#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void kaas::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		if (scene.get()->GetName() == m_ActiveScene)
		{
			scene->Update();
		}
	}
}

void kaas::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		if (scene.get()->GetName() == m_ActiveScene)
		{
			scene->Render();
		}
	}
}

kaas::Scene& kaas::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}

void kaas::SceneManager::SetActiveScene(std::string sceneName)
{
	m_ActiveScene = sceneName;
}

std::string kaas::SceneManager::GetActiveScene() const
{
	return m_ActiveScene;
}
