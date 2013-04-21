all:
	g++ -Wall -shared -fPIC -o libasp.so -I. -Inetwork/ ASPLib.cpp HashMap.cpp MurmurHash.cpp network/*.cpp network/packets/*.h
clean:
	g++ -Wall -shared -fPIC -o libasp.so -I. -Inetwork/ ASPLib.cpp HashMap.cpp MurmurHash.cpp network/*.cpp network/packets/*.h
debug:
	g++ -g -Wall -shared -fPIC -o libasp.so -I. -Inetwork/ ASPLib.cpp HashMap.cpp MurmurHash.cpp network/*.cpp network/packets/*.h
