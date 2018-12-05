<?php
	header("Content-Type: text/html; charset=UTF-8");
?>

<form action='loginBoss.php' method='post'>

	<div class="modal-header">
		<h2>Přihlášení vedoucího</h2>
	</div>

	<div class="modal-body">
		<!-- <p>Jméno</p> -->
			<input class="login-class input-class" type="text" placeholder="Jméno" name="name" id="name" value="" required>
	</div>

	<div class="modal-body">
		<!-- <p>Heslo</p> -->
			<input class="login-class input-class" type="password" placeholder="Heslo" name="psw" id="psw" value="" required>
	</div>

	<div class="modal-body">
		<input class="loginBtn changeButton" type="submit" value="Přihlásit"/>
	</div>
</form>

<?php

	if ($_SERVER['REQUEST_METHOD'] == "POST" and $_POST['name'] != "" && $_POST['psw'] != "")
	{
		include("db.php");
		header("Content-Type: text/html; charset=UTF-8");

		$name = $_POST['name'];
		$psw = $_POST['psw'];

		$result = mysqli_query($db, 'SELECT * FROM VedouciZOO v WHERE v.login="'.$name.'"');

		if(!$result)
		{
      		header("Location: wrongLogin.php");
		}
		else
		{
			$row = $result->fetch_assoc();
			$dbpsw = $row['password'];

			if($psw == $dbpsw)
			{
				session_start();
                $_SESSION['name'] = $name;
                $_SESSION['userlogin'] = $row['userlogin'];
                $_SESSION['psw'] = $row['psw'];
                $_SESSION['surname'] = $row['surname'];
                $_SESSION['degree'] = $row['degree'];
				$_SESSION['birthday'] = $row['birthday'];

				$_SESSION['user'] = 'boss';

				//$_SESSION['time'] = time();
				//$_SESSION['timout'] = 600;

				header("Location: profile.php");
			}
			else
			{
        		header("Location: wrongLogin.php");
			}
		}
	}

?>
