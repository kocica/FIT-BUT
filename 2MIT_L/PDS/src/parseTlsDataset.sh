#!/bin/bash
#
# Parse tls packers from pcap file
# 02/21/2021
#

if [ $# -ne 3 ]
then
    echo "Invalid arguments. Usage: parseTlsDataset.sh <path_to_dataset> <path_to_save> <delimeter>"
    exit 1
fi

sourceDir=$1
targetDir=$2
delimeter=$3

for filename in $sourceDir/*.pcapng; do
    # Remove path
    target=${filename##*/}
    # Remove extension (and application counter, e.g. appname_1.pcapng => appname)
    target=$(echo "$target" | cut -f 1 -d $delimeter)
    # Create new path + filename (e.g. appname.csv)
    target="$targetDir/$target.csv"

    # Create csv header
    # echo "Source IP;Destination IP;Source port;Destination port;Handshake type;Handshake version;Handshake ciphersuite;Handshake extensions;Handshake SNI;Handshake supported EC;Handshake EC type" > $target

    # Extract relevant TCP & TLS handshake data
    tshark -r $filename -T fields -E separator=";" -e ip.src \
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
                                                   -e ssl.handshake.certificate \
                                                   -R "ssl.handshake.type==1 or ssl.handshake.type==2 or ssl.handshake.certificate" -2 \
                                                   >> $target
done
