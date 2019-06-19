# 8일차
## 곱셈 명령어
- 잘 사용하지 않아

## load/store 명령어
* 자주 사용, 중요!!
* ![](./0619_008.png)
### 코드
 ```c
    ldr r0,[r1] /* r1=0x30100000 */
    str r0,[r2] /* r2=0x30200000 */
    ldr r0,[r1, #4] /* r1=0x30100000+4 */
    str r0,[r2, #4] /* r2=0x30200000+4 */
    mov r4,#16
    ldr r0,[r1,r4, lsl #4] /* r1=0x30100000 */
    /* 16(r4를 4로 밀어)*16 + 0x30100000 */
    str r0,[r2,r4, lsl #4] /* r2=0x30200000 */
    
    ldrb r0,[r1, #4]  /* r1=0x30100000, little endian : 30100004번지를 읽어들여 */
    /* big endian : 30100004번지... 동일행 */
    ldrsb r0,[r1, #4] /* 부호가 있는 값을 읽어 들이는 것 */
    /* sb 는 load 명령어에만 사용가능!! strsb는 없다!!*/
    
    ```
* 리틀, 빅 엔디안이어도 저장하는 곳은 0비트부터 
![](./0619_001.png)
  * 즉, 저장하는 공간은 같다. 

### pre-indexed addressing mode
 ```c
    mov r4,#16

    /* auto update!!, r1=0x30100000 -> r1=0x30100004 
    #4만큼 업데이트가 된다. 주소 계산후에 접근*/

    ldr r0,[r1, #4]!

    /* r2=0x30200000+4 */  
    str r0,[r2, #4] 
```

### post-indexed addressing mode
 ```c
    mov r4,#16

    /* r1=0x30100000번지에서 읽어온다!!, []안에 있는 메모리를 읽어온다!!
    메모리에 접근한 후에 #4만큼 주소 계산!!*/
    ldr r0,[r1], #0x4

    /* r2=0x30200000+4 */  
    str r0,[r2],#4
    /* 공통적으로 r1이 주소가 업데이트!! */

```

### relative 어드레스 지정 방식
  * ldr r0, 0x30100000 -> 0x30100000번지에서 word 단위로 읽어 들여
  * ldr r0,=0x30100000 -> 0x30100000 주소값이 들어가
  * label_1 dcd 0x12345678
  * ldr r0, label_1 -> 0x12345678번지에서 word 단위로 r0가 읽어 들여
  * ldr r0,=label_1 -> r0에 0x12345678 주소값이 들어가
  * 

## loop
```c
while(){}
do{}while(); // 어셈블리에서는 이걸 주로 사용!!
for(;;){}
```
```c
sum_loop:
	add r3,r3,r0  /* sum */
	add r0,r0,#1	/* count */
	cmp r0, r1
	bls sum_loop

	mov r0,r3
	mov pc, lr	/* return */
```
## 실습!!
빵꾸난 이유!!
[]! 사용해서!!!
![](./0619_002.png)

## 블럭 전송 명령
// MEMCPY_SINGLE 찾아
```c
ldm r0,{r4,r5,r6}   // 읽기, 메모리에서 적어둔 개수만큼 읽어 들여, 3*4=12비트 읽어와
                    @읽기(r0=0x30100000)
ldm r0,{r4-r6,r9}   // 적을게 많을때는 -하이픈 사용!!
ldmia r0!,{r4-r6,r9}   // !는 갱신, r0가 갱신!! 16비트만큼 업데이트 +16
ldmda r0!,{r4-r6,r9}   // !는 갱신, r0가 갱신!! 16비트만큼 업데이트 -16


ldr r4,[r0]!
ldr r5,[r0]!
ldr r6,[r0]!
    @ 복사에 소요된 시간이 ldm이 훨씬 적게 걸린다!! ldm이 빨라!!
stm r1,{r4,r5,r6}   // 쓰기, r1의 값을 r4,r5,r6가 나눠서 쓰는 것 
                    @쓰기(r1=0x30110000)
stm r1,{r4-r6,r9}   
stm r1!,{r4-r6,r9}   

```
* dma가 제일 빠르고 그다음은 ldm 




```c
copy_loop_multiple:
	ldmia r1!,{r3-r8}
	stmia r0!,{r3-r8}
	sub r2,r2,#6	@ 2번 돌아 
	cmp r2, #0
	bgt copy_loop_multiple

	ldmfd sp!, {r4-r5, pc}	/* pop */
```
```c
	.globl  MEMCPY_MULTIPLE
MEMCPY_MULTIPLE:
	stmfd sp!, {r4-r5, lr} 	/* push */
	@ stmfd나 stmdb가 똑같아...?
	@ push할 때는 stmfd나 stmdb를 사용해야...
	/* IMPLEMENT HERE */
copy_loop_multiple:
	ldmia r1!,{r3-r5}
	stmia r0!,{r3-r5}
	sub r2,r2,#3	
	cmp r2, #0
	bgt copy_loop_multiple

	ldmfd sp!, {r4-r5, pc}	/* pop */
```

