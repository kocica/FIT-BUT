<?php

$HOST   = "localhost:/var/run/mysql/mysql.sock";
$HOST2  = "localhost";
$PSW    = "ijgati3i";
$LOGIN  = "xignac00";

$db = mysqli_init();

if (!$db)	{
  die("mysqli_init failed");
}

if (!mysqli_real_connect($db, $HOST2, $LOGIN, $PSW, $LOGIN, 0, '/var/run/mysql/mysql.sock'))
{
	die("Connect Error: Can not connect to db -" . mysqli_connect_error());
}

$db->set_charset('utf8')

?>
