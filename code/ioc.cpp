/**********************************************
  > File Name		: ioc.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 24 Oct 2020 09:40:40 PM CST
  > Location        : Shanghai
 **********************************************/
#include <map>
#include <functional>
#include <string>
#include <stdexcept>
using namespace std;

//write an IoC framwork with C++11
// class T should be the Base class of all the classes that get registerd.
template <class T>
class IoCContainer {
public:
    template <class Derived>
    void registerType(string className) {
        if (classMap.find(className) != classMap.end()) {
            throw std::invalid_argument("This class already exists");
        }
        std::function<T*()> creator = []{return new Derived();};
        classMap.emplace(className, creator);
    }

    T* produceClass(string className) {
        if (classMap.find(className) == classMap.end()) {
            return nullptr;
        }
        std::function<T*()> creator = classMap[className];
        return creator();
    }
private:
    map<string, std::function<T*()>> classMap;
};
    
int main() {
    // 测试IoCContainer

}
