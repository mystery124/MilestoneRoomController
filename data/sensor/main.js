
function refresh(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        let toast;
        if(this.readyState == 4 && this.status == 200) {
            updateReads(this.responseText);
        } else if(this.readyState == 4 && (this.status >= 400 || this.status < 200)){
            toast = document.getElementsByClassName("toast fail")[0];
        }
        if(toast){
            toast.classList.add('show');
            setTimeout(function(){ toast.classList.remove("show"); }, 1000);
        }
    };
    xhttp.open("GET", "/sensor/readSensor", true);
    xhttp.send();
}

function updateReads(response){
    let result = JSON.parse(response);
    if(result) {
        document.getElementById("temp").innerHTML = result.temp + " C";
        document.getElementById("press").innerHTML = result.press + " hPa";
        document.getElementById("humid").innerHTML = result.humid + " %";
    }
}

document.addEventListener("DOMContentLoaded", function(){
    setInterval(refresh, 5000);
    refresh();
});
