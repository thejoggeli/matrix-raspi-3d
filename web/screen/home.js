function Home(){}
Home.init = function(){
}
Home.open = function(){
	MatrixClient.addEventListener(Home);
	Home.updateBrightness(AppManager.brightness);
	Home.updateConnectedMode();
}
Home.close = function(){
	MatrixClient.removeEventListener(Home);
}
Home.updateConnectedMode = function(){
	var mode = MatrixClient.isConnected();
	if(mode){
		$("button").prop("disabled", false);
		$(".connected-state").text("connected");
		$(".connected-state").addClass("connected");
		$(".connected-state").removeClass("disconnected");
	} else {
		$("button").prop("disabled", true);
		$(".connected-state").text("disconnected");
		$(".connected-state").addClass("disconnected");
		$(".connected-state").removeClass("connected");
	}
	Home.updateWindowTitle ();	
}
Home.onWebsocketOpen = function(){}
Home.onWebsocketClose = function(){}
Home.onWebsocketChange = function(){
	Home.updateConnectedMode();	
}
Home.onWebsocketMessage = function(json){
	console.log("onmsg");
	if(json.type == "welcome"){
		Home.updateBrightness(json.brightness);
	} else if(json.type == "set_brightness"){
		Home.updateBrightness(json.brightness);
	}
}
Home.updateWindowTitle = function(){
	var connected = MatrixClient.isConnected();
	var connectedStr = connected ? "connected" : "disconnected";
	document.title = "Raspi [" + connectedStr + "]";
}
Home.updateBrightness = function(val){
//	$(".msg-btn[data-msg=set_brightness]").removeClass("selected");
//	$(".msg-btn[data-msg=set_brightness][data-params="+val+"]").addClass("selected");
	$(".brightness-slider").val(val);
	$(".brightness-label .value").text(val);
}
