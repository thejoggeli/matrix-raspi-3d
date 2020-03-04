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
	PainterPenTouch.init();
	PainterTools.init();
	$("#painter .tool-button").on("click", function(){
		if($(this).data("tool") != "home"){
			PainterTools.selectTool($(this).data("tool"));
		}
	});
	$("#painter .tool-button[data-tool='home']").on("click", function(){
		ScreenManager.open("home");
	});
			
//	Painter.$rose = $("#painter .rose");
//	Painter.onRoseDragInit();


}
/*
Painter.onRoseDragInit = function(){
	$(document).on("mousedown touchstart", "#painter .rose", Painter.onRoseDragStart);
}
Painter.onRoseDragStart = function(e){
	console.log("DRAG START");
	$(document).off("mousedown touchstart", "#painter .rose", Painter.onRoseDragStart);
	$(document).on("mousemove touchmove", Painter.onRoseDragMove);
	$(document).on("mouseup touchend", Painter.onRoseDragEnd);
	if(e.originalEvent.touches){
		var touch = e.originalEvent.touches[0];
		var rect = e.currentTarget.getBoundingClientRect();
		Painter.dragOffsetX = touch.clientX - rect.left;
		Painter.dragOffsetY = touch.clientY - rect.top;
	} else {
		var rect = e.currentTarget.getBoundingClientRect();
		Painter.dragOffsetX = e.clientX - rect.left;
		Painter.dragOffsetY = e.clientY - rect.top;		
	}
	e.preventDefault();
}
Painter.onRoseDragMove = function(e){
	if(e.originalEvent.touches){
		var touch = e.originalEvent.touches[0];
		Painter.$rose.css("left", touch.clientX-Painter.dragOffsetX);
		Painter.$rose.css("top", touch.clientY-Painter.dragOffsetY);	
	} else {
		Painter.$rose.css("left", e.clientX-Painter.dragOffsetX);
		Painter.$rose.css("top", e.clientY-Painter.dragOffsetY);			
	}
	e.preventDefault();
}
Painter.onRoseDragEnd = function(e){
	$(document).on("mousedown touchstart", "#painter .rose", Painter.onRoseDragStart);
	$(document).off("mousemove touchmove", Painter.onRoseDragMove);
	$(document).off("mouseup touchend", Painter.onRoseDragEnd);
	e.preventDefault();
} */
Painter.open = function(){
	Haf.install({height:100, container:$("#painter .canvas-target")});	
	Haf.onResize = Painter.resize;
	Haf.onUpdate = Painter.update;
	Haf.onRender = Painter.render;
	Haf.getCanvas(0).setActive();
	Haf.getCanvas(0).clearColor = "black";
	Haf.inputOverlay.on("mousedown touchstart", function(){
		$("#painter .toolbar-overlay").hide();
	});
	$("#painter .toolbar-overlay").show();
	Haf.start();
	MatrixClient.addEventListener(Painter);
	Colors.hslToRgb255(randomFloat(0, 1.0), 1.0, 0.5);
	Painter.introTimerDuration = 5.0;
	Painter.introTimer = Painter.introTimerDuration;
	PainterTools.open();
}
Painter.close = function(){
	MatrixClient.removeEventListener(Painter);
	PainterTools.close();
	Haf.uninstall();
}
Painter.resize = function(){
	
}
Painter.update = function(){
	if(Painter.introTimer > 0.0){
		Painter.introTimer -= Time.deltaTime;
	}	
	PainterTools.selectedTool.update();
}
Painter.render = function(){
	var windowAspect = Haf.width / Haf.height;
	ctx.save();
	var w = Painter.width+2;
	var h = Painter.height+2;
	var a = w/h;
	if(windowAspect > a){
		ctx.scale(Haf.height/h, Haf.height/h);
	} else {
		ctx.scale(Haf.width/w, Haf.width/w);
	}	
	ctx.translate(-Painter.width/2, -Painter.height/2);
	
	ctx.fillStyle = "#333";
	ctx.fillRect(-1, -1, Painter.width+2, Painter.height+2);
	ctx.fillStyle = "black";
	ctx.fillRect(0, 0, Painter.width, Painter.height);
	
	for(var x = 0; x < Painter.width; x++){
		for(var y = 0; y < Painter.height; y++){
			var p = Painter.pixels[x][y];
			ctx.fillStyle = p.str;
			ctx.fillRect(x+0.1, y+0.1, 0.8, 0.8);			
		}
	}
	ctx.restore();
	if(Painter.introTimer > 0.0){
	/*	ctx.save();
		var hue1 = (Time.sinceStart * 0.2)%1.0;
		var hex1 = Colors.hslToHex(hue1, 1.0, 0.75);
		var hue2 = (Time.sinceStart * 0.2)%1.0;
		var hex2 = Colors.hslToHex(hue2, 1.0, 0.25);
		ctx.globalAlpha = Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration);
		var scale = Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration)*1.5;
		ctx.scale(scale, scale);
		ctx.rotate(Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration*2.0)*0.1);
		ctx.fillStyle = hex1;
		ctx.strokeStyle = hex2;
		ctx.textAlign = "center";
		ctx.textBaseline = "middle";
		ctx.font = (Haf.width*0.08)+"px Arial";
		ctx.lineWidth = Haf.width*0.005;
		ctx.strokeText("draw something!", 0, 0);
		ctx.fillText("draw something!", 0, 0);
		ctx.restore(); */
		ctx.save();
		
				
		var alpha = 1.0;
		var scale = 1.0;
		if(Painter.introTimer/Painter.introTimerDuration >= 0.5){
			scale = Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration);
		}
		if(Painter.introTimer/Painter.introTimerDuration <= 0.25){
			alpha = Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration*2);
			scale = Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration*2);
		}
		ctx.globalAlpha = alpha;
		ctx.translate(-3, 0);
		ctx.scale(scale, scale);
		ctx.rotate(Math.sin(Math.PI*Painter.introTimer/Painter.introTimerDuration*2.0)*0.05);
		ctx.translate(Painter.introTimer*2.5, 0);
		ctx.fillStyle = "white";
		ctx.strokeStyle = "black";
		ctx.textAlign = "center";
		ctx.textBaseline = "middle";
		ctx.font = "bold " + (Haf.width*0.08)+"px Courier New";
		ctx.lineWidth = Haf.width*0.005;
		ctx.strokeText("Draw Something!", 0, 0);
		ctx.fillText("Draw Something!", 0, 0);
		ctx.restore();
	}
}
Painter.transform = function(p){
	var windowAspect = Haf.width / Haf.height;
	var w = Painter.width+2;
	var h = Painter.height+2;
	var a = w/h;
	if(windowAspect > a){
		p.x *= h/Haf.height;
		p.y *= h/Haf.height;
	} else {
		p.x *= w/Haf.width;
		p.y *= w/Haf.width;
	}	
	p.x += Painter.width/2;
	p.y += Painter.height/2;
	p.x = Numbers.clamp(p.x, 0, Painter.width-1);
	p.y = Numbers.clamp(p.y, 0, Painter.height-1);
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
	} else if(json.type == "pixels"){
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
	} else if(json.type == "pixels_chunk"){
		var x = json.x;
		var y = json.y;
		var left = json.x;
		var right = left+json.w;
		var len = json.rgb.length-1; // h suffix
		for(var i = 0; i < len; i+=6){
			var r = parseInt(json.rgb.charAt(i+0)+json.rgb.charAt(i+1), 16);
			var g = parseInt(json.rgb.charAt(i+2)+json.rgb.charAt(i+3), 16);
			var b = parseInt(json.rgb.charAt(i+4)+json.rgb.charAt(i+5), 16);
			Painter.setPixelColor(x, y, r, g, b);
			x++;
			if(x >= right){
				x = left;
				y++;
			}
		}
	}
}
Painter.setPixelColor = function(x, y, r, g, b){
	var p = Painter.pixels[x][y];
	if(p.r255 == r && p.g255 == g && p.b255 == b){
		return false;
	}
	p.r255 = r;
	p.g255 = g;
	p.b255 = b;
	p.str = "rgb(" + r + ", " + g + ", " + b + ")";
	p.rgb = (r<<16)|(g<<8)|b;
	return true;
}

