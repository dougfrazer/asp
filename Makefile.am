AM_CXXFLAGS=-Wall -I../.. -I. -g

noinst_LIBRARIES = libasp.a
libasp_a_SOURCES = library/ASPLib.cpp library/HashMap.cpp library/MurmurHash.cpp library/network/PacketHandler.cpp library/network/PacketStream.cpp library/MemoryPool.cpp 

bin_PROGRAMS=bin/aspclient bin/aspserver

bin_aspclient_SOURCES=aspclient/src/actor.cpp aspclient/src/animation.cpp aspclient/src/player.cpp aspclient/src/camera.cpp aspclient/src/keyboard.cpp aspclient/src/main.cpp aspclient/src/network.cpp aspclient/src/world.cpp aspclient/src/packets/direction_ack.cpp aspclient/src/packets/keepalive.cpp aspclient/src/packets/login.cpp aspclient/src/packets/login_ack.cpp

bin_aspserver_SOURCES=aspserver/src/client.cpp aspserver/src/main.cpp aspserver/src/network.cpp aspserver/src/world.cpp aspserver/src/packets/direction.cpp aspserver/src/packets/direction_ack.cpp aspserver/src/packets/keepalive.cpp aspserver/src/packets/login.cpp aspserver/src/packets/login_ack.cpp

bin_aspclient_LDADD = libasp.a
bin_aspserver_LDADD = libasp.a
