#!/bin/bash
#
# Evaluate extracted fingerprints on database and calculate statistics
# 02/22/2021
#

if [ $# -lt 2 ]
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
    declare -a database=("${!2}")
    confMatrix=$3

    found=false

    for dbFingerprint in "${database[@]}"
    do
        if [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $1}' | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $1}' | tr -d '"') ] &&        # JA3
           [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $2}' | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $2}' | tr -d '"') ] &&        # JA3S
           [ $(echo "$fingerprint" | awk -F';' '{printf "%s", $3}' | tr -d '"') = $(echo "$dbFingerprint" | awk -F';' '{printf "%s", $3}' | tr -d '"') ]; then     # Cert
            a=$(echo "$fingerprint" | awk -F';' '{printf "%s", $5}' | tr -d '"')
            b=$(echo "$dbFingerprint" | awk -F';' '{printf "%s", $5}' | tr -d '"')
            confMatrix[$b,$a]=$((${confMatrix[$b,$a]} + 1))
            found=true
            break
        fi
    done

    if [ "$found" = false ]; then
        a=$(echo "$fingerprint" | awk -F';' '{printf "%s", $5}' | tr -d '"')
        confMatrix[UNKNOWN,$a]=$((${confMatrix[UNKNOWN,$a]} + 1))
    fi
}

#
# For each fingerprint do the comparison with the whole database and calculate statistics
#
function main()
{
    fingerprints=$1
    database=$2

    declare -a apps=("boltfood" "damejidlo" "discord" "pinterest" "reddit" "rossmanclub" "signal" "twitch" "zalando" "zonky" "UNKNOWN")
    declare -a databaseRecords
    declare -A confMatrix

    for i in "${apps[@]}"
    do
        for j in "${apps[@]}"
        do
            confMatrix[$i,$j]=0
        done
    done

    while read -r record
    do
        databaseRecords+=($record)
    done <<< $(tail -n +2 "$database")

    while read -r fingerprint
    do
        compareToDatabase $fingerprint databaseRecords[@] $confMatrix
    done <<< $(tail -n +2 "$fingerprints")

    echo
    printf "%12s" "Conf. matrix:"
    for i in "${apps[@]}"
    do
        printf "%12s" $i
    done
    echo

    STP=0
    SFP=0
    STN=0
    SFN=0
    SUM=0
    sumPrc=0.0
    sumRec=0.0

    for i in "${apps[@]}"
    do
        printf "%12s" $i
        for j in "${apps[@]}"
        do
            printf "%12s" ${confMatrix[$i,$j]}

            SUM=$((SUM+${confMatrix[$i,$j]}))
        done
        echo
    done
    echo

    for a in "${apps[@]}"
    do
        TP=0
        FP=0
        TN=0
        FN=0

        for i in "${apps[@]}"
        do
            for j in "${apps[@]}"
            do
                if [ "$i" = "$a" ] &&
                   [ "$j" = "$a" ]; then
                    TP=$((TP+${confMatrix[$i,$j]}))
                elif [ "$i" = "$a" ]; then
                    FP=$((FP+${confMatrix[$i,$j]}))
                elif [ "$j" = "$a" ]; then
                    FN=$((FN+${confMatrix[$i,$j]}))
                else
                    TN=$((TN+${confMatrix[$i,$j]}))
                fi
            done
        done

        if [ $(($TP + $FP)) -gt 0 ]; then
            sumPrc=$(echo "$sumPrc + ($TP / ($TP + $FP))" | bc -l)
        fi
        if [ $(($TP + $FN)) -gt 0 ]; then
            sumRec=$(echo "$sumRec + ($TP / ($TP + $FN))" | bc -l)
        fi

        STP=$(($STP+$TP))
        SFP=$(($SFP+$FP))
        STN=$(($STN+$TN))
        SFN=$(($SFN+$FN))
    done

    echo
    echo " [Summed stats]  Predicted"
    echo "                 +-----------------+-----------------+"
    echo "                 | Negative        | Positive        |"
    echo "      +----------+-----------------+-----------------+"
    printf "GT    | Negative | (TN) %10d | (FP) %10d |\n" $STN $SFP
    echo "      +----------+-----------------+-----------------+"
    printf "      | Positive | (FN) %10d | (TP) %10d |\n" $SFN $STP
    echo "      +----------+-----------------+-----------------+"
    echo

    acc=$(echo "$STP / $SUM" | bc -l)
    prc=$(echo "$sumPrc / 11" | bc -l)
    rec=$(echo "$sumRec / 11" | bc -l)
    echo "Accuracy:        $acc"
    echo "Precision:       $prc"
    echo "Recall:          $rec"
    echo
}

main $1 $2
