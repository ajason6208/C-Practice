/*
	�b Multi_Thread_Communication_2 ���ڭ̾ǲߨ�F�򥻪� unique_lock ����(�Ȧ���b)�A���U�ӭn����condition_variable
	�L�D�n���γ~�A�O�Ψӧ�ثe�� thread ���U�ӡB���Գq���Ϊ��F�򥻤W�A���Ӻ�O�A�Ω�h��������ҤU���ƥ�B�z�A�]�N�O�ѬY�ǰ���ǵo�X�q���B�i�D��L����ǭn�h�~�����Y�ǰʧ@�C

	�b�ϥ� condition_variable ���ɭԡA�ݭn���z�L unique_lock ����w�@�� mutex�A����A�I�s condition_variable ���Ѫ� wait() �禡�A�ӵ��Գo�� condition_variable �o�X���q���C
	�o�ӮɭԡA�o�Ӱ���ǴN�|��Ӿ�Ӱ��U�ӡB���Գq���F�p�G�n���o�Ӱ�����~��B�@���ܡA�h�O�n�b�O������� �B�z�L�I�s condition_variable ���Ѫ� notify_all() �� notify_one() 
	�o��Ө禡�A�ӳ���]�� wait() �Ӱ��U�Ӫ�����ǡC

	* Conditional Variable �䥦�����ݱ���

		A. wait() : ���� condition_variable gCV �o�X�q����~�i�H�~��Ӱ�����F

			��ڤW�Await() �o�Ө禡�A�O�٦��ĤG�ӰѼƪ���ĤG�ӰѼư򥻤W�����@�ӥi�I�s������A�ӧP�_�O�_�n����ݡF
			�ӳo�ӥi�H�Q�I�s�����󪺻ݭn�^�Ǥ@�� bool �ܼơA�p�G�O true ���ܡAcondition_variable �N�|����ݡB�~�����A�Ӧp�G�^�� false ���ܡA�L�h�|���s�}�l���ݤU�@�ӳq���C

			ex : gCV.wait( mLock, [](){ return gDone; } ); //gDone �O�@�� bool �ܼơA�򥻤W�w�p�|�O�b�~���ק�L���ȡC

				=	while( !gDone )
						gCV.wait( mLock );

		B. ��Ө禡�A�ӭ���ݪ��ɶ��F�]�N�O��ɶ���F����A�N��S������q���A�L�٬O�|������ǰ����
		   wait_for() : �B�~�A���@�ӫ��w���ת��ɶ�
		   wait_until() : �B�~���w�@�Ӯɶ��I

		   ex : gCV.wait_for( mLock, chrono::seconds(5) );
				gCV.wait_until( mLock, chrono::system_clock::now() + chrono::seconds(5) );

	* ���P���q���覡

		A. notify_all() : �|�h�q���Ҧ����b���ݳo�� condition_variable �������

		B. otify_one() : �u�|�q���䤤�@�ӡA���O���ӨS���W�d�q�������ǡC
*/

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector>

using namespace std;

mutex gMutex;
condition_variable gCV;

void funcThread()
{
	cout << "[" << this_thread::get_id() << "] Thread started." << endl;
	unique_lock<mutex> mLock(gMutex);
	gCV.wait(mLock);
	cout << "[" << this_thread::get_id() << "] Thread end." << endl;
}

void funcThread_ex2(size_t idx)
{
	cout << "[" << idx << "] Thread started." << endl;
	unique_lock<mutex> mLock(gMutex);
	gCV.wait_for(mLock, chrono::seconds(3));
	cout << "[" << idx << "] Thread end." << endl;
}

// ��ǳQ�������A�|���Ӥ@��B�M���X�����T����A�e�X�@�ӳq���A�ӳ���t�@�Ӧb���ݤ�������ǡA�o�˪��g�k�]�i�H�T�O�Ҧ��b���ݪ�����ǳ��|�Q����B�ӥB�@���u���@�Ӧb����
void funcThread_ex2_v2(size_t idx)
{
	cout << "[" << idx << "] Thread started." << endl;
	unique_lock<mutex> mLock(gMutex);
	gCV.wait(mLock);

	this_thread::sleep_for(chrono::seconds(1));
	cout << "[" << idx << "] Thread end, notify next." << endl;
	gCV.notify_one();
}

int main(int argc, char** argv)
{
	/************************************************* Ex1 : wait(), notify_all() �d�� *************************************************/

	//cout << "[Main] create new thread" << endl;
	//thread t1(funcThread);

	//this_thread::sleep_for(chrono::seconds(3)); //��@�U�ɶ��A�]�� IO �t�׺C

	//cout << "[Main] wait 1 second" << endl;
	//this_thread::sleep_for(chrono::seconds(3));

	//cout << "[Main] send notify" << endl;
	//gCV.notify_all();

	//cout << "[Main] wait thread stop" << endl;
	//t1.join();

	//system("pause");

	/********************************************* Ex2 : notify_one() V.S. notify_all() �d�� *********************************************/

	vector<thread> vThreads;
	vThreads.resize(3);

	cout << "[Main] create new thread" << endl;
	for (size_t i = 0; i < vThreads.size(); ++i)
		vThreads[i] = thread(funcThread_ex2_v2, i);

	cout << "[Main] wait 1 second" << endl;
	this_thread::sleep_for(chrono::seconds(1));

	cout << "[Main] send notify" << endl;
	gCV.notify_all();

	cout << "[Main] wait 1 second" << endl;
	this_thread::sleep_for(chrono::seconds(1));

	cout << "[Main] wait thread stop" << endl;
	for (size_t i = 0; i < vThreads.size(); ++i)
		vThreads[i].join();

	system("pause");
}