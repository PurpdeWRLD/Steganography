/*Name : Jaya Karunesh
Date: 29/07/2025
Sample input: ./a.out -d result.bmp hi
Sample output: 
INFO: ## Decoding Procedure Started
INFO: Opened result.bmp successfully
INFO: Decoding Magic String Signature 
INFO: Done
INFO: Decoding encoded file extension size
INFO: Done
INFO: Decoding encoded file extension
INFO: Done
INFO: Decoding the size of encoded data
INFO: Done
INFO: Decoding the contents of encoded data
INFO: Done
INFO: ##Decoding Done Successfully##
*/
#include <stdio.h>
#include "decode.h"
#include "types.h"

#include "common.h"
#include <string.h>
#include <stdlib.h>

//Function to validate the command line arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    int len1 = strlen(argv[2]);
    //Check the input is a .bmp file
    if(len1 >= 4 && strcmp(argv[2] + len1 - 4, ".bmp") != 0)
    {
        printf("Decoding: ./a.out <.bmp file> [output file name without extension]\n");
        printf("INFO: The stegonised image has to be a 'XXXXXXX.bmp' file\n");
        return e_failure;
    }
    decInfo -> stego_image_fname = malloc(len1 + 1);
    strcpy(decInfo -> stego_image_fname, argv[2]);

    
    if(argv[3] == NULL)
    {
        //If output file name is not declared "Secret_decode" will be the default name
        decInfo -> secret_fname = malloc(strlen("Secret_decode") + 1);
        printf("INFO: Created default file for storing the decrypted message\n");
        strcpy(decInfo -> secret_fname, "Secret_decode");
        return e_success;
    }
    else
    {
        //If output file name is mentioned
        int len2 = strlen(argv[3]), i;
        char invalid_symbols[4] = {'*', ':', '.', '?'};
        for(i = 0; i < 4; i++)
        {
            if(strchr(argv[3], invalid_symbols[i]) != NULL)
            {
                printf("Decoding: ./a.out <.bmp file> [output file name without extension]\n");
                printf("INFO: File name should not contain %c in it!\n", invalid_symbols[i]);
                return e_failure;
            }
        }
        decInfo -> secret_fname = malloc(len2 + 1);
        strcpy(decInfo -> secret_fname, argv[3]);
        return e_success;
    }
}

//Function for decoding process
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started\n");
    if(open_img_file(decInfo) == e_success)
    {
        if(skip_bmp_header(decInfo) == e_success)
        {
            printf("INFO: Decoding Magic String Signature\n");
            if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
            {
                printf("INFO: Decoding encoded file extension size\n");
                if(decode_secret_file_extn_size(decInfo) == e_success)
                {
                    printf("INFO: Decoding encoded file extension\n");
                    if(decode_secret_file_extn(decInfo) == e_success)
                    {
                        printf("INFO: Decoding the size of encoded data\n");
                        if(decode_secret_file_data_size(decInfo) == e_success)
                        {
                            printf("INFO: Decoding the contents of encoded data\n");
                            if(decode_secret_file_data(decInfo) == e_success)
                            {
                                printf("INFO: ##Decoding Done Successfully##\n");
                                return e_success;
                            }
                        }
                    }
                }
            }
        }
    }
    return e_failure;
}

//Function to open the input file
Status open_img_file(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");
    if (decInfo -> fptr_stego_image == NULL)
    {
        perror("INFO: Error: ");
        printf("INFO: Unable to open the file %s\n", decInfo -> stego_image_fname);
        return e_failure;
    }
    printf("INFO: Opened %s successfully\n", decInfo -> stego_image_fname);
    return e_success;
}

//Function to skip the header contents ie. 54 bytes
Status skip_bmp_header(DecodeInfo *decInfo)
{
    if(fseek(decInfo -> fptr_stego_image, 54, SEEK_SET) != 0)
    {
        return e_failure;
    }
    return e_success;
}

