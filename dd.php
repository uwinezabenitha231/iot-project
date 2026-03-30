<!DOCTYPE html>
<html>
<head>
    <title>IoT Dashboard</title>
    <style>
        body { font-family: Arial; background: #f4f4f4; margin:0; padding:0; text-align:center; }
        .container { width: 95%; margin:auto; }
        h1 { margin-top:20px; }

        .cards { display:flex; flex-wrap:wrap; justify-content:center; margin-bottom:20px; }
        .card { background:white; padding:20px; margin:10px; width:200px; border-radius:10px; box-shadow:0 0 10px gray; }
        .temp { font-size:2em; font-weight:bold; }
        .hum { font-size:2em; font-weight:bold; }

        .hot { color:red; }
        .normal { color:green; }

        table { width:100%; border-collapse:collapse; margin-top:20px; background:white; box-shadow:0 0 10px gray; }
        th, td { padding:10px; border:1px solid #ddd; }

        @media(max-width:600px){ 
            .cards{flex-direction:column; align-items:center;} 
            .card{width:80%;} 
        }
    </style>
</head>
<body>

<div class="container">
    <h1>IoT Dashboard</h1>

    <div class="cards">
        <div class="card">
            <h2>Temperature (°C)</h2>
            <p id="temp" class="temp">--</p>
        </div>
        <div class="card">
            <h2>Humidity (%)</h2>
            <p id="hum" class="hum">--</p>
        </div>
    </div>

    <table>
        <thead>
            <tr><th>Time</th><th>Temperature (°C)</th><th>Humidity (%)</th></tr>
        </thead>
        <tbody id="table-body"></tbody>
    </table>
</div>

<script>
let history = [];

function generateSensorData() {
    // Simulated sensor data
    let temp = (Math.random() * 0 + 32).toFixed(2); // 0-33°C
    let hum = (Math.random() * 0+ 34 ).toFixed(2);   // 40-80%
    let time = new Date().toLocaleTimeString();
    return {time, temp, hum};
}

function updateDashboard() {
    let data = generateSensorData();
    history.push(data);
    if(history.length > 20) history.shift(); // keep last 20

    // Update cards with color-coded temperature
    let tempEl = document.getElementById("temp");
    tempEl.innerText = data.temp;
    tempEl.className = data.temp > 30 ? "temp hot" : "temp normal";

    document.getElementById("hum").innerText = data.hum;

    // Update historical table
    let tableHTML = "";
    history.forEach(item=>{
        tableHTML += `<tr><td>${item.time}</td><td>${item.temp}</td><td>${item.hum}</td></tr>`;
    });
    document.getElementById("table-body").innerHTML = tableHTML;
}

// Auto-refresh every 3 seconds
setInterval(updateDashboard, 3000);
updateDashboard();
</script>

</body>
</html>