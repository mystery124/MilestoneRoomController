
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
    xhttp.open("POST", "/config/restart", true);
    xhttp.send();
}

function reset(){
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
    };
    xhttp.open("POST", "/config/reset", true);
    xhttp.send();
}