#include <iostream>
using namespace std;

class Request {
  public :
    virtual ~Request() {} // -- make polymorphic type.
};
class HelpRequest : public Request {};
class PrintRequest : public Request {};

class Handler {
  public : 
    Handler(Handler* pObj) : pSuccessor_(pObj) {}
    virtual void HandleRequest(Request *pReq) {
      if (dynamic_cast<HelpRequest*>(pReq) != NULL) {
        // -- HelpRequest에 대해 처리
        cout << "HelpRequest Processing" << endl;
      }
      else if (dynamic_cast<PrintRequest*>(pReq) != NULL) {
        // -- PrintRequest에 대해 처리
        cout << "PrintRequest Processing" << endl;
      }
      else {
        // -- 기본적인 처리
        cout << "Default Processing" << endl;
      }
    }

  private :
    Handler* pSuccessor_;
};

void 
main()
{
  Handler hdlr(0);
  Request *pReq = new PrintRequest;

  hdlr.HandleRequest(pReq);
}
