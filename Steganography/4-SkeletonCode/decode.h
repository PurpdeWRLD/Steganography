#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char *magic_string;
    char extn_secret_file[MAX_FILE_SUFFIX];
    int extn_size;
    int size_secret_file;


    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
} DecodeInfo;

/*Decoding functions prototype*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/*Perform Decoding */
Status do_decoding(DecodeInfo *decInfo);

/*To open the encrypted image file*/
Status open_img_file(DecodeInfo *decInfo);

/*Skip the header file*/
Status skip_bmp_header(DecodeInfo *decInfo);

/*Decode magic string*/
Status decode_magic_string(const char *magic_sting, DecodeInfo *decInfo);

/*Decode byte from lsb*/
Status decode_bytes_from_lsb(char *data, char *buffer, DecodeInfo *decInfo);

/*Decode secret file name extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode int from the lsb*/
Status decode_int_from_lsb(int *data, char *buffer);

/*Decode the extension of the secret file and concatenate*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*Decode the file size*/
Status decode_secret_file_data_size(DecodeInfo *decInfo);

/*Decode the extension of the secret file and concatenate*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif