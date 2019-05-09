#include "stack.h"

int isEmpty(STACK *s)
{
	return (s->stackTop == EmptyTOS);
}

int isFull(STACK *s)
{
	return (s->stackTop == s->capacity - 1);
}

STACK *createStack(int maxElements)//创建一个容量位 maxElements的栈
{
	STACK *s;

	if (maxElements < MinStackSize) {
		printf("Stack size si too small\n");
		return NULL;
	}
	s = (STACK *)malloc(sizeof(struct stackRecord));
	s->array = (elementType *)malloc(sizeof(elementType)* maxElements);
	s->capacity = maxElements;
	makeEmpty(s);

	return s;

}
void disposeStack(STACK *s)
{
	if (s != NULL) {
		free(s->array);
		free(s);
	}
}

void makeEmpty(STACK *s)
{
	if (s != NULL) {
		s->stackTop = EmptyTOS;
	}
}

void push(elementType element, STACK *s)
{
	if (!isFull(s)) {
		s->stackTop++;
		s->array[s->stackTop] = element;
	}
	else {
		printf("full stack\n");
	}
}

elementType top(STACK *s)
{
	if (!isEmpty(s)) {
		return s->array[s->stackTop];
	}
	else {
		printf("empty stack\n");
		return 0;
	}
}

void pop(STACK *s)
{
	if (!isEmpty(s))
		s->stackTop--;
	else
		printf("empty stack\n");
}

elementType topAndTop(STACK *s)
{
	if (!isEmpty(s)) {
		return s->array[s->stackTop--];
	}
	else {
		printf("empty stack\n");
		return 0;
	}
}
int main(int argc, char *argv[])
{
	int StackTop = 0;
	int StackTopTest;
	STACK* TestStack = createStack(10);//在堆区创建一个栈TestStack，返回栈的地址
	push(55, TestStack);
	push(66, TestStack);
	push(68, TestStack);
	StackTop = topAndTop(TestStack);
	printf("Stack_Top = %d \n", StackTop);
	StackTop = topAndTop(TestStack);
	printf("Stack_Top = %d \n", StackTop);
	StackTopTest = topAndTop(TestStack);
	printf("Stack_Top = %d \n", StackTopTest);
	return EXIT_SUCCESS;
}