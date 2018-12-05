<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="styles/styles.css">
        <link rel="shortcut icon" type="image/x-icon" href="images/Elephant.ico" />
        <title>Informační systém ZOO</title>
    </head>

    <?php
        header("Content-Type: text/html; charset=UTF-8");
        session_start();
    ?>

    <body>

        <header>
          <div id="element1">
            Informační systém pro správu ZOO
          </div>
        </header>

        <section>
          <nav>
            <ul>
              <!-- Vyhodi to nejake dialogove okno alebo prepoji na prihlasovanie/registrovacie nejake -->
              <li><button class="loginButtons">Přihlásit se jako Vedoucí</button></li>
              <li><button class="loginButtons">Přihlásit se jako Ošetřovatel</button></li>
              <li><button class="loginButtons">Registrovat se jako nový Ošetřovatel</button></li>
            </ul>
          </nav>


          <!-- Modals for logging to system -->

          <!-- The Modal for boss login-->
          <div id="modalBoss" class="modal">
            <!-- Modal content -->
            <div class="modal-content">
            <span class="close">&times;</span>
            <?php
              require_once 'loginBoss.php';
              //require_once 'tables.php';
            ?>
            </div>
          </div>

          <!-- The Modal for keeper login-->
        <div id="modalKeeper" class="modal">
          <!-- Modal content -->
          <div class="modal-content">
          <span class="close">&times;</span>
          <?php
            require_once 'loginKeeper.php';
          ?>
          </div>
        </div>

        <!-- The Modal for registration of new keeper-->
        <div id="modalReg" class="modal">
          <!-- Modal content -->
          <div class="modal-content">
          <span class="close">&times;</span>
          <?php
            require_once 'register.php';
          ?>
          </div>
        </div>

        <script src="application/model.js" type="text/javascript"></script>

        <article>
          <h2>
          Informační systém ZOO
          <br>
          </h2>
          <h1>
          <h2>Pro opravujícího</h2>
          <table>
          <tbody><tr><th>Login</th><th>Heslo</th><th>Role</th></tr>
          <tr><td>admin</td><td>admin123</td><td>Vedoucí ZOO</td></tr>
          <tr><td>osetrovatelA</td><td>osetrovatel123</td><td>Ošetřovatel</td></tr>
          <tr><td>osetrovatelB</td><td>osetrovatel123</td><td>Ošetřovatel</td></tr>
          </tbody></table>
          <br>
          <br>
          <button onclick="location.href = 'doc.html';" id="myButton" class="changeButton loginBtn" >Dokumentace</button>
          </h1>
          </article>
        </section>

        <div class="downsideinfo">
          
        </div>

    </body>
</html>
