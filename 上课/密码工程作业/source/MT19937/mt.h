/* MT19937标准的32位实现
 * 基于Wikipedia中梅森旋转算法的伪代码
 */

#include <stdint.h>
#include <stdio.h>

//MT19937中对梅森旋转算法的各参数规定
const int  w = 32;
const int  n = 624;
const int  m = 397;
const int  r = 31;
const unsigned int  a = 0x9908B0DF;
const int  u= 11;
const unsigned int  d= 0xFFFFFFFF;
const int  s = 7;
const unsigned int  b = 0x9D2C5680;
const int  t = 15;
const unsigned int  c= 0xEFC60000;
const int  l = 18;
const unsigned int  f = 1812433253;

const int lower_mask  = (1ull << r) - 1;
const int upper_mask  = (1ull << r);


static unsigned int  mt[624];
static unsigned short int   index;

//初始化
void seed_mt(const unsigned int  seed)
{
    unsigned int  i;
    index=n;
    mt[0] = seed;
    for ( i = 1; i < n; i++ )
    {
        mt[i] = (f * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
    }
    
}

//旋转更新内部状态
static void twist()
{
    unsigned int  i, x, xA;
    for ( i = 0; i < n; i++ )
    {
        x = (mt[i] & upper_mask) + (mt[(i + 1) % n] & lower_mask);
        xA = x >> 1;
        if ( x %2!=0 )
        {
            xA ^= a;
        }
        mt[i] = mt[(i + m) % n] ^ xA;
    }

    index = 0;
}

//Xorshift操作生成最终的伪随机数
unsigned int extract_number()
{
    unsigned int  y;
    if ( index >= n)
    {
        twist();
    }
    y = mt[index];
    index = index + 1;
    y ^= (y >> u);
    y ^= (y << s) & b;
    y ^= (y << t) & c;
    y ^= (y >> l);
    return y;
}

void init(unsigned int seed)
{
   seed_mt(seed);
}
unsigned int rng()
{
   return extract_number(); 
}

