#!/bin/bash
#
# Evaluate extracted fingerprints on database and calculate statistics
# 02/22/2021
#

if [ $# -lt 2 ]
then
    echo "Invalid arguments. Usage: evalTlsFingerprints.sh <path_to_fingerprints> <path_to_database> [-q]"
    exit 1
fi

TP=0
FP=0
TN=0
FN=0

#
# Compare retrieved fingerprint to the database of finerprints and report matching fingerprint
#
function compareToDatabase
{
    fingerprint=$1
    database=$2
    quiet=$3
    confMatrix=$4

    R=`tput setaf 1`
    G=`tput setaf 2`
    N=`tput sgr0`

    found=false

    while read -r dbFingerprint
    do
        a=$(echo "$fingerprint" | awk -F';' '{printf "%s", $5}'  | tr -d '"')
        b=$(echo "$dbFingerprint" | awk -F';' '{printf "%s", $5}'  | tr -d '"')

        if [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $1}'  | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $1}'  | tr -d '"') ] &&     # JA3
           [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $2}'  | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $2}'  | tr -d '"') ] &&     # JA3S
           [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $4}'  | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $4}'  | tr -d '"') ]; then  # Cert
            if [ "$a" = "$b" ]; then
                [ "$quiet" == "-q" ] || echo "[${G}OK${N}]  True  positive: $a | $b"
                TP=$(($TP+1))
            else
                [ "$quiet" == "-q" ] || echo "[${R}NOK${N}] False positive: $a | $b"
                FP=$(($FP+1))
            fi
            confMatrix[$a,$b]=$((${confMatrix[$a,$b]} + 1))
            found=true
            break
        fi
    done <<< $(tail -n +2 "$database") # Skip header

    if [ "$found" = false ]; then
        if [ "$a" = "UNKNOWN" ]; then
            [ "$quiet" == "-q" ] || echo "[${G}OK${N}]  True  negative: $a | ---"
            TN=$(($TN+1))
        else
            [ "$quiet" == "-q" ] || echo "[${R}NOK${N}] False negative: $a | ---"
            FN=$(($FN+1))
        fi
        confMatrix[$a,UNKNOWN]=$((${confMatrix[$a,UNKNOWN]} + 1))
    fi
}

#
# For each fingerprint do the comparison with the whole database and calculate statistics
#
function main()
{
    fingerprints=$1
    database=$2
    quiet=$3

    declare -a apps=("boltfood" "damejidlo" "discord" "pinterest" "reddit" "rossmanclub" "signal" "twitch" "zalando" "zonky" "UNKNOWN")
    declare -A confMatrix

    for i in "${apps[@]}"
    do
        for j in "${apps[@]}"
        do
            confMatrix[$i,$j]=0
        done
    done

    while read -r fingerprint
    do
        compareToDatabase $fingerprint $database $quiet $confMatrix
    done <<< $(tail -n +2 "$fingerprints") # Skip header

    echo
    printf "%12s" "Conf. matrix:"
    for i in "${apps[@]}"
    do
        printf "%12s" $i
    done
    echo

    for i in "${apps[@]}"
    do
        printf "%12s" $i
        for j in "${apps[@]}"
        do
            printf "%12s" ${confMatrix[$i,$j]}
        done
        echo
    done
    echo

    echo
    echo "                 Predicted"
    echo "                 +-----------------+-----------------+"
    echo "                 | Negative        | Positive        |"
    echo "      +----------+-----------------+-----------------+"
    printf "GT    | Negative | (TN) %10d | (FP) %10d |\n" $TN $FP
    echo "      +----------+-----------------+-----------------+"
    printf "      | Positive | (FN) %10d | (TP) %10d |\n" $FN $TP
    echo "      +----------+-----------------+-----------------+"
    echo

    acc=$(echo "($TP + $TN) / ($TP + $TN + $FP + $FN)" | bc -l)
    prc=$(echo "$TP / ($TP + $FP)" | bc -l)
    rec=$(echo "$TP / ($TP + $FN)" | bc -l)

    echo "Accuracy:        $acc"
    echo "Precision:       $prc"
    echo "Recall:          $rec"
    echo
}

main $1 $2 $3
