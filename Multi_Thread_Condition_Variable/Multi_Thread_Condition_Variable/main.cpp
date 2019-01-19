/*
	在 Multi_Thread_Communication_2 中我們學習到了基本的 unique_lock 應用(銀行轉帳)，接下來要介紹condition_variable
	他主要的用途，是用來把目前的 thread 停下來、等候通知用的；基本上，應該算是適用於多執行序環境下的事件處理，也就是由某些執行序發出通知、告訴其他執行序要去繼續執行某些動作。

	在使用 condition_variable 的時候，需要先透過 unique_lock 來鎖定一個 mutex，之後再呼叫 condition_variable 提供的 wait() 函式，來等候這個 condition_variable 發出的通知。
	這個時候，這個執行序就會整個整個停下來、等候通知；如果要讓這個執行序繼續運作的話，則是要在別的執行序 、透過呼叫 condition_variable 提供的 notify_all() 或 notify_one() 
	這兩個函式，來喚醒因為 wait() 而停下來的執行序。

	* Conditional Variable 其它的等待條件

		A. wait() : 等待 condition_variable gCV 發出通知後才可以繼續該執行緒；

			實際上，wait() 這個函式，是還有第二個參數的～第二個參數基本上接受一個可呼叫的物件，來判斷是否要停止等待；
			而這個可以被呼叫的物件的需要回傳一個 bool 變數，如果是 true 的話，condition_variable 就會停止等待、繼續執行，而如果回傳 false 的話，他則會重新開始等待下一個通知。

			ex : gCV.wait( mLock, [](){ return gDone; } ); //gDone 是一個 bool 變數，基本上預計會是在外部修改他的值。

				=	while( !gDone )
						gCV.wait( mLock );

		B. 兩個函式，來限制等待的時間；也就是當時間到了之後，就算沒有收到通知，他還是會讓執行序停止等待
		   wait_for() : 額外再給一個指定長度的時間
		   wait_until() : 額外指定一個時間點

		   ex : gCV.wait_for( mLock, chrono::seconds(5) );
				gCV.wait_until( mLock, chrono::system_clock::now() + chrono::seconds(5) );

	* 不同的通知方式

		A. notify_all() : 會去通知所有正在等待這個 condition_variable 的執行序

		B. otify_one() : 只會通知其中一個，但是應該沒有規範通知的順序。
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

// 行序被喚醒之後，會等個一秒、然後輸出結束訊息後，送出一個通知，來喚醒另一個在等待中的執行序，這樣的寫法也可以確保所有在等待的執行序都會被喚醒、而且一次只有一個在執行
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
	/************************************************* Ex1 : wait(), notify_all() 範例 *************************************************/

	//cout << "[Main] create new thread" << endl;
	//thread t1(funcThread);

	//this_thread::sleep_for(chrono::seconds(3)); //拖一下時間，因為 IO 速度慢

	//cout << "[Main] wait 1 second" << endl;
	//this_thread::sleep_for(chrono::seconds(3));

	//cout << "[Main] send notify" << endl;
	//gCV.notify_all();

	//cout << "[Main] wait thread stop" << endl;
	//t1.join();

	//system("pause");

	/********************************************* Ex2 : notify_one() V.S. notify_all() 範例 *********************************************/

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