<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="styles/styles.css">
        <link rel="shortcut icon" type="image/x-icon" href="images/Elephant.ico" />
        <title>Informační systém ZOO</title>
    </head>

    <?php
      header("Content-Type: text/html; charset=UTF-8");
      session_start();
     ?>

    <body>
      <header>
        <div id="element1">
          Informačný systém pre správu ZOO
        </div>
      </header>

      <?php
        session_destroy();
        session_unset();
        $_SESSION = array();
      ?>

      <div class="downside">
        <h1>Vaše spojení vypršelo, přihlaste se prosím znovu!</h1>
        <button class="loginButtons" type="button" name="button" onclick="document.location.href='index.php'">Zpět na přihlášení</button>
      </div>

      <div class="downsideinfo">

      </div>

    </body>
 </html>
