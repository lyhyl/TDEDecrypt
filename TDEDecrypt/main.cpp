#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <intrin.h>

#define _BYTE unsigned __int8
#define _QWORD unsigned __int64

const char* const a = "599Cc51887A8cb0C20F9CdE34cf9e0A535E5cAd1C26c7b562596ACC207Ae9A0bfB3E3161f31af5bEf1c2f064b3628174D83BF6E0739D9D6918fD9C2Eba02D5aD\0";
const char* const b = "0C3b676fe8d7188Cde022F71632830F36b98b181aD48Fed160006eA59\0";

auto input = "D:\\Steam\\steamapps\\common\\Teardown\\data\\snd\\alarm-background.ogg.tde";
//auto input = "D:\\Steam\\steamapps\\common\\Teardown\\data\\env\\cloudy.dds.tde";
//auto input = "D:\\Steam\\steamapps\\common\\Teardown\\data\\gfx\\rect.png.tde";
auto output = "D:\\Project\\TD\\test\\alarm-background.ogg";

char tdEncryptedFileInputStreamReadImpl(FILE* file, unsigned __int64 *fptr, _BYTE* chunk, unsigned __int64 chunkSize)
{
    unsigned __int64 readByteCount = 0;
    if (chunkSize)
    {
        while (1)
        {
            _BYTE* chunkPtr = &chunk[readByteCount];
            size_t rbc = fread(&chunk[readByteCount], 1, chunkSize - readByteCount, file);
            if (!rbc)
                break;
            if (rbc)
            {
                size_t v9 = *fptr - (size_t)chunkPtr;
                size_t v10 = rbc;
                do
                {
                    size_t v11 = (size_t)chunkPtr + v9;
                    unsigned __int64 v12;
                    _umul128((size_t)chunkPtr + v9, 0x8FB823EE08FB823Fui64, &v12);
                    *chunkPtr ^= a[(unsigned __int8)v11 & 0x7F] ^ b[-57 * (v12 >> 5) + v11];
                    chunkPtr++;
                    --v10;
                } while (v10);
            }
            *fptr += rbc;
            readByteCount += rbc;
            if (readByteCount >= chunkSize)
                return 1;
        }
    }
    return 0;
}

int main()
{
    FILE* infile = fopen(input, "rb");
    FILE* outfile = fopen(output, "wb");
    fseek(infile, 0, SEEK_END);
    size_t len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    _BYTE* buffer = new _BYTE[len];
    unsigned __int64 fptr = 0;

    size_t readLen = 0;
    while (readLen < len)
    {
        size_t rest = len - readLen;
        size_t r = rest < 4096 ? rest : 4096;
        auto result = tdEncryptedFileInputStreamReadImpl(infile, &fptr, buffer + readLen, r);
        printf("%d %lld\n", result, fptr);
        fwrite(buffer + readLen, 1, r, outfile);
        readLen += r;
    }
    fclose(infile);
    fclose(outfile);

    return 0;
}