<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="../styles/styles.css">
        <title>Aplikace pro zlepšení mentálních schopností seniorů</title>
    </head>

    <?php
        header("Content-Type: text/html; charset=UTF-8");
    ?>

    <body>

        <a class="close" onClick="document.location.href='../index.php'"></a>

        <center>
            <div class="grid-container">
                <div class="grid-item-img"><img width="600" src="../img/nature.jpg"></div>
                <div class="grid-item-q">Úkol: Určete, co je na obrázku</div>
                <div onClick="document.location.href='correct.php'" class="grid-item">1. Stromy </div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">2. Květiny </div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">3. Motýly </div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">4. Pampelišky </div>
            </div>
        </center>

    </body>

</html>