#define IP_DD		"192.168.0.56"
#define IP_KGCA2	"192.168.0.12"
#define IP_1		"192.168.31.236"
#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"

static char chatItems[2048] = {0,};
static char buffer[MAX_PATH];
static char ip_Address[30] = IP_DD;
char computer_name[24] = { 0, };
std::string	isConnect = "DisConnected";
ImVec4 login_text_color = ImVec4(1.0f,1.0f,1.0f,1.0f);

LRESULT  Sample::ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_Net.Select_MsgProc(hWnd, msg, wParam, lParam);
}
bool Sample::Init()
{
	auto size = static_cast<DWORD>((sizeof(char)*24)+ 1);
	if (!GetComputerNameA(computer_name, &size))
	{
		strcat(computer_name, "NoName");
	}
    m_Camera.CreateViewMatrix(KVector3(0, 0, -5), KVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	m_Net.InitNetwork();
	//타입 포트 아이피
	if(m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, ip_Address))
	{
		m_bConnect = true;
	}
    return true;
}
// 프레임 함수
bool Sample::Frame()
{
	
	(m_Net.m_bConnect)? isConnect = "Online" : isConnect = "Offline";
	(m_Net.m_bConnect)? login_text_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
						login_text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	#pragma region IMGUI INTERFACE
	if (ImGui::Begin("Chat"))
	{
		
		ImGui::TextColored(login_text_color,isConnect.c_str());
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
			ImGui::Text(chatItems);
			ImGui::EndChild();
			ImGui::Dummy(ImVec2(0.0f, 5));
			ImGui::InputText("", buffer, sizeof(buffer));
			
			ImGui::SameLine();

			if (ImGui::Button("Send") || g_InputData.bEnter)
			{
				char clear[MAX_PATH] = { 0, };
				KPacket kpacket(PACKET_CHAT_MSG);

				//인덱스 이름 메시지
				kpacket << 10 << computer_name << buffer;

				//리턴 값이 0보다 작으면 전송되지 않았음
				if (m_Net.SendMsg(m_Net.m_Sock, kpacket.m_uPacket) < 0)
				{
					ZeroMemory(&chatItems, sizeof(char) * 2048);
					strcat(chatItems, "Error\n");
					m_Net.m_bConnect = false;
				}
				ImGui::SetKeyboardFocusHere(0);
				strcpy(buffer, clear);
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
		strcpy(chatItems, clear);

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
			//strcat(chatItems, (char*)(*iter).m_uPacket.ph.time);
			strcat(chatItems, recv_data.name);
			strcat(chatItems, " : ");
			strcat(chatItems, recv_data.message);
			strcat(chatItems, "\n");
		
			(*iter).Reset();
		}
	}
    return true;
}
bool Sample::Render()
{
    DrawTestBox(g_fSecTimer * m_Speed,
        0.0f, 0.0f, 0.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);

    return true;
}
bool Sample::Release()
{
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

WinMain_OPT(채팅창 클라이언트, 800, 600);