//Function to decode the magic string
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    int i;
    char buffer[8];
    int len = strlen(MAGIC_STRING);
    decInfo -> magic_string = malloc(len + 1 * sizeof(char));
    
    for(i = 0; i < len; i++)
    {
        if(fread(buffer, 8, 1, decInfo -> fptr_stego_image) != 1)
        {
            printf("INFO: Failed to read 8 bytes from the image\n");
            return e_failure;
        }
        if(decode_bytes_from_lsb(&decInfo -> magic_string[i], buffer, decInfo) != e_success)
        {
            return e_failure;
        }
        printf("\n");
    }
    decInfo -> magic_string[i] = '\0';
    //Decoded magic string is comapared with original magic string ie. "#*"
    if(strcmp(decInfo -> magic_string, MAGIC_STRING) != 0)
        {
            printf("INFO: Doesn't contain the secret message\n");
            return e_failure;
        }
    printf("INFO: Done\n");    
    return e_success;
}

//Function to decode the encoded contents from the .bmp file
Status decode_bytes_from_lsb(char *data, char *image_buffer, DecodeInfo *decInfo)
{
    *data = 0;
    for (int i = 0; i < 8; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1);
    }
    return e_success;
}

//Function to decode the extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    int i;
    char buffer[32];
    if(fread(buffer, 32, 1, decInfo -> fptr_stego_image) != 1)
    {
        printf("INFO: Could not fetch 32 bytes\n");
        return e_failure;
    }
    if(decode_int_from_lsb(&decInfo -> extn_size, buffer) != e_success)
    {
        printf("Didnt perform decoding the int from lsb\n");
        return e_failure;
    }
    printf("INFO: Done\n");
    return e_success;
}

//Function to decode the integer contents from the image file
Status decode_int_from_lsb(int *data, char *buffer)
{
    *data = 0;
    for (int i = 0; i < 32; i++)
    {
        *data = (*data << 1) | (buffer[i] & 0x01);
    }
    return e_success;
}

//Function to decode the secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    int i; 
    char buffer[8];
    for(i = 0; i < decInfo -> extn_size; i++)
    {
        if(fread(buffer, 8, 1, decInfo -> fptr_stego_image) != 1)
        {
            printf("Could not fetch 8 bytes for extn\n");
            return e_failure;
        }
        if(decode_bytes_from_lsb(&decInfo -> extn_secret_file[i], buffer, decInfo) != e_success)
        {
            printf("Decoding extn error\n");
            return e_failure;
        }
    }
    decInfo->extn_secret_file[i] = '\0';  // Safely terminate the string
    int new_len = strlen(decInfo->secret_fname) + strlen(decInfo->extn_secret_file) + 1;
    decInfo->secret_fname = realloc(decInfo->secret_fname, new_len);
    if (decInfo->secret_fname == NULL)
    {
        printf("Memory reallocation failed for secret_fname\n");
        return e_failure;
    }
    //Concatenate the extension with the file name to open and write the contents
    strcat(decInfo->secret_fname, decInfo->extn_secret_file);
    printf("INFO: Done\n");
    return e_success;
}

//Function to decode the secret file size form the image file
Status decode_secret_file_data_size(DecodeInfo *decInfo)
{
    int i;
    char buffer[32];
    if(fread(buffer, 32, 1, decInfo -> fptr_stego_image) != 1)
    {
        printf("Could not fetch 32 bytes\n");
        return e_failure;
    }
    if(decode_int_from_lsb(&decInfo -> size_secret_file, buffer) != e_success)
    {
        printf("Didnt perform decoding the int from lsb\n");
        return e_failure;
    }
    printf("INFO: Done\n");
    return e_success;
}

//Function to decode the secret file data from the image file
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    int i;
    char buffer[8];
    char ch;
    decInfo -> fptr_secret = fopen(decInfo -> secret_fname, "w");
    if(decInfo -> fptr_secret == NULL)
    {
        perror("Error: \n");
        return e_failure;
    }
    for(i = 0; i < decInfo -> size_secret_file; i++)
    {
        if(fread(buffer, 8, 1, decInfo -> fptr_stego_image) != 1)
        {
            printf("Reading error during file data\n");
            return e_failure;
        }
        if(decode_bytes_from_lsb(&ch, buffer, decInfo)!= e_success)
        {
            printf("Encoding error during data file\n");
            return e_failure;
        }
        if(fwrite(&ch, 1, 1, decInfo -> fptr_secret) != 1)//After decoding write the contents from the output file
        {
            printf("Writing error\n");
            return e_failure;
        }
    }
    printf("INFO: Done\n");
    return e_success;
}