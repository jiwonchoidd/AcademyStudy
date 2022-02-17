#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"

ImVec4 login_text_color = ImVec4(1.0f,1.0f,1.0f,1.0f);
//WSAAsyncSelect
LRESULT  Sample::ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_Net.Select_MsgProc(hWnd, msg, wParam, lParam);
}
bool Sample::Init()
{
	#pragma region 네트워크 초기화
	auto size = static_cast<DWORD>((sizeof(char)*24)+1);
	if (!GetComputerNameA(computer_name, &size))
	{
		strcat(computer_name, "NoName");
	}

	m_Net.InitNetwork();
	//타입 포트 아이피
	if(m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, ip_Address))
	{
		m_bConnect = true;
	}
#pragma endregion

    m_Camera.CreateViewMatrix(KVector3(0, 0, -3), KVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	//플레이어 생성
	m_PlayerObj.SetRectSource({ 91,1,42,56 }); //소스 
	m_PlayerObj.SetRectDraw({0,0,42*2,56*2});
	m_PlayerObj.SetPosition(KVector2(400, 300));

	if(!m_PlayerObj.Init(m_pImmediateContext, 
		L"../../data/shader/vs_2D.txt",
		L"../../data/shader/ps_2D.txt",
		L"../../data/texture/bitmap1.bmp",
		L"../../data/texture/bitmap2.bmp"))
	{
		return false;
	}
	//NPC 생성
	for (int inpc = 0; inpc < 5; inpc++)
	{
		KNpc2D* npc = new KNpc2D;
		if (inpc % 2 == 0)
		{
			npc->SetRectSource({ 46,63,69,79 });
			npc->SetRectDraw({ 0,0, 69,79 });
		}
		else
		{
			npc->SetRectSource({ 1,63,42,76 });
			npc->SetRectDraw({ 0,0, 42,76 });
		}
		npc->SetPosition(KVector2(50 + inpc * 150, 50));
		if (!npc->Init(m_pImmediateContext,
			L"../../data/shader/vs_2D.txt",
			L"../../data/shader/ps_2D.txt",
			L"../../data/texture/bitmap1.bmp",
			L"../../data/texture/bitmap2.bmp"))
		{
			return false;
		}
		m_NpcLlist.push_back(npc);
	}

	box.Init(L"../../data/shader/vs_0.txt", L"../../data/shader/ps_0.txt",
		L"../../data/texture/uv.bmp");

    return true;
}
// 프레임 함수
bool Sample::Frame()
{
	#pragma region IMGUI INTERFACE
	(m_Net.m_bConnect)? str_isConnect = "Online" : str_isConnect = "Offline";
	(m_Net.m_bConnect)? login_text_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
						login_text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	if (ImGui::Begin("Chat"))
	{
		
		ImGui::TextColored(login_text_color,str_isConnect.c_str());
		//연결이 안되어 있으면 연결 재 시도
		if (!m_Net.m_bConnect)
		{
			ImGui::InputText("아이피 입력", ip_Address, sizeof(char) * 30);
			ImGui::SameLine();
			if (ImGui::Button(u8"연결 재시도"))
			{
				if (m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, ip_Address))
				{
					m_bConnect = true;
				}
			}
		}
		else
		{
			ImGui::BeginChild(u8"채팅창", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing() - 15));
			ImGui::Text(m_chatItems);
			ImGui::EndChild();
			ImGui::Dummy(ImVec2(0.0f, 5));
			ImGui::InputText("", m_buffer, sizeof(m_buffer));
			
			ImGui::SameLine();

			if (ImGui::Button("Send") || g_InputData.bEnter)
			{
				char clear[MAX_PATH] = { 0, };
				KPacket kpacket(PACKET_CHAT_MSG);

				//인덱스 이름 메시지
				kpacket << 10 << computer_name << m_buffer;

				//리턴 값이 0보다 작으면 전송되지 않았음
				if (m_Net.SendMsg(m_Net.m_Sock, kpacket.m_uPacket) < 0)
				{
					ZeroMemory(&m_chatItems, sizeof(char) * 2048);
					strcat(m_chatItems, "Error\n");
					m_Net.m_bConnect = false;
				}
				ImGui::SetKeyboardFocusHere(0);
				strcpy(m_buffer, clear);
			}
		}
	}
	ImGui::End();
#pragma endregion

	// 패킷 풀 사이즈로 채팅 수량 체크
	int iChatCnt = m_Net.m_User.m_lPacketPool.size();
	if (iChatCnt > 0 && m_iChatCount != iChatCnt)
	{
		m_iChatCount = iChatCnt;
		char clear[2048] = { 0, };
		strcpy(m_chatItems, clear);

		std::list<KPacket>::iterator iter;
		//대화양이 제한량을 넘으면 지워줌
		if (m_Net.m_User.m_lPacketPool.size() > 10)
		{
			m_Net.m_User.m_lPacketPool.pop_front();
		}
		for (iter = m_Net.m_User.m_lPacketPool.begin();
			iter != m_Net.m_User.m_lPacketPool.end();
			iter++)
		{
			
			KChatting recv_data;
			ZeroMemory(&recv_data, sizeof(recv_data));
			(*iter) >> recv_data.index >> recv_data.name
					>> recv_data.message;
			//strcat(m_chatItems, (char*)(*iter).m_uPacket.ph.time);
			strcat(m_chatItems, recv_data.name);
			strcat(m_chatItems, " : ");
			strcat(m_chatItems, recv_data.message);
			strcat(m_chatItems, "\n");
		
			(*iter).Reset();
		}
	}
	


	//플레이어 이동
	m_PlayerObj.Frame();

	//npc 이동
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		RECT rt = m_NpcLlist[iObj]->m_rtDraw;
		rt.right = rt.right + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		rt.bottom = rt.bottom + (cos(g_fSecTimer) * 0.5f + 0.5f) * 50.0f;
		m_NpcLlist[iObj]->UpdateRectDraw(rt);
		m_NpcLlist[iObj]->Frame();
	}

	box.Frame();
    return true;
}
bool Sample::Render()
{
	box.SetMatrix(nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
	box.Render(m_pImmediateContext);


	//npc 렌더링
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Render(m_pImmediateContext);
	}
	//플레이어 렌더링
	m_PlayerObj.Render(m_pImmediateContext);

    return true;
}
bool Sample::Release()
{
	m_PlayerObj.Release();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
	}
	box.Release();
	plane.Release();
    m_Camera.Release();
	m_Net.CloseNetwork();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(투명 텍스쳐, 960, 540);