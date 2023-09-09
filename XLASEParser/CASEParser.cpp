#include "CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Parse(const char* filepath)
{
	//Clear();
	Init();
	Load((LPSTR)filepath);

	return true;
}

bool CASEParser::Clear()	// TODO: ���ҽ� �Ŵ����� ���� �޸� ����
{
	////m_TokenString = { };
	//m_Token = 0;
	//m_parsingmode = 0;

	//m_data_asciiexport = 0;
	//m_scenedata = {};

	//m_materialcount = 0;
	//delete m_materialdata;
	//m_materialdata = nullptr;
	////delete m_list_materialdata;

	//delete m_OneMesh;
	//m_OneMesh = nullptr;
	////delete m_MeshList;

	//delete m_nowshapeline;
	//m_nowshapeline = nullptr;

	//delete m_nowshapevertex;
	//m_nowshapevertex = nullptr;

	//delete m_animation;
	//m_animation = nullptr;

	//delete m_list_animation;
	

	return true;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{


	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{

	/// ���ο� ���ؽ����� ���� + ������ �ֱ�
	// ���ؽ����� �ϴ� ��� ���� -> ???
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(_pVertex);
	}


	/// �ε��� �����
	// �׳� ����..
	
	for (auto&& face : pMesh->m_meshface)
	{
		int i = &face - &(pMesh->m_meshface[0]);
		
		pMesh->m_opt_index.emplace_back(pMesh->m_meshface[i]->m_vertexindex[0]);
		pMesh->m_opt_index.emplace_back(pMesh->m_meshface[i]->m_vertexindex[2]);
		pMesh->m_opt_index.emplace_back(pMesh->m_meshface[i]->m_vertexindex[1]);
	}

	// �������� face�� �������� �� �ε����� ã�ƾ� ��
	unsigned int _faceCount = pMesh->m_meshface.size();
	//TRACE("FaceCount : %d", _faceCount);


	/// ��� �ֱ�
	for (unsigned int i = 0; i < _faceCount; ++i)
	{
		//TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			int CurrentIndex = pMesh->m_meshface[i]->m_vertexindex[j];
			Vertex* CurrentVertex = pMesh->m_meshvertex[CurrentIndex];
			Vertex* NewVertex = pMesh->m_opt_vertex[CurrentIndex];

			// face�������� ���ؽ��� �븻�� �־��ش�.
			NewVertex->m_normal.x = CurrentVertex->m_normal.x;
			NewVertex->m_normal.y = CurrentVertex->m_normal.y;
			NewVertex->m_normal.z = CurrentVertex->m_normal.z;
		}
	}

	/// uv ��ǥ �ֱ�
	// Tface ������ŭ..
	for (unsigned int i = 0; i < pMesh->m_mesh_numtfaces; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			unsigned int CurrentTIndex = pMesh->m_meshface[i]->m_TFace[j];
			COneTVertex* CurrentTVertex = pMesh->m_mesh_tvertex[CurrentTIndex];

			// face �������� ���ؽ� �ε��� �˾Ƴ���?
			unsigned int CurrentIndex = pMesh->m_meshface[i]->m_vertexindex[j];

			pMesh->m_opt_vertex[CurrentIndex]->u = CurrentTVertex->m_u;
			pMesh->m_opt_vertex[CurrentIndex]->v = CurrentTVertex->m_v;

		}
	}



	return FALSE;
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

