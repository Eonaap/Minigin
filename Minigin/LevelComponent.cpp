#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "TextComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "../3rdParty/rapidjson/rapidjson.h"
#include "../3rdParty/rapidjson/reader.h"
#include "../3rdParty/rapidjson/document.h"
#include <fstream>
#include <string>

using namespace rapidjson;
kaas::LevelComponent::LevelComponent(GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, std::string discTexturePath, glm::vec2 tileSize, glm::vec2 startLocation)
	:BaseComponent{pGameObject}
	,m_TileSize{tileSize}
	,m_StartLocation{startLocation}
	,m_LevelNumber{0}
	,m_ResetTimer{0.0f}
	,m_OriginalResetTimer{0.1f}
	, m_DiscResetTimer{ 0.0f }
	, m_DiscOriginalResetTimer{ 1.0f }
	,m_DiscMovementSpeed{ 4.0f }
	,m_DiscOffset{ glm::vec2{45.0f, 15.0f} }
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(tileTexturePath);
	m_pDiscTexture = ResourceManager::GetInstance().LoadTexture(discTexturePath);
	m_pLevelText = pGameObject->GetComponent<TextComponent>();

	Document doc;

	std::ifstream file(levelFilePath);
	std::stringstream buffer;
	buffer << file.rdbuf();

	doc.Parse(std::string(buffer.str()).c_str());
	
	//Tile generation
	{
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

	//Disc generation
	{
		const Value& flyingDiscs = doc["flyingDiscs"];

		//Check if the data we got has an array
		if (flyingDiscs.IsArray())
		{
			for (SizeType i = 0; i < flyingDiscs.Size(); i++)
			{
				Disc disc{};
				bool isLeft{};
				for (Value::ConstMemberIterator itr = flyingDiscs[i].MemberBegin(); itr != flyingDiscs[i].MemberEnd(); ++itr)
				{
					if (itr->name == "tileConnection")
						disc.tileConnectionID = itr->value.GetInt();
					if (itr->name == "IsLeft")
						isLeft = itr->value.GetBool();
					if (itr->name == "level")
						disc.level = itr->value.GetInt();
				}

				//disc.tileConnectionID = flyingDiscs[i].GetInt();
				//bool isLeft = flyingDiscs[i].GetBool();
				disc.pos.x = isLeft ? m_pTiles[disc.tileConnectionID].pos.x - m_DiscOffset.x : m_pTiles[disc.tileConnectionID].pos.x + m_DiscOffset.x + m_DiscOffset.y;
				disc.pos.y = m_pTiles[disc.tileConnectionID].pos.y + m_DiscOffset.y;
				m_pPossibleDiscLocations.push_back(disc);
			}
		}

		m_TilesLeft = m_pTiles.size();
		m_DiscEndLocation = m_pTiles[0].pos - glm::vec2{ -m_DiscOffset.y, m_DiscOffset.x};
	}
}

kaas::LevelComponent::~LevelComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void kaas::LevelComponent::Update()
{
	if (m_DiscResetTimer > 0.0f)
	{
		m_DiscResetTimer -= Timer::GetInstance().GetDeltaTime();

		if (m_DiscResetTimer < 0.0f)
		{
			for (Disc& disc : m_pPossibleDiscLocations)
			{
				if (disc.pos == m_DiscEndLocation)
				{
					disc.level = -1;
				}
			}
		}
	}

	for (Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.InUse)
		{
			glm::vec2 movement{};

			movement = (disc.pos + (m_DiscEndLocation - disc.pos) * Timer::GetInstance().GetDeltaTime() * m_DiscMovementSpeed);

			if (glm::length(disc.pos - movement) < 0.2f)
			{
				disc.pos = m_DiscEndLocation;
				disc.InUse = false;
				m_DiscIsMoving = false;
				m_DiscResetTimer = m_DiscOriginalResetTimer;
			}
			else
			{
				disc.pos = movement;
			}
		}
	}

	if (m_ResetTimer > 0.0f) 
	{
		m_ResetTimer -= Timer::GetInstance().GetDeltaTime();

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

	for (const Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.level == m_LevelNumber)
		{
			rsc.x = static_cast<int>(0.0f);
			rsc.y = static_cast<int>(0);
			rsc.w = static_cast<int>(m_TileSize.x);
			rsc.h = static_cast<int>(m_TileSize.y);

			dst.x = static_cast<int>(disc.pos.x);
			dst.y = static_cast<int>(disc.pos.y);
			dst.w = static_cast<int>(35);
			dst.h = static_cast<int>(22);

			kaas::Renderer::GetInstance().RenderTexture(*m_pDiscTexture, dst, rsc);
		}
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

glm::vec2 kaas::LevelComponent::GetTilePos(int tileID)
{
	//Add the discOffset.y, which is the same as the offset between the pos of the tile and the middle of it
	glm::vec2 pos{ m_pTiles[tileID].pos };
	pos.x += m_DiscOffset.y;
	return pos;
}

glm::vec2 kaas::LevelComponent::GetVoidPos(int tileID, bool onLeftSide)
{
	glm::vec2 pos{ m_pTiles[tileID].pos };

	//On the right side, add the offset instead of subtracting + teh offset from the pos being on the left side of the texture
	pos.x = onLeftSide ? pos.x - m_DiscOffset.x : pos.x + m_DiscOffset.x + m_DiscOffset.y;
	pos.y += m_DiscOffset.y;

	for (const Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.tileConnectionID == tileID && disc.level == m_LevelNumber)
		{
			pos = disc.pos;
		}
	}
	
	return pos;
}

glm::vec2 kaas::LevelComponent::GetDiscEndLocation()
{
	return m_DiscEndLocation;
}

bool kaas::LevelComponent::IsOnDisc(glm::vec2 pos)
{
	for (Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.pos == pos && disc.level == m_LevelNumber)
		{
			disc.pos;
			disc.InUse = true;
			return true;
		}
	}

	return false;
}