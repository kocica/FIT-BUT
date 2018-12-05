<?php
    ob_start();
    header("Content-Type: text/html; charset=UTF-8");
?>

  <!-- POP for cleaningMenu -->

  <div id="modalCleaning" class="modal">
    <!-- Modal content -->
    <div class="modal-content">
    <span class="close">&times;</span>

        <div class="modal-header">
          <h2>Vyberte si aku akciu chcete vykonat</h2>
        </div>

        <table>
          <?php
          if ($_SESSION['user'] == 'boss')
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/modifyCleaning.png\" class=\"image\" alt=\"Upravit zaznamy o cisteni\" height=\"50\" width=\"50\" onclick=\"document.location.href='listKeeping.php'\"></div></th>";
              echo '<th><form action=\'listKeeping.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Upraviť záznamy o čištění" /></form></th>';
            echo "</tr>";
          }
          else if ($_SESSION['user'] == 'keeper')
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/listIcon.png\" class=\"image\" alt=\"Zobrazit zaznamy o cisteni\" height=\"50\" width=\"50\" onclick=\"document.location.href='listKeeping.php'\"></div></th>";
              echo '<th><form action=\'listKeeping.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Zobraziť záznamy o čištění" /></form></th>';
            echo "</tr>";
          }
          ?>
          <tr>
            <th>
              <div class="container cleaningMenu"><img src="images/addCleaningRecord.png" class="image" alt="Zaznamy o cisteni" height="50" width="50" onclick="document.location.href='addKeeping.php'"></div>
            </th>
            <th>
              <form action='addKeeping.php' method='post'><input class="textmenu" type="submit" name="zamestnanci" value="Pridať záznam o čistení" /></form>
            </th>
          </tr>
        </table>
    </div>
  </div>

  <!-- POP for feedingMenu -->
  <div id="modalFeeding" class="modal">
    <!-- Modal content -->
    <div class="modal-content">
    <span class="close">&times;</span>

        <div class="modal-header">
          <h2>Vyberte si aku akciu chcete vykonat</h2>
        </div>

        <table>
          <?php
          if ($_SESSION['user'] == 'boss')
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/modifyCleaning.png\" class=\"image\" alt=\"Upravit zaznamy o krmeni\" height=\"50\" width=\"50\" onclick=\"document.location.href='listFeeding.php'\"></div></th>";
              echo '<th><form action=\'listFeeding.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Upraviť záznamy o krmení" /></form></th>';
            echo "</tr>";
          }
          else if ($_SESSION['user'] == "keeper")
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/listIcon.png\" class=\"image\" alt=\"Zobrazit zaznamy o krmeni\" height=\"50\" width=\"50\" onclick=\"document.location.href='listFeeding.php'\"></div></th>";
              echo '<th><form action=\'listFeeding.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Zobraziť záznamy o krmení" /></form></th>';
            echo "</tr>";
          }
          ?>
          <tr>
            <th>
              <div class="container feedingMenu"><img src="images/addCleaningRecord.png" class="image" alt="Zaznamy o krmeni" height="50" width="50" onclick="document.location.href='addFeeding.php'"></div>
            </th>
            <th>
              <form action='addFeeding.php' method='post'><input class="textmenu" type="submit" name="zamestnanci" value="Pridať záznam o krmeni" /></form>
            </th>
          </tr>
        </table>
    </div>
  </div>

  <!-- POP for animals record -->
  <div id="modalAnimal" class="modal">
    <!-- Modal content -->
    <div class="modal-content">
    <span class="close">&times;</span>

        <div class="modal-header">
          <h2>Vyberte si aku akciu chcete vykonat</h2>
        </div>

        <table>
          <?php
          if ($_SESSION['user'] == 'boss')
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/modifyCleaning.png\" class=\"image\" alt=\"Upravit zaznamy o zviratech\" height=\"50\" width=\"50\" onclick=\"document.location.href='listAnimals.php'\"></div></th>";
              echo '<th><form action=\'listAnimals.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Upraviť záznamy o zviratech" /></form></th>';
            echo "</tr>";
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/modifyCleaning.png\" class=\"image\" alt=\"Upravit zaznamy o rasách\" height=\"50\" width=\"50\" onclick=\"document.location.href='listRaces.php'\"></div></th>";
              echo '<th><form action=\'listRaces.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Upraviť záznamy o rasách" /></form></th>';
          echo "</tr>";
          }
          else if ($_SESSION['user'] == "keeper")
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/listIcon.png\" class=\"image\" alt=\"Zobrazit zaznamy o zviratech\" height=\"50\" width=\"50\" onclick=\"document.location.href='listAnimals.php'\"></div></th>";
              echo '<th><form action=\'listAnimals.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Zobraziť záznamy o zviratech" /></form></th>';
            echo "</tr>";
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/listIcon.png\" class=\"image\" alt=\"Zobrazit zaznamy o rasách\" height=\"50\" width=\"50\" onclick=\"document.location.href='listRaces.php'\"></div></th>";
              echo '<th><form action=\'listRaces.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Zobraziť záznamy o rasách" /></form></th>';
            echo "</tr>";
          }
          ?>
          <tr>
            <th>
              <div class="container animalMenu"><img src="images/addCleaningRecord.png" class="image" alt="Zaznamy o zvieratach" height="50" width="50" onclick="document.location.href='addAnimal.php'"></div>
            </th>
            <th>
              <form action='addAnimal.php' method='post'><input class="textmenu" type="submit" name="zvierata" value="Pridať záznam o zvieratach" /></form>
            </th>
        </tr>
        <tr>
            <?php
            if ($_SESSION['user'] == 'boss')
            {
              ?>

              <th>
                <div class="container animalMenu"><img src="images/addCleaningRecord.png" class="image" alt="Zaznamy o rase" height="50" width="50" onclick="document.location.href='addRace.php'"></div>
              </th>
          
              <th>
                <form action='addRace.php' method='post'><input class="textmenu" type="submit" name="zvierata" value="Pridať záznam o rase" /></form>
              </th>
              
              <?php
            }
            ?>

          </tr>
        </table>
    </div>
  </div>

  <!-- POP for animals record -->
  <div id="modalFence" class="modal">
    <!-- Modal content -->
    <div class="modal-content">
    <span class="close">&times;</span>

        <div class="modal-header">
          <h2>Vyberte si aku akciu chcete vykonat</h2>
        </div>

        <table>
          <?php
          if ($_SESSION['user'] == 'boss')
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/modifyCleaning.png\" class=\"image\" alt=\"Upravit zaznamy o vybezich\" height=\"50\" width=\"50\" onclick=\"document.location.href='listCoops.php'\"></div></th>";
              echo '<th><form action=\'listCoops.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Upraviť záznamy o vybezich" /></form></th>';
            echo "</tr>";
          }
          else if ($_SESSION['user'] == "keeper")
          {
            echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/listIcon.png\" class=\"image\" alt=\"Zobrazit zaznamy o vybezich\" height=\"50\" width=\"50\" onclick=\"document.location.href='listCoops.php'\"></div></th>";
              echo '<th><form action=\'listCoops.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Zobraziť záznamy o vybezich" /></form></th>';
            echo "</tr>";
          }
          ?>
          <tr>
            <th>
              <div class="container fenceMenu"><img src="images/addCleaningRecord.png" class="image" alt="Zaznamy o vybezich" height="50" width="50" onclick="document.location.href='addCoop.php'"></div>
            </th>
            <th>
              <form action='addCoop.php' method='post'><input class="textmenu" type="submit" name="vybehy" value="Pridať záznam o vybezich" /></form>
            </th>
          </tr>
        </table>
    </div>
  </div>



  <script src="application/popout.js" type="text/javascript"></script>
