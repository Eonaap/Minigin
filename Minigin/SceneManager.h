#pragma once
#include "Singleton.h"
#include <string>

namespace kaas
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void SetActiveScene(std::string sceneName);
		std::string GetActiveScene() const;

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::string m_ActiveScene;
	};
}
