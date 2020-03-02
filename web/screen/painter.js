function Painter(){}
Painter.init = function(){
	$(".painter .send").on("click", function(){
		for(var i = 0; i < 100; i++){
			var r = randomInt(0, 256);
			var g = randomInt(0, 256);
			var b = randomInt(0, 256);
			var x = randomInt(0, 64);
			var y = randomInt(0, 32);
			MatrixClient.sendMessage("paint", [
				(r<<16)|(g<<8)|b, x, y
			]);
		}
	});
}
Painter.open = function(){
	
}
Painter.close = function(){
	
}
