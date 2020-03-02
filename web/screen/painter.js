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
Painter.open = function(){
	Haf.install({height:100,});	
	Haf.onResize = Painter.resize;
	Haf.onUpdate = Painter.update;
	Haf.onRender = Painter.render;
	Haf.getCanvas(0).setActive();
	Haf.getCanvas(0).clearColor = "#111";
	Haf.start();
	MatrixClient.addEventListener(Painter);
	Painter.touches = [];		
	Colors.hslToRgb255(randomFloat(0, 1.0), 1.0, 0.5);
	Painter.color = {
		r: Colors.r255,
		g: Colors.g255,
		b: Colors.b255,
	};
}
Painter.close = function(){
	MatrixClient.removeEventListener(Painter);
	Haf.uninstall();
}
Painter.resize = function(){
	
}
Painter.update = function(){		
	for(var i in Input.newTouches){		
		var touch = Input.newTouches[i];
		if(touch.taken) continue;
		touch.taken = true;
		Painter.touches.push(new PainterTouch(touch));
	}		
	// remove expired touches
	for(var i = Painter.touches.length-1; i >= 0; i--){
		if (Painter.touches[i].expired) {
			Painter.touches.splice(i, 1);
		}
	}
	// update touches
	for(var t in Painter.touches){
		Painter.touches[t].update();
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
Painter.onWebsocketOpen = function(){
	MatrixClient.sendMessage("request_pixels");
}
Painter.onWebsocketMessage = function(json){
	if(json.type == "pixel"){
		var x = parseInt(json.x);
		var y = parseInt(json.y);
		var r = parseInt(json.rgb.charAt(0)+json.rgb.charAt(1), 16);
		var g = parseInt(json.rgb.charAt(2)+json.rgb.charAt(3), 16);
		var b = parseInt(json.rgb.charAt(4)+json.rgb.charAt(5), 16);
		Painter.setPixelColor(x, y, r, g, b);		
	}
	if(json.type == "pixels"){
		var x = 0;
		var y = 0;
		var len = json.rgb.length-1; // h suffix
		for(var i = 0; i < len; i+=6){
			var r = parseInt(json.rgb.charAt(i+0)+json.rgb.charAt(i+1), 16);
			var g = parseInt(json.rgb.charAt(i+2)+json.rgb.charAt(i+3), 16);
			var b = parseInt(json.rgb.charAt(i+4)+json.rgb.charAt(i+5), 16);
			Painter.setPixelColor(x, y, r, g, b);
			x++;
			if(x >= Painter.width){
				x = 0;
				y++;
			}
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

function PainterTouch(touch){
	this.touch = touch;
	this.p1 = new Vector();
	this.p2 = new Vector();
	this.p3 = new Vector();
	this.last = new Vector(this.touch.worldPosition.x, this.touch.worldPosition.y);
	this.transform(this.last);
}
PainterTouch.prototype.transform = function(p){
	var windowAspect = Haf.width / Haf.height;
	if(windowAspect > Painter.aspect){
		p.x *= Painter.height/Haf.height;
		p.y *= Painter.height/Haf.height;
	} else {
		p.x *= Painter.width/Haf.width;
		p.y *= Painter.width/Haf.width;
	}	
	p.x += Painter.width/2;
	p.y += Painter.height/2;	
}
PainterTouch.prototype.update = function(){
	if(this.expired){
		console.log("touch expired");
		return;
	}
	
	var p1 = this.p1;
	var p2 = this.p2;
	var p3 = this.p3;
	
	p1.x = this.touch.worldPosition.x;
	p1.y = this.touch.worldPosition.y;
	
	this.transform(p1);
	
	if(p1.x >= 0 && p1.x < Painter.width && p1.y >= 0 && p1.y <= Painter.height){
		
		var rgb = (Painter.color.r<<16)|(Painter.color.g<<8)|Painter.color.b;
		
		p2.x = this.last.x;
		p2.y = this.last.y;
		
		var step = 0.25;
		var dx = p1.x-p2.x;
		var dy = p1.y-p2.y;
		var dist = Math.sqrt(dx*dx+dy*dy);
		if(dist == 0){
			p3.x = Math.floor(p1.x);
			p3.y = Math.floor(p1.y);
			if(Painter.setPixelColor(p3.x, p3.y, Painter.color.r, Painter.color.g, Painter.color.b)){
				MatrixClient.sendMessage("paint", [
					rgb, p3.x, p3.y
				]);				
			}
		} else {
			var dirx = (p2.x-p1.x)/dist;
			var diry = (p2.y-p1.y)/dist;
			for(var s = 0; s <= dist; s+=step){
				p3.x = Math.floor(p1.x + dirx * s);
				p3.y = Math.floor(p1.y + diry * s);
				if(Painter.setPixelColor(p3.x, p3.y, Painter.color.r, Painter.color.g, Painter.color.b)){
					MatrixClient.sendMessage("paint", [
						rgb, p3.x, p3.y
					]);				
				}
			}				
		}
		this.last.x = p1.x;
		this.last.y = p1.y;
	}
	
}
