#include <stdio.h>
#define MAX 10
int queue[MAX];
int front, rear;

//int rear = 0;
//int front = 0;
//int iCounter = 0;
//int put(int t) {
//	if (iCounter <= 0) front = rear = 0;
//	if (iCounter >= MAX)
//	{
//		printf("\n Queue Overflow");		return -1;
//	}
//	QueueArray[rear] = t;
//	if (iCounter + 1 < MAX)
//	{
//		rear = ++rear % MAX;
//	}
//	iCounter++;
//	return t;
//}
//
//int get() {
//	if (iCounter <= 0) { printf("\n Queue Underflow");		return -1; }
//	int iRet = QueueArray[front];
//	QueueArray[front] = -1;
//	front = ++front % MAX;
//	iCounter--;
//	return iRet;
//}

void InitQueue()
{
	front =  rear = 0;
}
void ClearQueue()
{
	front = rear;
}
int put(int t)
{
	if ((rear+1) % MAX == front ) // 큐가 가득 찼는가 ?
	{
		printf("\n Queue Overflow.");
		return -1;
	}
	queue[rear] = t;
	rear = ++rear % MAX;
	return t;
}

int get()
{
	if (front == rear) // 큐가 비어 있는가 ?
	{
		printf("\n Queue Underflow.");
		return -1;
	}
	int i = queue[front];
	front = ++front % MAX;
	return i;
}

void PrintQueue()
{
	printf("\n  Queue Contents : Top ---> Bottom\n");
	for (int i = front; i != rear; i = ++i % MAX)
	{
		printf("%-6d", queue[i]);
	}
}

void main()
{
	InitQueue();
	printf("\nPush 5,4,7,8,2,1");
	put(5);
	put(4);
	put(7);
	put(8);
	put(2);
	put(1);

	PrintQueue();

	printf("\nGet");
	int i = get();
	PrintQueue();

	printf("\n Put 3,2,5,7");
	put(3);
	put(2);
	put(5);
	put(7);
	PrintQueue();

	printf("\nNow queue is full, push 3");
	put(3);
	PrintQueue();

	printf("\n Initialize queue");
	ClearQueue();
	PrintQueue();

	printf("\nNow queue is empty, pop");
	i = get();
	printf("\n poppint value is %d", i);


}