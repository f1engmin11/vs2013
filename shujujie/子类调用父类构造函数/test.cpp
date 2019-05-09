#include <iostream>
using namespace std;
namespace AA {
	class A
	{
	public:
		A(int i, int j)
		{
			cout << "namespace AA::A construct start" << endl;
			cout << "I = " << i << endl;
			cout << "J = " << j << endl;
			cout << "namespace AA::A construct end" << endl << endl;
		}
	};

	class B :public A
	{
	public:
		B() :A(123, 456)
		{
			cout << "namespace AA::B construct" << endl;
		}
	};
}//namespace AA
namespace BB {
	class A
	{
	public:
		A(int i, int j)
		{
			cout << "namespace BB::A construct start" << endl;
			cout << "I = " << i << endl;
			cout << "J = " << j << endl;
			cout << "namespace BB::A construct end" << endl << endl;
		}
	};

	class B :public A
	{
	public:
		B() :A(7, 89)
		{
			cout << "namespace BB::B construct" << endl;
		}
	};

}//namespace BB
int main(void)
{
	int tm;
	AA::B b;
	cout << endl <<"================================================"<< endl;
	BB::B c;

	cout << endl << "================================================" << endl;
	tm = (1<<2)|(1<<3);
	cout <<"tm = "<< tm << endl;
	return 0;
}