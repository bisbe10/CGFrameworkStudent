// This variables comes from the vertex shader

varying vec3 v_Ip;

void main()
{
        
    gl_FragColor = vec4( v_Ip, 1.0 );
    

}
