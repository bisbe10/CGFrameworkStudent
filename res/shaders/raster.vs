//CODI IMPLEMENTAT DEL SIMPLE.VS
// Variables CPU.
uniform mat4 u_model;
uniform mat4 u_viewprojection;

// Variables que passarem també al fs
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;


void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// local position a world space.
    vec4 glVertex4 = vec4( gl_Vertex.xyz, 1.0);
	vec3 world_position = (u_model * glVertex4).xyz;

	// local normal a world space:
    vec4 glNormal4 = vec4( gl_Vertex.xyz, 0.0);
	vec3 world_normal = (u_model * glNormal4).xyz;

    
	v_world_position = world_position;
	v_world_normal = world_normal;

	// col·loquem la textura en clip space.
	gl_Position = u_viewprojection * vec4(world_position, 1.0); 
}
