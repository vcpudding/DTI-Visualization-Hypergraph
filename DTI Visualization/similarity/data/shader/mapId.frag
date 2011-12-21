#extension GL_ARB_draw_buffers : enable       
#extension GL_ARB_texture_rectangle : enable 
#extension GL_ARB_texture_rectangle : enable 
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_texture_integer : enable
varying out uvec4 fragData;
void main()
{
	uvec4 siteId = uvec4(gl_TexCoord[0]);
	fragData =siteId;
}