//----------------------------------------------------------------
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

		static int iv = 0;

		switch (nowtoken)
		{
			case TOKEND_BLOCK_START:

				Parsing_DivergeRecursiveALL(depth++);
				break;

			case TOKENR_HELPER_CLASS:
				break;

				//--------------------
				// 3DSMAX_ASCIIEXPORT
				//--------------------

			case TOKENR_3DSMAX_ASCIIEXPORT:
				m_data_asciiexport = Parsing_NumberLong();
				break;

				//--------------------
				// COMMENT
				//--------------------

			case TOKENR_COMMENT:
				Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
				//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
				break;

				//--------------------
				// SCENE
				//--------------------

			case TOKENR_SCENE:
				//
				TRACE(m_TokenString);
				break;
			case TOKENR_SCENE_FILENAME:
				m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
				break;
			case TOKENR_SCENE_FIRSTFRAME:
				m_scenedata.m_firstframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_LASTFRAME:
				m_scenedata.m_lastframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_FRAMESPEED:
				m_scenedata.m_framespeed = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_TICKSPERFRAME:
				m_scenedata.m_ticksperframe = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_MESHFRAMESTEP:
				m_scenedata.m_meshframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_KEYFRAMESTEP:
				m_scenedata.m_keyframestep = Parsing_NumberLong();
				break;
			case TOKENR_SCENE_BACKGROUND_STATIC:
				m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
				break;
			case TOKENR_SCENE_AMBIENT_STATIC:
				m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
				m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
				break;

			case TOKENR_SCENE_ENVMAP:
			{
				// �׳� ���� ������ �о������ }�� ���ö�����
				while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
				}
			}
			break;

			//--------------------
			// MATERIAL_LIST
			//--------------------


			//--------------------
			// GEOMOBJECT
			//--------------------

			case TOKENR_GROUP:
				//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
				break;

			case TOKENR_HELPEROBJECT:
				// �ϴ� �����ϰ�
				// ������Ʈ�� Ÿ�� ������. �̰Ϳ� ���� ���� �ٸ� �Ľ� ��� �ߵ�.
				break;

			case TOKENR_GEOMOBJECT:
				/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?
				Create_onemesh_to_list();
				break;

			case TOKENR_NODE_NAME:
				// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
				// ��忡 ���� �־�� �� ���� �ٸ���.
				/// ?? Node_Name�� Node_TM�� Node_Name �ߺ��ؼ� �о m_nodename�� �дµ�, ���� ���� �� �����ѵ�..
				m_OneMesh->m_nodename = Parsing_String();
				break;

			case TOKENR_NODE_PARENT:
				// �� ����� �θ� ����� ����.
				// �ϴ� �Է��� �ϰ�, ���߿� ��������.
				m_OneMesh->m_nodeparent = Parsing_String();
				break;

				/// NODE_TM

			case TOKENR_NODE_TM:
				m_parsingmode = eGeomobject;
				// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
				// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)
				/// ������: ��带 ���� �����Ѵٴ� ���� �� �ʹ��� �ʴ´�...

				/// �Դٰ�,
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
				//if (m_onemesh->m_camera_isloadTarget) {
				//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
				// ��ͱ����� �������̴�....

				///if(m_parsingmode == eGeomobject)
				///if(m_parsingmode == eCamera)

				/// ����. ������ ���� �̵�(?)
				/// �ƹ��͵� �а���� �ʾƵ� �׳� ����θ� �ȵȴ�(?)
				/// GetToken()�� ȣ������߸� ���� ��ū���� �Ѿ��(?)

				break;

			case TOKENR_INHERIT_POS:
				// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
				m_OneMesh->m_inherit_pos = Parsing_NumberVector3();
				break;
			case TOKENR_INHERIT_ROT:
				m_OneMesh->m_inherit_rot = Parsing_NumberVector3();
				break;
			case TOKENR_INHERIT_SCL:
				m_OneMesh->m_inherit_scl = Parsing_NumberVector3();
				break;
			case TOKENR_TM_ROW0:
				m_OneMesh->m_tm_row0 = Parsing_NumberVector3();
				break;
			case TOKENR_TM_ROW1:
				m_OneMesh->m_tm_row1 = Parsing_NumberVector3();
				break;
			case TOKENR_TM_ROW2:
				m_OneMesh->m_tm_row2 = Parsing_NumberVector3();
				break;
			case TOKENR_TM_ROW3:
				m_OneMesh->m_tm_row3 = Parsing_NumberVector3();
				break;
			case TOKENR_TM_POS:
				m_OneMesh->m_tm_pos = Parsing_NumberVector3();
				break;
			case TOKENR_TM_ROTAXIS:
				m_OneMesh->m_tm_rotaxis = Parsing_NumberVector3();
				break;
			case TOKENR_TM_ROTANGLE:
				m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
				break;
			case TOKENR_TM_SCALE:
				m_OneMesh->m_tm_scale = Parsing_NumberVector3();
				break;
			case TOKENR_TM_SCALEAXIS:
				m_OneMesh->m_tm_scaleaxis = Parsing_NumberVector3();
				break;
			case TOKENR_TM_SCALEAXISANG:
				m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
				// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
				break;


				/// MESH

			case TOKENR_MESH:
				// ������: �ƹ��͵� ���ص� ������ while�� ���ǽĿ��� GetToken()�� �Ѵ�.
				//Parsing_String();
				break;
			case TOKENR_TIMEVALUE:
				m_OneMesh->m_timevalue = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMBONE:
				// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
				// ���� �Է�
				break;
			case TOKENR_MESH_NUMSKINWEIGHT:
				break;
			case TOKENR_MESH_NUMVERTEX:
				m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
				break;
			case TOKENR_MESH_NUMFACES:
				m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
				break;

				/// MESH_VERTEX_LIST

			case TOKENR_MESH_VERTEX_LIST:
				break;
			case TOKENR_MESH_VERTEX:
				// ������ �Է�
				//Create_onevertex_to_list();
			{
				int tmpVertNo = Parsing_NumberInt();

				Create_onevertex_to_list();
				m_OneMesh->m_meshvertex[tmpVertNo]->m_pos = Parsing_NumberVector3();
			}

			break;

			/// Bone

			case TOKENR_SKIN_INITTM:
				break;
			case TOKENR_BONE_LIST:
				break;
			case TOKENR_BONE:
			{
				/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
				// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
				// Bone�� �ѹ��� �о� ����
			}
			break;
			//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
			case TOKENR_BONE_NAME:
			case TOKENR_BONE_PROPERTY:
				// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
				break;
				// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

			case TOKENR_MESH_WVERTEXS:
				break;

			case TOKENR_MESH_WEIGHT:
			{
				// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
			}
			break;
			case TOKENR_BONE_BLENGING_WEIGHT:
			{
				// ��ü �� �ܰ踦 ���°ž�...
				// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
				/// �� ��....
			}
			break;


			/// MESH_FACE_LIST
			case TOKENR_MESH_FACE_LIST:
				break;
			case TOKENR_MESH_FACE:
			{
				int tmpFaceNo = Parsing_NumberInt(); // Face No.
				int tmpVertexIdx[3]{ 0 };

				Parsing_String();
				tmpVertexIdx[0] = Parsing_NumberInt(); // Index A

				Parsing_String();
				tmpVertexIdx[1] = Parsing_NumberInt(); // Index B

				Parsing_String();
				tmpVertexIdx[2] = Parsing_NumberInt(); // Index C

				Parsing_String();	// AB
				Parsing_String();

				Parsing_String();	// BC
				Parsing_String();

				Parsing_String();	// CA
				Parsing_String();

				Parsing_String();	// Mesh Smoothing
				Parsing_String();

				Parsing_String();	// Mesh Material ID
				Parsing_String();


				Create_oneface_to_list();
				for (int i = 0; i < 3; ++i)
					m_OneMesh->m_meshface.at(tmpFaceNo)->m_vertexindex[i] = tmpVertexIdx[i];

			}
			break;


			case TOKENR_MESH_NUMTVERTEX:
				m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
				break;

				/// MESH_TVERTLIST

			case TOKENR_MESH_TVERTLIST:
				break;
			case TOKENR_MESH_TVERT:
			{
				for (int i = 0; i < m_OneMesh->m_mesh_numtvertex; ++i)
				{
					if (i != 0)
						Parsing_String(); // *MESH_TVERT ����. �� ó������ While�� ���ǽĿ��� �� �� �Ľ��ϹǷ� ����.
					Parsing_NumberInt(); // TVERT No. ����.

					Create_onetvertex_to_list();	// T���ؽ� �ϳ� ���� ��� ���Ϳ� push_back
					m_OneMesh->m_mesh_tvertex[i]->m_u = Parsing_NumberFloat();	// u �Ľ�
					m_OneMesh->m_mesh_tvertex[i]->m_v = Parsing_NumberFloat();	// v �Ľ�
					m_OneMesh->m_mesh_tvertex[i]->m_w = Parsing_NumberFloat();	// w �Ľ�
				}
				TRACE("\nTOKENR_MESH_TVERT END.");
			}
			break;
			case TOKENR_MESH_NUMTVFACES:
				m_OneMesh->m_mesh_numtfaces = Parsing_NumberInt();
				break;

				// MESH_TFACELIST
			case TOKENR_MESH_TFACELIST:
				break;
			case TOKENR_MESH_TFACE:
			{
				for (int i = 0; i < m_OneMesh->m_mesh_numtfaces; ++i)
				{
					if (i != 0)
						Parsing_String(); // *MESH_TVERT ����. �� ó������ While�� ���ǽĿ��� �� �� �Ľ��ϹǷ� ����.
					int tmpFaceNumber = Parsing_NumberInt(); // TFACE No.

					for (int j = 0; j < 3; ++j)
					{
						m_OneMesh->m_meshface[tmpFaceNumber]->m_TFace[j] = Parsing_NumberInt(); // TVertex�� index

					}
					
				}
			}
				break;

				// MESH_NORMALS
			case TOKENR_MESH_NORMALS:
				//Parsing_String();
				/// ����Լ� Ż���� ������ ã�Ҵ�... �� �� �ٷ� ���ؼ� ȣ��� ������ ¦�� �ȸ¾Ҵ� ��.. ���ȣ���� ���ؼ�..
				//TRACE(m_TokenString);
				break;
			case TOKENR_MESH_FACENORMAL:
				//TRACE(m_TokenString);
				break;
			case TOKENR_MESH_VERTEXNORMAL:
			{
				UINT tmpIndex = Parsing_NumberInt();
				Vector3 tmpNormal = Parsing_NumberVector3();


				m_OneMesh->m_meshvertex[tmpIndex]->m_normal = tmpNormal;

			}
			break;

			case TOKENR_PROP_MOTIONBLUR:
				//TRACE(m_TokenString);
				break;

			case TOKENR_PROP_CASTSHADOW:
				//TRACE(m_TokenString);
				break;
			case TOKENR_PROP_RECVSHADOW:
				//TRACE(m_TokenString);
				break;
			case TOKENR_MATERIAL_REF:
				//TRACE(m_TokenString);
				break;


			case TOKEND_END:
				// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

				//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
				TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
				return;

				break;

				/// ���� �ƹ��͵� �ش����� ������
			default:
				// �ƹ��͵� ���� �ʴ´�.
				break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3���� Float�� ���� �ϳ���
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

void CASEParser::Create_oneface_to_list()
{
	Face* temp = new Face;
	m_OneMesh->m_meshface.push_back(temp);
}

void CASEParser::Create_onetvertex_to_list()
{
	COneTVertex* temp = new COneTVertex;
	m_OneMesh->m_mesh_tvertex.push_back(temp);
}














