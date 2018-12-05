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
              //echo "USPESNE ULOZENO";
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
           Přidat záznam o výběhu
          </div>
          <ul>

            <form action="addCoop.php" method="POST">

                <?php
                    include("db.php");
                    include("isInSession.php");
                ?>

                    <table class="record" style="width:100%">

                    <?php
                    $jmenoVybehu = isInSession('jmenoVybehu');
                    $cistota = isInSession('cistota');

                    echo "<tr><td><h5>* Jméno výběhu: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Jméno výběhu\" name=\"typ_vybehu\" value=\"" . $jmenoVybehu . "\" required></td></tr>";
                    echo "<tr><td><h5>Čistota: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Čistota\" name=\"cistota\" value=\"" . $cistota . "\"></td></tr>";

                    for ($iter = 1; $iter < 4; $iter++)
                    {
                        $id_zvirete = isInSession('id_zvire' . $iter);

                        echo "<tr><td><h5>Zvíře</h5></td><td>";
                        $sql = 'SELECT * FROM Zvire';
                        $result2 = mysqli_query($db, $sql);
                        if (($result2) === false)
                        {
                            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                            $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
                        }
                        echo "<select class=\"input-class selector-class\" name=\"id_zvire" . $iter . "\">";
                        echo "<option value=\"0\"></option>";
                        while ($row2 = $result2->fetch_array())
                        {
                            $selected = "";
                            if ($id_zvirete == $row2["id_zvire"]) { $selected = "selected"; }
                            echo "<option " . $selected . " value=\"" . $row2['id_zvire'] . "\">" . $row2['jmeno'] . "</option>";
                        }
                        echo "</select>";
                        echo "</td></tr>";
                    }
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
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['typ_vybehu']))
    {
        //$id_vybehu = $_POST['id_vybehu'];
        $typ_vybehu = $_POST['typ_vybehu'];
        $kapacita_vybehu = '3';
        $cistota = $_POST['cistota'];
        $id_zvire1 = $_POST['id_zvire1'];
        $id_zvire2 = $_POST['id_zvire2'];
        $id_zvire3 = $_POST['id_zvire3'];

        // Get ID of coop
        $sql = "SELECT * FROM Vybeh";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
        }

        $id_vybehu = 0;
        while($row = $result->fetch_array())
        {
          $id_vybehu = $row['id_vybehu'];
        }
        $id_vybehu++;


        // Check
        if ($id_zvire1 != 0)
        {
            if ($id_zvire1 == $id_zvire2 || $id_zvire1 == $id_zvire3)
            {
                $_SESSION['msg'] = 'Stejné zvíře nemůže být vícekrát v jednom výběhu.';
                $_SESSION['error'] = '1';
                $_SESSION['jmenoVybehu'] = $typ_vybehu; $_SESSION['cistota'] = $cistota;
                $_SESSION['id_zvire2'] = $id_zvire2; $_SESSION['id_zvire3'] = $id_zvire3;
                header("Refresh:0"); die("");
            }

            $sql = "SELECT * FROM Zvire WHERE id_zvire=" . $id_zvire1;
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
            }
            $row = $result->fetch_array();
            if ($row['id_vybehu'] != '0')
            {
                $_SESSION['msg'] = 'Zvire uz je v jinem vybehu ! Prvni jej premistete do docasneho vybehu.';
                $_SESSION['error'] = '1';
                $_SESSION['jmenoVybehu'] = $typ_vybehu; $_SESSION['cistota'] = $cistota;
                $_SESSION['id_zvire2'] = $id_zvire2; $_SESSION['id_zvire3'] = $id_zvire3;
                header("Refresh:0"); die("");
            }
            else
            {
                $sql = "UPDATE Zvire SET id_vybehu = \"" . $id_vybehu . "\" WHERE id_zvire=\"" . $id_zvire1 . "\"";
                $result = mysqli_query($db, $sql);
                if (($result) === false)
                {
                    $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                    $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
                }
            }
        }
        if ($id_zvire2 != 0)
        {
            if ($id_zvire2 == $id_zvire3)
            {
                $_SESSION['msg'] = 'Stejné zvíře nemůže být ve výběhu vícekrát.';
                $_SESSION['error'] = '1';
                $_SESSION['jmenoVybehu'] = $typ_vybehu; $_SESSION['cistota'] = $cistota;
                $_SESSION['id_zvire1'] = $id_zvire1; $_SESSION['id_zvire3'] = $id_zvire3;
                header("Refresh:0"); die("");
            }

            $sql = "SELECT * FROM Zvire WHERE id_zvire=" . $id_zvire2;
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
            }
            $row = $result->fetch_array();
            if ($row['id_vybehu'] != '0')
            {
                $_SESSION['msg'] = 'Zvire uz je v jinem vybehu ! Prvni jej premistete do docasneho vybehu.';
                $_SESSION['error'] = '1';
                $_SESSION['jmenoVybehu'] = $typ_vybehu; $_SESSION['cistota'] = $cistota;
                $_SESSION['id_zvire1'] = $id_zvire1; $_SESSION['id_zvire3'] = $id_zvire3;
                header("Refresh:0"); die("");
            }
            else
            {
                $sql = "UPDATE Zvire SET id_vybehu = \"" . $id_vybehu . "\" WHERE id_zvire=\"" . $id_zvire2 . "\"";
                $result = mysqli_query($db, $sql);
                if (($result) === false)
                {
                    $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                    $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
                }
            }
        }
        if ($id_zvire3 != 0)
        {
            $sql = "SELECT * FROM Zvire WHERE id_zvire=" . $id_zvire3;
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
            }
            $row = $result->fetch_array();
            if ($row['id_vybehu'] != '0')
            {
                $_SESSION['msg'] = 'Zvire uz je v jinem vybehu ! Prvni jej premistete do docasneho vybehu.';
                $_SESSION['error'] = '1';
                $_SESSION['jmenoVybehu'] = $typ_vybehu; $_SESSION['cistota'] = $cistota;
                $_SESSION['id_zvire1'] = $id_zvire1; $_SESSION['id_zvire2'] = $id_zvire2;
                header("Refresh:0"); die("");
            }
            else
            {
                $sql = "UPDATE Zvire SET id_vybehu = \"" . $id_vybehu . "\" WHERE id_zvire=\"" . $id_zvire3 . "\"";
                $result = mysqli_query($db, $sql);
                if (($result) === false)
                {
                    $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                    $_SESSION['location'] = 'addCoop.php'; header("Location: showError.php"); die("");
                }
            }
        }

        $sql = "INSERT INTO Vybeh (id_vybehu, typ_vybehu, kapacita_vybehu, cistota, id_zvire1, id_zvire2, id_zvire3)
               VALUES('$id_vybehu', '$typ_vybehu', '$kapacita_vybehu', '$cistota', '$id_zvire1', '$id_zvire2', '$id_zvire3')";

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
