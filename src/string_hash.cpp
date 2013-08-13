/**
 * Implements several hash functions to be used by the kmv estimation algorithm
 *
 *
 */
#include "../include/string_hash.h"
#include <cstdlib>

using namespace std;
using namespace String_Hash;

stringHash* stringHash::getHashFunction(HASH_TYPE hashType)
{
	switch(hashType)
	{
	case HASH_TYPE::MD4:
		return new stringHash_MD4();
		break;
	case HASH_TYPE::MD5:
		return new stringHash_MD5();
		break;
	case HASH_TYPE::SHA:
		return new stringHash_SHA();
		break;
	default:
		return NULL;
		break;
	}
	return NULL;
}

stringHash::~stringHash()
{ }

/////////////////////////////// stringHash_MD4 ///////////////////////////////////
stringHash_SHA::stringHash_SHA( )
{
	mp_hashstruct = (SHA_CTX*) malloc( sizeof(SHA_CTX) );
}

stringHash_SHA::~stringHash_SHA( )
{
	free(mp_hashstruct);
}

uint64_t stringHash_SHA::hash(const char* str, int len)
{
	// the data array can be made static to slightly change performance, but it will
	// cause this function to not be thread safe
	unsigned char data[SHA_DIGEST_LENGTH];
	uint64_t* r_val = (uint64_t*) &data;

	SHA1_Init( mp_hashstruct );
	SHA1_Update(mp_hashstruct, str, len);
	SHA1_Final(data, mp_hashstruct);

	return *r_val;
}

/////////////////////////////// stringHash_MD4 ///////////////////////////////////
stringHash_MD4::stringHash_MD4( )
{
	mp_hashstruct = (MD4_CTX*) malloc( sizeof(MD4_CTX) );
}

stringHash_MD4::~stringHash_MD4( )
{
	free(mp_hashstruct);
}

uint64_t stringHash_MD4::hash(const char* str, int len)
{
	// the data array can be made static to slightly change performance, but it will
	// cause this function to not be thread safe
	unsigned char data[MD4_DIGEST_LENGTH];
	uint64_t* r_val = (uint64_t*) &data;

	MD4_Init( mp_hashstruct );
	MD4_Update(mp_hashstruct, str, len);
	MD4_Final(data, mp_hashstruct);

	return *r_val;
}

/////////////////////////////// stringHash_MD5 ///////////////////////////////////
stringHash_MD5::stringHash_MD5( )
{
	mp_hashstruct = (MD5_CTX*) malloc( sizeof(MD5_CTX) );
}

stringHash_MD5::~stringHash_MD5( )
{
	free(mp_hashstruct);
}

uint64_t stringHash_MD5::hash(const char* str, int len)
{
	// the data array can be made static to slightly change performance, but it will
	// cause this function to not be thread safe
	unsigned char data[MD5_DIGEST_LENGTH];
	uint64_t* r_val = (uint64_t*) &data;

	MD5_Init( mp_hashstruct );
	MD5_Update(mp_hashstruct, str, len);
	MD5_Final(data, mp_hashstruct);

	return *r_val;
}
