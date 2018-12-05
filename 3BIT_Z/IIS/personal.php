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
                Profil bol úspešne upravený.
              </div>';
              $_SESSION['saved'] = '0';
            }
           ?>
          <div id="personal">
           Moje osobné údaje
          </div>
          <ul>
            <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->

            <fieldset>
            <form action="personal.php" method="POST">

              <?php
                include("db.php");
                if ($_SESSION['user'] == 'boss')
                {
                  $sql = 'SELECT * FROM VedouciZOO v WHERE v.login="'. $_SESSION['name'] .'"';
                }
                else
                {
                  $sql = 'SELECT * FROM Osetrovatel o WHERE o.login="'. $_SESSION['name'] .'"';
                }
                $result = mysqli_query($db, $sql);

                echo "<table style=\"width:100%\">";

                if ($result->num_rows > 0)
                {
                  $row = $result->fetch_assoc();

                  $name = $row["jmeno"];
                  echo "<tr><td><h5>* Moje jméno:</h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Moje jméno\" name=\"name\" value=\"$name\" required></td></tr>";

                  $surname = $row["prijmeni"];
                  echo "<tr><td><h5>Moje přijmení:</h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Moje přijmení\" name=\"surname\" value=\"$surname\"></td></tr>";

                  $login = $row["login"];
                  echo "<tr><td><h5>* Moj login:</h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Moj login\" name=\"login\" value=\"$login\" readonly></td></tr>";

                  $psw = $row["password"];
                  echo "<tr><td><h5>* Moje heslo:</h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Moje heslo\" name=\"psw\" value=\"$psw\" required></td></tr>";

                  $degree = $row["titul"];
                  echo "<tr><td><h5>Moje tituly:</h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Moje tituly\" name=\"degree\" value=\"$degree\"></td></tr>";

                  $birthday = $row["datum_narozeni"];
                  echo "<tr><td><h5>* Môj datum narození:</h5></td><td><input class=\"input-class\" type=\"date\" name=\"birthday\" id=\"birthday\" value=\"$birthday\" required></td></tr>";

                  echo "<tr><td><h5>Moje specializace:</h5></td>";
                  echo "<td><select class=\"input-class selector-class\" name=\"specializace\">";
                    if ($row['specializace'] == 'Savci')         { echo "<option selected value=\"0\">Savci</option>"; }         else { echo "<option value=\"0\">Savci</option>"; }
                    if ($row['specializace'] == 'Ptáci')         { echo "<option selected value=\"1\">Ptáci</option>"; }         else { echo "<option value=\"1\">Ptáci</option>"; }
                    if ($row['specializace'] == 'Obojživelníci') { echo "<option selected value=\"2\">Obojživelníci</option>"; } else { echo "<option value=\"2\">Obojživelníci</option>"; }
                    if ($row['specializace'] == 'Plazi')         { echo "<option selected value=\"3\">Plazi</option>"; }         else { echo "<option value=\"3\">Plazi</option>"; }
                  echo "</select></td></tr>";
                }

                echo "</table>";
              ?>

              <div class="button_cont" align="center">
                <input class="changeButton" target="_blank" rel="nofollow noopener" type="submit" value="Upravit">
              </div>
            </form>
            </fieldset>

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

    // Change
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['name']))
    {
        $name = $_POST['name'];
        $surname = $_POST['surname'];
        $login = $_POST['login'];
        $psw = $_POST['psw'];
        $degree = $_POST['degree'];
        $birthday = $_POST['birthday'];

        $special = array(0 => 'Savci',
          1 => 'Ptáci',
          2 => 'Obojživelníci',
          3 => 'Plazi');
        $specializace = $special[$_POST['specializace']];

        if ($_SESSION['user'] == 'boss')
        {
          $sql = "UPDATE VedouciZOO SET jmeno = \"" . $name . "\" , prijmeni = \"" . $surname . "\" , password = \"" . $psw . "\", titul = \"" . $degree . "\", datum_narozeni = \"" . $birthday . "\", specializace = \"" . $specializace . "\" WHERE login=\"" . $login . "\"";
        }
        else
        {
          $sql = "UPDATE Osetrovatel SET jmeno = \"" . $name . "\" , prijmeni = \"" . $surname . "\" , password = \"" . $psw . "\", titul = \"" . $degree . "\", datum_narozeni = \"" . $birthday . "\", specializace = \"" . $specializace . "\" WHERE login=\"" . $login . "\"";
        }

        $result = mysqli_query($db, $sql);

        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // Reload page, so new values are shown to the user
        //header("Location: personal.php");
        $_SESSION['saved'] = '1';
        header("Refresh:0");
    }

?>
