// $Id: bigint.cpp,v 1.61 2014-06-26 17:06:06-07 - - $
//Robert Ho rokho@ucsc.edu
//Daniel Urrutia deurruti@ucsc.edu
#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>
using namespace std;

#include "bigint.h"
#include "debug.h"

//fix this constructor??
bigint::bigint (long that): big_value (that) { 
   //cout<<"Starting"<<endl;
   DEBUGF ('~', this << " -> " << that) 
   string newlong = to_string(that);
   //cout << newlong << endl;
   auto itor = newlong.rbegin(); 
   bigint::digit_t newval;
   if(this->big_value.size() > 0){
      this->big_value.pop_back();
   }
   while (itor != newlong.rend()){
   newval = *itor - '0';
   this->big_value.push_back(newval);
   itor++;
   }
   if(that < 0){ 
         this->negative = true;
      }else{ 
         this->negative = false; 
   }
   
}

bigint::bigint (const string& that) {
   
   bool isnegative = false;
   auto itor = that.rbegin();
   while (itor != that.rend() ){ 
     if(*itor == '_'){
       isnegative = true; 
       break;
     }
     bigint::digit_t temp = *itor - '0';
     this->big_value.push_back(temp);
     itor++;
   }
   this->negative = isnegative; 
   DEBUGF ('~', this << " -> " << that)
}

bigint::bigint (const bigvalue_t& input, bool sign){
   big_value = input;
   negative = sign;
}


bigint operator+ (const bigint& left, const bigint& right) {
   // also check to see if left or right are negative
   //check to see if left is > than right if it is than give
   //the vector with the bigger size must allocate size + 1 
   //vector for result

   bigint::bigvalue_t sum;
   bigint total;
   bool sign;
   bool smaller;
   if(left.negative == right.negative){
      sum = total.do_bigadd(left.big_value, right.big_value);     
      sign = left.negative;
   }else if ( left.negative != right.negative){ 
      smaller = total.do_bigless(left.big_value, right.big_value);
      if(smaller == true){
         sign = right.negative;
         sum = total.do_bigsub(right.big_value,left.big_value);
      }else{
         sign = left.negative;
         sum = total.do_bigsub(left.big_value,right.big_value);
      }
     
   }
   auto sumend = sum.end() -1;
   bigint::digit_t zero = *sumend; 
   bigint::digit_t cmp = (int)0; 
   if(sum.size() == 1 && zero + '0' == cmp + '0'){ 
      sign = false;
   } 
   total = bigint(sum,sign);
   return total;
}

bigint operator- (const bigint& left, const bigint& right) {

   bigint::bigvalue_t diff;
   bigint total;
   bool sign;
   if(left.negative == true && right.negative == true){
      if(right < left){
         diff = total.do_bigsub(left.big_value, right.big_value);
         sign = !right.negative;
      }else if(left < right){
         diff = total.do_bigsub(right.big_value, left.big_value); 
         sign = left.negative;
      }else{
       diff = total.do_bigsub(left.big_value,right.big_value);
      }
   }else if(left.negative != right.negative){
      diff = total.do_bigadd(left.big_value, right.big_value);
      sign = left.negative;
   }else if (left.negative == false and right.negative == false){
      diff = total.do_bigsub(left.big_value, right.big_value);
      sign = left.negative;
      if(left < right){
         sign = !left.negative;
      }
   }

   auto diffend = diff.end() - 1;
   bigint::digit_t zero = *diffend;
   bigint::digit_t cmp = (int)0;
   if(diff.size() == 1 and zero+'0' == cmp + '0'){
      sign = false;
   }
   total = bigint(diff, sign);
   return total;
}
 
bigint operator+ (const bigint& right) { 
   bigint pos; 
   bigint::bigvalue_t r = right.big_value; 
   bool sign = false; 
   pos = bigint(r,sign); 
   return pos;
}

bigint operator- (const bigint& right) {
   bigint neg; 
   bigint::bigvalue_t r = right.big_value; 
   bool sign = true; 
   neg = bigint(r,sign); 
   return neg;
}

