#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Caio Sobrinho da Silva
//RA: 211044156
using namespace std;

typedef struct header_file{
    char riff[4];
    int chunk_size; 
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;
}header;

int getFileSize(FILE *arq){
    int fileSize = 0;
    fseek(arq, 0, SEEK_END);
    fileSize = ftell(arq);
    
    fseek(arq, 0, SEEK_SET);
    return fileSize;
}

int main(){
    /*header wavHeader;
    riff_chunk riffChunk;*/
    FILE *wavFile;
    FILE *wav;
    FILE *invert;

    int lenght = 0;
    int bytesRead;
    unsigned long numSamples;
    header header;
    wavFile = fopen("applause.wav", "rb");
    wav = fopen("copia.wav", "wb");
    invert = fopen("invert.wav", "wb");

    if(wavFile == NULL){
        cout << "\nCannot open this file!";
    }
    
    fread(&header,1, sizeof(header), wavFile);
    fwrite(&header, 1, sizeof(header), wav);
    fwrite(&header, 1, sizeof(header), invert);
    cout << "\nFile type: "<< header.riff[0]<<header.riff[1]<<header.riff[2]<<header.riff[3];
    cout << "\nFile size, without the header: "<<getFileSize(wavFile)-(sizeof(header.riff) + sizeof(header.chunk_size));
    fseek(wavFile, sizeof(header), SEEK_SET);
    cout << "\nSubtype: "<< header.format[0]<<header.format[1]<<header.format[2]<<header.format[3];
    cout << "\nIdentifier: " << header.subchunk1_id[0]<<header.subchunk1_id[1]<<header.subchunk1_id[2]<<header.subchunk1_id[3];
    cout << "\nChunk Size: "<< header.subchunk1_size;
    cout << "\nAudio Format(Form of compression): "<<header.audio_format;
    cout << "\nNumber of channels: "<< header.num_channels;
    if(header.num_channels == 1){
        cout <<", Mono";
    }
    else
        cout <<", Stereo";
    cout << "\nSample rate: " << header.sample_rate;
    header.byte_rate = (header.sample_rate)*(header.num_channels)*(header.bits_per_sample/8);
    cout << "\nByte rate: " << header.byte_rate;
    header.block_align = (header.num_channels)*(header.bits_per_sample/8);
    cout << "\nBlock Align: " << header.block_align;
    cout << "\nBits per sample: " << (header.bits_per_sample) << " bits";
    cout << "\nSubchunk id: " <<header.subchunk2_id[0]<<header.subchunk2_id[1]<<header.subchunk2_id[2]<<header.subchunk2_id[3];
    cout <<"\nData size: " <<header.subchunk2_size << endl;
    int nBytes;
    short int data[header.subchunk2_size];
    short int invertedData[header.subchunk2_size];

    fread(data, 1, header.subchunk2_size, wavFile); //(vetor, quantos bytes le por iteracao, quantos bytes maximo, arquivo)
    fwrite(data, 1, header.subchunk2_size, wav);

    int i = 0;
    int j = 1;
    while (i < header.subchunk2_size && j < header.subchunk2_size)
    {
        invertedData[i] = data[j];
        invertedData[j] = data[i];
        i+=2;
        j+=2;
    }
    fwrite(invertedData, 1, header.subchunk2_size, invert);
    
    return(0);
}