#ifndef D2_MODULE_H_
#define D2_MODULE_H_

#include <string>

class Module {
public:
	virtual ~Module() { ; }
	virtual int* Run() = 0;
};


#endif


