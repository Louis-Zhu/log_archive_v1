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
 *
 */

#include <string>
#include <vector>
#include <inttypes.h>

#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

class kmv_est
{
public:
	//enum HASH_TYPE { MD2, MD4, MD5, SHA };

	/**
	 *
	 * @param[in] str - the string to build an estimator from
	 * @param[in] k -  the number of distinct values to retain in the synopsis
	 * @param[in] q_gram_length - the number of characters to grab for each set element
	 */
	kmv_est(const char* str, int k = 60, int q_gram_length = 6);

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

	virtual ~kmv_est();

	/**
	 * get the estimation of the number of distinct values in the set
	 */
	int get_DV();

	/**
	 * get the estimation of the number of distinct values in the
	 * intersection of this and the other set without
	 * creating a new set
	 */
	int intersection_DV(const kmv_est *val) const;

	/**
	 * get the estimation of the number of distinct values in the
	 * union of this set and the other set without creating a new set
	 */
	int union_DV(const kmv_est *val) const;

	/**
	 * estimate the jaccard value between this set and the
	 * set being passed in
	 */
	float jaccard_est(const kmv_est *val) const;

	/**
	 * calculates the following values between this set and the set passed in
	 * 1) distinct value estimation in intersection
	 * 2) distinct value estimation in union
	 * 3) jaccard estimation using the other estimated values
	 */
	void combine_DV(const kmv_est *val, int &intersection_DV, int &union_DV, float &jaccard_est) const;

private:
	//HASH_TYPE hash_function_used;
	// the synopsis values used to estimate disticnt values
	std::vector<uint64_t> *m_kmv_syn;

	// for creating the object using the hash function
	SHA_CTX* mp_hashstruct;

	uint64_t hash(const char* str, int len);
};


#endif
