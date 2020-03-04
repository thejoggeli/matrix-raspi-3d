var canvas, ctx;

function Haf(){};
Haf.installed = false;
Haf.install = function(_params){
	var params = _params === undefined ? {} : _params;
//	console.log("Haf: installing");
	if(Haf.installed){
		console.error("Haf already installed ... uninstaling first");
		Haf.uninstall();
	}
	Haf.onResize = function(){}
	Haf.onUpdate = function(){}
	Haf.onRender = function(){}
	Haf.width = 0;
	Haf.height = 0;
	Haf.scale = 1;
	Haf.canvases = [];
	Haf.started = false;
	Haf.inputOverlay = $("<div id='haf-input-overlay'>");
	Haf.inputOverlay.attr("tabindex", -1);
	Haf.$container = params.container === undefined ? $("body") : $(params.container);
	Haf.$container.append(Haf.inputOverlay);
	Haf.camera = new Camera();
	Haf.title = def(params, "title", "no title");
	Haf.width = def(params, "width", 0);
	Haf.height = def(params, "height", 1024);
	Haf.createCanvas({autoClear:params.autoClear||true, container:Haf.$container});
	Haf.getCanvas(0).setActive();
	Haf.requestAnimationFrameId = null;
	$(Haf.getCanvas(0).element).focus();
	$(window).on("resize orientationchange", Haf.resize);
	CollisionManager.buildMatrix();
	Haf.resize();
	Input.install();
	Haf.inputOverlay.focus();
	Haf.installed = true;
}
Haf.uninstall = function(){
	Haf.installed = false;
//	console.log("Haf: uninstalling");
	$(window).off("resize orientationchange", Haf.resize);
	canvas = null;
	ctx = null;
	for(var i in Haf.canvases){
		Haf.canvases[i].element.remove();
	}
	Input.uninstall();
	window.cancelAnimationFrame(Haf.requestAnimationFrameId);
	Haf.inputOverlay.remove();
}
Haf.start = function(){
	if(Haf.started){
		console.error("Haf already started");
		return;
	}
	Time.start();
	Haf.requestAnimationFrameId = window.requestAnimationFrame(Haf.frame);
	Haf.started = true;
}
Haf.resume = function(){
	if(!Haf.started){
		Haf.start();
	} else if (Haf.paused){
		Input.clear();
		Haf.paused = false;
		Haf.requestAnimationFrameId = window.requestAnimationFrame(Haf.frame);
	}
}
Haf.pause = function(){
	Haf.paused = true;
}
Haf.show = function(){
	for(var x in this.canvases){
		$(this.canvases[x].element).show();
	}
	Haf.inputOverlay.show();
	Haf.inputOverlay.focus();
	Haf.resize();
}
Haf.hide = function(){
	for(var x in this.canvases){
		$(this.canvases[x].element).hide();
	}
	Haf.inputOverlay.hide();	
}
Haf.getCanvas = function(index){
	return Haf.canvases[index];
}
Haf.createCanvas = function(_params){
	var params = _params === undefined ? {} : _params;
	if(params.container === undefined){
		params.container = Haf.$container;
	}
	var canvas = new Canvas("canvas-"+Haf.canvases.length+1, params);
	Haf.canvases.push(canvas);
//	canvas.element.width = document.documentElement.clientWidth;
//	canvas.element.height = document.documentElement.clientHeight;
	return canvas;
}
Haf.resize = function(){
	var containerWidth = Haf.$container.innerWidth();
	var containerHeight = Haf.$container.innerHeight();
	Haf.width = containerWidth/containerHeight * Haf.height;	
	for(var x in Haf.canvases){
		var canvas = Haf.canvases[x];
		canvas.element.width = containerWidth;
		canvas.element.height = containerHeight;
		Haf.scale = canvas.element.height / Haf.height;
	}
	var $hi = Haf.inputOverlay;
	$hi.width(containerWidth);
	$hi.height(containerHeight);
	Haf.onResize();
}
Haf.frame = function(){
	if(!Haf.installed || Haf.paused) return;
	Time.update();
	Input.update();
	Haf.onUpdate();
	if(!Haf.installed || Haf.paused) return;	
	for(var x in Haf.canvases){
		if(Haf.canvases[x].autoClear){
			if(Haf.canvases[x].clearColor != null){
				Haf.canvases[x].ctx.fillStyle = Haf.canvases[x].clearColor;
				Haf.canvases[x].ctx.fillRect(0,0,Haf.canvases[x].element.width, Haf.canvases[x].element.height);				
			} else {
				Haf.canvases[x].ctx.clearRect(0,0,Haf.canvases[x].element.width, Haf.canvases[x].element.height);				
			}
		}
		if(Haf.canvases[x].autoTransform){
			Haf.canvases[x].transform();
		}
	}
	Haf.onRender();
	for(var x in Haf.canvases){
		if(Haf.canvases[x].autoTransform){
			Haf.canvases[x].untransform();
		}
	}
	
	Input.clearFrameKeys();
	Haf.requestAnimationFrameId = window.requestAnimationFrame(Haf.frame);
}
Haf.setBackgroundColor = function(hex){
	$("body").css("background", hex);
}
Haf.screenToWorldX = function(x_pos){
	x_pos -= canvas.element.width/2;
	x_pos /= Haf.scale;
	x_pos /= Haf.camera.zoom;
	x_pos += Haf.camera.position.x;
	return x_pos;
}
Haf.screenToWorldY = function(y_pos){
	y_pos -= canvas.element.height/2;
	y_pos /= Haf.scale;
	y_pos /= Haf.camera.zoom;
	y_pos += Haf.camera.position.y;
	return y_pos;
}
Haf.cameraMovement = function(){	
	// camera
	if(Input.isKeyDown(82)){
		Haf.camera.zoom += Time.rawDeltaTime * Haf.camera.zoom;		
	} else if(Input.isKeyDown(70)){
		Haf.camera.zoom -= Time.rawDeltaTime * Haf.camera.zoom;
		if(Haf.camera.zoom < 0.01) Haf.camera.zoom = 0.01;
	}
	var cameraSpeed = 500;
	var cameraSpeed = 500;
	var cameraX = 0;
	var cameraY = 0;
	if(Input.isKeyDown(87)){
		cameraY = -1;
	} else if(Input.isKeyDown(83)){
		cameraY = 1;
	}
	if(Input.isKeyDown(65)){
		cameraX = -1;
	} else if(Input.isKeyDown(68)){
		cameraX = 1;
	}
	if(cameraX != 0 || cameraY != 0){	
		var cameraMoveAngle = Math.atan2(cameraY, cameraX) + Haf.camera.rotation;
		cameraX = Math.cos(cameraMoveAngle);
		cameraY = Math.sin(cameraMoveAngle);
	}
	Haf.camera.position.x += cameraX * Time.rawDeltaTime * cameraSpeed / Haf.camera.zoom;
	Haf.camera.position.y += cameraY * Time.rawDeltaTime * cameraSpeed / Haf.camera.zoom;
}

