#include "VecN.h"

VecN::VecN():N(0), data(nullptr){}

VecN::VecN(int N):N(N){
    data = new float[N];
}

VecN::VecN(const VecN& v){
    N = v.N;
    data = new float[N];
    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
}

VecN::~VecN(){
    delete[] data;
}

void VecN::Zero(){
    for (int i = 0; i < N; i++)
        data[i] = 0.0f;
}

float VecN::Dot(const VecN& v) const{
    float sum = 0.0f;
    for (int i = 0; i < N; i++)
        sum += data[i] * v.data[i];
    return sum;
}