/*
	Function Pointer : �U�W��q�A�L�N�O���� function �����СC

		�o�˼g���n�B�A�b��i�H�� function ��}����p������A���L�̥i�H���K�a�զX�B�ϥΡA���Ħa�W�[�{���X���i���ƨϥΩʡC
	
		�{�b�d�Ҹ̥u�� power2() �@�Ө禡�γ\�Pı���ӥX�ӡA���O�p�G�ڭ̤S�h�B�~�g�F��L�L���禡���ܡA�N����ݪ��X�ӤF���p���A���]�ڭ̲{�b���U���o���� function�G

			int plus_one( int v );
			int power2( int v );
			int power3( int v );
			int power4( int v );

			void custom_for_each( int *pArray, unsigned int size, int (*op)( int ) );
			void custom_for_each_2D( int **pArray, int sizeX, int sizeY, int (*op)( int ) );

		���O�o�˼g�����I�O�D�`��Ū!!!!!!! int (*op)(int)�v�o�˪��g�k�A�u�������@���N�ݥX�ӭ��Ӥ~�O�ܼơA�ҥH�p�G�g�`�|�ϥΨ�Y�� function pointer�A�i�H�Ρutypedef�v

			typedef int (*CustomOperator)( int );
 
			void custom_for_each( int *pArray, unsigned int size, int (*op)( int ) )
			void custom_for_each( int *pArray, unsigned int size, CustomOperator op )

	FUNCTION POINTER ���}�C : 

		int (*op[10])( int );
		for( int i = 0; i < 10; ++ i )
		  op[i] = &power2;
		  
		�b�o�̡A�uint (*op[10])( int );�v�N�O�|�ŧi�X�@�Ӥj�p�O 10 �� function pointer �}�C�C�b���w���̭��C�@�����Ȫ��ɭԡA���M���Ӫ� function pointer �g�k�@�ˡF
		���L�b�ϥΪ��ɭԡA�u�n�g�uop[1]( 10 )�v�o�˴N�i�F�A���ΦA�[�W�P���]���N�M�@�몺�}�C�N�q�@�ˡ^�C

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

// �ǤJ�@�Ӿ�ƪ��}�C pArray �M�L���j�p size�A�H�Τ@�� function pointer op
// �o��o�� function pointer ���g�k�A�N�O�uint (*op)( int )�v�A���N��F�@�ӦW�� op �� pointer�A����@�ӡu�����@�Ӿ�Ʒ�@�ѼơB�æ^�Ǥ@�Ӿ�ƪ� function�v�C
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