function Camera(){
	this.position = new Vector();
	this.zoom = 1;
	this.rotation = 0;
	this.bounds = new AABBCollider();
}
Camera.prototype.recalcBounds = function(){
	var aspect = Haf.width/Haf.height;
	this.bounds.position.x = this.position.x - (Haf.height*aspect*0.5) / this.zoom;
	this.bounds.width = (Haf.height*aspect) / this.zoom;
	this.bounds.position.y = this.position.y - (Haf.height*0.5) / this.zoom;
	this.bounds.height = Haf.height / this.zoom;
}

var ColliderType = {};
ColliderType.AABB = 0;
ColliderType.POINT = 1;
ColliderType.CIRCLE = 2;

function BaseCollider(){
	this.position = new Vector();
}
BaseCollider.prototype.intersects = function(other){
	return CollisionManager.checkIntersection(this, other);
}

var type = 0;
function PointCollider(){
	BaseCollider.apply(this);
}
PointCollider.prototype = new BaseCollider;
PointCollider.prototype.constructor = BaseCollider;
PointCollider.prototype.COLLIDER_TYPE = ColliderType.POINT;

function AABBCollider(){
	BaseCollider.apply(this);
	this.width = 0;
	this.height = 0;
}
AABBCollider.prototype = new BaseCollider;
AABBCollider.prototype.constructor = BaseCollider;
AABBCollider.prototype.COLLIDER_TYPE = ColliderType.AABB;

function CircleCollider(){
	BaseCollider.apply(this);
	this.radius = 0;
}
CircleCollider.prototype = new BaseCollider;
CircleCollider.prototype.constructor = BaseCollider;
CircleCollider.prototype.COLLIDER_TYPE = ColliderType.CIRCLE;

