#include "QBertGamePCH.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Subject.h"
#include "../3rdParty/rapidjson/rapidjson.h"
#include "../3rdParty/rapidjson/reader.h"
#include "../3rdParty/rapidjson/document.h"
#include "Structs.h"
#include <fstream>
#include <string>
#include "Components.h"

using namespace rapidjson;
LevelComponent::LevelComponent(kaas::GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, std::string discTexturePath, glm::vec2 tileSize, glm::vec2 startLocation)
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
	,m_ResetTileCounter{0}
	,m_ResettingScene{false}
{
	m_pTexture = kaas::ResourceManager::GetInstance().LoadTexture(tileTexturePath);
	m_pDiscTexture = kaas::ResourceManager::GetInstance().LoadTexture(discTexturePath);
	m_pLevelText = pGameObject->GetComponent<kaas::TextComponent>();

	if (!m_pLevelText)
	{
		std::cout << "text not found\n";
	}

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

		m_TilesLeft = int(m_pTiles.size());
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

				//for every variable in the object, store it in a disc
				for (Value::ConstMemberIterator itr = flyingDiscs[i].MemberBegin(); itr != flyingDiscs[i].MemberEnd(); ++itr)
				{
					if (itr->name == "tileConnection")
						disc.tileConnectionID = itr->value.GetInt();
					if (itr->name == "IsLeft")
						isLeft = itr->value.GetBool();
					if (itr->name == "level")
						disc.level = itr->value.GetInt();
				}

				//The pos of the disc is on the left side of the texture, so add an extra offset if the disc is on the right
				disc.pos.x = isLeft ? m_pTiles[disc.tileConnectionID].pos.x - m_DiscOffset.x : m_pTiles[disc.tileConnectionID].pos.x + m_DiscOffset.x + m_DiscOffset.y;
				disc.pos.y = m_pTiles[disc.tileConnectionID].pos.y + m_DiscOffset.y;
				m_pPossibleDiscLocations.push_back(disc);
			}
		}

		m_TilesLeft = int(m_pTiles.size());
		m_DiscEndLocation = m_pTiles[0].pos - glm::vec2{ -m_DiscOffset.y, m_DiscOffset.x};
	}
}

LevelComponent::~LevelComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;

	delete m_pDiscTexture;
	m_pTexture = nullptr;
}

void LevelComponent::Update()
{
	//Let the disc float a second before making it inactive
	if (m_DiscResetTimer > 0.0f)
	{
		m_DiscResetTimer -= kaas::Timer::GetInstance().GetDeltaTime();

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

	//Update the movement of the discs that are in use
	for (Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.InUse)
		{
			//Lerp the distance like the player
			glm::vec2 movement{ (disc.pos + (m_DiscEndLocation - disc.pos) * kaas::Timer::GetInstance().GetDeltaTime() * m_DiscMovementSpeed) };

			if (glm::length(disc.pos - movement) < 0.2f)
			{
				disc.pos = m_DiscEndLocation;
				disc.InUse = false;
				m_DiscResetTimer = m_DiscOriginalResetTimer;
			}
			else
			{
				disc.pos = movement;
			}
		}
	}

	//Timer for resetting the level
	if (m_ResetTimer > 0.0f) 
	{
		m_ResetTimer -= kaas::Timer::GetInstance().GetDeltaTime();

		if (m_ResetTimer <= 0.0f)
		{
			if (m_ResetTileCounter >= 0)
			{
				m_pTiles[m_ResetTileCounter].tileState = TileStates::standard;
				m_ResetTileCounter--;

				m_ResetTimer = m_OriginalResetTimer;

				if (m_ResetTileCounter == -1)
				{
					if (m_LevelNumber < 0) 
					{
						m_LevelNumber++;
						m_pLevelText->SetText("Level " + std::to_string(m_LevelNumber + 1));
						m_TilesLeft = int(m_pTiles.size());
					}
					else
					{
						kaas::SceneManager::GetInstance().SetActiveScene("EndMenu");
					}
				}
			}
		}
	}
}

void LevelComponent::Render() const
{
	SDL_Rect rsc{}, dst{};
	for (const Tile& tile : m_pTiles)
	{
		rsc.x = static_cast<int>((m_LevelNumber * m_TileSize.x * 3) + m_TileSize.x * int(tile.tileState));
		rsc.y = 0;
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
			kaas::Renderer::GetInstance().RenderTexture(*m_pDiscTexture, disc.pos.x, disc.pos.y);
		}
	}
}

Tile& LevelComponent::GetTile(int tileID)
{
	return m_pTiles[tileID];
}