//fix this for power
long bigint::to_long() const {
    //if (*this <= bigint (numeric_limits<long>::min())
    //or *this > bigint (numeric_limits<long>::max()))
    // throw range_error ("bigint__to_long: out of range");
   
   long tmp = 0;
   long numval = 0;

    for(auto rit = this->big_value.crbegin();
            rit!=this->big_value.crend();++rit){
      numval = numval * 10 + *rit ;
     }
     tmp = this->negative ? -numval : + numval;
    return tmp;
}

bool abs_less (bigint::bigvalue_t &left,bigint::bigvalue_t &right) {
if(left.size() < right.size()){ 
      return true;
   }else if(right.size() < left.size()){
      return false;
   }else{
      bigint::digit_t numl;
      bigint::digit_t numr; 
      auto rit_right = right.crbegin();
      for (auto rit = left.crbegin(); rit != left.crend(); ++rit){
         numl = *rit;
         if(rit_right != right.crend()){ 
            numr = *rit_right;
            rit_right++;
         }
         if(numl + '0' < numr + '0'){ 
            return true;
         }else if(numl + '0' > numr + '0'){ 
            return false;
         }
      } 
   }
   return false; 
}

//
// Multiplication algorithm.
//
bigint operator* (const bigint& left, const bigint& right) {
   
   bigint::bigvalue_t product;
   bigint total;
   bool sign;

   if(left.negative == right.negative){ 
      sign = false;
      }else{
      sign = true;  
   }
   product = total.do_bigmul(left.big_value,right.big_value);
   total = bigint(product, sign);
   return total;
}

//
// Division algorithm.
//

void multiply_by_2 (bigint::bigvalue_t &tmp) {
   bigint total;
   tmp = total.do_bigadd(tmp, tmp); 
}

void divide_by_2 (bigint::bigvalue_t& tmp) {
 
 bigint:: digit_t div;  
 bigint:: digit_t carry =0;  
 bigint:: digit_t look; 

  bigint::bigvalue_t result;
  bigint::digit_t temp;
  auto nrit = tmp.begin() + 1;

  for (auto rit = tmp.begin(); rit != tmp.end(); ++rit){ 
      temp = *rit;
   
     if(nrit != tmp.end()){ 
        look = *nrit; 
        ++nrit;
     }else{
       look = 0;
     }

      carry = 0;  
     if(look%2 != 0){
        carry = 5; 
     }

    div = (temp / 2) + carry;
    result.push_back(div);
    }
  
 
  auto rit_total = result.begin(); 
  auto total_end = result.end()-1;
  
  bigint::digit_t zero = (int)0;


  for(;total_end != rit_total ;--total_end){
     bigint::digit_t endt = *total_end ;
  
    if(endt + '0'  == zero + '0'){ 
       result.pop_back();
    }else{
      break;
     }
  }    


    tmp = result;
  
}


bigint::quot_rem divide (const bigint& left, const bigint& right) {
   if (right == 0)throw domain_error ("divide by 0");
   //cout << "here" << endl;
   if (right == 0) throw domain_error ("bigint::divide");
   
   bigint::bigvalue_t remainder =left.big_value;
   bigint::bigvalue_t quotient; 
   bigint::bigvalue_t divisor = right.big_value;
   bigint::bigvalue_t power_of_2;
   bigint::digit_t one = 1; 
   power_of_2.push_back(one);
   bigint::bigvalue_t zero;
   bigint::digit_t z = 0; 
   zero.push_back(z);
   bool sign; 
   if(left.negative == right.negative){  
   sign = false; 
   }else{ 
    sign = true; 
   } 

   bigint total;

   while (abs_less (divisor, remainder)) {
        multiply_by_2 (divisor);
        multiply_by_2 (power_of_2);
   }


   while (abs_less (zero, power_of_2)) {
        if (not abs_less (remainder, divisor)) {
           remainder = total.do_bigsub(remainder,divisor);
           quotient = total.do_bigadd(quotient,power_of_2);
        }
        divide_by_2 (divisor);
        divide_by_2 (power_of_2);
   }
   bigint q = bigint(quotient, sign); 
   bigint r = bigint(remainder, sign);;
   return {q, r};
}

