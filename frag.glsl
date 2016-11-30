#define MAX_LIGHTS 2

uniform sampler2D frag_texture;
uniform float ma, ml, ms, msp, msm;
varying vec3 vN;
varying vec3 v; 

const vec4 AMBIENT_BLACK = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 DEFAULT_BLACK = vec4(0.0, 0.0, 0.0, 0.0);
const vec4 WHITE = vec4(1.0, 1.0, 1.0, 1.0);

void main () { 

   vec3 N = normalize(vN);
   vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
   vec4 objectColor = texture2D(frag_texture, gl_TexCoord[0].st);
   vec4 spec = msm*objectColor + (1.0 - msm)*WHITE;

   for (int i=0;i<MAX_LIGHTS;i++)
   {
      vec3 L = normalize(gl_LightSource[i].position.xyz); // TODO: xyz-v probably not need for dir. 
      vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0) 
      vec3 R = normalize(-reflect(L,N)); 
   	  
   	// ambient
   	// vec4 Iamb = gl_FrontLightProduct[i].ambient*ma*objectColor; 
   	vec4 Iamb = gl_LightSource[i].ambient*ma*objectColor; 
   	Iamb = clamp(Iamb, 0.0, 1.0);

      // calculate Lambertian term
      vec4 Idiff = gl_LightSource[i].diffuse * max(dot(N,L), 0.0)*ml*objectColor;
      Idiff = clamp(Idiff, 0.0, 1.0); 
   
      // calculate Specular Term:
      vec4 Ispec = ms*spec*gl_LightSource[i].specular 
             * pow(max(dot(R,E),0.0),msp);
      Ispec = clamp(Ispec, 0.0, 1.0); 
   
      finalColor += Idiff + Ispec + Iamb;
   }
   
   // write Total Color: 
   gl_FragColor = gl_FrontLightModelProduct.sceneColor + finalColor;

}

