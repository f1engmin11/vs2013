#include <iostream>
using namespace std;
//void add(void)
//{
//	cout << "add" << endl << endl;
//}
//void printer(void(*fuc)(void))
//{
//	fuc();
//	cout << "printer_k   "<< endl;
//}
//int main(void)
//{
//	printer(add);
//	return 0;
//}

#include <stdio.h>
#define DEFINE
typedef void(*pfn_task)(void);

void printWelcome(int len)
{
	printf("��ӭ��ӭ -- %d\n", len);
}

void printGoodbye(int len)
{
	printf("�Ϳ��Ϳ� -- %d\n", len);
}

void callback(int times, void(*print)(int))
{
	int i;
	for (i = 0; i < times; ++i)
	{
		print(i);
	}
	printf("/n�Ҳ�֪������ӭ�ͻ����Ϳ�!\n\n");
}
void test(void)
{
	static int kk = 111;
//	kk++;
	cout << "testkk = " << kk << endl;
}
static int kk = 222;
pfn_task FUNC = test;;

void main(void)
{
#if defined(DEFINE)
//	callback(10, printWelcome);
#endif

#if 0
	callback(10, printGoodbye);
	printWelcome(5);
	printf("\n=============================\n");
	test();
	test();
	test();
	cout <<"kk = " << kk<< endl;
#endif
	/* ���ַ�ʽ��ͨ������ָ����ú�����Ч��һ�� */
	(*FUNC)();
	FUNC();
}