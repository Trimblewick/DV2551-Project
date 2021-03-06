struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
};

float4 main(PS_IN input) : SV_TARGET
{	
	float4 light = float4(1.f, 1.f, 1.f, 0.f);
	light = normalize(light);
	float specular = saturate(dot(light, input.nor));
	return float4(0.2f, 0.3f, 0.7f, 1.f) + float4(0.7f, 0.7f, 0.7f, 0.0f)* specular;
}
