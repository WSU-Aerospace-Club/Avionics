#ifndef _DATALOGGER
#define _DATALOGGER

#include <SD.h>>
#include "Parameters.h"

class DataLogger{
public:
	static Parameters::param_element* readParam(uint16_t* count);
	static void writeParam(Parameters * pars);
	static void readLog();
	static void writeLog(char * log);
	DataLogger();
	~DataLogger();
private:
	static constexpr char* logName = "Log.txt";
	static constexpr char* paramName = "PARAMS.TXT";
	static constexpr uint8_t chipEnable = 27;
};
/*
template<typename T, typename P>
T remove_if(T beg, T end, P pred)
{
	T dest = beg;
	for (T itr = beg; itr != end; ++itr)
	if (!pred(*itr))
		*(dest++) = *itr;
	return dest;
}
*/
#endif