/*
	Function Pointer : 顧名思義，他就是指到 function 的指標。

		這樣寫的好處，在於可以把 function 拆開成更小的元件，讓他們可以更方便地組合、使用，有效地增加程式碼的可重複使用性。
	
		現在範例裡只有 power2() 一個函式或許感覺不太出來，但是如果我們又去額外寫了其他他的函式的話，就比較看的出來了～比如說，假設我們現在有下面這六個 function：

			int plus_one( int v );
			int power2( int v );
			int power3( int v );
			int power4( int v );

			void custom_for_each( int *pArray, unsigned int size, int (*op)( int ) );
			void custom_for_each_2D( int **pArray, int sizeX, int sizeY, int (*op)( int ) );

		但是這樣寫的缺點是非常難讀!!!!!!! int (*op)(int)」這樣的寫法，真的很難一眼就看出來哪個才是變數，所以如果經常會使用到某個 function pointer，可以用「typedef」

			typedef int (*CustomOperator)( int );
 
			void custom_for_each( int *pArray, unsigned int size, int (*op)( int ) )
			void custom_for_each( int *pArray, unsigned int size, CustomOperator op )

	FUNCTION POINTER 的陣列 : 

		int (*op[10])( int );
		for( int i = 0; i < 10; ++ i )
		  op[i] = &power2;
		  
		在這裡，「int (*op[10])( int );」就是會宣告出一個大小是 10 的 function pointer 陣列。在指定它裡面每一項的值的時候，都和本來的 function pointer 寫法一樣；
		不過在使用的時候，只要寫「op[1]( 10 )」這樣就可了，不用再加上星號（其實就和一般的陣列意義一樣）。

		typedef int (*CustomOperator)( int );

		CustomOperator op[10];
		for( int i = 0; i < 10; ++ i )
		op[i] = &power2;
*/

#include <iostream>

int power2(int v)
{
	return v * v;
}

// 傳入一個整數的陣列 pArray 和他的大小 size，以及一個 function pointer op
// 這邊這個 function pointer 的寫法，就是「int (*op)( int )」，它代表了一個名為 op 的 pointer，指到一個「接受一個整數當作參數、並回傳一個整數的 function」。
void custom_for_each(int *pArray, unsigned int size, int(*op)(int))
{
	for (unsigned int i = 0; i < size; ++i)
		pArray[i] = (*op)(pArray[i]); // (*op) => power(int v)
}

int main(int argc, char* argv[])
{
	int size = 10;
	int* pArray = new int[size];
	for (unsigned int i = 0; i < size; ++i)
		pArray[i] = i;

	custom_for_each(pArray, size, &power2);

	for (unsigned int i = 0; i < size; ++i)
		printf("%d, ", pArray[i]);

	delete[] pArray;

	system("pause");
	return 0;
}