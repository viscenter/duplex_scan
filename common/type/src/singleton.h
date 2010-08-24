#pragma once

#include <memory>
#include <iostream>

namespace viz
{
	template<typename T>
	class Singleton
	{
		public:
			static T* getInstance()
			{
				static std::auto_ptr<T> instance (new T);
				return instance.get();
			}
			
		protected:
			Singleton()
			{
#if DEBUG  
	std::cout<<"Singleton()"<<std::endl;
#endif
			}
			virtual ~Singleton()
{
#if DEBUG  
 std::cout<<"~Singleton()"<<std::endl;
#endif
}
		private:
			Singleton(const Singleton&);
			Singleton& operator=(const Singleton&);
	};
}
