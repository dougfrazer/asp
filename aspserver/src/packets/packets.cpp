

void RegisterPackets()
{
    PACKET_HANDLER* Handler = PacketHandler_GetFirst();
    while(Handler != null) {
        Handler->Register();
        Handler = PacketHandler_GetNext(Handler);
    }
}
