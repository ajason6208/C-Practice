/*
	在 Multi_Thread_Communication_1 中我們學習到了基本的 mutex & lock_guard 應用，接下來要介紹更多進階的用法

	A. 其它種類的 mutex : timed_mutex(支援定時)、recursive_mutex(支援遞迴)、recursive_timed_mutex

		1. timed_mutex : 除了基本款 mutex 的 lock() 和 try_lock() 外，還額外提供了 try_lock_for() 和 try_lock_until() 這兩個函式，可以設定在指定的時間內、試著去進行 mutex 的鎖定的動作。

		2. recursive_mutex : 讓 mutex 認得鎖住自己的執行序，並且讓 mutex 在已經被鎖定的情況下，還是可以讓同一個執行序再去鎖定他、而不會被擋下來

			在 ClassA 裡，有 func1() 和 func2() 兩個函式，兩者都會去建立一個 lock_guard 的物件、來鎖定 mMutex 這個 mutex。
			不過，由於 func2() 裡、會去呼叫 func1()，所以如果去呼叫 func2() 的話，實際上可以發現，程式會在執行 func2() 的時候，透過 lock_guard 去鎖定 mMutex
			而當在 func2() 內去呼叫 func1() 的時候，同樣的要求鎖定動作，又會再進行一次！

			這時候如果是使用標準的 mutex 的話，在第二次試圖去鎖定 mMutex 的時候（func1()），會因為 mMutex 已經在 func2() 裡被鎖定了，而就因此停在這邊，等待 mMutex 被解除鎖定再繼續
			但是由於 mMutex 的鎖定狀態必須要等到 func2() 整個執行完成後才會解除，所以這邊就會變成永遠等不完的狀況，讓程式無法繼續執行。

		3. recursive_timed_mutex : 顧名思義，就是同時具有 timed_mutex、recursive_mutex 兩種特殊性質的 mutex 類別
	
	B. 比 lock_guard 更彈性的 :  unique_lock，有以下兩大特色

		1. unique_lock 不一定要擁有 mutex，所以可以透過 default constructor 建立出一個空的 unique_lock。

		2. unique_lock 雖然一樣不可複製（non-copyable），但是它是可以轉移的（movable）。所以，unique_lock 不但可以被函式回傳，也可以放到 STL 的 container 裡。

		EX : 雖然一般狀況下，大多是一次去鎖定一個 mutex 來使用的。但是在實際使用的時候，有的時候還是會需要同時去鎖定多個 mutex，來避免資料不同步的問題 (如下)

		* 這裡我們對於 transfer 進行探討，為什麼要延後在鎖定(defer_lock)，而不是直接寫成下面程式呢? 因為如果用上面的寫法，直接依序各自鎖定兩個 mutex 的話，有可能會在多執行序的情況下，產生 dead lock 的狀況。

			lock_guard<mutex> lock1( from.mMutex );
			lock_guard<mutex> lock2( to.mMutex );

			Ex : 如果同時要求進行「由 A 轉帳給 B」（thread 1）和「由 B 轉帳給 A」（thread 2）的動作的話，有可能會產生一個狀況，就是在 thread 1 裡面已經鎖定了 A 的 mutex，但是在試圖鎖定 B 的 mutex 的時候，thread 2 已經鎖定了 B 的 mutex；而同樣地，這時候 thread 2 也需要去鎖定 A 的 mutex，但是他卻已經被 thread 1 鎖定了。

			如此一來，就會變成 thread 1 鎖著 A 的 mutex 在等 thread 2 把 B 的 mutex 解鎖，而 thread 2 鎖著 B 的 mutex 在等 thread 1 把 A 的 mutex 解鎖的狀況…這樣的情況，基本上是無解的。

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

// 轉帳時，必須鎖定兩個帳號，以免其他人也在對這兩個帳號進行操錯
void transfer(bank_account &from, bank_account &to, int amount)
{
	// don't actually take the locks yet
	unique_lock<mutex> lock1(from.mMutex, defer_lock); // defer_lock : 雖然要讓他去管理指定的 mutex 物件，但是不要立刻去鎖定他、而是維持沒有鎖定的狀態
	unique_lock<mutex> lock2(to.mMutex, defer_lock);

	// lock both unique_locks without deadlock
	lock(lock1, lock2); // 而接下來，則是再透過 lock() 這個函式，來「同時」鎖定 lock1 和 lock2 這兩個 unique_lock 物件。

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