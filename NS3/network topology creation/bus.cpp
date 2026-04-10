#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
	
using namespace ns3;

int main () {

  NodeContainer nodes;
  nodes.Create (15);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  csma.Install (nodes);

  InternetStackHelper internet;
  internet.Install (nodes);

  // Mobility for clean layout
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator>();
  for (uint32_t i = 0; i < 15; i++) {
    pos->Add (Vector (i * 20.0, 0.0, 0.0));
  }

  mobility.SetPositionAllocator (pos);
  mobility.Install (nodes);

  AnimationInterface anim ("bus.xml");

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
