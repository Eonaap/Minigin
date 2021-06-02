#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "../3rdParty/rapidjson/rapidjson.h"
#include "../3rdParty/rapidjson/reader.h"
#include "../3rdParty/rapidjson/document.h"
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>

using namespace rapidjson;
kaas::LevelComponent::LevelComponent(GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, glm::vec2 tileSize, glm::vec2 startLocation)
	:BaseComponent{pGameObject}
	,m_TileSize{tileSize}
	,m_StartLocation{startLocation}
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(tileTexturePath);

	Document doc;

	std::ifstream file(levelFilePath);
	std::stringstream buffer;
	buffer << file.rdbuf();

	doc.Parse(std::string(buffer.str()).c_str());
	
	const Value& mainLevelBlocksLocation = doc["mainLevelBlocksLocation"];

	//Check if the data we got has an array
	if (mainLevelBlocksLocation.IsArray())
	{
		bool changeColour{ false };
		for (SizeType i = 0; i < mainLevelBlocksLocation.Size(); i++)
		{
			Tile tile{};
			if (changeColour)
			{
				tile.tileState = TileStates::target;
			}
			else
			{
				tile.tileState = TileStates::standard;
			}

			changeColour = !changeColour;

			tile.pos = { mainLevelBlocksLocation[i].GetArray()[0].GetInt(), mainLevelBlocksLocation[i].GetArray()[1].GetInt() };
			tile.pos += m_StartLocation;
			m_pTiles.push_back(tile);
		}
	}
		
	std::cout << "Done with reading json\n";
}

kaas::LevelComponent::~LevelComponent()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void kaas::LevelComponent::Update()
{
}

void kaas::LevelComponent::Render() const
{

	SDL_Rect rsc{}, dst{};
	for (const Tile& tile : m_pTiles)
	{
		rsc.x = static_cast<int>(m_TileSize.x * int(tile.tileState));
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
