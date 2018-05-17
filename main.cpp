//
//  main.cpp
//  ASP_Special_Caculator
//
//  Created by yansu on 2018/5/18.
//  Copyright © 2018 yansu. All rights reserved.
//

#include <iostream>
#define MAX_LEN 2001
class CBigInt
{
    CBigInt(unsigned int v = 0, unsigned int sign = 0, unsigned int length = 1, unsigned int mode = 10)
    {
        m_Sign = sign;
        m_nLength = length;
        m_nMode = mode;
        m_uValue[0] = v;
    }
    unsigned int m_Sign;
    unsigned int m_nLength;
    unsigned int m_nMode;
    unsigned int m_uValue[MAX_LEN];
    
    void Add(CBigInt& value1, CBigInt& value2, CBigInt& result);
    void Sub(CBigInt& value1, CBigInt& value2, CBigInt& result);
    void Mul(CBigInt& value1, CBigInt& value2, CBigInt& result);
    void Div(CBigInt& value1, CBigInt& value2, CBigInt& quotient, CBigInt& remainder);
    CBigInt operator+(CBigInt& value);
    CBigInt operator-(CBigInt& value);
    int CompareNoSign(CBigInt& value);

};

int CBigInt::CompareNoSign(CBigInt &value)
{
    if(m_nLength>value.m_nLength)
    {
      return 1;
    }
    else if (m_nLength<value.m_nLength)
    {
        return -1;
    }
    
    if(m_uValue[m_nLength-1]>value.m_uValue[m_nLength-1])
    {
        return 1;
    }
    else if (m_uValue[m_nLength-1]<value.m_uValue[m_nLength-1])
    {
        return -1;
    }
    return 0;
}

void CBigInt::Add(CBigInt& value1, CBigInt& value2, CBigInt& result)
{
    result = value1;
    
    unsigned int carry = 0;
    if(result.m_nLength<value2.m_nLength)
    {
        result.m_nLength = value2.m_nLength;
    }
    
    unsigned int i = 0;
    for(i=0;i<result.m_nLength;i++)
    {
        unsigned long long sum = value2.m_uValue[i];
        sum = sum + result.m_uValue[i] + carry;
        result.m_uValue[i] = sum & 0xFFFFFFFF;
        carry = sum>>32;
    }
    result.m_uValue[result.m_nLength] = carry;
    result.m_nLength+=carry;
}

void CBigInt::Sub(CBigInt &value1, CBigInt &value2, CBigInt &result)
{
    CBigInt r = value1;
    
    unsigned int borrow = 0;
    unsigned int i = 0;
    for(i = 0; i<r.m_nLength;i++)
    {
        if( (r.m_uValue[i]>value2.m_uValue[i]) || (r.m_uValue[i] == value2.m_uValue[i] && (borrow == 0)) )
        {
            r.m_uValue[i] = r.m_uValue[i] - borrow - value2.m_uValue[i];
            borrow = 0;
        }
        else
        {
            unsigned long long num = 0x100000000 + r.m_uValue[i];
            r.m_uValue[i] = (num - borrow - value2.m_uValue[i]) & 0xFFFFFFFF;
            borrow = 1;
        }
    }
    while(r.m_uValue[r.m_nLength-1]==0 && (r.m_nLength>1))
        r.m_nLength--;
    
    result = r;
}


CBigInt CBigInt::operator+(CBigInt &value)
{
    CBigInt r;
    if(m_Sign == value.m_Sign)
    {
        CBigInt::Add(*this, value, r);
        r.m_Sign = m_Sign;
    }
    else
    {
        if(CompareNoSign(value) >= 0)
        {
            CBigInt::Sub(*this, value, r);
            r.m_Sign = m_Sign;
        }
        else
        {
            CBigInt::Sub(value, *this, r);
            r.m_Sign = value.m_Sign;
        }
    }
    return r;
}

CBigInt CBigInt::operator-(CBigInt &value)
{
    CBigInt r;
    if(m_Sign != value.m_Sign)
    {
        CBigInt::Add(*this, value, r);
        r.m_Sign = m_Sign;
    }
    else
    {
        if(CompareNoSign(value) >= 0)
        {
            CBigInt::Sub(*this, value, r);
            r.m_Sign = m_Sign;
        }
        else
        {
            CBigInt::Sub(value, *this, r);
            r.m_Sign = m_Sign==0 ? 1 : 0;
        }
    }
    return r;
}

void CBigInt::Mul(CBigInt &value1, CBigInt &value2, CBigInt &result)
{
    unsigned long long carry = 0;
    result.m_nLength = value1.m_nLength + value2.m_nLength - 1;
    int i, j;
    for(i=0;i<result.m_nLength;i++)
    {
        unsigned long long sum = carry;
        carry = 0;
        for(j=0;j<value2.m_nLength;j++)
        {
            if((i-j)>=0 && ((i-j)<value1.m_nLength))
            {
                unsigned long long mul = value1.m_uValue[i-j];
                mul *= value2.m_uValue[j];
                carry += mul >> 32;
                mul = mul & 0xFFFFFFFF;
                sum+=mul;
            }
        }
        carry += sum >> 32;
        result.m_uValue[i] = sum & 0xFFFFFFFF;
    }
    if(carry != 0)
    {
        result.m_nLength++;
        result.m_uValue[result.m_nLength-1] = carry & 0xFFFFFFFF;
    }
}

void CBigInt::Div(CBigInt& value1, CBigInt& value2, CBigInt& quotient, CBigInt& remainder)
{
    CBigInt r(0);
    CBigInt a = value1;
    while(a.CompareNoSign(value2)>=0)
    {
        unsigned long long div = a.m_uValue[a.m_nLength - 1];
        unsigned long long num = value2.m_uValue[value2.m_nLength - 1];
        unsigned int len = a.m_nLength - value2.m_nLength;
        if(div == num && len == 0)
        {
            CBigInt temp(1);
            CBigInt::Add(r, temp, r);
            CBigInt::Sub(a, value2, a);
            break;
        }
        if(div<=num && (len>0))
        {
            len--;
            div= (div<<32)+a.m_uValue[a.m_nLength-2];
        }
        div = div/(num+1);
        CBigInt multi((unsigned int)div);
        
        if(len>0)
        {
            multi.m_nLength += len;
            unsigned int i;
            for(i=multi.m_nLength-1;i>=len;i--)
            {
                multi.m_uValue[i] = multi.m_uValue[i-len];
            }
            for(i=0;i<len;i++)
            {
                multi.m_uValue[i] = 0;
            }
        }
        CBigInt tmp;
        CBigInt::Add(r, multi, r);
        CBigInt::Mul(value2, multi, tmp);
        CBigInt::Sub(a, tmp, a);
    }
    
    quotient = r;
    remainder = a;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
