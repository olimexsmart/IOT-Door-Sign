<?php
ini_set('display_errors', 'On');
error_reporting(E_ALL);

// Read ini with application information
$ini = parse_ini_file('/home/fitbitFiles/fitbitData.ini');
$tokens = parse_ini_file('/home/fitbitFiles/tokens.ini');

// Access Token Request
$content = http_build_query(array('grant_type' => 'refresh_token', 'refresh_token' => $tokens['refreshToken']));
//echo $content;

$result = file_get_contents($ini['refreshURI'], false, stream_context_create([
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

    $tokenIni = 'accessToken = "' . $accessData['access_token'] . '"' . "\n" . 'refreshToken = "' . $accessData['refresh_token'] . '"';
    file_put_contents('/home/fitbitFiles/tokens.ini', $tokenIni);

    echo date('H:i:s d-m-Y') . "\tTokens refreshed\n";
} else {
    echo date('H:i:s d-m-Y') . "\tTokens not refreshed\n";
}


