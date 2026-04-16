<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST');

$recordingsDir = __DIR__ . '/../recordings/';

// Create recordings folder if not exists
if (!file_exists($recordingsDir)) {
    mkdir($recordingsDir, 0777, true);
}

$action = $_POST['action'] ?? '';

if ($action === 'start') {
    $duration = $_POST['duration'] ?? '0';
    
    // Simulate recording start
    file_put_contents($recordingsDir . 'recording_session.txt', json_encode([
        'started_at' => date('Y-m-d H:i:s'),
        'duration' => $duration,
        'status' => 'recording'
    ]));
    
    echo json_encode([
        'status' => 'ok',
        'message' => 'Recording started',
        'duration' => $duration
    ]);
    
} elseif ($action === 'stop') {
    // Create a dummy video file
    $filename = 'recording_' . date('Ymd_His') . '.mp4';
    $filepath = $recordingsDir . $filename;
    
    // Create empty file with metadata
    $dummyContent = json_encode([
        'type' => 'simulated_video',
        'created_at' => date('Y-m-d H:i:s'),
        'duration_seconds' => rand(5, 60)
    ]);
    file_put_contents($filepath, $dummyContent);
    
    // Delete session file
    if (file_exists($recordingsDir . 'recording_session.txt')) {
        unlink($recordingsDir . 'recording_session.txt');
    }
    
    echo json_encode([
        'status' => 'ok',
        'message' => 'Recording stopped and saved',
        'filename' => $filename,
        'path' => 'recordings/' . $filename
    ]);
    
} else {
    echo json_encode([
        'status' => 'error',
        'message' => 'Invalid action. Use: start or stop'
    ]);
}
?>