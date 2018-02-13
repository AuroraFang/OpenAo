// Cinema.cpp: implementation of the CCinema class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cinema.h"
#include "DXUtil.h"

#ifndef _ATUM_CLIENT

	#include "CinemaEditer.h"
	#include "CinemaEditerView.h"
	extern LPDIRECT3DDEVICE9		g_pD3dDev;
	extern CCinemaEditerView*		g_AppFormView;

#else

	#include "AtumApplication.h"
	extern CAtumApplication			*g_pD3dApp;

#endif // _ATUM_CLIENT_endif �ó׸������Ϳ����� �����


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


using namespace std;

CCinema::CCinema()
{
	// ���� �� X ��, ��� Y �� , �Ķ��� Z ��
	m_CurrentCinemaPoint.vPosition = D3DXVECTOR3(0,0,0);	// �ó׸�
	m_CurrentCinemaPoint.vDirection = D3DXVECTOR3(0,0,1);
	m_CurrentCinemaPoint.vUpVector = D3DXVECTOR3(0,1,0);
	m_CurrentCinemaPoint.vTarget = D3DXVECTOR3(0,0,0);
	m_CurrentCinemaPoint.fVelocity = 100.0f;					// �ʱ�ӵ� ���߿� �����ڿ��� �Ǵ� Setting�� �ؾ���..
	m_CurrentCinemaPoint.fCurvature = 1.0f;
	m_bRender = TRUE;
	m_nTargetPointIndex= 0;		// Ÿ�� ���� ��ȣ
	m_nNumOfCinema = 0;			// ���� Cinema ����Ʈ�� ����
	m_nNumOfEvent = 0;			// ���� Event ����Ʈ�� ����
	m_pCinemaVB = NULL;			// Cinema ����Ʈ ������
	m_pEventVB = NULL;			// Event ����Ʈ ������
	m_pSelectedVB = NULL;		// Selected ����Ʈ ������
	m_pCinemaUpVectorVB = NULL;		// UpVector �� ǥ���ϱ� ���� ������
	m_pEventUpVectorVB = NULL;  // Event Point ���� UpVector�����͸� Edit �ϱ� ���� ������
	m_pTargetVertexVB = NULL;	/// TargetVertex
	m_pTargetLineVB = NULL;		/// TargetLine
	m_vecEventPoint.clear();
	m_bNewEventPoint = TRUE;	// ó������ ���ο� �̺�Ʈ ������ �̴�.
//	m_fDistanceToTarget = 0;
	m_nSelectedPoint = -1;		// ���õ� ������ �ε��� -1 : ���� ���� �ȵ�.
	m_nFrame = 0;
	m_bInRange = FALSE;
	m_bUpVectorRender = FALSE;	// �������� UpVector ��ȭ�� ������ �� ���ΰ�?
	m_bTarget = TRUE;
	m_bSelectedType = TRUE;
	m_bTargetRender = FALSE;

	m_nWeaponSpeed = 0;
	m_fWeaponAngle = 0.0f;

	m_bDefaultData = FALSE;
}

CCinema::~CCinema()
{
	SAFE_RELEASE( m_pTargetVertexVB );
	SAFE_RELEASE( m_pTargetLineVB );
	SAFE_RELEASE( m_pEventUpVectorVB );
	SAFE_RELEASE( m_pCinemaUpVectorVB );
	SAFE_RELEASE( m_pSelectedVB );
	SAFE_RELEASE( m_pEventVB );
	SAFE_RELEASE( m_pCinemaVB );
	m_vecEventPoint.clear();
}

void CCinema::InitData(char* i_pData)
{

	memcpy((char*)&m_header,i_pData,sizeof(CINEMA_HEADER));
	EVENT_POINT ep;
	for( int i = 0 ; i < m_header.nEventNum ; i++ )
	{
		memcpy((char*)&ep,i_pData+sizeof(CINEMA_HEADER)+i*sizeof(EVENT_POINT),sizeof(EVENT_POINT));
		InsertEventPoint(0,ep);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCinema::InitDefaultData()
/// \brief		����Ʈ ��
/// \author		ispark
/// \date		2006-03-06 ~ 2006-03-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCinema::InitDefaultData()
{
	m_bDefaultData = TRUE;
}

BOOL CCinema::InsertEventPoint(int index, EVENT_POINT EventPoint)
{
	if (index == 0)
	{
		m_vecEventPoint.push_back(EventPoint);
		m_nNumOfEvent++;	
	}
	m_bRender = FALSE;
	// ���� �ε����� Size ���� ũ�� ����
	return TRUE;
}

void CCinema::InitCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode)
{
	m_CurrentCinemaPoint = i_StartEventPoint;
/*
	float fAngle;	// ���̰�
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);

#ifdef _ATUM_CLIENT
	fAngle = ACOS(D3DXVec3Dot(&D3DXVECTOR3(1,0,0),&m_CurrentCinemaPoint.vDirection));
#else
	fAngle = acos(D3DXVec3Dot(&D3DXVECTOR3(1,0,0),&m_CurrentCinemaPoint.vDirection));
#endif // _ATUM_CLIENT_endif

	// 1. ������ �̿��� �� ������ ���̰��� ���Ѵ�.
	D3DXVECTOR3 vAxis;	// ȸ����
	D3DXVec3Cross(&vAxis, &D3DXVECTOR3(1,0,0), &m_CurrentCinemaPoint.vDirection);	// 2. ������ �̿��� �� ������ ȸ������ ���Ѵ�.
	D3DXMATRIX matTrans;
	D3DXMatrixRotationAxis(&matTrans, &vAxis, fAngle);			// 3. ȸ���࿡ ���� ��ȯ ����� ���Ѵ�.
	D3DXVECTOR3 vResult;		// �ӽ� ���

	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();

	while(iter != m_vecEventPoint.end())
	{
		D3DXVec3TransformCoord(&vResult, &iter->vPosition, &matTrans);
		iter->vPosition = m_CurrentCinemaPoint.vPosition + vResult;
		D3DXVec3TransformCoord(&iter->vDirection, &iter->vDirection, &matTrans);
		D3DXVec3TransformCoord(&iter->vTarget, &iter->vTarget, &matTrans);
		D3DXVec3TransformCoord(&iter->vUpVector, &iter->vUpVector, &matTrans);
		iter++;
	}	
*/
	D3DXMATRIX	mat, mat2;
	D3DXMatrixLookAtLH( &mat, &(m_CurrentCinemaPoint.vPosition), 
		&(m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection), &m_CurrentCinemaPoint.vUpVector);
	D3DXMatrixInverse( &mat, NULL, &mat );
	mat2 = mat;
	mat2._41 = 0;
	mat2._42 = 0;
	mat2._43 = 0;



	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();

	if(iter != m_vecEventPoint.end())
	{
		if(bLHMode)
		{
			while(iter != m_vecEventPoint.end())
			{
				D3DXVec3TransformCoord(&iter->vPosition, &iter->vPosition, &mat);
				D3DXVec3TransformCoord(&iter->vDirection, &iter->vDirection, &mat2);
				D3DXVec3TransformCoord(&iter->vTarget, &iter->vTarget, &mat);
				D3DXVec3TransformCoord(&iter->vUpVector, &iter->vUpVector, &mat2);
				iter++;
			}	
		}
		else
		{
			D3DXPLANE p;
			D3DXVECTOR3 vNormal;//( mat2._11, mat2._12, mat2._13 );

			// ����� ������ ���ϱ�
			D3DXVec3Cross( &vNormal, &iter->vDirection, &iter->vUpVector);
			D3DXPlaneFromPointNormal( &p, &iter->vPosition, &vNormal );
			while(iter != m_vecEventPoint.end())
			{
				// ���� ��Ī�� ���ϱ�
				D3DXVECTOR3 v = iter->vPosition;
				float fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vPosition.x = 2*fT*p.a + v.x;
				iter->vPosition.y = 2*fT*p.b + v.y;
				iter->vPosition.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vPosition, &iter->vPosition, &mat);
				v = iter->vDirection;
				fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vDirection.x = 2*fT*p.a + v.x;
				iter->vDirection.y = 2*fT*p.b + v.y;
				iter->vDirection.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vDirection, &iter->vDirection, &mat2);
				v = iter->vTarget;
				fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vTarget.x = 2*fT*p.a + v.x;
				iter->vTarget.y = 2*fT*p.b + v.y;
				iter->vTarget.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vTarget, &iter->vTarget, &mat);
				v = iter->vUpVector;
				fT = - (p.a*v.x+p.b*v.y+p.c*v.z+p.d) / ( p.a*p.a + p.b*p.b + p.c*p.c );
				iter->vUpVector.x = 2*fT*p.a + v.x;
				iter->vUpVector.y = 2*fT*p.b + v.y;
				iter->vUpVector.z = 2*fT*p.c + v.z;
				D3DXVec3TransformCoord(&iter->vUpVector, &iter->vUpVector, &mat2);
				iter++;
			}	
		}
	}
}

