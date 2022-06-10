/**********************************************
  > File Name		: MessageBus.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 10 Jun 2022 09:37:20 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

/**
 * Message Bus:
 *  
 */

#include "any.hpp"
#include <map>
#include <string>
#include <functional>

//define function_traits to convert
//different function type into std::function type.
template <typename T>
struct function_traits;

//normal function
template <typename Ret, typename... Args>
struct function_traits<Ret(Args...)> {
public:
    enum { arity = sizeof...(Args) };
    typedef Ret function_type(Args...);
    typedef Ret return_type;
    typedef std::function<function_type> stl_function_type;
    typedef Ret(*function_pointer)(Args...);

    template <size_t I>
    struct args {
        static_assert(I < arity, "index is out of range");
        typedef typename std::tuple_element<I, std::tuple<Args...>>::type type;
    };
};

//function pointer
template <typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)>: function_traits<Ret(Args...)> {};

//stl function type
template <typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>: function_traits<Ret(Args...)> {};

//member function
#define FUNCTION_TAITS(...)\
    template <typename Ret, typename Class, typename... Args>\
    struct function_traits<Ret(Class::*)(Args...) __VA_ARGS__>: function_traits<Ret(Args...)> {};\

FUNCTION_TAITS()
FUNCTION_TAITS(const)
FUNCTION_TAITS(volatile)
FUNCTION_TAITS(const volatile)

template <typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function& lambda) {
    return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}

template <typename Function>
typename function_traits<Function>::stl_function_type to_function(Function&& lambda) {
    return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

template <typename Function>
typename function_traits<Function>::function_pointer to_function_pointer(const Function& lambda) {
    return static_cast<typename function_traits<Function>::function_pointer>(lambda);
}

class MessageBus {
    std::multimap<std::string, Any> bus;

    template <typename F>
    void add(const std::string& topic, F&& f) {
        std::string msgType = topic + typeid(F).name();
        std::cout << "attach " << msgType << std::endl;
        this->bus.emplace(std::move(msgType), std::forward<F>(f));
    }
    
public:
    MessageBus(const MessageBus&) = delete;
    MessageBus& operator = (const MessageBus&) = delete;
    MessageBus() = default;

    //attach callable objects
    template <typename F>
    void attach(const std::string& topic, const F& f) {
        this->add(topic, std::move(to_function(std::forward<F>(f))));
    }

    //attach member function
    template <typename C, typename... Args, typename P>
    void attach(const std::string& topic, void(C::*f)(Args...) const, const P&& p) {
        std::function<void(Args...)> func = [&p, f](Args... args) {
            return (*p.*f)(std::forward<Args>(args)...);
        };
        this->add(topic, std::move(func));
    }

    //template <typename Ret>
    //void send(const std::string& topic = "") {
        //typedef std::function<Ret()> function_type;
        //std::string msgType = topic + typeid(function_type).name();

        //auto range = this->bus.equal_range(msgType);
        //for (auto it = range.first; it != range.second; it++) {
            //auto f = it->second.AnyCast<function_type>();
            //f();
        //}
    //}

    //send request to objects
    template <typename Ret, typename... Args>
    void send(Args&&... args, const std::string& topic = "") {
        typedef std::function<Ret(Args...)> function_type;
        std::string msgType = topic + typeid(function_type).name();
        std::cout << "send " << msgType << std::endl;
        
        auto range = this->bus.equal_range(msgType);
        for (auto it = range.first; it != range.second; it++) {
            auto f = it->second.AnyCast<function_type>();
            f(std::forward<Args>(args)...);
        }
    }

    template <typename Ret, typename... Args>
    void remove(const std::string& topic = "") {
        typedef std::function<Ret(Args...)> function_type;
        
        std::string msgType = topic + typeid(function_type).name();
        auto range = this->bus.equal_range(msgType);
        this->bus.erase(range.first, range.second);
    }
};

#include <iostream>
void sendHello(int) {
    std::cout << "Hello" << std::endl;
}

void sendHell(int) {
    std::cout << "Hell" << std::endl;
}

int main() {
    MessageBus mb;
    mb.attach("send", sendHello);
    mb.attach("send", sendHell);

    const std::string topic = "send";
    mb.send<void, int>(1, topic);
}
