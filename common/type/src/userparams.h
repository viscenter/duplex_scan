/**
 *	@file common/type/src/userparams.h
 *
 *  Interface class that defines user parameters
 * 
 * Loosely defined class that allows the user to specify user parameters for specific implementations 
 * @see UserParamsGetImageImpl
 * @see UserParamsSetShutterImpl
 *
 */

#pragma once

/**
 * @brief viz namespace
 */
namespace viz
{
	/**
	 * @brief Base class for user parameters
	 *
	 * Allows for specialized implemenations to specify parameters. 
	 */
	class UserParams
	{
		public:
			/**
			 * @brief default constructor
			 */
			UserParams(){}
			/**
			 * @brief default virtual destructor
			 */
			virtual ~UserParams(){}
	};
}
