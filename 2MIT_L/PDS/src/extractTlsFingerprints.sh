#!/bin/bash
#
# Compute JA3(S) fingerprint for each record
# 02/21/2021
#

if [ $# -ne 2 ]
then
    echo "Invalid arguments. Usage: extractTlsFingerprints.sh <path_to_data> <path_to_whitelists>"
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
    c10=$(echo "$1" | awk -F';' '{printf "%s", $10}' | tr -d '"') # Handshake supported groups (former EC)
    c11=$(echo "$1" | awk -F';' '{printf "%s", $11}' | tr -d '"') # Handshake EC type

    declare -A grease=(
      [2570]=1
      [6682]=1
      [10794]=1
      [14906]=1
      [19018]=1
      [23130]=1
      [27242]=1
      [31354]=1
      [35466]=1
      [39578]=1
      [43690]=1
      [47802]=1
      [51914]=1
      [56026]=1
      [60138]=1
      [64250]=1
      [65281]=1
    )

    a=""
    for i in $(echo $c07 | sed "s/,/ /g")
    do
        [[ -n "${grease[$i]}" ]] || a="$a$i-"
    done

    if [ -n "$a" ]; then
        a=${a::-1}
    fi

    b=""
    for i in $(echo $c08 | sed "s/,/ /g")
    do
        [[ -n "${grease[$i]}" ]] || [[ "$i" = "21" ]] || b="$b$i-"
    done

    if [ -n "$b" ]; then
        b=${b::-1}
    fi

    for i in $(echo $c10 | sed "s/,/ /g")
    do
        [[ -n "${grease[$(($i))]}" ]] || c="$c$(($i))-"
    done

    if [ -n "$c" ]; then
        c=${c::-1}
    fi

    if [ "$c05" = "1" ]; then
        ja3="$(($c06)),$a,$b,$c,$c11"
        md5ja3=`echo -n $ja3 | md5sum | awk '{ print $1 }'`
        echo "$md5ja3"
    else
        ja3s="$(($c06)),$a,$b"
        md5ja3s=`echo -n $ja3s | md5sum | awk '{ print $1 }'`
        echo "$md5ja3s"
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
function extractTlsFingerprints()
{
    srcFile=$1
    whitelistsDir=$2

    declare -i i=0
    declare -a whitelistArr

    whitelist=${srcFile##*/}
    whitelist=$(echo "$whitelist" | cut -f 1 -d '.')
    whitelist="$whitelistsDir/$whitelist.txt"

    while read -r keyword
    do
        whitelistArr+=($keyword)
    done < $whitelist

    while read -r line
    do
        i=$(($i+1))

        if [ $(echo "$line" | awk -F';' '{printf "%s", $5}'  | tr -d '"') = "1" ]; then
            sni=$(echo "$line" | awk -F';' '{printf "%s", $9}'  | tr -d '"')

            pass=false
            for whitelistSni in "${whitelistArr[@]}"
            do
                if [[ $sni == *"$whitelistSni"* ]]; then
                    pass=true
                    break
                fi
            done

            if [ "$pass" = true ]; then
                clientHello=$line
                serverHello=$(lookupServerResponse $srcFile $clientHello $i)

                md5ja3=$(computeFingerprint $clientHello)
                md5ja3s=$(computeFingerprint $serverHello)

                echo "$md5ja3;$md5ja3s;$sni;${srcFile##*/}"
            fi
        fi
    done <<< $(tail -n +2 "$srcFile") # Skip header
}

sourceDir=$1
whitelistsDir=$2

echo "JA3;JA3S;SNI;File"

for filename in $sourceDir/*.csv; do
    extractTlsFingerprints $filename $whitelistsDir
done
