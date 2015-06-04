
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <aibase/OSSRegex.h>
#include <ailogger/Log_Msg.h>
#include "public.h"
#include "file.h"
namespace ibs {
	file::file()
		:homePath_(""),
		inPath_(""),
		tmpPath_(""),
		outPath_(""),
		errPath_(""),
		bakPath_(""),
		readName_(""),
		indbName_(""),
		fileDate_(),
		srcFile_(""),
		tmpFile_(""),
		outFile_(""),
		errFile_(""),
		bakFile_("")
	{ }

	file::~file() {
	}

	bool file::init(const string& homePath) {
		homePath_ = homePath;
		inPath_ = homePath+"/in";
		tmpPath_ = homePath+"/tmp";
		outPath_ = homePath+"/out";
		errPath_ = homePath+"/err";
		bakPath_ = homePath+"/bak";
		if(!is_dir(homePath_.c_str()) || !is_dir(inPath_.c_str()) || !is_dir(tmpPath_.c_str())
				|| !is_dir(outPath_.c_str()) || !is_dir(errPath_.c_str()) || !is_dir(bakPath_.c_str())) {
			OSS_DEBUG((LM_ERROR,"get parameter homePath[in|tmp|out|err|bak] error\n"));
			return false;
		}
		return true;
	}

	bool file::lock(const string& mask) {
		readName_ = "";
		indbName_ = "";
		srcFile_ = "";
		DIR *inDir;
		struct dirent *d;
		if((inDir=opendir(inPath_.c_str()))==NULL) {
			OSS_DEBUG((LM_ERROR,"can not open dir (%s)\n", inPath_.c_str()));
			return false;
		}
		while(d=readdir(inDir)) {
			string readname = d->d_name;
			string indbname = d->d_name;
			if(readname=="."||readname=="..") continue;
			if(readname.length()>4 && readname.substr(readname.length()-4, 4)==TMPFILESUFFIX) {
				indbname = readname.substr(0, readname.length()-4);
			}
			reg_match regmatch;
			reg_option regopt;
			int reg = regmatch.match(mask, indbname, regopt);
			if(reg == 1) {
				OSS_DEBUG((LM_INFO, "file (%s) mask not match\n", readname.c_str()));
				continue;
			}
			else if(reg == 0) {
				srcFile_ = inPath_+"/"+readname;
				if(readname.substr(readname.length()-4, 4)==TMPFILESUFFIX) {
					tmpFile_ = tmpPath_+"/"+indbname+TMPFILESUFFIX;
					outFile_ = outPath_+"/"+indbname+OUTFILESUFFIX;
					errFile_ = errPath_+"/"+indbname+ERRFILESUFFIX;
					bakFile_ = bakPath_+"/"+indbname;
				}
				else {
					tmpFile_ = tmpPath_+"/"+readname+TMPFILESUFFIX;
					outFile_ = outPath_+"/"+readname+OUTFILESUFFIX;
					errFile_ = errPath_+"/"+readname+ERRFILESUFFIX;
					bakFile_ = bakPath_+"/"+readname;
				}
				if(access(tmpFile_.c_str(), F_OK)==0
						|| access(outFile_.c_str(), F_OK)==0
						|| access(errFile_.c_str(), F_OK)==0
						|| access(bakFile_.c_str(), F_OK)==0) {
					OSS_DEBUG((LM_ERROR, "file (%s) tmp err bak cdr file exists\n", readname.c_str()));
					closedir(inDir);
					return false;
				}
				if(flock_.lock(srcFile_.c_str(), O_RDWR)==false) {
					OSS_DEBUG((LM_ERROR,"file (%s) lock error\n", readname.c_str()));
					continue;
				}
				OSS_DEBUG((LM_INFO, "start process file (%s)\n", readname.c_str()));
				if(myRename(srcFile_, tmpFile_)) {
					readName_ = readname;
					indbName_ = indbname;
					struct stat filestat;
					if(stat(tmpFile_.c_str(), &filestat)==0) {
						fileDate_ = OSSDateTime(ACE_Time_Value(filestat.st_mtime));
					}
					break;
				}
				else {
					closedir(inDir);
					return false;
				}
			}
			else {
				OSS_DEBUG((LM_ERROR,"regex match error (%s)\n", regmatch.error().c_str()));
				closedir(inDir);
				return false;
			}
		}
		closedir(inDir);
		return (readName_!=""&&indbName_!="");
	}

	bool file::unlock() {
		myRename(tmpFile_, bakFile_);
		return flock_.unlock();
	}

	bool file::myRename(const string& oldFile, const string& newFile) {
		if(access(oldFile.c_str(), F_OK)!=0) {
			OSS_DEBUG((LM_ERROR, "source file (%s) not exist\n", oldFile.c_str()));
			return false;
		}
		if(access(newFile.c_str(), F_OK)==0) {
			OSS_DEBUG((LM_ERROR, "target file (%s) exists\n", newFile.c_str()));
			return false;
		}
		int fd=open(oldFile.c_str(), O_RDONLY);
		if(fd<0) {
			OSS_DEBUG((LM_ERROR, "can not open source file (%s)\n", oldFile.c_str()));
			return false;
		}
		if(rename(oldFile.c_str(), newFile.c_str())!=0) {
			OSS_DEBUG((LM_ERROR, "can not rename source file (%s) error (%s)\n", oldFile.c_str(), strerror(errno)));
			return false;
		}
		close(fd);
		OSS_DEBUG((LM_DEBUG, "rename source file (%s) to target file (%s) ok\n", oldFile.c_str(), newFile.c_str()));
	}
}

