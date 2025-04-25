let theme = "dark";
let infoVisible = false;

function checkLogin() {
    const input = document.getElementById("pw").value;
    if (!input) return alert("Password required!");
    
    fetch("/authLogin?password=" + input)
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                document.getElementById("loginBox").style.display = "none";
                document.getElementById("controlBox").style.display = "block";
                updateLEDState();
                setInterval(updateSensor, 1000);
                setInterval(updateSysInfo, 1000);
            } else {
                alert("Wrong password!");
            }
        })
        .catch(error => {
            console.error('Error during login:', error);
            alert("Something went wrong!");
        });
}

function toggleLED(state) {
    fetch("/toggleLED?action=" + state).then(() => updateLEDState());
}

function updateLEDState() {
    fetch("/state")
        .then(res => res.json())
        .then(data => {
            document.getElementById("ledState").textContent = data.led.toUpperCase();
        });
}

function updateSensor() {
    fetch("/sensor")
        .then(res => res.text())
        .then(value => {
            document.getElementById("sensor").textContent = value;
        });
}

function updateLCD() {
    const txt = document.getElementById("lcdText").value;
    if (!txt) return alert("Enter text first!");
    fetch("/updateLCD?text=" + encodeURIComponent(txt))
        .then(res => res.text())
        .then(msg => {
            console.log("LCD:", msg);
        });
}

function showSignal() {
    fetch("/wifiSignal")
        .then(res => res.text())
        .then(msg => {
            console.log("Signal:", msg);
        });
}

function updateSysInfo() {
    fetch("/sysinfo")
        .then(res => res.json())
        .then(data => {
            document.getElementById('chipID').textContent = data.esp.chip_id;
            document.getElementById('coreVer').textContent = data.esp.core_version;
            document.getElementById('sdkVer').textContent = data.esp.sdk_version;
            document.getElementById('flashReal').textContent = (data.esp.flash_real_size / 1024).toFixed(1);
            document.getElementById('heap').textContent = data.esp.free_heap;
            document.getElementById('maxFreeBlock').textContent = data.esp.max_free_block;
            document.getElementById('uptime').textContent = (data.esp.uptime_ms / 1000).toFixed(1);
            document.getElementById('ssid').textContent = data.wifi.ssid;
            document.getElementById('rssi').textContent = data.wifi.rssi;
            document.getElementById('ip').textContent = data.wifi.ip;
            document.getElementById('mac').textContent = data.wifi.mac;
            document.getElementById('bssid').textContent = data.wifi.bssid;
            document.getElementById('channel').textContent = data.wifi.channel; 
            document.getElementById('hostname').textContent = data.wifi.hostname;
            document.getElementById('size').textContent = `${data.display.width}x${data.display.height}`;
            document.getElementById('rotation').textContent = data.display.rotation;
            document.getElementById('addr').textContent = data.display.i2c_addr;
        })
        .catch(err => {
            console.error("Failed to fetch sysinfo", err);
        });
}

function displayIdleState() {
    fetch("/idleState")
        .then(msg => {
            console.log("Idle:", msg);
        });
}