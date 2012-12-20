#version 110

uniform sampler2D	tex0;
uniform sampler2D	tex1;


void main()
{ 
    vec4 color = texture2D(tex0, gl_TexCoord[0].st);
    vec3 mask  = texture2D(tex1, gl_TexCoord[0].st).rgb;

    float lumma = 0.2126*mask.r + 0.7152*mask.g + 0.0722*mask.b;
    
    gl_FragColor.rgb =color.rgb;
    gl_FragColor.a = color.a*lumma*2.0;
}