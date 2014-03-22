#include "LogKeeper.h"

LogKeeper::LogKeeper() {
	logFile = fopen("log.txt", "w");
	if(logFile == NULL)
		printf("LOGKEEPER WARNING: can\'t create logfile\n");
}

LogKeeper::~LogKeeper() {
	fclose(logFile);
}

void LogKeeper::log(const char *_Format, ...) const {
	//TODO: Insert possible return because of disabled logging
	va_list args;
	va_start(args, _Format);
	char debug_buffer[256];
	vsnprintf(debug_buffer, sizeof(debug_buffer), _Format, args);
	fwrite(reinterpret_cast<void*>(debug_buffer), sizeof(char), strlen(debug_buffer), logFile);
	printf("%s", debug_buffer);
	fflush(logFile);
	va_end(args);
}