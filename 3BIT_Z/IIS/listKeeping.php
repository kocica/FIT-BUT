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
            Záznamy čištění
          </div>

          <?php include("db.php"); ?>

          <!-- FILTR -->
          <fieldset>
            <legend>Filtr</legend>
            <form action="listKeeping.php" method="POST">
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

                  <tr><th>Výběh: </th><th><?php
                    $sql = 'SELECT * FROM Vybeh';
                    $result = mysqli_query($db, $sql);
                    echo "<select class=\"input-class selector-class\" name=\"filterVybeh\">";
                    echo "<option value=\"\"></option>";
                    while ($row = $result->fetch_array())
                    {
                      echo "<option value=\"" . $row['id_vybehu'] . "\">" . $row['typ_vybehu'] . "</option>";
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
            <li>
                <form action="listKeeping.php" method="POST">
                    <?php

                        // FILTER
                        $where = "";
                        $filtrOsetr = "";
                        $filterDate = "";
                        $filterVybeh = "";
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
                          if (isset($_SESSION['filterVybeh']))
                          {
                            $filterVybeh = $and . "id_vybehu=\"" . $_SESSION['filterVybeh'] . "\"";
                            $and = " AND ";

                            unset($_SESSION['filterVybeh']);
                          }
                          if (isset($_SESSION['filterOd']) && isset($_SESSION['filterDo']))
                          {

                            $filterDate = $and . "cas BETWEEN \"" . $_SESSION['filterOd'] . "\" AND \"" . $_SESSION['filterDo'] . "\"";

                            unset($_SESSION['filterOd']); unset($_SESSION['filterDo']);
                          }

                          unset($_SESSION['filter']);
                        }
                        // ! FILTER

                        $sql = "SELECT * FROM ZaznamCisteni" . $where . $filtrOsetr . $filterVybeh . $filterDate;

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
                            echo "<tr><td><img src=\"images/cleaning.png\" class=\"image\" alt=\"Obrazek cisteni\" height=\"150\" width=\"150\"\></td></tr>";
                            echo "</th>";

                            echo "<tr><td style=\"display: none\"><input type=\"text\" name=\"id_cisteni" . $c . "\" value=\"" . $row["id_cisteni"] . "\" readonly></td></tr>";
                            echo "<tr><td>Délka čištění (min)</td><td><input class=\"input-class\" type=\"number\" name=\"delka_cisteni" . $c . "\" value=\"" . $row["delka_cisteni"] . "\" " . $readonly . " min=\"0\" max=\"180\"></td></tr>";
                            echo "<tr><td>Čas čištění</td><td><input class=\"input-class\" type=\"date\" name=\"cas" . $c . "\" value=\"" . $row['cas'] . "\" " . $readonly . " /></td></tr>";
                            echo "<tr><td>Pomůcky</td><td><input class=\"input-class\" type=\"text\" placeholder=\"Pomůcky\" name=\"pomucky" . $c . "\" value=\"" . $row["pomucky"] . "\" " . $readonly . "></td></tr>";

                            echo "<tr><td>Jmémo výběhu</td><td>";
                            $sql = 'SELECT * FROM Vybeh';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"id_vybehu" . $c . "\" required " . $disabled . ">";
                            while ($row2 = $result2->fetch_array())
                            {
                              $selected = "";
                              if ($row["id_vybehu"] == $row2["id_vybehu"]) { $selected = "selected"; }
                              echo "<option " . $selected . " value=\"" . $row2['id_vybehu'] . "\">" . $row2['typ_vybehu'] . "</option>";
                            }
                            echo "</select>";
                            echo "</td></tr>";

                            echo "<tr><td>Ošetřovatel</td><td>";
                            $sql = 'SELECT * FROM Osetrovatel';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"login" . $c . "\" required " . $disabled . ">";
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
                                <form action="listKeeping.php?name="<?php echo $row["id_cisteni"]; ?>"" method="post">
                                  <input type="hidden" name="id_user" value="<?php echo $row["id_cisteni"]; ?>">
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
      while (isset($_POST['id_cisteni' . $i]))
      {
        $id_cisteni = $_POST['id_cisteni' . $i];
        $delka_cisteni = $_POST['delka_cisteni' . $i];
        $cas = $_POST['cas' . $i];
        $pomucky = $_POST['pomucky' . $i];
        $id_vybehu = $_POST['id_vybehu' . $i];
        $login = $_POST['login' . $i];

        $sql = "UPDATE ZaznamCisteni SET delka_cisteni = \"" . $delka_cisteni . "\" , cas = \"" . $cas . "\" , pomucky = \"" . $pomucky . "\", id_vybehu = \"" . $id_vybehu . "\", login = \"" . $login . "\" WHERE id_cisteni=\"" . $id_cisteni . "\"";
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
        $sql = "DELETE FROM ZaznamCisteni WHERE id_cisteni= \"" . $_POST['id_user'] . "\"";

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

      // Coop filter
      if ($_POST['filterVybeh'] != "")
      {
        $_SESSION['filter'] = '1';
        $_SESSION['filterVybeh'] = $_POST['filterVybeh'];
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
