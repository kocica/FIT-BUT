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
                Záznam(y) byl(y) úspěšně upraveny/é.
              </div>';
              $_SESSION['saved'] = '0';
            }
            else if (isset($_SESSION['removed']) && $_SESSION['removed'] == '1')
            {
              echo '
              <div class="success-msg">
                <i class="fa fa-check"></i>
                Záznam byl úspěšně odstraněný.
              </div>';
              $_SESSION['removed'] = '0';
            }
            else if (isset($_SESSION['filter']) && $_SESSION['filter'] == '1')
            {
              echo '
              <div class="success-msg">
                <i class="fa fa-check"></i>
                Filtrování úspěšné.
              </div>';
            }
           ?>
          <div id="personal">
            Záznamy krmení
          </div>

          <?php include("db.php"); ?>

          <!-- FILTR -->
          <fieldset>
            <legend>Filtr</legend>
            <form action="listFeeding.php" method="POST">
              <table>
                <tbody>
                  <tr><th>Ošetřovatel: </th><th><?php
                    $sql = 'SELECT * FROM Osetrovatel';
                    $result = mysqli_query($db, $sql);
                    echo "<select class=\"input-class selector-class\" name=\"filterLogin\">";
                    echo "<option value=\"\"></option>";
                    while ($row = $result->fetch_array())
                    {
                      echo "<option value=\"" . $row['login'] . "\">" . $row['login'] . "</option>";
                    }
                    echo "</select>";
                  ?></th></tr>

                  <tr><th>Zvíře: </th><th><?php
                    $sql = 'SELECT * FROM Zvire';
                    $result = mysqli_query($db, $sql);
                    echo "<select class=\"input-class selector-class\" name=\"filterZvire\">";
                    echo "<option value=\"\"></option>";
                    while ($row = $result->fetch_array())
                    {
                      echo "<option value=\"" . $row['id_zvire'] . "\">" . $row['jmeno'] . "</option>";
                    }
                    echo "</select>";
                  ?></th></tr>

                  <tr><th>Čas čištění (od-do): </th><?php
                    echo "<th><input class=\"input-class\" type=\"date\" name=\"filterOd\"/></th>";
                    echo "<th><input class=\"input-class\" type=\"date\" name=\"filterDo\"/></th>";
                  ?></tr>

                  <tr><th><input class="filterButton" target="_blank" name="filtrovat" rel="nofollow noopener" type="submit" value="Filtrovat"></th></tr>
                </tbody>
              </table>
            </form>
          </fieldset>
          <!-- ! FILTR -->

          <ul>
            <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->
            <li>
                <form action="listFeeding.php" method="POST">
                    <?php
                        // FILTER
                        $where = "";
                        $filtrOsetr = "";
                        $filterZvire = "";
                        $filterDate = "";
                        $and = "";
                        if (isset($_SESSION['filter']))
                        {
                          $where = " WHERE ";

                          if (isset($_SESSION['filterLogin']))
                          {
                            $filtrOsetr = "login=\"" . $_SESSION['filterLogin'] . "\"";
                            $and = " AND ";

                            unset($_SESSION['filterLogin']);
                          }
                          if (isset($_SESSION['filterZvire']))
                          {
                            $filterZvire = $and . "id_zvire=\"" . $_SESSION['filterZvire'] . "\"";
                            $and = " AND ";

                            unset($_SESSION['filterZvire']);
                          }
                          if (isset($_SESSION['filterOd']) && isset($_SESSION['filterDo']))
                          {

                            $filterDate = $and . "cas BETWEEN \"" . $_SESSION['filterOd'] . "\" AND \"" . $_SESSION['filterDo'] . "\"";

                            unset($_SESSION['filterOd']); unset($_SESSION['filterDo']);
                          }

                          unset($_SESSION['filter']);
                        }
                        // ! FILTER

                        $sql = "SELECT * FROM ZaznamKrmeni" . $where . $filtrOsetr . $filterZvire . $filterDate;

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
                            echo "<tr><td><img src=\"images/feeding.png\" class=\"image\" alt=\"Obrazek cisteni\" height=\"150\" width=\"150\"\></td></tr>";
                            echo "</th>";

                            echo "<tr><td style=\"display: none\"><input type=\"text\"  name=\"id_krmeni" . $c . "\" value=\"" . $row["id_krmeni"] . "\" readonly></td></tr>";
                            echo "<tr><td>Délka krmení (min)</td><td><input class=\"input-class\" type=\"number\" placeholder=\"Délka krmení\" name=\"delka_krmeni" . $c . "\" value=\"" . $row["delka_krmeni"] . "\" " . $readonly . " min=\"0\" max=\"180\"></td></tr>";
                            echo "<tr><td>Čas krmení</td><td><input class=\"input-class\" type=\"date\" placeholder=\"Čas krmení\" name=\"cas" . $c . "\" value=\"" . $row['cas'] . "\" " . $readonly . "/></td></tr>";
                            echo "<tr><td>Množství krmení (kg)</td><td><input class=\"input-class\" type=\"number\" placeholder=\"Množství krmení\" name=\"mnozstvi_krmeni" . $c . "\" value=\"" . $row["mnozstvi_krmeni"] . "\" " . $readonly . " min=\"0\" max=\"100\"></td></tr>";

                            echo "<tr><td>Jméno zvířete</td><td>";
                            $sql = 'SELECT * FROM Zvire';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"id_zvire" . $c . "\" required " . $disabled . ">";
                            while ($row2 = $result2->fetch_array())
                            {
                              $selected = "";
                              if ($row["id_zvire"] == $row2["id_zvire"]) { $selected = "selected"; }
                              echo "<option " . $selected . " value=\"" . $row2['id_zvire'] . "\">" . $row2['jmeno'] . "</option>";
                            }
                            echo "</select>";
                            echo "</td></tr>";

                            //echo "<td><input type=\"text\" name=\"login" . $c . "\" value=\"" . $row["login"] . "\"></td>";
                            echo "<tr><td>Ošetřovatel</td><td>";
                            $sql = 'SELECT * FROM Osetrovatel';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"login" . $c . "\" required " . $disabled. ">";
                            while ($row2 = $result2->fetch_array())
                            {
                              $selected = "";
                              if ($row["login"] == $row2["login"]) { $selected = "selected"; }
                              echo "<option " . $selected . " value=\"" . $row2['login'] . "\">" . $row2['login'] . "</option>";
                            }
                            echo "</select>";
                            echo "</td></tr>";

                            if ($_SESSION['user'] == 'boss')
                            {
                              ?>
                              <td>
                                <form action="listFeeding.php?name="<?php echo $row["id_krmeni"]; ?>"" method="post">
                                  <input type="hidden" name="id_user" value="<?php echo $row["id_krmeni"]; ?>">
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
      while (isset($_POST['id_krmeni' . $i]))
      {
        $id_krmeni = $_POST['id_krmeni' . $i];
        $delka_krmeni = $_POST['delka_krmeni' . $i];
        $cas = $_POST['cas' . $i];
        $mnozstvi_krmeni = $_POST['mnozstvi_krmeni' . $i];
        $id_zvire = $_POST['id_zvire' . $i];
        $login = $_POST['login' . $i];

        $sql = "UPDATE ZaznamKrmeni SET delka_krmeni = \"" . $delka_krmeni . "\" , cas = \"" . $cas . "\" , mnozstvi_krmeni = \"" . $mnozstvi_krmeni . "\", id_zvire = \"" . $id_zvire . "\", login = \"" . $login . "\" WHERE id_krmeni=\"" . $id_krmeni . "\"";
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
        $sql = "DELETE FROM ZaznamKrmeni WHERE id_krmeni= \"" . $_POST['id_user'] . "\"";

        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          header("Location: showError.php"); die("");
        }
        $_SESSION['removed'] = '1';
        header("Refresh:0");
    }

    // Filter
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['filtrovat']))
    {
      // Login filter
      if ($_POST['filterLogin'] != "")
      {
        $_SESSION['filter'] = '1';
        $_SESSION['filterLogin'] = $_POST['filterLogin'];
      }

      // Animal filter
      if ($_POST['filterZvire'] != "")
      {
        $_SESSION['filter'] = '1';
        $_SESSION['filterZvire'] = $_POST['filterZvire'];
      }

      // Date filter
      if ((strtotime($_POST['filterOd']) > 0) && (strtotime($_POST['filterDo']) > 0))
      {
        $_SESSION['filter'] = '1';
        $_SESSION['filterOd'] = $_POST['filterOd'];
        $_SESSION['filterDo'] = $_POST['filterDo'];
      }

      header("Refresh:0");
    }
?>