function CollisionManager(){}
CollisionManager.checkIntersection = function(c1,c2){
	return CollisionManager.matrix[c1.COLLIDER_TYPE][c2.COLLIDER_TYPE](c1,c2);
}
CollisionManager.matrix = {}
CollisionManager.buildMatrix = function(){
	for(var i in ColliderType){
		CollisionManager.matrix[ColliderType[i]] = {};
		for(var j in ColliderType){
			var min, max;
			if(i < j){
				min = i;
				max = j;
			} else {
				min = j;
				max = i;
			}
			var funcName = min+"_"+max;
			var func;
			if(CollisionManager.checkFunctions[funcName] === undefined){
				func =	CollisionManager.checkFunctions["NOT_IMPLEMENTED"];
			//	console.log(funcName + " collision function not found");
			} else {
				func = CollisionManager.checkFunctions[funcName];
			}
			CollisionManager.matrix[ColliderType[i]][ColliderType[j]] = func;
		}	
	}	
}
CollisionManager.checkFunctions = {
	"NOT_IMPLEMENTED": function(){
		console.log("not implemented");
	},
	"AABB_AABB": function(c1,c2){
		if(c1.position.x > c2.position.x+c2.width) return false;
		if(c1.position.y > c2.position.y+c2.height) return false;
		if(c1.position.x+c1.width < c2.position.x) return false;
		if(c1.position.y+c1.height < c2.position.y) return false;
		return true;
	},
	"AABB_POINT": function(c1,c2){
		
	},
	"POINT_POINT": function(c1,c2){
		if(c1.position.x != c2.position.x) return false;
		if(c1.position.y != c2.position.y) return false;
		return true;		
	},
	"CIRCLE_CIRCLE": function(c1,c2){
		var xx = c1.position.x - c2.position.x;
		var yy = c1.position.y - c2.position.y;
		var radius = c1.radius + c2.radius;
		radius *= radius;
		var distance = xx*xx+yy*yy;
		return distance <= radius;
	},
	"CIRCLE_POINT": function(c1,c2){
		
	},
};
CollisionManager.isPointInsidePolygon = function(px, py, points){
	var j = points.length-1;
	var c = 0;
	for(var i = 0; i < points.length; j=i++){
		if(((points[i][1]>py) != (points[j][1]>py)) && 
			(px < (points[j][0]-points[i][0]) * (py-points[i][1]) / (points[j][1]-points[i][1]) + points[i][0]))
				c = c == 0 ? 1 : 0;
	} 
	return c == 0 ? false : true;
}
CollisionManager.isPointInsideRectangle = function(px, py, rx, ry, rw, rh){
	if(px < rx-rw/2) return false;
	if(px > rx+rw/2) return false;
	if(py < ry-rh/2) return false;
	if(py > ry+rh/2) return false;
	return true;
}
CollisionManager.isPointInsideCircle = function(px, py, cx, cy, cr){
	var x = px-cx;
	var y = py-cy;
	return x*x+y*y < cr*cr;
}

function Time(){}
Time.fps = 0;
Time.fpsCount = 0;
Time.lastFpsTime; 	// initialized on start
Time.rawDeltaTime; 	// initialized on start
Time.deltaTime;		// initialized on start
Time.sinceStart = 0;		// initialized on start
Time.sinceReset;
Time.currentTime;		// initialized on start
Time.lastTime;		// initialized on start
Time.startTime;		// initialized on start
Time.resetTime;
Time.scale = 1;
Time.maxDeltaTime = 1/4;
Time.rawFixedStep = 1/60;
Time.fixedStep = 1/60;
Time.lostTimeSinceStart = 0;
Time.lostTimeSinceReset = 0;
Time.sinceStartMinusLost = 0;
Time.sinceResetMinusLost = 0;

Time.start = function(){
	Time.currentTime = Time.getTime();
	Time.lastTime = Time.currentTime
	Time.lastFpsTime = Time.currentTime;
	Time.deltaTime = 0;
	Time.sinceStart = 0;
	Time.sinceReset = 0;
	Time.startTime = Time.currentTime;
	Time.resetTime = Time.currentTime;
}

Time.update = function(){
	Time.lastTime = Time.currentTime;
	Time.currentTime = Time.getTime();
	Time.rawDeltaTime = (Time.currentTime - Time.lastTime);
	Time.deltaTime = Time.rawDeltaTime * Time.scale;
	Time.sinceStart = Time.currentTime - Time.startTime;
	Time.sinceReset = Time.currentTime - Time.resetTime;
	if(Time.rawDeltaTime > Time.maxDeltaTime){
		Time.lostTimeSinceStart += Time.rawDeltaTime;
		Time.lostTimeSinceReset += Time.rawDeltaTime;
		Time.rawDeltaTime = Time.maxDeltaTime;
	}
	if(Time.deltaTime > Time.maxDeltaTime){
		Time.deltaTime = Time.maxDeltaTime;
	}
	Time.sinceStartMinusLost = Time.sinceStart - Time.lostTimeSinceStart;
	Time.sinceResetMinusLost = Time.sinceReset - Time.lostTimeSinceReset;
	// fps count
	Time.fpsCount++;
	if(Time.currentTime-Time.lastFpsTime >= 1.0){
		Time.lastFpsTime += 1.0;
		Time.fps = Time.fpsCount;
		Time.fpsCount = 0;
	}
}

Time.reset = function(){
	Time.resetTime = Time.currentTime;
	Time.sinceReset = 0;
	Time.lostTimeSinceReset = 0;
}

Time.getTime = function(){
	return (new Date()).getTime()/1000;
}

