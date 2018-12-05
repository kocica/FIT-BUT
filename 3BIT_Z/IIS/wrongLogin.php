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
      <!--/*
        if ($_SESSION['user'] == 'passdontmatch')
        {
          echo
          '<div class="downside">
            <h1>Zadáno nesprávné uživatelské heslo</h1>
            <h5>Zadané hesla se neschodují.</h5>
            <button class="loginButtons" type="button" name="button" onclick="document.location.href=\'index.php\'">Zpět na registraci</button>
          </div>';
        }*/
       ?>-->

      <div class="downside">
        <h1>Zadáno nesprávné uživatelské jméno nebo heslo</h1>
        <h5>Přístup do informačního systému ZOO je povolen pouze oprávněným uživatelům.</h5>
        <button class="loginButtons" type="button" name="button" onclick="document.location.href='index.php'">Zpět na přihlášení</button>
      </div>

      <div class="downsideinfo">

      </div>

    </body>
 </html>
