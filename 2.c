#include <stdlib.h>
#include <stdio.h>
int k=0;
int count=0;
int containerIndex = 0;  // 容器索引，记录当前容器中的位置
int skiphead=0;
unsigned short currentBit;
unsigned short waveform[6] = {0x833E,0xD574,0x304E,0x97E0,0x97E0,0x97E0};  // 第一个16位波形



unsigned short container[10] = {0};  // 容器数组，可以容纳两个16位波形
unsigned short receivedBits = 0;   // 用于接收每一位的值
unsigned short bitCount = 0;            // 记录接收的位数

unsigned short* General_leftmove1Bit(unsigned short *arr, unsigned char size)
{
    unsigned short *Result = malloc(size * sizeof(unsigned short));

    for (int i = 0; i < size; i++) 
    {
        Result[i] = (arr[i] << 1) | (arr[(i + 1)] >> 15);
    }
    return Result;
}

//找到与第一帧结尾最接近的16的倍数的数组大小
unsigned char findNearestMultipleOf16(unsigned char num) {
    unsigned char remainder = num % 16;
    unsigned char nearestMultiple = num + (16 - remainder);
    return nearestMultiple/16;
}


void Enqueuebit1_D(void)
{
		
    container[k] |= (1 << (15 - containerIndex));
    containerIndex++;
    // 判断容器是否已满
    if (containerIndex == 16) {
        k++;
        containerIndex = 0;
    }


}

void Enqueuebit0_D(void)
{
    container[k] |= (0 << (15 - containerIndex));
    containerIndex++;
    // 判断容器是否已满
    if (containerIndex == 16) {
        k++;
        containerIndex = 0;
    }
}

void Remove_StuffingBit(unsigned char size)
{
        for(int i = 0;i<size;i++)
               {
                    for (int j = 15; j >= 0; j--)
                    {
                       if(count<4)
                       {
                           if(j==15&&skiphead==1)
                           {
                               j--;
                               skiphead=0;
                           }
                           currentBit = (waveform[i] >> j) & 1;  // 当前位的值
                           if(currentBit==1){Enqueuebit1_D();}
                           if(currentBit==0){Enqueuebit0_D();}
                           if(j>=1)
                           {
                               count = (currentBit == ((waveform[i] >> (j - 1)) & 1)) ? count + 1 : 0;//当前位和下一位相比
                           }else if(j==0)
                           {
                               count = (currentBit == ((waveform[i+1] >> 15) & 1)) ? count + 1 : 0;//当前位和下一位相比
                           }

                       }
                       else if(count==4)
                       {

                               currentBit = (waveform[i] >> j) & 1;  // 当前位的值
                               if(currentBit==1){Enqueuebit1_D();}
                               if(currentBit==0){Enqueuebit0_D();}
                               count=0;
                               if(j>=1){
                                   j--;

                               }else if(j==0)
                               {
                                   skiphead=1;
                               }

                       }
                    }

               }

}


int main() {

       unsigned char size1=findNearestMultipleOf16(59);
        Remove_StuffingBit(size1);
    unsigned char size = sizeof(container) / sizeof(container[0]);
    unsigned short *result = General_leftmove1Bit(container, size);

    // 打印结果
    for (int i = 0; i < size1; i++) {
        printf("result[%d] = 0x%04X\n", i, container[i]);
    }

    return 0;
}