Time.setScale = function(scale){
	Time.scale = scale;
	Time.fixedStep = Time.rawFixedStep = Time.scale;
}

Time.setFixedStep = function(step){
	Time.rawFixedStep = step;
	Time.fixedStep = Time.rawFixedStep * Time.scale;
}

function Canvas(id, params){
	this.clearColor = def(params, "clearColor", null);
	this.autoClear = def(params, "autoClear", true);
	this.autoTransform = def(params, "autoTransform", true);
	this.element = document.createElement("canvas");
	$(params.container).append(this.element);
	this.ctx = this.element.getContext("2d");
	this.autoResize = params.autoResize === undefined ? true : params.autoResize;
	extendedCtx(this.ctx);
}
Canvas.prototype.setActive = function(){
	canvas = this;
	ctx = this.ctx;
}
Canvas.prototype.transform = function(){	
	var ctx = this.ctx;
	var canvas = this;
	ctx.save();
	ctx.translate(canvas.element.width/2, canvas.element.height/2);
	ctx.scale(Haf.scale, Haf.scale);
	ctx.rotate(-Haf.camera.rotation);
	ctx.scale(Haf.camera.zoom, Haf.camera.zoom);
	ctx.translate(-Haf.camera.position.x, -Haf.camera.position.y);
}
Canvas.prototype.untransform = function(){
	this.ctx.restore();
}
Canvas.prototype.clear = function(){
	this.ctx.clearRect(0, 0, this.element.width, this.element.height);
}

function Input(){}
Input.install = function(params){
	Input.handlers = {keyDown:[], keyUp:[]};
	Input.downKeys = [];
	Input.frameDownKeys = [];
	Input.frameUpKeys = [];
	Input.newTouches = [];
	Input.touches = {};
	Input.hasFrameKeys = false;
	Input.mouse = {
		screenPosition: new Vector(),
		worldPosition: new Vector(),
		isDown: false,
		downFrame: false,
		upFrame: false,
	};
	for(var i = 0; i < 256; i++){
		Input.downKeys[i] = false;
		Input.frameDownKeys[i] = false;
		Input.frameUpKeys[i] = false;
	}
	Haf.inputOverlay.on("keydown", function(e){
		if(Input.downKeys[e.keyCode] == false){ 
			Input.downKeys[e.keyCode] = true;
			for(var x in Input.handlers.keyDown){
				Input.handlers.keyDown[x](e.keyCode);
			}
			Input.frameDownKeys[e.keyCode] = true;
			Input.hasFrameKeys = true;
		}
	});
	Haf.inputOverlay.on("keyup", function(e){
		Input.downKeys[e.keyCode] = false;
		for(var x in Input.handlers.keyUp){
			Input.handlers.keyUp[x](e.keyCode);
		}		
		Input.frameUpKeys[e.keyCode] = true;
		Input.hasFrameKeys = true;
	});	
	Haf.inputOverlay.on("touchstart", function(e){
		for(var t = 0; t < e.changedTouches.length; t++){
			var touch = e.changedTouches[t];
			var touchHandle = new TouchHandle(touch);
			Input.newTouches.push(touchHandle);
			Input.touches[touchHandle.id] = touchHandle;	
		}
		e.preventDefault();
		e.stopPropagation();
	});
	Haf.inputOverlay.on("touchmove", function(e){
		for(var t = 0; t < e.changedTouches.length; t++){		
			var touch = e.changedTouches[t];
			var touchHandle = Input.touches[touch.identifier];
			touchHandle.screenPosition.setFloats(touch.clientX, touch.clientY);	
		}
		e.preventDefault();
		e.stopPropagation();
	});
	Haf.inputOverlay.on("touchend", function(e){
		for(var t = 0; t < e.changedTouches.length; t++){
			var touch = e.changedTouches[t];
			var touchHandle = Input.touches[touch.identifier];
			touchHandle.expired = true;
			delete Input.touches[touchHandle.id];			
		}	
		e.preventDefault();
		e.stopPropagation();
	});
	Haf.inputOverlay.on("mousedown", function(e){
		if(e.which == 1){
			Input.mouse.isDown = true;
			Input.mouse.downFrame = true;
			var touchHandle = new TouchHandle("mouse");
			Input.newTouches.push(touchHandle);
			Input.touches.mouse = touchHandle;
			Input.updateMousePosition(e);
		}
	});
	Haf.inputOverlay.on("mouseup", function(e){
		if(e.which == 1){
			Input.mouse.isDown = false;
			Input.mouse.upFrame = true;
			Input.updateMousePosition(e);
			if(Input.touches.mouse !== undefined){
				Input.touches.mouse.expired = true;
				delete Input.touches.mouse;
			}
		}
	});
	Haf.inputOverlay.on("mousemove ", function(e){			
		Input.mouse.screen_x = e.pageX;
		Input.mouse.screen_y = e.pageY;
		Input.updateMousePosition(e);
	});
}
Input.uninstall = function(){
	Input.clearFrameKeys();
}
Input.update = function(){
	for(var t in Input.touches){
		var touch = Input.touches[t];
		touch.worldPosition.x = Haf.screenToWorldX(touch.screenPosition.x);
		touch.worldPosition.y = Haf.screenToWorldY(touch.screenPosition.y);	
	}
}
Input.updateMousePosition = function(e){	
	Input.mouse.screenPosition.x = e.pageX;
	Input.mouse.screenPosition.y = e.pageY;
	Input.mouse.worldPosition.x = Haf.screenToWorldX(Input.mouse.screenPosition.x);
	Input.mouse.worldPosition.y = Haf.screenToWorldY(Input.mouse.screenPosition.y);
	if(Input.touches.mouse !== undefined){
		Input.touches.mouse.worldPosition.setVector(Input.mouse.worldPosition);
		Input.touches.mouse.screenPosition.setVector(Input.mouse.screenPosition);
	}
	
}
Input.clearFrameKeys = function(){
	if(Input.hasFrameKeys){
		for(var i = 0; i < 256; i++){
			Input.frameDownKeys[i] = false;
			Input.frameUpKeys[i] = false;
		}
		Input.hasFrameKeys = false;
	}
	Input.mouse.downFrame = false;
	Input.mouse.upFrame = false;
	// clear touches
	if(Input.newTouches.length > 0){
		Input.newTouches = [];
	}
}
Input.clear = function(){
	Input.downKeys = [];
	Input.frameDownKeys = [];
	Input.frameUpKeys = [];
	Input.newTouches = [];	
	for(var i = 0; i < 256; i++){
		Input.downKeys[i] = false;
		Input.frameDownKeys[i] = false;
		Input.frameUpKeys[i] = false;
	}
	Input.touches = {};
	Input.hasFrameKeys = false;
	Input.mouse.isDown = false;
	Input.mouse.downFrame = false;
	Input.mouse.upFrame = false;
}
Input.isKeyDown = function(code){
	return (Input.downKeys[code] === true);
}
Input.onKeyDown = function(handler){
	Input.handlers.keyDown.push(handler);
}
Input.onKeyUp = function(handler){
	Input.handlers.keyUp.push(handler);
}
Input.keyDown = function(code){
	return Input.frameDownKeys[code];
}
Input.keyUp = function(code){
	return Input.frameUpKeys[code];	
}
Input.isMouseDown = function(){
	return Input.mouse.isDown;
}
Input.mouseDown = function(){
	return Input.mouse.downFrame;
}
Input.mouseUp = function(){
	return Input.mouse.upFrame;	
}

