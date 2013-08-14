/*
 * LogEntry.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: robertc
 */

#include "../include/LogEntry.h"
#include "../include/kmvEst.h"

uint64_t Log_Entry::NEXT_INDEX = 0;

Log_Entry::Log_Entry(const char *str, uint length)
: mp_contents(str)
, m_strLength(length)
, m_index(NEXT_INDEX++)
{
	mp_kmvSyn = new kmvEst(str, length);
}

Log_Entry::~Log_Entry() {
	// TODO Auto-generated destructor stub
	if(mp_contents != NULL)
	{
		delete mp_contents;
		mp_contents = NULL;
	}
	if(mp_kmvSyn != NULL)
	{
		delete mp_kmvSyn;
		mp_kmvSyn = NULL;
	}
}

