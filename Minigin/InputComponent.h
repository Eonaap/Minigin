#include "BaseComponent.h"
#include "Structs.h"
#include <vector>
namespace kaas 
{
	class InputComponent : public BaseComponent
	{
	public:
		InputComponent(GameObject* pGameObject, int playerID);
		~InputComponent() override;

		void Update() override;
		void UpdateThumbStick(ControllerAction& action);
		void Render() const override;
		void AddAction(ControllerAction action);
		int GetPlayerID() const;
	private:
		std::vector<ControllerAction> m_Actions;
		int m_PlayerID;
	};
}