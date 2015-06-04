
#ifndef LOGGER_HANDLE_H
#define LOGGER_HANDLE_H
#include "logger.h"
class LoggerHandle {
	public:
		LoggerHandle(Connection *db);
		~LoggerHandle();
		int select(const string& start, const string& end, vector<Logger>& lgrs);
		int insert(const vector<Logger>& lgrs);
	private:
		Connection *db_;
		Statement *s_, *i_;
};
#endif

