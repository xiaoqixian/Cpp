/**********************************************
  > File Name		: variant.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri 03 Jun 2022 10:00:51 PM CST
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/

#include <type_traits>
#include <typeindex>
#include <utility>
#include <iostream>

// select max aligned type size
template <std::size_t arg, std::size_t... rest>
struct IntegerMax;

template <std::size_t arg>
struct IntegerMax<arg>: std::integral_constant<std::size_t, arg> {};

template <std::size_t arg1, std::size_t arg2, std::size_t... rest>
struct IntegerMax<arg1, arg2, rest...>: std::integral_constant<std::size_t, arg1 <= arg2 ? 
    IntegerMax<arg2, rest...>::value : IntegerMax<arg1, rest...>::value> {};

template <typename... TypeList>
struct MaxAlign: std::integral_constant<std::size_t, IntegerMax<std::alignment_of<TypeList>::value...>::value> {};

// if List contains type T
template <typename T, typename... List>
struct Contains;

template <typename T, typename Head, typename... Rest>
struct Contains<T, Head, Rest...>: std::conditional<std::is_same<T, Head>::value, std::true_type, Contains<T, Rest...>>::type {};

//end recursion
template <typename T>
struct Contains<T>: std::false_type {};

template <typename... Types>
struct Variant {
    enum {
        data_size = IntegerMax<sizeof(Types)...>::value,
        align_size = MaxAlign<Types...>::value
    };

    using data_t = typename std::aligned_storage<data_size, align_size>::type;
private:
    data_t _data;
    std::type_index _type_index;

    template <typename T>
    void destory_as(const std::type_index& id, void* data) {
        if (id == std::type_index(typeid(T))) {
            reinterpret_cast<T*>(data)->~T();
        }
    }

    void destory(const std::type_index& id, void* buffer) {
        [](Types&&...){}((destory_as<Types>(id, buffer), 0)...);
    }

    template <typename T>
    void move_as(const std::type_index& id, void* old_v, void* new_v) {
        if (id == std::type_index(typeid(T))) {
            new (new_v)T(std::move(*reinterpret_cast<T*>(old_v)));
        }
    }

    void move(const std::type_index& id, void* old_v, void* new_v) {
        [](Types&&...){}((move_as<Types>(id, old_v, new_v), 0)...);
    }

    template <typename T>
    void copy_as(const std::type_index& id, const void* old_v, void* new_v) {
        if (id == std::type_index(typeid(T))) {
            new (new_v)T(reinterpret_cast<T*>(old_v));
        }
    }

    void copy(const std::type_index& id, const void* old_v, void* new_v) {
        [](Types...){}((copy_as<Types>(id, old_v, new_v), 0)...);
    }

public:
    Variant(void): _type_index(std::type_index(typeid(void))) {}

    ~Variant() {
        this->destory(this->_type_index, &this->_data);
    }

    Variant(Variant<Types...>&& old): _type_index(old._type_index) {
        this->move(old._type_index, &old._data, &this->_data);
    }

    Variant(Variant<Types...>& old): _type_index(old._type_index) {
        this->copy(old._type_index, &old._data, &this->_data);
    }

    Variant& operator=(const Variant& old) {
        this->copy(old._type_index, &old._data, &this->_data);
        this->_type_index = old._type_index;
        return *this;
    }

    Variant& operator=(Variant&& old) {
        this->move(old._type_index, &old._data, &this->_data);
        this->_type_index = old._type_index;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<Contains<typename std::decay<T>::type, Types...>::value>::type>
    Variant(T&& value): _type_index(typeid(void)) {
        std::cout << "reassignd to type " << typeid(T).name()  << std::endl;
        this->destory(this->_type_index, &this->_data);
        typedef typename std::decay<T>::type U;
        new (&this->_data)U(std::forward<T>(value));
        this->_type_index = std::type_index(typeid(U));
    }

    template <typename T>
    bool is() const {
        return this->_type_index == std::type_index(typeid(T));
    }

    bool empty() const {
        return this->_type_index == std::type_index(typeid(void));
    }

    std::type_index type() const {
        return this->_type_index;
    }

    template <typename T>
    typename std::decay<T>::type get() {
        using U = typename std::decay<T>::type;
        if (!this->is<U>()) {
            std::cout << typeid(U).name() << " is not defined. current type is"
                << this->_type_index.name() << std::endl;
            throw std::bad_cast{};
        }

        return *(U*)(&this->_data);
    }
};

int main() {
    typedef Variant<int, double, char> vt;
    vt v = 1;
    v = 12.34;

    //int i = v.get<int>(); // throw bad_cast
    double d = v.get<double>();
    std::cout << d << std::endl;
}
