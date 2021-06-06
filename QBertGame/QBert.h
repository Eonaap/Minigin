#pragma once
#include "QBertGamePCH.h"
#include "Minigin.h"

class QBert : public kaas::Minigin
{
public:
	QBert() = default;
	virtual ~QBert();
	void LoadGame() const override;

private:
	void SetupInput() const;
	void SetupScene() const;
	
};

