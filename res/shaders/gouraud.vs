// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;


// Varyings (varibales que passem al fs)
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;


//LI PASSEM AL FS
varying vec3 v_Ip;

//here create uniforms for all the data we need here
uniform float shine;
uniform sampler2D u_anna;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform mat4 model;
uniform mat4 view_projection;
uniform vec3 Ia;
uniform vec3 pos;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 eye;
uniform vec3 pos1;
uniform vec3 Id1;
uniform vec3 Is1;
uniform float option;


void main()
{
    //parctica 4----------------------------------------------------------
	v_uv = gl_MultiTexCoord0.xy;

    // local position a world space.
    vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;

    // local normal a world space:
    vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
	v_world_position = world_position;
	v_world_normal = world_normal;
    
    vec3 L; vec3 N; vec3 V; vec3 R; vec4 texture_color;float dist1;vec3 R1;float dist;vec3 L1;
    //practica 5-------------------------------------------------


    L = normalize((pos - v_world_position));
    L1 = normalize((pos1 - v_world_position));
    //normal del punt
    N = normalize((v_world_normal));

    ////vector que va de p a eye
    V = normalize((eye - v_world_position));

    //reflexe (inverció de l sobre n)
    R = reflect(-L, N);
    R1 = reflect(-L1, N);

    
    vec3 l_a= ka * Ia;
    //normalitzem amb el clamp ( per evitar els valors negatius)
    vec3 l_dif=  kd * (clamp(dot(L,N),0.0,1.0)) * Id;
    
    vec3 l_dif1=  kd * (clamp(dot(L1,N),0.0,1.0)) * Id1;
    
    vec3 l_spec= ks * pow((clamp(dot(R,V),0.0,1.0)),shine)*Is;
    
    vec3 l_spec1= ks * pow((clamp(dot(R1,V),0.0,1.0)),shine)*Is1;
    //OBJECTIU -> TROBAR Ip
    dist=pow(distance(v_world_position,pos),-2.0);
    
    dist1=pow(distance(v_world_position,pos1),-2.0);
    
     v_Ip = l_a + dist*(l_dif + l_spec) + dist1*(l_dif1+l_spec1);
    
	 //Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}
