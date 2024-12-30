#version 450 core
in vec2 textureOut;
out vec4 FragColor;
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;
void main() {
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture2D(tex_y, textureOut).r;
        yuv.y = texture2D(tex_u, textureOut).r - 0.5;
	yuv.z = texture2D(tex_v, textureOut).r - 0.5;
	rgb = mat3(1, 		1, 		  1,
			   0, 		-0.39465, 2.03211,
			   1.13983, -0.58060, 0) * yuv;
        FragColor = vec4(rgb, 1);
}
