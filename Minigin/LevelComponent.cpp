#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "TextComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"
#include "../3rdParty/rapidjson/rapidjson.h"
#include "../3rdParty/rapidjson/reader.h"
#include "../3rdParty/rapidjson/document.h"
#include <fstream>
#include <string>

using namespace rapidjson;
kaas::LevelComponent::LevelComponent(GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, glm::vec2 tileSize, glm::vec2 startLocation)
	:BaseComponent{pGameObject}
	,m_TileSize{tileSize}
	,m_StartLocation{startLocation}
	,m_LevelNumber{0}
	,m_ResetTimer{0.0f}
	,m_OriginalResetTimer{0.1f}
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(tileTexturePath);
	m_pLevelText = pGameObject->GetComponent<TextComponent>();

	Document doc;

	std::ifstream file(levelFilePath);
	std::stringstream buffer;
	buffer << file.rdbuf();

	doc.Parse(std::string(buffer.str()).c_str());
	
	const Value& mainLevelBlocksLocation = doc["mainLevelBlocksLocation"];

	//Check if the data we got has an array
	if (mainLevelBlocksLocation.IsArray())
	{
		for (SizeType i = 0; i < mainLevelBlocksLocation.Size(); i++)
		{
			Tile tile{};

			tile.pos = { mainLevelBlocksLocation[i].GetArray()[0].GetInt(), mainLevelBlocksLocation[i].GetArray()[1].GetInt() };
			tile.pos += m_StartLocation;
			m_pTiles.push_back(tile);
		}
	}

	m_TilesLeft = m_pTiles.size();
}

kaas::LevelComponent::~LevelComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void kaas::LevelComponent::Update()
{
	if (m_ResetTimer > 0.0f) 
	{
		m_ResetTimer -= Time::GetInstance().GetDeltaTime();

		if (m_ResetTimer <= 0.0f)
		{
			if (m_ResetTileCounter >= 0)
			{
				m_pTiles[m_ResetTileCounter].tileState = TileStates::standard;
				m_ResetTileCounter--;

				m_ResetTimer = m_OriginalResetTimer;

				if (m_ResetTileCounter == -1)
				{
					if (m_LevelNumber < 2) 
					{
						m_LevelNumber++;
						m_pLevelText->SetText("Level " + std::to_string(m_LevelNumber + 1));

						m_TilesLeft = m_pTiles.size();
					}
					else
					{
						m_LevelCompleted = true;
						m_pLevelText->SetText("Level Completed!");
					}
						

					
				}
			}
		}
	}
}

void kaas::LevelComponent::Render() const
{
	SDL_Rect rsc{}, dst{};
	for (const Tile& tile : m_pTiles)
	{
		rsc.x = static_cast<int>((m_LevelNumber * m_TileSize.x * 3) + m_TileSize.x * int(tile.tileState));
		rsc.y = static_cast<int>(0);
		rsc.w = static_cast<int>(m_TileSize.x);
		rsc.h = static_cast<int>(m_TileSize.y);

		dst.x = static_cast<int>(tile.pos.x);
		dst.y = static_cast<int>(tile.pos.y);
		dst.w = static_cast<int>(m_TileSize.x);
		dst.h = static_cast<int>(m_TileSize.y);

		kaas::Renderer::GetInstance().RenderTexture(*m_pTexture, dst, rsc);
	}
}

kaas::Tile& kaas::LevelComponent::GetTile(int tileID)
{
	return m_pTiles[tileID];
}

void kaas::LevelComponent::SetTileState(int tileID)
{
	switch (m_LevelNumber)
	{
	case 0:
		if (m_pTiles[tileID].tileState == TileStates::standard)
		{
			m_pTiles[tileID].tileState = TileStates::target;
			m_TilesLeft--;
		}
		break;
	case 1:
		if (m_pTiles[tileID].tileState == TileStates::standard)
			m_pTiles[tileID].tileState = TileStates::intermediate;

		else if(m_pTiles[tileID].tileState == TileStates::intermediate)
		{
			m_pTiles[tileID].tileState = TileStates::target;
			m_TilesLeft--;
		}
		break;
	case 2:
		if (m_pTiles[tileID].tileState == TileStates::standard) 
		{
			m_pTiles[tileID].tileState = TileStates::target;
			m_TilesLeft--;
		}	
		else
		{
			m_pTiles[tileID].tileState = TileStates::standard;
			m_TilesLeft++;
		}	
		break;
	}

	if (m_TilesLeft == 0)
	{
		ResetScene();
	}
}

void kaas::LevelComponent::ResetScene()
{
	m_ResetTileCounter = m_pTiles.size() - 1;

	//Multiply by 2 to give it a small delay
	m_ResetTimer = m_OriginalResetTimer * 2;
}

bool kaas::LevelComponent::GetLevelFinished()
{
	return m_TilesLeft == 0;
}