function TouchHandle(touch){
	this.screenPosition = new Vector();
	this.worldPosition = new Vector();	
	if(touch === "mouse"){
		this.id = "mouse_" + TouchHandle.mouseIdCount++;
		this.isMouse = true;
		this.touch = null;
	} else {
		this.id = touch.identifier;
		this.isMouse = false;
		this.touch = touch;
		this.screenPosition.setFloats(touch.clientX, touch.clientY);
		this.worldPosition.x = Haf.screenToWorldX(this.screenPosition.x);
		this.worldPosition.y = Haf.screenToWorldY(this.screenPosition.y);		
	}	
	this.expired = false;
	this.taken = false;
}
TouchHandle.mouseIdCount = 0;

function Monitor(){}
Monitor.$element;
Monitor.rows = {};
Monitor.setup = function(_params){
	Monitor.$element = $("#monitor-box");
	Monitor.$element.show();
}

Monitor.add = function(key, name, value){
	if(Monitor.rows[key] !== undefined){
		console.log("ERROR: monitor " + key + " already registered");
	} else {
		var row = new MonitorRow(key, name);
		Monitor.rows[key] = row;
		Monitor.$element.append(row.$element);
		if(value !== undefined){
			Monitor.set(key, value);
		}
	}
}

Monitor.setVisible = function(visible){
	if(visible){
		Monitor.$element.show();
	} else {		
		Monitor.$element.hide();
	}
}
Monitor.isVisible = function(){
	return Monitor.$element.is(":visible");
}

Monitor.set = function(key, value){
	if(Monitor.rows[key] === undefined){
		Monitor.add(key, key, value);
	} else {
		Monitor.rows[key].set(value);
	}
}
Monitor.label = function(name){
	var $label = $("<div class='monitor-label'></div>");
	$label.html(name);
	Monitor.$element.append($label);
}

function Controls(){}
Controls.setup = function(){
	Controls.$element = $("#controls-box");	
	Controls.$element.show();
}
Controls.add = function(name, value){
	var $element = $("<div class='controls-row'>");
	$element.append("<span class='controls-name'>"+name+": </span>");
	var $value = $("<span class='controls-value'></span>");
	$value.html(value);
	$element.append($value);
	Controls.$element.append($element);
}
Controls.label = function(name){
	var $label = $("<div class='controls-label'></div>");
	$label.html(name);
	Controls.$element.append($label);
}

