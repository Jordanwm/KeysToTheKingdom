#version 120

varying vec4 theColor;
uniform float time;

void main(void) {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	float red = mod(time, 2);
	
	theColor = vec4(red, 0.0, 0.0, 1.0);
}