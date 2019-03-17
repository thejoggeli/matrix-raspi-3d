$(document).ready(function(){
	$(".inp-btn").each(function(){
		$(this).data("pressed", false);
	});
	$(".inp-btn").on("mousedown touchstart", function(){
		MatrixClient.sendInput(0, $(this).data("keycode"));
		$(this).data("pressed", true);
	});
	$(".inp-btn").on("mouseup mouseout touchend", function(){
		if($(this).data("pressed")){
			$(this).data("pressed", false)
			MatrixClient.sendInput(1, $(this).data("keycode"));
		}
	});
	$(".msg-btn").on("click", function(){
		MatrixClient.sendMessage($(this).data("msg"), $(this).data("params"));
	});
	$(".set-fullscreen").on("click", function(){
		var mode = $(this).data("mode");
		switch(mode){
			case "enter": Fullscreen.enter(); break;
			case "leave": Fullscreen.leave(); break;
			case "toggle": Fullscreen.toggle(); break;
		}
		Fullscreen.updateButtons();
	});
	$(".set-screen").on("click", function(){
		AppManager.setScreen($(this).data("screen"));
	});
	var sendBrightnessTimeout = null;
	var lastBrightness;
	$(".brightness-slider").on("input", function(){
		lastBrightness = $(this).val();
		if(sendBrightnessTimeout == null){
			sendBrightnessTimeout = setTimeout(function(){			
				sendBrightnessTimeout = null;
				MatrixClient.sendMessage("set_brightness", lastBrightness);
			}, 200);
		}
		HomeScreen.updateBrightness(lastBrightness);
	});
	$(".brightness-slider").on("change", function(){
		var value = $(this).val();
		if(sendBrightnessTimeout != null){
			clearTimeout(sendBrightnessTimeout);
			sendBrightnessTimeout = null;
		}
		MatrixClient.sendMessage("set_brightness", value);
	});
	$(document).on("fullscreenchange mozfullscreenchange webkitfullscreenchange msfullscreenchange", function(){
		Fullscreen.onChange();
	});
	Fullscreen.onChange();
	MatrixClient.connect();	
	MatrixClient.addEventListener(HomeScreen);
	MatrixClient.addEventListener(Sfx);
	MatrixClient.addEventListener(AppManager);
	HomeScreen.updateConnectedMode();
	// haf 
	Haf.setup({
		height:100,
	});	
	Haf.hide();
	var screen = HashUrl.getValue("screen", "home");
	AppManager.setScreen(screen);	
});

function AppManager(){}
AppManager.apps = {
	"Flappy-Bird": {
		"welcome": function(){
			AppManager.setScreen("gamepad");
		},
	},
	"Pong": {
		"welcome": function(){
			AppManager.setScreen("gamepad");			
		},
	},
	"Tetris": {
		"welcome": function(){
			AppManager.setScreen("gamepad");
		},
	},
	"Ledlib2d-Test": {
		"welcome": function(){
			AppManager.setScreen("gamepad");
		},
	},
	"Snake": {
		"welcome": function(){
			AppManager.setScreen("gamepad");
		},
	},
};
AppManager.setScreen = function(screen){
	Gamepad.close();
	HomeScreen.close();
	switch(screen){
		case "gamepad": Gamepad.open(); break;
		default: HomeScreen.open(); break;		
	}
	HashUrl.setValue("screen", screen);
}
AppManager.onWebsocketClose = function(){
	AppManager.setScreen("home");
}

function HomeScreen(){}
HomeScreen.isOpen = false;
HomeScreen.open = function(){
	if(HomeScreen.isOpen) return;
	HomeScreen.isOpen = true;
	$(".home-screen").show();
}
HomeScreen.close = function(){
	if(!HomeScreen.isOpen) return;
	HomeScreen.isOpen = false;
	$(".home-screen").hide();
}
HomeScreen.updateConnectedMode = function(){
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
	HomeScreen.updateWindowTitle ();	
}
HomeScreen.onWebsocketOpen = function(){}
HomeScreen.onWebsocketClose = function(){}
HomeScreen.onWebsocketChange = function(){
	HomeScreen.updateConnectedMode();	
}
HomeScreen.onWebsocketMessage = function(json){
	if(json.type == "welcome"){
		HomeScreen.updateBrightness(json.brightness);
		if(AppManager.apps[json.app] !== undefined && AppManager.apps[json.app].welcome !== undefined){
			AppManager.apps[json.app].welcome();
		} else {
			AppManager.setScreen("home");
		}
	} else if(json.type == "set_brightness"){
		HomeScreen.updateBrightness(json.brightness);		
	}
}
HomeScreen.updateWindowTitle = function(){
	var connected = MatrixClient.isConnected();
	var connectedStr = connected ? "connected" : "disconnected";
	document.title = "Raspi [" + connectedStr + "]";
}
HomeScreen.updateBrightness = function(val){
//	$(".msg-btn[data-msg=set_brightness]").removeClass("selected");
//	$(".msg-btn[data-msg=set_brightness][data-params="+val+"]").addClass("selected");
	$(".brightness-slider").val(val);
	$(".brightness-label .value").text(val);
}

function Fullscreen(){}
Fullscreen.updateButtons = function(){
	if(Fullscreen.isEnabled()){
		$(".set-fullscreen[data-mode=enter]").hide();
		$(".set-fullscreen[data-mode=leave]").show();	
	} else {		
		$(".set-fullscreen[data-mode=enter]").show();
		$(".set-fullscreen[data-mode=leave]").hide();	
	}
}
Fullscreen.isEnabled = function(){
	var doc = window.document;
	var docEl = doc.documentElement;
	if(doc.fullscreenElement || doc.mozFullScreenElement || doc.webkitFullscreenElement || doc.msFullscreenElement) {
		return true;
	}
	return false;
};
Fullscreen.enter = function(){
	var doc = window.document;
	var docEl = doc.documentElement;
	var enter = docEl.requestFullscreen || docEl.mozRequestFullScreen || docEl.webkitRequestFullScreen || docEl.msRequestFullscreen;
	enter.call(docEl);
}
Fullscreen.leave = function(){
	var doc = window.document;
	var docEl = doc.documentElement;
	var leave = doc.exitFullscreen || doc.mozCancelFullScreen || doc.webkitExitFullscreen || doc.msExitFullscreen;
	leave.call(doc);
}
Fullscreen.toggle = function(){
	if(Fullscreen.isEnabled()){
		Fullscreen.leave();
	} else {
		Fullscreen.enter();
	}
}
Fullscreen.onChange = function(){
	Fullscreen.updateButtons();
}

function HashUrl(){}
HashUrl.getValue = function(key, def){
	var json = HashUrl.urlToJson();
	if(json[key] === undefined) return def;
	return json[key];
}
HashUrl.setValue = function(key, val){
	var json = HashUrl.urlToJson();
	json[key] = val;
	HashUrl.jsonToUrl(json);	
}
HashUrl.urlToJson = function(){
	var queryString = window.location.hash.substr(1);
	if(queryString.indexOf('?') > -1){
		queryString = queryString.split('?')[1];
	}
	var pairs = queryString.split('&');
	var result = {};
	pairs.forEach(function(pair) {
		pair = pair.split('=');
		result[pair[0]] = decodeURIComponent(pair[1] || '');
	});
	delete result[""];
	return result;
}
HashUrl.jsonToUrl = function(json){
	var query = $.param(json);
	window.location.hash = query;
}
