
#include <Engine1.h>


/*
	trida pro hlavni instanci programu pod engine Engine1
*/
class IPS : public Engine1::Application {
private:


public:

	//IPS();
	//~IPS();


};


Engine1::Application* Engine1::CreateApplication() {
	return new Application();
}




