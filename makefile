default : tinyServer tinyThreadServer

objects = main.o rio/rio.o tiny/tinyweb.o socket/ocs.o common/common.o sbuf/sbuf.o threadpoll/threadpool.o

objectsthread = mainthread.o rio/rio.o tiny/tinyweb.o socket/ocs.o common/common.o sbuf/sbuf.o threadpoll/threadpool.o

clean : 
	rm -rf ${objects} ${objectsthread}

tinyServer : $(objects)
	cc -o tinyServer $(objects) defs.h pub.h -lpthread

tinyThreadServer : $(objectsthread)
	cc -o tinyThreadServer $(objectsthread) defs.h pub.h -lpthread
	
main.o : pub.h defs.h

mainthread.o : pub.h defs.h

rio.o : rio.h ../defs.h ../pub.h

tinyweb.o : tinyweb.h ../defs.h ../pub.h

ocs.o : ocs.h ../defs.h ../pub.h

common.o : common.h ../defs.h ../pub.h

sbuf: sbuf.h ../defs.h ../pub.h

threadpool.o: threadpool.h 

