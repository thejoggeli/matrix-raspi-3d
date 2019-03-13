varying vec2 iCoords;
uniform float iTime;
uniform vec3 iView;
uniform vec3 iCamPos;
uniform vec3 iCamPosRot;
uniform mat3 iCamRot;
uniform mat3 iCamRotInv;
uniform vec3 iCamScale;

void main() {
	vec3 col = vec3(0.0);
	vec3 p = vec3((iCoords.x-0.5)*iView.z, iCoords.y-0.5, 0.0)*2.0;
	p = p * iCamScale;
	p = iCamRot * p;
	p = p + iCamPos*0.1;
	float time = iTime;
	float zoo = 1.0; // 0.62 + 0.38*cos(.07*time);
	float coa = 1.0; // cos(0.15*(1.0-zoo)*time);
	float sia = 1.0; // sin(0.15*(1.0-zoo)*time);
//	zoo = pow(zoo,8.0);
	vec2 xy = p.xy;
	vec2 c = vec2(-.745,.186) + xy*zoo;
	const float B = 256.0;
	float l = 0.0;
	vec2 z  = vec2(0.0);
	for(int i=0; i<50; i++){
		// z = z*z + c		
		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;		
		if(dot(z,z)>(B*B)) break;
		l += 1.0;
	}
	float sl = l - log2(log2(dot(z,z))) + 4.0; 	
	float al = smoothstep(-0.1, 0.0, abs(sin(0.5*6.2831*iTime)));
	l = mix(l, sl, al);
	col += 0.5 + 0.5*cos(3.0 + l*0.15 + vec3(0.0,0.6,1.0));
    gl_FragColor = vec4(col, 1.0);
}