```c
	.globl  MEMCPY_MULTIPLE
MEMCPY_MULTIPLE:
	stmfd sp!, {r4-r5, lr} 	/* push */
	@ stmfd나 stmdb가 똑같아...?
	@ push할 때는 stmfd나 stmdb를 사용해야...
	/* IMPLEMENT HERE */
copy_loop_multiple:
	ldmia r1!,{r3-r5}
	stmia r0!,{r3-r5}
	sub r2,r2,#3	
	cmp r2, #0
	bgt copy_loop_multiple

	ldmfd sp!, {r4-r5, pc}^	/* pop, 인터럽트 핸들러 일 때만 ^를 붙일 수 있어,cpsr 복구 역할  */
```
* ldmia, ldmfd : 동작 같아, increment after load
* stmdb, stmfd : 동작 같아, decrement before store 
## ldm과 stm
![](./0619_006.png)
![](./0619_005.png)
![](./0619_003.png)
![](./0619_004.png)

## 명령어 뒤에 s
```c
	sub r2,r2,#3	
	cmp r2, #0
```
```c
    subs r2,r2,#3	
    /* 상태가 업그레이드 되므로 cmp 명령어가 필요없다.  */
```

## APCS(Arm/Thumb P,C,S)
```c
	.globl  MEMCPY_MULTIPLE
MEMCPY_MULTIPLE:
	stmfd sp!, {r0-r5, lr} 	/* push */
copy_loop_multiple:
	ldmia r1!,{r3-r5}
	stmia r0!,{r3-r5}
	subs r2,r2,#3	
	bgt copy_loop_multiple

	ldmfd sp!, {r0-r5, pc}^	/* pop */
```
R0,R1,R2,R3는 TEMP... 그외에는 PUSH,POP를 사용...?  
함수 호출하고 ...값이 같도록 하려고...?  
    stmfd sp!, {r4-r5, lr} 	/* push */  
	ldmfd sp!, {r4-r5, pc}	/* pop */  
    ldmfd sp!, {r0-r5, pc}	/* pop */ -> R0이기 때문에 에러!!  
    R0-3이외에 레지스터가 오염?되면 PUSH,POP을 안쓰면 안돼!!  
    ldmfd sp!, {r0-r5, pc}^	/* pop */ -> R0이기 때문에 에러!!  
    인터럽트 핸들러이면 r0-r3도 무시하면 안된다!!!  
    오염된 것 만 적는게.... 포인트....  
### lr의 한계

## arm코드와 thumb 코드
![](./0619_007.png)
### BX 명령어!!
* Branch and exchange : goto 하는데 동작모드(arm, thumb)를 바꾼다

## 기본적인 함수 형태
```c
	.globl  getIntNum
getIntNum:
	stmfd sp!,{r4-,lr}


	ldmfd sp!,{r4-,pc}
```
![](./0619_008.png)
![](./0619_009.png)
![](./0619_011.png)
![](./0619_012.png)

## cache 실습
```c
	MMU_EnableMMU();
	//MMU_EnableICache();	/* cache 사용 */
	MMU_DisableICache();
	//MMU_EnableDCache(); /* IMPORTANT : DCache should be turned on after MMU is turned on. */
	MMU_DisableDCache();
	// DCache가 효과적이다. 
```
* 코프로세서 레지스터 제어 방법
```c
	/* 코프로세서 레지스터를 읽는, READ */
    mrc  p15,0,r0,c1,c0,0
    /* 코프로세서 레지스터를 쓰는, WRITE */
	mcr  p15,0,r0,c1,c0,0
    /* 3블럭으로 나눌 수 있어
        1. p15,0
        2. r0
        3. c1,c0,0 
        2,3(c1)이 알짜배기!
        mrc는 c1이 r0 복사
        mcr는 r0가 c1에 복사
        */
```
```c
	/* 
	 * void MMU_EnableICache(void)
	 * -----------------------------------------------------------------
	 */
	.globl MMU_EnableICache
MMU_EnableICache:
	/* IMPLEMENT HERE */      
	mrc  p15,0,r0,c1,c0,0
	orr  r0,r0,#R1_I    /* 어떤 비트를 1로 만들겠다. */
	mcr  p15,0,r0,c1,c0,0
	mov	 pc,lr
```
![](./0619_013.png)

```c
	/* 
	 * void MMU_InvalidateIDCache(void)
     Invalidate : 캐시를 전부 무효화 시켜주는....? 
     void가 이상해, r0에 쓰레기 값이 들어가 있어
	 * -----------------------------------------------------------------
	 */
	.globl MMU_InvalidateIDCache
MMU_InvalidateIDCache:
	mcr  p15,0,r0,c7,c7,0
    /* 위와 같은 경우는 특별히..뒤에 c7,0을 다른 방식으로 해석해야해.... */
    /* rm,op2라고 해... */
	mov	 pc,lr
```

