#pragma once

#ifndef DOMAIN_H_
#define DOMAIN_H_


namespace zios::domain
{

	enum DataType
	{
	    UNKNOWN = 0,  //!< UNKNOWN type (error)
	    DISCRETE = 1, //!< Boolean (0 FALSE, !0 TRUE (usually 1))
	    CHAR = 2,     //!< 1 byte signed
	    U_CHAR = 3,   //!< 1 byte unsigned
	    SHORT = 4,    //!< 2 bytes signed
	    U_SHORT = 5,  //!< 2 bytes unsigned
	    LONG = 6,     //!< 4 bytes signed
	    U_LONG = 7,   //!< 4 bytes unsigned
	    FLOAT = 8,    //!< single precision floating point
	    DOUBLE = 9,   //!< double precision floating point
	    STRING = 10,  //!< ascii string - 2 bytes length followed by [n] bytes character array. (serialized)
	    BLOB = 11,    //!< binary blob - 4 bytes length followed by [n] bytes (serialized)
	    LLONG = 12,   //!< 8 bytes signed
	    U_LLONG = 13, //!< 8 bytes unsigned
	};

}
#endif /* DOMAIN_H_ */
