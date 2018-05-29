#ifndef PASS_CLASS
#define PASS_CLASS
#include <iostream>
#include <vector>
#include <string>

namespace pass_class
{
    class passClass
    {
        public:
            passClass();

        private:
            std::vector<std::string> qasm_representation_;
    };

} //namespace pass_class


#endif