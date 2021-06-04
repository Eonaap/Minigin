#pragma once
#include "BaseComponent.h"
#include <vector>
#pragma warning (disable:4201)
#pragma warning (disable:4189)
#include <glm.hpp>

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
		bool InUse;
		int tileConnectionID;
		int level = -1;
	};

	class TextureComponent;
	class TextComponent;
	class LevelComponent : public BaseComponent
	{
	public:
		LevelComponent(GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, std::string discTexturePath, glm::vec2 tileSize, glm::vec2 startLocation);
		~LevelComponent();
		void Update() override;
		void Render() const override;

		Tile& GetTile(int tileID);
		glm::vec2 GetTilePos(int tileID);

		glm::vec2 GetVoidPos(int tileID, bool onLeftSide);

		glm::vec2 GetDiscEndLocation();

		bool IsOnDisc(glm::vec2 pos);

		void SetTileState(int tileID);
		bool GetLevelFinished();

	private:
		void ResetScene();
		std::vector<Tile> m_pTiles;
		std::vector<Disc> m_pPossibleDiscLocations;
		Texture2D* m_pTexture;
		Texture2D* m_pDiscTexture;
		TextComponent* m_pLevelText;
		glm::vec2 m_TileSize, m_StartLocation, m_DiscOffset, m_DiscEndLocation;
		Disc m_LeftDisc, m_RightDisc;
		int m_LevelNumber, m_TilesLeft, m_ResetTileCounter;
		bool m_ResettingScene, m_LevelCompleted, m_DiscIsMoving;
		float m_ResetTimer, m_OriginalResetTimer, m_DiscMovementSpeed, m_DiscResetTimer, m_DiscOriginalResetTimer;
	};
}