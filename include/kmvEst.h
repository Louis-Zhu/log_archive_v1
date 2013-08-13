#ifndef KMV_EST_H__
#define KMV_EST_H__
/**
 * This class will do the distinct value estimation found in the paper
 * "On Synopses for Distinct-Value Estimation Under Multiset Operations"
 * [Kevin Beyer et. al]
 *
 * A few specification about this implementation:
 *  * This uses have values that have 64 bit length.
 *  * Hash functions can be changed.  From experimenting, the MD
 *      hash functions and SHA work well.  If speed is desired md4
 *      works very well.
 *  * This creates the set from a character string, grabs q-grams
 *      from the string and inserts the values into the estimator
 *  * Only needed functionality of the estimator is implemented.
 *
 * Programmer: Robert Christensen
 *             robertc@eng.utah.edu
 */

#include "../include/string_hash.h"

#include <string>
#include <vector>
#include <inttypes.h>

#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

class kmvEst
{
public:
	/**
	 *
	 * @param[in] str - the string to build an estimator from
	 * @param[in] k -  the number of distinct values to retain in the synopsis
	 * @param[in] q_gram_length - the number of characters to grab for each set element
	 */
	kmvEst(const char* str, unsigned int k = 60, int q_gram_length = 6);

	/**
	 * optimized version where the string length is passed to prevent calculating
	 * the string length in the function
	 *
	 * @param[in] str - the string to build an estimator from
	 * @param[in] str_len - the length of the string str
	 * @param[in] k - the number of distict values to retain in the synopsis
	 * @param[in] q_gram_length - the number of characters to grab for each set element
	 */
	//kmv_est(const char* str, int str_len, int k = 60, int q_gram_length = 6);

	virtual ~kmvEst();

	/**
	 * get the estimation of the number of distinct values in the set
	 */
	int get_DV();

	/**
	 * get the estimation of the number of distinct values in the
	 * intersection of this and the other set without
	 * creating a new set
	 */
	int intersection_DV(const kmvEst *val) const;

	/**
	 * get the estimation of the number of distinct values in the
	 * union of this set and the other set without creating a new set
	 */
	int union_DV(const kmvEst *val) const;

	/**
	 * estimate the jaccard value between this set and the
	 * set being passed in
	 */
	float jaccard_est(const kmvEst *val) const;

	/**
	 * calculates the following values between this set and the set passed in
	 * 1) distinct value estimation in intersection
	 * 2) distinct value estimation in union
	 * 3) jaccard estimation using the other estimated values
	 */
	void combine_DV(const kmvEst *val, int &intersection_DV, int &union_DV, float &jaccard_est) const;

	/**
	 * sets the hash function used by the kmv estimator.  This is static so that
	 * all kmv estimators will have the same hash function.  This makes it so
	 * all kmv estimators can be used with each other.
	 *
	 * NOTE: If this behavior needs to change, the estimator only needs to check
	 *       that the synopsis was built using the same hash function.  It should
	 *       be a trivial change if needed.
	 *
	 * @param type the hash function type to use
	 * @returns bool true if it was able to change the value, false otherwise (if a hash function has already been used or set)
	 */
	static bool set_hash_function(String_Hash::HASH_TYPE type);

private:
	//HASH_TYPE hash_function_used;
	// the synopsis values used to estimate distinct values
	std::vector<uint64_t> *m_kmv_syn;

	// true if the hash function value has been set or used
	static bool HASH_FUNCTION_USED;

	static String_Hash::HASH_TYPE HASH_FUNCTION;

};


#endif
