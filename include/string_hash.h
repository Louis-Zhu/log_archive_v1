#ifndef STRING_HASH_H__
#define STRING_HASH_H__
/**
 * implements several hash functions for use by the kmv estimation algorithm
 * Some hash algorithms produce more bits then is needed for the kmv estimation
 * algorithm.  Because each bit should be random with any hash function
 * we only take the number of bits needed and discard the rest
 *
 * Programmer: Robert Christensen
 *             robertc@eng.utah.edu
 */

//#include <openssl/md2.h>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include <cstdint>

namespace String_Hash
{
  enum HASH_TYPE { MD4, MD5, SHA, INVALID };

  class stringHash
  {
  public:
	  /**
	   * constructs a string hash function given the type of hash
	   * function that is desired
	   *
	   */
	  //stringHash(HASH_TYPE hashType);

	  static stringHash* getHashFunction(HASH_TYPE hashType);

	  virtual ~stringHash();

	  /**
	   * hash a string of length len.  The length is provided
	   * separately so it is simple to hash multiple q-grams
	   * in the bast string
	   */
	  virtual uint64_t hash(const char* str, int len) = 0;

	  /**
	   * get the type of hash function this object uses
	   */
	  virtual HASH_TYPE get_hash_type( ) const = 0;
  };

  class stringHash_SHA : public stringHash
  {
  public:
	  stringHash_SHA( );

	  virtual ~stringHash_SHA( );

	  virtual uint64_t hash(const char* str, int len);

	  virtual HASH_TYPE get_hash_type( ) const
	  {
		  return HASH_TYPE::SHA;
	  }

  private:
	  SHA_CTX* mp_hashstruct;
  };

  class stringHash_MD4 : public stringHash
  {
  public:
	  stringHash_MD4( );

	  virtual ~stringHash_MD4( );

	  virtual uint64_t hash(const char* str, int len);

	  virtual HASH_TYPE get_hash_type( ) const
	  {
		  return HASH_TYPE::MD4;
	  }
  private:
	  MD4_CTX* mp_hashstruct;
  };

  class stringHash_MD5 : public stringHash
  {
  public:
	  stringHash_MD5( );

	  virtual ~stringHash_MD5( );

	  virtual uint64_t hash(const char* str, int len);

	  virtual HASH_TYPE get_hash_type( ) const
	  {
		  return HASH_TYPE::MD5;
	  }
  private:
	  MD5_CTX* mp_hashstruct;
  };
}



#endif
