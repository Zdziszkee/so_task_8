#
# make: all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------

CLIENT_NAME = client
SERVER_NAME = server
MAIN_NAME = so_task_8

NUMBER_OF_CLIENTS = 2
MESSAGE_QUEUE_NAME = message_queue
SERVER_MESSAGE_QUEUE_NAME = server_message_queue
#----------------------
EXEC1 = $(CLIENT_NAME)
OBJS1 = $(CLIENT_NAME).o
#----------------------
EXEC2 = $(SERVER_NAME)
OBJS2 = $(SERVER_NAME).o
#----------------------
EXEC3 = $(MAIN_NAME)
OBJS3 = $(MAIN_NAME).o
#----------------------
LIBS1 = $(MESSAGE_QUEUE_NAME).c
#----------------------
#############################
COFLAGS = -Wall -std=c99 -pedantic
LDFLAGS = -Wall -std=c99 -pedantic
CO = gcc
LD = $(CO)
#############################
%.o: %.c
	$(CO) $(COFLAGS) -c $<
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2)

.PHONE: d
d: $(EXEC1)

.PHONE: i
i: $(EXEC2)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################
$(EXEC2): $(OBJS2) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################
$(EXEC3): $(OBJS3) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################
.PHONE: run
run: $(EXEC3) $(EXEC2) $(SERVER_MESSAGE_QUEUE_NAME) $(NUMBER_OF_CLIENTS)
	xterm -hold -title $(SERVER_NAME) -bg red -e ./$(EXEC2) &
	xterm -hold -title $(CLIENT_NAME)1 -bg green -e ./$(EXEC1) &
	xterm -hold -title $(CLIENT_NAME)2 -bg green -e ./$(EXEC1)

.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2)

clean:
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
