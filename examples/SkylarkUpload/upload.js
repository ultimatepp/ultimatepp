// toggles progress bar visibility
function toggleBarVisibility() {
	var e = document.getElementById("progress_container");
	e.style.display = (e.style.display == "block") ? "none" : "block";
}

// create http request object for upload
// browser dependent -- probably NOT completely cross browser
// if you use JQuery there are better ways
function createRequestObject() {
	var http;
	if (navigator.appName == "Microsoft Internet Explorer") {
		http = new ActiveXObject("Microsoft.XMLHTTP");
	}
	else {
		http = new XMLHttpRequest();
	}
	return http;
}


function sendRequest() {
	var http = createRequestObject();
	
	// use session cookie and current time to build an unique upload identifier
	var session = document.getElementById('session').value;
	var time = document.getElementById('submittime').value;
	
	// get upload url
	var url = document.getElementById('progressurl').value;
	
	// build GET uri and do the request
	http.open("GET", url + "?uploadid=" + session + time);
	
	// setup a callback handling end of request
	http.onreadystatechange = function () { handleResponse(http); };
	http.send(null);
}

// this callback handles 'progress' http requests results
function handleResponse(http) {
	var response;
	if (http.readyState == 4) {
		response = http.responseText;
		document.getElementById("progress").style.width = response + "%";
		document.getElementById("progress").innerHTML = response + "%";
		
		if (response < 100) {
			// if upload still not complete, repost a progress request after some time
			// (here 200 msec)
			setTimeout("sendRequest()", 200);
		}
		else {
			// if progress is complete, put 'Done' inside bar, then
			// run a callback that hides it after some time
			document.getElementById("progress").innerHTML = "100% Done.";
			
			// hides 'Done' status 
			setTimeout("closeProgress()", 3000);
		}
	}
}

// this callback hides status bar after some time upload is over
function closeProgress() {
			toggleBarVisibility();
			document.getElementById("progress").innerHTML = "";
}

function startUpload(me) {
	// sets an unique 'uploadid' field composed by session cookie and current time
	var session = document.getElementById('session').value;
	var time = Date.now();
	document.getElementById('submittime').value = time;
	
	// build upload URI adding the 'uploadid' variable
	me.action = document.getElementById('uploadurl').value + '?uploadid=' + session + time;
	
	// setup progress bar
	document.getElementById("progress").style.width = 0;
	document.getElementById("progress").innerHTML = "0%";
	toggleBarVisibility();
	
	// starts the delayed progress handler
	setTimeout("sendRequest()", 200);
}
