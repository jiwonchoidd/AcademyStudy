#include "TSpinLock.h"
#include "TThreadEx.h"

void SpinLock::enterWriteLock()
{
	thisThread->getLockOrderChecker()->push(this);

	while (true)
	{
		while (mLockFlag & WRITE_MASK)
			YieldProcessor();

		if (((mLockFlag += WRITE_FLAG) & WRITE_MASK) == WRITE_FLAG)
		{
			while (mLockFlag & READ_MASK)
				YieldProcessor();

			return;
		}

		mLockFlag -= WRITE_FLAG;
	}
}

void SpinLock::leaveWriteLock()
{
	mLockFlag -= WRITE_FLAG;

	thisThread->getLockOrderChecker()->pop(this);
}

void SpinLock::enterReadLock()
{
	thisThread->getLockOrderChecker()->push(this);

	while (true)
	{
		while (mLockFlag & WRITE_MASK)
			YieldProcessor();

		if ((++mLockFlag & WRITE_MASK) == 0)
		{
			return;
		}

		--mLockFlag;
	}
}

void SpinLock::leaveReadLock()
{
	--mLockFlag;

	thisThread->getLockOrderChecker()->pop(this);
}
//-semaphore
//
//-- > N 이라는 자원 공유 가능한 Max Count 설정하여, 해당 Count만큼 자원이 이미 할당되어 있으면 Unlock 되기를 대기한다. (N = 1 이면 mutex)
//-- > Lock 을 획득하지 못하고 대기하는 경우, 쓰레드는 sleep 된다.
//-- > 장점 : Lock Waiting 시 sleep 을 통해 다른 쓰레드로 제어권을 넘길 수 있다.
//-- > 단점 : sleep 을 이용해서 Waiting 을 하기 때문에, sleep 되면 안되는 코드에서는 활용할 수 없다.
//	: 동기화 영역이 작은 경우, Lock waiting 불필요한 오버헤드 줄 수 있다.
//
//
//	- spinlock
//
//	-- > semaphore 의 단점 극복을 위해, Lock 획득 못했을 때 Busy Waiting 을 한다. (sleep 하지 않고 계속 Lock 얻는 것을 시도)
//	-- > 장점 : 동기화 영역이 작다면, Lock Waiting 이 굉장히 빨리 종료되기 때문에, Lock 으로 인한 오버헤드가 적다.
//	-- > 단점 : 동기화 영역이 큰 경우, 즉 Lock Waiting 걸리는 시간이 긴 경우, 다른 쓰레드로 제어권을 넘기지 않고 계속 Lock 얻는 것 시도하므로 오버헤드 크다.write 가 발생하지 않고 여러 쓰레드에서 read 만 하는 경우에도, , reader 쓰레드끼리 Lock Waiting 을 해야 하므로, 불필요한 오버헤드 발생할 수 있다.
//
//
//	- rwlock
//
//	-- > spinlock 과 유사하게 동작.spinlock 의 단점을 극복하기 위해 나옴.
//	-- > read / write Lock, Unlock 을 구분하여 동작한다.
//	-- > Write Lock 이 걸려있지 않은 상태라면, , read Lock 은 중복하여 얻을 수 있다. (즉, 공유 자원에 대해 read 작업은 동시에 여러 개가 가능)
//	-- > read Lock 이 걸려있는 상태라면, , write Lock 은 read Unlock 될 때까지 대기한다. (reader 가 잘못된 값을 read 하는 것을 방지하기 위해)
//	-- > 장점 : reader 쓰레드가 많거나 read 동기화 영역이 큰 경우(writer 나 write 동기화 영역보다), spinlock 에 비해 성능이 좋다.
//	-- > 단점 : reader 가 너무 많아서 read Lock 이 빠르게 계속 잡힌다면, , write Lock 이 잡히지 않게 되는 기아 현상 발생할 수 있다.
//
//
//	- seqlock
//
//	-- > rwlock 과 유사하게 동작.rwlock 의 단점을 극복하기 위해 나옴. (rwlock 의 단점을 극복하면서 생기는 새로운 단점이 있음)
//	-- > reader 가 작업 중에도, writer 는 write 작업 가능하다.
//	-- > reader 는 seq 번호를 저장->원하는 값을 읽어서 작업함.->작업 끝나면 seq 번호가 변경되었는지 확인(writer 에 의해 변경되었는지)->변경되었다면, 다시 처음 과정으로 돌아가서 반복.
//	즉, reader 는 writer 에 의해 race 발생하지 않을 때까지 반복적으로 read 를 시도 하는 것이다.
//	-- > write 보다 read 가 더 많은 작업에 유용하다.
//	-- > dcache(dentry cache) path lookup 할 때 성능최적화를 위해, seqlock 사용된다.
//	-- > 장점 : write Lock 에 대한 기아 현상 발생 방지.lock 없이 사용 가능.
//	-- > 단점 : read 작업이 굉장히 느려질 수 있다.
//
//
//
//	-RCU(Read - Copy - Update)
//
//	-- > 위 방법들과 다르게 Lock 구조체나 Counter 가 필요없는 방법.
//	-- > 오직 동적할당되고, 포인터에 의해 참조되는 자료 구조만이 RCU 로 보호 받을 수 있고, 동기화 영역에서 sleep 하면 안된다.
//	-- > read Lock, Unlock 때는 중요한 작업이 없다.그냥 포인터에서 데이터를 읽는다.
//	-- > write Lock 을 하면, , 공유자원(동적할당된 메모리에 있음) 을 새로운 메모리(역시 동적할당)에 복사한다.새로운 메모리에 write 작업을 한다.write 가 끝나면, , 원래 공유자원 포인터가 새로운 메모리를 가리키게 한다. (이 포인터 복사작업이 원자연산이라서 가능)
//	-- > 장점 : Lock 구조체 사용 안함.
//	-- > 단점 : 이전 공유자원 데이터는  모든 reader 들이 read 를 끝내기 전까지는 해제될 수 없다.