function PainterTools(){}
PainterTools.init = function(){
	$("#painter .close-overlay").on("click", function(){
		$("#painter .toolbar-overlay").hide();
	});
	$("#painter .open-overlay").on("click", function(){
		$("#painter .toolbar-overlay").show();	
	});
	PainterTools.colors = [];
	var lig_steps = 5;
	var lig_step = 1/(lig_steps-1);
	for(var l = 0; l < lig_steps; l++){
		var lig = 1-l*lig_step;
		Colors.hslToRgb255(0, 0, lig);
		PainterTools.colors.push({
			r255: Colors.r255,
			g255: Colors.g255,
			b255: Colors.b255,
			hex: Colors.rgbToHex(Colors.r255, Colors.g255, Colors.b255),
			rgb: (Colors.r255<<16)|(Colors.g255<<8)|Colors.b255,
		});
	}
	PainterTools.colorWhite = PainterTools.colors[0];
	PainterTools.colorBlack = PainterTools.colors[lig_steps-1];
		
	var hsls = [
		[0, 	1.0, 	0.5], // red
		[30,	1.0, 	0.5], // red-orange
		[60,	1.0, 	0.5], // yellow
		[90,	1.0, 	0.5], // green yellowish
		[180, 	1.0, 	0.5], // cyan
		[210, 	1.0, 	0.5], // cyan-blue
		[240, 	1.0, 	0.5], // blue
		[270, 	1.0, 	0.5], // purple
		[300,	1.0, 	0.5], // magenta
		[330,	1.0, 	0.5], // magenta-red	
	];	
	for(var h = 0; h < hsls.length; h++){
		Colors.hslToRgb255(hsls[h][0]/360.0, hsls[h][1], hsls[h][2]);
		PainterTools.colors.push({
			r255: Colors.r255,
			g255: Colors.g255,
			b255: Colors.b255,
			hex: Colors.rgbToHex(Colors.r255, Colors.g255, Colors.b255),
			rgb: (Colors.r255<<16)|(Colors.g255<<8)|Colors.b255,
		});				
	}
	
	/*
	var hue_steps = 24;
	var hue_step = 1/hue_steps;
	for(var h = 0; h < hue_steps; h++){
		var hue = h*hue_step;
		Colors.hslToRgb255(hue, 1, 0.5);
		PainterTools.colors.push({
			r255: Colors.r255,
			g255: Colors.g255,
			b255: Colors.b255,
			hex: Colors.rgbToHex(Colors.r255, Colors.g255, Colors.b255),
			rgb: (Colors.r255<<16)|(Colors.g255<<8)|Colors.b255,
		});		
	}
	*/ 
	
	var $boxes = $("#painter .color-boxes");
	var $row = null;
	for(var i in PainterTools.colors){
		if(i%lig_steps == 0){
			$row = $("<div class='color-box-row'>");
			$boxes.append($row);
		}
		var $box = $("<div class='color-box'>");
		$box.data("red", PainterTools.colors[i].r255);
		$box.data("green", PainterTools.colors[i].g255);
		$box.data("blue", PainterTools.colors[i].b255);
		$box.data("hex", PainterTools.colors[i].hex);
		$box.data("index", i);
		$box.css("background-color", PainterTools.colors[i].hex);
		
		$row.append($box);
	}
	$($boxes.find(".color-box")[0]).addClass("white");
	$("#painter .color-boxes").on("click", ".color-box", function(){
		PainterTools.selectColor($(this).data("index"));
	});
	$("#painter .sizepicker input[type=range]").on("input", function(){
		PainterTools.selectSize($(this).val());
	});
	PainterTools.selectColor(randomInt(lig_steps, lig_steps+hsls.length));
}
PainterTools.open = function(){	
	PainterTools.selectedSize = 1;
	PainterTools.selectedTool = null;
	PainterTools.tools = {
		pen: new PainterToolsPen(),
		bucket: new PainterToolsBucket(),
		eraser: new PainterToolsEraser(),
	}
	PainterTools.selectTool("pen");
}
PainterTools.close = function(){
	PainterTools.selectedTool.onUnselect();
	PainterTools.tools = null;
}
PainterTools.selectTool = function(tool){
	if(tool == "home") return;
	$("#painter .toolbar-label-extra").hide();
	$("#painter .tool-button").removeClass("selected");
	$("#painter .tool-options").hide();
	var button = $("#painter .tool-button[data-tool='"+tool+"']");
	button.addClass("selected");
	if(PainterTools.selectedTool != null){
		PainterTools.selectedTool.onUnselect();
	}	
	PainterTools.selectedTool = PainterTools.tools[tool];
	PainterTools.selectedTool.onSelect();
	$("#painter .toolbar-label-value").text(PainterTools.selectedTool.name);
}
PainterTools.selectColor = function(index){
	$("#painter .color-box").removeClass("selected");
	$($("#painter .color-boxes .color-box")[index]).addClass("selected");
	PainterTools.selectedColor = PainterTools.colors[index];
	$("#painter .toolbar-label-extra-color").css("background", PainterTools.colors[index].hex);
}
PainterTools.selectSize = function(size){
	PainterTools.selectedSize = size;
	$("#painter .sizepicker .size-value").html(size);
	$("#painter .sizepicker input[type=range]").val(size);
	$("#painter .toolbar-label-extra-size").text(size);
}

