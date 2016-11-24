<html>
	<head>
		<title>Assignment 4</title>
		<link rel="stylesheet" type="text/css" href="a4.css"> 
	</head>
	<body>
		<div>
			<!--Top button bar-->
			<div class="fileUpload button button2">
				<form action="upload.php" method="post" enctype="multipart/form-data">
   				<span>Upload</span>
   				<input type="file" class="upload" onchange="this.form.submit()" name="fileToUpload">
				</form>
			</div>
			<div class="fileConvert button button1" id="conB1">
				<span>Convert</span>
				<P> 
			</div>			
			<div class="dbStore button button3" id="dbB">
				<span>DBStore</span>
				<P/>
			</div>
		</div>
		<!--Modal select window convert-->
		<div id="convertModal" class="modal">
  			<div class="modal-content">
    			<span class="close">×</span>
    			<p>Choose a file to convert:</p>
    			<form action="./convert.php" method="post">
    				<select name="s1">
    	  				<option value="" selected="selected"></option>
	  					<?php 
							$files = scandir('./files/');
							$files = array_diff($files, array('.', '..'));
							sort($files);
							foreach($files as $file){
								if (endsWith($file,".html") == false and endsWith($file,".info") == false){
									echo "<option value='".$file."'>".$file."</option>";
								}
							}
						?>
					</select>
					<input type="submit">
					<button type="buttonCancel">Cancel</button>
				</form>
			</div>
		</div>
		<!--Modal select window dbstore-->
		<div id="dbModal" class="modal">
  			<div class="modal-content">
    			<span class="close">×</span>
    			<p>Choose a file to store:</p>
    			<form action="./dbstore.php" method="post">
    				<select name="s1">
    	  				<option value="" selected="selected"></option>
	  					<?php 
							$files = scandir('./files/');
							$files = array_diff($files, array('.', '..'));
							sort($files);
							foreach($files as $file){
								if (endsWith($file,".html") == false and endsWith($file,".info") == false){
									echo "<option value='".$file."'>".$file."</option>";
								}
							}
						?>
					</select>
					<input type="submit">
					<button type="buttonCancel">Cancel</button>
				</form>
			</div>
		</div>
		<!--php function for checking extensions-->
		<?php
			function endsWith($string, $find){
    			$length = strlen($find);
    			if ($length == 0) {
        			return true;
    			}
    			return (substr($string, -$length) === $find);
			}
		?>
		<!--HTML File List Needs to be updated-->
		<div class = "box">
			<?php
				$output = shell_exec('./a4 -files');
				$fp = fopen("files.tmp","r");
				while (($line = fgets($fp)) !== false) {
					$fps[] = $line;
    			}
    			fclose($fp);
    			unlink("files.tmp");
				echo "HTML Files:";
				$files = scandir('./files/');
				$files = array_diff($files, array('.', '..'));
				foreach($files as $afile){
					if (endsWith($afile,".html")){
						$htmlfiles[] = $afile;
					}
				}
				if (empty($fps)) {
					$allfiles = $htmlfiles;
				} else {
					$allfiles = array_merge($htmlfiles,$fps);
				}
				sort($allfiles);

				echo "<ul>";
				$counter =0;
				foreach($allfiles as $file){
					if (in_array($file, $htmlfiles)){
						echo'<li><a href="./files/'.$file.'">'.$file.'</a>'.'<BR>';
					} else {
						echo'<li><a href="./dbfetch.php?theChosenOne='.$file.'">'.$file.'</a>'.'<BR>';
					}
				}
				echo "</ul>";
			?>
		</div>
		<!--Text file list good as is-->
		<div class = "box">
			<?php
				echo "Text Files:";
				$files = scandir('./files/');
				$files = array_diff($files, array('.', '..'));
				sort($files); // this does the sorting
				echo "<ul>";
				foreach($files as $file){
					if (endsWith($file,".html") == false and endsWith($file,".info") == false){
						echo'<li><a href="./files/'.$file.'">'.$file.'</a>'.'<BR>';
					}
				}
				echo "</ul>";
			?>
		</div>
 		<script> 
			var cmodal = document.getElementById('convertModal');
			var dbmodal = document.getElementById('dbModal');
			var cbtn = document.getElementById("conB1");
			var dbbtn = document.getElementById("dbB");
			var cspan = document.getElementsByClassName("close")[0];
			var dbspan = document.getElementsByClassName("close")[0];
			cbtn.onclick = function() {
    			cmodal.style.display = "block";
			}
			cspan.onclick = function() {
    			cmodal.style.display = "none";
			}
			window.onclick = function(event) {
		    if (event.target == cmodal) {
				    cmodal.style.display = "none";
    			}
			}
			dbbtn.onclick = function() {
    			dbmodal.style.display = "block";
			}
			dbspan.onclick = function() {
    			dbmodal.style.display = "none";
			}
			window.onclick = function(event) {
		    if (event.target == dbmodal) {
				    dbmodal.style.display = "none";
    			}
			}
		</script> 
	</body>
</html>