function MonitorRow(key, name){
	this.key = key;
	this.name = name;
	this.$element = $("<div class='monitor-row'>");
	this.$element.append("<span class='monitor-name'>"+name+": </span>");
	this.$value = $("<span class='monitor-value'></span>");
	this.$element.append(this.$value);
}
MonitorRow.prototype.set = function(value){
	if(value === true) value = "true";
	else if(value === false) value = "false";
	this.$value.html(value);
};
MonitorRow.prototype.destory = function(){
	$element.remove();
}

function Vector(x,y){
	this.x = x===undefined?0:x;
	this.y = y===undefined?0:y;	
	return this;
}
Vector.prototype.setFloats = function(x, y){
	this.x = x;
	this.y = y;
	return this;
}
Vector.prototype.setVector = function(v){
	this.setFloats(v.x, v.y);
	return this;
}
Vector.prototype.getLength = function(){
	return Math.sqrt(this.x*this.x+this.y*this.y);
}
Vector.prototype.addFloats = function(x, y){
	this.setFloats(this.x+x, this.y+y);
	return this;
}
Vector.prototype.addVector = function(v){
	this.setFloats(this.x+v.x, this.y+v.y);
	return this;
}
Vector.prototype.subFloats = function(x, y){
	this.setFloats(this.x-x, this.y-y);
	return this;
}
Vector.prototype.subVector = function(v){
	this.setFloats(this.x-v.x, this.y-v.y);
	return this;
}
Vector.prototype.mulFloat = function(f){
	this.x *= f;
	this.y *= f;
	return this;
}
Vector.prototype.mulFloats = function(x,y){
	this.x *= x;
	this.y *= y;
	return this;
}
Vector.prototype.getAngle = function(){
	return Math.atan2(this.y, this.x);
}
Vector.prototype.normalize = function(){
	var mag = this.getLength();
	this.setFloats(this.x/mag, this.y/mag);
	return this;
}
Vector.prototype.clone = function(){
	return new Vector(this.x, this.y);
}
Vector.prototype.toString = function(){
	return "Vector(x:" + roundTo(this.x,3) + "|y:" + roudnTo(this.y,3) + ")";
}
Vector.getDistanceBetween = function(v1, v2){
	var x = v1.x-v2.x;
	var y = v1.y-v2.y;
	return Math.sqrt(x*x+y*y);
}
Vector.getAngleBetween = function(v1, v2){
	return Math.atan2(v2.y-v1.y, v2.x-v1.x);
}

function Functions(){}
function randomPositiveNegative(val){
	if(randomInt(0,2)==0){
		return -val;
	}
	return val;
}

function roundTo(val, decimals){
	var p = Math.pow(10, decimals);
	return Math.round(val*p)/p;
}

function roundToFixed(val, decimals){
	return roundTo(val,decimals).toFixed(decimals);
}

function randomInt(min,max){
	return Math.floor(Math.random()*(max-min)+min);
}

function randomElement(array){
	return array[0, randomInt(0, array.length)];
}

function randomFloat(min,max){
	return Math.random()*(max-min)+min;
}

function randomBool(trueChance){
	return Math.random() < trueChance;
}

function Colors(){};
Colors.h = 0;
Colors.s = 0;
Colors.l = 0;
Colors.r = 0;
Colors.g = 0;
Colors.b = 0;
Colors.a = 0;
Colors.r255 = 0;
Colors.g255 = 0;
Colors.b255 = 0;
Colors.a255 = 0;

Colors.rgbToHex = function(r, g, b) {
	return "#" + Colors.componentToHex(r) + Colors.componentToHex(g) + Colors.componentToHex(b);
}

Colors.rgbToHexInt = function(r, g, b) {
	return (r << 16) + (g << 8) + b;
}

// helper function for rgbToHex()
Colors.componentToHex = function(c) {
	var hex = c.toString(16);
	return hex.length == 1 ? "0" + hex : hex;
}

Colors.hslToHex = function(h, s, l){
	Colors.hslToRgb(h,s,l);
	Colors.rgbTo255(Colors.r, Colors.g, Colors.b);
	return Colors.rgbToHex(Colors.r255, Colors.g255, Colors.b255);
}

Colors.hslToHexInt = function(h, s, l){
	Colors.hslToRgb(h,s,l);
	Colors.rgbTo255(Colors.r, Colors.g, Colors.b);
	return Colors.rgbToHexInt(Colors.r255, Colors.g255, Colors.b255);
}

/*
 * h = hue (0-1)
 * s = saturation (0-1)
 * l = lightness (0-1)
 */
