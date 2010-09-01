/**
 *	@file common/type/src/singleton.h
 * template class the defines a singleton pattern to allow only once instance in a program 
 * Used for  
	@see CameraFirewireImpl
	@see CameraPASImpl
 *
 */

#pragma once

#include <memory>
#include <iostream>

/**
* viz namespace
*/
namespace viz
{
	/**
	 * simple templated singleton pattern
	 */
	template<typename T>
	class Singleton
	{
		public:
			/**
			 * the only accessor
			 * 
			 * @returns a pointer to the datatype's only instance in a running program
			 */
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
