<?php
	$output = shell_exec('./a4 -fetch '.$_GET['theChosenOne']);
	$fp = fopen("html.tmp","r");
	while (($line = fgets($fp)) !== false) {
		echo $line;
    }
    fclose($fp);
    unlink("html.tmp");	
?>