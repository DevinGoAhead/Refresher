#include "Vector.hpp"

int main()
{
	Vector<int> v1;
	std::cout<<v1.Empty()<<std::endl;

	Vector<int> v2(1);
	v2.PushBack(2);
	v2.PushBack(3);
	v2.PushBack(4);
	v2.PushBack(5);
	v2.PushBack(6);
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "Size: "<< v2.Size() << " | Capacity: " << v2.Capacity() << std::endl;
	
	std::cout<<std::endl;

	std::cout<<"Insert(0, 0)"<<std::endl;
	Vector<int>::iterator posBegin = v2.begin();
	v2.Insert(posBegin, 0);
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "Size: "<< v2.Size() << " | Capacity: " << v2.Capacity() << std::endl;

	std::cout<<std::endl;

	std::cout<<"Insert(posEnd - 2, 40,4)"<<std::endl;
	Vector<int>::iterator posEnd = v2.end();
	v2.Insert(posEnd - 2, 40,4);
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "Size: "<< v2.Size() << " | Capacity: " << v2.Capacity() << std::endl;

	std::cout<<std::endl;

	std::cout<<"Resize(17,-2)"<<std::endl;
	v2.Resize(17,-2);
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "Size: "<< v2.Size() << " | Capacity: " << v2.Capacity() << std::endl;
	
	std::cout<<std::endl;

	std::cout<<"Resize(8,-2)"<<std::endl;
	v2.Resize(8,-2);
	std::cout << "v2: " << v2 << std::endl;
	std::cout << "Size: "<< v2.Size() << " | Capacity: " << v2.Capacity() << std::endl;

	std::cout<<std::endl;

	std::cout<<"Vector<int> v3(v2)"<<std::endl;
	Vector<int> v3(v2);
	std::cout << "v3: " << v3 << std::endl;
	std::cout << "Size: "<< v3.Size() << " | Capacity: " << v3.Capacity() << std::endl;

	std::cout<<std::endl;

	v3.PopBack();
	v3.PopBack();
	std::cout << "v3: " << v3 << std::endl;
	std::cout << "Size: "<< v3.Size() << " | Capacity: " << v3.Capacity() << std::endl;
	
	std::cout<<std::endl;
	
	v3.Swap(v2);
	v3.Erase(v3.begin() + 2, 3);
	std::cout << "v3: " << v3 << std::endl;
	std::cout << "Size: "<< v3.Size() << " | Capacity: " << v3.Capacity() << std::endl;

	std::cout<<std::endl;

	v3[2] = 3;
	v3[3] = 4;
	std::cout << "v3: " << v3 << std::endl;
	std::cout << "Size: "<< v3.Size() << " | Capacity: " << v3.Capacity() << std::endl;

	v3.clear();
	std::cout<<v3.Empty()<<std::endl;

}