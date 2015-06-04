
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ailogger/Log_Msg.h>
#include "public.h"
namespace ibs {
	bool is_dir(const char* path) {
		struct stat statBuf;
		if(stat(path, &statBuf)<0) {
			return false;
		}
		return S_ISDIR(statBuf.st_mode);
	}

	char upper(char c) {
		if(c>='a'&&c<='z') {
			return c-32;
		}
		return c;
	}

	char* trim(char* str, char ch) {
		if((str==NULL)||(strlen(str)==0)) {
			return str;
		}
		for(int i=strlen(str)-1; i>=0; --i) {
			if(str[i] == ch) {
				str[i] = '\0';
			}else
				break;
		}
		int j = 0;
		for(;j<strlen(str);++j) {
			if(str[j] != ch) {
				break;
			}
		}
		return str+j;
	}
}

