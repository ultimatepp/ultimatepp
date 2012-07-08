function Log(x)
{
//	document.writeln(x + "<br>");
}

function ProcessAjaxResult(ss)
{
	for(i = 0; i < ss.length; i++) {
		var pos = ss[i].indexOf(':');
		if(pos >= 0) {
			var id = ss[i].slice(0, pos);
			var text = ss[i].slice(pos + 1);
			if(id == "!")
				eval(text);
			else
			if(id.length > 1 && id.charAt(0) == '>')
				document.getElementById(id.slice(1)).value = text;
			else
				document.getElementById(id).innerHTML = text;
		}
	}
}

function AjaxRequest()
{
	if(window.XMLHttpRequest)
		return new XMLHttpRequest();
	else
		return new ActiveXObject("Microsoft.XMLHTTP");
}

function IsNull(x)
{
	return x == null || x.length == 0;
}

function ScanForValues(x, result)
{
	if(x) {
		if(x.nodeType == 1 &&
		   (x.nodeName == "INPUT" || x.nodeName == "SELECT" || x.nodeName == "TEXTAREA")) {
			if(result.val.length)
				result.val += "&";
			Log(x.nodeName);
			Log("Name: " + x.name);
			Log("Id: " + x.id);
			Log(x.name == undefined);
			Log(x.name == null);
			var id = IsNull(x.name) ? x.id : x.name;
			Log("ID: " + id);
			if(!IsNull(id))
				result.val += id + '=' + encodeURIComponent(x.value);
		}
		for(var i = 0; i < x.childNodes.length; i++)
			ScanForValues(x.childNodes[i], result);
	}
}

function UxGet(request)
{
	var xmlhttp = AjaxRequest();
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200)
			ProcessAjaxResult(xmlhttp.responseText.split('\1'));
	}
	xmlhttp.open("GET", request, true);
	xmlhttp.send();
}

function UxPost(request)
{
	var xmlhttp = AjaxRequest();
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200)
			ProcessAjaxResult(xmlhttp.responseText.split('\1'));
	}
	var parameters = { val: "" };
	for(var i = 1; i < arguments.length; i++)
		ScanForValues(document.getElementById(arguments[i]), parameters);
	if(__js_identity__ != undefined) {
		if(parameters.val.length)
			parameters.val += "&";
		parameters.val += "__js_identity__=" + __js_identity__;
	}
	xmlhttp.open("POST", request, true);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send(parameters.val);
}
