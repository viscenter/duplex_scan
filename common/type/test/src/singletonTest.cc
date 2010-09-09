#include<common/type/src/singleton.h>
#include<string>
#include<iostream>

using namespace viz;
using namespace std;

class MyClass : public virtual Singleton<MyClass>
{
	friend class viz::Singleton<MyClass>; //to get access to the default constuctor
	public:
		void hi(){ cout <<"\nhello world im single\n";}
		virtual ~MyClass(){ cout<<"~MyClass()"<<endl;}
	protected:
		MyClass(){ cout<<"MyClass()"<<endl;}
		MyClass(const MyClass&);
		MyClass& operator=(const MyClass&);
		

};


int main(int argc, char** argv)
{
   MyClass *b = MyClass::getInstance();
   MyClass *c = MyClass::getInstance();
	c->hi();
	b = c;
	c->hi();
	string line;
	cout <<"\nHit key to terminate";
	getline(cin, line);
   return 0;
}
