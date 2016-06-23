DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = aoa_process

PROJECT_SOURCEFILES = aoa_plug.c

all: $(CONTIKI_PROJECT)

CONTIKI = ../contiki
include $(CONTIKI)/Makefile.include
