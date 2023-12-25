./opus_demo -e voip 48000 1 32000 ../../speech.pcm ../../speech.opus #编码
./opus_demo -d 24000 1 speech.opus speech1.pcm #解码
