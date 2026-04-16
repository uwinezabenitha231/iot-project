<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, DELETE');

$recordingsDir = __DIR__ . '/../recordings/';

if (!file_exists($recordingsDir)) {
    mkdir($recordingsDir, 0777, true);
}

$action = $_GET['action'] ?? '';

if ($action === 'list') {
    $files = scandir($recordingsDir);
    $fileList = [];
    
    foreach ($files as $file) {
        if ($file !== '.' && $file !== '..' && $file !== 'recording_session.txt') {
            $filePath = $recordingsDir . $file;
            $fileList[] = [
                'name' => $file,
                'size' => formatBytes(filesize($filePath)),
                'date' => date('Y-m-d H:i:s', filemtime($filePath)),
                'url' => 'recordings/' . $file
            ];
        }
    }
    
    // Sort by date descending (newest first)
    usort($fileList, function($a, $b) {
        return strtotime($b['date']) - strtotime($a['date']);
    });
    
    echo json_encode([
        'status' => 'ok',
        'files' => $fileList,
        'count' => count($fileList)
    ]);
    
} elseif ($action === 'delete') {
    $filename = basename($_GET['file'] ?? '');
    $filepath = $recordingsDir . $filename;
    
    if (file_exists($filepath) && is_file($filepath)) {
        unlink($filepath);
        echo json_encode([
            'status' => 'ok',
            'message' => "Deleted: $filename"
        ]);
    } else {
        echo json_encode([
            'status' => 'error',
            'message' => 'File not found'
        ]);
    }
    
} else {
    echo json_encode([
        'status' => 'error',
        'message' => 'Invalid action. Use: list or delete'
    ]);
}

function formatBytes($bytes, $precision = 2) {
    $units = ['B', 'KB', 'MB', 'GB'];
    $i = 0;
    while ($bytes >= 1024 && $i < count($units) - 1) {
        $bytes /= 1024;
        $i++;
    }
    return round($bytes, $precision) . ' ' . $units[$i];
}
?>