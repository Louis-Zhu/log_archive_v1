/*
 * LogEntry.h
 *
 *  Created on: Aug 13, 2013
 *      Author: robertc
 */

#ifndef LOGENTRY_H_
#define LOGENTRY_H_

#include "../include/kmvEst.h"

#include <cstdint>

class Log_Entry {
public:
	/**
	 * builds the log entry object and builds the kmv estimation
	 * synopsis using the string.
	 *
	 * NOTE: this object takes responsibility for the strings memory
	 */
	Log_Entry(const char *str, uint length);
	virtual ~Log_Entry();

	/**
	 * get the length of the stored data entry
	 */
	inline uint get_length()
	{ return m_strLength; }

	/**
	 * get the log entry index.  In other words, how many
	 * log entries have been created before this one
	 */
	inline uint64_t get_index()
	{ return m_index; }

	/**
	 * get the contents of the log
	 */
	inline const char *get_logdata()
	{ return mp_contents; }

	/**
	 * get the kmv synopsis
	 */
	inline const kmvEst *get_kmvSyn()
	{ return mp_kmvSyn; }

private:
	const char   *mp_contents;
	kmvEst *mp_kmvSyn;

	uint     m_strLength;
	uint64_t m_index;

	static uint64_t NEXT_INDEX;
};

#endif /* LOGENTRY_H_ */
