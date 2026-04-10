#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"
#include <vector>

using namespace ns3;

int main(int argc, char *argv[])
{
    Time::SetResolution(Time::NS);

    // ---- TREE STRUCTURE ----
    const uint32_t nRoot = 1;
    const uint32_t nLevel1 = 3;
    const uint32_t nLevel2PerL1 = 5; // 3 * 5 = 15
    const uint32_t totalNodes = nRoot + nLevel1 + (nLevel1 * nLevel2PerL1);

    NodeContainer nodes;
    nodes.Create(totalNodes);

    InternetStackHelper internet;
    internet.Install(nodes);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.4.0.0", "255.255.255.0");

    uint32_t index = 0;

    // Root node
    Ptr<Node> root = nodes.Get(index++);

    // Level 1 nodes
    std::vector<Ptr<Node>> level1;
    for (uint32_t i = 0; i < nLevel1; ++i)
    {
        Ptr<Node> l1 = nodes.Get(index++);
        level1.push_back(l1);

        NetDeviceContainer d = p2p.Install(NodeContainer(root, l1));
        ipv4.Assign(d);
        ipv4.NewNetwork();
    }

    // Level 2 nodes
    for (uint32_t i = 0; i < nLevel1; ++i)
    {
        for (uint32_t j = 0; j < nLevel2PerL1; ++j)
        {
            Ptr<Node> l2 = nodes.Get(index++);
            NetDeviceContainer d = p2p.Install(NodeContainer(level1[i], l2));
            ipv4.Assign(d);
            ipv4.NewNetwork();
        }
    }

    // ---- NETANIM OUTPUT ----
    AnimationInterface anim("outputs/topology4_tree.xml");

    // ---- CLEAN TREE LAYOUT ----

    // Root position
    anim.SetConstantPosition(root, 50.0, 90.0);

    // Level 1 positioning
    double l1StartX = 30.0;
    double l1Gap = 20.0;

    for (uint32_t i = 0; i < nLevel1; ++i)
    {
        anim.SetConstantPosition(level1[i],
                                 l1StartX + i * l1Gap,
                                 65.0);
    }

    // Level 2 positioning (grouped under parents)
    uint32_t l2Index = 1 + nLevel1;
    double l2Gap = 4.0;

    for (uint32_t i = 0; i < nLevel1; ++i)
    {
        double parentX = l1StartX + i * l1Gap;
        double childStartX = parentX - (nLevel2PerL1 - 1) * l2Gap / 2;

        for (uint32_t j = 0; j < nLevel2PerL1; ++j)
        {
            anim.SetConstantPosition(nodes.Get(l2Index++),
                                     childStartX + j * l2Gap,
                                     35.0);
        }
    }

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}