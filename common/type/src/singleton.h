/**
 *	@file common/type/src/singleton.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */

#pragma once

#include <memory>
#include <iostream>

namespace viz
{
	/**
	 * @brief templated singleton pattern
	 *
	 * allowing a single instace of the defined  type  
	 *	@see CameraPASImpl
	 *	@see CameraFirewireImpl
	 */
	template<typename T>
	class Singleton
	{
		public:
			/**
			 * @brief the only accessor
			 * 
			 * the static pointer is wrapped in a std::auto_pointer  
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
