
CC= g++
LD= g++
CFLAGS= -I. -I/home/unibss/soft/unibss-5.0.1/include -O2 -Wall -m32 -DNEW_NAMESPACE=1
#-Wimplicit-function-declaration -Wuninitialized -Wnon-literal-null-conversion -Wunused-const-variable -Wreturn-type
LDFLAGS= -m32 -lm -lpthread -L/home/unibss/soft/unibss-5.0.1/lib -laibase -laiodbc -laiodbc_uac -laiuuid
src_LOGGER=browsers.o \
		   commons.o \
		   error.o \
		   gdns.o \
		   app_logger.o \
		   logger.o \
		   logger_handle.o \
		   opesys.o \
		   options.o \
		   output_txt.o \
		   output_db.o \
		   parser.o \
		   settings.o \
		   util.o \
		   xmalloc.o
src_STAT= stat_base.o \
		  stat_app_user.o \
		  stat_collect_total.o \
		  stat_collect_detail.o \
		  stat_comment_total.o \
		  stat_comment_detail.o \
		  stat_login.o \
		  stat_main_visit.o \
		  stat_visit_detail.o \
		  stat_scan_user.o \
		  stat_scan_program_user.o \
		  stat_vote_total.o \
		  logger.o \
		  logger_handle.o \
		  app_stat.o
bin_LOGGER= app_logger
bin_STAT= app_stat
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(bin_LOGGER): $(src_LOGGER)
	$(LD) -o $@ $(LDFLAGS) $(src_LOGGER)
$(bin_STAT): $(src_STAT)
	$(LD) -o $@ $(LDFLAGS) $(src_STAT)
.SUFFIXES: .c .d
.PHONY: all clean
all: $(bin_LOGGER) $(bin_STAT)
install:
	cp -f app_logger $(AIOSS_HOME)/bin
	cp -f app_stat $(AIOSS_HOME)/bin
clean:
	rm -f *.o app_logger app_stat

