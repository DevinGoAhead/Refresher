#include "BitSet.hpp"
#include "iostream"

int main()
{
	BitSet<0xffffff> bitSet;
	bitSet.Set(10124);
	bitSet.Set(0);
	bitSet.Set(1012);
	bitSet.Set(1);
	std::cout << bitSet.Prob(10121) << std::endl;
	std::cout << bitSet.Prob(10124) << std::endl;
	std::cout << bitSet.Prob(1) << std::endl;
	std::cout << bitSet.Prob(0) << std::endl;
	bitSet.Reset(1);
	std::cout << bitSet.Prob(1) << std::endl;
}