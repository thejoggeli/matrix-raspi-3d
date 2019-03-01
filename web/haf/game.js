
function main(){
	// setup haf 
	Haf.setup({
		height:100,
	});
	Haf.update = Gamepad.update;
	Haf.render = Gamepad.render;
	// init
	Gamepad.init();
	Gamepad.recalcButtons();
	// start
	Haf.setBackgroundColor("#131313");
	Haf.start();
	$(window).on("resize", function(){
		Gamepad.recalcButtons();
	});
};

function Gamepad(){}
Gamepad.debugDraw = false;
Gamepad.buttons = {};
Gamepad.sticks = {};
Gamepad.cross = {};
Gamepad.colors = {
	main_button: "#ff0b00",
	main_button_pressed: "yellow",
	main_button_label: "#eee",
	main_button_label_pressed: "#eee",
	outline: "#eee",
	border: "black",
	pad: "#333",
	pad_pressed: "yellow",
	arrows: "black",	
	arrows_pressed: "yellow",
};
Gamepad.lineWidth;
Gamepad.init = function(){
	Gamepad.buttons.up = new GamepadArrowButton({
		label: "up", code: 0, color: "blue", angle:0,
	});
	Gamepad.buttons.down = new GamepadArrowButton({
		label: "down", code: 0, color: "blue", angle:Math.PI,
	});	
	Gamepad.buttons.left = new GamepadArrowButton({
		label: "left", code: 0, color: "blue", angle:Math.PI*1.5,
	});
	Gamepad.buttons.right = new GamepadArrowButton({
		label: "right", code: 0, color: "blue", angle:Math.PI*0.5
	}); 
	Gamepad.buttons.a = new GamepadMainButton({
		label: "A", code: 0, color: "red",
	});
	Gamepad.buttons.b = new GamepadMainButton({
		label: "B", code: 0, color: "green",
	});
/*	Gamepad.buttons.start = new GamepadCircleButton({
		label: "start", code: 0, color: "black",
	});
	Gamepad.buttons.select = new GamepadCircleButton({
		label: "select", code: 0, color: "black",
	}); */
	Gamepad.sticks.left = new GamepadStick();
	Gamepad.sticks.right = new GamepadStick();
	Gamepad.sticks.right.enabled = false;
}
Gamepad.update = function(){
	for(var x in Gamepad.sticks){
		if(!Gamepad.sticks[x].enabled) continue;
		Gamepad.sticks[x].update();	
	}
	for(var x in Gamepad.buttons){
		var px, py;	
		var button = Gamepad.buttons[x];
		var released = false;
		var pressed = false;
		
		// check if current touch is still active
		if(button.fingerTouch !== null){			
			if(button.fingerTouch.expired){
				// touchend
				button.fingerTouch = null;
				released = true;
			} else {		
				// check if touch left button hitbox
				px = button.fingerTouch.worldPosition.x;
				py = button.fingerTouch.worldPosition.y;
				if(!button.isPointInside(px, py)){
					// yep
					button.fingerTouch.taken = false;
					button.fingerTouch = null;
					released = true;
				}		
			}		
		}		
		// check if a touch is over button
		if(button.fingerTouch == null){
			for(var t in Input.touches){
				var touch = Input.touches[t];
				if(touch.taken || touch.expired) continue;
				px = touch.worldPosition.x;
				py = touch.worldPosition.y;
				if(button.isPointInside(px, py)){
					// touch over button found!
					pressed = true;
					button.fingerTouch = touch;
					button.fingerTouch.taken = true;
					break;
				}
			}
		}
		if(pressed && released){
			// no change
		} else if(pressed){
			button.onDown();
		} else if(released){
			button.onUp();
		}
		button.update();
	}
}
Gamepad.render = function(){
	if(Haf.width < Haf.height){
		ctx.font = "16px Arial";
		ctx.fillStyle = "black";
		ctx.textBaseline = "middle";
		ctx.textAlign = "center";
		ctx.fillText("TURN", 0, -27);
		ctx.fillText("YOUR", 0, -9);
		ctx.fillText("PHONE", 0, 9);
		ctx.save();
		ctx.font = "20px Arial";
		ctx.translate(0, 30);
		ctx.rotate(Math.sin(Time.sinceStart)*Math.PI/2+Math.PI/2);
		ctx.fillText(":)", 0, 0);		
		ctx.restore();
	} else {
		if(Gamepad.debugDraw){		
			for(var x in Gamepad.buttons){
				Gamepad.buttons[x].renderDebug();
			}
		}
		for(var x in Gamepad.buttons){
			Gamepad.buttons[x].render();
		}
		var cross = Gamepad.cross;
		var s1 = cross.center_w+cross.border_offset*2;
		var s2 = cross.center_w+cross.border_offset*2+cross.outline_offset*2;
		ctx.fillStyle = Gamepad.colors.border;
		ctx.fillRect(cross.x-s1/2, cross.y-s2/2, s1, s2);
		s1 = cross.center_w;
		s2 = cross.center_w+cross.border_offset*2+cross.outline_offset*2;
		ctx.fillStyle = Gamepad.colors.pad;
		ctx.fillRect(cross.x-s1/2, cross.y-s2/2, s1, s2);
		ctx.fillRect(cross.x-s2/2, cross.y-s1/2, s2, s1);
		for(var x in Gamepad.sticks){
			if(!Gamepad.sticks[x].enabled) continue;
			Gamepad.sticks[x].render();	
		}
	}
	
}
Gamepad.recalcButtons = function(){
	var btn = Gamepad.buttons;
	var width = Haf.width;
	var height = Haf.height;	
	
	// cross
	var cross_max_w = width*0.5; 
	var cross_w = height;
	var cross_h = height;
	if(cross_w > cross_max_w){
		cross_w = cross_max_w;
		cross_h = cross_w;
	}
	var center_s = cross_w*0.225;
	var cross_x = -width/2 + cross_w/2;
	var cross_dx = cross_w/2 - center_s/2;
	var cross_dy = cross_h/2 - center_s/2;
	var cross = Gamepad.cross;
	cross.x = cross_x; 
	cross.y = 0;
	cross.w = cross_w;
	cross.h = cross_h;
	cross.center_w = center_s; //cross_w*0.22;
	cross.center_h = center_s; //cross.center_w;
	cross.arrow_length = cross.h*0.44;
	cross.outline_offset = cross.w*0.015;
	cross.border_offset = cross.w*0.0125;
	Gamepad.lineWidth = cross.border_offset;
	// cross center
	btn.up.position.setFloats(cross_x, -center_s/2-cross_dy/2);
	btn.down.position.setFloats(cross_x, +center_s/2+cross_dy/2);
	btn.left.position.setFloats(cross_x-center_s/2-cross_dx/2, 0);
	btn.right.position.setFloats(cross_x+center_s/2+cross_dx/2, 0);
	// cross size
	btn.up.setSize(center_s, cross_w, cross_dy);
	btn.down.setSize(center_s, cross_w, cross_dy);
	btn.left.setSize(center_s, cross_h, cross_dx);
	btn.right.setSize(center_s, cross_h, cross_dx);
	var cross_right = cross_x + cross_w/2;
	var rem_w = width - cross_w;
	
	// stick
	var stick_radius = cross.center_w/2*1.25;
	var stick_max_distance = cross_w*0.375 - stick_radius;
	var stick_center_radius = stick_radius * 0.8;
	var stick = Gamepad.sticks.left;
	stick.setPosition(cross_x, 0);
	stick.radius = stick_radius;
	stick.centerRadius = stick_center_radius;
	stick.maxDistance = stick_max_distance;
	
	stick = Gamepad.sticks.right;
	stick.setPosition(-cross_x, 0);
	stick.radius = stick_radius;
	stick.centerRadius = stick_center_radius;
	stick.maxDistance = stick_max_distance;
	
	// ab
	var ab_w = rem_w/3*2;
	if(ab_w > cross_w) {
		ab_w = cross_w;
	}
	var ab_h = cross_h;
	var ab_x = width/2-ab_w/2;
	btn.a.position.setFloats(ab_x, ab_h/4);
	btn.b.position.setFloats(ab_x, -ab_h/4);
	btn.a.setSize(ab_w, ab_h/2);
	btn.b.setSize(ab_w, ab_h/2);
}