EVENT_POINT	CCinema::GetCurrentCinemaPoint()
{
	return m_CurrentCinemaPoint;
}


#ifdef _ATUM_CLIENT

BOOL CCinema::Tick(D3DXVECTOR3 i_vTargetPosition)
{
	float fElapsedTime=g_pD3dApp->GetElapsedTime();	
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// ���� ������ Size�� ���Ѵ�.

	// Ÿ���� ������..
	if(i_vTargetPosition == D3DXVECTOR3(0,0,0))
	{
		m_bTarget = FALSE;		
	}

	// ������ ó�� ����ɶ�..
	if (m_nTargetPointIndex == 0)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// ��ǥ ���� �����͸� �޾ƿ´�.
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return TRUE;
	}
	// �߰� ������ ���� ��..
	else if(m_nTargetPointIndex > 0 && m_nTargetPointIndex < size)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// ��ǥ ���� �����͸� �޾ƿ´�.
	}
	// ������ Ÿ�� �϶�..
	else if (m_nTargetPointIndex == size)
	{
		m_TargetEventPoint.vPosition = i_vTargetPosition;
		m_TargetEventPoint.fVelocity = m_CurrentCinemaPoint.fVelocity;
	}
	// Ÿ���� ������..
	else if(m_nTargetPointIndex > size)
	{
		
	}

	// ��ǥ ��ġ���� ���� ��ġ�� ���� ��ǥ ������ �����Ѵ�.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// ��ǥ ������ ���� �������� �����.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	

	if(m_nTargetPointIndex < size)
	{
		m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	}
	// ���� ������ ���Ѵ�.
	m_vDifferenceDirection =m_TargetEventPoint.vDirection -  m_CurrentCinemaPoint.vDirection ;

	if(m_nTargetPointIndex < size)
	{
	// ���� ������ ���� �������� �����.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	}
	
	// ���ο� �̺�Ʈ �����Ͷ�� �⺻������ �����Ѵ�.
	if (m_bNewEventPoint)	
	{

		// �̹� �������� �������� �����͸� �����Ѵ�.(�ӷ� ����� ����..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// ���� CinemaPoint�� �̹��������� StartPoint�� �ȴ�.

		/// ���� �ӷ��� Ÿ�� �ӷ� ���� ũ�ٸ�..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// �̹� �������� ��ü �Ÿ��� ���Ѵ�. ( �ӷ� ����� ����..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// ���ο� ��ǥ���� ���� ��ġ�� �Ÿ��� ��ü �Ÿ��� ����.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// ���� ���̻� ���ο� EventPoint�� �ƴϴ�.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// ���� �� ���� ���� �� ���� �Ÿ�
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� ���� ���� �� ���� �Ÿ�
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� �ӷ��� ���Ѵ�.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// ���� �ӷ��� �ּ� �ӷº��� �۴ٸ� �ּ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// ���� �ӷ��� �ִ� �ӷº��� ũ�ٸ� �ִ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// ���� ������ ���Ѵ�.
	if (m_bTarget)
	{
		if(m_nTargetPointIndex < size)
		{
			m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature)*(m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // ȸ���� ����

		}
		else if(m_nTargetPointIndex == size)
		{
			m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature); // ȸ���� ����
		}
	}					
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// ���� ��ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;

	// ���� Ÿ����ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	// fDistance �� ������ ���� �� ���� ���� ������ �Ÿ� 
	// ���� �����͸� ���Ѵ�.
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);

//	m_fDistance = fDistanceToStart;
	
