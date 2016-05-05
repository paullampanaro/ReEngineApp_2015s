#include "MyOctant.h"
using namespace ReEng;
//  MyOctant
void MyOctant::Init(void)
{
	m_v3Position = vector3(0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_fSize = 0.0f;
	m_nChildCount = 8;
}
void MyOctant::Swap(MyOctant& other)
{

}
void MyOctant::Release(void)
{
}
//The big 3
MyOctant::MyOctant(){Init();}
MyOctant::MyOctant(float a_fSize) { Init(); m_fSize = a_fSize; }
MyOctant::MyOctant(MyOctant const& other)
{

}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant(){Release();};

void MyOctant::Display(void)
{
	for (uint n = 0; n < m_nChildCount; n++)
	{ 
		m_pChildren[n].Display();
	}
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Position) * glm::scale(vector3(m_fSize)), REWHITE, WIRE);
}

void MyOctant::Subdivide(void)
{
	m_pChildren = new MyOctant[8];
	m_nChildCount = 8;
	float fNewSize = this->m_fSize / 2;
	for (uint index = 0; index < 8; index++)
	{
		m_pChildren[index].m_fSize = fNewSize;
		m_pChildren[index].m_v3Position = m_v3Position;
	}

	fNewSize /= 2.0f;

	// index 0
	m_pChildren[0].m_v3Position.x += fNewSize;
	m_pChildren[0].m_v3Position.y += fNewSize;
	m_pChildren[0].m_v3Position.z += fNewSize;

	// index 1
	m_pChildren[1].m_v3Position.x -= fNewSize;
	m_pChildren[1].m_v3Position.y += fNewSize;
	m_pChildren[1].m_v3Position.z += fNewSize;

	// index 2
	m_pChildren[2].m_v3Position.x -= fNewSize;
	m_pChildren[2].m_v3Position.y -= fNewSize;
	m_pChildren[2].m_v3Position.z += fNewSize;

	// index 3
	m_pChildren[3].m_v3Position.x += fNewSize;
	m_pChildren[3].m_v3Position.y -= fNewSize;
	m_pChildren[3].m_v3Position.z += fNewSize;

	// index 4
	m_pChildren[4].m_v3Position.x += fNewSize;
	m_pChildren[4].m_v3Position.y += fNewSize;
	m_pChildren[4].m_v3Position.z -= fNewSize;

	// index 5
	m_pChildren[5].m_v3Position.x += fNewSize;
	m_pChildren[5].m_v3Position.y -= fNewSize;
	m_pChildren[5].m_v3Position.z -= fNewSize;

	// index 6
	m_pChildren[6].m_v3Position.x += fNewSize;
	m_pChildren[6].m_v3Position.y += fNewSize;
	m_pChildren[6].m_v3Position.z -= fNewSize;

	// index 7
	m_pChildren[7].m_v3Position.x -= fNewSize;
	m_pChildren[7].m_v3Position.y += fNewSize;
	m_pChildren[7].m_v3Position.z -= fNewSize;
}
void MyOctant::ReleaseChildren(void)
{
	if (m_pChildren != nullptr)
	{
		delete[] m_pChildren;
		m_pChildren = nullptr;
	}
}
//Accessors

//--- Non Standard Singleton Methods

