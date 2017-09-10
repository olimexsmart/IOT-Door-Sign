<?php

//read ini file here

$url = 'https://api.fitbit.com/1/user/-/activities/heart/date/today/1d.json';


$data = file_get_contents($url, false, stream_context_create([
    'http' => [ 'method'          => 'GET'
              , 'follow_location' => true              
              , 'header'          => implode("\r\n", ['Accept: */*'                                                    
                                                    , 'Content-Type: application/x-www-form-urlencoded'
                                                    , "Authorization: Bearer $accessToken"
                                                    , 'User-Agent: olli']) . "\r\n"
    ]]));





echo $data;

// update database here