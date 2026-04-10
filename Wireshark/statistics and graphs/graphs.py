from scapy.all import *
import matplotlib.pyplot as plt

packets = rdpcap("wireshark_stats.pcapng")

times = [pkt.time for pkt in packets]
sizes = [len(pkt) for pkt in packets]

# Total packets over time
plt.plot(times, range(len(times)))
plt.title("Total Packets Over Time")
plt.xlabel("Time")
plt.ylabel("Packets")
plt.show()

# TCP packets over time
tcp = [pkt for pkt in packets if TCP in pkt]
plt.plot([pkt.time for pkt in tcp], range(len(tcp)))
plt.title("TCP Packets Over Time")
plt.show()

# UDP packets over time
udp = [pkt for pkt in packets if UDP in pkt]
plt.plot([pkt.time for pkt in udp], range(len(udp)))
plt.title("UDP Packets Over Time")
plt.show()

# ICMP packets over time
icmp = [pkt for pkt in packets if ICMP in pkt]
plt.plot([pkt.time for pkt in icmp], range(len(icmp)))
plt.title("ICMP Packets Over Time")
plt.show()

# Packet size over time
plt.plot(times, sizes)
plt.title("Packet Size Over Time")
plt.xlabel("Time")
plt.ylabel("Bytes")
plt.show()