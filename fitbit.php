<?php
ini_set('display_errors', 'On');
error_reporting(E_ALL);

// Read ini with application information
$ini = parse_ini_file('/home/fitbitFiles/fitbitData.ini');
//var_dump($ini); 

// Get the code from the request
$code = $_GET['code'];

// Access Token Request
$content = http_build_query(array('clientId' => $ini['clientId'], 'redirect_uri' => $ini['callbackURL'], 'code' => $code, 'grant_type' => 'authorization_code'));
//echo $content;

$result = file_get_contents($ini['authorizationURI'], false, stream_context_create([
    'http' => [ 'method'          => 'POST'
              , 'follow_location' => true
              , 'content'         => $content
              , 'header'          => implode("\r\n", ['Accept: */*'
                                                    , 'Content-Type: application/x-www-form-urlencoded'
                                                    , 'Authorization: Basic ' . base64_encode($ini['clientId'] . ':' . $ini['clientSecret'])
                                                    , 'User-Agent: olli']) . "\r\n"
    ]]));

//echo $result;
if($result != false) {
    $accessData = json_decode($result, true);
    //var_dump($accessData);
    // Save tokens into a file for other script use
    $tokenIni = 'accessToken = "' . $accessData['access_token'] . '"' . "\n" . 'refreshToken = "' . $accessData['refresh_token'] . '"';
    file_put_contents('/home/fitbitFiles/tokens.ini', $tokenIni);

    echo date('H:i:s d-m-Y') . "\tSuccess getting tokens\n";
} else {
    echo date('H:i:s d-m-Y') . "\tFailed getting tokens\n";
}


