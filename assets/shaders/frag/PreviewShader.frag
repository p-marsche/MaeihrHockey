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

    gl_FragColor = vec4(color, mask.a);
}