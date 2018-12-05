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
              Záznamy byly úspěšně upraveny.
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
            Záznamy zvířat
          </div>

          <?php include("db.php"); ?>

          <!-- FILTR -->
          <fieldset>
            <legend>Filtr</legend>
            <form action="listAnimals.php" method="POST">
              <table>
                <tbody>
                  <tr><th>Rasa: </th><th><?php
                    $sql = 'SELECT * FROM Rasa';
                    $result = mysqli_query($db, $sql);
                    echo "<select class=\"input-class selector-class\" name=\"filterLogin\">";
                    echo "<option value=\"\"></option>";
                    while ($row = $result->fetch_array())
                    {
                      echo "<option value=\"" . $row['jmeno'] . "\">" . $row['jmeno'] . "</option>";
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

                  <tr><th>Potomci zvířete: </th><th><?php
                    $sql = 'SELECT * FROM Zvire';
                    $result = mysqli_query($db, $sql);
                    echo "<select class=\"input-class selector-class\" name=\"filterRodic\">";
                    echo "<option value=\"\"></option>";
                    while ($row = $result->fetch_array())
                    {
                      echo "<option value=\"" . $row['id_zvire'] . "\">" . $row['jmeno'] . "</option>";
                    }
                    echo "</select>";
                  ?></th></tr>

                  <tr><th>Datum narození (od-do): </th><?php
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
                <form action="listAnimals.php" method="POST">
                    <?php
                        // FILTER
                        $where = "";
                        $filtrOsetr = "";
                        $filterVybeh = "";
                        $filterRodic = "";
                        $filterDate = "";
                        $and = "";
                        if (isset($_SESSION['filter']))
                        {
                          $where = " WHERE ";

                          if (isset($_SESSION['filterLogin']))
                          {
                            $filtrOsetr = "rasa=\"" . $_SESSION['filterLogin'] . "\"";
                            $and = " AND ";

                            unset($_SESSION['filterLogin']);
                          }
                          if (isset($_SESSION['filterVybeh']))
                          {
                            $filterVybeh = $and . "id_vybehu=\"" . $_SESSION['filterVybeh'] . "\"";
                            $and = " AND ";

                            unset($_SESSION['filterVybeh']);
                          }
                          if (isset($_SESSION['filterRodic']))
                          {
                            $filterRodic = $and . "id_rodic=\"" . $_SESSION['filterRodic'] . "\"";
                            $and = " AND ";

                            unset($_SESSION['filterRodic']);
                          }
                          if (isset($_SESSION['filterOd']) && isset($_SESSION['filterDo']))
                          {

                            $filterDate = $and . "datum_narozeni BETWEEN \"" . $_SESSION['filterOd'] . "\" AND \"" . $_SESSION['filterDo'] . "\"";

                            unset($_SESSION['filterOd']); unset($_SESSION['filterDo']);
                          }

                          unset($_SESSION['filter']);
                        }
                        // ! FILTER

                        $sql = "SELECT * FROM Zvire" . $where . $filtrOsetr . $filterVybeh . $filterRodic . $filterDate;
                        $result = mysqli_query($db, $sql);

                        $c = 0;

                        $readonly = "";
                        $disabled = "";
                        if ($_SESSION['user'] == 'keeper')
                        {
                          $readonly = "readonly";
                          $disabled = "disabled=\"true\"";
                        }

                        while($row = $result->fetch_array())
                        {
                            echo "<table class=\"record\";>";

                            echo "<tr>";
                            echo "<th>";
                            echo "<tr><td><img src=\"images/animals.png\" class=\"image\" alt=\"Obrazek zvirete\" height=\"150\" width=\"150\"\></td></tr>";
                            echo "</th>";
                            echo "<tr><td style=\"display: none\"><input class=\"setWidth input-class\" type=\"text\" name=\"id_zvire" . $c . "\" value=\"" . $row["id_zvire"] . "\" readonly></td></tr>";
                            echo "<tr><td>Jméno zvířete</td><td><input class=\"setWidth input-class\" type=\"text\" placeholder=\"Jméno zvířete\" name=\"jmeno" . $c . "\" value=\"" . $row["jmeno"] . "\" readonly></td></tr>";

                            echo "<tr><td>Rasa zvířete</td><td>";
                            $sql = 'SELECT * FROM Rasa';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"rasa" . $c . "\" required " . $disabled . ">";
                            echo "<option value=\"0\">Neznámá</option>";
                            while ($row2 = $result2->fetch_array())
                            {
                              $selected = "";
                              if ($row["rasa"] == $row2["jmeno"]) { $selected = "selected"; }
                              echo "<option " . $selected . " value=\"" . $row2['jmeno'] . "\">" . $row2['jmeno'] . "</option>";
                            }
                            echo "</select>";
                            echo "</td></tr>";

                            echo "<tr><td>Země původu</td><td><input class=\"setWidth input-class\" type=\"text\" placeholder=\"Země původu\" name=\"krajina_puvodu" . $c . "\" value=\"" . $row["krajina_puvodu"] . "\" " . $readonly . "></td></tr>";
                            echo "<tr><td>Datum narození</td><td><input class=\"setWidth input-class\" type=\"date\" placeholder=\"Datum narození\" name=\"datum_narozeni" . $c . "\" value=\"" . $row['datum_narozeni'] . "\" " . $readonly . "/></td></tr>";
                            echo "<tr><td>Datum úmrtí</td><td><input class=\"setWidth input-class\" type=\"date\" placeholder=\"Datum úmrtí\" name=\"datum_umrti" . $c . "\" value=\"" . $row['datum_umrti'] . "\" " . $readonly . "/></td></tr>";
                            echo "<tr><td>Oblast výskytu</td><td><input class=\"setWidth input-class\" type=\"text\" placeholder=\"Oblast výskytu\" name=\"oblast_vyskytu" . $c . "\" value=\"" . $row["oblast_vyskytu"] . "\" " . $readonly . "></td></tr>";
                            //echo "<td><input type=\"text\" name=\"id_vybehu" . $c . "\" value=\"" . $row["id_vybehu"] . "\"></td>";
                            echo "<tr><td>Jméno výběhu</td><td>";
                            $sql = 'SELECT * FROM Vybeh';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"id_vybehu" . $c . "\" required " . $disabled . ">";
                            echo "<option value=\"0\">Dočasný výběh</option>";
                            while ($row2 = $result2->fetch_array())
                            {
                              $selected = "";
                              if ($row["id_vybehu"] == $row2["id_vybehu"]) { $selected = "selected"; }
                              echo "<option " . $selected . " value=\"" . $row2['id_vybehu'] . "\">" . $row2['typ_vybehu'] . "</option>";
                            }
                            echo "</select>";
                            echo "</td></tr>";

                            //echo "<td><input type=\"text\" name=\"id_rodic" . $c . "\" value=\"" . $row["id_rodic"] . "\"></td>";
                            echo "<tr><td>Jméno rodiče</td><td>";
                            $sql = 'SELECT * FROM Zvire';
                            $result2 = mysqli_query($db, $sql);
                            if (($result2) === false)
                            {
                              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                              header("Location: showError.php"); die("");
                            }
                            echo "<select class=\"input-class selector-class\" name=\"id_rodic" . $c . "\" " . $disabled . ">";
                            echo "<option value=\"0\"></option>";
                            while ($row2 = $result2->fetch_array())
                            {
                              $selected = "";
                              if ($row["id_rodic"] == $row2["id_zvire"]) { $selected = "selected"; }
                              echo "<option " . $selected . " value=\"" . $row2['id_zvire'] . "\">" . $row2['jmeno'] . "</option>";
                            }
                            echo "</select>";
                            echo "</td></tr>";

                            if ($_SESSION['user'] == 'boss')
                            {
                              ?>
                              <td>
                              <form action="listAnimals.php?name="<?php echo $row["id_zvire"]; ?>"" method="post">
                                <input type="hidden" name="id_user" value="<?php echo $row["id_zvire"]; ?>">
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
      while (isset($_POST['id_zvire' . $i]))
      {
        $id_zvire = $_POST['id_zvire' . $i];
        $jmeno = $_POST['jmeno' . $i];
        $rasa = $_POST['rasa' . $i];
        $krajina_puvodu = $_POST['krajina_puvodu' . $i];
        $datum_narozeni = $_POST['datum_narozeni' . $i];
        $datum_umrti = $_POST['datum_umrti' . $i];
        $oblast_vyskytu = $_POST['oblast_vyskytu' . $i];
        $id_vybehu = $_POST['id_vybehu' . $i];
        $id_rodic = $_POST['id_rodic' . $i];

        if ($id_zvire == $id_rodic)
        {
          $_SESSION['msg'] = 'Zvíře nemůže být rodič sám sobě.';
          $_SESSION['error'] = '1';
          header("Refresh:0"); die("");
        }

        $sql = 'SELECT * FROM Vybeh v WHERE v.id_vybehu="'. $id_vybehu .'"';
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }
        $row = $result->fetch_array();

        if ($row['id_zvire1'] == $id_zvire || $row['id_zvire2'] == $id_zvire || $row['id_zvire3'] == $id_zvire)
        {
          // Zvire uz ve specifikovanem vybehu je
        }
        else if ($id_vybehu == '0')
        {
          // Docasny vybeh
          $sql = "UPDATE Vybeh SET id_zvire1 = 0 WHERE id_zvire1=\"" . $id_zvire . "\"";
          $result = mysqli_query($db, $sql);
          $sql = "UPDATE Vybeh SET id_zvire2 = 0 WHERE id_zvire2=\"" . $id_zvire . "\"";
          $result = mysqli_query($db, $sql);
          $sql = "UPDATE Vybeh SET id_zvire3 = 0 WHERE id_zvire3=\"" . $id_zvire . "\"";
          $result = mysqli_query($db, $sql);
        }
        else
        {
          if($row['id_zvire1'] == '0')
          {
            $sql = "UPDATE Vybeh SET id_zvire1 = 0 WHERE id_zvire1=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);
            $sql = "UPDATE Vybeh SET id_zvire2 = 0 WHERE id_zvire2=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);
            $sql = "UPDATE Vybeh SET id_zvire3 = 0 WHERE id_zvire3=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);

            $sql = "UPDATE Vybeh SET id_zvire1 = \"" . $id_zvire . "\" WHERE id_vybehu=\"" . $id_vybehu . "\"";
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
            }
          }
          else if($row['id_zvire2'] == '0')
          {
            $sql = "UPDATE Vybeh SET id_zvire1 = 0 WHERE id_zvire1=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);
            $sql = "UPDATE Vybeh SET id_zvire2 = 0 WHERE id_zvire2=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);
            $sql = "UPDATE Vybeh SET id_zvire3 = 0 WHERE id_zvire3=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);

            $sql = "UPDATE Vybeh SET id_zvire2 = \"" . $id_zvire . "\" WHERE id_vybehu=\"" . $id_vybehu . "\"";
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
            }
          }
          else if($row['id_zvire3'] == '0')
          {
            $sql = "UPDATE Vybeh SET id_zvire1 = 0 WHERE id_zvire1=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);
            $sql = "UPDATE Vybeh SET id_zvire2 = 0 WHERE id_zvire2=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);
            $sql = "UPDATE Vybeh SET id_zvire3 = 0 WHERE id_zvire3=\"" . $id_zvire . "\"";
            $result = mysqli_query($db, $sql);

            $sql = "UPDATE Vybeh SET id_zvire3 = \"" . $id_zvire . "\" WHERE id_vybehu=\"" . $id_vybehu . "\"";
            $result = mysqli_query($db, $sql);
            if (($result) === false)
            {
              $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
              header("Location: showError.php"); die("");
            }
          }
          else
          {
            $_SESSION['msg'] = 'Ve výběhu již není místo.';
            $_SESSION['error'] = '1';
            header("Refresh:0"); die("");
          }
        }

        $sql = "UPDATE Zvire SET jmeno = \"" . $jmeno . "\", rasa = \"" . $rasa . "\", krajina_puvodu = \"" . $krajina_puvodu . "\" , datum_narozeni = \"" . $datum_narozeni . "\" , datum_umrti = \"" . $datum_umrti . "\", oblast_vyskytu = \"" . $oblast_vyskytu . "\", id_vybehu = \"" . $id_vybehu . "\", id_rodic = \"" . $id_rodic . "\" WHERE id_zvire=\"" . $id_zvire . "\"";
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
        $sql = "DELETE FROM Zvire WHERE id_zvire= \"" . $_POST['id_user'] . "\"";

        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // Remove feeding of the animal
        $sql = "DELETE FROM ZaznamKrmeni WHERE id_zvire=" . $_POST['id_user'];
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // If the animal is parent, remove the parent record
        $sql = "UPDATE Zvire SET id_rodic=0 WHERE id_rodic=" . $_POST['id_user'];
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }

        // If the animal is in some coop, remove the record
        $sql = "UPDATE Vybeh SET id_zvire1=0 WHERE id_zvire1=" . $_POST['id_user'];
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }
        $sql = "UPDATE Vybeh SET id_zvire2=0 WHERE id_zvire2=" . $_POST['id_user'];
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            header("Location: showError.php"); die("");
        }
        $sql = "UPDATE Vybeh SET id_zvire3=0 WHERE id_zvire3=" . $_POST['id_user'];
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

      // Parent filter
      if ($_POST['filterRodic'] != "")
      {
        $_SESSION['filter'] = '1';
        $_SESSION['filterRodic'] = $_POST['filterRodic'];
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
