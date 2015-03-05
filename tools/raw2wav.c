/**
 * Convert original RSS files to standard WAV files.
 *
 * RSS file is in fact plain RIFF/WAVE file without headers. The format is
 * RIFF little-endian data, WAVE audio, Microsoft PCM, 16 bit, stereo 22050 Hz
 *
 * Compilation: $ gcc -o raw2wav raw2wav.c
 *
 * Copyright 2010 (C) OpenAtomic contributors. See COPYING for license.
 */

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	const char subchunk1[] = "\x66\x6d\x74\x20\x10\x00\x00\x00\x01\x00\x02\x00\x22\x56\x00\x00\x88\x58\x01\x00\x04\x00\x10\x00";
	FILE *input = NULL;
	FILE *output = NULL;
	unsigned int length = -1;
	char * buffer = NULL;
	size_t returnSize = -1;
	struct stat stats;
	int error = 0;

	if(argc != 3)
	{
		fprintf(stderr,"Usage: %s INPUTFILE OUTPUTFILE\n\r", argv[0]);
		return 1;
	}

	input = fopen(argv[1], "rb");
	if(!input)
	{
		perror("Opening input file for reading");
		return 2;
	}

	error = fstat(fileno(input), &stats);
	if(error)
	{
		perror("Reading input file stats");
		return 3;
	}

	output = fopen(argv[2], "wb");
	if(!output)
	{
		perror("Opening output file for writing");
		return 4;
	}
	returnSize = fwrite("RIFF", sizeof(char)*4, 1, output);
	length = stats.st_size + 36;    /* header size + data */
	//length = 1600000;
	returnSize = fwrite(&length, sizeof(int), 1, output);
	returnSize = fwrite("WAVE", sizeof(char)*4, 1, output);
	returnSize = fwrite(subchunk1, sizeof(char)*(sizeof(subchunk1)-1), 1, output);
	returnSize = fwrite("data", sizeof(char)*4, 1, output);
	returnSize = fwrite(&(stats.st_size), sizeof(int), 1, output);
	
	if( (buffer = malloc(stats.st_size) ) == NULL )
	{
		perror("Allocating memory for input file");
		return 5;
	}
	
	returnSize = fread (buffer, 1, stats.st_size, input);
	if (returnSize != stats.st_size)
	{
		perror("Reading input file");
		return 6;
	}
	
	returnSize = fwrite(buffer, stats.st_size, 1, output);
	  
	free(buffer);
	fclose (input);
	fclose (output);
	return 0;
}

