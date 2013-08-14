/*
 * BucketBlock.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: robertc
 */

#include "../include/BucketBlock.h"
#include "../include/kmvEst.h"

#include <list>
#include <cstring>

using namespace std;

Bucket_Block::Bucket_Block(int bucket_index)
: m_buffer_capacity( 1024*900 ) // 900 K is the default block size for compression algorithms
, m_buffer_used( 0 )
, m_bucket_index( bucket_index )
, m_entries_inserted( 0 )
{
  mp_buffer = new char[m_buffer_capacity];
  mp_similarity_data = new list<const kmvEst*>();
}

Bucket_Block::~Bucket_Block() {
	if(mp_buffer != NULL)
	{
		delete mp_buffer;
		mp_buffer = NULL;
	}
	if(mp_similarity_data != NULL)
	{
		// each element in the list must be cleaned up individually
		list<const kmvEst*>::iterator current = mp_similarity_data->begin();
		while (current != mp_similarity_data->end())
		{
		  delete *current;
		  ++current;
		}
		delete mp_similarity_data;
		mp_similarity_data = NULL;
	}
}

float Bucket_Block::avg_sym(const kmvEst *test)
{
	int total = mp_similarity_data->size();
	float sum = 0.0;

	for(auto current = mp_similarity_data->begin();
			current != mp_similarity_data->end();
			current++)
	  sum += test->jaccard_est(*current);

	return sum / (float)total;
}

int Bucket_Block::insert_record(Log_Entry *rec)
{
	// check to see if it will fit
	// if it does not fit, just return bad value
	// TODO: throw an exception
	if(this->get_buffer_remaining_bytes() < rec->get_length())
		return -1;

	// copy the information from the log entry to the buffer
	memcpy(mp_buffer + m_buffer_used, rec->get_logdata(), rec->get_length());

	// update the total number of bytes used
	m_buffer_used += rec->get_length();

	// if there are not very many elements in the estimator list, add this estimator
	// NOTE: this is a potentially interesting location to try out different
	//       style of detecting if the number of kmv_estimators should be larger
	//       or smaller.  This is a place for future work
	if(mp_similarity_data->size() < 5)
	{
		mp_similarity_data->push_back(rec->get_kmvSyn());
		rec->forget_kmvSyn();
	}

	// cleanup the log entry.
	// NOTE: this may be better taken care of somewhere else
	delete rec;

	// return the current number of bytes remaining in the block
	return this->get_buffer_remaining_bytes();
}
