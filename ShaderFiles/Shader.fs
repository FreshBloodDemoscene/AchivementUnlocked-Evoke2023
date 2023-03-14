#version 460 core
out vec4 FragColor;

layout(location = 0) uniform vec2 uScreenResolution;
layout(location = 1) uniform float uTime;
layout(location = 2) uniform vec3 ro;
layout(location = 3) uniform mat4 uViewMatrix;
layout(location = 4) uniform vec3 LIGHT_COLOR;
layout(location = 5) uniform int fullSquare;


const int MAX_MARCHING_STEPS = 255;
const float MIN_DISTANCE = 0.0;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.001;
const vec3 MaterialAmbiantColor = vec3(0.1,0.1,0.1);

vec3 LIGHT_POSITION;
float box;

//vec2 dmMod2(vec2 p, vec2 s) 
//{
//    vec2 h = s * 0.5;
//    return mod(p + h, s) - h;
//}

vec2 pMod2(inout vec2 p, vec2 size)
{
	vec2 c = floor((p + size*0.5)/size);
	p = mod(p + size*0.5,size) - size*0.5;
	return c;
}

vec2 rotate(vec2 p, float a)
{
    float c = cos(a);
    float s = sin(a);
    return vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

vec3 rotateX(vec3 p, float a) 
{
    p.yz = rotate(p.yz, a);
    return p;
}

vec3 rotateY(vec3 p, float a) 
{
    p.xz = rotate(p.xz, a);
    return p;
}

vec3 rotateZ(vec3 p, float a) 
{
    p.xy = rotate(p.xy, a);
    return p;
}

float sdSphere(vec3 p, float r)
{
    vec3 offset = vec3(0,0,-2);
    return length(p - offset) - r;
}

float sdBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdFloor(vec3 p)
{
    return p.y + sin(p.x * 10) * 0.1;
}

float map(vec3 p)
{
    float sphere = sdSphere(p, 1.);
     
    vec3 p2 = p;
    
    vec2 cellSize = vec2(2.0);
    vec2 cellId = pMod2(p2.xz, cellSize);

    vec3 pp = rotateX(rotateY(rotateZ(p2-vec3(0.0, 2.0 + fract(0.5 * cellId.x), 0.0), uTime), uTime), uTime);

    if(fullSquare == 0)
    {
        box = sdBox(p, vec3(0.5));
    }
    else
    {
        box = sdBox(pp, vec3(0.5));
    }
    
    LIGHT_POSITION = vec3(cellSize * vec2(4., 1.), 5.0).xzy;

    
    
    float sol = sdFloor(p);

    float trouSphere = max(sol, -sphere);
    return min(box, trouSphere);
}

vec3 calcNormal(vec3 p)
{
    vec2 e = vec2(EPSILON, 0.);
    return normalize(vec3(
        map(p + e.xyy) - map(p - e.xyy),
        map(p + e.yxy) - map(p - e.yxy),
        map(p + e.yyx) - map(p - e.yyx)
    ));
}

float rayMarch(vec3 ro, vec3 rd, float start, float end)
{
    float depth = start;

    for(int i = 0; i < MAX_MARCHING_STEPS; i++)
    {
        vec3 p = ro + depth * rd;
        float d = map(p);
        depth += d;

        if(d < EPSILON || depth > end)break;
    }
    return depth;
}

float softshadow( in vec3 ro, in vec3 rd, float mint, float maxt, float k )
{
    float res = 1.0;
    float t = mint;
    for(int i = 0; i < 256 && t < maxt; i++ )
    {
        float h = map(ro + rd*t);
        if( h<0.001 )
            return 0.0;
        res = min( res, k*h/t );
        t += h;
    }
    return res;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / uScreenResolution;
    uv = uv * 2. - 1.;
    uv.x *= uScreenResolution.x / uScreenResolution.y;

    vec3 MaterialDifuseColor = vec3(0);
    float specularIntensity = 0.;

     vec3 col = vec3(0.0);

    float LightPower = 60.;
    float shininess = 40.;

    vec3 rd = normalize(vec3(uv, -1.));

    vec3 ro = (uViewMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    rd = (uViewMatrix * vec4(rd, 0.0)).xyz;

    float d = rayMarch(ro, rd, MIN_DISTANCE, MAX_DISTANCE);
    if(d > MAX_DISTANCE)
    {
        MaterialDifuseColor = vec3(0.0f);
    }else
    {
        vec3 p = ro + d * rd;
        vec3 n = calcNormal(p);
        vec3 L = normalize(LIGHT_POSITION - p);

        vec3 V = normalize(ro - p);
        vec3 H = normalize(L + V);

        float NoH = clamp(dot(n, H), 0., 1.);
        float NoL = clamp(dot(n, L), 0., 1.);

        specularIntensity = pow(clamp(NoH, 0., 1.), shininess);

        MaterialDifuseColor = NoL * LIGHT_COLOR;

        float shadow = softshadow(p + n * EPSILON * 10.0, L, 0.0, 10.0, 10.0);
        col = (MaterialDifuseColor + vec3(specularIntensity)) * shadow;

    } 
    
    fragColor = vec4(col, 1.0);
}

void main()
{
    mainImage(FragColor, gl_FragCoord.xy);
}

