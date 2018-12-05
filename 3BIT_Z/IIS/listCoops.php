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
                Záznam bol úspešne upravený.
              </div>';
              $_SESSION['saved'] = '0';
            }
            else if (isset($_SESSION['removed']) && $_SESSION['removed'] == '1')
            {
              echo '
              <div class="success-msg">
                <i class="fa fa-check"></i>
                Záznam bol úspešne odstránený.
              </div>';
              $_SESSION['removed'] = '0';
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
            Záznamy o výbězích
          </div>
          <ul>
            <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->
            <li>
                <form action="listCoops.php" method="POST">
                    <?php
                        include("db.php");
                        $sql = "SELECT * FROM Vybeh";
                        $result = mysqli_query($db, $sql);

                        $readonly = "";
                        $disabled = "";
                        if ($_SESSION['user'] == 'keeper')
                        {
                          $readonly = "readonly";
                          $disabled = "disabled=\"true\"";
                        }

                        $c = 0;
                        while($row = $result->fetch_array())
                        {

                            #echo "<form class=\"sign-up-modal\">";
                            echo "<table class=\"record\";>";
                            echo "<tr>";
                            echo "<th>";
                            echo "<tr><td><img src=\"images/fence.png\" class=\"image\" alt=\"Obrazek vybehu\" height=\"150\" width=\"150\"\></td></tr>";
                            echo "</th>";

                            echo "<tr><td style=\"display: none\"><input type=\"text\" name=\"id_vybehu" . $c . "\" value=\"" . $row["id_vybehu"] . "\" readonly></td></tr>";
                            echo "<tr><td>Jméno výběhu</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Jméno výběhu\" name=\"typ_vybehu" . $c . "\" value=\"" . $row["typ_vybehu"] . "\" " . $readonly . "></td></tr>";
                            //echo "<td><input type=\"text\" name=\"kapacita_vybehu" . $c . "\" value=\"" . $row["kapacita_vybehu"] . "\" " . $readonly . "></td>";
                            echo "<tr><td>Čistota výběhu</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Čistota výběhu\" name=\"cistota" . $c . "\" value=\"" . $row["cistota"] . "\" " . $readonly . "></td></tr>";

                            for ($ii = 1; $ii < 4; $ii++)
                            {
                              echo "<tr><td>Zvíře</td><td>";
                              $sql = 'SELECT * FROM Zvire';
                              $result2 = mysqli_query($db, $sql);
                              if (($result2) === false)
                              {
                                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                                header("Location: showError.php"); die("");
                              }
                              echo "<select class=\"input-class selector-class\" name=\"id_zvire" . $ii . "" . $c . "\" " . $disabled . ">";
                              echo "<option disabled selected value=\"0\"></option>";
                              while ($row2 = $result2->fetch_array())
                              {
                                $selected = "";
                                if ($row["id_zvire" . $ii] == $row2["id_zvire"]) { $selected = "selected"; }
                                echo "<option " . $selected . " value=\"" . $row2['id_zvire'] . "\">" . $row2['jmeno'] . "</option>";
                              }
                              echo "</select>";
                              echo "</td></tr>";
                            }

                            if ($_SESSION['user'] == 'boss')
                            {
                              ?>
                              <td>
                                <form action="listCoops.php?name="<?php echo $row["id_vybehu"]; ?>"" method="post">
                                  <input type="hidden" name="id_user" value="<?php echo $row["id_vybehu"]; ?>">
                                  <input type="submit" class="logoutButton" name="odstranit" value="Odstranit">
                                </form>
                              </td>
                              <td>
                                <div align="center">
                                    <input class="changeButton" target="_blank" name="upravit" rel="nofollow noopener" type="submit" value="Upraviť">
                                </div>
                              </td>
                              <?php
                            }

                            echo "</tr>";
                            $c++;
                        }
                        echo "</table>";
                        #echo "</form>";

                    ?>

                </form>
            </li>
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

    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['upravit']))
    {
      $i = 0;
      while (isset($_POST['id_vybehu' . $i]))
      {
        $id_vybehu = $_POST['id_vybehu' . $i];
        $typ_vybehu = $_POST['typ_vybehu' . $i];
        $kapacita_vybehu = '3';
        $cistota = $_POST['cistota' . $i];
        $id_zvire1 = '0';
        $id_zvire2 = '0';
        $id_zvire3 = '0';

        if (isset($_POST['id_zvire1' . $i]))
        {
            $id_zvire1 = $_POST['id_zvire1' . $i];
        }
        if (isset($_POST['id_zvire2' . $i]))
        {
            $id_zvire2 = $_POST['id_zvire2' . $i];
        }
        if (isset($_POST['id_zvire3' . $i]))
        {
            $id_zvire3 = $_POST['id_zvire3' . $i];
        }

        if ($id_zvire1 != '0')
        {
            if ($id_zvire1 == $id_zvire2 || $id_zvire1 == $id_zvire3)
            {
                $_SESSION['msg'] = 'Stejné zvíře nemůže být vícekrát v jednom výběhu.';
                $_SESSION['error'] = '1';
                header("Refresh:0"); die("");
            }

            $sql = "SELECT * FROM Zvire WHERE id_zvire=" . $id_zvire1;
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                header("Location: showError.php"); die("");
            }
            $row = $result->fetch_array();
            if ($row['id_vybehu'] != '0' && $row['id_vybehu'] != $id_vybehu)
            {
                $_SESSION['msg'] = 'Zvíře už je v jiném výběhu! První jej přemístěte do dočasného výběhu';
                $_SESSION['error'] = '1';
                header("Refresh:0"); die("");
            }
            else
            {
                $sql = "UPDATE Zvire SET id_vybehu = \"" . $id_vybehu . "\" WHERE id_zvire=\"" . $id_zvire1 . "\"";
                $result = mysqli_query($db, $sql);
                if (($result) === false)
                {
                    $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                    header("Location: showError.php"); die("");
                }
            }
        }
        if ($id_zvire2 != '0')
        {
            if ($id_zvire1 == $id_zvire2 || $id_zvire2 == $id_zvire3)
            {
                $_SESSION['msg'] = 'Jeden výběh nemůže obsahovat více stejných zvířat';
                $_SESSION['error'] = '1';
                header("Refresh:0"); die("");
            }

            $sql = "SELECT * FROM Zvire WHERE id_zvire=" . $id_zvire2;
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                header("Location: showError.php"); die("");
            }
            $row = $result->fetch_array();
            if ($row['id_vybehu'] != '0' && $row['id_vybehu'] != $id_vybehu)
            {
                $_SESSION['msg'] = 'Zvíře už je v jiném výběhu! První jej přemístěte do dočasného výběhu';
                $_SESSION['error'] = '1';
                header("Refresh:0"); die("");
            }
            else
            {
                $sql = "UPDATE Zvire SET id_vybehu = \"" . $id_vybehu . "\" WHERE id_zvire=\"" . $id_zvire2 . "\"";
                $result = mysqli_query($db, $sql);
                if (($result) === false)
                {
                    $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                    header("Location: showError.php"); die("");
                }
            }
        }
        if ($id_zvire3 != '0')
        {
            if ($id_zvire1 == $id_zvire3 || $id_zvire2 == $id_zvire3)
            {
                $_SESSION['msg'] = 'Jeden výběh nemůže obsahovat více stejných zvířat';
                $_SESSION['error'] = '1';
                header("Refresh:0"); die("");
            }

            $sql = "SELECT * FROM Zvire WHERE id_zvire=" . $id_zvire3;
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
                $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                header("Location: showError.php"); die("");
            }
            $row = $result->fetch_array();
            if ($row['id_vybehu'] != '0' && $row['id_vybehu'] != $id_vybehu)
            {
                $_SESSION['msg'] = 'Zvíře už je v jiném výběhu! První jej přemístěte do dočasného výběhu';
                $_SESSION['error'] = '1';
                header("Refresh:0"); die("");
            }
            else
            {
                $sql = "UPDATE Zvire SET id_vybehu = \"" . $id_vybehu . "\" WHERE id_zvire=\"" . $id_zvire3 . "\"";
                $result = mysqli_query($db, $sql);
                if (($result) === false)
                {
                    $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                    header("Location: showError.php"); die("");
                }
            }
        }

        $sql = "UPDATE Vybeh SET typ_vybehu = \"" . $typ_vybehu . "\" , kapacita_vybehu = \"" . $kapacita_vybehu . "\" , cistota = \"" . $cistota . "\" , id_zvire1 = \"" . $id_zvire1 . "\" , id_zvire2 = \"" . $id_zvire2 . "\" , id_zvire3 = \"" . $id_zvire3 . "\" WHERE id_vybehu=\"" . $id_vybehu . "\"";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        $i++;
      }
      $_SESSION['saved'] = '1';
      header("Refresh:0");
    }

    // Remove animal
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['odstranit']))
    {
        $sql = "DELETE FROM Vybeh WHERE id_vybehu= \"" . $_POST['id_user'] . "\"";

        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // Remove keeping
        $sql = "DELETE FROM ZaznamCisteni WHERE id_vybehu=" . $_POST['id_user'];
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // Remove animals coop
        $sql = "UPDATE Zvire SET id_vybehu=0 WHERE id_vybehu=" . $_POST['id_user'];
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }
        $_SESSION['removed'] = '1';
        header("Refresh:0");
    }

?>