## MMU INIT
```c
	MMU_SetMTT(0x00000000,0x07f00000,0x00000000,RW_NCNB); //bank0
	/* 가상번지 첫, 가상번지 끝, 물리주소 번지, 00~07f~ 128메가  */
	MMU_SetMTT(0x08000000,0x0ff00000,0x08000000,RW_NCNB); //bank1
    /* RW_NCNB : 캐시 사용x , 캐시 오프 : 어떤 때 쓰라고?*/
	MMU_SetMTT(0x10000000,0x17f00000,0x10000000,RW_NCNB); //bank2
	MMU_SetMTT(0x18000000,0x1ff00000,0x18000000,RW_NCNB); //bank3
	MMU_SetMTT(0x20000000,0x27f00000,0x20000000,RW_NCNB); //bank4
	MMU_SetMTT(0x28000000,0x2ff00000,0x28000000,RW_NCNB); //bank5
    
    MMU_SetMTT(0x28000000,0x2ff00000,0x28000000,RW_CB); //bank5
    /* RW_CB라고 캐시를 활성화 시키라고 해야해!! 그럼 그 지정된 영역이내에 것은 2,12비트 그거 사용할 수 있어
    그 외에 것은 아무리 활성화해도 소용없어!!*/
    /* 0x28000000에서 280 3바이트만 사용... */
    
```

![](./0619_014.png)
![](./0619_015.png)
![](./0619_016.png)
![](./0619_017.png)

![](./book/전부삭제후_최적_페이지_001.jpg)

### MMU INIT
```c
    void MMU_Init(void)
    {
        unsigned long i,j;
        //========================== IMPORTANT NOTE =========================
        //The current stack and code area can't be re-mapped in this routine.
        //If you want memory map mapped freely, your own sophiscated MMU
        //initialization code is needed.
        //===================================================================

        MMU_DisableDCache();
        MMU_DisableICache();

        //If write-back is used,the DCache should be cleared.
        for(i=0;i<64;i++)
        for(j=0;j<8;j++)
                MMU_CleanInvalidateDCacheIndex((i<<26)|(j<<5));
        MMU_InvalidateICache();
        
        MMU_DisableMMU();
        MMU_InvalidateTLB();

        /* MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr) */
        MMU_SetMTT(0x00000000,0x07f00000,0x00000000,RW_NCNB); //bank0
        /* 가상번지 첫, 가상번지 끝, 물리주소 번지 */
        MMU_SetMTT(0x08000000,0x0ff00000,0x08000000,RW_NCNB); //bank1
        MMU_SetMTT(0x10000000,0x17f00000,0x10000000,RW_NCNB); //bank2
        MMU_SetMTT(0x18000000,0x1ff00000,0x18000000,RW_NCNB); //bank3
        MMU_SetMTT(0x20000000,0x27f00000,0x20000000,RW_NCNB); //bank4
        MMU_SetMTT(0x28000000,0x2ff00000,0x28000000,RW_NCNB); //bank5
        
        /* 
        * USER WILL TEST WITH BANK 6
        * the each attribute value such as RW_CB, RW_NCNB, .... was decleared on libc.h
        *
        * 1. Please IMPLEMENT MMU_SetMTT function
        */
        MMU_SetMTT(0x30000000,0x31f00000,0x30000000,RW_CB);   //bank6-1, TEXT/DATA
        MMU_SetMTT(0x32000000,0x33e00000,0x32000000,RW_CB);  //bank6-2, TEST DATA,LCD FrameBuffer
        MMU_SetMTT(0x33f00000,0x33f00000,0x33f00000,RW_CB);   //bank6-3
        MMU_SetMTT(0x38000000,0x3ff00000,0x38000000,RW_NCNB); //bank7
        
        MMU_SetMTT(0x40000000,0x5af00000,0x40000000,RW_NCNB);//SFR+StepSram    
        MMU_SetMTT(0x5b000000,0xfff00000,0x5b000000,RW_FAULT);//not used

        /* 
        * 2. Please IMPLEMENT MMU_SetTTBase function : libs.S 
        */
        /* _MMUTT_STARTADDRESS : r0의 테이블 주소? */
        MMU_SetTTBase(_MMUTT_STARTADDRESS);
        
        /* DOMAIN1: no_access, DOMAIN0,2~15=client(AP is checked) */
        MMU_SetDomain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR); 

        MMU_SetProcessId(0x0);
        MMU_EnableAlignFault();

        /* 
        * 3. Please IMPLEMENT MMU_EnableMMU,MMU_EnableICache and MMU_EnableDCache
        *     : libs.S 
        */
        MMU_EnableMMU();
        MMU_EnableICache();	/* cache 사용 */
        //MMU_DisableICache();
        MMU_EnableDCache(); /* IMPORTANT : DCache should be turned on after MMU is turned on. */
        //MMU_DisableDCache();
        // DCache가 효과적이다. 
    }   
```