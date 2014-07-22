objects := $(patsubst %.c,%.o,$(wildcard *.c))

#a.out: libstubs.a sample_client.o sample_server.o
	#g++ sample_client.o sample_server.o -L. -lstubs -o a.out

libstubs.a: server_stub.cpp client_stub.cpp ece454rpc_types.h
	g++ -c server_stub.cpp client_stub.cpp
	ar r libstubs.a server_stub.o client_stub.o

$(objects): %.o: %.cpp ece454rpc_types.h
	g++ $< -o $@
	
clean:
	rm -rf core a.out *.o *.a
	

