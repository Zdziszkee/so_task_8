#
# make: all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------

CLIENT_NAME = client
SERVER_NAME = server

MESSAGE_QUEUE_NAME = message_queue
SERVER_MESSAGE_QUEUE_NAME = /server_message_queue
#----------------------
EXEC1 = $(CLIENT_NAME).x
OBJS1 = $(CLIENT_NAME).o
#----------------------
EXEC2 = $(SERVER_NAME).x
OBJS2 = $(SERVER_NAME).o
#----------------------

#----------------------
LIBS1 = $(MESSAGE_QUEUE_NAME).c
#----------------------
#############################
COFLAGS = -lpthread -lrt -Wall -O
LDFLAGS = -lpthread -lrt -Wall -O
CO = gcc
LD = $(CO)
#############################
%.o: %.c
	$(CO) $(COFLAGS) -c $<
#############################


.PHONE: d
d: $(EXEC1)

.PHONE: i
i: $(EXEC2)

.PHONY: all
all: $(EXEC1) $(EXEC2)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################
$(EXEC2): $(OBJS2) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################

.PHONE: run
run: $(EXEC2) $(EXEC1)
	xterm -hold -title $(SERVER_NAME) -bg red -e ./$(EXEC2) $(SERVER_MESSAGE_QUEUE_NAME) &
	sleep 1
	xterm -hold -title $(CLIENT_NAME)1 -bg green -e ./$(EXEC1) $(SERVER_MESSAGE_QUEUE_NAME) &
	xterm -hold -title $(CLIENT_NAME)2 -bg green -e ./$(EXEC1) $(SERVER_MESSAGE_QUEUE_NAME)

.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2) $(EXEC3)

clean:
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
