#pragma once
#include "BaseComponent.h"
#include "Structs.h"
#include <vector>
#pragma warning (disable:4201)
#pragma warning (disable:4189)
#include <glm.hpp>

namespace kaas 
{
	class Texture2D;
	class TextureComponent;
	class TextComponent;
}

class CharacterControllerComponent;
class LevelComponent : public kaas::BaseComponent
{
public:
	LevelComponent(kaas::GameObject* pGameObject, std::string levelFilePath, std::string tileTexturePath, std::string discTexturePath, glm::vec2 tileSize, glm::vec2 startLocation);
	~LevelComponent();
	void Update() override;
	void Render() const override;

	Tile& GetTile(int tileID);
	glm::vec2 GetTilePos(int tileID);

	glm::vec2 GetVoidPos(int tileID, bool onLeftSide);

	glm::vec2 GetDiscEndLocation();

	bool IsOnDisc(glm::vec2 pos);

	void SetTileState(int tileID, TileAffection tileAffection);
	bool GetLevelFinished();
private:
	void ResetScene();
	std::vector<Tile> m_pTiles;
	std::vector<Disc> m_pPossibleDiscLocations;
	kaas::Texture2D* m_pTexture;
	kaas::Texture2D* m_pDiscTexture;
	kaas::TextComponent* m_pLevelText;
	glm::vec2 m_TileSize, m_StartLocation, m_DiscOffset, m_DiscEndLocation;
	Disc m_LeftDisc, m_RightDisc;
	int m_LevelNumber, m_TilesLeft, m_ResetTileCounter;
	bool m_ResettingScene;
	float m_ResetTimer, m_OriginalResetTimer, m_DiscMovementSpeed, m_DiscResetTimer, m_DiscOriginalResetTimer;
};
	
