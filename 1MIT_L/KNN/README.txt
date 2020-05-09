====================================================================
Instrukce pro testování
====================================================================

Sdílené odkazy na našem google disku (buď lze vytvořit simlink či přímo překopírovat data na Váš disk):
    1) Model:  https://drive.google.com/open?id=10BXLczB_ODr5aqKQyuHUx4nfqPsqCTZK
    2) Váhy:   https://drive.google.com/open?id=15iH3RtDqXmJPCh5yC0wJQRqm6AtvvJLp
    3) ReId:   https://drive.google.com/open?id=15O2AEprMY0XufHvJqJf4-UbjBe3IT9U7
    4) HDR:    https://drive.google.com/open?id=1Br3tw_XwPzKgvx3jIopfqN1kYHKY0rdw

1. V adresáři utils/ se nachází skript `evaluate_model_show.ipynb`, který zobrazuje snímky SPZ společně s predikcí modelu a ground truth.
2. Otevřete tento skript v python notebooku (pokud nemáte lokálně, lze využít https://colab.research.google.com/notebooks).
3. Ve druhé buňce si lze vybrat jeden z ReId a HDR datových sad.
4. Ve druhé buňce taktéž nastavte cestu k modelu, vahám a datové sadě (na které jsou sdílené odkazy nahoře).
5. Spustťe postupně jednotlivé buňky (v první buňce je třeba povolit přístup k Vašemu google disku, kde máte simlink na naše data nebo data přímo překopírované).
6. Měli by jste vidět predikce a ground truth jednotlivých SPZ. Nakonec se vypíše úspěšnost.

(V případě potřeby můžeme nasdílet více modelů/vah, stačí nás kontaktovat na školní e-mail.)