bigint operator/ (const bigint& left, const bigint& right) {
   bigint total;
   total = divide (left, right).first;
   return total;   
}

bigint operator% (const bigint& left, const bigint& right) {
   bigint total;
   total = divide (left, right).second;
   return total; 
}


bool operator== (const bigint& left, const bigint& right) {
   //cout<<"entering =="<<endl;
   if(left.negative != right.negative) return false; 
   if (left.big_value.size() != right.big_value.size()) return false; 
   bigint::digit_t numl;
   bigint::digit_t numr;
   auto rit_right = right.big_value.begin();
   auto right_end = right.big_value.end();
   for(auto rit_left = left.big_value.begin(); 
         rit_left != left.big_value.end();++rit_left){
      numl = *rit_left; 
      if(rit_right != right_end) {
      numr = *rit_right;
      rit_right++; 
      }
      if(numl != numr) return false; 
   }     

   return true;
}

bool operator< (const bigint& left, const bigint& right) {
   if(left.negative == true && right.negative == false ){
      return true;
   }
   if(left.negative == true and right.negative == true){
      if(left.big_value.size() > right.big_value.size()){
         return true;
      }else if(left.big_value.size() < right.big_value.size()){
         return false;
      } 
   }else if(left.negative == false and right.negative == false){
      if(left.big_value.size() > right.big_value.size()){
         return false;
      }else if(left.big_value.size() < right.big_value.size()){
         return true;
      } 
   }
   if(left.negative == false and right.negative == true){
      return false;
   }
   bigint::digit_t numl;
   bigint::digit_t numr;  
   int size = left.big_value.size(); 
   for (int i = size-1; i >= 0; --i){
   numl = left.big_value.at(i) + '0';
   numr = right.big_value.at(i) + '0'; 
      if(numl > numr){ 
         return false;
      } else if(numl < numr){ 
        return true;
      }
   }
   return false;
}

 ostream& operator<< (ostream& out, const bigint& that) {
  bigint::digit_t slash = '\\';
   
  bigint::digit_t temp;
  int charcount = 0;
  if(that.negative == true) cout << '-';
  for (auto rit = that.big_value.crbegin(); 
           rit != that.big_value.crend(); ++rit){
    temp = *rit + '0';
    charcount++;
    if(charcount == 70){
       out << slash << '\n';
       charcount = 1;
    }
    
    out << temp;
    
  }
   return out;
}



bigint &bigint::operator= (const bigint &that){
bigint tmp; 
return tmp;
} 


bigint pow (const bigint& base, const bigint& exponent) {
   DEBUGF ('^', "base = " << base << ", exponent = " << exponent);
   //cout<<"entering pow"<<endl;
   bigint zero = 0;
   if (base == 0){
     //cout<<"stuff"<<endl;
     return 0;
    }
   //cout<<"after first if"<<endl;
   bigint base_copy = base;
   long expt = exponent.to_long();
  bigint result = 1;
  if (expt < 0) {
      //base_copy = 1 / base_copy;
      //expt = - expt;
      return zero;
  }
  while (expt > 0) {
      if (expt & 1) { //odd
         result = result * base_copy;
         --expt;
      }else { //even
         base_copy = base_copy * base_copy;
         expt /= 2;
      }
   }

   DEBUGF ('^', "result = " << result);
   return result;
}
bigint::bigvalue_t bigint::do_bigadd(const bigvalue_t &left,
                                     const bigvalue_t &right){
  bigvalue_t total;

   auto  rit_left = left.begin();
   auto  left_end = left.end();
   auto  rit_right = right.begin();
   auto  right_end = right.end();
   
  if(left.size() >= right.size()){
         rit_left = left.begin();
         left_end = left.end();
         rit_right = right.begin();
         right_end = right.end();
   }else if(right.size() > left.size()){ 
         rit_right = left.begin();
         right_end = left.end();
         rit_left = right.begin();
         left_end = right.end();
   } 

        bigint::digit_t carry = 0;
  //iterating over the larger
      for (; rit_left != left_end; ++rit_left){
        bigint::digit_t numl = *rit_left;
        bigint::digit_t numr;

         //if theres more in right
         if(rit_right != right_end){
           
          numr = *rit_right;
         rit_right++;
         }else{
            numr = 0;
         }         
         //add the two
         bigint::digit_t sum = numl + numr + carry ; 
         carry = 0;
         if(sum >= 10){
           sum = sum - 10;
           carry = 1;
         }
         total.push_back(sum);

      }      
      if(carry > 0){
        total.push_back(carry);
      }
    
      return total;
   
}

