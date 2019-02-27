function MatrixClient(){}
MatrixClient.debugLog = false;
MatrixClient.eventListeners = [];
MatrixClient.websocket = null;
MatrixClient.connectedFlag = false;
MatrixClient.connect = function(){
	var hostname = window.location.hostname;
	var url = "ws://"+hostname+":9090/ws";
	var ws = MatrixClient.websocket = new WebSocket(url);
	console.log("attempting to connect to " + url + " ...");
	ws.onopen = function(event){
		console.log("connection opened");
		MatrixClient.connectedFlag = true;
		for(var e in MatrixClient.eventListeners){
			if(MatrixClient.eventListeners[e].onWebsocketOpen !== undefined){
				MatrixClient.eventListeners[e].onWebsocketOpen();
			}
			if(MatrixClient.eventListeners[e].onWebsocketChange !== undefined){
				MatrixClient.eventListeners[e].onWebsocketChange();
			}
		}
	};
	ws.onmessage = function(event){
		var message = $.parseJSON(event.data);
		console.log("Message from server: " + message.type);
		console.log(message);
		for(var e in MatrixClient.eventListeners){
			if(MatrixClient.eventListeners[e].onWebsocketMessage !== undefined){
				MatrixClient.eventListeners[e].onWebsocketMessage(message);
			}
		}
	};
	ws.onclose = function(event){
		console.log("connection closed");
		// fire close event if websocket was open
		console.log(MatrixClient.websocket.readyState);
		if(MatrixClient.connectedFlag){
			MatrixClient.connectedFlag = false;
			MatrixClient.websocket = null;
			for(var e in MatrixClient.eventListeners){
				if(MatrixClient.eventListeners[e].onWebsocketClose !== undefined){
					MatrixClient.eventListeners[e].onWebsocketClose();
				}
				if(MatrixClient.eventListeners[e].onWebsocketChange !== undefined){
					MatrixClient.eventListeners[e].onWebsocketChange();
				}
			}
		}
		// retry connection
		setTimeout(function(){
			MatrixClient.connect();
		}, 1000);
	};
	ws.onerror = function(event){
		console.log("onerror: " + event.data);
	};
}
MatrixClient.addEventListener = function(listener){
	MatrixClient.eventListeners.push(listener);
}
MatrixClient.isConnected = function(){
	var ws = MatrixClient.websocket;
	return ws !== null && ws !== undefined && ws.readyState === WebSocket.OPEN;
}
MatrixClient.send = function(str){
	if(MatrixClient.debugLog) console.log("Sending: " + str);
	if(MatrixClient.isConnected()){
		MatrixClient.websocket.send(str);
	}
}
MatrixClient.sendMessage = function(msg, params){
	if(params === null || params === undefined){
		MatrixClient.send("msg"+msg);		
	} else {
		var str = "msg" + msg + ":";
		if(!(params instanceof Array)){
			params = [params];
		}
		for(var p in params){
			var pstr = params[p] + "";
			str += pstr.length + "/" + pstr;
		}
		MatrixClient.send(str);		
	}
}
MatrixClient.sendInput = function(state, code){
	MatrixClient.send("inp" + state + "" + code);

}
MatrixClient.sendJoystick = function(state, code, rel_x, rel_y){
	if(state != 2){
		MatrixClient.send("joy"+state+""+code);
	} else {
		var sign_x = rel_x >= 0 ? "+" : "";
		var sign_y = rel_y >= 0 ? "+" : "";
		MatrixClient.send("joy"+state+""+code+"/"+sign_x+rel_x+"/"+sign_y+rel_y);
	}
}
