// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex

//textura (anna)
uniform sampler2D u_anna;
//variables del raster fs
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

void main()
{
	// Set the ouput color per pixel
    vec4 texture_color = texture2D(u_anna, v_uv);

    gl_FragColor = texture_color;
}
