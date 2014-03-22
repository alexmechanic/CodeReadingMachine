
#ifndef LOG_KEEPER_H

#define LOG_KEEPER_H

#include "Config.h"

class LogKeeper {
private:
	FILE *logFile;
public:
	LogKeeper();
	~LogKeeper();
	void log(const char *_Format, ...) const;
};


#endif //LOG_KEEPER_H
