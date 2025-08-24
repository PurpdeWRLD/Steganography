/*
Name: Jaya Karunesh
Date: 29/07/2025
Sample input: ./a.out beautiful.bmp secret.txt stego.bmp
Sample output: 
INFO:  is the encrypted output file
INFO: All validations for encoding part are correct!
INFO: Opened beautiful.bmp
INFO: Opened secret.txt
INFO: Opened result.bmp
INFO: ## Encoding Procedure Started ##
width = 1024
height = 768
INFO: Secret file is not emptyINFO: beautiful.bmp have the capacity to handle secret.txt
INFO: Copying Image Header
INFO: Done
INFO: Encoding Magic String Signature
INFO: Done
INFO: Encoding secret.txt extension size
INFO: Done
INFO: Encoding secret.txt extension
INFO: Done
INFO: Encoding secret.txt file size
INFO: Encoding secret.txt file contents
INFO: Copying the remaining left over data
INFO: ##Encodeing Done Successfully##
 */
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

/* Function Definitions */

//To check the operations
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

//Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
//{
    // //If argv[2] contains .bmp => store filename in the sturcture
    // if(strcmp(".bmp", strstr(argv[2], ".bmp")) == 0)
    // {
    //     encInfo -> src_image_fname = malloc(strlen(argv[2]) + 1);
    //     strcpy(encInfo -> src_image_fname, argv[2]);
    // }
    // //Else print error msg, return e_faliure
    // else
    // {
    //     printf("Error it is not a bmp file1\n");
    //     return e_failure;
    // }
    // //If argv[3] is .txt, .c, .sh => store filename
    // //Else print error, return e_faliure
    // char *extension[3] = {".txt", ".c", ".sh"};
    // int i, flag = 0;
    // for(i = 0; i < 3; i++)
    // {
    //     if(strcmp((extension[i]), strstr(argv[3], extension[i])) == 0)
    //     {
    //         encInfo -> secret_fname = malloc(strlen(argv[3]) + 1);
    //         strcpy(encInfo -> secret_fname, argv[3]);
    //         strcpy(encInfo -> extn_secret_file, strchr(argv[3], '.'));
    //         flag = 1; 
    //     }
    // }
    // if(flag == 0)
    // {
    //     printf("Error in fetching the secret file");
    //     return e_failure;
    // }
    // //If argv[4] is NULL store default filename 
    // //ELse if argv[4] is present store the filename
    // //Else error msg, print e_failure
    // if (argv[4] == NULL)
    // {
    //     encInfo -> stego_image_fname = malloc(strlen("Stego.bmp") + 1);
    //     strcpy(encInfo -> stego_image_fname, "Stego.bmp");
    // }
    // else if(strcmp(".bmp", strstr(argv[4], ".bmp")) == 0)
    // {
    //     encInfo -> stego_image_fname = malloc(strlen(argv[4]) + 1);
    //     strcpy(encInfo -> stego_image_fname, argv[4]);
    // }
    // else
    // {
    //     printf("Error it is not a bmp file\n");
    //     return e_failure;
    // }
    // //If all are valid return e_success
    // return e_success;
    //validate encode arguements and return e_success or e_failure

