/**********************************************
  > File Name		: ioc.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sat 24 Oct 2020 09:40:40 PM CST
  > Location        : Shanghai
 **********************************************/
#include <unordered_map>
#include <functional>
#include <string>
#include <stdexcept>
#include "any.hpp"
#include <memory>
using namespace std;


//write an IoC framwork with C++11
/*
 * 这个IoC容器可以创建所有类型的接口对象和普通的对象
 */
struct IoCContainer {
    IoCContainer(void) {}
    ~IoCContainer(void) {}

    //注入继承了同一接口的类型
    //T表示包含了接口类的一个类型
    //下面两个通过enable_if进行RegisterType的重载是老套路了
    template <typename T, typename Depend, typename... Args>
    typename std::enable_if<!std::is_base_of<T, Depend>::value>::type RegisterType(const string& className) {
        std::function<T*(Args...)> function = [](Args... args){return new T(new Depend(args...));};
        RegisterType(className, function);
    }

    template <typename T, typename Depend, typename... Args>
    typename std::enable_if<std::is_base_of<T, Depend>::value>::type RegisterType(const string& className) {
        std::function<T*(Args...)> function = [](Args... args){return new Depend(args...);};
        RegisterType(className, function);
    }

    template <typename T, typename... Args>
    void RegisterType(const string& className) {
        std::function<T*(Args...)> function = [](Args... args){return new T(args...);};
        RegisterType(className, function);
    }

    template <class T, typename... Args>
    T* Resolve(const string& className, Args... args) {
        auto it = m_creatorMap.find(className);
        if (it == m_creatorMap.end()) {
            return nullptr;
        }
        Any resolver = it->second;
        std::function<T*(Args...)> function = resolver.AnyCast<std::function<T*(Args...)>>();
        return function(args...);
    }

    //与Resolve作用类似，但是返回值通过shared_ptr包裹，符合RAII原则
    template <class T, typename... Args>
    std::shared_ptr<T> ResolveShared(const string& className, Args... args) {
        T* t = Resolve<T>(className, args...);
        return std::shared_ptr<T>(t);
    }

private:
    void RegisterType(const string& className, Any constructor) {
        if (m_creatorMap.find(className) != m_creatorMap.end()) {
            std::cout << "This class already exists" << std::endl;
            return ;
        }
        m_creatorMap.emplace(className, constructor);
    }

    unordered_map<string, Any> m_creatorMap;
};

struct Bus {
    void test() const {
        cout << "Bus::test()" << endl;
    }
};

struct Car {
    void test() const {
        cout << "Car::test()" << endl;
    }
};

struct Base {
    virtual void func() {}
    virtual ~Base() {}
};

struct DerivedB: Base {
    void func() override {
        cout << "call DerivedB::func()" << endl;
    }
};

struct DerivedC: Base {
    void func() override {
        cout << "call DerivedC::func()" << endl;
    }
};

// 通过A的闭包来擦除类型
struct A {
    A(Base* ptr): m_ptr(ptr) {}
    void func() {
        m_ptr->func();
    }

    ~A() {
        if (m_ptr != nullptr) {
            delete m_ptr;
            m_ptr = nullptr;
        }
    }

private:
    Base* m_ptr;
};

int main() {
    // 测试IoCContainer
    IoCContainer ioc;
    // 配置依赖关系
    ioc.RegisterType<A, DerivedB>("B");
    auto pB = ioc.ResolveShared<A>("B");
    pB->func();
    return 0;
}
