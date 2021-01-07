#include <stdio.h>
#include <stddef.h>
#if 1
#define GET_STRUCT_MEMBER_OFFSET(type, member) (unsigned int)(&((type *)0)->member)    //返回结构体成员member在结构体类型type中偏移的字节数

#define FIFO_LEN  10

#if (FIFO_LEN>=255)
#error the fifo buf is too big .
#endif

#pragma pack(4)
/*__align(4)*/ typedef struct EERLOGO
{
	unsigned short last_err_len[3];
	unsigned int last_end_top;
	unsigned char last_end_len;
	unsigned int IniFlag;
	unsigned short crc16;
}EERLOGO;

/*__align(4)*/ typedef struct  SYS_PARA
{
	unsigned char sys_mode;       
	unsigned int IniFlag;   
	unsigned short crc16;
}SYS_PARA;
#pragma pack


int test = 0;
unsigned char testflag = 0;
int main(void)
{
	EERLOGO eerlogo;
	unsigned short eerlogo_len;

	unsigned char a, b = 0, c = 0;
	a = 200;
	b = 200;
	c = a + b;
	printf("uchar-c = %d \n", c);

	eerlogo_len = offsetof(EERLOGO, last_err_len);
	printf("EERLOGO len（Byte） = %d \n", eerlogo_len);

	eerlogo_len = offsetof(EERLOGO, last_end_top);
	printf("EERLOGO len（Byte） = %d \n", eerlogo_len);


	eerlogo_len = offsetof(EERLOGO, last_end_len);
	printf("EERLOGO len（Byte） = %d \n", eerlogo_len);

	eerlogo_len = offsetof(EERLOGO, IniFlag);
	printf("EERLOGO len（Byte） = %d \n", eerlogo_len);

	eerlogo_len = offsetof(EERLOGO, crc16);
	printf("EERLOGO len（Byte） = %d \n", eerlogo_len);

	printf("\r\nEERLOGO len（Byte） = %d \n", sizeof(eerlogo));

	return 0;
}

#else
#include <iostream>
using namespace std;

#pragma pack(4)
struct example1
{
	short a;// 4
	long b;// 4
 };
//#pragma pack()

//#pragma pack(8)
 struct example2
{
	char c;// 4
	example1 struct1;// 以long为准（4 Byte）
	short e;// 4
};
#pragma pack()

int main(int argc, char* argv[])
{
	example2 struct2;

	cout << sizeof(example1) << endl;

	cout << sizeof(example2) << endl;

	cout << (unsigned int)(&struct2.struct1) - (unsigned int)(&struct2) << endl;

	 return 0;
 }
#endif
