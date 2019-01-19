/*
	�b Multi_Thread �M�ש��U�ڭ̾Ǩ�F�@�ǰ�¦���Ϊk

		1. �z�L�إߤ@�ӷs�� std::thread ����B���ͤ@�ӷs�������

		2. �b���n�ɩI�s std::thread ���� join() �禡�A�T�O�Ӱ���Ǥw����

	���L�A���²�檺�g�k�A�N�O���h�Ӱ���ǡB�U�ۥh���橼���������������u�@�A�b�o�ر��p�U�A���q�`���|������j���D
	���O�A�p�G���P������Ǥ����A���h�ק��@�Ϊ��ܼƪ��ܡA�N�i��|���]�������D���Ӱ�����|�������B�Ӧ����D�o�ͤF

	�H�U�����Ҥl�ӻ��A���`�� call OutputValue() ���|��������D�A���O�p�G�H������өI�s���ܷ|�o�{���G�n���ǩǪ��A�ҥH�ڭ̦���حק�覡

	A. Mutex �ѨM : OutputValue_Mutex()

		Mutex ��k�N�O�z�L�@�ӥ����ܼ� gMutex �ӹ��ƶi�汱��A�D�n�N�O�z�L lock() �M unlock() �o��Ө禡�A�ӳ]�w�ܼƪ����A�O�_�Q��w�C

			lock() : �ˬd gMutex �O�_�w�g�Q��w�A�p�G�S���Q����ܡA�L�N�|�� gMutex �]�w����w�B�M���~�����F�p�G�w�g�Q����ܡA�L�h�|���b�o��B������w�Q�Ѱ��B�A�� gMutex ���B�~�����C
			unlock() : �������������|�i�����
		
		���O!!!!!!!�o�˰��ä��n�A��]�b��p�G�A����Ƥ��p�ߦ]�� return ���}���ܡA�N�|�ɭP�û����|�I�s�� unlock(),���A�N�i�J�L�����ݡA�ҥH�@��|�z�L��k B �ӸѨM
	
	B. lock_guard �ѨM :  OutputValue_Lock_Guard()

		lock_guad<mutex> ������ mLock�B�Ӻ޲z�����ܼ� gMutex
		�� mLock �Q�إߪ��P�ɡAgMutex �N�|�Q�۰���w�A�ӷ� mLock �]���ͩR�g�������Ӯ����ɡAgMutex �]�|�]���Q�۰ʸ���
		�۸���e����ʨϥ� lock() �M unlock()�A�ϥ� lock_guard ��O�@�Ӥ����K�B�]����w������k�C
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

void OutputValue(int n)
{
	cout << "Number:";
	for (int i = 0; i < n; ++i)
	{
		this_thread::sleep_for(chrono::duration<int, std::milli>(5));
		cout << " " << i;
	}
	cout << endl;
}

mutex gMutex;
void OutputValue_Mutex(int n)
{
	gMutex.lock();
	cout << "Number:";
	for (int i = 0; i < n; ++i)
	{
		this_thread::sleep_for(chrono::duration<int, std::milli>(5));
		cout << " " << i;
	}
	cout << endl;
	gMutex.unlock();
}

void OutputValue_Lock_Guard(int n)
{
	lock_guard<mutex> mLock(gMutex);
	cout << "Number:";
	for (int i = 0; i < n; ++i)
	{
		this_thread::sleep_for(chrono::duration<int, std::milli>(5));
		cout << " " << i;
	}
	cout << endl;
}

int main(int argc, char** argv)
{
	cout << "Normal function call" << endl;
	OutputValue(3);
	OutputValue(4);

	cout << "\nCall function with thread" << endl;
	thread mThread1(OutputValue, 3);
	thread mThread2(OutputValue, 4);
	mThread1.join();
	mThread2.join();
	cout << endl;

	cout << "\nCall function with Mutex thread" << endl;
	thread mThread3(OutputValue_Mutex, 3);
	thread mThread4(OutputValue_Mutex, 4);
	mThread3.join();
	mThread4.join();
	cout << endl;

	cout << "\nCall function with Lock Guard thread" << endl;
	thread mThread5(OutputValue_Mutex, 3);
	thread mThread6(OutputValue_Mutex, 4);
	mThread5.join();
	mThread6.join();
	cout << endl;

	system("pause");
	return 0;
}