function PainterToolsPen(){
	this.savedSize = 1;
	this.name = "Draw";
}
PainterToolsPen.prototype.onSelect = function(){
	this.touches = [];
	PainterTools.selectSize(this.savedSize);
	$("#painter .tool-options.colorpicker").show();
	$("#painter .tool-options.sizepicker").show();
	$("#painter .tool-options.sizepicker").show();
	$("#painter .toolbar-label-extra-color").show();
//	$("#painter .toolbar-label-extra-size").show();
}
PainterToolsPen.prototype.onUnselect = function(){
	for(var t in this.touches){
		this.touches[t].canceled = true;
	}
	this.savedSize = PainterTools.selectedSize;
}
PainterToolsPen.prototype.update = function(){
	for(var i in Input.newTouches){		
		var touch = Input.newTouches[i];
		if(touch.taken) continue;
		touch.taken = true;
		this.touches.push(new PainterPenTouch(touch, PainterTools.selectedColor, PainterTools.selectedSize));
	}
	// remove expired touches
	for(var i = this.touches.length-1; i >= 0; i--){
		if (this.touches[i].touch.expired) {
			this.touches.splice(i, 1);
		}
	}
	// update touches
	for(var t in this.touches){
		this.touches[t].update();
	} 
}

function PainterToolsBucket(){
	this.v1 = new Vector();
	this.name = "Fill";
}
PainterToolsBucket.prototype.onSelect = function(){
	$("#painter .tool-options.colorpicker").show();	
	$("#painter .toolbar-label-extra-color").show();
}
PainterToolsBucket.prototype.onUnselect = function(){
	
}
PainterToolsBucket.prototype.update = function(){
	
	var touch = false;
	if(Input.newTouches.length > 0){
		touch = Input.newTouches[0];
	}	
	
	
	if(touch){
		this.v1.x = touch.worldPosition.x;
		this.v1.y = touch.worldPosition.y;
		Painter.transform(this.v1);
		var nx = Math.floor(this.v1.x);
		var ny = Math.floor(this.v1.y);
		var tc = Painter.pixels[nx][ny];
		var rc = PainterTools.selectedColor;
		this.floodFill(nx, ny, tc.rgb, rc.rgb);
						
		var chunk_size = 16;
		var data = null;
		for(var cx = 0; cx < Painter.width; cx += chunk_size){
			for(var cy = 0; cy < Painter.height; cy += chunk_size){
				data = new Array(chunk_size*chunk_size+4);
				data[0] = cx; // x offset
				data[1] = cy; // y offset
				data[2] = chunk_size; // width
				data[3] = chunk_size; // height
				var i = 4;
				for(var x = 0; x < chunk_size; x++){
					for(var y = 0; y < chunk_size; y++){
						data[i++] = Painter.pixels[cx+x][cy+y].rgb;
					}
				}
				MatrixClient.sendMessage("pixels_chunk", data);
			}			
		} 
		/*
		var pixels = new Array(Painter.width*Painter.height);
		var i = 0;
		for(var x = 0; x < Painter.width; x++){
			for(var y = 0; y < Painter.height; y++){
				pixels[i++] = Painter.pixels[x][y].rgb;
			}
		}
		MatrixClient.sendMessage("pixels", pixels);
		*/
	}
}
PainterToolsBucket.prototype.floodFill = function(nx, ny, tc, rc){
	// https://en.wikipedia.org/wiki/Flood_fill#The_algorithm
	var nc = Painter.pixels[nx][ny].rgb;
	if(tc == rc){
		//  1. If target-color is equal to replacement-color, return.
		return;
	} else if(nc != tc){
		// 2. ElseIf the color of node is not equal to target-color, return.
		return;
	} else {
		Painter.setPixelColor(nx, ny, (rc>>16)&0xFF, (rc>>8)&0xFF, rc&0xFF);
		if(nx > 0) 					this.floodFill(nx-1, ny, tc, rc);
		if(nx < Painter.width-1) 	this.floodFill(nx+1, ny, tc, rc);
		if(ny > 0) 					this.floodFill(nx, ny-1, tc, rc);
		if(ny < Painter.height-1) 	this.floodFill(nx, ny+1, tc, rc);		
	}
} 

