#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Subject.h"

kaas::GameObject::GameObject()
{
	m_pSubject = new Subject();
}

kaas::GameObject::~GameObject()
{
	delete m_pSubject;
	m_pSubject = nullptr;

	for (kaas::BaseComponent* pComponent : m_pComponents)
	{
		delete pComponent;
		pComponent = nullptr;
	}
};

void kaas::GameObject::Update()
{
	for (kaas::BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void kaas::GameObject::Render() const
{
	for (kaas::BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Render();
	}
}

void kaas::GameObject::AddComponent(BaseComponent* pComponent)
{
	m_pComponents.push_back(pComponent);
}
