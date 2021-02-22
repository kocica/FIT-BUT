#!/bin/bash
#
# Evaluate extracted fingerprints on database and calculate statistics
# 02/22/2021
#

if [ $# -ne 2 ]
then
    echo "Invalid arguments. Usage: evalTlsFingerprints.sh <path_to_fingerprints> <path_to_database>"
    exit 1
fi

#
# Compare retrieved fingerprint to the database of finerprints and report matching fingerprint
#
function compareToDatabase
{
    fingerprint=$1
    database=$2

    R=`tput setaf 1`
    G=`tput setaf 2`
    N=`tput sgr0`

    while read -r dbFingerprint
    do
        a=$(echo "$fingerprint" | awk -F';' '{printf "%s", $3}'  | tr -d '"')
        b=$(echo "$dbFingerprint" | awk -F';' '{printf "%s", $3}'  | tr -d '"')

#       if [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $1}'  | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $1}'  | tr -d '"') ]; then
        if [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $2}'  | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $2}'  | tr -d '"') ]; then
            if [ "$a" = "$b" ]; then
                echo "[${G}OK${N}]  True  positive: $a | $b"
            else
                echo "[${R}NOK${N}] False positive: $a | $b"
            fi
        else
            if [ "$a" = "$b" ]; then
                echo "[${R}NOK${N}] False negative: $a | $b"
            else
                echo "[${G}OK${N}]  True  negative: $a | $b"
            fi
        fi
    done <<< $(tail -n +2 "$database") # Skip header
}

#
# For each fingerprint do the comparison with the whole database and calculate statistics
#
function main()
{
    fingerprints=$1
    database=$2

    while read -r fingerprint
    do
        compareToDatabase $fingerprint $database
    done <<< $(tail -n +2 "$fingerprints") # Skip header
}

main $1 $2
