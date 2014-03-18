template <int cond, typename A, typename B>
struct enable_if{
  typedef B type;
};

template <typename A, typename B>
struct enable_if<1, A, B>{
  typedef A type;
};


template <int A, int B>
struct GCD{
  enum {
    value = GCD<B, A % B> :: value
  };
};

template <int A>
struct GCD<A, 0>{
  enum {
    value = A
  };
};


template <int N>
struct Number{
  typedef Number<N> type;
  enum { value = N };
};

struct NilType{

};

template <typename Range, typename Func, typename Init>
struct Sigma{
  typedef typename Range::Next::type a;
  typedef typename Func::template apply<Init, typename Range::Now::type>::type b;
  typedef typename Sigma<a, Func, b>::type type;
};

template <typename Func, typename Init>
struct Sigma<NilType, Func, Init>{
  typedef Init type;
};


template <int A, int B, int K = 1>
struct Range{
  struct Next{
     typedef typename enable_if< (A+K>=B), NilType, Range<A+K, B> >::type type;
  };
  struct Now{
     typedef Number<A> type;
  };
};


template <int A, int B>
struct Fraction{
  enum{
    a  = A,
    b  = B,
  };
  enum { _gcd = GCD<a, b>::value };

  struct Reduced{
    typedef typename ::Fraction < a / _gcd, b / _gcd > type;
  };

  template <typename F>
  struct Add {
    typedef typename ::Fraction< a * F::b + b * F::a , b * F::b >::Reduced::type type;
  };
  template <typename F>
  struct Sub {
    typedef typename ::Fraction< a * F::b - b * F::a , b * F::b >::Reduced::type type;
  };
  template <typename F>
  struct Mul {
    typedef typename ::Fraction< a * F::a, b * F::b >::Reduced::type type;
  };
  template <typename F>
  struct MulNumber {
    typedef typename ::Fraction< a * F::value, b>::Reduced::type type;
  };
  template <typename F>
  struct Div {
    typedef typename ::Fraction< a * F::b, b * F::a >::Reduced::type type;
  };
  template <typename F>
  struct DivNumber {
    typedef typename ::Fraction< a, b * F::value >::Reduced::type type;
  };

  template <int N>
  struct Power{
    struct Mul{
      template <typename Aa, typename Bb>
      struct apply{
        typedef typename Aa::template Mul<typename Reduced::type>::type::Reduced::type type;
      };
    };

    typedef typename Sigma< ::Range<0, N>, Mul, ::Fraction<1, 1> >::type type;
    
  };
};

template <int N>
struct Fac{
  enum { value = Fac<N-1>::value * N };
};

template <>
struct Fac<0>{
  enum { value = 1 };
};




template <typename Frac> 
struct Sine{
  template <int N>
    struct Sign{
     enum {
      value = N % 2 == 0 ? 0 :
        N % 4 == 1 ? 1 :
         -1
    };
  };
  struct Func{
    template <typename Acc, typename ItemNumber>
     struct apply{
        typedef typename Acc::template Add< 
           typename Frac::template Power<ItemNumber::value>::type::
                 template DivNumber< Fac< ItemNumber::value > >::type::
                 template MulNumber< Sign< ItemNumber::value > >::type
        >::type type;
     };
  };
  typedef typename Sigma< Range<1, 11, 2>, Func, ::Fraction<0, 1> >::type type;
};



#include <cstdio>
int main(){
  typedef Sine< Fraction<1, 1> >  ::type type;
  printf("%d %d\n", type::a, type::b);
}
