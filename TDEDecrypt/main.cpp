#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <intrin.h>

#define _BYTE unsigned __int8

const char* const a = "599Cc51887A8cb0C20F9CdE34cf9e0A535E5cAd1C26c7b562596ACC207Ae9A0bfB3E3161f31af5bEf1c2f064b3628174D83BF6E0739D9D6918fD9C2Eba02D5aD\0";
const char* const b = "0C3b676fe8d7188Cde022F71632830F36b98b181aD48Fed160006eA59\0";

auto input = "D:\\Steam\\steamapps\\common\\Teardown\\\data\\music\\menu.ogg.tde";
auto output = "D:\\Project\\TD\\test\\menu.ogg";
//auto input = "D:\\Steam\\steamapps\\common\\Teardown\\data\\snd\\alarm-background.ogg.tde";
//auto output = "D:\\Project\\TD\\test\\alarm-background.ogg";
//auto input = "D:\\Steam\\steamapps\\common\\Teardown\\data\\env\\cloudy.dds.tde";
//auto output = "D:\\Project\\TD\\test\\cloudy.dds";
//auto input = "D:\\Steam\\steamapps\\common\\Teardown\\data\\gfx\\rect.png.tde";
//auto output = "D:\\Project\\TD\\test\\rect.png";

char tdEncryptedFileInputStreamReadImpl(FILE* file, size_t* fptr, _BYTE* chunk, unsigned __int64 chunkSize)
{
	if (chunkSize == 0)
		return 0;
	for (size_t bytes = 0; bytes < chunkSize;)
	{
		_BYTE* chunkPtr = &chunk[bytes];
		size_t rbc = fread(&chunk[bytes], 1, chunkSize - bytes, file);
		if (!rbc)
			return 0;
		for (size_t i = 0; i < rbc; i++)
		{
			size_t v11 = *fptr + i;
			unsigned long long v12;
			_umul128(v11, 0x8FB823EE08FB823Fui64, &v12);
			chunkPtr[i] ^= a[(unsigned __int8)v11 & 0x7F] ^ b[-57 * (v12 >> 5) + v11];
		}
		*fptr += rbc;
		bytes += rbc;
	}
	return 1;
}

int main()
{
	FILE* infile = fopen(input, "rb");
	FILE* outfile = fopen(output, "wb");
	fseek(infile, 0, SEEK_END);
	size_t len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	const size_t BufferSize = 4096;
	_BYTE* buffer = new _BYTE[BufferSize];
	size_t fptr = 0;

	size_t readLen = 0;
	while (readLen < len)
	{
		size_t rest = len - readLen;
		size_t r = rest < BufferSize ? rest : BufferSize;
		auto result = tdEncryptedFileInputStreamReadImpl(infile, &fptr, buffer, r);
		printf("%d %lld\n", result, fptr);
		fwrite(buffer, 1, r, outfile);
		readLen += r;
	}
	fclose(infile);
	fclose(outfile);

	return 0;
}