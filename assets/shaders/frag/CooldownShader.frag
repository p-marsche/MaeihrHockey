uniform float progress;
uniform int team;
uniform int ability;
uniform int passive;
uniform sampler2D palette;
uniform sampler2D texture;

void main()
{
//Palette Swapping
    vec4 mask = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;
    float teamIdx = float(team);
    float abilityIdx = float(ability);
    float passiveIdx = float(passive);
    int val = int(mask.r * 255.0);

    // get corresponding position in palette texture (4x3 Texture)
    float paletteW = 4.0;
    float paletteH = 3.0;
    float x = float(val)+0.5;
    float y = 0.0;

    if (val == 1)
        y = passiveIdx;
    else if (val == 2)
        y = abilityIdx;
    else if (val == 3)
        y = teamIdx;

    vec3 color = texture2D(palette, vec2(x/paletteW, (y+0.5)/paletteH)).rgb;

// Cooldown visual
    vec2 uv = gl_TexCoord[0].xy - vec2(0.5);
    float angle = -1*atan(uv.x, uv.y) / (2.0 * 3.1415);
    angle += 0.5;
    if (angle < 0.0) { angle += 1.0; };
    
    vec4 col = vec4(color, mask.a);
    float distSq = uv.y*uv.y + uv.x*uv.x;
    
    if (distSq > 0.07 && distSq < 0.22)
    {
        if (angle > progress)
        {
        col = vec4(0.8, 0.8, 0.8, mask.a);
        }
    }
    
    // Output to screen
    gl_FragColor = vec4(col);
}