<?php
    ob_start();
    header("Content-Type: text/html; charset=UTF-8");
?>

<menu>
  <movingmenu>
    <h1>Správa systému</h1>
    <table>
      <tr>
        <th>
          <div class="container cleaningMenu"><img src="images/cleaning.png" class="image" alt="Zaznamy o cisteni" height="50" width="50"></div>
        </th>
        <th>
          <div class="cleaningMenu"><button class="textmenu">Záznamy čištění</button></div>
        </th>
      </tr>
      <tr>
        <th>
          <div class="container feedingMenu"><img src="images/feeding.png" class="image" alt="Zaznamy o cisteni" height="50" width="50"></div>
        </th>
        <th>
          <div class="feedingMenu"><button class="textmenu">Záznamy krmení</button></div>
        </th>
      </tr>
      <tr>
        <th>
          <div class="container animalMenu"><img src="images/animals.png" class="image" alt="Zaznamy o zviratech" height="50" width="50"></div>
        </th>
        <th>
          <div class="animalMenu"><button class="textmenu">Záznamy zvířat</button></div>
        </th>
      </tr>
      <tr>
        <th>
          <div class="container fenceMenu"><img src="images/fence.png" class="image" alt="Zaznamy o vybezich" height="50" width="50"></div>
        </th>
        <th>
          <div class="fenceMenu"><button class="textmenu">Záznamy výběhů</button></div>
        </th>
      </tr>
      <tr>
        <th>
          <div class="container"><img src="images/personal.png" class="image" alt="Osobne udaje" height="50" width="50" onclick="document.location.href='personal.php'"></div>
        </th>
        <th>
          <form action='personal.php' method='post'><input class="textmenu" type="submit" name="cisteni" value="Osobní údaje" /></form>
        </th>
      </tr>
      <?php
      if ($_SESSION['user'] == 'boss')
      {
          echo "<tr>";
              echo "<th><div class=\"container\"><img src=\"images/keeperInfo.png\" class=\"image\" alt=\"Informacie o osetrovateloch\" height=\"50\" width=\"50\" onclick=\"document.location.href='modifyUsers.php'\"></div></th>";
              echo '<th><form action=\'modifyUsers.php\' method=\'post\'><input class="textmenu" type="submit" name="zamestnanci" value="Zaměstnanci" /></form></th>';
          echo "</tr>";
      }
      ?>
    </table>
  </movingmenu>
</menu>
