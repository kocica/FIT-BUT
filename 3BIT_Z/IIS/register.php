<script src="application/passmatch.js" type="text/javascript"></script>
<script src="application/model.js" type="text/javascript"></script>
<form action='register.php' method='post'>
	<div class="modal-header">
		<h2>Registrace</h2>
	</div>

	<?php
		if(!session_id())
		{
			session_start();
		}
	?>

	<div class="modal-body">
			<?php
				$name = "";
				if (session_id())
				{
					if (isset($_SESSION['name']))	{
						$name = $_SESSION['name'];
					}
				}
				echo "<input class=\"input-class login-class\" type=\"text\" placeholder=\"* Jméno\" name=\"name\" id=\"name\" class=\"UserName\" value=\"" . $name . "\" required>";
			?>
	</div>

    <div class="modal-body">
			<?php
				$surname = "";
				if (session_id())
				{
					if(isset($_SESSION['surname']))	{
						$surname = $_SESSION['surname'];
					}
				}
				echo "<input class=\"input-class login-class UserName\" type=\"text\" name=\"surname\" id=\"surname\" placeholder=\"* Příjmení\" value=\"" . $surname . "\" required>";
			?>
	</div>

	<div class="modal-body">
			<input class="input-class login-class loginCheck" type="text" name="userlogin" id="userlogin" placeholder="* Login" value="" onkeyup='loginControl();' required> <span id='messageLoginShort'></span>
	</div>

  <div class="modal-body">
			<input class="input-class login-class password" type="password" name="psw" id="psw" placeholder="* Heslo" value="" onkeyup='check();' required> <span id='messageShort'></span>
	</div>

  <div class="modal-body">
			<input class="input-class login-class confirm_password" type="password" name="psw-repeat" id="psw-repeat" placeholder="* Heslo znovu" value="" onkeyup='check();' required> <span id='message'></span>
	</div>

  <div class="modal-body">
		<label>* Datum narození</label>
		<?php
			$birthday = date('Y-m-d');
			if (session_id())
			{
				if(isset($_SESSION['birthday']))	{
					$birthday = $_SESSION['birthday'];
				}
			}
			echo "<input class=\"input-class login-class\" type=\"date\" name=\"birthday\" id=\"birthday\" value=\"".$birthday . "\"  required/>";
		?>
	</div>

    <div class="modal-body">
			<?php
				$degree = "";
				if (session_id())
				{
					if(isset($_SESSION['degree']))	{
						$degree = $_SESSION['degree'];
					}
				}
				echo "<input class=\"input-class UserName login-class\" type=\"text\" placeholder=\"Tituly\" name=\"degree\" id=\"degree\" value=\"" . $degree . "\">";
			?>
	</div>

    <div class="modal-body">
		<?php
			$value = '0';
			if (session_id())
			{
				if(isset($_SESSION['spec']))	{
					$value = $_SESSION['spec'];
				}
			}

			echo "<select class=\"selector-class login-class\" id=\"cmbMake\" name=\"specialization\" required>";
				echo "<option " . ($value === '0' ? "selected" : " ") . " value=\"0\">Savci</option>";
				echo "<option " . ($value === '1' ? "selected" : " ") . " value=\"1\">Ptáci</option>";
				echo "<option " . ($value === '2' ? "selected" : " ") . " value=\"2\">Obojživelníci</option>";
				echo "<option " . ($value === '3' ? "selected" : " ") . " value=\"3\">Plazi</option>";
			echo "</select>";
		?>
	</div>
	<div class="modal-body">
		<input class="changeButton loginBtn" id="regButton" type="submit" value="Registrovat"/>
	</div>
</form>

<?php
    if ($_SERVER['REQUEST_METHOD'] == "POST" and $_POST['name'] != "" && $_POST['userlogin'] != "" && $_POST['psw'] != "" && $_POST['psw-repeat'] != ""
    && $_POST['surname'] != "")
	{
		include("db.php");
		header("Content-Type: text/html; charset=UTF-8");

		$name = $_POST['name'];
        $login = $_POST['userlogin'];
        $psw = $_POST['psw'];
        $psw_repeat = $_POST['psw-repeat'];
        $surname = $_POST['surname'];
        $degree = $_POST['degree'];
        $birthday = $_POST['birthday'];

        $special = array(0 => 'Savci',
                    1 => 'Ptáci',
                    2 => 'Obojživelníci',
					3 => 'Plazi');

        $specialization = $special[$_POST['specialization']];

				$_SESSION['name'] = $name;
				$_SESSION['surname'] = $surname;
				$_SESSION['degree'] = $degree;
				$_SESSION['birthday'] = $birthday;
				$_SESSION['spec'] = $_POST['specialization'];

	        $result = mysqli_query($db, 'SELECT * FROM Osetrovatel o WHERE o.login="'.$login.'"');

	        if ($result->fetch_assoc())
	        {
	            header("Location: existingUser.php");
							die();
					}

			$result = mysqli_query($db, 'SELECT * FROM VedouciZOO z WHERE z.login="'.$login.'"');

	        if ($result->fetch_assoc())
	        {
	            header("Location: existingUser.php");
							die();
	        }

	        //rodne_cislo_osetrovatel, prac_doba, login TODO
	        $sql = "INSERT INTO Osetrovatel (password, login, jmeno, prijmeni, datum_narozeni, titul, specializace)
	                VALUES ('$psw', '$login', '$name', '$surname', '$birthday', '$degree', '$specialization')";


	        $result = mysqli_query($db, $sql);

			header("Location: successRegister.php");
		}
?>
