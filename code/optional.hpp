/**********************************************
  > File Name		: optional.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Thu 02 Jun 2022 09:21:08 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <type_traits>
#include <utility>
#include <stdexcept>

template<typename T>
class Optional {
    using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
private:
    bool m_hasInit = false;
    data_t m_data;

    template<typename... Args>
    void create(Args&&... args) {
        new (&this->m_data) T(std::forward<Args>(args)...);
        //this->m_data = (data_t*)(new T(std::forward<Args>(args)...));
        this->m_hasInit = true;
    }

    void destory() {
        if (this->m_hasInit) {
            this->m_hasInit = false;
            ((T*)(&this->m_data))->~T();
        }
    }

    void assign(const Optional& other) {
        if (other.isInit()) {
            copy(other.m_data);
            this->m_hasInit = true;
        }
    }

    void copy(const data_t& val) {
        destory();
        new (&this->m_data) T(*((T*)(&val)));
    }

public:
    Optional() {}

    Optional(const T& v) {
        create(v);
    }

    ~Optional() {
        destory();
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        destory();
        create(std::forward<Args>(args)...);
    }

    bool isInit() {return this->m_hasInit;}

    //overload bool operator
    explicit operator bool() const {
        return this->isInit();
    }

    T const& operator*() const {
        if (this->isInit()) {
            return *((T*)(&this->m_data));
        }

        throw std::logic_error("is not init");
    }
};
