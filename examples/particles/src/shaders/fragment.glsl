#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vUv;

layout(location = 0) out vec4 outColor;
layout(location = 1) out float outAlpha;

layout(location = 3) uniform vec3 ro;
layout(location = 4) uniform float time;

const float pi = 3.1415926535;

float noise(in vec3 p) {
    vec3 i = floor(p);
    vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
    vec3 f = cos((p - i) * pi) * (-.5) + .5;

    a = mix(sin(cos(a) * a), sin(cos(1. + a) * (1. + a)), f.x);

    a.xy = mix(a.xz, a.yw, f.y);

    return mix(a.x, a.y, f.z);
}

#define OCTAVE_COUNT 5
float fbm(in vec3 p) {
    float n = 0.0;
    float amp = 0.5;

    for(int i = 0; i < OCTAVE_COUNT; i++) {
        n += noise(p) * amp;
        p *= 2.03;
        amp *= 0.5;
    }

    return n;
}

float opU(in float d1, in float d2, in float k) {
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

vec2 rot(in vec2 p, in float a) {
    vec2 res;

    res.x = p.x * cos(a) - p.y * sin(a);
    res.y = p.x * sin(a) + p.y * cos(a);

    return res;
}

vec2 rotf(in vec2 p, in float a) {
    vec2 res = p;

    res = rot(p, -pi / (a * 2.0));
    res = rot(p, floor(atan(res.x, res.y) / pi * a) * (pi/a));

    return res;
}

float sdFire(in vec3 p, in float r, in float amp) {
    float freq = 2.;
    float d = length(p) - r;

    return d + fbm(p * freq + amp * time) * amp;
}

float sdFrag(in vec3 p, in vec3 b) {
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

vec2 sdGrenade(in vec3 p) {
    float m = 1.;
    vec3 p1 = p;

    p1.xz = rotf(p1.xz, 6.0);
    p1.yz = rotf(p1.yz, 6.0);
    p1.z -= 0.6;

    float d = sdFrag(p1, vec3(.1, .1, .125));

    vec3 p2 = p;
    p2.y -= min(.3, max(-.3, p2.y));

    d = opU(d, length(p2) - 0.46, 0.6);

    vec3 p3 = p;
    p3.y -= 0.5;

    p3.y -= min(.4, max(-.4, p3.y));

    d = opU(d, length(p3) - 0.3, 0.1);

    vec3 p4 = p;
    p4.y -= 0.9;
    p4.y -= min(.8, max(-.8, p4.y));

    {
        float d1 = length(p4) - 0.18;
        m = d1 < d ? 2. : m;
        d = min(d1, d);
    }

    vec3 p5 = p;

    {
        p5.x -= 0.75;
        p5.y -= 0.025;

        p5.y -= min(.4, max(-.4, p5.y));

        vec3 p6 = p5;
        p6.y += .05;
        p6.x += .1;

        float d1 = max(length(p5) - 0.15, 0.23 - length(p6));

        vec3 p7 = p;

        p7.x = cos(pi / -4.) * p.x - sin(pi / -4.) * p.y;
        p7.y = sin(pi / -4.) * p.x + cos(pi / -4.) * p.y;
        p7.y -= 0.325;
        p7.x -= 0.825;

        p7.y -= min(.55, max(-.55, p7.y));

        vec3 p8 = p7;
        p8.y += 0.08;
        p8.x += 0.08;

        float d2 = max(length(p7) - 0.15, 0.23 - length(p8));

        d1 =  opU(d1, d2, 0.0125);

        m = d1 < d ? 2. : m;
        d = min(d1, d);
    }

    return vec2(d, m);
}

vec3 calcNormal(in vec3 p) {
    const float h = 0.1e-2;
    const vec2 k = vec2(1.0, -1.0);

    return normalize(
        k.xyy * sdGrenade(p + k.xyy * h).x +
        k.yyx * sdGrenade(p + k.yyx * h).x +
        k.yxy * sdGrenade(p + k.yxy * h).x +
        k.xxx * sdGrenade(p + k.xxx * h).x);
}

vec4 explosion(in vec3 ro, in vec3 stp, in float exptime) {
    vec4 sum = vec4(0.0);
    vec3 pos = ro;

    float density = mix(0., 0.2, min(1., step(exptime, 0.25) + 1.0 / (2. * exptime)));
    float amp = mix(0.4 * .05, 0.4 * 1.2, clamp(exptime * 4., 0., 1.));
    float r = mix(0.6 * 0.5, mix(-0.5, 0.6, clamp(exptime * 8, 0., 1.0)), 1. / max(1., (1.1 * exptime - .3)));

    for(int i = 0; i < 64; i++) {
        float d = sdFire(pos, r, amp);
        vec4 col = vec4(0.);

        if (d < 0.2)
            col = mix(vec4(vec3(2.), 1.), vec4(1., 1., 0., 1.), d / 0.2);
        else if (d >= 0.2 && d < 0.4)
            col = mix(vec4(1., 1., 0, 1.), vec4(.9, 0., 0., 0.5), (d - 0.2) / 0.2);
        else if (d >= 0.4 && d < 0.6)
            col = mix(vec4(1., 0., 0., 0.5), vec4(0., 0., 0., 0.4), (d - 0.4) / 0.2);
        if (d >= 0.6 && d < 0.8)
            col = mix(vec4(0.2, 0.2, 0.2, 0.4), vec4(.5, .5, .5, 0.2), (d - 0.6) / 0.2);
        // else if (d >= 0.8 && d < 1.0)
        //  col = mix(vec4(0.5, .5, 0.5, .2), vec4(0, 0, 0, 0), (d - 0.8) / 0.2);

        col.a *= density;

        col.rgb *= col.a;

        sum = sum + col * (1.0 - sum.a);

        pos += stp;
    }

    return sum;
}

vec4 grenade(in vec3 ro, in vec3 rd) {
    float d = 0.1e-2;
    float m = 0.;

    for (int i = 0; i < 64 && d < 10.; i++) {
        vec3 pos = ro + rd  * d;
        vec2 res = sdGrenade(pos);

        if (abs(res.x) < 0.1e-2) break;

        d += res.x;
        m = res.y;
    }

    vec4 col = vec4(0., 0., 0., 0.1);

    if (d < 10.) {
        vec3 pos = ro + rd * d;
        vec3 nor = calcNormal(pos);

        col.rgb = max(dot(nor, -rd), 0.) * vec3(.0, 0.5, .0) + vec3(0.1, .2, 0.1);
        col.a = 1.;
    }

    return col;
}

vec4 raymarch(in vec3 ro, in vec3 rd, in float stepSize) {
    float delay = 40.;
    return time < delay ? grenade(ro, rd) : explosion(ro, rd * stepSize, time - delay);
}

float weight(float z, float a) {
    return clamp(pow(min(1., a * 10.0) + 0.01, 3.0)  * 1e8 * pow(1. - z * 0.9, 3.0), 1e-2, 3e3);
}

void main() {
    vec2 uv = vUv * 2. - 0.5; // scale

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 ww = -ro;
    vec3 uu = normalize(cross(ww, up));
    vec3 vv = normalize(cross(uu, ww));

    vec3 p = vec3(-1.0 + 2.0 * uv, 1.0);

    vec3 rd = normalize(p.x * uu + p.y * vv + p.z * ww);

    vec4 col = vec4(0.0);

    col.rgb *= col.a;

    float delta = 0.04;

    vec3 start = ro + rd * 5.; // make it closer to shape

    col = raymarch(start, rd, delta);

    float w = weight(gl_FragCoord.z, col.a);

    outColor = vec4(col.rgb * w, col.a);
    outAlpha = col.a * w;
}
