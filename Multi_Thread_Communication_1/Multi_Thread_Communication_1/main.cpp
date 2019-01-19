/*
	在 Multi_Thread 專案底下我們學到了一些基礎的用法

		1. 透過建立一個新的 std::thread 物件、產生一個新的執行序

		2. 在必要時呼叫 std::thread 物件的 join() 函式，確保該執行序已結束

	不過，比較簡單的寫法，就是讓多個執行序、各自去執行彼此之間不相關的工作，在這種情況下，其實通常不會有什麼大問題
	但是，如果不同的執行序之間，有去修改到共用的變數的話，就可能會有因為不知道哪個執行續會先執行到、而有問題發生了

	以下面的例子來說，正常的 call OutputValue() 不會有任何問題，但是如果以執行緒來呼叫的話會發現結果好像怪怪的，所以我們有兩種修改方式

	A. Mutex 解決 : OutputValue_Mutex()

		Mutex 方法就是透過一個全域變數 gMutex 來對函數進行控制，主要就是透過 lock() 和 unlock() 這兩個函式，來設定變數的狀態是否被鎖定。

			lock() : 檢查 gMutex 是否已經被鎖定，如果沒有被鎖住的話，他就會把 gMutex 設定成鎖定、然後繼續執行；如果已經被鎖住的話，他則會停在這邊、等到鎖定被解除、再把 gMutex 鎖住、繼續執行。
			unlock() : 做完鎖住的部分後會進行解鎖
		
		但是!!!!!!!這樣做並不好，原因在於如果你的函數不小心因為 return 離開的話，就會導致永遠不會呼叫到 unlock(),那你就進入無限等待，所以一般會透過方法 B 來解決
	
	B. lock_guard 解決 :  OutputValue_Lock_Guard()

		lock_guad<mutex> 的物件 mLock、來管理全域變數 gMutex
		當 mLock 被建立的同時，gMutex 就會被自動鎖定，而當 mLock 因為生命週期結束而消失時，gMutex 也會因此被自動解鎖
		相較於前面手動使用 lock() 和 unlock()，使用 lock_guard 算是一個比較方便、也比較安全的方法。
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