// 2004.06.29 jschoi ����

	if(m_nTargetPointIndex < size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		//	DBGOUT("%.2f\n",fDistanceToNext);
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//			m_fDistanceToTarget = 0.0f;	
		}
		
	}
	else if(m_nTargetPointIndex == size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//		m_fDistanceToTarget = 0.0f;
		}
		
	}

	if(m_bTarget == FALSE || m_nTargetPointIndex >= size)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCinema::SkillTick()
{
	float fElapsedTime=g_pD3dApp->GetElapsedTime();	
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// ���� ������ Size�� ���Ѵ�.

	if (m_nTargetPointIndex >= size)
	{
		m_CurrentCinemaPoint.vPosition = 
			m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;
		return TRUE;
	}

	m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// ��ǥ ���� �����͸� �޾ƿ´�.

	if (m_nTargetPointIndex == 0)
	{
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}

	// ��ǥ ��ġ���� ���� ��ġ�� ���� ��ǥ ������ �����Ѵ�.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// ��ǥ ������ ���� �������� �����.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	// ���� ������ ���Ѵ�.
	m_vDifferenceDirection =m_TargetEventPoint.vDirection -  m_CurrentCinemaPoint.vDirection ;
	// ���� ������ ���� �������� �����.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	

	if (m_bNewEventPoint)	// ���ο� �̺�Ʈ �����Ͷ�� �⺻������ �����Ѵ�.
	{

		// �̹� �������� �������� �����͸� �����Ѵ�.(�ӷ� ����� ����..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// ���� CinemaPoint�� �̹��������� StartPoint�� �ȴ�.

		// ���� �����͸� ���Ѵ�.
		m_vDifferenceUpVector = m_TargetEventPoint.vUpVector - m_StartEventPoint.vUpVector;

		// ���� Ÿ�Ϻ��͸� ���Ѵ�.
//		m_vDifferenceTarget = m_TargetEventPoint.vDirection - m_StartEventPoint.vDirection;

		/// ���� �ӷ��� Ÿ�� �ӷ� ���� ũ�ٸ�..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// �̹� �������� ��ü �Ÿ��� ���Ѵ�. ( �ӷ� ����� ����..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// ���ο� ��ǥ���� ���� ��ġ�� �Ÿ��� ��ü �Ÿ��� ����.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// ���� ���̻� ���ο� EventPoint�� �ƴϴ�.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// ���� �� ���� ���� �� ���� �Ÿ�
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� ���� ���� �� ���� �Ÿ�
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� �ӷ��� ���Ѵ�.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// ���� �ӷ��� �ּ� �ӷº��� �۴ٸ� �ּ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// ���� �ӷ��� �ִ� �ӷº��� ũ�ٸ� �ִ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// ���� ������ ���Ѵ�.
	m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature*m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // ȸ���� ����

	// ���� ������ ���� �������� 				
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// ���� ��ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;

	// ���� Ÿ����ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);
//	m_CurrentCinemaPoint.vTarget = m_StartEventPoint.vTarget;

	// ���� �����͸� ���Ѵ�.
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	// ���� UpVector�� ���� �������� �����.
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);	

//	m_fDistance = fDistanceToStart;

// 2004.06.29 jschoi ����


	if(m_nTargetPointIndex < size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		//	DBGOUT("%.2f\n",fDistanceToNext);
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//			m_fDistanceToTarget = 0.0f;	
		}
		
	}
	else if(m_nTargetPointIndex == size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < 15)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//		m_fDistanceToTarget = 0.0f;
		}
		
	}

	return FALSE;
}

#endif	// _ATUM_CLIENT_endif Ŭ���̾�Ʈ������ ���Ǵ� �Լ�..


#ifndef _ATUM_CLIENT	// ������� �ó׸� �����Ϳ����� ���� �Լ���..

BOOL CCinema::Tick()
{
	float fElapsedTime=g_AppFormView->GetElapsedTime();	
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// ���� ������ Size�� ���Ѵ�.

	if (size<2)
		return FALSE;

	if (m_nTargetPointIndex >= size)
		return FALSE;

	m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// ��ǥ ���� �����͸� �޾ƿ´�.

	if (m_nTargetPointIndex == 0)
	{
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}

	// ��ǥ ��ġ���� ���� ��ġ�� ���� ��ǥ ������ �����Ѵ�.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// ��ǥ ������ ���� �������� �����.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	// ���� ������ ���Ѵ�.
	m_vDifferenceDirection =m_TargetEventPoint.vDirection -  m_CurrentCinemaPoint.vDirection ;
	// ���� ������ ���� �������� �����.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	

	if (m_bNewEventPoint)	// ���ο� �̺�Ʈ �����Ͷ�� �⺻������ �����Ѵ�.
	{
		// �̹� �������� �������� �����͸� �����Ѵ�.(�ӷ� ����� ����..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// ���� CinemaPoint�� �̹��������� StartPoint�� �ȴ�.

		// ���� �����͸� ���Ѵ�.
		m_vDifferenceUpVector = m_TargetEventPoint.vUpVector - m_StartEventPoint.vUpVector;

		// ���� Ÿ�Ϻ��͸� ���Ѵ�.
//		m_vDifferenceTarget = m_TargetEventPoint.vDirection - m_StartEventPoint.vDirection;

		/// ���� �ӷ��� Ÿ�� �ӷ� ���� ũ�ٸ�..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// �̹� �������� ��ü �Ÿ��� ���Ѵ�. ( �ӷ� ����� ����..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// ���ο� ��ǥ���� ���� ��ġ�� �Ÿ��� ��ü �Ÿ��� ����.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// ���� ���̻� ���ο� EventPoint�� �ƴϴ�.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// ���� �� ���� ���� �� ���� �Ÿ�
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� ���� ���� �� ���� �Ÿ�
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� �ӷ��� ���Ѵ�.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// ���� �ӷ��� �ּ� �ӷº��� �۴ٸ� �ּ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// ���� �ӷ��� �ִ� �ӷº��� ũ�ٸ� �ִ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// ���� ������ ���Ѵ�.
	m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature*m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // ȸ���� ����

	// ���� ������ ���� �������� 				
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// ���� ��ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;

	// ���� Ÿ����ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

//	DBGOUT("%.2f, %.2f, %.2f \n",m_CurrentCinemaPoint.vTarget.x,m_CurrentCinemaPoint.vTarget.y,m_CurrentCinemaPoint.vTarget.z);

	// ���� �����͸� ���Ѵ�.
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;

	// ���� UpVector�� ���� �������� �����.
	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);	

//	m_fDistance = fDistanceToStart;

	XYZ_DIFUSE_VERTEX* v;

	m_pCinemaVB->Lock( 0, 0, (BYTE**)&v, 0  );

	if(m_nNumOfCinema>=99999)
		return FALSE;
	v[m_nNumOfCinema].p = m_CurrentCinemaPoint.vPosition;
	v[m_nNumOfCinema].c = 0xffdddddd;

	m_pCinemaVB->Unlock();

	// �ó׸� �������� �� ���۸� ä���.
	m_pCinemaUpVectorVB->Lock( 0, 0, (BYTE**)&v, 0  );
	D3DXVECTOR3 vTemp;

	if(m_nNumOfCinema>=99999)
		return FALSE;
	v[m_nNumOfCinema*2].p = m_CurrentCinemaPoint.vPosition;
	v[m_nNumOfCinema*2].c = 0xffbbbbbb;
	vTemp = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vUpVector*50;

	v[m_nNumOfCinema*2+1].p = vTemp;
	v[m_nNumOfCinema*2+1].c = 0xff5555aa;

	m_pCinemaVB->Unlock();
	m_nNumOfCinema++;

// 2004.06.29 jschoi ����


	float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
	if (fDistanceToNext < 15)
		fDistanceToNext = 15;
	if(fDistanceToTarget < fDistanceToNext)
	{
		m_bInRange = TRUE;
		m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
		m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}
	else
	{
//		m_fDistanceToTarget = fDistanceToTarget;
		m_nFrame++;
	}
	return TRUE;
/*
	// ��� ���� �������� �Ÿ��� ���� ���� �������� �Ÿ����� ũ�� �� ���� ����ģ�Ŵ�. 
	// �׸��� �Ÿ� 15 �������� �ѹ��̶� ��� �Ծ�� �Ѵ�.
	if(fDistanceToTarget > m_fDistanceToTarget && m_bInRange)
	{
		m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
		m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return FALSE;
	}
*/
}


