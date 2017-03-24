///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Projektas LIEPA (https://liepa.raðtija.lt)
// Sintezatoriaus komponentas LithUSStest.exe
// Failas raw2wav.cpp
// Autorius dr. Pijus Kasparaitis (pkasparaitis@yahoo.com)
// 2015 08 11
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

unsigned long rID=0x46464952, rLen, wID=0x45564157, fID=0x20746D66;
unsigned short wFormatTag=0x0001, nChannels=0x0001;
unsigned long fLen=0x00000010, nSamplesPerSec=22050, nAvgBytesPerSec;
unsigned short nBlockAlighn=0x0002, wBitsPerSample=0x0010;
unsigned long dID=0x61746164, dLen;

int raw2wav(int ib, FILE* fr, short* buf)
{
dLen=ib*2;

//---------------.WAV failo antgalvio suformavimas------------------
fwrite(&rID,1,4,fr);
rLen=dLen+36;
fwrite(&rLen,1,4,fr);
fwrite(&wID,1,4,fr);
fwrite(&fID,1,4,fr);
fwrite(&fLen,1,4,fr);
fwrite(&wFormatTag,1,2,fr);
fwrite(&nChannels,1,2,fr);
fwrite(&nSamplesPerSec,1,4,fr);
nAvgBytesPerSec=nSamplesPerSec*2;
fwrite(&nAvgBytesPerSec,1,4,fr);
fwrite(&nBlockAlighn,1,2,fr);
fwrite(&wBitsPerSample,1,2,fr);
fwrite(&dID,1,4,fr);
fwrite(&dLen,1,4,fr);

fwrite(buf, 2, ib, fr);

return 0;
}

