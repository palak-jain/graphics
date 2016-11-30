varying vec3 vN;
varying vec3 v;  // varying or uniform check?

void main()
{
  	gl_Position = ftransform();
  	gl_TexCoord[0] = gl_MultiTexCoord0;
	v = vec3(gl_ModelViewMatrix * gl_Vertex);       
	vN = normalize(gl_NormalMatrix * gl_Normal);
  	gl_FrontColor = gl_Color; 
  	gl_BackColor = gl_Color;
}