Colors.hslToRgb = function(h, s, l){
	h %= 1.0;
	var r, g, b;
	if(s == 0){
		r = g = b = l; // achromatic
	} else {
		var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		var p = 2 * l - q;
		r = Colors.hue2rgb(p, q, h + 1/3);
		g = Colors.hue2rgb(p, q, h);
		b = Colors.hue2rgb(p, q, h - 1/3);
	}
	Colors.r = r;
	Colors.g = g;
	Colors.b = b;
}
Colors.hslToRgb255 = function(h, s, l){
	Colors.hslToRgb(h, s, l);
	Colors.rgbTo255(Colors.r, Colors.g, Colors.b);
}
Colors.rgbTo255 = function(r, g, b){
	if(r < 0) r = 0;
	if(g < 0) g = 0;
	if(b < 0) b = 0;
	Colors.r255 = r >= 1.0 ? 255 : Math.floor(r*256);
	Colors.g255 = g >= 1.0 ? 255 : Math.floor(g*256);
	Colors.b255 = b >= 1.0 ? 255 : Math.floor(b*256);
}

// helper function for hslToRgb
Colors.hue2rgb = function(p, q, t){
	if(t < 0) t += 1;
	if(t < 0) t += 1;
	if(t > 1) t -= 1;
	if(t < 1/6) return p + (q - p) * 6 * t;
	if(t < 1/2) return q;
	if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
	return p;
}

function def(obj, key, def){
	if(obj[key] === undefined){
		return def;
	}
	return obj[key];
}

function extendedCtx(ctx){
	ctx.strokeLine = function(x1, y1, x2, y2){
		ctx.beginPath();
		ctx.moveTo(x1,y1);
		ctx.lineTo(x2,y2);
		ctx.stroke();	
	};
	ctx.strokeCircle = function(x, y, radius){
		ctx.beginPath();
		ctx.arc(x, y, radius, 0, Numbers.PI2, false);
		ctx.stroke();	
	};
	ctx.fillCircle = function(x, y, radius){
		ctx.beginPath();
		ctx.arc(x, y, radius, 0, Numbers.PI2, false);
		ctx.fill();	
	};
	ctx.fillArrow = function(x, y, width, length){
		ctx.beginPath();
		ctx.moveTo(x, y);
		ctx.lineTo(x+width/2, y-width/1.5);
		ctx.lineTo(x+width/6, y-width/1.5);
		ctx.lineTo(x+width/6, y-length);
		ctx.lineTo(x-width/6, y-length);
		ctx.lineTo(x-width/6, y-width/1.5);
		ctx.lineTo(x-width/2, y-width/1.5);
		ctx.fill();
	}
	ctx.strokeArrow = function(x, y, width, length){
		ctx.beginPath();
		ctx.moveTo(x, y);
		ctx.lineTo(x+width/2, y-width/1.5);
		ctx.lineTo(x+width/6, y-width/1.5);
		ctx.lineTo(x+width/6, y-length);
		ctx.lineTo(x-width/6, y-length);
		ctx.lineTo(x-width/6, y-width/1.5);
		ctx.lineTo(x-width/2, y-width/1.5);
		ctx.closePath();
		ctx.stroke();
	}
	ctx.fillQuad = function(x1, y1, x2, y2, x3, y3, x4, y4){
		ctx.beginPath();
		ctx.moveTo(x1, y1);
		ctx.lineTo(x2, y2);
		ctx.lineTo(x3, y3);
		ctx.lineTo(x4, y4);
		ctx.closePath();
		ctx.fill();
	}
	ctx.fillRoundRect = function(x, y, width, height, radius){
		ctx.pathRoundRect(x, y, width, height, radius);
		ctx.fill();
	}
	ctx.strokeRoundRect = function(x, y, width, height, radius){
		ctx.pathRoundRect(x, y, width, height, radius);	
		ctx.stroke();	
	}
	ctx.pathRoundRect = function(x, y, width, height, radius){
	/*	if (typeof stroke == 'undefined') {
			stroke = true;
		}
		if (typeof radius === 'undefined') {
			radius = 5;
		}
		if (typeof radius === 'number') {
			radius = {tl: radius, tr: radius, br: radius, bl: radius};
		} else {
			var defaultRadius = {tl: 0, tr: 0, br: 0, bl: 0};
			for (var side in defaultRadius) {
				radius[side] = radius[side] || defaultRadius[side];
			}
		}
		ctx.beginPath();
		ctx.moveTo(x + radius.tl, y);
		ctx.lineTo(x + width - radius.tr, y);
		ctx.quadraticCurveTo(x + width, y, x + width, y + radius.tr);
		ctx.lineTo(x + width, y + height - radius.br);
		ctx.quadraticCurveTo(x + width, y + height, x + width - radius.br, y + height);
		ctx.lineTo(x + radius.bl, y + height);
		ctx.quadraticCurveTo(x, y + height, x, y + height - radius.bl);
		ctx.lineTo(x, y + radius.tl);
		ctx.quadraticCurveTo(x, y, x + radius.tl, y);
		ctx.closePath(); */
		ctx.beginPath();
		ctx.moveTo(x + radius, y);
		ctx.lineTo(x + width - radius, y);
		ctx.quadraticCurveTo(x + width, y, x + width, y + radius);
		ctx.lineTo(x + width, y + height - radius);
		ctx.quadraticCurveTo(x + width, y + height, x + width - radius, y + height);
		ctx.lineTo(x + radius, y + height);
		ctx.quadraticCurveTo(x, y + height, x, y + height - radius);
		ctx.lineTo(x, y + radius);
		ctx.quadraticCurveTo(x, y, x + radius, y);
		ctx.closePath();	
	}
}

