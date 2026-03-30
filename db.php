<?php
$servername = "localhost";   
$username   = "root";        
$password   = "";            
$dbname     = "sensor_data"; 
$conn =mysqli_connect($servername, $username, $password,$dbname);
if ($conn) 
    {
      echo" connection successfully";
}
else
    {
        echo" connection fail";
    }
?>
