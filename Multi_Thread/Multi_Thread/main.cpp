#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

// EX1
void test_func()
{
	// do something
	double dSum = 0;
	for (int i = 0; i < 10000; ++i)
		for (int j = 0; j < 10000; ++j)
			dSum += i*j;

	cout << "Thread: " << dSum << endl;
}

// EX2
void test_func2(int i)
{
	cout << i << endl;
}

// EX3
class funcObj
{
public:
	int iData;

	funcObj()
	{
		iData = 0;
	}

	void operator()()
	{
		++iData;
	}
};

void sleep_for_test() {
	int i = 0;
	while (i < 10)
	{
		// Print Thread ID and Counter i
		cout << this_thread::get_id() << " :: " << i++ << endl;

		// Sleep this thread for 200 MilliSeconds
		// std::chrono::nanoseconds / std::chrono::microseconds / std::chrono::milliseconds / std::chrono::seconds / std::chrono::minutes / std::chrono::hours
		this_thread::sleep_for(chrono::milliseconds(200)); 
	}
}

// Print Current Time
const string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void sleep_until_test() {

	// Print Current Time
	cout << "currentDateTime()=" << currentDateTime() << endl;

	// create a time point pointing to 10 second in future
	chrono::system_clock::time_point timePoint = chrono::system_clock::now() + chrono::seconds(10);

	// Sleep Till specified time point
	// Accepts std::chrono::system_clock::time_point as argument
	this_thread::sleep_until(timePoint);

	// Print Current Time
	cout << "currentDateTime()=" << currentDateTime() << endl;
}


int main(int argc, char** argv)
{
	/************************************************** EX1 : �N����, ���, ��ƪ��� ���Ѽƪ����ǤJ **************************************************/

	//// execute thread (�إߤ@�� thread ������)
	//thread mThread_ex1(test_func);

	//// do somthing
	//cout << "main thread" << endl;

	//// wait the thread stop (�i�D�sĶ����������o�Ӱ���ǧ�����~�i�H�i��U�@�B)
	//// �`�N : �p�G�Y�ǵ{�����ݭn�Ψ��L��������p�⵲�G�ɡA�������[�W join �~��O�Ҹ� thread �B�@����
	//mThread_ex1.join();

	//system("pause");

	/******************************************************** EX2 : ��ƪ���ݭn�Ѽƪ��I�s��k *******************************************************/

	//thread mThread_ex2(test_func2, 10);
	//mThread_ex2.join();
	//system("pause");

	/********************************************** EX3 : �ǤJ������ݭn�ק糧����ƪ���k & �䥦��ƥΪk ***********************************************/

	// callable object �ǻ��� STL Thread �}�Ҥ@�ӷs�� thread ���ɭԡA�L�|�O�ĥνƻs����k�A��ǤJ������ƻs�@���ӥ�
	// �ҥH�p�G�b�̭����ק�D��������ƪ��ܡA�N�ݭn�ϥ� std::ref() �Ӳ��ͪ��󪺰ѦҡB�M��A�Ƕi�h

	funcObj co;

	// copy
	thread mThread1(co); 
	mThread1.join();
	cout << co.iData << endl;

	// reference
	thread mThread2(ref(co));
	mThread2.join();
	cout << co.iData << endl;

	// �䥦���

	// �i�H�ΨӨ��o�b�w��h���W�i�H�P�ɰ��檺����Ǫ��ƶq�A�򥻤W�i�H�����B�z�����֤߼ƥءF���L��ڤW�o�u�O���p�ȡA�p�G�L�k�P�_�ɡA�ȷ|�O 0�C
	cout << "mThread1.hardware_concurrency: " << mThread1.hardware_concurrency() << endl;

	// this_thread �O STL thread �̪��@�ӯS�O�� namespace�A���U���� get_id()�Byield()�Bsleep_until()�Bsleep_for() �|�Ө禡�i�H�I�s�A���O�w��ثe������Ƕi��ާ@���C
	
	// get_id() : ���o�ثe������Ǫ� id
	cout << "mThread1.get_id(): " << mThread1.get_id() << endl; // �ѩ�o�� thread �w�g�����F�A�ҥH id = 0
	cout << "mThread2.get_id(): " << mThread2.get_id() << endl; // �ѩ�o�� thread �w�g�����F�A�ҥH id = 0
	
	// sleep_for()�Bsleep_until() : ���ثe������ǼȮɰ��U�Ӫ� => �e�̬O����@�q���w���ɶ��B��̫h�O�]�w�@�ӵ���ɶ��B������Ǧb���w���ɶ��A�~�����B�ɶ����ѼơA�h�O�n�ϥ� std::chrono
	thread mThread3(sleep_for_test);
	mThread3.join();

	thread mThread4(sleep_until_test);
	mThread4.join();

	// yield() �O�Ȯɩ��@�q CPU �ɶ��B������L����ǨϥΪ� - �i���ϥΥH��A��

	system("pause");
	
}