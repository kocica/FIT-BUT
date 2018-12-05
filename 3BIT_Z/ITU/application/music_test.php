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
            <div class="grid-container-music-test">
                <div class="grid-item-c" onclick="changeIconMusicTest()"><img id="playStop" width="100" src="../img/play.png"></div>
            </div>
            <div class="grid-item-q-space">Úkol: Určete, co je to za skladbu</div>
            <div class="grid-container">
                <div onClick="document.location.href='wrong.php'"   class="grid-item">1. Beethovenova Symfonie č. 5 & 7</div>
                <div onClick="document.location.href='correct.php'" class="grid-item">2. Antonín Dvořák – Symfonie č. 9</div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">3. JS Bach: Goldbergovi Variace</div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item">4. Wagner: Tristan a Isolda</div>
            </div>
        </center>

        <script src="../js/logic.js" type="text/javascript"></script>

    </body>

</html>