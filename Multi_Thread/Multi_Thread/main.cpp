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
	/************************************************** EX1 : 將物件, 函數, 函數物件 當成參數直接傳入 **************************************************/

	//// execute thread (建立一個 thread 的物件)
	//thread mThread_ex1(test_func);

	//// do somthing
	//cout << "main thread" << endl;

	//// wait the thread stop (告訴編譯器必須等到這個執行序完成後才可以進行下一步)
	//// 注意 : 如果某些程式中需要用到其他執行緒的計算結果時，那必須加上 join 才能保證該 thread 運作完成
	//mThread_ex1.join();

	//system("pause");

	/******************************************************** EX2 : 函數物件需要參數的呼叫方法 *******************************************************/

	//thread mThread_ex2(test_func2, 10);
	//mThread_ex2.join();
	//system("pause");

	/********************************************** EX3 : 傳入的物件需要修改本身資料的方法 & 其它函數用法 ***********************************************/

	// callable object 傳遞給 STL Thread 開啟一個新的 thread 的時候，他會是採用複製的方法，把傳入的物件複製一份來用
	// 所以如果在裡面有修改道本身的資料的話，就需要使用 std::ref() 來產生物件的參考、然後再傳進去

	funcObj co;

	// copy
	thread mThread1(co); 
	mThread1.join();
	cout << co.iData << endl;

	// reference
	thread mThread2(ref(co));
	mThread2.join();
	cout << co.iData << endl;

	// 其它函數

	// 可以用來取得在硬體層面上可以同時執行的執行序的數量，基本上可以視為處理器的核心數目；不過實際上這只是估計值，如果無法判斷時，值會是 0。
	cout << "mThread1.hardware_concurrency: " << mThread1.hardware_concurrency() << endl;

	// this_thread 是 STL thread 裡的一個特別的 namespace，底下提供 get_id()、yield()、sleep_until()、sleep_for() 四個函式可以呼叫，都是針對目前的執行序進行操作的。
	
	// get_id() : 取得目前的執行序的 id
	cout << "mThread1.get_id(): " << mThread1.get_id() << endl; // 由於這裡 thread 已經做完了，所以 id = 0
	cout << "mThread2.get_id(): " << mThread2.get_id() << endl; // 由於這裡 thread 已經做完了，所以 id = 0
	
	// sleep_for()、sleep_until() : 讓目前的執行序暫時停下來的 => 前者是停止一段指定的時間、後者則是設定一個絕對時間、讓執行序在指定的時間再繼續執行、時間的參數，則是要使用 std::chrono
	thread mThread3(sleep_for_test);
	mThread3.join();

	thread mThread4(sleep_until_test);
	mThread4.join();

	// yield() 是暫時放棄一段 CPU 時間、讓給其他執行序使用的 - 進階使用以後再說

	system("pause");
	
}