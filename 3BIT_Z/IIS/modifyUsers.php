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
            else if (isset($_SESSION['povysen']) && $_SESSION['povysen'] == '1')
            {
              echo '
              <div class="success-msg">
                <i class="fa fa-check"></i>
                Ošetřovatel byl úspěšně povýšen.
              </div>';
              $_SESSION['povysen'] = '0';
            }
           ?>
          <div id="personal">
            Zaměstnanci
          </div>
          <ul>
            <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->
            <li>

              <form action="modifyUsers.php" method="POST">
              <fieldset>
              <legend>Vedoucí</legend>
                <?php
                    include("db.php");
                    $sql = "SELECT * FROM VedouciZOO";
                    $result = mysqli_query($db, $sql);

                    $c = 0;
                    while($row = $result->fetch_array())
                    {
                        //echo "<form class=\"sign-up-modal\">";
                        echo "<table class=\"record\" style=\"width:auto\">";

                        echo "<tr>";
                        echo "<th>";
                        echo "<tr><td><img src=\"images/personal.png\" class=\"image\" alt=\"Obrazek uzivatele\" height=\"150\" width=\"150\"\></td></tr>";
                        echo "</th>";
                        echo "<th>";
                        echo "<tr><td>* Jméno</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Jméno\" name=\"nameBoss" . $c . "\" value=\"" . $row["jmeno"] . "\" required></td></tr>";
                        echo "<tr><td>Příjmení</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Příjmení\" name=\"surnameBoss" . $c . "\" value=\"" . $row["prijmeni"] . "\"></td></tr>";
                        echo "<tr><td>* Login</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Login\" name=\"loginBoss" . $c . "\" value=\"" . $row["login"] . "\" readonly></td></tr>";
                        echo "<tr><td>* Heslo</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Heslo\" name=\"pswBoss" . $c . "\" value=\"" . $row["password"] . "\" required></td></tr>";
                        echo "<tr><td>Tituly</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Tituly\" name=\"degreeBoss" . $c . "\" value=\"" . $row["titul"] . "\"></td></tr>";
                        echo "<tr><td>* Datum narození</td><td><input class=\"input-class\" type=\"date\" name=\"birthdayBoss" . $c . "\" value=\"" . $row["datum_narozeni"] . "\" required></td></tr>";

                        echo "<td>Specializace</td><td><select class=\"input-class selector-class\" name=\"specializationBoss" . $c . "\" required>";
                          if ($row['specializace'] == 'Savci')         { echo "<option selected value=\"0\">Savci</option>"; }         else { echo "<option value=\"0\">Savci</option>"; }
                          if ($row['specializace'] == 'Ptáci')         { echo "<option selected value=\"1\">Ptáci</option>"; }         else { echo "<option value=\"1\">Ptáci</option>"; }
                          if ($row['specializace'] == 'Obojživelníci') { echo "<option selected value=\"2\">Obojživelníci</option>"; } else { echo "<option value=\"2\">Obojživelníci</option>"; }
                          if ($row['specializace'] == 'Plazi')         { echo "<option selected value=\"3\">Plazi</option>"; }         else { echo "<option value=\"3\">Plazi</option>"; }
                        echo "</select></td>";
                        echo "</tr>";
                        echo "</th>";

                        if ($row['login'] != $_SESSION['name'])
                        {
                          ?>
                          <td>
                          <form action="modifyUsers.php?name="<?php echo $row["login"]; ?>"" method="post">
                            <input type="hidden" name="id_boss" value="<?php echo $row["login"]; ?>">
                            <input type="submit" class="logoutButton" name="odstranit_boss" value="Odstranit">
                          </form>
                          </td>
                          <?php
                        }
                        else
                        {
                          echo "<td></td>";
                        }
                          ?>
                          <td>
                            <div align="center">
                              <input class="changeButton" target="_blank" name="upravit_boss" rel="nofollow noopener" type="submit" value="Upraviť">
                            </div>
                          </td>
                          <?php
                        echo "</tr>";
                        echo "</table>";
                        //echo "</form>";

                        $c++;
                    }

                    echo "</table>";
                ?>

              </fieldset>
              </form>
            </li>
          </ul>

          <ul>
            <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->
            <li>
              <form action="modifyUsers.php" method="POST">
              <fieldset>
              <legend>Ošetřovatelé</legend>
                <?php
                    include("db.php");
                    $sql = "SELECT * FROM Osetrovatel";
                    $result = mysqli_query($db, $sql);

                    $c = 0;
                    while($row = $result->fetch_array())
                    {
                        echo "<table class=\"record\" style=\"width:auto\">";


                        echo "<tr>";
                        echo "<th>";
                        echo "<tr><td><img src=\"images/personal.png\" class=\"image\" alt=\"Obrazek uzivatele\" height=\"150\" width=\"150\"\></td><td>";
                        echo "<form action=\"modifyUsers.php\" method=\"post\">";
                        echo "<input type=\"hidden\" name=\"povysit\" value=\"" . $row['login'] . "\">";
                        echo "<input type=\"submit\" class=\"filterButton\" name=\"povysitKeeper\" onclick=\"return confirm('Budou nevratně smazány ošetřovatelovi záznamy krmení a čištění, přejete si pokračovat ?');\" value=\"Povýšit na Vedoucího\">";
                        echo "</form>";
                        echo "</td></tr></th>";
                        echo "<th>";
                        echo "<tr><td>* Jméno</td><td><input class=\"input-class setWidth\" type=\"text\" placeholder=\"Jméno\" name=\"nameKeeper" . $c . "\" value=\"" . $row["jmeno"] . "\" required></td></tr>";
                        echo "<tr><td>Příjmení</td><td><input class=\"input-class setWidth\" type=\"text\" placeholder=\"Příjmení\" name=\"surnameKeeper" . $c . "\" value=\"" . $row["prijmeni"] . "\"></td></tr>";
                        echo "<tr><td>* Login</td><td><input class=\"input-class setWidth\" type=\"text\" placeholder=\"Login\" name=\"loginKeeper" . $c . "\" value=\"" . $row["login"] . "\" readonly></td></tr>";
                        echo "<tr><td>* Heslo</td><td><input class=\"input-class setWidth\" type=\"text\" placeholder=\"Heslo\" name=\"pswKeeper" . $c . "\" value=\"" . $row["password"] . "\" required></td></tr>";
                        echo "<tr><td>Tituly</td><td><input class=\"input-class setWidth\" type=\"text\" placeholder=\"Tituly\" name=\"degreeKeeper" . $c . "\" value=\"" . $row["titul"] . "\"></td></tr>";
                        echo "<tr><td>* Datum narození</td><td><input class=\"input-class setWidth\" type=\"date\" name=\"birthdayKeeper" . $c . "\" value=\"" . $row["datum_narozeni"] . "\" required></td></tr>";

                        echo "<td>Specializace</td><td><select class=\"input-class selector-class\" name=\"specializationKeeper" . $c . "\" required>";
                          if ($row['specializace'] == 'Savci')         { echo "<option selected value=\"0\">Savci</option>"; }         else { echo "<option value=\"0\">Savci</option>"; }
                          if ($row['specializace'] == 'Ptáci')         { echo "<option selected value=\"1\">Ptáci</option>"; }         else { echo "<option value=\"1\">Ptáci</option>"; }
                          if ($row['specializace'] == 'Obojživelníci') { echo "<option selected value=\"2\">Obojživelníci</option>"; } else { echo "<option value=\"2\">Obojživelníci</option>"; }
                          if ($row['specializace'] == 'Plazi')         { echo "<option selected value=\"3\">Plazi</option>"; }         else { echo "<option value=\"3\">Plazi</option>"; }
                        echo "</select></td>";
                        echo "</tr>";
                        echo "</th>";

                        ?>
                        <td>
                          <form action="modifyUsers.php?name="<?php echo $row["login"]; ?>"" method="post">
                            <input type="hidden" name="id_keeper" value="<?php echo $row["login"]; ?>">
                            <input type="submit" class="logoutButton" name="odstranit_keeper" value="Odstranit">
                          </form>
                        </td>
                        <td>
                          <div align="center">
                            <input class="changeButton" target="_blank" name="upravit_keeper" rel="nofollow noopener" type="submit" value="Upraviť">
                          </div>
                        </td>
                        <?php
                        echo "</tr>";
                        echo "</table>";
                        $c++;
                    }

                    echo "</table>";
                ?>

              </fieldset>
              </form>
            </li>
          </ul>
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

    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['upravit_boss']))
    {
      $special = array(0 => 'Savci',
        1 => 'Ptáci',
        2 => 'Obojživelníci',
				3 => 'Plazi');

      $i = 0;
      while (isset($_POST['nameBoss' . $i]))
      {
        $nameBoss = $_POST['nameBoss' . $i];
        $surnameBoss = $_POST['surnameBoss' . $i];
        $loginBoss = $_POST['loginBoss' . $i];
        $pswBoss = $_POST['pswBoss' . $i];
        $degreeBoss = $_POST['degreeBoss' . $i];
        $birthdayBoss = $_POST['birthdayBoss' . $i];
        $specialization = $special[$_POST['specializationBoss' . $i]];

        $sql = "UPDATE VedouciZOO SET jmeno = \"" . $nameBoss . "\" , prijmeni = \"" . $surnameBoss . "\" , password = \"" . $pswBoss . "\", titul = \"" . $degreeBoss . "\", datum_narozeni = \"" . $birthdayBoss . "\", specializace = \"" . $specialization . "\" WHERE login=\"" . $loginBoss . "\"";
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

    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['upravit_keeper']))
    {
      $special = array(0 => 'Savci',
        1 => 'Ptáci',
        2 => 'Obojživelníci',
        3 => 'Plazi');

      $i = 0;
      while (isset($_POST['nameKeeper' . $i]))
      {
        $nameKeeper = $_POST['nameKeeper' . $i];
        $surnameKeeper = $_POST['surnameKeeper' . $i];
        $loginKeeper = $_POST['loginKeeper' . $i];
        $pswKeeper = $_POST['pswKeeper' . $i];
        $degreeKeeper = $_POST['degreeKeeper' . $i];
        $birthdayKeeper = $_POST['birthdayKeeper' . $i];
        $specialization = $special[$_POST['specializationKeeper' . $i]];

        $sql = "UPDATE Osetrovatel SET jmeno = \"" . $nameKeeper . "\" , prijmeni = \"" . $surnameKeeper . "\" , password = \"" . $pswKeeper . "\", titul = \"" . $degreeKeeper . "\", datum_narozeni = \"" . $birthdayKeeper . "\", specializace = \"" . $specialization . "\" WHERE login=\"" . $loginKeeper . "\"";
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
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['odstranit_keeper']))
    {
        // Odstranit jeho zaznamy krmeni
        $sql = "DELETE FROM ZaznamCisteni WHERE login = \"" . $_POST['id_keeper'] . "\"";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // Odstranit jeho zaznamy cisteni
        $sql = "DELETE FROM ZaznamKrmeni WHERE login = \"" . $_POST['id_keeper'] . "\"";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // Odstranit keepra
        $sql = "DELETE FROM Osetrovatel WHERE login = \"" . $_POST['id_keeper'] . "\"";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        $_SESSION['removed'] = '1';
        header("Refresh:0");
    }

    // Remove admin
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['odstranit_boss']))
    {
        $sql = "DELETE FROM VedouciZOO WHERE login = \"" . $_POST['id_boss'] . "\"";

        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }
        $_SESSION['removed'] = '1';
        header("Refresh:0");
    }

    // Povysit na vedouciho
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['povysitKeeper']))
    {
        $sql = "SELECT * FROM Osetrovatel WHERE login=\"" . $_POST['povysit'] . "\"";

        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          header("Location: showError.php"); die("");
        }

        $row = $result->fetch_array();

        $sql = "INSERT INTO VedouciZOO(login, password, jmeno, prijmeni, datum_narozeni, titul, specializace) VALUES ('" . $row['login'] . "', '" . $row['password'] . "', '" . $row['jmeno'] . "', '" . $row['prijmeni'] . "', '" . $row['datum_narozeni'] . "', '" . $row['titul'] . "', '" . $row['specializace'] . "')";

        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          header("Location: showError.php"); die("");
        }

        // Odstranit jeho zaznamy krmeni
        $sql = "DELETE FROM ZaznamCisteni WHERE login = \"" . $row['login'] . "\"";
        $result = mysqli_query($db, $sql);

        // Odstranit jeho zaznamy cisteni
        $sql = "DELETE FROM ZaznamKrmeni WHERE login = \"" . $row['login'] . "\"";
        $result = mysqli_query($db, $sql);

        // Odstranit keepra
        $sql = "DELETE FROM Osetrovatel WHERE login = \"" . $row['login'] . "\"";
        $result = mysqli_query($db, $sql);

        $_SESSION['povysen'] = '1';
        header("Refresh:0");
    }
