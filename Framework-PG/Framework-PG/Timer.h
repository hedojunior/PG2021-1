#pragma once
#include <time.h>
class Timer
{
public:
	Timer() { reset(); }
	void start() { begin = clock(); }
	void finish() { end = clock(); }
	void reset() {
		begin = 0;
		end = 0;
	}
	double getEllapsedTimeMs() {
		double e = ((double)(end - begin)) / CLOCKS_PER_SEC;
		return e * 1000;
	}
private:
	clock_t begin, end;
};