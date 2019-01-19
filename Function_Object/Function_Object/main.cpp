/*
	Function Object 又稱作「functor」，算是一個物件導向概念下的東西，他基本上是把函式視為一個物件來做操作。

	而如果想要自己寫一個的 function object 的話，可以透過自訂一個新的類別，去重新定義他的 call operator、也就是 operator() 來做到；
	如此一來，這個類別的物件就可以用類似一般函式的方法來當作函式使用了。

	使用說明 : 

		A. 使用 class 實作

			在定義好自己的 function object 後，實際要套用到 STL 內建的演算法的時候，則是可以以下面的形式，來把一個新的、型別是 Output 的物件傳給 for_each() 使用：
		
				vector<int> v;
				...
				for_each( v.begin(), v.end(), Output() );

			如此一來，在執行 for_each() 的時候，就會把 v 這個 vector 裡的每一項都當作參數、依序傳給 Output 的 來執行自己定義的 Output::operator() 了
		
			而這樣的寫法，其實就相當於直接把程式寫成下面的形式：

				vector<int> v;
				...
				for( vector<int>::iterator it = v.begin(); it != v.end(); ++ it )
				  std::cout << *it << ", ";

		B. 使用 structure 實作 : 這樣雖然 OutputFunc 本身只是一個 function，不過也是可以拿來當作 object 用的

			void OutputFunc( int x )
			{
				std::cout << x << ", ";
			}
			for_each( v.begin(), v.end(), OutputFunc );

	優點 : 基本上 function object 和 function pointer 可以做的事其實差不多，不過和 function pointer 相比，function object 是有一些優點的

		- 他可以透過 inline 來提升效能

		- 他可以在 function 內部來記錄狀態

	------------------------------------------------------------------------------------------------------------------------------------------------------------------

																				STL Binder
	STL 有定義出：只有一個參數的「一元函式」（unary_function）和有兩參數的「二元函式」（binary_function）。
	STL 裡不同的演算法，視需求的不同，要傳入的 function object 可能會是一元的（例如 for_each()），也有可能是二元的（例如 sort()）。

	STL function object 裡，除了 negate 和 logical_not 是一元函式外，其他的都是二元函式。
	而一元函式因為本身的特性，本來就不太可能變成二元函數來使用；但是有的時候，我們可能會希望透過指定其中一項參數的值，來把二元函式變成一個一元函式來使用，這時候就可以透過 STL 的 binder 來做了

		A. bind1st() : 用來指定二元函式的第一個參數的值
		B. bind2nd() : 用來指定二元函式的第二個參數的值
*/

#include <iostream>
#include <functional> 
#include <vector>
#include <algorithm>

using namespace std;

class Output
{
public:
	void operator() (int x)
	{
		std::cout << x << ", ";
	}
};

struct Vector3
{
	Vector3(const int& a, const int& b, const int& c)
	{
		m_iaData[0] = a;
		m_iaData[1] = b;
		m_iaData[2] = c;
	}

	int  m_iaData[3];
};

//這個 class 裡面除了定義了比較大小用的 operator() 外，也另外宣告出了一個成員變數 idx、以及對應的建構子，用來指定要比較的項目；這樣要使用的時候，只要透過 constructor 來指定要比較的項目就可以了
class Comparsion
{
public:
	int idx;
	Comparsion(const int& i) : idx(i) {};

	inline bool operator() (const Vector3& v1, const Vector3& v2) const
	{
		return v1.m_iaData[idx] < v2.m_iaData[idx];
	}
};

struct GreaterThan
{
	int x;
	GreaterThan(int value) : x(value) {};

	bool operator() (int value)
	{
		return value > x;
	}
};

int main() {

	/***************************************************** EX1 : 覆寫 operator() *****************************************************/
	Output pout;
	pout(10);

	system("pause");

	/***************************************************** EX2 : Function Object *****************************************************/

	// 如果又建立了一個 vector< Vector3 > 的資料，那想要用 STL 的 sort() 來針對某一項排序的話，可以建立一個比較大小用的 function object
	vector<Vector3> v3;
	v3.push_back(Vector3(1, 2, 10));
	v3.push_back(Vector3(7, 50, 2));
	v3.push_back(Vector3(6, 10, 1));
	sort(v3.begin(), v3.end(), Comparsion(1)); // 指定用第一個元素的大小排序

	system("pause");

	/******************************************************** EX3 : STL BIND ********************************************************/

	// 我們可以透過 count_if() 以及自己定義的 GreaterThan 來計算 vector v 裡面，值大於某個數的項目個數
	// 不過實際上，我們也可以透過 STL 的 binder 和 greater 來做到同樣的事，而不用自己去定義這個 function object
	vector<int> v = {1, 5, 4, 10, 6, 8};
	int num = count_if(v.begin(), v.end(), GreaterThan(4));

	// 透過 bind2nd 改寫, greater 是一個二元函式 http://www.cplusplus.com/reference/functional/greater/ ， bool operator() (const T& x, const T& y)
	num = count_if(v.begin(), v.end(), bind2nd(greater<int>(), 4)); // 直接把第二個參數綁成 4 ，那就會一直比較 x > y(4) 的一個一元函式

	system("pause");
}