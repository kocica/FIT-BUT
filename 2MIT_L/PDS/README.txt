Autor:  Filip Kočica
Email:  xkocic01 AT stud.fit.vutbr.cz
Datum:  3/4/2021

--------------------------------------------------------------------------------------------------------------------------------------

Pro celkové otestování funkčnosti je nutné stáhnout datovou sadu a extrahovat ji ve složce ./data/

        https://drive.google.com/file/d/1S3LKfIDIqSGFQ3PHkHcS1ZTCEqUs8Jq5/view?usp=sharing

tak, aby cesty byly: ./data/datasets/train a ./data/datasets/test.

--------------------------------------------------------------------------------------------------------------------------------------

Poté lze postupovat pomocí tohoto návodu (v kořenu projektu spustit následující):

        # Smazání předchozích CSV souborů (pouze pokud chcete celkové otestování i s extrakcí dat z .pcapng)
        find ./data/ -name '*.csv' -delete

        # Získání Client Hello + Server Hello + Server Certificate
        ./src/parseTlsDataset.sh data/datasets/train/ data/datasets/train/ _
        ./src/parseTlsDataset.sh data/datasets/test/  data/datasets/test/  .

        # Extrakce TLS otisků
        ./src/extractTlsFingerprints.sh ./data/datasets/train/ ./data/whitelists/ ./data/sni_blacklist.txt -u > data/database.csv
        ./src/extractTlsFingerprints.sh ./data/datasets/test/  ./data/whitelists/ ./data/sni_blacklist.txt -u > data/testFingerprints.csv

        # Zajištění unikátnosti záznamů
        awk -i inplace '!seen[$0]++' data/database.csv
        awk -i inplace '!seen[$0]++' data/testFingerprints.csv

        # Vyhodnocení
        ./src/evalTlsFingerprints.sh ./data/testFingerprints.csv ./data/database.csv

jak je popsáno i v dokumentaci, celý proces trvá velmi dlouho.

--------------------------------------------------------------------------------------------------------------------------------------

Struktura adresáře:

  - README.txt                             (Tento soubor)

  - xkocic01.pdf                           (Dokumentace projektu)

  - src/parseTlsDataset.sh                 (Zdrojové kódy projektu)
  - src/extractTlsFingerprints.sh
  - src/evalTlsFingerprints.sh

  - data/database.csv                      (Databáze otisků)
  - data/testFingerprints.csv              (Testovací otisky)

  - data/sni_blacklist.txt                 (Blacklist jmen analytických/reklamních serverů)
  - data/whitelists/*.txt                  (Whitelist jmen serverů všech deseti aplikací)

  - data/datasets/test/*.{pcapng|csv}      (Datová sada pro testování -> data/testFingerprints.csv)
  - data/datasets/train/*.{pcapng|csv}     (Datová sada pro trénování -> data/dataset.csv)
