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

  // Connect nodes in a ring
  for (uint32_t i = 0; i < 14; i++) {
    p2p.Install (NodeContainer (nodes.Get (i), nodes.Get (i + 1)));
  }
  p2p.Install (NodeContainer (nodes.Get (14), nodes.Get (0)));

  // Mobility: circular layout
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator>();
  double radius = 60.0;

  for (uint32_t i = 0; i < 15; i++) {
    double angle = 2 * M_PI * i / 15;
    pos->Add (Vector (radius * cos(angle), radius * sin(angle), 0));
  }

  mobility.SetPositionAllocator (pos);
  mobility.Install (nodes);

  AnimationInterface anim ("ring.xml");

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
