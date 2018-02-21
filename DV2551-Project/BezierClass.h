#pragma once
#include <d3d12.h>
#include <d3dcompiler.h>
#include <assert.h>

struct float4
{
	float x;
	float y;
	float z;
	float w;
};

class BezierClass
{
public:
	//public functions
	BezierClass(/**/);
	~BezierClass();
	void CalculateBezierVertices();
	void bezierConstantBuffer(std::vector<float4> vertices);

private:
	//private functions
	

	//private variables
	std::vector<float4>		m_pBezierVertices;
	std::vector<float4>		m_pPreviouslyCalculatedBezierVertices;

	ID3D12DescriptorHeap*	m_pConstantDescHeap;
	ID3D12Resource*			m_pConstantUploadHeap;
};