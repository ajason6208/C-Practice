/*
	�b Multi_Thread_Communication_1 ���ڭ̾ǲߨ�F�򥻪� mutex & lock_guard ���ΡA���U�ӭn���Ч�h�i�����Ϊk

	A. �䥦������ mutex : timed_mutex(�䴩�w��)�Brecursive_mutex(�䴩���j)�Brecursive_timed_mutex

		1. timed_mutex : ���F�򥻴� mutex �� lock() �M try_lock() �~�A���B�~���ѤF try_lock_for() �M try_lock_until() �o��Ө禡�A�i�H�]�w�b���w���ɶ����B�յۥh�i�� mutex ����w���ʧ@�C

		2. recursive_mutex : �� mutex �{�o���ۤv������ǡA�åB�� mutex �b�w�g�Q��w�����p�U�A�٬O�i�H���P�@�Ӱ���ǦA�h��w�L�B�Ӥ��|�Q�פU��

			�b ClassA �̡A�� func1() �M func2() ��Ө禡�A��̳��|�h�إߤ@�� lock_guard ������B����w mMutex �o�� mutex�C
			���L�A�ѩ� func2() �̡B�|�h�I�s func1()�A�ҥH�p�G�h�I�s func2() ���ܡA��ڤW�i�H�o�{�A�{���|�b���� func2() ���ɭԡA�z�L lock_guard �h��w mMutex
			�ӷ�b func2() ���h�I�s func1() ���ɭԡA�P�˪��n�D��w�ʧ@�A�S�|�A�i��@���I

			�o�ɭԦp�G�O�ϥμзǪ� mutex ���ܡA�b�ĤG���չϥh��w mMutex ���ɭԡ]func1()�^�A�|�]�� mMutex �w�g�b func2() �̳Q��w�F�A�ӴN�]�����b�o��A���� mMutex �Q�Ѱ���w�A�~��
			���O�ѩ� mMutex ����w���A�����n���� func2() ��Ӱ��槹����~�|�Ѱ��A�ҥH�o��N�|�ܦ��û������������p�A���{���L�k�~�����C

		3. recursive_timed_mutex : �U�W��q�A�N�O�P�ɨ㦳 timed_mutex�Brecursive_mutex ��دS��ʽ誺 mutex ���O
	
	B. �� lock_guard ��u�ʪ� :  unique_lock�A���H�U��j�S��

		1. unique_lock ���@�w�n�֦� mutex�A�ҥH�i�H�z�L default constructor �إߥX�@�ӪŪ� unique_lock�C

		2. unique_lock ���M�@�ˤ��i�ƻs�]non-copyable�^�A���O���O�i�H�ಾ���]movable�^�C�ҥH�Aunique_lock �����i�H�Q�禡�^�ǡA�]�i�H��� STL �� container �̡C

		EX : ���M�@�몬�p�U�A�j�h�O�@���h��w�@�� mutex �ӨϥΪ��C���O�b��ڨϥΪ��ɭԡA�����ɭ��٬O�|�ݭn�P�ɥh��w�h�� mutex�A���קK��Ƥ��P�B�����D (�p�U)

		* �o�̧ڭ̹�� transfer �i�汴�Q�A������n����b��w(defer_lock)�A�Ӥ��O�����g���U���{���O? �]���p�G�ΤW�����g�k�A�����̧ǦU����w��� mutex ���ܡA���i��|�b�h����Ǫ����p�U�A���� dead lock �����p�C

			lock_guard<mutex> lock1( from.mMutex );
			lock_guard<mutex> lock2( to.mMutex );

			Ex : �p�G�P�ɭn�D�i��u�� A ��b�� B�v�]thread 1�^�M�u�� B ��b�� A�v�]thread 2�^���ʧ@���ܡA���i��|���ͤ@�Ӫ��p�A�N�O�b thread 1 �̭��w�g��w�F A �� mutex�A���O�b�չ���w B �� mutex ���ɭԡAthread 2 �w�g��w�F B �� mutex�F�ӦP�˦a�A�o�ɭ� thread 2 �]�ݭn�h��w A �� mutex�A���O�L�o�w�g�Q thread 1 ��w�F�C

			�p���@�ӡA�N�|�ܦ� thread 1 ��� A �� mutex �b�� thread 2 �� B �� mutex ����A�� thread 2 ��� B �� mutex �b�� thread 1 �� A �� mutex ���ꪺ���p�K�o�˪����p�A�򥻤W�O�L�Ѫ��C

*/

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

using namespace std;

class ClassA
{
public:
	void func1()
	{
		lock_guard<recursive_mutex> lock(mMutex);
	}

	void func2()
	{
		lock_guard<recursive_mutex> lock(mMutex);
		func1();
	}

private:
	recursive_mutex mMutex;
};

struct Box {
	explicit Box(int num) : num_things{ num } {} // explicit : http://hatsukiakio.blogspot.com/2009/05/c-explicit.html

	int num_things;
	std::mutex m;
};

struct bank_account
{
	explicit bank_account(string name, int money)
	{
		sName = name;
		iMoney = money;
	}

	string sName;
	int iMoney;
	mutex mMutex;
};

// ��b�ɡA������w��ӱb���A�H�K��L�H�]�b��o��ӱb���i��޿�
void transfer(bank_account &from, bank_account &to, int amount)
{
	// don't actually take the locks yet
	unique_lock<mutex> lock1(from.mMutex, defer_lock); // defer_lock : ���M�n���L�h�޲z���w�� mutex ����A���O���n�ߨ�h��w�L�B�ӬO�����S����w�����A
	unique_lock<mutex> lock2(to.mMutex, defer_lock);

	// lock both unique_locks without deadlock
	lock(lock1, lock2); // �ӱ��U�ӡA�h�O�A�z�L lock() �o�Ө禡�A�ӡu�P�ɡv��w lock1 �M lock2 �o��� unique_lock ����C

	from.iMoney -= amount;
	to.iMoney += amount;

	// output log
	cout << "Transfer " << amount << " from "
		<< from.sName << " to " << to.sName << endl;
}

int main()
{
	bank_account Account1("User1", 100);
	bank_account Account2("User2", 50);

	thread t1([&]() { transfer(Account1, Account2, 10); });
	thread t2([&]() { transfer(Account2, Account1, 5); });

	t1.join();
	t2.join();

	cout << "Account1.iMoney: " << Account1.iMoney << endl;
	cout << "Account2.iMoney: " << Account2.iMoney << endl;

	system("pause");
}