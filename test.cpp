#include <iostream>
#include <map>
#include <algorithm>

class test
{
    public :
        test()
        {
            std::cout << "Calling default constructor" << std::endl;
        }
        test    &operator=(const test &copy)
        {
            std::cout << "Calling the copy overload operator" << std::endl;
            return (*this);
        }
};
int main()
{
    const test tt;
    std::map<int, const test> t;
    std::cout << "HOLA" << std::endl;
    t.insert(std::pair<int, const test>(0, tt));
    return(0);
}