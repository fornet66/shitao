
#ifndef __FILE_LOCK_H__
#define __FILE_LOCK_H__
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <aibase/compiler.h>
namespace ibs {
	class filelock {
		public:
			filelock(): m_handle(-1) { }
			~filelock() {unlock();}
			bool lock(const char* file_name, int flag=O_RDWR|O_CREAT) {
				if(m_handle==-1) {
					m_handle=open(file_name,flag,0666);
					if (m_handle==-1) 
						return false;
					struct flock ret ;
					ret.l_type = F_WRLCK ;
					ret.l_start = 0 ;
					ret.l_whence = SEEK_SET ;
					ret.l_len = 0 ;
					if(fcntl(m_handle,F_SETLK ,&ret)<0) {
						close(m_handle);
						m_handle=-1;
						return false;
					}
					else {
						return true;
					}
				}
				return false;
			}
			bool unlock() {
				if(m_handle!=-1) {
					struct flock ret ;
					ret.l_type = F_UNLCK ;
					ret.l_start = 0 ;
					ret.l_whence = SEEK_SET ;
					ret.l_len = 0 ;
					fcntl(m_handle,F_SETLK ,&ret);
					close(m_handle);
				}
				m_handle = -1;
				return true;
			}
		private:
			int m_handle;
	};
}
#endif	// __FILE_LOCK_H__

