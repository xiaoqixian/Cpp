/**********************************************
  > File Name		: move.h
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Fri Apr 14 15:55:09 2023
  > Location        : Shanghai
  > Copyright@ https://github.com/xiaoqixian
 **********************************************/
#ifndef _MOVE_H
#define _MOVE_H

#include "../type_traits"

namespace evo {

/*
 * move语义擦除参数的引用类型，统一返回右值引用
 */
    template <typename T>
    inline typename evo::remove_reference<T>::type&& move(T tp) {
        typedef typename evo::remove_reference<T>::type&& r_reference;
        return static_cast<r_reference>(tp);
    }
}

#endif /* _MOVE_H*/
