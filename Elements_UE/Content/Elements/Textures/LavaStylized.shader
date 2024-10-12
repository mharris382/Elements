struct Functions {
	float Time;
	Texture2D texture_1;
	SamplerState texture_1Sampler;
	Texture2D texture_2;
	SamplerState texture_2Sampler;
	Texture2D texture_3;
	SamplerState texture_3Sampler;
	Texture2D texture_4;
	SamplerState texture_4Sampler;
	
#define hlsl_atan(x,y) atan2(x, y)
#define mod(x,y) ((x)-(y)*floor((x)/(y)))
inline float4 textureLod(sampler tex, float2 uv, float lod) {
    return tex2D(tex, uv);
}
inline float2 tofloat2(float x) {
    return float2(x, x);
}
inline float2 tofloat2(float x, float y) {
    return float2(x, y);
}
inline float3 tofloat3(float x) {
    return float3(x, x, x);
}
inline float3 tofloat3(float x, float y, float z) {
    return float3(x, y, z);
}
inline float3 tofloat3(float2 xy, float z) {
    return float3(xy.x, xy.y, z);
}
inline float3 tofloat3(float x, float2 yz) {
    return float3(x, yz.x, yz.y);
}
inline float4 tofloat4(float x, float y, float z, float w) {
    return float4(x, y, z, w);
}
inline float4 tofloat4(float x) {
    return float4(x, x, x, x);
}
inline float4 tofloat4(float x, float3 yzw) {
    return float4(x, yzw.x, yzw.y, yzw.z);
}
inline float4 tofloat4(float2 xy, float2 zw) {
    return float4(xy.x, xy.y, zw.x, zw.y);
}
inline float4 tofloat4(float3 xyz, float w) {
    return float4(xyz.x, xyz.y, xyz.z, w);
}
inline float4 tofloat4(float2 xy, float z, float w) {
    return float4(xy.x, xy.y, z, w);
}
inline float2x2 tofloat2x2(float2 v1, float2 v2) {
    return float2x2(v1.x, v1.y, v2.x, v2.y);
}
#define textureLod(t, uv, lod) t.SampleLevel(t##Sampler, uv, lod)
float rand(float2 x) {
    return frac(cos(mod(dot(x, tofloat2(13.9898, 8.141)), 3.14)) * 43758.5453);
}
float2 rand2(float2 x) {
    return frac(cos(mod(tofloat2(dot(x, tofloat2(13.9898, 8.141)),
						      dot(x, tofloat2(3.4562, 17.398))), tofloat2(3.14))) * 43758.5453);
}
float3 rand3(float2 x) {
    return frac(cos(mod(tofloat3(dot(x, tofloat2(13.9898, 8.141)),
							  dot(x, tofloat2(3.4562, 17.398)),
                              dot(x, tofloat2(13.254, 5.867))), tofloat3(3.14))) * 43758.5453);
}
float param_rnd(float minimum, float maximum, float seed) {
	return minimum+(maximum-minimum)*rand(tofloat2(seed));
}
float3 rgb2hsv(float3 c) {
	float4 K = tofloat4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	float4 p = c.g < c.b ? tofloat4(c.bg, K.wz) : tofloat4(c.gb, K.xy);
	float4 q = c.r < p.x ? tofloat4(p.xyw, c.r) : tofloat4(c.r, p.yzx);
	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return tofloat3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
float3 hsv2rgb(float3 c) {
	float4 K = tofloat4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	float3 p = abs(frac(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * lerp(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
float value_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float p00 = rand(mod(o, size));
	float p01 = rand(mod(o + tofloat2(0.0, 1.0), size));
	float p10 = rand(mod(o + tofloat2(1.0, 0.0), size));
	float p11 = rand(mod(o + tofloat2(1.0, 1.0), size));
	p00 = sin(p00 * 6.28318530718 + offset * 6.28318530718) / 2.0 + 0.5;
	p01 = sin(p01 * 6.28318530718 + offset * 6.28318530718) / 2.0 + 0.5;
	p10 = sin(p10 * 6.28318530718 + offset * 6.28318530718) / 2.0 + 0.5;
	p11 = sin(p11 * 6.28318530718 + offset * 6.28318530718) / 2.0 + 0.5;
	float2 t =  f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
	return lerp(lerp(p00, p10, t.x), lerp(p01, p11, t.x), t.y);
}
float fbm_2d_value(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = value_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float perlin_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float a00 = rand(mod(o, size)) * 6.28318530718 + offset * 6.28318530718;
	float a01 = rand(mod(o + tofloat2(0.0, 1.0), size)) * 6.28318530718 + offset * 6.28318530718;
	float a10 = rand(mod(o + tofloat2(1.0, 0.0), size)) * 6.28318530718 + offset * 6.28318530718;
	float a11 = rand(mod(o + tofloat2(1.0, 1.0), size)) * 6.28318530718 + offset * 6.28318530718;
	float2 v00 = tofloat2(cos(a00), sin(a00));
	float2 v01 = tofloat2(cos(a01), sin(a01));
	float2 v10 = tofloat2(cos(a10), sin(a10));
	float2 v11 = tofloat2(cos(a11), sin(a11));
	float p00 = dot(v00, f);
	float p01 = dot(v01, f - tofloat2(0.0, 1.0));
	float p10 = dot(v10, f - tofloat2(1.0, 0.0));
	float p11 = dot(v11, f - tofloat2(1.0, 1.0));
	float2 t =  f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
	return 0.5 + lerp(lerp(p00, p10, t.x), lerp(p01, p11, t.x), t.y);
}
float fbm_2d_perlin(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = perlin_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float perlinabs_noise_2d(float2 coord, float2 size, float offset, float seed) {
	return abs(2.0*perlin_noise_2d(coord, size, offset, seed)-1.0);
}
float fbm_2d_perlinabs(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = perlinabs_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float fbm_2d_mod289(float x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}
float fbm_2d_permute(float x) {
	return fbm_2d_mod289(((x * 34.0) + 1.0) * x);
}
float2 fbm_2d_rgrad2(float2 p, float rot, float seed) {
	float u = fbm_2d_permute(fbm_2d_permute(p.x) + p.y) * 0.0243902439 + rot; // Rotate by shift
	u = frac(u) * 6.28318530718; // 2*pi
	return tofloat2(cos(u), sin(u));
}
float simplex_noise_2d(float2 coord, float2 size, float offset, float seed) {
	coord *= 2.0; // needed for it to tile
	coord += rand2(tofloat2(seed, 1.0-seed)) + size;
	size *= 2.0; // needed for it to tile
	coord.y += 0.001;
	float2 uv = tofloat2(coord.x + coord.y*0.5, coord.y);
	float2 i0 = floor(uv);
	float2 f0 = frac(uv);
	float2 i1 = (f0.x > f0.y) ? tofloat2(1.0, 0.0) : tofloat2(0.0, 1.0);
	float2 p0 = tofloat2(i0.x - i0.y * 0.5, i0.y);
	float2 p1 = tofloat2(p0.x + i1.x - i1.y * 0.5, p0.y + i1.y);
	float2 p2 = tofloat2(p0.x + 0.5, p0.y + 1.0);
	i1 = i0 + i1;
	float2 i2 = i0 + tofloat2(1.0, 1.0);
	float2 d0 = coord - p0;
	float2 d1 = coord - p1;
	float2 d2 = coord - p2;
	float3 xw = mod(tofloat3(p0.x, p1.x, p2.x), size.x);
	float3 yw = mod(tofloat3(p0.y, p1.y, p2.y), size.y);
	float3 iuw = xw + 0.5 * yw;
	float3 ivw = yw;
	float2 g0 = fbm_2d_rgrad2(tofloat2(iuw.x, ivw.x), offset, seed);
	float2 g1 = fbm_2d_rgrad2(tofloat2(iuw.y, ivw.y), offset, seed);
	float2 g2 = fbm_2d_rgrad2(tofloat2(iuw.z, ivw.z), offset, seed);
	float3 w = tofloat3(dot(g0, d0), dot(g1, d1), dot(g2, d2));
	float3 t = 0.8 - tofloat3(dot(d0, d0), dot(d1, d1), dot(d2, d2));
	t = max(t, tofloat3(0.0));
	float3 t2 = t * t;
	float3 t4 = t2 * t2;
	float n = dot(t4, w);
	return 0.5 + 5.5 * n;
}
float fbm_2d_simplex(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = simplex_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float cellular_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float min_dist = 2.0;
	for(float x = -1.0; x <= 1.0; x++) {
		for(float y = -1.0; y <= 1.0; y++) {
			float2 neighbor = tofloat2(float(x),float(y));
			float2 node = rand2(mod(o + tofloat2(x, y), size)) + tofloat2(x, y);
			node =  0.5 + 0.25 * sin(offset * 6.28318530718 + 6.28318530718 * node);
			float2 diff = neighbor + node - f;
			float dist = length(diff);
			min_dist = min(min_dist, dist);
		}
	}
	return min_dist;
}
float fbm_2d_cellular(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = cellular_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float cellular2_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float min_dist1 = 2.0;
	float min_dist2 = 2.0;
	for(float x = -1.0; x <= 1.0; x++) {
		for(float y = -1.0; y <= 1.0; y++) {
			float2 neighbor = tofloat2(float(x),float(y));
			float2 node = rand2(mod(o + tofloat2(x, y), size)) + tofloat2(x, y);
			node = 0.5 + 0.25 * sin(offset * 6.28318530718 + 6.28318530718*node);
			float2 diff = neighbor + node - f;
			float dist = length(diff);
			if (min_dist1 > dist) {
				min_dist2 = min_dist1;
				min_dist1 = dist;
			} else if (min_dist2 > dist) {
				min_dist2 = dist;
			}
		}
	}
	return min_dist2-min_dist1;
}
float fbm_2d_cellular2(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = cellular2_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float cellular3_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float min_dist = 2.0;
	for(float x = -1.0; x <= 1.0; x++) {
		for(float y = -1.0; y <= 1.0; y++) {
			float2 neighbor = tofloat2(float(x),float(y));
			float2 node = rand2(mod(o + tofloat2(x, y), size)) + tofloat2(x, y);
			node = 0.5 + 0.25 * sin(offset * 6.28318530718 + 6.28318530718*node);
			float2 diff = neighbor + node - f;
			float dist = abs((diff).x) + abs((diff).y);
			min_dist = min(min_dist, dist);
		}
	}
	return min_dist;
}
float fbm_2d_cellular3(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = cellular3_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float cellular4_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float min_dist1 = 2.0;
	float min_dist2 = 2.0;
	for(float x = -1.0; x <= 1.0; x++) {
		for(float y = -1.0; y <= 1.0; y++) {
			float2 neighbor = tofloat2(float(x),float(y));
			float2 node = rand2(mod(o + tofloat2(x, y), size)) + tofloat2(x, y);
			node = 0.5 + 0.25 * sin(offset * 6.28318530718 + 6.28318530718*node);
			float2 diff = neighbor + node - f;
			float dist = abs((diff).x) + abs((diff).y);
			if (min_dist1 > dist) {
				min_dist2 = min_dist1;
				min_dist1 = dist;
			} else if (min_dist2 > dist) {
				min_dist2 = dist;
			}
		}
	}
	return min_dist2-min_dist1;
}
float fbm_2d_cellular4(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = cellular4_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float cellular5_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float min_dist = 2.0;
	for(float x = -1.0; x <= 1.0; x++) {
		for(float y = -1.0; y <= 1.0; y++) {
			float2 neighbor = tofloat2(float(x),float(y));
			float2 node = rand2(mod(o + tofloat2(x, y), size)) + tofloat2(x, y);
			node = 0.5 + 0.5 * sin(offset * 6.28318530718 + 6.28318530718*node);
			float2 diff = neighbor + node - f;
			float dist = max(abs((diff).x), abs((diff).y));
			min_dist = min(min_dist, dist);
		}
	}
	return min_dist;
}
float fbm_2d_cellular5(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = cellular5_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
float cellular6_noise_2d(float2 coord, float2 size, float offset, float seed) {
	float2 o = floor(coord)+rand2(tofloat2(seed, 1.0-seed))+size;
	float2 f = frac(coord);
	float min_dist1 = 2.0;
	float min_dist2 = 2.0;
	for(float x = -1.0; x <= 1.0; x++) {
		for(float y = -1.0; y <= 1.0; y++) {
			float2 neighbor = tofloat2(float(x),float(y));
			float2 node = rand2(mod(o + tofloat2(x, y), size)) + tofloat2(x, y);
			node = 0.5 + 0.25 * sin(offset * 6.28318530718 + 6.28318530718*node);
			float2 diff = neighbor + node - f;
			float dist = max(abs((diff).x), abs((diff).y));
			if (min_dist1 > dist) {
				min_dist2 = min_dist1;
				min_dist1 = dist;
			} else if (min_dist2 > dist) {
				min_dist2 = dist;
			}
		}
	}
	return min_dist2-min_dist1;
}
float fbm_2d_cellular6(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = cellular6_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
// MIT License Inigo Quilez - https://www.shadertoy.com/view/Xd23Dh
float voronoise_noise_2d( float2 coord, float2 size, float offset, float seed) {
	float2 i = floor(coord) + rand2(tofloat2(seed, 1.0-seed)) + size;
	float2 f = frac(coord);
	
	float2 a = tofloat2(0.0);
	
	for( int y=-2; y<=2; y++ ) {
		for( int x=-2; x<=2; x++ ) {
			float2  g = tofloat2( float(x), float(y) );
			float3  o = rand3( mod(i + g, size) + tofloat2(seed) );
			o.xy += 0.25 * sin(offset * 6.28318530718 + 6.28318530718*o.xy);
			float2  d = g - f + o.xy;
			float w = pow( 1.0-smoothstep(0.0, 1.414, length(d)), 1.0 );
			a += tofloat2(o.z*w,w);
		}
	}
	
	return a.x/a.y;
}
float fbm_2d_voronoise(float2 coord, float2 size, int folds, int octaves, float persistence, float offset, float seed) {
	float normalize_factor = 0.0;
	float value = 0.0;
	float scale = 1.0;
	for (int i = 0; i < octaves; i++) {
		float noise = voronoise_noise_2d(coord*size, size, offset, seed);
		for (int f = 0; f < folds; ++f) {
			noise = abs(2.0*noise-1.0);
		}
		value += noise * scale;
		normalize_factor += scale;
		size *= 2.0;
		scale *= persistence;
	}
	return value / normalize_factor;
}
// uniform sampler  texture_1;
float2 transform2_clamp(float2 uv) {
	return clamp(uv, tofloat2(0.0), tofloat2(1.0));
}
float2 transform2(float2 uv, float2 translate, float rotate, float2 scale) {
 	float2 rv;
	uv -= translate;
	uv -= tofloat2(0.5);
	rv.x = cos(rotate)*uv.x + sin(rotate)*uv.y;
	rv.y = -sin(rotate)*uv.x + cos(rotate)*uv.y;
	rv /= scale;
	rv += tofloat2(0.5);
	return rv;	
}
// uniform sampler  texture_2;
float3 blend_normal(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*c1 + (1.0-opacity)*c2;
}
float3 blend_dissolve(float2 uv, float3 c1, float3 c2, float opacity) {
	if (rand(uv) < opacity) {
		return c1;
	} else {
		return c2;
	}
}
float3 blend_multiply(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*c1*c2 + (1.0-opacity)*c2;
}
float3 blend_screen(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*(1.0-(1.0-c1)*(1.0-c2)) + (1.0-opacity)*c2;
}
float blend_overlay_f(float c1, float c2) {
	return (c1 < 0.5) ? (2.0*c1*c2) : (1.0-2.0*(1.0-c1)*(1.0-c2));
}
float3 blend_overlay(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*tofloat3(blend_overlay_f(c1.x, c2.x), blend_overlay_f(c1.y, c2.y), blend_overlay_f(c1.z, c2.z)) + (1.0-opacity)*c2;
}
float3 blend_hard_light(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*0.5*(c1*c2+blend_overlay(uv, c1, c2, 1.0)) + (1.0-opacity)*c2;
}
float blend_soft_light_f(float c1, float c2) {
	return (c2 < 0.5) ? (2.0*c1*c2+c1*c1*(1.0-2.0*c2)) : 2.0*c1*(1.0-c2)+sqrt(c1)*(2.0*c2-1.0);
}
float3 blend_soft_light(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*tofloat3(blend_soft_light_f(c1.x, c2.x), blend_soft_light_f(c1.y, c2.y), blend_soft_light_f(c1.z, c2.z)) + (1.0-opacity)*c2;
}
float blend_burn_f(float c1, float c2) {
	return (c1==0.0)?c1:max((1.0-((1.0-c2)/c1)),0.0);
}
float3 blend_burn(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*tofloat3(blend_burn_f(c1.x, c2.x), blend_burn_f(c1.y, c2.y), blend_burn_f(c1.z, c2.z)) + (1.0-opacity)*c2;
}
float blend_dodge_f(float c1, float c2) {
	return (c1==1.0)?c1:min(c2/(1.0-c1),1.0);
}
float3 blend_dodge(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*tofloat3(blend_dodge_f(c1.x, c2.x), blend_dodge_f(c1.y, c2.y), blend_dodge_f(c1.z, c2.z)) + (1.0-opacity)*c2;
}
float3 blend_lighten(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*max(c1, c2) + (1.0-opacity)*c2;
}
float3 blend_darken(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*min(c1, c2) + (1.0-opacity)*c2;
}
float3 blend_difference(float2 uv, float3 c1, float3 c2, float opacity) {
	return opacity*clamp(c2-c1, tofloat3(0.0), tofloat3(1.0)) + (1.0-opacity)*c2;
}
float3 blend_additive(float2 uv, float3 c1, float3 c2, float oppacity) {
	return c2 + c1 * oppacity;
}
float3 blend_addsub(float2 uv, float3 c1, float3 c2, float oppacity) {
	return c2 + (c1 - .5) * 2.0 * oppacity;
}
float4 adjust_levels(float4 input, float4 in_min, float4 in_mid, float4 in_max, float4 out_min, float4 out_max) {
	input = clamp((input-in_min)/(in_max-in_min), 0.0, 1.0);
	in_mid = (in_mid-in_min)/(in_max-in_min);
	float4 dark = step(in_mid, input);
	input = 0.5*lerp(input/(in_mid), 1.0+(input-in_mid)/(1.0-in_mid), dark);
	return out_min+input*(out_max-out_min);
}
float3 process_normal_default(float3 v, float multiplier) {
	return 0.5*normalize(v.xyz*multiplier+tofloat3(0.0, 0.0, -1.0))+tofloat3(0.5);
}
float3 process_normal_opengl(float3 v, float multiplier) {
	return 0.5*normalize(v.xyz*multiplier+tofloat3(0.0, 0.0, 1.0))+tofloat3(0.5);
}
float3 process_normal_directx(float3 v, float multiplier) {
	return 0.5*normalize(v.xyz*tofloat3(1.0, -1.0, 1.0)*multiplier+tofloat3(0.0, 0.0, 1.0))+tofloat3(0.5);
}
float o8090_input_depth_tex(float2 uv, float _seed_variation_) {
float o8586_0_1_f = fbm_2d_value((uv), tofloat2(1.000000000, 1.000000000), int(0.000000000), int(3.000000000), 0.500000000, 0.000000000, (-57371.000000000+frac(_seed_variation_)));
float4 o8124_0 = textureLod(texture_1, (frac(transform2((uv), tofloat2(0.250000000*(2.0*o8586_0_1_f-1.0), (Time*.05)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), 0.0);
float4 o8123_0_1_rgba = tofloat4(tofloat3(1.0)-o8124_0.rgb, o8124_0.a);
float4 o8584_0_1_rgba = o8123_0_1_rgba;
return (dot((o8584_0_1_rgba).rgb, tofloat3(1.0))/3.0);
}
float4 o8635_gradient_gradient_fct(float x) {
  if (x < 0.113748000) {
    return tofloat4(0.945312023, 0.825215995, 0.276946992, 1.000000000);
  } else if (x < 0.233667000) {
    return lerp(tofloat4(0.945312023, 0.825215995, 0.276946992, 1.000000000), tofloat4(0.820312023, 0.288841993, 0.201874003, 1.000000000), ((x-0.113748000)/(0.233667000-0.113748000)));
  } else if (x < 0.391878000) {
    return lerp(tofloat4(0.820312023, 0.288841993, 0.201874003, 1.000000000), tofloat4(0.222655997, 0.152294993, 0.127853006, 1.000000000), ((x-0.233667000)/(0.391878000-0.233667000)));
  }
  return tofloat4(0.222655997, 0.152294993, 0.127853006, 1.000000000);
}
float o8100_input_in(float2 uv, float _seed_variation_) {
float4 o8124_0 = textureLod(texture_1, uv, 0.0);
return (dot((o8124_0).rgb, tofloat3(1.0))/3.0);
}
float3 o8100_fct(float2 uv, float _seed_variation_) {
	float3 e = tofloat3(1.0/1024.000000000, -1.0/1024.000000000, 0);
	float2 rv = tofloat2(1.0, -1.0)*o8100_input_in(uv+e.xy, _seed_variation_);
	rv += tofloat2(-1.0, 1.0)*o8100_input_in(uv-e.xy, _seed_variation_);
	rv += tofloat2(1.0, 1.0)*o8100_input_in(uv+e.xx, _seed_variation_);
	rv += tofloat2(-1.0, -1.0)*o8100_input_in(uv-e.xx, _seed_variation_);
	rv += tofloat2(2.0, 0.0)*o8100_input_in(uv+e.xz, _seed_variation_);
	rv += tofloat2(-2.0, 0.0)*o8100_input_in(uv-e.xz, _seed_variation_);
	rv += tofloat2(0.0, 2.0)*o8100_input_in(uv+e.zx, _seed_variation_);
	rv += tofloat2(0.0, -2.0)*o8100_input_in(uv-e.zx, _seed_variation_);
	return tofloat3(rv, 0.0);
}
	fixed4 generated_shader(float2 uv, float _time,
							Texture2D _texture_1, SamplerState _texture_1Sampler,
							Texture2D _texture_2, SamplerState _texture_2Sampler,
							Texture2D _texture_3, SamplerState _texture_3Sampler,
							Texture2D _texture_4, SamplerState _texture_4Sampler,
							out float metallic, out float roughness, out float3 normal, out float3 emission) {
	  	float _seed_variation_ = 0.0;
		Time = _time;
		texture_1 = _texture_1;
		texture_1Sampler = _texture_1Sampler;
		texture_2 = _texture_2;
		texture_2Sampler = _texture_2Sampler;
		texture_3 = _texture_3;
		texture_3Sampler = _texture_3Sampler;
		texture_4 = _texture_4;
		texture_4Sampler = _texture_4Sampler;
		
float o8586_0_1_f = fbm_2d_value((uv), tofloat2(1.000000000, 1.000000000), int(0.000000000), int(3.000000000), 0.500000000, 0.000000000, (-57371.000000000+frac(_seed_variation_)));
float4 o8662_0 = textureLod(texture_2, (frac(transform2((uv), tofloat2(0.250000000*(2.0*o8586_0_1_f-1.0), (Time*.05)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), 0.0);
float4 o8124_0 = textureLod(texture_1, (frac(transform2((uv), tofloat2(0.250000000*(2.0*o8586_0_1_f-1.0), (Time*.05)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), 0.0);
float4 o8635_0_1_rgba = o8635_gradient_gradient_fct((dot((o8124_0).rgb, tofloat3(1.0))/3.0));
float4 o8092_0_s1 = o8662_0;
float4 o8092_0_s2 = o8635_0_1_rgba;
float o8092_0_a = 0.300000000*1.0;
float4 o8092_0_2_rgba = tofloat4(blend_overlay((frac(transform2((uv), tofloat2(0.250000000*(2.0*o8586_0_1_f-1.0), (Time*.05)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), o8092_0_s1.rgb, o8092_0_s2.rgb, o8092_0_a*o8092_0_s1.a), min(1.0, o8092_0_s2.a+o8092_0_a*o8092_0_s1.a));
float4 o8581_0_1_rgba = o8092_0_2_rgba;
float o8709_0_1_f = fbm_2d_perlin((frac(transform2((uv), tofloat2((Time*.05)*(2.0*1.0-1.0), (Time*.01)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), tofloat2(3.000000000, 3.000000000), int(0.000000000), int(2.000000000), 0.500000000, 0.000000000, (30434.000000000+frac(_seed_variation_)));
float4 o8711_0_1_rgba = tofloat4(tofloat3(o8709_0_1_f), 1.0);
float4 o8712_0_1_rgba = tofloat4(clamp(o8711_0_1_rgba.rgb*2.000000000+tofloat3(0.000000000)+0.5-2.000000000*0.5, tofloat3(0.0), tofloat3(1.0)), o8711_0_1_rgba.a);
float4 o8124_1 = textureLod(texture_1, (frac(transform2((uv), tofloat2(0.250000000*(2.0*o8586_0_1_f-1.0), (Time*.05)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), 0.0);
float4 o8635_0_3_rgba = o8635_gradient_gradient_fct((dot((o8124_1).rgb, tofloat3(1.0))/3.0));
float4 o8707_0_1_rgba = adjust_levels(o8635_0_3_rgba, tofloat4(0.221295998, 0.221295998, 0.221295998, 0.000000000), tofloat4(0.721296012, 0.721296012, 0.721296012, 0.500000000), tofloat4(1.000000000, 1.000000000, 1.000000000, 1.000000000), tofloat4(0.000000000, 0.000000000, 0.000000000, 0.000000000), tofloat4(1.000000000, 1.000000000, 1.000000000, 1.000000000));
float4 o8708_0_1_rgba = o8707_0_1_rgba;
float4 o8710_0_s1 = o8712_0_1_rgba;
float4 o8710_0_s2 = o8708_0_1_rgba;
float o8710_0_a = 1.000000000*1.0;
float4 o8710_0_2_rgba = tofloat4(blend_multiply((uv), o8710_0_s1.rgb, o8710_0_s2.rgb, o8710_0_a*o8710_0_s1.a), min(1.0, o8710_0_s2.a+o8710_0_a*o8710_0_s1.a));
float3 o8100_0_1_rgb = process_normal_default(o8100_fct((frac(transform2((uv), tofloat2(0.250000000*(2.0*o8586_0_1_f-1.0), (Time*.05)*(2.0*1.0-1.0)), 0.000000000*0.01745329251*(2.0*1.0-1.0), tofloat2(1.000000000*(2.0*1.0-1.0), 1.000000000*(2.0*1.0-1.0))))), _seed_variation_), 1.000000000*1024.000000000/128.0);
float4 o8583_0_1_rgba = tofloat4(o8100_0_1_rgb, 1.0);

		// sample the generated texture
		fixed4 rv = tofloat4(((o8581_0_1_rgba).rgb), 1.0)*tofloat4(1.000000000, 1.000000000, 1.000000000, 1.000000000);
		metallic = 1.0*0.000000000;
		roughness = 1.0*1.000000000;
		normal = ((o8583_0_1_rgba).rgb)*float3(-1.0, -1.0, -1.0)+float3(1.0, 1.0, 1.0);
		emission = ((o8710_0_2_rgba).rgb)*3.000000000;
		return rv;

	}
};
Functions f;
fixed4 albedo = f.generated_shader(TexCoords, Time, 
								   texture_1, texture_1Sampler,
								   texture_2, texture_2Sampler,
								   texture_3, texture_3Sampler,
								   texture_4, texture_4Sampler,
								   metallic, roughness, normal, emission);
return albedo;



