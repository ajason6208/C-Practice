/*
	在所有 Multi_Thread_xxx 系列的程式碼中我們學習到了使用 thread 函式庫的使用方法。

	基本上，使用 std::thread 來建立一個新的執行序、另外執行某些計算，在 C++ 中算是比較底層的方法；它的功能很彈性，但是相對的，也有一些比較麻煩的地方。
	比如說透過 std::thread 執行的函式基本上無法回傳值，而在其他執行序中產生的 exception 也沒有辦法被 catch 到。

	* std::async() 在某方面來說，則算是一個特化的功能，他的設計概念，就是 ↓ (類似 openMP 的 for parallel)

			當要進行一個沒有馬上要用其結果的複雜計算的時候，把計算丟到另一個執行序去，等到之後真的要用的時候，才去確認他跑完沒、並取得他的結果。
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>

template <typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end)
{
	auto len = end - beg;
	if (len < 1000)
		return std::accumulate(beg, end, 0);

	RandomIt mid = beg + len / 2;
	auto handle = std::async(std::launch::async, parallel_sum<RandomIt>, mid, end); // (7500 , 10000),(8750 , 10000),(9375 , 10000)
	int sum = parallel_sum(beg, mid); // sum = 625 (很早就算好了)
	return sum + handle.get(); // 不用擔心個別的執行序是否已經計算完成了，因為 get() 這個函式會幫忙去確認這件事。
}

int main()
{
	/************************************************* Ex1 : Async *************************************************/
	//std::vector<int> v(10000, 1);
	//std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';
	//system("pause");

	/************************************************* Ex2 : Future *************************************************/

	// future from a packaged_task
	std::packaged_task<int()> task([] { return 7; }); // wrap the function
	std::future<int> f1 = task.get_future();  // get a future
	std::thread t(std::move(task)); // launch on a thread

	// future from an async()
	std::future<int> f2 = std::async(std::launch::async, [] { return 8; });

	// future from a promise
	std::promise<int> p;
	std::future<int> f3 = p.get_future();
	std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

	std::cout << "Waiting..." << std::flush;
	f1.wait();
	f2.wait();
	f3.wait();
	std::cout << "Done!\nResults are: "
		<< f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
	t.join();

	system("pause");
}