HRESULT CCinema::Render()
{
	float fCinemaPointSize = 5.0f;
	float fEventPointSize = 12.0f;
	float fSelectedPointSize = 14.0f;
	float fTargetPointSize = 10.0f;

	// �̺�Ʈ ����Ʈ �� ����
	g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&(fEventPointSize)));
	g_pD3dDev->SetStreamSource( 0, m_pEventVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
	g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,m_nNumOfEvent);

	if(m_bUpVectorRender)
	{
		// �̺�Ʈ ����Ʈ�� UpVector ����
		g_pD3dDev->SetStreamSource( 0, m_pEventUpVectorVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_LINELIST , 0,m_nNumOfEvent);
	}

	if(m_bTargetRender)
	{
		// Ÿ�� ����Ʈ�� �� ����
		g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&(fTargetPointSize)));
		g_pD3dDev->SetStreamSource( 0, m_pTargetVertexVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,m_nNumOfEvent);

		// Ÿ�� ������ ������
		g_pD3dDev->SetStreamSource( 0, m_pTargetLineVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_LINELIST , 0,m_nNumOfEvent);
	}

	if(m_nSelectedPoint != -1)
	{
		// ���õ� ����Ʈ �� ����
		g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&(fSelectedPointSize)));
		g_pD3dDev->SetStreamSource( 0, m_pSelectedVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,1);
	}

	if(m_bRender)
	{
		// �ó׸� ����Ʈ �� ����
		g_pD3dDev->SetRenderState( D3DRS_POINTSIZE,*((DWORD*)&fCinemaPointSize));
		g_pD3dDev->SetStreamSource( 0, m_pCinemaVB,0, sizeof(XYZ_DIFUSE_VERTEX) );
//		g_pD3dDev->DrawPrimitive( D3DPT_POINTLIST , 0,m_nNumOfCinema);
		g_pD3dDev->DrawPrimitive( D3DPT_LINESTRIP , 0,m_nNumOfCinema-1);

//		if(m_bUpVectorRender)
//		{
			// UpVector �� ���� ���� ������
//			g_pD3dDev->SetStreamSource( 0, m_pCinemaUpVectorVB, sizeof(XYZ_DIFUSE_VERTEX) );
//			g_pD3dDev->DrawPrimitive( D3DPT_LINELIST , 0,m_nNumOfCinema);
//		}

	}

	return S_OK;
}


BOOL CCinema::Reset()
{
	m_bRender = TRUE;
	m_CurrentCinemaPoint.vPosition = D3DXVECTOR3(0,0,0);
	m_CurrentCinemaPoint.vDirection = D3DXVECTOR3(1,0,0);
	m_CurrentCinemaPoint.vUpVector = D3DXVECTOR3(0,1,0);
	m_CurrentCinemaPoint.vTarget = D3DXVECTOR3(0,0,0);
	m_CurrentCinemaPoint.fVelocity = 100.0f;
	m_CurrentCinemaPoint.fCurvature = 1.0f;
	m_nNumOfCinema = 0;
	m_bNewEventPoint = TRUE;	// ó������ ���ο� �̺�Ʈ ������ �̴�.
	m_nTargetPointIndex= 0;		// Ÿ�� ���� ��ȣ
	m_nFrame = 0;
	return TRUE;
}


BOOL CCinema::RemoveEventPoint(int index)
{
	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// ���� ������ Size�� ���Ѵ�.
	// ���� �ε����� Size ���� ũ�� ����
	if (index == 0 && m_nSelectedPoint != -1 && size>0)
	{
		vector<EVENT_POINT>::iterator iter = &m_vecEventPoint[m_nSelectedPoint];
		m_vecEventPoint.erase(iter);
		m_nSelectedPoint = -1;
		m_nNumOfEvent--;
		UpdatePoint();

	}
	return TRUE;
}

HRESULT CCinema::InitDeviceObjects()
{

	return S_OK;
}

HRESULT CCinema::RestoreDeviceObjects()
{

	CreateVB();

	return S_OK;
}

HRESULT CCinema::InvalidateDeviceObjects()
{

	SAFE_RELEASE( m_pTargetVertexVB );
	SAFE_RELEASE( m_pTargetLineVB );
	SAFE_RELEASE( m_pEventUpVectorVB );
	SAFE_RELEASE( m_pCinemaUpVectorVB );
	SAFE_RELEASE( m_pSelectedVB );
	SAFE_RELEASE( m_pEventVB );
	SAFE_RELEASE( m_pCinemaVB );

	return S_OK;
}

HRESULT CCinema::DeleteDeviceObjects()
{

	return S_OK;
}

BOOL CCinema::SaveEventPoints()
{
	return TRUE;
}

BOOL CCinema::LoadEventPoints()
{
	return TRUE;
}

BOOL CCinema::SaveCameraPoints()
{
	return TRUE;
}

BOOL CCinema::LoadCameraPoints()
{
	return TRUE;
}

BOOL CCinema::UpdatePoint()
{
	XYZ_DIFUSE_VERTEX* v;

	// ���õ� Point �� ����
	if(m_nSelectedPoint != -1)
	{		
		m_pSelectedVB->Lock( 0, 0, (BYTE**)&v, 0  );
		v[0].p = m_vecEventPoint[m_nSelectedPoint].vPosition;
		v[0].c = 0xffff0000;
		m_pSelectedVB->Unlock();
	}

	// Event Point �� ����
	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();
	int i = 0;
	m_pEventVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i].p = iter->vPosition;
		v[i].c = 0xff00ff00;
		iter++;
		i++;
	}
	m_pEventVB->Unlock();

	/// UpVector �� ���� 
	iter = m_vecEventPoint.begin();
	i = 0;
	m_pEventUpVectorVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i*2].p = iter->vPosition;
		v[i*2].c = 0xffffff00;
		v[i*2+1].p = iter->vPosition + iter->vUpVector*100;
		v[i*2+1].c = 0xffffff00;
		iter++;
		i++;
	}
	m_pEventUpVectorVB->Unlock();

	// Target Point �� ����
	iter = m_vecEventPoint.begin();
	i = 0;
	m_pTargetVertexVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i].p = iter->vTarget;
		v[i].c = 0xff0055ff;
		iter++;
		i++;
	}
	m_pTargetVertexVB->Unlock();

	/// Target Line �� ���� 
	iter = m_vecEventPoint.begin();
	i = 0;
	m_pTargetLineVB->Lock( 0, 0, (BYTE**)&v, 0  );
	while (iter != m_vecEventPoint.end())
	{
		v[i*2].p = iter->vPosition;
		v[i*2].c = 0xffff1111;
		v[i*2+1].p = iter->vTarget;
		v[i*2+1].c = 0xffff1111;
		iter++;
		i++;
	}
	m_pTargetLineVB->Unlock();


	m_bRender = FALSE;


	return TRUE;
}


HRESULT CCinema::CreateVB()
{
	//Cinema Point �� ���� ����
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 100000*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pCinemaVB,NULL ) ) )
        return E_FAIL;

	//Event Point �� ���� ����
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 100*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pEventVB,NULL ) ) )
        return E_FAIL;

	//UpVector �� ���� ���� ( CinemaPoint*2 ����)
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 200000*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pCinemaUpVectorVB,NULL ) ) )
        return E_FAIL;

	//Event UpVector �� ���� ���� ( EventPoint*2 ����)
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 200*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pEventUpVectorVB,NULL ) ) )
        return E_FAIL;

	//Selected Point �� ���� ����
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 1*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pSelectedVB,NULL ) ) )
        return E_FAIL;

	//Target Point �� ���� ����
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 100*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pTargetVertexVB ,NULL) ) )
        return E_FAIL;

	//TargetLine Point �� ���� ����
    if( FAILED( g_pD3dDev->CreateVertexBuffer( 200*sizeof( XYZ_DIFUSE_VERTEX ),
		D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFUSE_VERTEX, D3DPOOL_MANAGED, &m_pTargetLineVB,NULL ) ) )
        return E_FAIL;

	return S_OK;
}

int CCinema::GetTargetIndex()
{
	return m_nTargetPointIndex;
}

BOOL CCinema::LengthPoint(D3DXVECTOR3 vEyes, D3DXVECTOR3 vDir, D3DXVECTOR3 vPosition)
{
	/// ���� ���� Size
	float fTestSize = 100.0f;

	// ���� �ӽ� ����
	float fTempLen,u,w;
	// ������ ���� ������ �������� ����
	/// ������ ����� ���������� ���� ����
	D3DXVECTOR3 vPl;
	vPl = vPosition - vEyes;
	// ���� ������ �������� ���Ϳ� ���� ������ ����
	u = D3DXVec3Dot(&vDir, &vPl);
	// ���� ������ ���� ���Ϳ� 90�� �̻��� ��쿡�� ����
//	if(u<0.001f)
//		return FALSE;
	// ���� ������ �������� ������ ũ�� ���
	w = D3DXVec3Length(&vPl);
	// �Ÿ� ���
	fTempLen = sqrt(w*w-u*u);
	// ���� ������ ���ԵǴ��� �˻��Ѵ�.

	if(fTempLen < fTestSize)					/// fTestSize �˻� ����
	{
		/// ���� �ּ� �Ÿ����� Ȯ�� �Ѵ�.
		if(fTempLen < m_fMinimumLength)
		{
			m_fMinimumLength = fTempLen;			/// �ּ� �Ÿ� ����
			return TRUE;						
		}
	}
	return FALSE;
}

int CCinema::PickUpPoint(CPoint i_pointMouse, DWORD i_dwViewType, float i_fZoom)
{

	float fShortDistance = 100.0f;	// �⺻ �ִ� �Ÿ�
	float fDistance;
	int nResult = -1;			// ��ȯ�� �ʱ�ȭ 

	// �� ���۸� ����
//	XYZ_DIFUSE_VERTEX* v;

//	m_pEventVB->Lock( 0, 0, (BYTE**)&v, 0  );

	CPoint pointPixel;

	/// ������ ��� ������ 2D ��ǥ�� ȯ���� �� �Է����� ���� ���콺 �����Ϳ� �Ÿ��� ����Ͽ� ���� ����� �Ÿ��� �ִ� ���� �����Ѵ�.
	for( int i = 0; i < m_nNumOfEvent ; i++)
	{
		pointPixel = GetPixelFromPosition(m_vecEventPoint[i].vPosition,i_dwViewType,i_fZoom);
		fDistance = (float)sqrt((pointPixel.x - i_pointMouse.x)*(pointPixel.x - i_pointMouse.x)+(pointPixel.y - i_pointMouse.y)*(pointPixel.y - i_pointMouse.y));
		if(fDistance < fShortDistance)
		{
			nResult = i;
			m_bSelectedType = TRUE;
			fShortDistance = fDistance;
		}
		if(m_bTargetRender)
		{
			pointPixel = GetPixelFromPosition(m_vecEventPoint[i].vTarget,i_dwViewType,i_fZoom);
			fDistance = (float)sqrt((pointPixel.x - i_pointMouse.x)*(pointPixel.x - i_pointMouse.x)+(pointPixel.y - i_pointMouse.y)*(pointPixel.y - i_pointMouse.y));
			if(fDistance < fShortDistance)
			{
				nResult = i;
				m_bSelectedType = FALSE;
				fShortDistance = fDistance;
			}
		}		
	}
	// ���۸� �ݴ´�.
//	m_pEventVB->Unlock();

	if(fShortDistance < 10)
	{
		m_nSelectedPoint = nResult;
		UpdatePoint();
	}
	else
	{
		m_nSelectedPoint = -1;
	}
	return -1;
}


