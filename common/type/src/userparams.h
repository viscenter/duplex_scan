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
	 * Base class  for UserParams
	 *
	 * Allows for specialized implemenations to specify parameters. 
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