//To check the command line arguments are entered without any error    
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    int len;

    // Validate Source Image: Must be a .bmp file
    len = strlen(argv[2]);
    if (strcmp(argv[2] + (len - 4), ".bmp") != 0)
    {
        printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
        printf("INFO: Source image must be a .bmp file\n");
        return e_failure;
    }
    encInfo->src_image_fname = malloc((len + 1) * sizeof(char));
    strcpy(encInfo->src_image_fname, argv[2]);

    // Validate Secret File: Must be a .txt file or .bmp file or .c file
    len = strlen(argv[3]);
    char *arg3[5] = {".bmp", ".txt", ".c", ".h", ".sh"};
    int i = 0;
    int index = -1;
    char a3[5];
    while(i < 5)
    {
        if(strstr(argv[3], arg3[i]) != NULL)
        {
            index = i;
            strcpy(a3, strstr(argv[3], arg3[i]));
            break;
        }
        i++;
    }
    if(index != -1)
    {
        if(strcmp(a3, arg3[index]) == 0)
        {
            encInfo->secret_fname = malloc((len + 1) * sizeof(char));
            strcpy(encInfo->secret_fname, argv[3]);
            strcpy(encInfo->extn_secret_file, a3);
            
        }
        else
        {
            printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
            printf("INFO: secret file extension must be a valid extension\n");
            return e_failure;
        }
    }
    else
    {
        printf("Encoding: ./a.out <.bmp file> <.txt file> [output file]\n");
        printf("INFO: secret file extension must be a valid extension\n");
        return e_failure;
    }
    
    // Optional Stego Image File validation/ destination file validation
    //if the file is present validate it or assign a new file name
    if (argv[4] == NULL)
    {
        printf("INFO: Output File not mentioned. Creating stego.bmp as default\n");
        encInfo->stego_image_fname = malloc((strlen("stego.bmp")) * sizeof(char));
        strcpy(encInfo->stego_image_fname, "stego.bmp");
        return e_success;
    }
    else
    {
        len = strlen(argv[4]);
        if (len < 4 || strcmp(argv[4] + (len - 4), ".bmp") != 0)
        {
            printf("INFO: Output stego image must be a .bmp file\n");
            return e_failure;
        }
        encInfo->stego_image_fname = malloc((len + 1) * sizeof(char));
        printf("INFO: %s is the encrypted output file\n", encInfo -> stego_image_fname);
        strcpy(encInfo->stego_image_fname, argv[4]);
    }
    printf("INFO: All validations for encoding part are correct!\n");
    return e_success;
}

