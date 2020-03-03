$(document).ready(function(){
	$(document).on("mousedown touchstart", ".inp-btn", function(){
		MatrixClient.sendInput(0, $(this).data("keycode"));
		$(this).data("pressed", true);
	});
	$(document).on("mouseup mouseout touchend", ".inp-btn", function(){
		if($(this).data("pressed")){
			$(this).data("pressed", false)
			MatrixClient.sendInput(1, $(this).data("keycode"));
		}
	});
	$(document).on("click", ".msg-btn", function(){
		MatrixClient.sendMessage($(this).data("msg"), $(this).data("params"));
	});
	$(document).on("click", ".set-fullscreen", function(){
		var mode = $(this).data("mode");
		switch(mode){
			case "enter": Fullscreen.enter(); break;
			case "leave": Fullscreen.leave(); break;
			case "toggle": Fullscreen.toggle(); break;
		}
		Fullscreen.updateButtons();
	});
	$(document).on("click", ".set-screen", function(){
		ScreenManager.open($(this).data("screen"));
	});
	var sendBrightnessTimeout = null;
	var lastBrightness;
	$(document).on("input", ".brightness-slider", function(){
		lastBrightness = $(this).val();
		if(sendBrightnessTimeout == null){
			sendBrightnessTimeout = setTimeout(function(){			
				sendBrightnessTimeout = null;
				MatrixClient.sendMessage("set_brightness", lastBrightness);
			}, 200);
		}
		Home.updateBrightness(lastBrightness);
	});
	$(document).on("change", ".brightness-slider", function(){
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
	MatrixClient.addEventListener(Sfx);
	MatrixClient.addEventListener(AppManager);
	$(window).on("hashchange", function(){
		if(HashUrl.suppressNextChangeEvent){
			HashUrl.suppressNextChangeEvent = false;
		} else {
			var screen = HashUrl.getValue("screen", "home");
			if(screen != ScreenManager.activeScreen.name){	
				ScreenManager.open(screen);
			}
		}
	});
	ScreenManager.open("home");
});

function ScreenManager(){}
ScreenManager.activeScreen = null;
ScreenManager.loading = false;
ScreenManager.screens = {};
ScreenManager.queuedScreen = null;
ScreenManager.open = function(name){
	name = $.trim(name.toLowerCase());
	console.log("opening screen: " + name);
	// screen already open?
	if(ScreenManager.activeScreen != null && ScreenManager.activeScreen.name == name){
		return;
	}
	// block other screens from being loaded
	if(ScreenManager.loading){		
		ScreenManager.queuedScreen = name;
		console.log("screen queued: " + name);
		return;
	}
	ScreenManager.queuedScreen = null;
	ScreenManager.loading = true;
	// close current screen
	if(ScreenManager.activeScreen != null){
		if(ScreenManager.activeScreen.handle.isOpen)
			ScreenManager.activeScreen.handle.close();
			ScreenManager.activeScreen.container.hide();
		ScreenManager.activeScreen.handle.isOpen = false;
	}
	// check if already loaded
	HashUrl.setValue("screen", name, true);
	if(ScreenManager.screens[name] === undefined){
		var htmlFile = "screen/"+name+".html";
		var cssFile = "screen/"+name+".css";
		var jsFile = "screen/"+name+".js";	
		
		var html = null; 
		var css = null;
		var script = false; 
		
		console.log("loading files:", [htmlFile, cssFile, jsFile]);
		
		$.when(
			$.get(htmlFile).done(function(data){
				html = data;
			}),
			$.get(cssFile).done(function(data){
				css = data;
			}),
			$.get(jsFile).done(function(){	
				script = true;
			})
		).then(function(){
			if(script){
				// handle success		
				var $container = null;		
				if(html != null){
					$container = $("<div>");
					$container.addClass("screen-container-"+name);
					$container.html(html);
					$(".screens").append($container);
				}
				if(css != null){				
					$("<style />").html(css).appendTo("head");
				}
				var handleName = name[0].toUpperCase() + name.slice(1); 
				var screen = ScreenManager.activeScreen = ScreenManager.screens[name] = {
					handle: window[handleName],
					name: name,
					container: $container,
				}
				if(!screen.handle.isInitialized){
					screen.handle.init();
					screen.handle.isInitialized = true;
				}
				if(!screen.handle.isOpen){
					screen.handle.open();
					screen.handle.isOpen = true;
					// fire onWebsocketOpen if connected
					if(screen.handle.onWebsocketOpen !== undefined && MatrixClient.isConnected()){
						screen.handle.onWebsocketOpen();
					}
					screen.container.show();
				}
				ScreenManager.loading = false;
			} else {
				// handle failure
				if(name != "home"){
					console.error("couldn't open screen: " + name);
					ScreenManager.loading = false;		
					ScreenManager.activeScreen = null;
					ScreenManager.open("home");
				} else {
					alert("fatal error");
				}
			}
			if(ScreenManager.queuedScreen != null){
				ScreenManager.open(ScreenManager.queuedScreen);
			}
		});
	} else {
		var screen = ScreenManager.activeScreen = ScreenManager.screens[name];
		if(!screen.handle.isOpen){
			screen.handle.open();
			screen.handle.isOpen = true;
			// fire onWebsocketOpen if connected
			if(screen.handle.onWebsocketOpen !== undefined && MatrixClient.isConnected()){
				screen.handle.onWebsocketOpen();
			}
			screen.container.show();
		}
		ScreenManager.loading = false;
		if(ScreenManager.queuedScreen != null){
			ScreenManager.open(ScreenManager.queuedScreen);
		}
	}
}

function AppManager(){}
AppManager.brightness = 0;
AppManager.activeApp = null;


AppManager.apps = {
	"Welcome": {
		"name": "Welcome",
		"screen": "home",
	},
	"Painter": {
		"name": "Painter",
		"screen": "painter",
	},
	"Gallery": {
		"name": "Gallery",
		"screen": "gallery",
	},
	"Flappy-Bird": {
		"name": "Flappy Bird",
		"screen": "gamepad",
	},
	"Pong": {
		"name": "Pong",
		"screen": "gamepad",
	},
	"Tetris": {
		"name": "Tetris",
		"screen": "gamepad",
	},
	"Snake": {
		"name": "Snake",
		"screen": "gamepad",
	},
	"Asteroids": {
		"name": "Snake",
		"screen": "gamepad",
	},
	"Urho3D-Test": {
		"name": "3D-Test",
		"screen": "gamepad",
	},
	"Shader-Playground": {
		"name": "Mandelbrot",
		"screen": "gamepad",
	},
};
AppManager.startApp = function(app){
	if(AppManager.activeApp == app){
		if(AppManager.apps[app] !== undefined && AppManager.apps[app].screen !== undefined){
			ScreenManager.open(AppManager.apps[app].screen);
		}
	} else {
		MatrixClient.sendMessage("launch_app", app);
	}
}
AppManager.onWebsocketMessage = function(json){	
	if(json.type == "welcome"){
		AppManager.activeApp = json.app;
		AppManager.brightness = json.brightness;
		if(AppManager.apps[json.app] !== undefined && AppManager.apps[json.app].welcome !== undefined){
			AppManager.apps[json.app].welcome();
		}
		if(AppManager.apps[json.app] !== undefined && AppManager.apps[json.app].screen !== undefined){
			ScreenManager.open(AppManager.apps[json.app].screen);
		} else {
			ScreenManager.open("home");
		}
	} else if(json.type == "set_brightness"){
		AppManager.brightness = json.brightness;
	}
}
AppManager.onWebsocketClose = function(){
	AppManager.activeApp = null;
	ScreenManager.open("home");
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
HashUrl.suppressNextChangeEvent = false;
HashUrl.getValue = function(key, def){
	var json = HashUrl.urlToJson();
	if(json[key] === undefined) return def;
	return json[key];
}
HashUrl.setValue = function(key, val, suppress){
	if(HashUrl.getValue(key, null) == val){
		return;
	}
	if(suppress){
		HashUrl.suppressNextChangeEvent = true;
	}
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
