#version 120

varying vec4 theColor;
uniform sampler2D tex;

void main() {
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    finalColor += theColor;
	
    gl_FragColor = clamp( finalColor, 0.0, 1.0 ); // make sure all our values are between 0 and 1
}