bigint::bigvalue_t bigint::do_bigsub(const bigvalue_t &left,
                                     const bigvalue_t &right){
   
   bigvalue_t total;

   auto  rit_left = left.begin();
   auto  left_end = left.end();
   auto  rit_right = right.begin();
   auto  right_end = right.end();
   bigint::digit_t zero = (int)0;
 
   if(left.size() >= right.size()){ 
      rit_left = left.begin();
      left_end = left.end();
      rit_right = right.begin();
      right_end = right.end();
   }else if(right.size() > left.size()){
      rit_left = right.begin();
      left_end = right.end();
      rit_right = left.begin();
      right_end = left.end();   
   }
 
  bigint::digit_t carry = 0;
  for(;rit_left != left_end; ++rit_left){
      bigint::digit_t numl = *rit_left;
      bigint::digit_t numr; 
      
       if(rit_right != right_end){  
         numr = *rit_right;
         rit_right++;
         }else{
            numr = 0;
            
         }
       
       char diff = numl - numr + carry;
       carry = 0;
       if(diff < 0){ 
          diff = diff + 10;
          carry = -1;
       }
       
       total.push_back(diff);
       
      
    }
   //check for all zeroes and trim
  auto rit_total = total.begin(); 
  auto total_end = total.end()-1;
  

  for(;total_end != rit_total ;--total_end){
     bigint::digit_t endt = *total_end ;
     if(endt + '0'  == zero + '0'){ 
       total.pop_back();
     }else{
       break;
     }
  }

return total;
   
}


bigint::bigvalue_t bigint::do_bigmul(const bigvalue_t &left,
                                     const bigvalue_t &right){
   bigvalue_t total;
   int size = left.size() + right.size(); 
   total.resize(size);

   bigint::digit_t carry; 
   bigint::digit_t digit;
   bigint::digit_t zero = (int)0; 

   for(size_t i = 0; i < left.size(); ++i){ 
   carry = 0; 
      for(size_t j = 0; j < right.size(); ++j) {
          digit = total.at(i+j) + left.at(i)*right.at(j) + carry;
          total.at(i+j) = digit%10; 
          carry = digit/10; 
      }
   total.at(i+right.size()) = carry;
   } 

 auto rit_total = total.begin(); 
  auto total_end = total.end()-1;
  

  for(;total_end != rit_total ;--total_end){
     bigint::digit_t endt = *total_end ;
     if(endt + '0'  == zero + '0'){ 
       total.pop_back();
     }else{
       break;
     }
  }


return total; 
}

bool bigint::do_bigless(const bigvalue_t &left, 
                        const bigvalue_t &right){
   if(left.size() < right.size()){ 
      return true;
   }else if(right.size() < left.size()){
      return false;
   }else{
      int numl = 0;
      int numr = 0; 
      auto rit_right = right.crbegin();
      for (auto rit = left.crbegin(); rit != left.crend(); ++rit){
         numl = *rit - '0';
         if(rit_right != right.crend()){ 
            numr = *rit_right - '0';
            rit_right++;
         }
         if(numl < numr){ 
            return true;
         }else if(numl > numr){ 
            return false;
         }
      } 
   }
   return false; 
}
