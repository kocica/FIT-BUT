<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8" meta http-equiv="refresh" content="300;url=logTimeoutDisconnect.php"/>
        <link rel="stylesheet" href="styles/styles.css">
        <link rel="shortcut icon" type="image/x-icon" href="images/Elephant.ico" />
        <title>Informační systém ZOO</title>
    </head>

    <?php
        header("Content-Type: text/html; charset=UTF-8");
        session_start();

        if (!isset($_SESSION['name']))
        {
            header("Location: logTimeoutDisconnect.php");
        }
    ?>

    <body>
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
        <articlelogged>
        <?php
            if (isset($_SESSION['saved']) && $_SESSION['saved'] == '1')
            {
              echo '
              <div class="success-msg">
                <i class="fa fa-check"></i>
                Záznam bol úspešne vložený.
              </div>';
              $_SESSION['saved'] = '0';
            }
            if (isset($_SESSION['error']))
            {
              echo '
              <div class="error-msg">
                <i class="fa fa-check"></i>
                ' . $_SESSION['msg'] . '
              </div>';
              unset($_SESSION['error']);
            }
           ?>
          <div id="personal">
           Přidat záznam o rase
          </div>
          <ul>

            <form action="addRace.php" method="POST">

                <?php
                    include("db.php");
                    include("isInSession.php");
                ?>

                    <table class="record" style="width:100%">
                    <?php
                    $typ = isInSession('jmenoRasy');
                    $popis = isInSession('popisRasy');

                    echo "<tr><td><h5>* Jméno rasy: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Jméno rasy\" name=\"typ_rasy\" value=\"" . $typ . "\" required></td></tr>";
                    echo "<tr><td><h5>Popis rasy: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Popis rasy\" name=\"popis\" value=\"" . $popis . "\"></td></tr>";

                    ?>
                    <td>
                      <div class="button_cont" align="center">
                          <input class="changeButton" target="_blank" rel="nofollow noopener" type="submit" value="Uložit záznam">
                      </div>
                    </td>
                </table>

        </form>

        </ul>
        <!-- popout menu -->
        <?php
          include("popMenu.php");
        ?>
        </articlelogged>

      </sectionlogged>

      <div class="downsideinfo">

      </div>

    </body>
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

    // Feeding saved
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['typ_rasy']))
    {
        $typ_rasy = $_POST['typ_rasy'];
        $popis = $_POST['popis'];

        $sql = "SELECT * FROM Rasa WHERE jmeno=\"" . $typ_rasy . "\"";
        $result = mysqli_query($db, $sql);
        if ($row = $result->fetch_array())
        {
            $_SESSION['msg'] = 'Rasa již existuje.';
            $_SESSION['error'] = '1';
            $_SESSION['jmenoRasy'] = $typ_rasy;
            $_SESSION['popisRasy'] = $popis;

            header("Refresh:0"); die("");
        }

        $sql = "INSERT INTO Rasa (jmeno, popis) VALUES('$typ_rasy', '$popis')";

        $result = mysqli_query($db, $sql);

        if (($result) === false)
        {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
        }
        $_SESSION['saved'] = '1';
        header("Refresh:0");
    }

?>
