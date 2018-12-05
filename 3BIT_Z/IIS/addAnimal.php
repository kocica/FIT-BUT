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
           Přidat záznam o zvířeti
          </div>
          <ul>

            <form action="addAnimal.php" method="POST">

                <?php
                    include("db.php");
                    include("isInSession.php");
                ?>

                    <table class="record" style="width:100%">

                    <?php
                    $jmenoZvirete = isInSession('jmenoZvirete');
                    $rasa = isInSession('rasa');
                    $puvod = isInSession('puvod');
                    $narozeni = isInSession('narozeni');
                    $umrti = isInSession('umrti');
                    $vyskyt = isInSession('vyskyt');
                    $vybeh = isInSession('vybeh');
                    $rodic = isInSession('rodic');

                    echo "<tr><td><h5>* Jméno zvířete: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Jméno zvířete\" name=\"animal_name\" value=\"" . $jmenoZvirete . "\" required></td></tr>";

                    echo "<tr><td><h5>* Rasa:</h5></td><td>";

                    $sql = 'SELECT * FROM Rasa';
                    $result = mysqli_query($db, $sql);
                    if (($result) === false)
                    {
                      $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                      $_SESSION['location'] = 'addAnimal.php';
                      header("Location: showError.php"); die("");
                    }
                    echo "<select class=\"input-class selector-class\" name=\"rasa\" required>";
                    while ($row = $result->fetch_array())
                    {
                      $selected = "";
                      if ($rasa == $row["jmeno"]) { $selected = "selected"; }
                      echo "<option " . $selected . " value=\"" . $row['jmeno'] . "\">" . $row['jmeno'] . "</option>";
                    }
                    echo "</select>";

                    echo "</td></tr>";
                    echo "<tr><td><h5>Krajina původu: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Krajina původu\" name=\"country\" value=\"" . $puvod . "\"></td></tr>";
                    echo "<tr><td><h5>Datum narození: </h5></td><td><input class=\"input-class\" type=\"date\" placeholder=\"Datum narození\" name=\"birthday\" value=\"" . $narozeni . "\"/></td></tr>";
                    echo "<tr><td><h5>Datum úmrtí: </h5></td><td><input class=\"input-class\" type=\"date\" placeholder=\"Datum úmrtí\" name=\"dieday\" value=\"" . $umrti . "\"/></td></tr>";
                    echo "<tr><td><h5>Oblast výskytu: </h5></td><td><input class=\"input-class\" type=\"text\" placeholder=\"Oblast výskytu\" name=\"vyskyt\" value=\"" . $vyskyt . "\"></td></tr>";
                    echo "<tr><td><h5>* Výběh:</h5></td><td>";

                    $sql = 'SELECT * FROM Vybeh';
                    $result = mysqli_query($db, $sql);
                    if (($result) === false)
                    {
                      $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                      $_SESSION['location'] = 'addAnimal.php';
                      header("Location: showError.php"); die("");
                    }
                    echo "<select class=\"input-class selector-class\" name=\"id_vybeh\" required>";
                    while ($row = $result->fetch_array())
                    {
                      $selected = "";
                      if ($vybeh == $row["id_vybehu"]) { $selected = "selected"; }
                      echo "<option " . $selected . " value=\"" . $row['id_vybehu'] . "\">" . $row['typ_vybehu'] . "</option>";
                    }
                    echo "</select>";

                    echo "</td></tr>";
                    echo "<tr><td><h5>Rodič:</h5></td><td>";

                    $sql = 'SELECT * FROM Zvire';
                    $result = mysqli_query($db, $sql);
                    if (($result) === false)
                    {
                      $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                      $_SESSION['location'] = 'addAnimal.php';
                      header("Location: showError.php"); die("");
                    }
                    echo "<select class=\"input-class selector-class\" name=\"id_parent\">";
                    echo "<option value=\"0\"></option>";
                    while ($row = $result->fetch_array())
                    {
                      $selected = "";
                      if ($rodic == $row["id_zvire"]) { $selected = "selected"; }
                      echo "<option " . $selected . " value=\"" . $row['id_zvire'] . "\">" . $row['jmeno'] . "</option>";
                    }
                    echo "</select>";
                    ?>

                    </td></tr>
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
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['country']))
    {
        //$id_animal = $_POST['id_animal'];
        $animal_name = $_POST['animal_name'];
        $rasa = $_POST['rasa'];
        $country = $_POST['country'];
        $birthday = $_POST['birthday'];
        $dieday = $_POST['dieday'];
        $vyskyt = $_POST['vyskyt'];
        $id_vybeh = $_POST['id_vybeh'];
        if (isset($_POST['id_parent']))
        {
          $id_parent = $_POST['id_parent'];
        }
        else
        {
          $id_parent = "";
        }

        // Check existing ID
        $sql = 'SELECT * FROM Vybeh v WHERE v.id_vybehu="'. $id_vybeh .'"';
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addAnimal.php';
          header("Location: showError.php"); die("");
        }

        $sql = "SELECT * FROM Zvire";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addAnimal.php';
          header("Location: showError.php"); die("");
        }

        $id_animal = 0;
        while($row = $result->fetch_array())
        {
          $id_animal = $row['id_zvire'];
        }
        $id_animal++;

        // Check
        $sql = 'SELECT * FROM Vybeh v WHERE v.id_vybehu="'. $id_vybeh .'"';
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addAnimal.php';
            header("Location: showError.php"); die("");
        }
        $row = $result->fetch_array();

        /*if ($row['id_zvire1'] == $id_animal || $row['id_zvire2'] == $id_animal || $row['id_zvire3'] == $id_animal)
        {
          die ("Zvire uz ve specifikovanem vybehu je.");
        }*/

        if($row['id_zvire1'] == '0')
        {
          $sql = "UPDATE Vybeh SET id_zvire1 = \"" . $id_animal . "\" WHERE id_vybehu=\"" . $id_vybeh . "\"";
          $result = mysqli_query($db, $sql);
          if (($result) === false)
          {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            $_SESSION['location'] = 'addAnimal.php';
            header("Location: showError.php"); die("");
          }
        }
        else if($row['id_zvire2'] == '0')
        {
          $sql = "UPDATE Vybeh SET id_zvire2 = \"" . $id_animal . "\" WHERE id_vybehu=\"" . $id_vybeh . "\"";
          $result = mysqli_query($db, $sql);
          if (($result) === false)
          {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            $_SESSION['location'] = 'addAnimal.php';
            header("Location: showError.php"); die("");
          }
        }
        else if($row['id_zvire3'] == '0')
        {
          $sql = "UPDATE Vybeh SET id_zvire3 = \"" . $id_animal . "\" WHERE id_vybehu=\"" . $id_vybeh . "\"";
          $result = mysqli_query($db, $sql);
          if (($result) === false)
          {
            $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
            $_SESSION['location'] = 'addAnimal.php';
            header("Location: showError.php"); die("");
          }
        }
        else
        {
          $_SESSION['msg'] = 'Ve výběhu již není místo.';
          $_SESSION['error'] = '1';
          $_SESSION['jmenoZvirete'] = $_POST['animal_name'];
          $_SESSION['rasa'] = $_POST['rasa'];
          $_SESSION['puvod'] = $_POST['country'];
          $_SESSION['narozeni'] = $_POST['birthday'];
          $_SESSION['umrti'] = $_POST['dieday'];
          $_SESSION['vyskyt'] = $_POST['vyskyt'];
          //$_SESSION['vybeh'] = $_POST['id_vybeh'];
          $_SESSION['rodic'] = $_POST['id_parent'];
          header("Refresh:0"); die("");
        }


        $sql = "INSERT INTO Zvire(id_zvire, rasa, jmeno, krajina_puvodu, datum_narozeni, datum_umrti, oblast_vyskytu, id_vybehu, id_rodic)
               VALUES('$id_animal', '$rasa', '$animal_name', '$country', '$birthday', '$dieday', '$vyskyt', '$id_vybeh', '$id_parent')";

        $result = mysqli_query($db, $sql);

        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addAnimal.php';
            header("Location: showError.php"); die("");
        }
        $_SESSION['saved'] = '1';
        header("Refresh:0");
    }

?>
