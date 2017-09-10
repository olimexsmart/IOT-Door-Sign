<?php
ini_set('display_errors', 'On');
error_reporting(E_ALL);

// Read ini with token information
$ini = parse_ini_file('tokens.ini');
//var_dump($ini); 
// Reading logs of two days before
$url = 'https://api.fitbit.com/1/user/-/activities/heart/date/' . date('Y-m-d', time() - 172800) . '/1d.json';


$data = file_get_contents($url, false, stream_context_create([
    'http' => [ 'method'          => 'GET'
              , 'follow_location' => true              
              , 'header'          => implode("\r\n", ['Accept: */*'                                                    
                                                    , 'Content-Type: application/x-www-form-urlencoded'
                                                    , "Authorization: Bearer " . $ini['accessToken']
                                                    , 'User-Agent: olli']) . "\r\n"
    ]]));


// Database connection
require_once 'login.php';
$sql = new mysqli($hostName, $userName, $passWord, $dataBase);
if ($sql->connect_error) {
    die($sql->connect_error);
}    

if($data != false) { // Checking if there was a valid response
    $values = json_decode($data, true);
    //var_dump($values);

    // Delete all data since we are getting all of it again
    $query = "TRUNCATE TABLE heartrate";
    $sql->query($query);

    for($i = 0; $i < count($values['activities-heart-intraday']['dataset']); $i++) {
        $time = $values['activities-heart-intraday']['dataset'][$i]['time'];
        $heart = $values['activities-heart-intraday']['dataset'][$i]['value'];
        $query = "INSERT INTO heartrate VALUES('$time', $heart)";
        $sql->query($query);
        //echo $query . '<br>';
    }

    echo "Done updating database";

} else {
    // Delete all data since the retreiving was unsuccesful
    $query = "TRUNCATE TABLE heartrate";
    $sql->query($query);  

    echo "Failed updating database";
}
