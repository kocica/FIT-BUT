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
            <center>

            <p class="correct">Správně !</p>

            <img src="../img/correct.png" width="100" height="100"></img>

            <div class="grid-container">
                <div onClick="window.history.back()" class="grid-item">Další otázka</div>
                <div onClick="document.location.href='results.php'" class="grid-item">Vyhodnotit</div>
            </div>

            <a class="close" onClick="document.location.href='../index.php'">

            </center>


    </body>

</html>