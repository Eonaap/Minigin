#pragma once
namespace kaas
{
	class GameObject;

	enum class Event {
		LoseLife
	};

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() {};
		virtual void onNotify(const GameObject& gameObject, const Event& eventInfo) = 0;
	};
}