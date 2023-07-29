#version 460 core
out vec4 FragColor;

layout(location = 0) uniform vec2 uScreenResolution;
layout(location = 1) uniform float uTime;
layout(location = 2) uniform vec3 ro;
layout(location = 3) uniform mat4 uViewMatrix;
layout(location = 4) uniform vec3 LIGHT_COLOR;
layout(location = 5) uniform int fullSquare;
layout(location = 6 ) uniform float propagation;
layout(location = 7) uniform float scene2laser;
layout(location = 8) uniform float sceneId;
layout(location = 9) uniform vec3 rotationObj;
layout(location = 10) uniform float inflation;

layout(location = 15) uniform vec3 lpIntro;
layout(location = 16) uniform vec3 lpIntro2;
layout(location = 17) uniform vec3 FLASH_COLOR;


const int MAX_MARCHING_STEPS = 255;
const float MIN_DISTANCE = 0.0;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.001;
const vec3 MaterialAmbiantColor = vec3(0.1,0.1,0.1);

vec3 LIGHT_POSITION = vec3(0.0, 0.8, -5.0);
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
    return p.y;// + sin(p.x * 10) * 0.1;
}

float sdCylinder(vec3 p, float r, float height) {
	float d = length(p.xy) - r;
	d = max(d, abs(p.z) - height);
	return d;
}


float sdSegment(vec3 p, vec3 a, vec3 b)
{
    vec3 ab = b - a;
    float t = clamp(dot(p - a, ab) / dot(ab, ab), 0.0, 1.0);
    return length((ab*t + a) - p);
}

float mapLight(vec3 p)
{
   
   if (sceneId < 1.0)
   {
      float plIntro = sdSphere(p - lpIntro, 0.);
        float plIntro2 = sdSphere(p - lpIntro2, 0.);
        return min(plIntro, plIntro2);
   }
   else
   {
        float l = 50.0;
        p = rotateX(p,rotationObj.x);
        p = rotateY(p, rotationObj.y);
        p = rotateZ(p,rotationObj.z);

        float ray1 = sdSegment(p, vec3(-1,1,1)*l, vec3(0));
        float ray2 = sdSegment(p, vec3(1,1,1)*l, vec3(0));
        float ray3 = sdSegment(p, vec3(1,1,-1)*l, vec3(0));
        float ray4 = sdSegment(p, vec3(-1,1,-1)*l, vec3(0));
    
        float result = 1000.0;
        int isl = int(scene2laser);
        if ((isl % 2)  > 0) result = min(result, ray1);
        if ((isl % 4)  > 1) result = min(result, ray2);
        if ((isl % 8)  > 3) result = min(result, ray3);
        if ((isl % 16) > 7) result = min(result, ray4);

        return result;
        
   }
   
}

float map(vec3 p)
{
    //float sphere = sdSphere(p, 1.);
    if(sceneId < 1.0)
    {
    float cyl = -sdCylinder(p, 2., 300.0); //chgmt taille couloir
    float floor = p.y;
    return min(floor, cyl);
    }
    else
    {
    
        p = rotateX(p,rotationObj.x);
        p = rotateY(p, rotationObj.y);
        p = rotateZ(p,rotationObj.z);
    float cube = sdBox(p, vec3(1.0))-inflation;
    return cube;
    }
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

float rayMarchLight(vec3 ro, vec3 rd, float start, float end, out vec3 outP)
{
    float depth = start;
    float dMin = 1000000.0;

    for(int i = 0; i < MAX_MARCHING_STEPS; i++)
    {
        vec3 p = ro + depth * rd;
        float d = mapLight(p);
        depth += d;
        
        if (d < dMin)
        {
            dMin = d;
            outP = p;
        }
        //dMin = min(dMin, d);

        if(d < EPSILON || depth > end)
            break;
    }
    return dMin;
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

vec3 shadeWithLight(vec3 ro, vec3 p, vec3 n, vec3 lp, vec3 lc, float shininess)
{
    vec3 L = normalize(lp - p);
    vec3 V = normalize(ro - p);
    vec3 H = normalize(L + V);

    float NoH = clamp(dot(n, H), 0., 1.);
    float NoL = clamp(dot(n, L), 0., 1.);

    float specularIntensity = pow(clamp(NoH, 0., 1.), shininess);

    vec3 MaterialDifuseColor = NoL * lc;

    float shadow = softshadow(p + n * EPSILON * 10.0, L, 0.0, 10.0, 10.0);
    return (MaterialDifuseColor + vec3(specularIntensity)) * shadow;
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
    float att = 0.0;

    
    vec3 pLight = vec3(0.0);
    float distToLightObject = rayMarchLight(ro, rd, MIN_DISTANCE, d, pLight);
    att = 1.0 / (1.0 + distToLightObject * distToLightObject * 10.0);

    if(d > MAX_DISTANCE)
    {
        MaterialDifuseColor = vec3(0.0f);
    }else
    {
        vec3 p = ro + d * rd;
        vec3 n = calcNormal(p);
       col += shadeWithLight(ro, p, n, LIGHT_POSITION, LIGHT_COLOR, 40.);

        vec3 LSelfIllum = normalize(pLight - p);
        float selfIllumIntensity = clamp(dot(n, LSelfIllum), 0., 1.);

        vec3 V = normalize(ro - p);
        vec3 H = normalize(LSelfIllum + V);

        float NoH = clamp(dot(n, H), 0., 1.);
        float specularIntensity = pow(NoH, 8.0);
        
        float dToSelfIllum = mapLight(p);
        float attSelfIllum = 1.0 / (0.2 + dToSelfIllum * dToSelfIllum * propagation);
        col += (FLASH_COLOR * selfIllumIntensity + vec3(specularIntensity)) * attSelfIllum;

    } 
    col = mix(col, FLASH_COLOR, att * clamp(d * d * 0.05, 0.0, 1.0));
    
    fragColor = vec4(col, 1.0); 
}

void main()
{
    mainImage(FragColor, gl_FragCoord.xy);
}

