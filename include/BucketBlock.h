/*
 * BucketBlock.h
 *
 *  Created on: Aug 13, 2013
 *      Author: robertc
 */

#ifndef BUCKETBLOCK_H_
#define BUCKETBLOCK_H_

#include "../include/kmvEst.h"
#include "../include/LogEntry.h"

#include <list>

class Bucket_Block {
public:
	/**
	 * construct an empty bucket
	 *
	 * @param bucket_index - the bucket index which is used to say what bucket the
	 *                       block will be inserted to
	 */
	Bucket_Block(int bucket_index);
	virtual ~Bucket_Block();

	/**
	 * return the average jaccard score between the estimator passed
	 * in and the records in this bucket.
	 */
	float avg_sym(const kmvEst*);

	/**
	 * insert a record into the bucket_block.
	 * THE LOG ENTRY WILL BE CONSUMBED BY THIS FUNCTION,
	 * meaning after this function the log entry passed
	 * into the function will be invalid.  This is because
	 * it takes pointers from the log entry.
	 *
	 * @returns the number of bytes remaining in the block
	 */
	int insert_record(Log_Entry *);

	inline uint get_buffer_capacity()
	{ return m_buffer_capacity;	}

	inline uint get_buffer_remaining_bytes()
	{ return m_buffer_capacity - m_buffer_used; }

	inline uint get_entry_count()
	{ return m_entries_inserted; }

	inline uint get_bucket_index()
	{ return m_bucket_index; }

private:
	char *mp_buffer;

	std::list<const kmvEst*> *mp_similarity_data;

	uint m_buffer_capacity;
	uint m_buffer_used;
	uint m_bucket_index;
	uint m_entries_inserted;

};

#endif /* BUCKETBLOCK_H_ */
