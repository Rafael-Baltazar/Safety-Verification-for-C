#if !defined(verVariable_P)
#define verVariable_P

#include <string>

namespace ver {

class Variable {
	private:
		std::string _name;

	public:
		Variable(const std::string& name) { _name = name; }
		~Variable() { _name.clear(); };
		std::string getName() { return _name; }
		void setName(const std::string& name) { _name = name; }
};

}

#endif /* varVariable_P */
