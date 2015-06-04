
#include <signal.h>
#include <unistd.h>
#include <aiosp/ACE.h>
#include <aibase/Exceptions.h>
#include "pga.h"
using namespace ibs;
PGA* pga=0;
static void signalDeal(int signo) {
	if (signo == SIGINT || signo == SIGTERM) {
		if(pga) {
			pga->stop();
			OSS_DEBUG((LM_INFO,"receive exit signal %d\n", signo));
		}
		else {
			OSS_DEBUG((LM_ERROR,"receive exit signal %d, can not stop\n", signo));
		}
	}
	return;
}

int main(int argc, char **argv) {
	if (argc!=2) {
		printf("usage:\n");
		printf("  ibs_rate home_dir\n");
		printf("Example:\n");
		printf("  ibs_rate /ibs_home\n");
		printf("\n");
		return 0;
	}
	if (signal(SIGTERM,signalDeal)==SIG_ERR || signal(SIGINT, signalDeal)==SIG_ERR) {
		fprintf(stderr,"signal handle error\n");
		exit(-1);
	}
	ACE::daemonize("/tmp");
	string logFile = "log/ibs.log";
	Logger logger("IBS", logFile);
	OSS_DEBUG((LM_STARTUP,"ibs startup...\n"));
	try {
		OSS_DEBUG((LM_INFO, "ibs home dir is %s\n", argv[1]));
		pga = new PGA(string(argv[1]));
		pga->start();
		delete pga;
	}
	catch (SQLException& e) {
		OSS_DEBUG((LM_ERROR, "SQLException(%s)\n", e.what()));
		OSS_DEBUG((LM_SHUTDOWN, "abort process\n"));
		return -1;
	}
	catch (OSSException& e) {
		OSS_DEBUG((LM_ERROR, "OSSException(%s)\n", e.what()));
		OSS_DEBUG((LM_SHUTDOWN, "abort process\n"));
		return -1;
	}
	catch (exception& e) {
		OSS_DEBUG((LM_ERROR, "std exception(%s)\n", e.what()));
		return -1;
    }
	catch (...) {
		OSS_DEBUG((LM_ERROR, "unknown error\n"));
		return -1;
    }
	OSS_DEBUG((LM_SHUTDOWN,"ibs shutdown ...\n"));
	return 0;
}

