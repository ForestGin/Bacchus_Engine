#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__

#include "p2Defs.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	Uint64 ReadTicks() const;

private:
	Uint64	started_at;
	static Uint64 frequency;
};

#endif //__PERFTIMER_H__