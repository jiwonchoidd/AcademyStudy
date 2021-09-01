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
//-- > N �̶�� �ڿ� ���� ������ Max Count �����Ͽ�, �ش� Count��ŭ �ڿ��� �̹� �Ҵ�Ǿ� ������ Unlock �Ǳ⸦ ����Ѵ�. (N = 1 �̸� mutex)
//-- > Lock �� ȹ������ ���ϰ� ����ϴ� ���, ������� sleep �ȴ�.
//-- > ���� : Lock Waiting �� sleep �� ���� �ٸ� ������� ������� �ѱ� �� �ִ�.
//-- > ���� : sleep �� �̿��ؼ� Waiting �� �ϱ� ������, sleep �Ǹ� �ȵǴ� �ڵ忡���� Ȱ���� �� ����.
//	: ����ȭ ������ ���� ���, Lock waiting ���ʿ��� ������� �� �� �ִ�.
//
//
//	- spinlock
//
//	-- > semaphore �� ���� �غ��� ����, Lock ȹ�� ������ �� Busy Waiting �� �Ѵ�. (sleep ���� �ʰ� ��� Lock ��� ���� �õ�)
//	-- > ���� : ����ȭ ������ �۴ٸ�, Lock Waiting �� ������ ���� ����Ǳ� ������, Lock ���� ���� ������尡 ����.
//	-- > ���� : ����ȭ ������ ū ���, �� Lock Waiting �ɸ��� �ð��� �� ���, �ٸ� ������� ������� �ѱ��� �ʰ� ��� Lock ��� �� �õ��ϹǷ� ������� ũ��.write �� �߻����� �ʰ� ���� �����忡�� read �� �ϴ� ��쿡��, , reader �����峢�� Lock Waiting �� �ؾ� �ϹǷ�, ���ʿ��� ������� �߻��� �� �ִ�.
//
//
//	- rwlock
//
//	-- > spinlock �� �����ϰ� ����.spinlock �� ������ �غ��ϱ� ���� ����.
//	-- > read / write Lock, Unlock �� �����Ͽ� �����Ѵ�.
//	-- > Write Lock �� �ɷ����� ���� ���¶��, , read Lock �� �ߺ��Ͽ� ���� �� �ִ�. (��, ���� �ڿ��� ���� read �۾��� ���ÿ� ���� ���� ����)
//	-- > read Lock �� �ɷ��ִ� ���¶��, , write Lock �� read Unlock �� ������ ����Ѵ�. (reader �� �߸��� ���� read �ϴ� ���� �����ϱ� ����)
//	-- > ���� : reader �����尡 ���ų� read ����ȭ ������ ū ���(writer �� write ����ȭ ��������), spinlock �� ���� ������ ����.
//	-- > ���� : reader �� �ʹ� ���Ƽ� read Lock �� ������ ��� �����ٸ�, , write Lock �� ������ �ʰ� �Ǵ� ��� ���� �߻��� �� �ִ�.
//
//
//	- seqlock
//
//	-- > rwlock �� �����ϰ� ����.rwlock �� ������ �غ��ϱ� ���� ����. (rwlock �� ������ �غ��ϸ鼭 ����� ���ο� ������ ����)
//	-- > reader �� �۾� �߿���, writer �� write �۾� �����ϴ�.
//	-- > reader �� seq ��ȣ�� ����->���ϴ� ���� �о �۾���.->�۾� ������ seq ��ȣ�� ����Ǿ����� Ȯ��(writer �� ���� ����Ǿ�����)->����Ǿ��ٸ�, �ٽ� ó�� �������� ���ư��� �ݺ�.
//	��, reader �� writer �� ���� race �߻����� ���� ������ �ݺ������� read �� �õ� �ϴ� ���̴�.
//	-- > write ���� read �� �� ���� �۾��� �����ϴ�.
//	-- > dcache(dentry cache) path lookup �� �� ��������ȭ�� ����, seqlock ���ȴ�.
//	-- > ���� : write Lock �� ���� ��� ���� �߻� ����.lock ���� ��� ����.
//	-- > ���� : read �۾��� ������ ������ �� �ִ�.
//
//
//
//	-RCU(Read - Copy - Update)
//
//	-- > �� ������ �ٸ��� Lock ����ü�� Counter �� �ʿ���� ���.
//	-- > ���� �����Ҵ�ǰ�, �����Ϳ� ���� �����Ǵ� �ڷ� �������� RCU �� ��ȣ ���� �� �ְ�, ����ȭ �������� sleep �ϸ� �ȵȴ�.
//	-- > read Lock, Unlock ���� �߿��� �۾��� ����.�׳� �����Ϳ��� �����͸� �д´�.
//	-- > write Lock �� �ϸ�, , �����ڿ�(�����Ҵ�� �޸𸮿� ����) �� ���ο� �޸�(���� �����Ҵ�)�� �����Ѵ�.���ο� �޸𸮿� write �۾��� �Ѵ�.write �� ������, , ���� �����ڿ� �����Ͱ� ���ο� �޸𸮸� ����Ű�� �Ѵ�. (�� ������ �����۾��� ���ڿ����̶� ����)
//	-- > ���� : Lock ����ü ��� ����.
//	-- > ���� : ���� �����ڿ� �����ʹ�  ��� reader ���� read �� ������ �������� ������ �� ����.