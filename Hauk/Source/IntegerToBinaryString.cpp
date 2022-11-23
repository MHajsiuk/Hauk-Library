// ----------------------------------------------------------------------------------------------------------------------
//
// Day 7 -	Convert any integer size into its binary string representation
//
// Reason:	While messing around with pointers yesterday (day 6) I wanted to see the difference between the integers 
//			-5 and 4294967291 but C doesn't have the format specifier in printf() to display binary numbers. I usually
//			Windows calculator in prgrammer mode but it has one massive fault with it, it cant do unsigned numbers
//			so entering 4294967291 in DWORD size is not possible because the largest signed number you can enter is
//			2147483647 and if we add 1 to this number we get the number -2147483648. So my goal today is to write a
//			function that takes a value and the number of bytes so that it can be used with sizeof() and produce the
//			string of 1, 0's to represent the binary number. 
//
// Extras:	I thought about returning the integer representation of the number but its kind of duplicating work. 
//			As example, the string will be either 0x30 or 0x31, if i return and integer with either a 0 or 1 it is the
//			same as a string just differnt glyphs to represent 0 or 1.
//
//
//----------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "Hauk.h"

char * IntegerToBinaryString(unsigned __int64 uUnsignedInteger, char *szOutputBuffer, unsigned int uUnsignedIntegerByteSize)
{
	unsigned int uLargestByteSize = sizeof(__int64);
	unsigned int uBitLength = 0;
	unsigned __int64 uBitMask = 1;

	// First check the byte size and if larger than the input byte size, set to the largest possible size. 
	if (uUnsignedIntegerByteSize > uLargestByteSize || uUnsignedIntegerByteSize == NULL)
	{
		uUnsignedIntegerByteSize = uLargestByteSize;
	}

	uBitLength = uUnsignedIntegerByteSize * 8;

	// Free() then Allocate enough memory of bytes +1 for nul terminator, if we fail return false.
	free(szOutputBuffer);
	szOutputBuffer = (char *)malloc(uBitLength + 1);
	if (szOutputBuffer == NULL)
	{
		return false;
	}

	// This loop calculates the lagest bit number to use in the converstion ie, if 8 bits, the loop returns 128.
	for (unsigned int uLoop = 1; uLoop < uBitLength; ++uLoop)
	{
		uBitMask = uBitMask * 2;
	}

	for (unsigned int uIndex = 0; uIndex < uBitLength; ++uIndex)
	{
		// To see if the intger has a bit turned on, we AND it with the uBitMask we created.
		bool bBitcheck = uUnsignedInteger & uBitMask;

		if (bBitcheck == true)
		{
			szOutputBuffer[uIndex] = '1';
		}
		else
		{
			szOutputBuffer[uIndex] = '0';
		}

		// Set the uBitMask to the next bit.
		uBitMask = uBitMask / 2;
	}

	// Terminate the string.
	szOutputBuffer[uBitLength] = 0x00;

	return szOutputBuffer;
}