CPoint CCinema::GetPixelFromPosition(D3DXVECTOR3 i_vPosition, DWORD i_dwViewType, float i_fZoom)
{
	CPoint pointPixel;
	switch(i_dwViewType)
	{
	case TOPVIEW:	/// TOPVIEW
		{
			pointPixel.x = (WINDOW_WIDTH/2)+(i_vPosition.x*10)/i_fZoom;
			pointPixel.y = (WINDOW_HEIGHT/2)-(i_vPosition.z*7.5)/i_fZoom;
		}
		break;
	case SIDEVIEW: /// SIDEVIEW
		{
			pointPixel.x = (WINDOW_WIDTH/2)+(i_vPosition.z*10)/i_fZoom;
			pointPixel.y = (WINDOW_HEIGHT/2)-(i_vPosition.y*7.5)/i_fZoom;
		}
		break;
	case FRONTVIEW: /// FRONTVIEW
		{
			pointPixel.x = (WINDOW_WIDTH/2)+(i_vPosition.x*10)/i_fZoom;
			pointPixel.y = (WINDOW_HEIGHT/2)-(i_vPosition.y*7.5)/i_fZoom;
		}
		break;
	}
	return pointPixel;
}

void CCinema::MovePoint(CPoint i_pointMouseMove, DWORD i_dwViewType, float i_fZoom)		/// ���õ� �̺�Ʈ ������ �̵�
{
	if(m_nSelectedPoint == -1)
		return;
	if(m_bSelectedType)
	{
		switch(i_dwViewType)
		{
		case TOPVIEW: /// TOPVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vPosition.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vPosition.z += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case SIDEVIEW: /// SIDEVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vPosition.z -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vPosition.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case FRONTVIEW: /// FRONTVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vPosition.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vPosition.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		}
	}
	else
	{
		switch(i_dwViewType)
		{
		case TOPVIEW: /// TOPVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vTarget.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vTarget.z += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case SIDEVIEW: /// SIDEVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vTarget.z -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vTarget.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		case FRONTVIEW: /// FRONTVIEW
			{
				m_vecEventPoint[m_nSelectedPoint].vTarget.x -= (i_pointMouseMove.x*i_fZoom)/10;
				m_vecEventPoint[m_nSelectedPoint].vTarget.y += (i_pointMouseMove.y*1.3333*i_fZoom)/10;
			}
			break;
		}

	}
	UpdatePoint();	
}

void CCinema::ChangeUpVector(CPoint i_pointMouseMove,DWORD i_dwViewType, float i_fZoom)
{
	if(m_nSelectedPoint == -1)
		return ; /// ���õ� ����Ʈ�� ������ �׳� ����
	if(!m_bUpVectorRender)
		return ; /// Up Vector �� ���������� �ƴϸ� �׳� ����
	switch(i_dwViewType)
	{
	case TOPVIEW:	/// TOPVIEW
		{
			m_vecEventPoint[m_nSelectedPoint].vUpVector.x -= (i_pointMouseMove.x*i_fZoom)/1000;
			m_vecEventPoint[m_nSelectedPoint].vUpVector.z += (i_pointMouseMove.y*1.3333*i_fZoom)/1000;			
		}
		break;
	case SIDEVIEW: /// SIDEVIEW
		{
			m_vecEventPoint[m_nSelectedPoint].vUpVector.z -= (i_pointMouseMove.x*i_fZoom)/1000;
			m_vecEventPoint[m_nSelectedPoint].vUpVector.y += (i_pointMouseMove.y*1.3333*i_fZoom)/1000;
		}
		break;
	case FRONTVIEW: /// FRONTVIEW
		{
			m_vecEventPoint[m_nSelectedPoint].vUpVector.x -= (i_pointMouseMove.x*i_fZoom)/1000;
			m_vecEventPoint[m_nSelectedPoint].vUpVector.y += (i_pointMouseMove.y*1.3333*i_fZoom)/1000;
		}
		break;
	}
	D3DXVec3Normalize(&m_vecEventPoint[m_nSelectedPoint].vUpVector,&m_vecEventPoint[m_nSelectedPoint].vUpVector);
	UpdatePoint();
}

void CCinema::Save(char* strName)
{
	HANDLE hFile = NULL;
	DWORD dwWritten;
	hFile = CreateFile(strName, GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();
	vector<EVENT_POINT>::iterator iter = m_vecEventPoint.begin();

	CINEMA_HEADER header;

	header.nEventNum = (int)size;

	// Ÿ���� ������ �ϰ� �ִٸ� Ÿ�Ϲ����� �ִ� �����̴�.
	if(m_bTargetRender)
	{
		header.nPatternType = 1;	// Ÿ�Ϲ����� �ִ� ����
	}
	else
	{
		header.nPatternType = 2;	// Ÿ�Ϲ����� ���� ����
	}

	ZeroMemory(&header.strDescription,sizeof(header.strDescription));

	WriteFile(hFile,(char*)&header,sizeof(header),&dwWritten,NULL);

	EVENT_POINT ep;
	while(iter != m_vecEventPoint.end())
	{
		ep.vPosition = iter->vPosition;
		ep.vDirection = iter->vDirection;
		ep.vUpVector = iter->vUpVector;
		ep.fVelocity = iter->fVelocity;
		ep.fCurvature = iter->fCurvature;
		ep.vTarget = iter->vTarget;
		WriteFile(hFile,(char*)&ep,sizeof(EVENT_POINT),&dwWritten,NULL);
		iter++;
	}
	CloseHandle(hFile);
}

void CCinema::Open(char* strName)
{
	HANDLE hFile = NULL;
	DWORD dwRead;
	hFile = CreateFile(strName, GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	ReadFile(hFile,(char*)&m_header,sizeof(CINEMA_HEADER),&dwRead,NULL);

	EVENT_POINT ep;
	m_vecEventPoint.clear();
	for(int i = 0 ; i < m_header.nEventNum ; i++)
	{
		ReadFile(hFile,(char*)&ep,sizeof(EVENT_POINT),&dwRead,NULL);
		InsertEventPoint(0, ep);
	}
	CloseHandle(hFile);

	if(m_header.nPatternType == 1)
	{
		m_bTargetRender = TRUE;
	}
	else if(m_header.nPatternType == 2)
	{
		m_bTargetRender = FALSE;
	}
	m_nNumOfEvent = i;
	UpdatePoint();
	m_nSelectedPoint = -1;
	m_bRender = FALSE;

}

int	CCinema::GetSelectedPoint()
{
	return m_nSelectedPoint;
}

void CCinema::UpVectorRender()
{
	m_bUpVectorRender = !m_bUpVectorRender;
}


BOOL CCinema::IsRender()
{
	return m_bRender;
}


void CCinema::TargetRender()
{
	m_bTargetRender = !m_bTargetRender;
}

BOOL CCinema::IsTargetRender()
{
	return m_bTargetRender;
}

void CCinema::Next()
{
	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();

	if(m_vecEventPoint.empty())
		return ;

	if(m_nSelectedPoint == -1)
	{
		m_nSelectedPoint = 0;
	}
	else if((m_nSelectedPoint+1) < size)
	{
		m_nSelectedPoint++;
	}
	else if((m_nSelectedPoint+1) == size)
	{
		m_nSelectedPoint = 0;
	}
}

#endif // _ATUM_CLIENT_endif ������� �ó׸� �����Ϳ����� ���� �Լ���..



///////////////////////////////////////////////////////////////////////////////
/// \fn			CCinema::WeaponTick(D3DXVECTOR3 i_vTargetPosition, float fElapsedTime)
/// \brief		2�� ���� �ó׸� ƽ
/// \author		ispark
/// \date		2005-12-02 ~ 2005-12-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
//BOOL CCinema::WeaponTick(D3DXVECTOR3 i_vTargetPosition, float fElapsedTime)
BOOL CCinema::WeaponTick( D3DXVECTOR3 i_vTargetPosition, float fElapsedTime, float fDecSpeed /* = 0.0f */ )
// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
{
	float fDistanceToStart;
	float fDistanceToTarget;

	vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// ���� ������ Size�� ���Ѵ�.

	// Ÿ���� ������..
	if(i_vTargetPosition == D3DXVECTOR3(0,0,0))
	{
		m_bTarget = FALSE;		
	}

	// ������ ó�� ����ɶ�..
	if(m_bDefaultData == FALSE && m_nTargetPointIndex == 0)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// ��ǥ ���� �����͸� �޾ƿ´�.
		m_CurrentCinemaPoint = m_TargetEventPoint;
		m_nTargetPointIndex++;
		m_bNewEventPoint = TRUE;
//		m_fDistanceToTarget = 0.0f;
		m_nFrame = 0;
		return TRUE;
	}
	// �߰� ������ ���� ��..
	else if(m_nTargetPointIndex > 0 && m_nTargetPointIndex < size)
	{
		m_TargetEventPoint = m_vecEventPoint[m_nTargetPointIndex];		// ��ǥ ���� �����͸� �޾ƿ´�.
	}
	// ������ Ÿ�� �϶�..
	else if (m_nTargetPointIndex == size)
	{
		m_TargetEventPoint.vPosition = i_vTargetPosition;
		m_TargetEventPoint.fVelocity = m_CurrentCinemaPoint.fVelocity = m_nWeaponSpeed;
	}
	// Ÿ���� ������..
	else if(m_nTargetPointIndex > size)
	{
		
	}

	if(m_bDefaultData == TRUE && m_nTargetPointIndex == 0)
	{
		m_TargetEventPoint.vPosition = i_vTargetPosition;
		m_TargetEventPoint.fVelocity = m_CurrentCinemaPoint.fVelocity = m_nWeaponSpeed;
	}
	
	// ��ǥ ��ġ���� ���� ��ġ�� ���� ��ǥ ������ �����Ѵ�.
	m_TargetEventPoint.vDirection = m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition;	
	// ��ǥ ������ ���� �������� �����.
	D3DXVec3Normalize(&m_TargetEventPoint.vDirection,&m_TargetEventPoint.vDirection);	

	if(m_nTargetPointIndex < size)
	{
		m_TargetEventPoint.vDirection = m_TargetEventPoint.vDirection * 2;
	}
	// ���� ������ ���Ѵ�.
	m_vDifferenceDirection = m_TargetEventPoint.vDirection - m_CurrentCinemaPoint.vDirection ;

	if(m_nTargetPointIndex < size)
	{
	// ���� ������ ���� �������� �����.
	D3DXVec3Normalize(&m_vDifferenceDirection,&m_vDifferenceDirection);		
	}
	
	// ���ο� �̺�Ʈ �����Ͷ�� �⺻������ �����Ѵ�.
	if(m_bNewEventPoint)	
	{

		// �̹� �������� �������� �����͸� �����Ѵ�.(�ӷ� ����� ����..)
		m_StartEventPoint = m_CurrentCinemaPoint;		// ���� CinemaPoint�� �̹��������� StartPoint�� �ȴ�.

		/// ���� �ӷ��� Ÿ�� �ӷ� ���� ũ�ٸ�..
		if (m_StartEventPoint.fVelocity >= m_TargetEventPoint.fVelocity)
		{
			m_nMaxVelocity = m_StartEventPoint.fVelocity;
			m_nMinVelocity = m_TargetEventPoint.fVelocity;
		}
		else
		{
			m_nMinVelocity = m_StartEventPoint.fVelocity;
			m_nMaxVelocity = m_TargetEventPoint.fVelocity;
		}

		// �̹� �������� ��ü �Ÿ��� ���Ѵ�. ( �ӷ� ����� ����..)
		m_fTotalDistance = D3DXVec3Length(&(m_TargetEventPoint.vPosition-m_StartEventPoint.vPosition));

		// ���ο� ��ǥ���� ���� ��ġ�� �Ÿ��� ��ü �Ÿ��� ����.
//		m_fDistanceToTarget = m_fTotalDistance;
//		m_fDistance = 0;
		// ���� ���̻� ���ο� EventPoint�� �ƴϴ�.
		m_bNewEventPoint = FALSE;
		m_bInRange = FALSE;

	}

	// ���� �� ���� ���� �� ���� �Ÿ�
	fDistanceToStart = D3DXVec3Length(&(m_StartEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� ���� ���� �� ���� �Ÿ�
	fDistanceToTarget = D3DXVec3Length(&(m_TargetEventPoint.vPosition - m_CurrentCinemaPoint.vPosition));

	// ���� �ӷ��� ���Ѵ�.
	m_CurrentCinemaPoint.fVelocity = ((m_StartEventPoint.fVelocity*fDistanceToTarget)+(m_TargetEventPoint.fVelocity*fDistanceToStart))/(m_fTotalDistance);
	// ���� �ӷ��� �ּ� �ӷº��� �۴ٸ� �ּ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity < m_nMinVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMinVelocity;
	}
	// ���� �ӷ��� �ִ� �ӷº��� ũ�ٸ� �ִ� �ӷ�����
	if(m_CurrentCinemaPoint.fVelocity > m_nMaxVelocity)
	{
		m_CurrentCinemaPoint.fVelocity = m_nMaxVelocity;
	}

	// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
	float fCurEPVelocity, fTarEPVelocity;
	fCurEPVelocity = m_CurrentCinemaPoint.fVelocity;
	fTarEPVelocity = m_TargetEventPoint.fVelocity;
	if( fDecSpeed != 0.0f )
	{
		float fDecreasedSpeed = m_CurrentCinemaPoint.fVelocity - fDecSpeed;
		if( fDecreasedSpeed < 0.0f )
			fDecreasedSpeed = 0.0f;

		fCurEPVelocity = fTarEPVelocity = fDecreasedSpeed;
	}
	// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)

	// ���� ������ ���Ѵ�.
	if(m_bTarget)
	{
		if(m_nTargetPointIndex < size)
		{
			m_CurrentCinemaPoint.vDirection = m_CurrentCinemaPoint.vDirection + 2*(m_vDifferenceDirection*fElapsedTime*m_StartEventPoint.fCurvature)*(m_CurrentCinemaPoint.fVelocity/m_fTotalDistance); // ȸ���� ����
		}
		else if(m_nTargetPointIndex == size)
		{
			// ������ �ƴ� �Ѿư��� ����
			float dot;
			double dRadian = 0;
			D3DXVECTOR3 vCrossVec;

			D3DXVec3Cross(&vCrossVec, &m_TargetEventPoint.vDirection, &m_CurrentCinemaPoint.vDirection);
			dot = D3DXVec3Dot(&m_CurrentCinemaPoint.vDirection, &m_TargetEventPoint.vDirection);
			float fElapsedWeaponAngle = m_fWeaponAngle * fElapsedTime;
			if(D3DXToRadian(fElapsedWeaponAngle) < ACOS(dot))
			{
				dot = cos(D3DXToRadian(fElapsedWeaponAngle));
			}
			if(1.0f > dot)
			{
				dRadian = ACOS(dot);
			}
//			DBGOUT("%f(%f), Time = %f\n", D3DXToDegree(ACOS(dot)), m_fWeaponAngle, fElapsedTime);
//			DBGOUT("%f, Time = %f\n", dRadian, fElapsedTime);
//			DBGOUT("%f, Time = %f\n", D3DXToDegree(dRadian), fElapsedTime);
			D3DXQUATERNION qOrg(m_CurrentCinemaPoint.vDirection.x, m_CurrentCinemaPoint.vDirection.y, m_CurrentCinemaPoint.vDirection.z, 0.0f);
			D3DXQUATERNION qRot, qRotConj;
			D3DXQuaternionRotationAxis(&qRot, &vCrossVec, (float)dRadian);
			D3DXQuaternionConjugate(&qRotConj, &qRot);
			D3DXQUATERNION qRst = qRot * qOrg * qRotConj;

			m_CurrentCinemaPoint.vDirection.x = qRst.x;
			m_CurrentCinemaPoint.vDirection.y = qRst.y;
			m_CurrentCinemaPoint.vDirection.z = qRst.z;
		}
	}

	D3DXVec3Normalize(&m_CurrentCinemaPoint.vDirection,&m_CurrentCinemaPoint.vDirection);	

	// ���� ��ġ�� ���Ѵ�.
	// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
// 	if(m_nTargetPointIndex < size)
// 	{
// 		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_CurrentCinemaPoint.fVelocity*fElapsedTime;
// 	}
// 	else if(m_nTargetPointIndex == size)
// 	{
// 		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition + m_CurrentCinemaPoint.vDirection*m_nWeaponSpeed*fElapsedTime;
// 	}

	if(m_nTargetPointIndex < size)
	{
		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition
										 + m_CurrentCinemaPoint.vDirection * fCurEPVelocity * fElapsedTime;
	}
	else if(m_nTargetPointIndex == size)
	{
		float fWeaponSpeed = (float)m_nWeaponSpeed - fDecSpeed;
		if( fWeaponSpeed < 0.0f )
			fWeaponSpeed = 0.0f;
		m_CurrentCinemaPoint.vPosition = m_CurrentCinemaPoint.vPosition
										 + m_CurrentCinemaPoint.vDirection * fWeaponSpeed * fElapsedTime;
	}
	// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)

	// ���� Ÿ����ġ�� ���Ѵ�.
	m_CurrentCinemaPoint.vTarget = (m_StartEventPoint.vTarget*fDistanceToTarget + m_TargetEventPoint.vTarget*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	// fDistance �� ������ ���� �� ���� ���� ������ �Ÿ� 
	// ���� �����͸� ���Ѵ�.
//	m_CurrentCinemaPoint.vUpVector = m_CurrentCinemaPoint.vUpVector + m_vDifferenceUpVector*(fDistanceToStart-m_fDistance)/m_fTotalDistance;
	m_CurrentCinemaPoint.vUpVector = (m_StartEventPoint.vUpVector*fDistanceToTarget+m_TargetEventPoint.vUpVector*fDistanceToStart)/(fDistanceToTarget+fDistanceToStart);

	D3DXVec3Normalize(&m_CurrentCinemaPoint.vUpVector,&m_CurrentCinemaPoint.vUpVector);

//	m_fDistance = fDistanceToStart;
	
// 2004.06.29 jschoi ����

	if(m_nTargetPointIndex < size)
	{
		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
		//	DBGOUT("%.2f\n",fDistanceToNext);
		if (fDistanceToNext < 15)
			fDistanceToNext = 15;
		if(fDistanceToTarget < fDistanceToNext)
		{
			//		m_bInRange = TRUE;
			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
			m_nTargetPointIndex++;
			m_bNewEventPoint = TRUE;
			//			m_fDistanceToTarget = 0.0f;	
		}
		
	}
	else if(m_nTargetPointIndex == size)
	{
//		float fDistanceToNext = m_CurrentCinemaPoint.fVelocity*fElapsedTime*5;
//		if (fDistanceToNext < 15)
//			fDistanceToNext = 15;
//		if(fDistanceToTarget < fDistanceToNext)
//		{
//			//		m_bInRange = TRUE;
//			m_CurrentCinemaPoint.fVelocity = m_TargetEventPoint.fVelocity;
//			m_CurrentCinemaPoint.fCurvature = m_TargetEventPoint.fCurvature;
//			m_nTargetPointIndex++;
//			m_bNewEventPoint = TRUE;
//			//		m_fDistanceToTarget = 0.0f;
//		}
	}

	if(m_bTarget == FALSE || m_nTargetPointIndex >= size)
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CCinema::InitWeaponCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode, int Speed, float Angle)
/// \brief		������� �ʱ� ����
/// \author		ispark
/// \date		2005-12-02 ~ 2005-12-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCinema::InitWeaponCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode, int Speed, float Angle)
{
	m_nWeaponSpeed = Speed;
	m_fWeaponAngle = Angle;

	InitCinemaData(i_StartEventPoint, bLHMode);
}
