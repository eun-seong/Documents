- audio file를 binary로 보는 방법
```
hexdump -C filename | vi -
```
***

# WAV file   
### RIFF(12byte)  
1. Chunk ID(4byte) : ('RIFF' 고정) 52 49 46 46  
2. Chunk Size(4byte) Little Endian : 파일 전체 사이즈 - (ChunkID와 ChunkSize 사이즈)
3. Format(4byte) : ('WAVE' 고정) 57 41 56 45

### FMT(24byte)   
1. Chunk ID(4byte) : ('fmt ' 고정) 66 6D 74 20
2. Chunk Size(4byte) Little Endian : (현재 사이즈에서 이어지는 값 고정) 10 00 00 00
3. Audio Format(2byte) Little Endian : (PCM 고정) 01 00
4. Number Of Channel(2byte) Little Endian : 음성 파일 채널 수   
```
1 | mono
2 | stereo
3 | 3 channel
4 | quad
5 | 4 channel
6 | 6 channel
```
5. Sample Rate(4byte) Little Endian : Number of Samples Per Second
6. Byte Rate(4byte) Little Endian : (Bits Per Sample)*(Sample Rate)*(Number of Channel)
7. Block Align(2byte) Little Endian : Sample Frame = (Sample Size)*(Number of Channel)
8. Bit Per Sample(2byte) : Sample 하나 당 bit 수
   
### DATA  
1. Chunk ID(4byte) : ('data' 고정) 64 61 74 61
2. Chunk Size(4byte) Little Endian : 뒤에 나올 data size(파일 전체 사이즈 - header)
***



