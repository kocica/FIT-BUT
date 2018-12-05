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
            <div class="grid-item-img-iq-test"><img width="700" src="../img/guitar.jpg"></div>
            <div class="grid-item-q-iq-test">Úkol: Vyberte jeden z obrázků</div>
            <div class="grid-container-iq-test">
                <div onClick="document.location.href='wrong.php'"   class="grid-item-iq-test"><img width="250" src="../img/darthvader.jpg"></div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item-iq-test"><img width="250" src="../img/dog.jpg"></div>
                <div onClick="document.location.href='correct.php'" class="grid-item-iq-test"><img width="250" src="../img/piano.jpg"></div>
                <div onClick="document.location.href='wrong.php'"   class="grid-item-iq-test"><img width="250" src="../img/stop-sign.png"></div>
            </div>
        </center>

    </body>

</html>