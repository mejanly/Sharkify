varying vec3 normal;
varying vec3 light;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;

uniform float Ushine;
uniform float Uints;

void main()
{
   vec3 lNormal = normalize(normal);
   vec3 lLight = normalize(light);
   vec3 color = vec3(0.0, 0.0, 0.0);
   vec3 ambient, diffuse, specular;
   float diffuseCoefficient, specularCoefficient;

   //ambient
   ambient = UaColor * Uints;

   //diffuse
   diffuseCoefficient = max(0.0, dot(lNormal, lLight));
   diffuse = diffuseCoefficient * UdColor * Uints;

   //specular
   specularCoefficient = 0.0;
   if(diffuseCoefficient > 0.0) {
	   specularCoefficient = pow(max(0.0, dot(vec3(0.0,0.0,1.0), reflect(-lLight, lNormal))), Ushine);
   }
   specular = specularCoefficient * UsColor * Uints;

   color += ambient + diffuse + specular;

   gl_FragColor = vec4(color, 1.0);
}
