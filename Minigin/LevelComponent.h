#pragma once
#include "BaseComponent.h"
#include <vector>
#pragma warning (disable:4201)
#pragma warning (disable:4189)
#include <glm/vec2.hpp>

namespace kaas 
{
	class Texture2D;
	enum class TileStates
	{
		standard,
		intermediate,
		target
	};
	struct Tile 
	{
		glm::vec2 pos;
		TileStates tileState = TileStates{ TileStates::standard};
	};

	class TextureComponent;
	class LevelComponent : public BaseComponent
	{
	public:
		LevelComponent(GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, glm::vec2 tileSize, glm::vec2 startLocation);
		~LevelComponent();
		void Update() override;
		void Render() const override;

		Tile& GetTile(int tileID);
	private:
		std::vector<Tile> m_pTiles;
		Texture2D* m_pTexture;
		glm::vec2 m_TileSize, m_StartLocation;
	};
}