varying vec2 iCoords;
uniform float iTime;
uniform vec3 iRect;
uniform vec3 iCamPos;
uniform mat4 iCamRot;
uniform vec2 iPixel;

mat2 rotate(float angle){
	float c = cos(angle);
	float s = sin(angle);
	return mat2(c,-s,s,c);
}

vec3 hsl2rgb(vec3 hsl);
void main() {
	vec2 point = iCoords.xy * iRect.xy;
	if(point.x < 1.0 || point.x > iRect.x-1.0 || point.y < 1.0 || point.y > iRect.y-1.0){
		float hue = mod(mod(abs(sin(iCoords.x*1.5 + iTime*0.25)), 1.0)*0.1+sin(iTime*0.1)*0.5+0.5, 1.0);
		gl_FragColor = vec4(hsl2rgb(vec3(hue, 1.0, 0.5)), 1.0);
	} else {
		gl_FragColor = vec4(0.0);
	}
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

