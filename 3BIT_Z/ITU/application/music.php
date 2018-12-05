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
            <div id="currentlyPlaying" class="grid-item-q-music">Přehrávání vypnuto</div>
            <div class="grid-container-music">
                <div class="grid-item-c" onclick="changeMusic()"><img width="100" src="../img/music-left.png"></div>
                <div class="grid-item-c" onclick="changeIcon()"><img id="playStop" width="100" src="../img/play.png"></div>
                <div class="grid-item-r" onclick="changeMusic()"><img width="100" src="../img/music-left.png"></div>
            </div>
        </center>

        <script src="../js/logic.js" type="text/javascript"></script>

    </body>

</html>