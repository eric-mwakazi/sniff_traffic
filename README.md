# sniff_traffic
Simple network traffic sniffer implemented in C. This program captures and analyzes network packets on a specified network interface.
## Features
- Captures Ethernet frames and extracts MAC addresses.
- Analyzes IP packets and extracts source and destination IP addresses.
- Analyzes TCP and UDP packets to extract source and destination ports.
## Usage
1. Clone the repository and navigate to the `sniff_traffic` directory:
   ```bash
   git clone <repository_url>
   cd sniff_traffic
   ```
2. Compile the program using `make`:
   ```bash
   make .
   ```
3. Run the program with root privileges to capture packets:
   ```bash
   sudo ./output/sniff_traffic <network_interface>
   ```
   or if one nic is available, you can run it without specifying the interface:
   ```bash
    sudo ./output/sniff_traffic
    ```
   Replace `<network_interface>` with the name of the network interface you want to sniff (e.g., eth0, wlan0).
4. The program will display captured packet information in the terminal.

