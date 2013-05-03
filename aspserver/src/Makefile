ASP=/home/doug/asp

all:
	g++ -g -o aspserver -Ipackets/ -I. -I$(ASP) -I$(ASP)/library/ *.cpp packets/*.cpp -L../../library/ -lasp
