/**********************************************
  > File Name		: any.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Sun 25 Oct 2020 10:12:21 AM CST
  > Location        : Shanghai
 **********************************************/
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>

/*
 * implement a Any class that can contain any type
 */

struct Any {
    Any(void): m_tpIndex(std::type_index(typeid(void))) {std::cout << "call default constructor" << std::endl;}
    Any(Any& that): m_ptr(that.clone()), m_tpIndex(that.m_tpIndex) {}
    Any(Any&& that): m_ptr(std::move(that.m_ptr)), m_tpIndex(that.m_tpIndex) {}

    // use std::decay to remove reference and cv modifier and get the original type
    // `typename =` equals to `typename T =`, as T is not used, so it can be omitted.
    template <typename U, typename = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> 
    Any(U&& value): m_ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(value))), m_tpIndex(std::type_index(typeid(typename std::decay<U>::type))) {
        std::cout << "call rvalue constructor" << std::endl;
    }

    bool isNull() const {
        return !bool(m_ptr);
    }

    template <class U> bool Is() const {
        return m_tpIndex == std::type_index(typeid(U));
    }

    // transform U into the actual type
    template <class U>
    U& AnyCast() {
        if (!Is<U>()) {
            std::cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << std::endl;
        }

        auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
        return derived->m_value;
    }

    Any& operator=(const Any& a) {
        std::cout << "call operator = " << std::endl;
        if (m_ptr == a.m_ptr) {
            return *this;
        }
        m_ptr = a.clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
    }

private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;

    struct Base {
        virtual ~Base() {}
        virtual BasePtr clone() const = 0;
    };

    template <typename T>
    struct Derived: Base {
        template <typename U>
        Derived(U&& value): m_value(std::forward<U>(value)) {}

        BasePtr clone() const {
            return BasePtr(new Derived<T>(this->m_value));
        }

        T m_value;
    };

    BasePtr clone() const {
        if (m_ptr != nullptr) {
            return m_ptr->clone();
        }
        return nullptr;
    }

    BasePtr m_ptr;
    std::type_index m_tpIndex;
};

int main() {
    Any n;
    auto r = n.isNull();
    std::cout << "auto r" << std::endl;
    double d = 1.3;
    n = d;
    std::cout << n.Is<double>() << std::endl;
    return 0;
}
