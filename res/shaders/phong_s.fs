// Global variables from the CPU
varying vec3 v_world_normal;
varying vec3 v_world_position;

// Varyings (varibales que passem al fs)
varying vec2 v_uv;

//here create uniforms for all the data we need here
uniform float shine;
uniform float option;
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
uniform vec3 pos1;
uniform vec3 Id1;
uniform vec3 Is1;
uniform vec3 eye;

void main()
{
    float option1= 1.0;
    vec3 L;vec3 L1; vec3 N; vec3 V; vec3 R;vec3 v_Ip; vec4 texture_color;
    float dist;float dist1;vec3 R1;
    //practica 5-------------------------------------------------
    //vector que va de p a light
    L = normalize((pos - v_world_position));
    L1 = normalize((pos1 - v_world_position));
    //normal del punt
    N = normalize((v_world_normal));

    ////vector que va de p a eye
    V = normalize((eye - v_world_position));

    //reflexe (inverció de l sobre n)
    R = reflect(-L, N);
    R1 = reflect(-L1, N);

    texture_color = texture2D(u_anna, v_uv);
    

    vec3 kd = vec3(texture_color.w);
    vec3 ka = vec3(texture_color.w);
    vec3 ks = vec3(texture_color.w);
    
    
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
    
    gl_FragColor = vec4( v_Ip, 1.0 );
}

