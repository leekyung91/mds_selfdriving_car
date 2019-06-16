# 5일차
### LCD
1. 이미지 만들 때
   1. .bmp 파일을 만들어서 비트를 확인(24비트면 꼭 해당하는 BPP를 사용)
   2. 가로비율을 4의 정수배 픽셀로 만들어야
2. convert -geometry 480x272! -type truecolor bts-grammys-1.jpg myimg.bmp
   1. 480x272비율로 convert(24비트면 24비트로 만들어줘)
3. 참고 자료!! [LCD 변환](http://www.fmwconcepts.com/imagemagick)

## 오디오 인터페이스
 
### 소리가 이상하면 파일 삭제하고 
mw.b 37000000 0 110000
tftp 37000000 heart.wav

> [UART 관련 페이지](https://changkyun.tistory.com/entry/UART-Device)