function PainterToolsEraser(){
	this.savedSize = 8;
	this.name = "Erase";
}
PainterToolsEraser.prototype.onSelect = function(){
	this.touches = [];
	PainterTools.selectSize(this.savedSize);
	$("#painter .tool-options.sizepicker").show();
//	$("#painter .toolbar-label-extra-size").show();
}
PainterToolsEraser.prototype.onUnselect = function(){
	for(var t in this.touches){
		this.touches[t].canceled = true;
	}	
	this.savedSize = PainterTools.selectedSize;
}
PainterToolsEraser.prototype.update = function(){
	for(var i in Input.newTouches){		
		var touch = Input.newTouches[i];
		if(touch.taken) continue;
		touch.taken = true;
		this.touches.push(new PainterPenTouch(touch, PainterTools.colorBlack, PainterTools.selectedSize));
	}		
	// remove expired touches
	for(var i = this.touches.length-1; i >= 0; i--){
		if (this.touches[i].touch.expired) {
			this.touches.splice(i, 1);
		}
	}
	// update touches
	for(var t in this.touches){
		this.touches[t].update();
	} 	
}

function PainterPenTouch(touch, color, size){
	this.touch = touch;
	this.color = color;
	this.size = size;
	this.p1 = new Vector();
	this.p2 = new Vector();
	this.last = new Vector(this.touch.worldPosition.x, this.touch.worldPosition.y);
	Painter.transform(this.last);
	this.canceled = false;
}
PainterPenTouch.init = function(){
	var stamps = PainterPenTouch.stamps = {};
	for(var i = 2; i <= 16; i++){
		var radius = i/2.0;
		var stamp = new Array(i);
		var radius_sqr = (radius-0.1)*(radius-0.1);
		var off = -radius+0.5;
		for(var x = 0; x < i; x++){
			stamp[x] = new Array(i);
			var xo = x+off;
			if(i==2) console.log(xo);
			for(var y = 0; y < i; y++){
				var yo = y+off;
				var dist_sqr = xo*xo+yo*yo;
				stamp[x][y] = dist_sqr < radius_sqr ? 1 : 0;
			}
		}
		stamps[i] = stamp;
	}
}
PainterPenTouch.prototype.applyStamp = function(x, y){
	if(this.size == 1){	
		if(Painter.setPixelColor(x, y, this.color.r255, this.color.g255, this.color.b255)){
			MatrixClient.sendMessage("pixel", [
				this.color.rgb, x, y
			]);				
		}
	} else {
		var stamp = PainterPenTouch.stamps[this.size];
		var x_left = x-Math.floor(this.size/2);
		var y_top = y-Math.floor(this.size/2);
		for(var i = 0; i < this.size; i++){
			var xx = x_left+i;
			if(xx < 0 || xx >= Painter.width) continue;
			for(var j = 0; j < this.size; j++){
				var yy = y_top+j;
				if(yy < 0 || yy >= Painter.height) continue;
				if(stamp[i][j] != 0){
					if(Painter.setPixelColor(xx, yy, this.color.r255, this.color.g255, this.color.b255)){
						MatrixClient.sendMessage("pixel", [
							this.color.rgb, xx, yy
						]);
					}
				}
			}			
		}
	}
}
PainterPenTouch.prototype.update = function(){
	if(this.touch.expired || this.canceled){
		console.log("touch expired");
		return;
	}
	
	var p1 = this.p1;
	var p2 = this.p2;
	var p3 = this.p3;
	
	p1.x = this.touch.worldPosition.x;
	p1.y = this.touch.worldPosition.y;
	
	Painter.transform(p1);
	
	var color = this.color;
	
	p2.x = this.last.x;
	p2.y = this.last.y;
	
	var step = 0.25*this.size;
	var dx = p1.x-p2.x;
	var dy = p1.y-p2.y;
	var dist = Math.sqrt(dx*dx+dy*dy);
	if(dist == 0){
		this.applyStamp(Math.floor(p1.x), Math.floor(p1.y));
	} else {
		var dirx = (p2.x-p1.x)/dist;
		var diry = (p2.y-p1.y)/dist;
		for(var s = 0; s <= dist; s+=step){
			this.applyStamp(Math.floor(p1.x + dirx * s), Math.floor(p1.y + diry * s));
		}				
	}
	this.last.x = p1.x;
	this.last.y = p1.y;
	
}
