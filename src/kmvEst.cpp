#include "../include/kmvEst.h"
#include "../include/string_hash.h"

// required for c++11 libraries (such as unordered_set)
//#define __GXX_EXPERIMENTAL_CXX0X__

#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cstring>
#include <limits>
#include <cstdint>

#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

using namespace std;

bool kmvEst::HASH_FUNCTION_USED              = false;
String_Hash::HASH_TYPE kmvEst::HASH_FUNCTION = String_Hash::HASH_TYPE::SHA;

kmvEst::kmvEst(const char* str, unsigned int k, int q_gram_length)
{
	HASH_FUNCTION_USED = true;
	String_Hash::stringHash* hasher = String_Hash::stringHash::getHashFunction(HASH_FUNCTION);

	m_kmv_syn = new vector<uint64_t>( );

	unordered_set<uint64_t> contained;

	int strLen = strlen(str);
	int end_idx = strLen - q_gram_length;
	uint64_t max_value_in_set = 0; // the current max value in the k min value estimator list.

	for(int i = 0; i < end_idx; i++)
	{
		const uint64_t val = hasher->hash(str+i, q_gram_length);

		if(!(contained.count(val) > 0)) // this is true if the value does not already exist in the set
		{
			if( m_kmv_syn->size() < k )
			{
				m_kmv_syn->push_back(val);
				push_heap(m_kmv_syn->begin(), m_kmv_syn->end());

				// insert the value into the hash set
				contained.insert(val);
				max_value_in_set = m_kmv_syn->front();
			}else if( val < max_value_in_set )
			{
				// insert the new element into kmv
				m_kmv_syn->push_back(val);
				push_heap(m_kmv_syn->begin(), m_kmv_syn->end());

				// remove the largest element in kmv
				// (because we are at the max number of elements to place in the estimator)
				pop_heap(m_kmv_syn->begin(), m_kmv_syn->end());
				contained.erase(m_kmv_syn->back()); // the last element in the vector will be the largest value.  Remove this from the contains set
				m_kmv_syn->pop_back();

				// insert the new value into the hash set
				contained.insert(val);
				max_value_in_set = m_kmv_syn->front();
			}
		}
	}
	// the elements were stored in a heap to make it more easy to work with.  We now sort it linearly
	sort_heap(m_kmv_syn->begin(), m_kmv_syn->end());

	// cleanup the hash function
	delete hasher;
}

kmvEst::~kmvEst()
{
	delete m_kmv_syn;
}

int kmvEst::get_DV()
{
	// I am using a double, because I don't know if accuracy is significantly effected by using a float
	double U = (0.0 + m_kmv_syn->back()) / (sizeof(uint64_t) << 8);
	return (int) (m_kmv_syn->size() - 1) / U;
}

int kmvEst::intersection_DV(const kmvEst *val) const
{
	int int_DV;
	int un_DV;
	float jac_est;
	combine_DV(val, int_DV, un_DV, jac_est);

	return int_DV;
}

int kmvEst::union_DV(const kmvEst *val) const
{
	int int_DV;
	int un_DV;
	float jac_est;
	combine_DV(val, int_DV, un_DV, jac_est);

	return un_DV;
}

float kmvEst::jaccard_est(const kmvEst *val) const
{
	int int_DV;
	int un_DV;
	float jac_est;
	combine_DV(val, int_DV, un_DV, jac_est);

	return jac_est;
}

void kmvEst::combine_DV(const kmvEst *val, int &intersection_DV, int &union_DV, float &jaccard_est) const
{
	// get the set with the smallest number of elements
	int min_k = min( this->m_kmv_syn->size(), val->m_kmv_syn->size() );

	// if the number of elements is too small to perform an estimation,
	// not use trying.  Just say it is invalid
	if(min_k < 1)
	{
		intersection_DV = union_DV = jaccard_est = 0;
		return;
	}

	uint64_t last_hash;
	int current             = 0;
	int intersection_count  = 0;
	auto it_a = this->m_kmv_syn->begin();
	auto it_b =  val->m_kmv_syn->begin();
	// do the operation L_a (+) L_b. In order to calculate the union estimation,
	// I only need the value of U(k) and k.  I throw away all other values.
	while(current < min_k)
	{
		current++;
		if(*it_a < *it_b)
		{
			last_hash = *it_a;
			it_a++;
		}
		else if(*it_a > *it_b)
		{
			last_hash = *it_b;
			it_b++;
		}
		else // if *it_a == *it_b
		{
			intersection_count++;
			last_hash = *it_b;
			it_a++;
			it_b++;
		}
	}

	// we first need to normalize the value of U(k)
	double U        = (0.0 + last_hash) / (numeric_limits<uint64_t>::max());
	union_DV        = (min_k - 1) / U;
	jaccard_est     = (float) intersection_count / min_k;
	intersection_DV = jaccard_est * union_DV;
}






