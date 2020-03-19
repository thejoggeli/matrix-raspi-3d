function Gallery(){}
Gallery.init = function(){
	$("#gallery .image-upload").on("change", function(e){
		e.preventDefault();
		e.stopPropagation();
		Gallery.processFiles(e.originalEvent.target.files);
	});
	$("#gallery .image-drop").on("click", function(e){
		if($(e.target).hasClass("image-drop")){
			$("#gallery .image-upload").trigger("click")
		}
	})
	$("#gallery .image-drop").on("dragenter", function(e){
		e.preventDefault()
		e.stopPropagation()
		$(this).addClass("drag")
	})
	$("#gallery .image-drop").on("dragleave", function(e){
		e.preventDefault()
		e.stopPropagation()
		$(this).removeClass("drag")
	})
	$("#gallery .image-drop").on("dragover", function(e){
		e.preventDefault()
		e.stopPropagation()
	})
	$("#gallery .image-drop").on("drop", function(e){
		e.preventDefault()
		e.stopPropagation()
		$(this).removeClass("drag")
		var ee = e.originalEvent
		var dt = ee.dataTransfer
		var files = dt.files
		Gallery.processFiles(files)
	})
	Gallery.offscreen = {}
	Gallery.offscreen.canvas = document.createElement("canvas")
	Gallery.offscreen.ctx = Gallery.offscreen.canvas.getContext("2d", {preserveDrawingBuffer: true})
	Gallery.image = null
	Gallery.renderRequired = false
}
Gallery.open = function(){	
	Haf.install({
		height:100, 
		container:$("#gallery .canvas-target"), 
	})		
	Haf.onResize = Gallery.resize
	Haf.onUpdate = Gallery.update
	Haf.onRender = Gallery.render
	Haf.getCanvas(0).setActive()
	Haf.getCanvas(0).clearColor = "gray"
	Haf.start()
	Haf.inputOverlay.hide()
}
Gallery.close = function(){
	Haf.uninstall()
}
Gallery.processFiles = function(files){
	var _file = files[0]
	var imageType = /image.*/
	if (!_file.type.match(imageType)) {
		alert("not an image")
		return
	}
	$("#gallery .loading-overlay").show()
	img = new Image()
	img.name = _file.name
	var reader = new FileReader()
	reader.onload = function(e){
		img.src = e.target.result
	}
	img.onload = function(){
		Gallery.image = img
		$("#gallery .loading-overlay").hide()
		$("#gallery .canvas-target").show()
		$("#gallery .image-drop").hide()
		Gallery.onImageReady()
	}
	reader.readAsDataURL(_file)
	file = _file
}
Gallery.onImageReady = function(){
	Gallery.renderRequired = true
	Haf.resize()
}
Gallery.resize = function(){
	Gallery.renderRequired = true	
}
Gallery.update = function(){
	Haf.getCanvas(0).autoClear = Gallery.renderRequired
}
Gallery.render = function(){
	if(!Gallery.renderRequired){
		return
	}
	var image = Gallery.image
	if(image){
		var aspect = image.width/image.height
		var w = 100 * aspect
		var h = 100
		ctx.drawImage(image, -w/2, -h/2, w, h)
	}
	Gallery.renderRequired = false
}
Gallery.test = function(){
	Gallery.generateOffscreenImage()
	var canvas = Gallery.offscreen.canvas
	
	// JPG Single
	var jpg_header = "data:image/jpeg;base64,"
	var base_64 = canvas.toDataURL('image/jpeg', 0.9).substring(jpg_header.length)
	MatrixClient.sendMessage("gallery_upload_jpg", [base_64])
	console.log(base_64)
	
	$("#gallery .canvas-target").hide()
	$("#gallery .image-drop").show()
	
	// PNG Single
/*	var png_header = "data:image/png;base64,"
	var base_64 = canvas.toDataURL().substring(png_header.length)
	MatrixClient.sendMessage("gallery_upload_png", [base_64])  */
	
	// PNG Chunks
/*	var png_header = "data:image/png;base64,"
	var chunk_size = 8 * 1024 // 16KB
	var base_64 = canvas.toDataURL().substring(png_header.length)
	var chunks = chunkSubstr(base_64, chunk_size)
	
	MatrixClient.sendMessage("gallery_upload_png", ["begin"])
	for(var i in chunks){		
		MatrixClient.sendMessage("gallery_upload_png", ["chunk", chunks[i]])
	}
	MatrixClient.sendMessage("gallery_upload_png", ["end"])
	console.log(chunks) */
}
Gallery.generateOffscreenImage = function(w, h){
	var image = Gallery.image
	if(!image){
		alert("no image")
		return false
	}
	var canvas = Gallery.offscreen.canvas
	var ctx = Gallery.offscreen.ctx
	// ACHTUNG: resulting canvas dimensions have to be power of 2 (mipmaps) 
	var scaling_factor = 8
	canvas.width = 64*scaling_factor
	canvas.height = 32*scaling_factor
	var canvas_aspect = canvas.width/canvas.height
	var image_aspect = image.width/image.height
	var image_w = 0
	var image_h = 0
	var image_x = 0
	var image_y = 0
		
	if(image_aspect > canvas_aspect){
		image_h = canvas.height
		image_w = canvas.height*image_aspect
		image_x = -image_w/2 + canvas.width/2
		image_y = 0
	} else {
		image_w = canvas.width
		image_h = canvas.width/image_aspect
		image_x = 0
		image_y = -image_h/2 + canvas.height/2
	}
	
	ctx.drawImage(image, image_x, image_y, image_w, image_h)
		
	return true
}
