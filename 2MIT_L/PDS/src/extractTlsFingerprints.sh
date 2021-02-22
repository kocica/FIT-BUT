#!/bin/bash
#
# Compute JA3(S) fingerprint for each record
# 02/21/2021
#

if [ $# -ne 1 ]
then
    echo "Invalid arguments. Usage: extractTlsFingerprints.sh <source_file>"
    exit 1
fi

#
# Function which computes JA3/JA3S hash from captured TLS record
#
function computeFingerprint()
{
    c01=$(echo "$1" | awk -F';' '{printf "%s", $1}'  | tr -d '"') # Src IP
    c02=$(echo "$1" | awk -F';' '{printf "%s", $2}'  | tr -d '"') # Dst IP
    c03=$(echo "$1" | awk -F';' '{printf "%s", $3}'  | tr -d '"') # Src Port
    c04=$(echo "$1" | awk -F';' '{printf "%s", $4}'  | tr -d '"') # Dst Port
    c05=$(echo "$1" | awk -F';' '{printf "%s", $5}'  | tr -d '"') # Handshake type
    c06=$(echo "$1" | awk -F';' '{printf "%s", $6}'  | tr -d '"') # Handshake version
    c07=$(echo "$1" | awk -F';' '{printf "%s", $7}'  | tr -d '"') # Handshake ciphersuite
    c08=$(echo "$1" | awk -F';' '{printf "%s", $8}'  | tr -d '"') # Handshake extensions
    c09=$(echo "$1" | awk -F';' '{printf "%s", $9}'  | tr -d '"') # Handshake SNI
    c10=$(echo "$1" | awk -F';' '{printf "%s", $10}' | tr -d '"') # Handshake supported EC
    c11=$(echo "$1" | awk -F';' '{printf "%s", $11}' | tr -d '"') # Handshake EC type

    a=""
    for i in $(echo $c07 | sed "s/,/ /g")
    do
        a="$a$i-"
    done

    if [ -n "$a" ]; then
        a=${a::-1}
    fi

    b=""
    for i in $(echo $c08 | sed "s/,/ /g")
    do
        b="$b$i-"
    done

    if [ -n "$b" ]; then
        b=${b::-1}
    fi

    for i in $(echo $c10 | sed "s/,/ /g")
    do
        c="$c$(($i))-"
    done

    if [ -n "$c" ]; then
        c=${c::-1}
    fi

    if [ "$c05" = "1" ]; then
        ja3="$(($c06)),$a,$b,$c,$c11"
        md5ja3=`echo -n $ja3 | md5sum | awk '{ print $1 }'`
        echo "$ja3;$md5ja3"
    else
        ja3s="$(($c06)),$c07,$b"
        md5ja3s=`echo -n $ja3s | md5sum | awk '{ print $1 }'`
        echo "$ja3s;$md5ja3s"
    fi
}

#
# Function which is able to find server hello response in captured traffic
#
function lookupServerResponse()
{
    srcFile=$1
    clientHello=$2
    skipLines=$3

    while IFS= read line
    do
        if [ $(echo "$line" | awk -F';' '{printf "%s", $5}'  | tr -d '"') = "2" ] &&
           [ $(echo "$line" | awk -F';' '{printf "%s", $1}'  | tr -d '"') = $(echo "$clientHello" | awk -F';' '{printf "%s", $2}'  | tr -d '"') ] &&
           [ $(echo "$line" | awk -F';' '{printf "%s", $2}'  | tr -d '"') = $(echo "$clientHello" | awk -F';' '{printf "%s", $1}'  | tr -d '"') ] &&
           [ $(echo "$line" | awk -F';' '{printf "%s", $3}'  | tr -d '"') = $(echo "$clientHello" | awk -F';' '{printf "%s", $4}'  | tr -d '"') ] &&
           [ $(echo "$line" | awk -F';' '{printf "%s", $4}'  | tr -d '"') = $(echo "$clientHello" | awk -F';' '{printf "%s", $3}'  | tr -d '"') ] &&
           [ $(echo "$line" | awk -F';' '{printf "%s", $6}'  | tr -d '"') = $(echo "$clientHello" | awk -F';' '{printf "%s", $6}'  | tr -d '"') ]; then
            echo $line
        fi
    done <<< $(tail -n +$(($skipLines+1)) "$srcFile")
}

#
# For each client hello lookup server hello response, compute their JA3/JA3S hashes and store them with SNI to the database
#
function main()
{
    srcFile=$1

    declare -i i=0
    while read -r line
    do
        i=$(($i+1))

        if [ $(echo "$line" | awk -F';' '{printf "%s", $5}'  | tr -d '"') = "1" ]; then
            clientHello=$line
            serverHello=$(lookupServerResponse $srcFile $clientHello $i)

            md5ja3=$(computeFingerprint $clientHello)
            md5ja3s=$(computeFingerprint $serverHello)

            sni=$(echo "$line" | awk -F';' '{printf "%s", $9}'  | tr -d '"')

            echo "$sni;$md5ja3;$md5ja3s"
        fi
    done < "$srcFile"
}

main $1
