<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8" meta http-equiv="refresh" content="300;url=logTimeoutDisconnect.php"/>
        <link rel="stylesheet" href="styles/styles.css">
        <link rel="shortcut icon" type="image/x-icon" href="images/Elephant.ico" />
        <title>Informační systém ZOO</title>
    </head>

    <?php
        ob_start();
        header("Content-Type: text/html; charset=UTF-8");
        session_start();

        if (!isset($_SESSION['name']))
        {
            header("Location: logTimeoutDisconnect.php");
        }
    ?>

    <div class="backgroundContainer">
      <div class="content">
        <header>
         <div id="element1">
          Informačný systém pre správu ZOO
         </div>

         <div id="element3">
          <form action='profile.php' method='post'>
              <input class="logoutButton" type="submit" name="logout" value="Odhlásit" />
          </form>
         </div>

         <div id="element2">
           <?php
             echo '<div id="logname">' . "Přihlášen jako: " .$_SESSION['name']. "&nbsp&nbsp" . '</div>';
           ?>
         </div>

        </header>
        <sectionlogged>
          <!-- Menu in there -->
          <?php
            include("menu.php");
          ?>
          <articlelogged id="profileArticle">
            <?php
              include("popMenu.php");
            ?>
          </articlelogged>
        </sectionlogged>
      </div>
      <div class="downsideinfo">
      </div>
    </div>
</html>

<?php

    // Logout
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['logout']))
    {
        session_destroy();
        session_unset();
        unset($_SESSION['name']);
        $_SESSION = array();

        // Redirect to main page
        header("Location: index.php");
    }

?>
