function Painter(){}
Painter.pixels;
Painter.init = function(){
	Painter.width = 64;
	Painter.height = 32;
	Painter.aspect = Painter.width/Painter.height;
	Painter.pixels = new Array(Painter.width);
	for(var x = 0; x < Painter.width; x++){
		Painter.pixels[x] = new Array(Painter.height);
		for(var y = 0; y < Painter.height; y++){
			Painter.pixels[x][y] = {};
			Painter.setPixelColor(x, y, 0, 0, 0);
		}
	}	
}
Painter.setPixelColor = function(x, y, r, g, b){
	var p = Painter.pixels[x][y];
	if(p.r == r && p.g == g && p.b == b){
		return false;
	}
	p.r = r;
	p.g = g;
	p.b = b;
	p.str = "rgb(" + r + ", " + g + ", " + b + ")";
	return true;
}
Painter.open = function(){
	Haf.install({height:100,});	
	Haf.onResize = Painter.resize;
	Haf.onUpdate = Painter.update;
	Haf.onRender = Painter.render;
	Haf.getCanvas(0).setActive();
	Haf.getCanvas(0).clearColor = "#111";
	Haf.start();
	MatrixClient.addEventListener(Painter);
	MatrixClient.sendMessage("request_pixels");
}
Painter.close = function(){
	MatrixClient.removeEventListener(Painter);
	Haf.uninstall();
}
Painter.resize = function(){
	
}
Painter.update = function(){
	if(Input.isMouseDown()){
		var windowAspect = Haf.width / Haf.height;
		var x1 = Input.mouse.worldPosition.x;
		var y1 = Input.mouse.worldPosition.y;
		
		if(windowAspect > Painter.aspect){
			x1 *= Painter.height/Haf.height;
			y1 *= Painter.height/Haf.height;
		} else {
			x1 *= Painter.width/Haf.width;
			y1 *= Painter.width/Haf.width;
		}	
		x1 += Painter.width/2;
		y1 += Painter.height/2;
		
		if(x1 >= 0 && x1 < Painter.width && y1 >= 0 && y1 <= Painter.height){
			
			var r = 255;
			var g = 0;
			var b = 127;
			
			var x2 = x1;
			var y2 = y1;
			if(Painter.mouseWasDown){
				x2 = Painter.lastMouseX;
				y2 = Painter.lastMouseY;
			} 			
			var step = 0.25;
			var dx = x1-x2;
			var dy = y1-y2;
			var dist = Math.sqrt(dx*dx+dy*dy);
			if(dist == 0){
				var x3 = Math.floor(x1);
				var y3 = Math.floor(y1);
				if(Painter.setPixelColor(x3, y3, r, g, b)){
					MatrixClient.sendMessage("paint", [
						0xFF00FF, x3, y3
					]);				
				}
			} else {
				var dirx = (x2-x1)/dist;
				var diry = (y2-y1)/dist;
				for(var s = 0; s <= dist; s+=step){
					var x3 = Math.floor(x1 + dirx * s);
					var y3 = Math.floor(y1 + diry * s);
					if(Painter.setPixelColor(x3, y3, r, g, b)){
						MatrixClient.sendMessage("paint", [
							0xFF00FF, x3, y3
						]);				
					}
				}		
					
			}
			Painter.mouseWasDown = true;
			Painter.lastMouseX = x1;
			Painter.lastMouseY = y1;
		} else {
			Painter.mouseWasDown = false;	
		}
	} else {
		Painter.mouseWasDown = false;
	}
}

Painter.render = function(){
	var windowAspect = Haf.width / Haf.height;	
	ctx.save();
	if(windowAspect > Painter.aspect){
		ctx.scale(Haf.height/Painter.height, Haf.height/Painter.height);
	} else {
		ctx.scale(Haf.width/Painter.width, Haf.width/Painter.width);
	}	
	ctx.translate(-Painter.width/2, -Painter.height/2);
	
	for(var x = 0; x < Painter.width; x++){
		for(var y = 0; y < Painter.height; y++){
			var p = Painter.pixels[x][y];
			ctx.fillStyle = p.str;
			ctx.fillRect(x+0.1, y+0.1, 0.8, 0.8);			
		}
	}
	ctx.restore();
}

Painter.onWebsocketMessage = function(json){
	if(json.type == "pixels"){
		var x = 0;
		var y = 0;
		for(var i = 0; i < json.pixel_data.length; i+=6){
			var r = parseInt(json.pixel_data.charAt(i+0)+json.pixel_data.charAt(i+1), 16);
			var g = parseInt(json.pixel_data.charAt(i+2)+json.pixel_data.charAt(i+3), 16);
			var b = parseInt(json.pixel_data.charAt(i+4)+json.pixel_data.charAt(i+5), 16);
			Painter.setPixelColor(x, y, r, g, b);
			x++;
			if(x >= Painter.width){
				x = 0;
				y++;
			}
		}
	}
}

