#pragma once
#include <vector>
namespace kaas
{
	class BaseComponent;
	class Subject;

	class GameObject final
	{
	public:
		void Update();
		void Render() const;

		void AddComponent(kaas::BaseComponent* pComponent);

		template<typename T>
		T* GetComponent() const;
		Subject* GetSubject() { return m_pSubject; };

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<kaas::BaseComponent*> m_pComponents;
		Subject* m_pSubject;
	};

	template<typename T>
	inline T* GameObject::GetComponent() const
	{
		for (BaseComponent* pComponent : m_pComponents)
		{
			T* pComponentCast = dynamic_cast<T*>(pComponent);

			if (pComponentCast)
				return pComponentCast;
		}

		return nullptr;
	}
}
