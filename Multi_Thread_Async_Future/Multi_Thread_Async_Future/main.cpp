/*
	�b�Ҧ� Multi_Thread_xxx �t�C���{���X���ڭ̾ǲߨ�F�ϥ� thread �禡�w���ϥΤ�k�C

	�򥻤W�A�ϥ� std::thread �ӫإߤ@�ӷs������ǡB�t�~����Y�ǭp��A�b C++ ����O������h����k�F�����\��ܼu�ʡA���O�۹諸�A�]���@�Ǥ���·Ъ��a��C
	��p���z�L std::thread ���檺�禡�򥻤W�L�k�^�ǭȡA�Ӧb��L����Ǥ����ͪ� exception �]�S����k�Q catch ��C

	* std::async() �b�Y�譱�ӻ��A�h��O�@�ӯS�ƪ��\��A�L���]�p�����A�N�O �� (���� openMP �� for parallel)

			��n�i��@�ӨS�����W�n�Ψ䵲�G�������p�⪺�ɭԡA��p����t�@�Ӱ���ǥh�A���줧��u���n�Ϊ��ɭԡA�~�h�T�{�L�]���S�B�è��o�L�����G�C
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
	int sum = parallel_sum(beg, mid); // sum = 625 (�ܦ��N��n�F)
	return sum + handle.get(); // ���ξ�߭ӧO������ǬO�_�w�g�p�⧹���F�A�]�� get() �o�Ө禡�|�����h�T�{�o��ơC
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