function GamepadStick(params){
	this.enabled = true;
	this.center = new Vector();
	this.centerRadius;
	this.position = new Vector();
	this.touchOffset = new Vector();
	this.releasePosition = new Vector();
	this.releaseTimer = 1;
	this.releaseTimerScale = 1;
	this.radius = 5;
	this.touch = null;
	this.maxDistance;
}
GamepadStick.prototype.update = function(){
	for(var i in Input.newTouches){
		var touch = Input.newTouches[i];
		if(touch.taken) continue;
		var px = touch.worldPosition.x;
		var py = touch.worldPosition.y;
		if(CollisionManager.isPointInsideCircle(px, py, this.position.x, this.position.y, this.radius)){
			this.touch = touch;
		 	touch.taken = true;
		 	this.touchOffset.x = this.position.x - this.touch.worldPosition.x;
		 	this.touchOffset.y = this.position.y - this.touch.worldPosition.y;
		}
	}
	if(this.touch != null){
		if(this.touch.expired){
			this.touch = null;
			this.releasePosition.setVector(this.position);
			this.releaseTimer = 0;
			this.releaseTimerScale = 1/Vector.getDistanceBetween(this.position, this.center)*200;
			this.touchOffset.setFloats(0, 0);
		} else {
			this.position.setVector(this.touch.worldPosition);
			this.position.addVector(this.touchOffset);
		}
	}
	if(this.touch == null){
		this.releaseTimer += Time.deltaTime * this.releaseTimerScale;
		this.position.x = Numbers.lerp(this.releasePosition.x, this.center.x, this.releaseTimer);
		this.position.y = Numbers.lerp(this.releasePosition.y, this.center.y, this.releaseTimer);
	}
	this.distance = Vector.getDistanceBetween(this.position, this.center);
	if(this.distance > this.maxDistance){	
		var angle = Vector.getAngleBetween(this.center, this.position);
		this.position.x = this.center.x + Math.cos(angle)*this.maxDistance;
		this.position.y = this.center.y + Math.sin(angle)*this.maxDistance;
	}
}
GamepadStick.prototype.render = function(){
	// draw line
	ctx.lineWidth = this.radius*0.5;
	ctx.strokeStyle = "black";
	ctx.strokeLine(this.center.x, this.center.y, this.position.x, this.position.y);
	ctx.fillStyle = "black";
	ctx.fillCircle(this.center.x, this.center.y, ctx.lineWidth/2);
	ctx.lineWidth = Gamepad.lineWidth;
	ctx.strokeCircle(this.center.x, this.center.y, this.radius);
	// draw cirlce
	var cross = Gamepad.cross;
	ctx.lineWidth = Gamepad.lineWidth;
	ctx.fillStyle = this.isTouched() ? Gamepad.colors.pad_pressed : Gamepad.colors.pad;
	ctx.fillCircle(this.position.x, this.position.y, this.radius);
/*	if(!this.isTouched() && this.releaseTimer < 1){
		ctx.globalAlpha = 1-(this.releaseTimer < 1 ? this.releaseTimer : 0);
		ctx.fillStyle = Gamepad.colors.pad_pressed;
		ctx.fillCircle(this.position.x, this.position.y, this.radius);
		ctx.globalAlpha = 1;
	} */
	ctx.strokeStyle = Gamepad.colors.arrows;
	ctx.strokeCircle(this.position.x, this.position.y, this.radius);
	
}
GamepadStick.prototype.setPosition = function(x, y){
	this.center.setFloats(x, y);
	this.position.setFloats(x, y);
	this.releasePosition.setFloats(x, y);
}
GamepadStick.prototype.isTouched = function(){
	return this.touch == null ? false : true;
}

