.PHONY:all
all:Client Server
Server:Server.cc
	g++ -o $@ $^ -std=c++17
Client: Client.cc
	g++ -o $@ $^ -std=c++17
.PHONY:clean
clean:
	rm Client Server
