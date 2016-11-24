<?php
ini_set('display_errors', 1);
error_reporting(E_ALL ^ E_NOTICE);
echo "balls";
$dbh = new PDO( 'mysql:host=dursley.socs.uoguelph.ca;', 'fhasson', '0878596' );
$dbh->query("use fhasson");
foreach($db->query('SELECT * FROM htmls') as $row) {
    echo $row['name'].'<BR>'; //etc...
}
echo "test";


//echo 'Current PHP version: ' . phpversion();
// if (!function_exists('mysqli_init') && !extension_loaded('mysqli')) {
//     echo 'no mysqli :(';
// } else {
//     echo 'we gots it';
// }
// ini_set('display_errors', 1);
// error_reporting(E_ALL ^ E_NOTICE);
// $servername = "dursley.socs.uoguelph.ca";
// $username = "fhasson";
// $password = "0878596";
// $dbname = "fhasson";

// // Create connection
// $conn = new mysqli($servername, $username, $password, $dbname);
// // Check connection
// if ($conn->connect_error) {
//     die("Connection failed: " . $conn->connect_error);
// } 

// $sql = "SELECT name FROM htmls";
// $result = $conn->query($sql);

// if ($result->num_rows > 0) {
//     // output data of each row
//     while($row = $result->fetch_assoc()) {
//         echo "name: " . $row["name"]."<br>";
//     }
// } else {
//     echo "0 results";
// }
//$conn->close();
?>