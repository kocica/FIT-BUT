#!/bin/bash
#
# Parse tls packers from pcap file
# 02/21/2021
#

if [ $# -ne 1 ]
then
    echo "Invalid arguments. Usage: parseTlsPcap.sh <pcap_file>"
    exit 1
fi

tshark -r $1 -T fields -E separator=";" -e ip.src \
                                        -e ip.dst \
                                        -e tcp.srcport \
                                        -e tcp.dstport \
                                        -e ssl.handshake.type \
                                        -e ssl.handshake.version \
                                        -e ssl.handshake.ciphersuite \
                                        -e ssl.handshake.extension.type \
                                        -e ssl.handshake.extensions_server_name \
                                        -e ssl.handshake.extensions_supported_group \
                                        -e ssl.handshake.extensions_ec_point_format \
                                        -R "ssl.handshake.type==1 or ssl.handshake.type==2" -2
