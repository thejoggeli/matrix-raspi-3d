function Home(){}
Home.adminUnlocked = false;
Home.init = function(){
	$(document).on("click", "#home .app-btn", function(){
		$(this).addClass("semi-active");
		AppManager.startApp($(this).data("app"));
	});
	var $apps = $("#home .apps");
	for(var key in AppManager.apps){
		var handle = AppManager.apps[key];
		var name = handle.name;
		var $row = $("<div class='app-row'>");
		var $btn = $("<button class='app-btn' data-app='"+key+"'>");
		$btn.html(name);
		$row.append($btn);
		$apps.append($row);
	}
	$("#home .admin-row").hide()
	$("#home .admin-unlock-row").on("click", Home.adminPrompt)
}
Home.adminPrompt = function(){
	var val = ""+prompt("PASSWORD");
	if(val == "420"){
		Home.adminUnlocked = true;
		$("#home .admin-unlock-row").hide();
		$("#home .admin-row").show();
	} else {
		alert("NEIN!!!");
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
	$("#home .app-row").removeClass("active");
	$("#home .app-btn").removeClass("active");
	$("#home .app-row").removeClass("semi-active");
	$("#home .app-btn").removeClass("semi-active");
	var activeApp = AppManager.activeApp;
	if(activeApp == null) return;
	var $btn = $("#home .app-btn[data-app='"+activeApp+"']");
	$btn.addClass("active");
	$btn.closest("#home .app-row").addClass("active");
	console.log($btn);
}
Home.updateConnectedMode = function(){
	var mode = MatrixClient.isConnected();
	if(mode){
		$("#home button").prop("disabled", false);
		$("#home .connected-state").text("connected");
		$("#home .connected-state").addClass("connected");
		$("#home .connected-state").removeClass("disconnected");
	} else {
		$("#home button").prop("disabled", true);
		$("#home .connected-state").text("disconnected");
		$("#home .connected-state").addClass("disconnected");
		$("#home .connected-state").removeClass("connected");
	}
	Home.updateWindowTitle ();	
}
Home.onWebsocketOpen = function(){}
Home.onWebsocketClose = function(){
	$("#home .app-row").removeClass("active");
	$("#home .app-btn").removeClass("active");
	$("#home .app-row").removeClass("semi-active");
	$("#home .app-btn").removeClass("semi-active");	
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
//	$("#home .msg-btn[data-msg=set_brightness]").removeClass("selected");
//	$("#home .msg-btn[data-msg=set_brightness][data-params="+val+"]").addClass("selected");
	$("#home .brightness-slider").val(val);
	$("#home .brightness-label .value").text(val);
}
