function Home(){}
Home.init = function(){
	$(document).on("click", ".home .app-btn", function(){
		AppManager.startApp($(this).data("app"));
	});
	var $apps = $(".home .apps");
	for(var name in AppManager.apps){
		var app = AppManager.apps[name];
		var $row = $("<div class='app-row'>");
		var $btn = $("<button class='app-btn' data-app='"+name+"'>");
		$btn.html(name);
		$row.append($btn);
		$apps.append($row);
	}
}
Home.open = function(){
	MatrixClient.addEventListener(Home);
	Home.updateBrightness(AppManager.brightness);
	Home.updateConnectedMode();
	Home.updateActiveApp();
}
Home.close = function(){
	MatrixClient.removeEventListener(Home);
}
Home.updateActiveApp = function(){
	$(".home .app-row").removeClass("active");
	$(".home .app-btn").removeClass("active");
	$(".home .app-row").removeClass("semi-active");
	$(".home .app-btn").removeClass("semi-active");
	var activeApp = AppManager.activeApp;
	if(activeApp == null) return;
	var $btn = $(".home .app-btn[data-app='"+activeApp+"']");
	$btn.addClass("active");
	$btn.closest(".home .app-row").addClass("active");
	console.log($btn);
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
Home.onWebsocketClose = function(){
	$(".app-row").removeClass("active");
	$(".app-btn").removeClass("active");
	$(".app-row").removeClass("semi-active");
	$(".app-btn").removeClass("semi-active");	
}
Home.onWebsocketChange = function(){
	Home.updateConnectedMode();	
}
Home.onWebsocketMessage = function(json){
	console.log("onmsg");
	if(json.type == "welcome"){
		Home.updateBrightness(json.brightness);
		Home.updateActiveApp();
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
