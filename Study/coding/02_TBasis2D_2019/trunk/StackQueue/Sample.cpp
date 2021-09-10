#include <iostream>
#include <stack>
#include <queue>
using namespace std;

int main() 
{
	// 스택 생성
	stack<int> s;
	// push
	s.push(3);
	s.push(2);
	s.push(1);
	// top
	cout << "top element : " << s.top() << '\n';
	// pop
	s.pop(); // 1이 삭제
	s.pop(); // 2가 삭제
	// size
	cout << "stack size : " << s.size() << '\n';
	// empty
	cout << "Is it empty? : " << (s.empty() ? "Yes" : "No") << '\n';


	// 큐 생성
	queue<int> q;
	// push
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);
	q.push(6);

	q.push(7);
	q.push(8);
	q.push(9);

	cout << "Queue Size : " << q.size() << endl;

	cout << "Queue Items" << endl;

	while (!q.empty())
	{
		cout << q.front() << endl;
		q.pop();
	}

	// pop
	q.pop();
	q.pop();
	q.pop();
	// front
	cout << "front element : " << q.front() << '\n';
	// back
	cout << "back element : " << q.back() << '\n';
	// size
	cout << "queue size : " << q.size() << '\n';
	// empty
	cout << "Is it empty? : " << (q.empty() ? "Yes" : "No") << '\n';


	cout << "size of queue: " << Q.size() << endl;
	cout << "Is queue empty?: " << Q.empty() << endl << endl;

	q.push(1);
	q.push(2);
	q.push(3);

	cout << "size of queue: " << q.size() << endl;
	cout << "Is queue empty?: " << q.empty() << endl << endl;

	cout << "Front of queue: " << q.front() << endl;
	cout << "Back of queue: " << q.back() << endl;
	cout << "size of queue: " << q.size() << endl << endl;

	q.pop();

	cout << "size of queue after popped: " << q.size() << endl;
	cout << "Front of queue: " << q.front() << endl;
	return 0;
}
