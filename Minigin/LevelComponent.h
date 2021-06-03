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
		target,
		intermediate
	};

	struct Tile 
	{
		glm::vec2 pos;
		TileStates tileState = TileStates{ TileStates::standard};
	};

	struct Disc
	{
		glm::vec2 pos;
		bool hasPlayerOnTop;
	};

	class TextureComponent;
	class TextComponent;
	class LevelComponent : public BaseComponent
	{
	public:
		LevelComponent(GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, glm::vec2 tileSize, glm::vec2 startLocation);
		~LevelComponent();
		void Update() override;
		void Render() const override;

		Tile& GetTile(int tileID);
		void SetTileState(int tileID);
		bool GetLevelFinished();

	private:
		void ResetScene();
		std::vector<Tile> m_pTiles;
		Texture2D* m_pTexture;
		TextComponent* m_pLevelText;
		glm::vec2 m_TileSize, m_StartLocation;
		Disc m_LeftDisc, m_RightDisc;
		int m_LevelNumber, m_TilesLeft, m_ResetTileCounter;
		bool m_ResettingScene, m_LevelCompleted;
		float m_ResetTimer, m_OriginalResetTimer;
	};
}