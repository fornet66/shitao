
#ifndef __IO_FILE_H__
#define __IO_FILE_H__
#include <string>
#include <fstream>
#include "define.h"
#include "flock.h"
namespace ibs {
	class file {
		public:
			file();
			~file();
			bool init(const string &homePath);
			bool lock(const string& pattern);
			bool unlock();
			const string& readName() const {return readName_;}
			const string& indbName() const {return indbName_;}
			const OSSDateTime& fileDate() const {return fileDate_;}
			const string& tmpFile() const {return tmpFile_;}
			const string& outFile() const {return outFile_;}
			const string& errFile() const {return errFile_;}
		private:
			bool myRename(const string &oldFile, const string &newFile);
		private:
			string homePath_;
			string inPath_;
			string tmpPath_;
			string outPath_;
			string errPath_;
			string bakPath_;
			string readName_;
			string indbName_;
			OSSDateTime fileDate_;
			string srcFile_;
			string tmpFile_;
			string outFile_;
			string errFile_;
			string bakFile_;
			filelock flock_;
	};
}
#endif	// __IO_FILE_H__

