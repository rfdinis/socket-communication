all: client server

client:
	g++ client.cpp -o client -lboost_system -pthread

server:
	g++ server.cpp -o server -lboost_system -pthread -lboost_thread

clear:
	rm client
	rm server
