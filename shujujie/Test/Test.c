#include <stdio.h>
/*
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;


void insertion_sort(unsigned short a[], unsigned short n)//数组插入排序法
{
	unsigned short i, v;
//	//signed short j;
	unsigned short j;
	for (i = 1; i<n; i++)
	{
		//如果第i个元素小于第j个，则第j个向后移动
		for (v = a[i], j = i - 1; j >= 0 && v<a[j]; j--)
		{
			a[j + 1] = a[j];
		}
		a[j + 1] = v;
	}
}

static void sort_data(uint16_t arr[], uint8_t n)
{
	uint16_t tmp;
	uint8_t i, j, last_swap;
	for (j = n - 1; j>0; j = last_swap)
	{
		last_swap = 0;
		for (i = 0; i<j; ++i)
		{
			if (arr[i] > arr[i + 1])
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				// 最后一次交换位置的坐标
				last_swap = i;
			}
		}
	}
}
signed short aa[10] = { 9356,5, 3488, 355, 3416, 3498, 3422, 3488, 3496, 9425 };
unsigned char arr[5] = { 0x11, 0x22, 0x33, 0x44, 0x55 };

void printf_arr(unsigned short *p_element)
{
	printf("%x \r\n ", *p_element);
	printf("%x \r\n ", p_element[1]);
}

int main(void)
{
	int i;
	unsigned char arr_val_u8;
	unsigned short arr_val_u16;

	unsigned char *p_arr_val_u8;
	unsigned int *p_arr_val_u16;

	unsigned short www = 0;
	unsigned short wwww = 0;
	sort_data(aa, 10);
	for (i = 0; i < 10; ++i)
	{
		printf("uchar-c = %d \r\n ", aa[i]);
	}

	wwww--;
	wwww--;
	printf("wwww = %d \r\n ", wwww);
	
	
	unsigned short test = 1;

	if ((test++) && (test==2))
		printf("okokok \r\n ");

	printf("\r\n ");
	printf("\r\n ");
	printf("\r\n ");



	p_arr_val_u8 = arr;
	
	printf("%x \r\n ", *p_arr_val_u8);
	printf("%x \r\n ", (unsigned int)&arr[0]);
	printf("%x \r\n ", p_arr_val_u8++);
	printf("%x \r\n ", *p_arr_val_u8);
	printf("%d \r\n ", p_arr_val_u8++);
	printf("%x \r\n ", *p_arr_val_u8);
	printf("%d \r\n ", p_arr_val_u8++);
	printf("%x \r\n ", *p_arr_val_u8);
	printf("%d \r\n ", p_arr_val_u8++);
	printf("%x \r\n ", *p_arr_val_u8);
	printf("%d \r\n ", p_arr_val_u8++);



	printf("\r\n ");
	printf("\r\n ");
	printf("\r\n ");

	p_arr_val_u16 = (unsigned int*)arr;
	printf("%x \r\n ", *p_arr_val_u16);
	printf("%d \r\n ", p_arr_val_u16++);
	printf("%x \r\n ", *p_arr_val_u16);
	printf("%d \r\n ", p_arr_val_u16++);
	printf("\r\n ");
	printf("\r\n ");
	printf("\r\n ");

	printf_arr((unsigned int*)arr);

	printf("%d \r\n ", sizeof(p_arr_val_u8)); 
	printf("%d \r\n ", sizeof(p_arr_val_u16));

	return 0;
}
*/

/*
#include<stdio.h>
#include<stdlib.h>
//全局变量数组
short gStr1[15];  //每个数据占据2字节
int   gStr2[15];  //每个数据占据4字节

//将字符串存到全局变量数组中
void ReadStr1(short *buf, int size)
{
	int i;
	for (i = 0; i<size; i++)
		gStr1[i] = buf[i];

	printf("gStr1=%s\n", gStr1);         //打印数组
	printf("gStr1[0]=%c\n", gStr1[0]);   //数组中第一个数据
	printf("gStr1[1]=%c\n", gStr1[1]);   //数组中第二个数据 
	printf("gStr1=%s\n", gStr1 + 1);       //打印数组首地址+1后的数据
}
void ReadStr2(int *buf, int size)
{
	int i;
	for (i = 0; i<size; i++)
		gStr2[i] = buf[i];

	printf("gStr2=%s\n", gStr2);
	printf("gStr2[0]=%c\n", gStr2[0]);
	printf("gStr2[1]=%c\n", gStr2[1]);
	printf("gStr2=%s\n", gStr2 + 1);
}

int main()
{
	char s1[] = { "ABCDEFHIJK" };
	char s2[] = { "abcdefhijk" };
	printf("gStr1数组的情况分析：\n");
	ReadStr1((short*)s1, 5);
	printf("\n");
	printf("gStr2数组的情况分析：\n");
	ReadStr2((int*)s2, 3);

	system("pause");//在vs下测试，防止终端显示闪退
	return 0;
}
*/

int main(void)
{
	int a[4] = { 1, 2, 3, 4 };

	int *ptr1 = (a);
	int ptr2 = (&a);
	int c = ((&a) + 1);
	printf("%x \n\r%d \n\r%d \n\r", *ptr1, ptr2, c);
	return 0;
}

