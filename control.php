<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST');

$direction = $_GET['dir'] ?? '';

$validDirections = ['up', 'down', 'left', 'right', 'stop'];

if (in_array($direction, $validDirections)) {
    // Simulate PTZ movement
    echo json_encode([
        'status' => 'ok',
        'message' => "PTZ command executed: $direction",
        'simulated' => true,
        'timestamp' => date('Y-m-d H:i:s')
    ]);
} else {
    echo json_encode([
        'status' => 'error',
        'message' => 'Invalid direction. Use: up, down, left, right, stop'
    ]);
}
?>