void LevelComponent::SetTileState(int tileID, TileAffection tileAffection)
{
	switch (m_LevelNumber)
	{
	case 0:
		if (m_pTiles[tileID].tileState == TileStates::standard && tileAffection == TileAffection::always)
		{
			m_pTiles[tileID].tileState = TileStates::target;
			m_TilesLeft--;
			m_pGameObject->GetSubject()->notify(*m_pGameObject, Event::ColorChange);
		}
		if (m_pTiles[tileID].tileState == TileStates::target && tileAffection == TileAffection::onlyActive)
		{
			m_pTiles[tileID].tileState = TileStates::standard;
			m_TilesLeft++;
		}
		break;
	case 1:

		if (tileAffection == TileAffection::always)
		{
			if (m_pTiles[tileID].tileState == TileStates::standard)
			{
				m_pTiles[tileID].tileState = TileStates::intermediate;
				m_pGameObject->GetSubject()->notify(*m_pGameObject, Event::ColorChange);
			}

			else if (m_pTiles[tileID].tileState == TileStates::intermediate)
			{
				m_pTiles[tileID].tileState = TileStates::target;
				m_TilesLeft--;
				m_pGameObject->GetSubject()->notify(*m_pGameObject, Event::ColorChange);
			}
		}
		else if (tileAffection == TileAffection::onlyActive)
		{
			if (m_pTiles[tileID].tileState == TileStates::intermediate)
				m_pTiles[tileID].tileState = TileStates::standard;

			else if (m_pTiles[tileID].tileState == TileStates::target)
			{
				m_pTiles[tileID].tileState = TileStates::intermediate;
				m_TilesLeft++;
			}
		}
		
		break;
	case 2:
		if (tileAffection == TileAffection::always)
		{
			if (m_pTiles[tileID].tileState == TileStates::target)
			{
				m_pTiles[tileID].tileState = TileStates::standard;
				m_TilesLeft++;
			}
			else
			{
				m_pTiles[tileID].tileState = TileStates::target;
				m_pGameObject->GetSubject()->notify(*m_pGameObject, Event::ColorChange);
				m_TilesLeft--;
			}
		}
		else if (tileAffection == TileAffection::onlyActive)
		{
			if (m_pTiles[tileID].tileState == TileStates::target)
			{
				m_pTiles[tileID].tileState = TileStates::standard;
				m_TilesLeft++;
			}
		}
		break;
	}

	if (m_TilesLeft == 0)
	{
		ResetScene();
	}
}

void LevelComponent::ResetScene()
{
	m_ResetTileCounter = int(m_pTiles.size()) - 1;

	for (const Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.level == m_LevelNumber)
			m_pGameObject->GetSubject()->notify(*m_pGameObject, Event::DiscRemained);
	}

	//Multiply by 2 to give it a small delay
	m_ResetTimer = m_OriginalResetTimer * 2;
}

bool LevelComponent::GetLevelFinished()
{
	return m_TilesLeft == 0;
}

glm::vec2 LevelComponent::GetTilePos(int tileID)
{
	//Add the discOffset.y, which is the same as the offset between the pos of the tile and the middle of it
	glm::vec2 pos{ m_pTiles[tileID].pos };
	pos.x += m_DiscOffset.y;
	return pos;
}

glm::vec2 LevelComponent::GetVoidPos(int tileID, bool onLeftSide)
{
	glm::vec2 pos{};
	if (tileID <= int(m_pTiles.size())-1)
	{
		//If tile has a connection to a disc, return that position
		for (const Disc& disc : m_pPossibleDiscLocations)
		{
			if (disc.tileConnectionID == tileID && disc.level == m_LevelNumber)
			{
				return disc.pos;
			}
		}

		pos = m_pTiles[tileID].pos;

		//On the right side, add the offset plus the offset from the pos being on the left side of the texture
		pos.x = onLeftSide ? pos.x - m_DiscOffset.x : pos.x + m_DiscOffset.x + m_DiscOffset.y;
		pos.y += m_DiscOffset.y;
	}
	else
	{
		//Get the pos under the pyramid
		pos = m_pTiles[tileID - 7].pos;
		pos.x += m_DiscOffset.x;
		pos.y += m_DiscOffset.y;

		return pos;
	}

	return pos;
}

glm::vec2 LevelComponent::GetDiscEndLocation()
{
	return m_DiscEndLocation;
}

bool LevelComponent::IsOnDisc(glm::vec2 pos)
{
	for (Disc& disc : m_pPossibleDiscLocations)
	{
		if (disc.pos == pos && disc.level == m_LevelNumber)
		{
			disc.InUse = true;
			return true;
		}
	}

	return false;
}