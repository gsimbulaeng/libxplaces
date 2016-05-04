/*
 * xpToolSet.h
 *
 *  Created on: May 21, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPTOOLSET_H_
#define XPTOOLSET_H_

#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

#define SESSION_STRING 64, true, true, true

//!  xpToolSet is a tools class.
/*!
  xpToolSet defines methods to print debug informations, convert endianess and obtain a random string.
*/
class xpToolSet {
public:
	/*! fn static std::string randomString(int length, bool letters, bool numbers, bool symbols)
	    \brief It returns a random string.
	    \param length The lenght of the random string as int.
		\param letters Enable the use of letters if true.
		\param numbers Enable the use of numbers if true.
		\param symbols Enable the use of symbols if true.
		\details It returns a random string as std::string generated by the specified parameters.
		\return A random string as std::string.
	*/
	static std::string randomString(int length, bool letters, bool numbers, bool symbols);

	/*! fn static void debugOutput(std::string string)
	    \brief It prints debug informations.
	    \param string The string to print as std::string.
		\details It prints the specified string.
	*/
	static void debugOutput(std::string string);

	/*! fn static void debugOutput(class xpPropertyList)
	    \brief It prints debug informations.
	    \param properties The properties list to print as xpPropertyList.
		\details It prints the specified xpPropertyList.
	*/
	static void debugOutput(class xpPropertyList properties);
//	static void debugOutput(std::vector<struct property >);

	/*! fn static bool isNumber(std::string s)
	    \brief It checks if a string is a number.
	    \param s The string to test
		\details It checks if a string is a number.
		\return True if the string is a number.
	*/
	inline static bool isNumber(std::string s)
	{
	   for (unsigned int i = 0; i < s.length(); i++) {
		   #ifdef WIN32
			 if (!isdigit(s[i]))
			#else
			 if (!std::isdigit(s[i]))
			#endif

		  return false;
	   }

	   return true;
	}

	/*! fn static bool testAddress(std::string address)
	    \brief It checks if an address is well formatted.
	    \param address The address to test
		\details It checks if an address is well formatted in the following form [http://aaa.bbb.ccc.ddd:port].
		\return True is the test is successful.
	*/
	inline static bool testAddress(std::string address) {
		//Check if address starts with "http://"
		std::string copy = address;
		std::string protocol;
		protocol.append("http://");
		if( address.compare(0, protocol.length(), protocol) == 0) {
			std::string::size_type ip_start = protocol.length();
			//Move forward the start of the string
			address = address.substr(ip_start);
			//Check if address ends with a number
			std::string::size_type loc1 = address.find(":");
			std::string::size_type port_start = loc1;
			if(port_start != std::string::npos) {
				std::string port = address.substr(port_start + 1);
				if(isNumber(port)) {
					//Check if the ip is well formatted
					std::string::size_type start = 0;
					std::string::size_type end = 0;
					address = address.substr(start, port_start);

					while(end != std::string::npos) {
						address = address.substr(start);
						start = 0;
						end = address.find_first_of(".", 0);
						std::string octet;
						octet = address.substr(start, end - start);
						if(!isNumber(octet)) {
							fprintf(stderr, "Error in octect %s in address %s\n", octet.c_str(), copy.c_str());
							return false; //octet is not a number
						}
						start = end + 1;
					}
					return true; //Test ends successfully
				}
				else {
					fprintf(stderr, "Error in port %s in address %s\n", port.c_str(), copy.c_str());
					return false; //port is not a number
				}
			}
			else {
				fprintf(stderr, "Could not find port in address %s\n", copy.c_str());
				return false; //port not found (couln't find ":")
			}
		}
		else {
			fprintf(stderr, "Error in protocol in address %s\n", copy.c_str());
			return false; //"http://" not found
		}
	}

	/*! fn static void testAndBlockAddress(std::string address)
	    \brief Blocking method: It checks if an address is well formatted and abort the execution if the test is unsuccessful.
	    \param address The address to test
		\details Blocking version method: It checks if an address is well formatted in the following form [http://aaa.bbb.ccc.ddd:port] and, if fails, it aborts the execution.
	*/
	inline static void testAndBlockAddress(std::string address) {
		if(!testAddress(address)) {
			fprintf(stderr, "Address not formatted: %s\n", address.c_str());
			abort();
		}
	}

	/*! fn inline static bool testBigEndian()
	    \brief It tests the big endianess.
		\details It tests the big endianess.
		\return True if the machine is big endian
	*/
	inline static bool testBigEndian() {
		uint16_t test = 0xff;
		return ((test >> 8) == 0) ? true : false ;
	}

	/*! fn inline static uint32_t toStdEndianess(uint32_t value)
	    \brief It converts to the standard endianess.
	    \param value The value to convert
		\details It converts the parameter to the standard endianess.
		\return The value converted to the standard endianess as uint32_t.
	*/
	inline static uint32_t toStdEndianess(uint32_t value) {
		if (testBigEndian() == false) {
			return value;
		} else {
			uint32_t retVal;
			unsigned char * p1 = (unsigned char *)&value, *p2 = (unsigned char *)&retVal;

			*(p2) = *(p1 + 3);
			*(p2 + 1) = *(p1 + 2);
			*(p2 + 2) = *(p1 + 1);
			*(p2 + 3) = *(p1);

			return retVal;
		}
	}

	/*! fn inline static uint32_t toLocalEndianess(uint32_t value)
	    \brief It converts to the local endianess.
	    \param value The value to convert
		\details It converts the parameter to the local endianess.
		\return The value converted to the local endianess as uint32_t.
	*/
	inline static uint32_t toLocalEndianess(uint32_t value) {
		if (testBigEndian() == false) {
			return value;
		} else {
			uint32_t retVal;
			unsigned char * p1 = (unsigned char *)&value, *p2 = (unsigned char *)&retVal;

			*(p2) = *(p1 + 3);
			*(p2 + 1) = *(p1 + 2);
			*(p2 + 2) = *(p1 + 1);
			*(p2 + 3) = *(p1);

			return retVal;
		}

	}
};

#endif /* XPTOOLSET_H_ */
