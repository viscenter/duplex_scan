/**
 *	@file common/type/src/userparams.h
 *
 * 
 * Loosely defined class that allows the user to specify user parameters for specific implementations 
 * @see UserParamsGetImageImpl
 * @see UserParamsSetShutterImpl
 *
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */

#pragma once

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
