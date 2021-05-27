#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

kaas::GameObject::~GameObject()
{
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