function GamepadArrowButton(params){
	this.position = new Vector();
	this.win; this.wout; this.h;
	this.angle = params.angle;
	this.label = params.label; 
	this.code = params.code;
	this.points;
	this.fingerTouch = null;
}
GamepadArrowButton.prototype.update = function(){}
GamepadArrowButton.prototype.renderDebug = function(){
	// debug draw
	ctx.beginPath();
	ctx.lineWidth = 0.666;
	ctx.strokeStyle = "black";
	ctx.fillStyle = this.isTouched() ? "yellow" : this.color;
	for(var i in this.points){
		ctx.lineTo(this.points[i][0], this.points[i][1]);
	}
	ctx.closePath();
	ctx.fill();
	ctx.stroke(); 
}
GamepadArrowButton.prototype.render = function(){
	// cross draw
	var cross = Gamepad.cross;
	var border_offset = cross.border_offset;
	var outline_offset = cross.outline_offset;
	var width = cross.center_w;
	var height = cross.arrow_length;
	var radius = cross.center_w*0.1;
	ctx.save();
	ctx.translate(cross.x, cross.y);
	ctx.rotate(this.angle+Math.PI);
	// outline
	var w1 = cross.center_w + outline_offset*2 + border_offset*2;
	var h1 = height + outline_offset + border_offset;
	ctx.fillStyle = Gamepad.colors.outline;
	ctx.fillRoundRect(0-w1/2, 0, w1, h1, radius);	
	// border
	var w2 = cross.center_w + border_offset*2;
	var h2 = height + border_offset;
	ctx.fillStyle = Gamepad.colors.border;
	ctx.fillRoundRect(0-w2/2, 0, w2, h2, radius);	
	// pad
	var w3 = cross.center_w;
	var h3 = height;
	ctx.fillStyle = Gamepad.colors.pad;
	ctx.fillRoundRect(0-w3/2, 0, w3, h3, radius);
	// pad touched
	if(this.isTouched()){
		var h4 = h3*0.62;
		ctx.fillStyle = Gamepad.colors.pad_pressed;
		ctx.fillRoundRect(0-w3/2, h3-h4, w3, h4, radius);
		ctx.fillRect(0-w3/2, h3-h4, w3, radius, radius);
	}
	// arrow
	ctx.lineWidth = Gamepad.lineWidth;
	var ay1 = height*0.93;
	var ay2 = height*0.7;
	var ay3 = height*0.5;
	var aw2 = width*0.7;
	var aw3 = width*0.5;
	ctx.beginPath();
	ctx.lineTo(0, ay1);
	ctx.lineTo(aw2/2, ay2);
	ctx.lineTo(aw3/2, ay2);
	ctx.lineTo(aw3/2, ay3);
	ctx.lineTo(-aw3/2, ay3);
	ctx.lineTo(-aw3/2, ay2);
	ctx.lineTo(-aw2/2, ay2);
	ctx.closePath();
	if(this.isTouched()){
		ctx.fillStyle = Gamepad.colors.arrows_pressed;
		ctx.fill();
	}
	ctx.strokeStyle = Gamepad.colors.arrows;
	ctx.stroke();
	ctx.restore();
}
GamepadArrowButton.prototype.setSize = function(win, wout, h){
	this.win = win;
	this.wout = wout;
	this.h = h;
	var cosa = Math.cos(this.angle);
	var sina = Math.sin(this.angle);
	this.points = [];
	this.points[0] = [-this.wout/2, -this.h/2];
	this.points[1] = [this.wout/2, -this.h/2];
	this.points[2] = [this.win/2, this.h/2];
	this.points[3] = [-this.win/2, this.h/2];
	for(var i in this.points){
		var x = this.points[i][0];
		var y = this.points[i][1];
		this.points[i][0] = x*cosa - y*sina;
		this.points[i][1] = x*sina + y*cosa;
		this.points[i][0] += this.position.x;
		this.points[i][1] += this.position.y;
	}
}
GamepadArrowButton.prototype.isTouched = function(){
	return this.fingerTouch != null;
}
GamepadArrowButton.prototype.onDown = function(){
	console.log("ondown");
}
GamepadArrowButton.prototype.onUp = function(){
	console.log("onup");
}
GamepadArrowButton.prototype.isPointInside = function(x, y){
	return CollisionManager.isPointInsidePolygon(x, y, this.points);
}

