#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include <cmath>

using namespace ns3;

int main () {
  NodeContainer nodes;
  nodes.Create (15);

  InternetStackHelper internet;
  internet.Install (nodes);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  for (uint32_t i = 1; i < 15; i++) {
    p2p.Install (NodeContainer (nodes.Get (0), nodes.Get (i)));
  }

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator>();
  pos->Add (Vector (0, 0, 0));

  double r = 60;
  for (uint32_t i = 1; i < 15; i++) {
    double a = 2 * M_PI * i / 14;
    pos->Add (Vector (r * cos(a), r * sin(a), 0));
  }

  mobility.SetPositionAllocator (pos);
  mobility.Install (nodes);

  AnimationInterface anim ("star.xml");

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
