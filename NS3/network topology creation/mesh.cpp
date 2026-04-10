#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main ()
{
  NodeContainer nodes;
  nodes.Create (15);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  for (uint32_t i = 0; i < 15; i++)
  {
    for (uint32_t j = i + 1; j < 15; j++)
    {
      NodeContainer pair (nodes.Get (i), nodes.Get (j));
      p2p.Install (pair);
    }
  }

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator> ();

  pos->Add (Vector (0, 0, 0));
  pos->Add (Vector (20, 0, 0));
  pos->Add (Vector (40, 0, 0));
  pos->Add (Vector (60, 0, 0));
  pos->Add (Vector (80, 0, 0));

  pos->Add (Vector (0, 20, 0));
  pos->Add (Vector (20, 20, 0));
  pos->Add (Vector (40, 20, 0));
  pos->Add (Vector (60, 20, 0));
  pos->Add (Vector (80, 20, 0));

  pos->Add (Vector (0, 40, 0));
  pos->Add (Vector (20, 40, 0));
  pos->Add (Vector (40, 40, 0));
  pos->Add (Vector (60, 40, 0));
  pos->Add (Vector (80, 40, 0));

  mobility.SetPositionAllocator (pos);
  mobility.Install (nodes);

  AnimationInterface anim ("mesh.xml");

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}



