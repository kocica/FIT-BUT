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

        // TODO: User not logged in !!!!!!!
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
           ?>
          <div id="personal">
            Záznamy o rasách
          </div>
          <ul>
            <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->
            <li>
                <form action="listRaces.php" method="POST">
                    <?php
                        include("db.php");
                        $sql = "SELECT * FROM Rasa";
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
                            echo "<table class=\"record\";>";

                            echo "<tr>";
                            echo "<th>";
                            echo "<tr><td><img src=\"images/races.png\" class=\"image\" alt=\"Obrazek rasy\" height=\"150\" width=\"150\"\></td></tr>";
                            echo "</th>";

                            echo "<tr><td>Jméno rasy</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Jméno rasy\" name=\"jmeno" . $c . "\" value=\"" . $row["jmeno"] . "\" readonly></td></tr>";
                            echo "<tr><td>Popis rasy</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Popis rasy\" name=\"popis" . $c . "\" value=\"" . $row['popis'] . "\"" . $readonly . "/></td></tr>";

                            if ($_SESSION['user'] == 'boss')
                            {
                              ?>
                              <td>
                                <form action="listRaces.php?name="<?php echo $row["jmeno"]; ?>"" method="post">
                                  <input type="hidden" name="id_user" value="<?php echo $row["jmeno"]; ?>">
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
      while (isset($_POST['jmeno' . $i]))
      {
        $jmeno = $_POST['jmeno' . $i];
        $popis = $_POST['popis' . $i];

        $sql = "UPDATE Rasa SET popis = \"" . $popis . "\" WHERE jmeno=\"" . $jmeno . "\"";
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
        $sql = "DELETE FROM Rasa WHERE jmeno= \"" . $_POST['id_user'] . "\"";

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
