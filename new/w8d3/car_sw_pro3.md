
b-re
    d15 : 40F73333

B::dcsa r(PCXI)
    33 33 F7 40 
    으로 저장 


r.set pc 0x70000000 -> 오른쪽 -> assemmbly ~~ 클릭
-> ld.w d0, [a0] -> 0x70000000 명령어가 저장됨을 확인 
b:register 
    값을 더블클릭, 그 값을 수정 0xf0000000 (pheriperal 영역)
    IO -> 0으로 변경