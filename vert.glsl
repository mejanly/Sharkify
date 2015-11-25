attribute vec4 aPosition;
attribute vec3 aNormal;
attribute vec3 uColor;

uniform vec3 uLightPos;
uniform mat4 uProjMatrix;
uniform mat4 uModelMatrix;

varying vec3 normal;
varying vec3 light;

void main()
{
	normal = aNormal;
	light = uLightPos;
	gl_Position = uProjMatrix * uModelMatrix * aPosition;
}

