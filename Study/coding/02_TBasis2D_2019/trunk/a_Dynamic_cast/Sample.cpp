#include <stdio.h>
#include <iostream>
// dynamic_cast는 vtable를 사용하기 때문에
// 반드시 virtual 함수가 있어야 작동한다.
struct A {
	virtual void test() {
		printf_s("in A\n");
	}
};

struct B : A {
	virtual void test() {
		printf_s("in B\n");
	}

	void test2() {
		printf_s("test2 in B\n");
	}
};

struct C : B {
	virtual void test() {
		printf_s("in C\n");
	}

	void test2() {
		printf_s("test2 in C\n");
	}
};

void Globaltest(A& a) {
	try {
		C &c = dynamic_cast<C&>(a);		
		printf_s("in GlobalTest\n");
	}
	catch (std::bad_cast) {
		printf_s("Can't cast to C\n");
	}
}

int main() {
	A *pa = new C;
	A *pa2 = new B;

	pa->test();

	B * pb = dynamic_cast<B *>(pa);
	if (pb)
		pb->test2();

	C * pc = dynamic_cast<C *>(pa2);
	if (pc)
		pc->test2();

	C ConStack;
	Globaltest(ConStack);

	// B가 C를 알고 있지 않기 때문에 실패한다.
	B BonStack;
	Globaltest(BonStack);
}