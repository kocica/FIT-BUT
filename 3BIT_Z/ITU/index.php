<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="styles/styles.css">
        <title>Aplikace pro zlepšení mentálních schopností seniorů</title>
    </head>

    <?php
        header("Content-Type: text/html; charset=UTF-8");
    ?>

	<body>

			<div class="button-group">

				<div id="buttons" class="wrapper">

				  <div class="one">
				  	<button class="double" onClick="document.location.href='application/quiz.php'">
				  		<img src="img/quiz.png" width="100" height="100"><br><h1>Kvíz</h1>
				  	</button>
				  </div>

				  <div class="three">
				  	<button class="double" onClick="document.location.href='application/iq_test.php'">
				  		<img src="img/test.png" width="100" height="100"><br><h1>IQ Test</h1>
				  	</button>
				  </div>

				  <div class="five">
				  	<button class="double" onClick="document.location.href='application/nature.php'">
				  		<img src="img/nature.png" width="100" height="100"><br><h1>Příroda</h1>
				  	</button>
				  </div>

				  <div class="four">
				  	<button class="double" onClick="document.location.href='application/music.php'">
				  		<img src="img/music.png" width="100" height="100"><br><h1>Hudba</h1>
				  	</button>
				  </div>

				  <div class="two">
				  	<button class="double" class="double" onClick="document.location.href='application/music_test.php'">
				  		<img src="img/music_test.png" width="100" height="100"><br><h1>Hudební test</h1>
				  	</button>
				  </div> 

				</div>

			<script src="js/logic.js" type="text/javascript"></script>
			  
			</div>

    </body>

</html>