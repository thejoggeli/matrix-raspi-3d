varying vec2 iCoords;
uniform float iTime;
uniform vec3 iRect;
uniform vec3 iCamPos;
uniform mat4 iCamRot;
uniform sampler2D iClockTex;

mat2 rotate(float angle){
	float c = cos(angle);
	float s = sin(angle);
	return mat2(c,-s,s,c);
}

vec3 hsl2rgb(vec3 hsl);
void main() {
	vec2 point = vec2(iCoords.x, 1.0-iCoords.y);
	point.y = clamp(point.y + sin(point.x*5.0+iTime*2.0)*0.1, 0.0, 1.0);
	vec4 tex = texture2D(iClockTex, vec2(point.x, point.y));
	vec2 pp = rotate(mod(iTime*0.1, 3.141*2.0)) * iCoords.xy;
	float hue = (sin(iTime*2.0+pp.x*2.5*pp.y*5.0)*0.5+0.5)*0.1 + sin(iTime*0.1)*0.5+0.5;
//	float light = clamp(sin(iCoords.x*50.0)*0.25+0.75-iCoords.y*0.25, 0.0, 1.0); 
	gl_FragColor = vec4(hsl2rgb(vec3(hue, 1.0, 0.5)), tex.w);
	
}
float hue2rgb(float f1, float f2, float hue) {
	if (hue < 0.0)
		hue += 1.0;
	else if (hue > 1.0)
		hue -= 1.0;
	float res;
	if ((6.0 * hue) < 1.0)
		res = f1 + (f2 - f1) * 6.0 * hue;
	else if ((2.0 * hue) < 1.0)
		res = f2;
	else if ((3.0 * hue) < 2.0)
		res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
	else
		res = f1;
	return res;
}
vec3 hsl2rgb(vec3 hsl) {
	vec3 rgb;			
	if (hsl.y == 0.0) {
		rgb = vec3(hsl.z);
	} else {
		float f2;
		
		if (hsl.z < 0.5)
			f2 = hsl.z * (1.0 + hsl.y);
		else
			f2 = hsl.z + hsl.y - hsl.y * hsl.z;					
		float f1 = 2.0 * hsl.z - f2;				
		rgb.r = hue2rgb(f1, f2, hsl.x + (1.0/3.0));
		rgb.g = hue2rgb(f1, f2, hsl.x);
		rgb.b = hue2rgb(f1, f2, hsl.x - (1.0/3.0));
	}   
	return rgb;
}

