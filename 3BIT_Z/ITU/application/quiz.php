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
                <div class="grid-item-quiz">Jak se do lesa ______, tak se z lesa ozývá.</div>
                <div class="grid-item-q-space">Úkol: Doplňte přísloví</div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">1. Chodívá </div>
                <div onClick="document.location.href='correct.php'" class="grid-item">2. Volá </div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">3. Zpívá </div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">4. Mává </div>
            </div>
        </center>

    </body>

</html>