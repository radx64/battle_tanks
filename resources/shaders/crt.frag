// CRT post-processing shader.
// Simulates an old CRT monitor: barrel distortion, scanlines,
// chromatic aberration, vignette, phosphor flicker and a rolling refresh band.

uniform sampler2D texture;   // scene rendered to an offscreen texture
uniform vec2 resolution;     // texture resolution in pixels
uniform float time;          // seconds since boot

// Bend the flat image around a virtual tube.
vec2 curve(vec2 uv)
{
    uv = (uv - 0.5) * 2.0;
    uv *= 1.15;
    uv.x *= 1.0 + pow(abs(uv.y) / 20.0, 2.0);
    uv.y *= 1.0 + pow(abs(uv.x) / 8.0, 2.0);
    uv = uv * 0.5 + 0.5;
    return uv;
}

// Moulded plastic monitor frame surrounding the glass.
vec3 bezel(vec2 uv)
{
    vec2 edge = clamp(uv, 0.0, 1.0);   // nearest point on the glass
    vec2 d = uv - edge;                // how far we are out into the frame
    float dist = length(d);

    // Base plastic tone.
    vec3 plastic = vec3(0.10, 0.11, 0.09);

    // Light from the top-left gives the frame some volume.
    vec2 n = normalize(d + vec2(1e-4));
    float lit = clamp(dot(n, normalize(vec2(-0.6, -1.0))), 0.0, 1.0);
    vec3 col = plastic * (0.55 + 0.55 * lit);

    // Bright inner lip where the plastic meets the glass...
    col += smoothstep(0.035, 0.0, dist) * 0.20;
    // ...and a dark recessed groove hugging the glass.
    float groove = smoothstep(0.0, 0.006, dist) * (1.0 - smoothstep(0.006, 0.02, dist));
    col -= groove * 0.22;

    // Faint, non-repeating grain so the matte plastic isn't dead flat.
    float grain = fract(sin(dot(uv * resolution, vec2(12.9898, 78.233))) * 43758.5453);
    col += (grain - 0.5) * 0.025;

    // The outer edge of the frame falls into shadow.
    col *= 1.0 - smoothstep(0.10, 0.22, dist) * 0.7;

    return col;
}

void main()
{
    vec2 uv = curve(gl_TexCoord[0].xy);

    // Anything outside the curved tube is the plastic monitor bezel.
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    {
        gl_FragColor = vec4(bezel(uv), 1.0);
        return;
    }

    // Chromatic aberration: split the RGB channels slightly.
    float aberration = 0.0011;
    float r = texture2D(texture, vec2(uv.x + aberration, uv.y)).r;
    float g = texture2D(texture, uv).g;
    float b = texture2D(texture, vec2(uv.x - aberration, uv.y)).b;
    vec3 col = vec3(r, g, b);

    // Horizontal scanlines tied to the vertical resolution.
    float scan = sin(uv.y * resolution.y * 3.14159265);
    col *= 0.85 + 0.15 * scan;

    // Aperture-grille style vertical mask.
    float mask = 0.95 + 0.05 * sin(uv.x * resolution.x * 3.14159265);
    col *= mask;

    // A soft band that rolls up the screen like a refresh sweep.
    float roll = sin((uv.y + time * 0.1) * 35.0) * 0.01;
    col += roll;

    // Global phosphor flicker.
    col *= 1.0 - 0.04 * sin(time * 9.0);

    // Vignette darkening towards the corners.
    float vignette = uv.x * (1.0 - uv.x) * uv.y * (1.0 - uv.y);
    vignette = clamp(pow(32.0 * vignette, 0.28), 0.0, 1.0);
    col *= vignette;

    // Lift the black point a touch so the tube looks "warm", not pure black.
    col += vec3(0.02, 0.03, 0.05);

    gl_FragColor = vec4(col, 1.0);
}
