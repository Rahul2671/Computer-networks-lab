#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <vector>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("UnifiedChatApp");

class ChatServerApp : public Application {
public:
    ChatServerApp() : m_socket(0), m_port(9000) {}

    void Setup(uint16_t port) { m_port = port; }

private:
    virtual void StartApplication(void) {
        Ptr<SocketFactory> socketFactory =
            GetNode()->GetObject<SocketFactory>(UdpSocketFactory::GetTypeId());

        m_socket = socketFactory->CreateSocket();
        m_socket->Bind(InetSocketAddress(Ipv4Address::GetAny(), m_port));

        m_socket->SetRecvCallback(MakeCallback(&ChatServerApp::HandleRead, this));
    }

    void HandleRead(Ptr<Socket> socket) {
        Ptr<Packet> packet;
        Address from;

        while ((packet = socket->RecvFrom(from))) {
            std::stringstream msg;
            msg << "ACK " << Simulator::Now().GetSeconds();

            Ptr<Packet> responsePkt =
                Create<Packet>((uint8_t*)msg.str().c_str(), msg.str().length());

            socket->SendTo(responsePkt, 0, from);
        }
    }

    Ptr<Socket> m_socket;
    uint16_t m_port;
};

class ChatClientApp : public Application {
public:
    ChatClientApp() : m_socket(0) {}

    void Setup(Address address, Time interval) {
        m_peer = address;
        m_interval = interval;
    }

private:
    virtual void StartApplication(void) {
        m_socket = Socket::CreateSocket(GetNode(), UdpSocketFactory::GetTypeId());
        m_socket->Connect(m_peer);

        SendChat();
    }

    void SendChat() {
        Ptr<Packet> p = Create<Packet>(100);
        m_socket->Send(p);

        Simulator::Schedule(m_interval, &ChatClientApp::SendChat, this);
    }

    Ptr<Socket> m_socket;
    Address m_peer;
    Time m_interval;
};

int main()
{
    NodeContainer nodes;
    nodes.Create(3);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer d1 = p2p.Install(nodes.Get(1), nodes.Get(0));
    NetDeviceContainer d2 = p2p.Install(nodes.Get(2), nodes.Get(0));

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper addr;

    addr.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i1 = addr.Assign(d1);

    addr.SetBase("10.2.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i2 = addr.Assign(d2);

    Ptr<ChatServerApp> server = CreateObject<ChatServerApp>();
    server->Setup(9000);
    nodes.Get(0)->AddApplication(server);

    Ptr<ChatClientApp> c1 = CreateObject<ChatClientApp>();
    c1->Setup(InetSocketAddress(i1.GetAddress(1),9000), Seconds(2.0));
    nodes.Get(1)->AddApplication(c1);

    Ptr<ChatClientApp> c2 = CreateObject<ChatClientApp>();
    c2->Setup(InetSocketAddress(i2.GetAddress(1),9000), Seconds(3.0));
    nodes.Get(2)->AddApplication(c2);

    Simulator::Stop(Seconds(15));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}