function Sfx(){}
Sfx.files = {};
Sfx.currentMusicFile = null;
Sfx.onWebsocketMessage = function(json){
	if(json.type == "load_sfx"){
		if(!(json.files instanceof Array)){
			json.files = [json.files];
		}
		if(!(json.volume instanceof Array)){
			json.volume = [json.volume];
		}
		if(!(json.start instanceof Array)){
			json.start = [json.start];
		}
		if(!(json.end instanceof Array)){
			json.end = [json.end];
		}
		for(var x in json.files){
			Sfx.loadFile(json.files[x], json.start[x], json.end[x], json.volume[x]);
		}
	} else if(json.type == "play_sound"){
		Sfx.playSound(json);
	} else if(json.type == "start_music"){
		Sfx.startMusic(json);
	} else if(json.type == "stop_music"){
		Sfx.stopMusic();
	}
}
Sfx.onWebsocketClose = function(){
	Sfx.stopMusic();
}
Sfx.playSound = function(json){
	var file = Sfx.getFile(json.file);
	if(file != null){
		file.volume = json.volume !== undefined ? json.volume : file._def_volume;
		file.currentTime = json.start !== undefined ? json.start : file._def_start;
		file.loop = false;
		file.play();
	}
	if(Sfx.currentMusicFile != null && Sfx.currentMusicFile.currentTime == 0){
		Sfx.currentMusicFile.play();
	}
}
Sfx.startMusic = function(json){
	var file = Sfx.getFile(json.file);
	if(file != null){
		if(Sfx.currentMusicFile == file){
			console.log("Music already playing");			
		} else {
			if(Sfx.currentMusicFile != null){
				Sfx.stopMusic();
			}
			Sfx.currentMusicFile = file;
			file.volume = json.volume !== undefined ? json.volume : file._def_volume;
			file.currentTime = json.start !== undefined ? json.start : file._def_start;
			file.loop = true;
			file.play();
		}
	}	
}
Sfx.stopMusic = function(){
	if(Sfx.currentMusicFile != null){
		Sfx.currentMusicFile.pause();
		Sfx.currentMusicFile = null;
	}	
}
Sfx.getFile = function(name){
	if(Sfx.files[name] === undefined){
		console.log("Audio file not found: " + name);
		return null;
	}
	return Sfx.files[name];
}
Sfx.loadFile = function(name, start, end, volume){
	var audio;
	if(Sfx.files[name] !== undefined){
		console.log("Audio file already added: " + name);
		audio = Sfx.files[name];
	} else {
		audio = new Audio();
		audio.src = "sfx/" + name;
		Sfx.files[name] = audio;
		console.log("loading: " + audio.src);
		audio.addEventListener("canplaythrough", function(){
			console.log("ready: " + this.src);
		});		
	}
	audio._def_start = start;
	audio._def_end = end;
	audio._def_volume = volume;
}
