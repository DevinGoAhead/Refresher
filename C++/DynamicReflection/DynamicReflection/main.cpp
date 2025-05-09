#include "Type.h"
#include <iostream>
enum class EState {
	jump, run, idle
};
int main() {
	auto it = Register<EState>();
	it->Register("EState");
	it->Add("EState", EState::jump);
	it->Add("EState", EState::run);
	it->Add("EState", EState::idle);
	std::cout << it->Info().GetName() << "\n";
	std::cout << it->Info().GetItems()[0]._value << "\n";
	std::cout << it->Info().GetItems()[1]._value << "\n";
	std::cout << it->Info().GetItems()[2]._value << "\n";
}