function GamepadMainButton(params){
	this.position = new Vector();
	this.width = 25;
	this.height = 25;
	this.label = params.label; 
	this.color = params.color;
	this.code = params.code;
	this.fingerTouch = null;
}
GamepadMainButton.prototype.update = function(){

}
GamepadMainButton.prototype.renderDebug = function(){
	ctx.fillStyle = this.isTouched() ? "yellow" : this.color;
	ctx.lineWidth = 0.666;
	ctx.fillRect(this.position.x-this.width/2, this.position.y-this.height/2, this.width, this.height);	
	ctx.strokeStyle = "black";
	ctx.strokeRect(this.position.x-this.width/2, this.position.y-this.height/2, this.width, this.height);
}
GamepadMainButton.prototype.render = function(){	
	var w = this.height*0.9;
	var h = w;
	var r1 = w*0.1;
	var x = this.position.x; // Haf.width/2 - this.height/2;
	ctx.save();
	ctx.translate(x,this.position.y);
	ctx.fillStyle = Gamepad.colors.outline;
	ctx.fillRoundRect(-w/2, -h/2, w, h, r1);
	var button_radius = w/2*0.8;
	ctx.fillStyle = this.isTouched() ? Gamepad.colors.main_button_pressed : Gamepad.colors.main_button;
	ctx.fillCircle(0, 0, button_radius);
	// label
	ctx.font = "bold 16px Arial";
	ctx.textAlign = "center";
	ctx.textBaseline = "middle";
	ctx.fillStyle = this.isTouched() ? Gamepad.colors.main_button_label_pressed : Gamepad.colors.main_button_label;
	ctx.fillText(this.label, 0, 0);
	ctx.restore();
}
GamepadMainButton.prototype.setSize = function(w, h){
	this.width = w;
	this.height = h;
}
GamepadMainButton.prototype.isTouched = function(){
	return this.fingerTouch != null;
}
GamepadMainButton.prototype.onDown = function(){
	console.log("ondown");
}
GamepadMainButton.prototype.onUp = function(){
	console.log("onup");
}
GamepadMainButton.prototype.isPointInside = function(x, y){
	return CollisionManager.isPointInsideRectangle(x, y, this.position.x, this.position.y, this.width, this.height);
}

function GamepadMidButton(){
	this.width = 25;
	this.height = 25;
	this.label = params.label; 

}



