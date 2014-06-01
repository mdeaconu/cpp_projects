#include <iostream>

class MSingleton 
{
public:
	static MSingleton& instance();
	~MSingleton();

	inline void print() const {
		std::cout << "MSingleton::print()" << std::endl;
	}
	
private:
	// methods
	MSingleton() { std::cout << "MSingleton()" << std::endl; };
	MSingleton(const MSingleton&);
	MSingleton& operator= (const MSingleton&);
};

#define M_MSingleton MSingleton::instance()

MSingleton& MSingleton::instance() 
{
	static MSingleton& sr_instance = MSingleton();

	return sr_instance;
}


MSingleton::~MSingleton() 
{
	std::cout << "~MSingleton()" << std::endl;
}

int main()
{
	M_MSingleton.print();	
	M_MSingleton.print();

	return 0;
}