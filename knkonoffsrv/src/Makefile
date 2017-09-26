# Build and create the binary:

knkonoffsrv: knkonoffsrv.o powerctl.o tcpsrv.o msgprotocol.o
	$(CC) $(LDFLAGS) knkonoffsrv.o powerctl.o tcpsrv.o msgprotocol.o -o knkonoffsrv

knkonoffsrv.o: knkonoffsrv.c
	$(CC) $(CFLAGS) -c knkonoffsrv.c

powerctl.o: powerctl.c
	$(CC) $(CFLAGS) -c powerctl.c

tcpsrv.o: tcpsrv.c
	$(CC) $(CFLAGS) -c tcpsrv.c

msgprotocol.o: msgprotocol.c
	$(CC) $(CFLAGS) -c msgprotocol.c

# Clean up - Remove object files and binary:
clean:
	rm *.o knkonoffsrv
