
#ifndef __IO_PUBLIC_H__
#define __IO_PUBLIC_H__
#include <aibase/compiler.h>
namespace ibs {
	bool is_dir(const char* path);
	char upper(char c);
	char* trim(char* str, char ch=' ');
}
#endif	// __IO_PUBLIC_H__

