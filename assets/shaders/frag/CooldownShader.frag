uniform float progress;
uniform sampler2D texture;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;
    vec2 uv = gl_TexCoord[0].xy - vec2(0.5);

    float angle = -1*atan(uv.x, uv.y) / (2.0 * 3.1415);
    angle += 0.5;
    if (angle < 0.0) { angle += 1.0; };
    
    vec4 col = color;
    float distSq = uv.y*uv.y + uv.x*uv.x;
    
    if (distSq > 0.07 && distSq < 0.22)
    {
        if (angle > progress)
        {
        col = vec4(0.8, 0.8, 0.8, color.a);
        }
    }
    
    // Output to screen
    gl_FragColor = vec4(col);
}