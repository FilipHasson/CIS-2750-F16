<?php
	$output = shell_exec('./a4 -store files/'.$_POST['s1']);
    header("Location: ./a4.php");
    exit;
?>
