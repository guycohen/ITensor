//
// Distributed under the ITensor Library License, Version 1.1.
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_HAMBUILDER_H
#define __ITENSOR_HAMBUILDER_H
#include "mpo.h"

#define String std::string

namespace itensor {

//
//
// HamBuilder
//
// Class for creating product-operator MPOs,
// usually to be combined into a more complex
// MPO such as a Hamiltonian.
//

template <class Tensor>
class HamBuilder
    {
    public:

    HamBuilder() { };

    HamBuilder(const Model& mod);

    HamBuilder(const Model& mod,
               const String& opname1, int j1,
               const String& opname2 = "", int j2 = 0,
               const String& opname3 = "", int j3 = 0,
               const String& opname4 = "", int j4 = 0);

    HamBuilder(const Model& mod,
               const Tensor& op1, int j1,
               const Tensor& op2 = Tensor(), int j2 = 0,
               const Tensor& op3 = Tensor(), int j3 = 0,
               const Tensor& op4 = Tensor(), int j4 = 0);

    HamBuilder&
    set(const String& opname1, int j1,
        const String& opname2 = "", int j2 = 0,
        const String& opname3 = "", int j3 = 0,
        const String& opname4 = "", int j4 = 0);

    HamBuilder&
    set(const Tensor& op1, int j1,
        const Tensor& op2 = Tensor(), int j2 = 0,
        const Tensor& op3 = Tensor(), int j3 = 0,
        const Tensor& op4 = Tensor(), int j4 = 0);

    operator MPOt<Tensor>() const { putlinks_(); return W_; }

    HamBuilder&
    operator*=(Real val) { W_ *= val; return *this; }

    HamBuilder&
    operator*=(Complex z) { W_ *= z; return *this; }

    private:

    /////////////////
    //
    // Data Members

    const Model* mod_;
    mutable MPOt<Tensor> W_;
    mutable bool initted_;

    //
    /////////////////

    void
    putlinks_() const;

    void
    setident_() const;

    static int
    hamNumber()
        {
        static int num_ = 0;
        ++num_;
        return num_;
        }

    };

template <class Tensor> 
HamBuilder<Tensor>
operator*(HamBuilder<Tensor> hb, Real x)
    {
    hb *= x;
    return hb;
    }

template <class Tensor> 
HamBuilder<Tensor>
operator*(Real x, HamBuilder<Tensor> hb)
    {
    hb *= x;
    return hb;
    }

template <class Tensor> 
HamBuilder<Tensor>
operator*(HamBuilder<Tensor> hb, Complex z)
    {
    hb *= z;
    return hb;
    }

template <class Tensor> 
HamBuilder<Tensor>
operator*(Complex z, HamBuilder<Tensor> hb)
    {
    hb *= z;
    return hb;
    }

template <class Tensor> 
HamBuilder<Tensor>::
HamBuilder(const Model& mod)
    :
    mod_(&mod),
    W_(mod),
    initted_(false)
    { 
    setident_();
    }

template <class Tensor> 
HamBuilder<Tensor>::
HamBuilder(const Model& mod,
           const String& opname1, int j1,
           const String& opname2, int j2,
           const String& opname3, int j3,
           const String& opname4, int j4)
    :
    mod_(&mod),
    W_(mod),
    initted_(false)
    { 
    setident_();
    set(opname1,j1,opname2,j2,opname3,j3,opname4,j4);
    }

template <class Tensor>
HamBuilder<Tensor>::
HamBuilder(const Model& mod,
           const Tensor& op1, int j1,
           const Tensor& op2, int j2,
           const Tensor& op3, int j3,
           const Tensor& op4, int j4)
    :
    mod_(&mod),
    W_(mod),
    initted_(false)
    { 
    setident_();
    set(op1,j1,op2,j2,op3,j3,op4,j4);
    }

template <class Tensor> 
HamBuilder<Tensor>& HamBuilder<Tensor>::
set(const String& opname1, int j1,
    const String& opname2, int j2,
    const String& opname3, int j3,
    const String& opname4, int j4)
    {
    if(initted_)
        {
        Error("Cannot set additional site operators once MPO has been retrieved from HamBuilder.");
        }
    W_.Anc(j1) = mod_->op(opname1,j1);
    if(j2 != 0)
        W_.Anc(j2) = mod_->op(opname2,j2);
    if(j3 != 0)
        W_.Anc(j3) = mod_->op(opname3,j3);
    if(j4 != 0)
        W_.Anc(j4) = mod_->op(opname4,j4);
    return *this;
    }

template <class Tensor> 
HamBuilder<Tensor>& HamBuilder<Tensor>::
set(const Tensor& op1, int j1,
    const Tensor& op2, int j2,
    const Tensor& op3, int j3,
    const Tensor& op4, int j4)
    {
    if(initted_)
        {
        Error("Cannot set additional site operators once MPO has been retrieved from HamBuilder.");
        }
    W_.Anc(j1) = op1;
    if(j2 != 0)
        W_.Anc(j2) = op2;
    if(j3 != 0)
        W_.Anc(j3) = op3;
    if(j4 != 0)
        W_.Anc(j4) = op4;
    return *this;
    }

template <class Tensor> 
void HamBuilder<Tensor>::
putlinks_() const
    {
    if(initted_) return;
    putMPOLinks(W_);
    initted_ = true;
    }

template <class Tensor> 
void HamBuilder<Tensor>::
setident_() const
    {
    for(int j = 1; j <= mod_->N(); ++j)
        W_.Anc(j) = mod_->op("Id",j);
    }

}; //namespace itensor

#undef String

#endif
