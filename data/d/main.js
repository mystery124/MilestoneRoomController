function loadSSIDs() {
    document.getElementById("wifilist").innerHTML = "<tr><td  class=\"col\" colspan=\"4\">Searching...</td></tr>";
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        let toast;
        if (this.readyState == 4 && this.status == 200) {
            updateSSIDlist(this.responseText);
        } else if(this.readyState == 4 && (this.status >= 400 || this.status > 200)){
            toast = document.getElementsByClassName("toast fail")[0];
        }
        if(toast){
            toast.classList.add('show');
            setTimeout(function(){ toast.classList.remove("show"); }, 3000);
        }
    };
    xhttp.open("GET", "/getSSIDs", true);
    xhttp.send();
};

function updateSSIDlist(response){
    let result = JSON.parse(response);
    let tableSSIDhtml = "";
    if(result.length == 0 ) {
        tableSSIDhtml = "<tr><td colspan=\"4\">No items</td></tr>";
    } else {
        result.ssids.sort(function (ssida, ssidsb) {
            if (ssida.signalStrength > ssidsb.signalStrength) {
                return -1;
            }
            if (ssidsb.signalStrength > ssida.signalStrength) {
                return 1;
            }
            return 0;
        });
        for(ssid of result.ssids){
            let trOpen =      "<tr onclick=\"selectNetwork('"+ssid.name+"')\">";
            let ssidName =    "<td scope=\"row\" data-label=\"SSID\" class=\"col\">"+ssid.name+"</td>";
            let isPublic =    "<td scope=\"row\" data-label=\"Is Public\" class=\"col\"><img height=\"42\" width=\"42\" src=\"lock"+ssid.isPublic+".png\"></td>";
            let signalStrgh = "<td scope=\"row\" data-label=\"Signal\" class=\"col\"><img height=\"42\" width=\"42\" src=\"wifi"+ssid.signalStrength+".png\"></td>"
            let channel =     "<td scope=\"row\" data-label=\"Channel\" class=\"col\">"+ssid.channel+"</td>";
            tableSSIDhtml += trOpen + ssidName + isPublic + signalStrgh + channel + "</tr>";
        }
    }
    document.getElementById("wifilist").innerHTML = tableSSIDhtml;
};
function selectNetwork(ssid){
    document.getElementById("ssid").value = ssid;
};

function submit(){
    let submitButton = document.getElementById("submit");
    let ssid = document.getElementById("ssid").value;
    let password = document.getElementById("pass").value;
    let nodeName = document.getElementById("name").value;

    if(!ssid || !password || !nodeName){
        let toast = document.getElementsByClassName("toast warning")[0];
        toast.classList.add('show');
        setTimeout(function(){ toast.classList.remove("show"); }, 3000);
        return;
    }

    submitButton.value = 'Saving....';
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        let toast;
        if(this.readyState == 4){
            submitButton.value = 'Submit';
        }
        if(this.readyState == 4 && this.status == 200) {
            toast = document.getElementsByClassName("toast success")[0];
        } else if(this.readyState == 4 && (this.status >= 400 || this.status < 200)){
            toast = document.getElementsByClassName("toast fail")[0];
        }
        if(toast){
            toast.classList.add('show');
            setTimeout(function(){ toast.classList.remove("show"); }, 3000);
        }
    };
    let config = {};
    config["SSID"] = encodeURI(ssid);
    config["password"] = encodeURI(password);
    config["nodeName"] = encodeURI(nodeName);
    xhttp.open("POST", "/saveConfig", true);
    xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhttp.send(JSON.stringify(config));
}

function restart(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        let toast;
        if(this.readyState == 4 && this.status == 200) {
            toast = document.getElementsByClassName("toast success")[0];
        } else if(this.readyState == 4 && (this.status >= 400 || this.status < 200)){
            toast = document.getElementsByClassName("toast fail")[0];
        }
        if(toast){
            toast.classList.add('show');
            setTimeout(function(){ toast.classList.remove("show"); }, 1000);
        }
        setTimeout(function(){ document.getElementById("formBody").innerHTML = "<h1>Restarting...</h1>" }, 1000);
    };
    xhttp.open("POST", "/restart", true);
    xhttp.send();
}