//To perform encoding
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO: ## Encoding Procedure Started ##\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf("INFO: Copying Image Header\n");
            if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
            {
                printf("INFO: Encoding Magic String Signature\n");
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("INFO: Encoding %s extension size\n", encInfo -> secret_fname);
                    if(encode_secret_file_ext_size(strlen(encInfo -> extn_secret_file), encInfo, encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
                    {
                        printf("INFO: Encoding %s extension\n", encInfo -> secret_fname);
                        if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
                        {
                            printf("INFO: Encoding %s file size\n", encInfo -> secret_fname);
                            if(encode_secret_file_size(encInfo -> size_secret_file, encInfo, encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
                            {
                                printf("INFO: Encoding %s file contents\n", encInfo -> secret_fname);
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("INFO: Copying the remaining left over data\n");
                                    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
                                    {
                                        printf("INFO: ##Encodeing Done Successfully##\n");
                                        return e_success;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return e_failure;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    // No failure return e_success
    printf("INFO: Opened %s\n", encInfo -> src_image_fname);
    printf("INFO: Opened %s\n", encInfo -> secret_fname);
    printf("INFO: Opened %s\n", encInfo -> stego_image_fname);
    return e_success;
}

//Function to check capacity
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);//Get src file
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);//Get secret file
    if(encInfo -> size_secret_file == 0) //If secret file is empty
    {
        printf("INFO: Secret file is empty\n");
        return e_failure;
    }
    else
    {
        printf("INFO: Secret file is not empty");
    }
    uint size = 54 + (strlen(MAGIC_STRING) + 4 + strlen(encInfo -> extn_secret_file) + 4 + encInfo -> size_secret_file) * 8;//Checking the overall ecodeing size
    if(encInfo -> image_capacity > size)//Check whether image size is greater than the encoding size
    {
        printf("INFO: %s have the capacity to handle %s\n", encInfo -> src_image_fname, encInfo -> secret_fname);
        return e_success;
    }
    else
    {
        printf("INFO: %s does not have the capacity to handle %s\n", encInfo -> src_image_fname, encInfo -> secret_fname);
        return e_failure;
    }
}

//Function to get file size
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint res = ftell(fptr);
    rewind(fptr);
    return res;
}

//Function to copy the bmp file header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    if(fread(buffer, 54, 1, fptr_src_image) == 1)//Get 54 file from src file
    {
        if(fwrite(buffer, 54, 1, fptr_dest_image) == 1)//Write 54 bytes in dest file
        {
            printf("INFO: Done\n");
            return e_success;
        }
        else
        {
            printf("Error while writing to destination file");
            return e_failure;
        }
    }
    else
    {
        printf("Error while reading from source file");
        return e_failure;
    }
}

//Function to encode magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if(encode_data_to_image(MAGIC_STRING, 2, encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        printf("INFO: Done\n");
        return e_success;
    }
    printf("INFO: Failure in encrypting the magic sting signature\n");
    return e_failure;
}

//Function to encode a single character
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    int i;
    char buffer[8];
    for(i = 0; i < size; i++)
    {
        if(fread(buffer, 1, 8, fptr_src_image) != 8)//Get 8 bytes from src image
        {
            printf("ERROR: Failed to read 8 bytes from source image during encoding.\n");
            return e_failure;
        }
        if(encode_byte_to_lsb(data[i], buffer) == e_success)//To encode a single byte of charcter from secret file with the 8 bytes from src image
        {
            if(fwrite(buffer, 1, 8, fptr_stego_image) != 8)
            {
                return e_failure;
            }
        }
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    unsigned get, n = 7, i;
    for(i = 0; i < 8; i++)
    {
        image_buffer[i] = image_buffer[i] & ~1; //clear lsb bit
        get = (data & (1 << n)) >> n;//Get the bit from the character
        image_buffer[i] = image_buffer[i] | get;//Encrypting the bit
        n--;
    }
    return e_success;
}

//Finction to encode the file size extension
Status encode_secret_file_ext_size(int file_size, EncodeInfo *encInfo, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    int i;
    char buffer[32];
    fread(buffer, 32, 1, fptr_src_image);//Read 32 bytes from the src image
    if(encode_int_to_lsb(file_size, buffer) == e_success)//Encode the file extn size
    {
        if(fwrite(buffer, 32, 1, fptr_stego_image) == 1)//Write the 32 byres into stego image
        {
            printf("INFO: Done\n");
            return e_success;
        }
        else
        {
            printf("INFO: Extension size encryption failed\n");
            return e_failure;
        }
    }
}

Status encode_int_to_lsb(int data, char *image_buffer)
{
    for (int i = 31; i >= 0; i--)
    {
        // Clear LSB of each byte
        image_buffer[31 - i] &= 0xFE;

        // Set LSB with bit from data
        image_buffer[31 - i] |= ((data >> i) & 1);
    }
    return e_success;
}

//Function to encode the file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char *str = malloc(strlen(file_extn) + 1);
    strcpy(str, file_extn);
    if(encode_data_to_image(str, strlen(encInfo -> extn_secret_file), encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
    {
        free(str);
        printf("INFO: Done\n");
        return e_success;
    }
    free(str);
    printf("INFO: Encryption of file extenion falied\n");
    return e_failure;
}

//Function to encode the file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    int i;
    char buffer[32];
    fread(buffer, 32, 1, fptr_src_image);
    if(encode_int_to_lsb(file_size, buffer) == e_success)
    {
        if(fwrite(buffer, 32, 1, fptr_stego_image) == 1)
        {
            return e_success;
        }
        else
        {
            return e_failure;
        }
    }
}

//To encode the secret file contents
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    for (long i = 0; i < encInfo->size_secret_file; i++)
    {
        if (fread(&ch, 1, 1, encInfo->fptr_secret) != 1)
        {
            printf("Error reading byte %ld from secret file\n", i);
            return e_failure;
        }

        if (encode_data_to_image(&ch, 1, encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        {
            printf("Encoding failed at byte %ld\n", i);
            return e_failure;
        }
    }

    return e_success;
}

//Function to copy the remaining src image data into encoded image 
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int i;
    char ch;
    while(fread(&ch, 1, 1, fptr_src) == 1)
    {
        if(fwrite(&ch, 1, 1, fptr_dest) != 1)
        {
            return e_failure;
        }
    }
    return e_success;
}