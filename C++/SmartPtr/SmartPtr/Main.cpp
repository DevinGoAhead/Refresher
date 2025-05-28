#include "UniquePtr.hpp"
#include "SharedPtr.hpp"

#include <iostream>

int main() {
	UniquePtr<int> ptr1(new int(1));
	std::cout << *ptr1 << '\n';
	
	auto ptr2(std::move(ptr1));
	std::cout << *ptr2 << '\n';
	ptr1 = std::move(ptr2);
	std::cout << *ptr1 << '\n';

	SharedPtr<int> ptr3(new int(2));
	std::cout << *ptr3 << '\n';

	SharedPtr<int> ptr4 = ptr3;
	std::cout << *ptr3 << '\n';

	SharedPtr<int> ptr5(new int(3));
	SharedPtr<int> ptr6(ptr5);
	ptr6 = std::move(ptr4);
	std::cout << *ptr5 << '\n';
}