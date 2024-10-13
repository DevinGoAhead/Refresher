#include "String.hpp"

int main()
{
	const char* cStr1 = "constCString";
	std::cout << strlen(cStr1)<<std::endl;

	//construction
	String str1(cStr1);
	String str2(str1);
	String str4;
	str4 = cStr1;
	String str5 = "String5";
	
	std::cout << "***str1(copied from cStr1)***" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;
	
	//Rsize
	str1.Resize(20, 'c');
	std::cout << "str1.Resize(14, 'c');" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;

	//Erase
	str1.Erase(8, 6);
	std::cout << "str1.Erase(8, 14)" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;

	//Insert char
	str1.Insert(10, 'x');
	std::cout << "Insert char(10, 'x')" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;
	
	str1.Insert(0, 'a');
	std::cout << "Insert char(0, 'a')" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;
	
	str1.Insert(str1.Size(), 'z');
	std::cout << "Insert char(str1.Size() - 1, 'z')" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;
	
	//Insert str
	str1.Insert(11, "-insert");
	std::cout << "str1.Insert(11, -insert)" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;
	
	// String str6 = "0123456789";
	// str6.Insert(7, "abcd");
	// std::cout << "str6.Insert(7, abcd)" << std::endl;
	// std::cout << str6 <<" | size: "<< str6.Size() << " | capacity:" << str6.Capacity()<< std::endl;
	
	str1.Insert(0, str5);
	std::cout << "str1.Insert(0, str5);" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;
	
	str1.Insert(str1.Size(), (String)"last");
	std::cout << "str1.Insert(str1.Size(), last);" << std::endl;
	std::cout << str1 <<" | size: "<< str1.Size() << " | capacity:" << str1.Capacity()<< std::endl;

	//find
	size_t pos1 = str1.Find('c', 9);
	size_t pos2 = str1.Find("const", 5);
	std::cout << pos1 << " | "<< pos2 << std::endl;
}