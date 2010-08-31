/**
 *	@file common/type/src/userparams.h
 * Interface class that defines user parameters
 * 
 * Loosely defined class that allows the user to specify user parameters for specific implementations 
	@see UserParamsGetImageImpl
	@see UserParamsSetShutterImpl
 *
 */

#pragma once

/**
 * viz namespace
 */
namespace viz
{
	/**
	 * Base class that users must inherit from to allow for specialized implemenations for specify parameters. 
	 */
	class UserParams
	{
		public:
			/**
			 * default constructor
			 */
			UserParams(){}
			/**
			 * default virtual destructor
			 */
			virtual ~UserParams(){}
	};
}