function Numbers(){}
Numbers.PI2 = Math.PI*2;
Numbers.rad2deg = 1/Math.PI*180;
Numbers.deg2rad = 1/180*Math.PI;

Numbers.clamp = function(x, min, max){
	if(x < min) return min;
	if(x > max) return max;
	return x;
}

Numbers.moveTowardsAngle = function(current, target, speed){
	current = current%Numbers.PI2;
	target = target%Numbers.PI2;
	if(current<0) current += Numbers.PI2;
	if(target<0) target += Numbers.PI2;
	var d1 = current-target;
	if(d1 < 0) {d1 += Numbers.PI2};
	var d2 = target-current;
	if(d2 < 0) {d2 += Numbers.PI2};
	if(d1>d2){
		return current+speed*Numbers.PI2;		
	} else {
		return current-speed*Numbers.PI2;
	}
}

Numbers.deltaAngle = function(a1, a2){
	a1 = a1%Numbers.PI2;
	a2 = a2%Numbers.PI2;
	if(a1<Numbers.PI2) a1 += Numbers.PI2;
	if(a2<Numbers.PI2) a2 += Numbers.PI2;
	var diff = a2-a1;
	if(diff > Math.PI){
		diff = -(Numbers.PI2-diff);
	} else if(diff < -Math.PI){
		diff = Numbers.PI2+diff;
	}
	return diff;
}

Numbers.lerpAngle = function(minAngle, maxAngle, time){
	if(time >= 1){
		return maxAngle;
	} else {		
		minAngle = minAngle%Numbers.PI2;
		maxAngle = maxAngle%Numbers.PI2;
		if(minAngle<Numbers.PI2) minAngle += Numbers.PI2;
		if(maxAngle<Numbers.PI2) maxAngle += Numbers.PI2;
		var diff = maxAngle-minAngle;
		if(diff > Math.PI){
			diff = -(Numbers.PI2-diff);
		} else if(diff < -Math.PI){
			diff = Numbers.PI2+diff;
		}
		return minAngle + diff * time;
	}
}
Numbers.lerp = function(a, b, d){
	if(a == b) return a;
	var min = a > b ? b : a;
	var max = a > b ? a : b;
	return Numbers.clamp(a*(1-d)+b*d, min, max);
}

function ArrayHelper(){}
ArrayHelper.removeObject = function(array, obj){
	var index = array.indexOf(obj);
	if (index > -1) {
		array.splice(index, 1);
	}
}
ArrayHelper.removeIndex = function(array, index){	
	array.splice(index, 1);
}

var hsl_r;
var hsl_g;
var hsl_b;

var si_formats = [
	{value: 1E-18, symbol: "a"},
	{value: 1E-15, symbol: "f"},
	{value: 1E-12, symbol: "p"},
	{value: 1E-9, symbol: "n"},
	{value: 1E-6, symbol: "u"},
	{value: 1E-3, symbol: "m"},
	{value: 1, symbol: ""},
	{value: 1E3, symbol: "k"},
	{value: 1E6, symbol: "M"},
	{value: 1E9, symbol: "G"},
	{value: 1E12, symbol: "T"},
	{value: 1E15, symbol: "P"},
	{value: 1E18, symbol: "E"}
];

function nFormat(num, digits) {
	digits = digits || 2;
	if(num == 0) return 0;
	var rx = /\.0+$|(\.[0-9]*[1-9])0+$/;
	var i;
	var pos = Math.abs(num);
	for (i = si_formats.length - 1; i > 0; i--) {
		if (pos >= si_formats[i].value) {
			break;
		}
	}
	return (num / si_formats[i].value).toFixed(digits).replace(rx, "$1") + si_formats[i].symbol;
}

function pad(pad, str, padLeft) {
	if (typeof str === 'undefined') 
		return pad;
	if (padLeft) {
		return (pad + str).slice(-pad.length);
	} else {
		return (str + pad).substring(0, pad.length);
	}
}



