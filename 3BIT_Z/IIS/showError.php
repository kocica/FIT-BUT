<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="styles/styles.css">
        <link rel="shortcut icon" type="image/x-icon" href="images/Elephant.ico" />
        <title>Informační systém ZOO</title>
    </head>

    <?php
      ob_start();
      session_start();
      header("Content-Type: text/html; charset=UTF-8");
     ?>

    <body>
      <header>
        <div id="element1">
          Informačný systém pre správu ZOO
        </div>
      </header>
        <?php
          $msg = $_SESSION['msg'];
        ?>
        <div class="downside">
            <h1>Nastala chyba v IS</h1>
            <h5><?php echo $msg; ?></h5>
            <button class="loginButtons" type="button" name="button" onclick="document.location.href='profile.php'">Zpět do menu</button>
        </div>
        <div class="downsideinfo">
        </div>
    </body>
 </html>
