from scapy.all import PcapReader, IP, TCP, UDP, ICMP, ARP
from collections import Counter

pcap_path = r"C:\Users\Student\Documents\traffic_capture.pcapng"

src_ips = set()
dst_ips = set()
protocols = Counter()
packet_count = 0

with PcapReader(pcap_path) as packets:
    for pkt in packets:
        packet_count += 1

        if pkt.haslayer(IP):
            src_ips.add(pkt[IP].src)
            dst_ips.add(pkt[IP].dst)

            if pkt.haslayer(TCP):
                protocols["TCP"] += 1
            elif pkt.haslayer(UDP):
                protocols["UDP"] += 1
            elif pkt.haslayer(ICMP):
                protocols["ICMP"] += 1
            else:
                protocols["IP_Other"] += 1

        elif pkt.haslayer(ARP):
            src_ips.add(pkt[ARP].psrc)
            dst_ips.add(pkt[ARP].pdst)
            protocols["ARP"] += 1

print("Total number of packets:", packet_count)
print("Number of Source IPs:", len(src_ips))
print("Number of Destination IPs:", len(dst_ips))
print("Number of protocols used:", len(protocols))

print("\nPackets under each protocol:")
for proto, count in protocols.items():
    print(f"{proto}: {count} packets")