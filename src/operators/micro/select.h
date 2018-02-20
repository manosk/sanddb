//
// Created by manolee on 11/2/17.
//

#ifndef SANDDB_SELECT_H_H
#define SANDDB_SELECT_H_H


#include <datum/array_vector.h>
#include "common/common.h"

template<typename T1, typename T2>
void select_gt_branching(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
    for(int i = 0; i < size; i++)   {
        if((*in)[i] > val) {
            selVector.set(i,true);
        }
    }
}

template<typename T1, typename T2>
void select_gt_pred(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
    for(int i = 0; i < size; i++)   {
        selVector.set(i,(*in)[i] > val);
    }
}

template<typename T1, typename T2>
void select_gt_pred(const ArrayVector<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
    for(int i = 0; i < size; i++)   {
        selVector.set(i,in.get(i) > val);
    }
}

//template<typename T1, typename T2>
//void select_lt_branching(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        if(in[i] < val) {
//            selVector.set(i,true);
//        }
//    }
//}
//
//template<typename T1, typename T2>
//void select_lt_pred(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        selVector.set(i,in[i] < val);
//    }
//}
//
//template<typename T1, typename T2>
//void select_ge_branching(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        if(in[i] >= val) {
//            selVector.set(i,true);
//        }
//    }
//}
//
//template<typename T1, typename T2>
//void select_ge_pred(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        selVector.set(i,in[i] >= val);
//    }
//}
//
//template<typename T1, typename T2>
//void select_le_branching(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        if(in[i] <= val) {
//            selVector.set(i,true);
//        }
//    }
//}
//
//template<typename T1, typename T2>
//void select_le_pred(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        selVector.set(i,in[i] <= val);
//    }
//}
//
//template<typename T1, typename T2>
//void select_eq_branching(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        if(in[i] == val) {
//            selVector.set(i,true);
//        }
//    }
//}
//
//template<typename T1, typename T2>
//void select_eq_pred(const std::unique_ptr<T1>& in, T2 val, int size, std::bitset<VECTORSIZE>& selVector)   {
//    for(int i = 0; i < size; i++)   {
//        selVector.set(i,in[i] == val);
//    }
//}

#endif //SANDDB_SELECT_H_H
