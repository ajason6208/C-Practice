/*
	Function Object �S�٧@�ufunctor�v�A��O�@�Ӫ���ɦV�����U���F��A�L�򥻤W�O��禡�����@�Ӫ���Ӱ��ާ@�C

	�Ӧp�G�Q�n�ۤv�g�@�Ӫ� function object ���ܡA�i�H�z�L�ۭq�@�ӷs�����O�A�h���s�w�q�L�� call operator�B�]�N�O operator() �Ӱ���F
	�p���@�ӡA�o�����O������N�i�H�������@��禡����k�ӷ�@�禡�ϥΤF�C

	�ϥλ��� : 

		A. �ϥ� class ��@

			�b�w�q�n�ۤv�� function object ��A��ڭn�M�Ψ� STL ���ت��t��k���ɭԡA�h�O�i�H�H�U�����Φ��A�ӧ�@�ӷs���B���O�O Output ������ǵ� for_each() �ϥΡG
		
				vector<int> v;
				...
				for_each( v.begin(), v.end(), Output() );

			�p���@�ӡA�b���� for_each() ���ɭԡA�N�|�� v �o�� vector �̪��C�@������@�ѼơB�̧Ƕǵ� Output �� �Ӱ���ۤv�w�q�� Output::operator() �F
		
			�ӳo�˪��g�k�A���N�۷�󪽱���{���g���U�����Φ��G

				vector<int> v;
				...
				for( vector<int>::iterator it = v.begin(); it != v.end(); ++ it )
				  std::cout << *it << ", ";

		B. �ϥ� structure ��@ : �o�����M OutputFunc �����u�O�@�� function�A���L�]�O�i�H���ӷ�@ object �Ϊ�

			void OutputFunc( int x )
			{
				std::cout << x << ", ";
			}
			for_each( v.begin(), v.end(), OutputFunc );

	�u�I : �򥻤W function object �M function pointer �i�H�����ƨ��t���h�A���L�M function pointer �ۤ�Afunction object �O���@���u�I��

		- �L�i�H�z�L inline �Ӵ��ɮį�

		- �L�i�H�b function �����ӰO�����A

	------------------------------------------------------------------------------------------------------------------------------------------------------------------

																				STL Binder
	STL ���w�q�X�G�u���@�ӰѼƪ��u�@���禡�v�]unary_function�^�M����Ѽƪ��u�G���禡�v�]binary_function�^�C
	STL �̤��P���t��k�A���ݨD�����P�A�n�ǤJ�� function object �i��|�O�@�����]�Ҧp for_each()�^�A�]���i��O�G�����]�Ҧp sort()�^�C

	STL function object �̡A���F negate �M logical_not �O�@���禡�~�A��L�����O�G���禡�C
	�Ӥ@���禡�]���������S�ʡA���ӴN���ӥi���ܦ��G����ƨӨϥΡF���O�����ɭԡA�ڭ̥i��|�Ʊ�z�L���w�䤤�@���Ѽƪ��ȡA�ӧ�G���禡�ܦ��@�Ӥ@���禡�ӨϥΡA�o�ɭԴN�i�H�z�L STL �� binder �Ӱ��F

		A. bind1st() : �Ψӫ��w�G���禡���Ĥ@�ӰѼƪ���
		B. bind2nd() : �Ψӫ��w�G���禡���ĤG�ӰѼƪ���
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

//�o�� class �̭����F�w�q�F����j�p�Ϊ� operator() �~�A�]�t�~�ŧi�X�F�@�Ӧ����ܼ� idx�B�H�ι������غc�l�A�Ψӫ��w�n��������ءF�o�˭n�ϥΪ��ɭԡA�u�n�z�L constructor �ӫ��w�n��������شN�i�H�F
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

	/***************************************************** EX1 : �мg operator() *****************************************************/
	Output pout;
	pout(10);

	system("pause");

	/***************************************************** EX2 : Function Object *****************************************************/

	// �p�G�S�إߤF�@�� vector< Vector3 > ����ơA���Q�n�� STL �� sort() �Ӱw��Y�@���ƧǪ��ܡA�i�H�إߤ@�Ӥ���j�p�Ϊ� function object
	vector<Vector3> v3;
	v3.push_back(Vector3(1, 2, 10));
	v3.push_back(Vector3(7, 50, 2));
	v3.push_back(Vector3(6, 10, 1));
	sort(v3.begin(), v3.end(), Comparsion(1)); // ���w�βĤ@�Ӥ������j�p�Ƨ�

	system("pause");

	/******************************************************** EX3 : STL BIND ********************************************************/

	// �ڭ̥i�H�z�L count_if() �H�Φۤv�w�q�� GreaterThan �ӭp�� vector v �̭��A�Ȥj��Y�Ӽƪ����حӼ�
	// ���L��ڤW�A�ڭ̤]�i�H�z�L STL �� binder �M greater �Ӱ���P�˪��ơA�Ӥ��Φۤv�h�w�q�o�� function object
	vector<int> v = {1, 5, 4, 10, 6, 8};
	int num = count_if(v.begin(), v.end(), GreaterThan(4));

	// �z�L bind2nd ��g, greater �O�@�ӤG���禡 http://www.cplusplus.com/reference/functional/greater/ �A bool operator() (const T& x, const T& y)
	num = count_if(v.begin(), v.end(), bind2nd(greater<int>(), 4)); // ������ĤG�ӰѼƸj�� 4 �A���N�|�@����� x > y(4) ���@�Ӥ@���禡

	system("pause");
}