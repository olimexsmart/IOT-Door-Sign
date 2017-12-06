<?php
//echo "Bomber";

// Database connection
require_once 'login.php';
$sql = new mysqli($hostName, $userName, $passWord, $dataBase);
if ($sql->connect_error) {
    die($sql->connect_error);
}

// Return the the HR relative to the current minute
$currentMinute = date('H:i:00');
$query = "SELECT value FROM fitbit.heartrate WHERE time = '$currentMinute'";

// Query it
if (!$result = $sql->query($query)) {
    die("Error retreiving HR.");
}
$HR = $result->fetch_row()[0];

// Return result
echo $HR;


