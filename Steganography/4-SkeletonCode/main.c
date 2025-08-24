#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int num, char *string[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    
    if(num > 2)
    {
        //To chech whether the user need to perform encoding
        if(check_operation_type(string) == e_encode)
        {
            if(num >= 4 && num <= 5)
            {
                //Validate and Start encode
                if(read_and_validate_encode_args(string, &encInfo) == e_success)
                {
                    do_encoding(&encInfo);
                }
                else
                {
                    printf("INFO: Failure in doing encoding.\n");
                }
            }
            else
            {
                printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
                printf("Decoding: ./a.out <.bmp file> [output file]\n");
            }
        }
        
        //If user need to perform
        else if(check_operation_type(string) == e_decode)
        {
            if(num <= 4)
            {
                //Validate and Start decode
                if(read_and_validate_decode_args(string, &decInfo) == e_success)
                {
                    do_decoding(&decInfo);
                }
                else
                {
                    printf("INFO: Failure in doing decoding\n");
                }
            }
            else
            {
                printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
                printf("Decoding: ./a.out <.bmp file> [output file]\n");
            }
        }
        else if(check_operation_type(string) == e_unsupported)
        {
            printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
            printf("Decoding: ./a.out <.bmp file> [output file]\n");
        }
    }
    //If the user only typed only ./a.out
    else
    {
        printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
        printf("Decoding: ./a.out <.bmp file> [output file]\n");
    }
    return 0;
}
