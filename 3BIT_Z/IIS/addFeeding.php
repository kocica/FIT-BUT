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
           ?>
          <div id="personal">
           Přidat záznam o krmení
          </div>
          <ul>

            <form action="addFeeding.php" method="POST">

                <?php
                    include("db.php");
                ?>

                    <table class="record" style="width:100%">

                    <tr><td><h5>Délka krmení (min):</h5></td><td><input class="input-class" type="number" name="length" min="0" max="180"></td></tr>
                    <tr><td><h5>Čas krmení:</h5></td><td><input class="input-class" type="date" name="time"></td></tr>
                    <tr><td><h5>Množství krmení (kg):</h5></td><td><input class="input-class" type="number" placeholder="Množství krmení" name="amount" min="0" max="100"></td></tr>
                    <tr><td><h5>* Zvíře:</h5></td><td>
                    <?php
                      $sql = 'SELECT * FROM Zvire';
                      $result = mysqli_query($db, $sql);
                      if (($result) === false)
                      {
                        $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                         header("Location: showError.php"); die("");
                      }
                      echo "<select class=\"input-class selector-class\" id=\"id\" name=\"id\" required>";
                      while ($row = $result->fetch_array())
                      {
                        echo "<option value=\"" . $row['id_zvire'] . "\">" . $row['jmeno'] . "</option>";
                      }
                      echo "</select>";
                    ?>
                    </td></tr>

                    <?php
                    if ($_SESSION['user'] == 'boss')
                    {
                      ?>
                      <tr><td><h5>* Login ošetřovatele:</h5></td><td>
                      <?php
                        $sql = 'SELECT * FROM Osetrovatel';
                        $result = mysqli_query($db, $sql);
                        if (($result) === false)
                        {
                          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
                           header("Location: showError.php"); die("");
                        }
                        echo "<select class=\"input-class selector-class\" name=\"login\" required>";
                        while ($row = $result->fetch_array())
                        {
                          echo "<option value=\"" . $row['login'] . "\">" . $row['login'] . "</option>";
                        }
                        echo "</select>";
                      ?>
                      </td></tr>
                      <?php
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
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['length']))
    {
        //$id_feeding = $_POST['id_feeding'];
        $length = $_POST['length'];
        $time = $_POST['time'];
        $amount = $_POST['amount'];
        $id = $_POST['id'];

        $login = $_SESSION['name'];
        if ($_SESSION['user'] == 'boss')
        {
          $login = $_POST['login'];
        }

        // Check existing ID
        $sql = 'SELECT * FROM Zvire z WHERE z.id_zvire="'. $id .'"';
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addFeeding.php'; header("Location: showError.php"); die("");
        }

        $sql = "SELECT * FROM ZaznamKrmeni";
        $result = mysqli_query($db, $sql);
        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addFeeding.php'; header("Location: showError.php"); die("");
        }

        $id_feeding = 0;
        while($row = $result->fetch_array())
        {
          $id_feeding = $row['id_krmeni'];
        }
        $id_feeding++;
        //$id_feeding = $result->num_rows;

        $sql = "INSERT INTO ZaznamKrmeni(id_krmeni, delka_krmeni, cas, mnozstvi_krmeni, id_zvire, login)
               VALUES('$id_feeding', '$length', '$time', '$amount', '$id', '$login')";

        $result = mysqli_query($db, $sql);

        if (($result) === false)
        {
          $_SESSION['msg'] = 'Chyba databáze. Nepodařilo se načíst nebo uložit data.';
          $_SESSION['location'] = 'addFeeding.php'; header("Location: showError.php"); die("");
        }
        $_SESSION['saved'] = '1';
        header("Refresh:0");
    }

?>
