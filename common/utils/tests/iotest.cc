#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main(int argc, char** argv)
{
	ifstream fin("test.dat", ios::binary);
	int val;
	while(fin >>val)
	{
		cout << "Read:"<<" \"" << val <<"\"";
	}

	return 0;
}
