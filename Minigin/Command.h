#pragma once
#include <iostream>

namespace kaas
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class EmptyCommand : public Command
	{
	public:
		void Execute() override { std::cout << "This button has no command assigned\n"; }
	};

	class JumpCommand : public Command
	{
	public:
		void Execute() override { std::cout << "Jump!\n"; }
	};

	class DuckCommand : public Command
	{
	public:
		void Execute() override { std::cout << "Duck!\n"; }
	};

	class FireCommand : public Command
	{
	public:
		void Execute() override { std::cout << "Fire!\n"; }
	};

	class ShieldCommand : public Command
	{
	public:
		void Execute() override { std::cout << "Shield!\n"; }
	};
}