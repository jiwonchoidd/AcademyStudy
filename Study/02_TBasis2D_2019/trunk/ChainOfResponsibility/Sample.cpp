//chain of responsibility는 책임들이 연결되어 있어 내가 책임을 못 질 것 같으면 
//다음 책임자에게 자동으로 넘어가는 구조를 말한다.( 전화중계 )
#include <iostream>
#include <string>
using namespace std;

class HelpHandler 
{
public:
	HelpHandler(HelpHandler* pObj = 0, string helpMsg = "") 
	{
		pSuccessor_ = pObj; 
		helpMsg_ = helpMsg;
	}
	virtual void SetHandler(HelpHandler* pObj, string helpMsg) 
	{
		pSuccessor_ = pObj; 
		helpMsg_ = helpMsg;
	}
	virtual bool HasHelp() { return !helpMsg_.empty(); }
	virtual void HandleHelp() 
	{
		if (pSuccessor_) 
			pSuccessor_->HandleHelp();
	}
	virtual void ShowHelpMsg() 
	{
		// -- helpMsg_를 보여준다.
		cout << helpMsg_ << endl;
	}
protected:
	string helpMsg_;
private:
	HelpHandler * pSuccessor_;
};

class Widget : public HelpHandler
{
protected:
	Widget(Widget* pObj, string helpMsg = "") : HelpHandler(pObj, helpMsg) 
	{
		pParent_ = pObj;
	}
private:
	Widget * pParent_;
};

class Button : public Widget 
{
public:
	Button(Widget* pObj, string helpMsg = "") : Widget(pObj, helpMsg) {}
	virtual void HandleHelp() 
	{
		if (HasHelp()) ShowHelpMsg();
		else HelpHandler::HandleHelp();
	}
	virtual void ShowHelpMsg()
	{
		// -- helpMsg_ 내용을 Button 도움말 양식에 맞추어 보여준다.
		cout << helpMsg_ << endl;
	}
};

class Dialog : public Widget 
{
public:
	Dialog(HelpHandler *pObj, string helpMsg = "") : Widget(0) 
	{
		SetHandler(pObj, helpMsg);
	}
	virtual void HandleHelp() 
	{
		if (HasHelp()) ShowHelpMsg();
		else HelpHandler::HandleHelp();
	}
	virtual void ShowHelpMsg() 
	{
		// -- helpMsg_ 내용을 Dialog 도움말 양식에 맞추어 보여준다.
		cout << helpMsg_ << endl;
	}
};

class Application : public HelpHandler 
{
public:
	Application(string helpMsg) : HelpHandler(0, helpMsg) {}
	virtual void HandleHelp() 
	{
		ShowHelpMsg();
	}
	virtual void ShowHelpMsg() 
	{
		// -- helpMsg_ 내용을 보여준다.
		cout << helpMsg_ << endl;
	}
};

void main()
{
	Application *pApp = new Application("Application Help");
	Dialog* pDialog = new Dialog(pApp, "Dialog Help");
	Button* pButton = new Button(pDialog);

	pButton->HandleHelp();
}


//class Handler {
//public:
//	Handler(Handler* pObj) : pSuccessor_(pObj) {}
//	virtual void HandleRequest(Request *pReq) {
//		if (dynamic_cast<HelpRequest*>(pReq) != NULL) {
//			// -- HelpRequest에 대해 처리
//			cout << "HelpRequest Processing" << endl;
//		}
//		else if (dynamic_cast<PrintRequest*>(pReq) != NULL) {
//			// -- PrintRequest에 대해 처리
//			cout << "PrintRequest Processing" << endl;
//		}
//		else {
//			// -- 기본적인 처리
//			cout << "Default Processing" << endl;
//		}
//	}
//
//private:
//	Handler * pSuccessor_;
//};