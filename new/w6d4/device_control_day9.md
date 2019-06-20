# 9일차
## cache memory
1. I-Cache / D-Cache 활성화 방법
2. Memory Mapped I/O D-Cache 비활성화
3. Cache Clean 과 Cache Flush (Invalidate)
   1. 여기서 캐시는 D-Cache를 의미
   2. 데이터가 수정되면 자연스럽게 업데이트 되는데 강제로 업데이트 하고 싶으면 cache clean 
   3. cache flush를 캐시 메모리에 들어가있는 값을 무효화 시키는 것(invalidate)
* 주의!! dma를 사용할 때는 d-cache를 off 시켜놓고 사용해야해!!


## mmu
1. 링커(linker) 스크립트 파일에서의 주소 표현
   1. ![](./0620_001.png)
2. 예외처리(exception) 벡터 테이블
3. 보호된 메모리 접근 -> data abort(소프트웨어가 죽어버려)
4. 존재하지 않는 가상 메모리 접근 -> data abort

* mmu는 상대 주소로 분기(몇바이트 떨어졌는지에 따라 이동..?)
  * main.c에서 mmu_init()전까지 0xC0000000의 주소는 아무 의미 없어...
  * Linker Script
    * ```c
        MEMORY
        {
            CODE (rw)    	: ORIGIN  = 0xC0000000, LENGTH = 0x04000000
            _ISR_START(rw) 	: ORIGIN  = 0x33ffff00, LENGTH = 0x00000100
        }
        ```
    * MDS2450.dis
      * ```c
        c0000000 <_start>:
        c0000000:	ea00002a 	b	c00000b0 <ResetHandler>
        c0000004:	ea000011 	b	c0000050 <HandlerUndef>
        c0000008:	ea000016 	b	c0000068 <HandlerSWI>
        c000000c:	ea000021 	b	c0000098 <HandlerPabort>
        c0000010:	ea00001a 	b	c0000080 <HandlerDabort>
        c0000014:	eafffffe 	b	c0000014 <_start+0x14>
        c0000018:	ea000006 	b	c0000038 <HandlerIRQ>
        c000001c:	eaffffff 	b	c0000020 <HandlerFIQ>
        ```
    * libc.c
      * ```c
        MMU_SetMTT(0x00000000,0xfff00000,0x30000000,RW_NCNB);//not used
        ```
    * 리눅스 명령어(dis 파일 생성)
      *  arm-eabi-objdump -d MDS2450 > MDS2450.dis
 *  tcm이란
    *  캐시 처럼 속도가 빠르고 용량이 작은 메모리
    *  일부분의 cpu 접근하면 속도가 높아진다. 
## ![](./0620_002.png)
![](./0620_003.png)
![](./0620_004.png)
![](./0620_005.png)
![](./0620_006.png)

## 어셈블리 코드로 클럭 변경하는 법
![](./0620_007.png)
![](./0620_008.png)
![](./0620_009.png)
![](./0620_010.png)
- 240M으로 변환
![](./0620_011.png)
![](./0620_012.png)
![](./0620_013.png)

- 추후 정리
![](./0620_014.png)
![](./0620_015.png)
![](./0620_016.png)
![](./0620_017.png)
![](./0620_018.png)
![](./0620_019.png)
![](./0620_020.png)
![](./0620_021.png)
![](./0620_022.png)
![](./0620_023.png)
![](./0620_024.png)


- 최근에 사용되는 에스디램은 대부분 디디알 에스디램(DDR SDRAM)이라고 보시면 됩니다. 에스디램이 하는 역할은 임베디드 시스템이 동작을 할 때 프로그램을 수행하며 주기적으로 변하는 데이타를 저장하거나 읽기를 하는 일이랍니다. 
- BSP : 보드 지원 패키지 

arm-eabi-gcc : 펌웨어 만들 때
arm-linux-gnueabihf-gcc : 리눅스 커널 컴파일, 리눅스에서 러닝하는 어플리케이션에 대한 컴파일러

링크 스크립트
*(.text) : 와일드 카드, 모든걸 여기로 가져와라...?

- reset 예외
  - por(파워온리셋)
  - h/w 리셋(리셋 스위치 입력)
  - wdt 리셋
  - * s/w 리셋(cpu 마다 달라, 레지스터에 값을 기록하면 그때 사용하는...?)
  - pc=30000000(리셋이라 부르긴 그렇지만...우리가 만든 코드를 처음부터 실행하도록)

- arm exception
  - prefetch abort & data abort
    - 1. *비정렬 액세스(data abort only)
    - 2. 보호된 메모리 액세스 : 특권모드에 유저모드가 접근하려고 할 때
    - 3. 존재하지 않는 가상 메모리 액세스 : 레코드를 만들어 놓았지만 어떤 가상 메모리와도 연결이 안됬으면 존재하지 않아서 abort
    - abort는 부정적인 것!
## b라는 명령어 외에도
mov pc,#ResetHandler  
ldr pc,=ResetHandler  
이렇게 사용이 가능하다.  

## FIQ가 IRQ보다 빠르게 처리되는 이유
1. 파이프라인 파괴가 되는 것을 방지할 수 있다.
   1. 부트코드에서 제일 마지막에 있기때문에, 그 바로 밑에 HandlerFIR:사용하면 된다.
2. 우선순위(2)가 IRQ보다 높다
3. Private 한 레지스터(컨텍스트 저장/복원이 필요없는)가 5개 존재(r8~r12)
