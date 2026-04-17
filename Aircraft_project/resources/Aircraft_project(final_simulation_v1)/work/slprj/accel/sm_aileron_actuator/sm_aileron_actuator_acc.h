#ifndef RTW_HEADER_sm_aileron_actuator_acc_h_
#define RTW_HEADER_sm_aileron_actuator_acc_h_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn
#define S_FUNCTION_LEVEL 2
#ifndef RTW_GENERATED_S_FUNCTION
#define RTW_GENERATED_S_FUNCTION
#endif
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "sm_aileron_actuator_acc_types.h"
#include "mwmathutil.h"
#include <stddef.h>
extern "C" {
#include "rt_nonfinite.h"
}
#include <float.h>
extern "C" {
#include "rtGetInf.h"
}
#include "rt_defines.h"
struct B_sm_aileron_actuator_T { void * B_17_0_0 ; real_T B_18_1_8 [ 26 ] ;
real_T B_18_27_216 [ 10 ] ; real_T B_18_37_296 ; real_T B_18_38_304 ; real_T
B_18_39_312 [ 3 ] ; real_T B_18_42_336 ; real_T B_18_43_344 ; real_T
B_18_44_352 ; real_T B_18_45_360 ; real_T B_18_46_368 ; real_T B_18_47_376 ;
real_T B_18_48_384 ; real_T B_18_49_392 ; real_T B_18_50_400 ; real_T
B_18_51_408 ; real_T B_18_52_416 ; real_T B_18_53_424 ; real_T B_18_54_432 [
4 ] ; real_T B_18_58_464 ; real_T B_18_59_472 ; real_T B_18_60_480 ; real_T
B_18_61_488 ; real_T B_18_62_496 ; real_T B_18_63_504 ; real_T B_18_64_512 ;
real_T B_18_65_520 ; real_T B_18_66_528 ; real_T B_18_67_536 ; real_T
B_18_68_544 ; real_T B_18_69_552 ; real_T B_18_70_560 ; real_T B_18_71_568 ;
real_T B_18_72_576 ; real_T B_18_73_584 [ 4 ] ; real_T B_18_77_616 ; real_T
B_18_78_624 ; real_T B_18_79_632 [ 4 ] ; real_T B_18_83_664 ; real_T
B_18_84_672 [ 4 ] ; real_T B_18_88_704 [ 18 ] ; real_T B_18_106_848 [ 7 ] ;
real_T B_18_113_904 [ 4 ] ; real_T B_18_117_936 [ 4 ] ; real_T B_18_121_968 ;
real_T B_18_122_976 ; real_T B_18_123_984 ; real_T B_18_124_992 ; real_T
B_18_125_1000 ; real_T B_18_126_1008 ; real_T B_18_127_1016 ; real_T
B_18_128_1024 ; real_T B_18_129_1032 ; real_T B_18_130_1040 ; real_T
B_18_131_1048 ; real_T B_18_132_1056 ; real_T B_18_133_1064 ; real_T
B_18_134_1072 ; real_T B_18_135_1080 [ 4 ] ; real_T B_18_139_1112 ; real_T
B_18_140_1120 ; real_T B_18_141_1128 [ 4 ] ; real_T B_18_145_1160 ; real_T
B_18_146_1168 [ 4 ] ; real_T B_18_150_1200 [ 13 ] ; real_T B_18_163_1304 [ 7
] ; real_T B_18_170_1360 [ 4 ] ; real_T B_18_174_1392 [ 4 ] ; real_T
B_18_178_1424 ; real_T B_18_179_1432 ; real_T B_18_180_1440 ; real_T
B_18_181_1448 ; real_T B_18_182_1456 ; real_T B_18_183_1464 ; real_T
B_18_184_1472 ; real_T B_18_185_1480 ; real_T B_18_186_1488 ; real_T
B_18_187_1496 ; real_T B_18_188_1504 ; real_T B_18_189_1512 ; real_T
B_18_190_1520 ; real_T B_18_191_1528 ; real_T B_18_192_1536 [ 4 ] ; real_T
B_18_196_1568 ; real_T B_18_197_1576 ; real_T B_18_198_1584 [ 4 ] ; real_T
B_18_202_1616 ; real_T B_18_203_1624 [ 4 ] ; real_T B_18_207_1656 [ 12 ] ;
real_T B_18_219_1752 [ 7 ] ; real_T B_18_226_1808 [ 4 ] ; real_T
B_18_230_1840 [ 4 ] ; real_T B_18_234_1872 [ 3 ] ; real_T B_18_237_1896 ;
real_T B_18_238_1904 ; real_T B_18_239_1912 ; real_T B_18_240_1920 [ 5 ] ;
real_T B_18_245_1960 [ 2 ] ; real_T B_18_247_1976 ; real_T B_18_248_1984 ;
real_T B_18_249_1992 ; real_T B_18_250_2000 ; real_T B_18_251_2008 ; real_T
B_18_252_2016 ; real_T B_18_253_2024 [ 5 ] ; real_T B_18_258_2064 [ 2 ] ;
real_T B_18_260_2080 ; real_T B_18_261_2088 ; real_T B_18_262_2096 ; real_T
B_18_263_2104 ; real_T B_18_264_2112 ; real_T B_18_265_2120 [ 5 ] ; real_T
B_18_270_2160 [ 5 ] ; real_T B_18_275_2200 [ 5 ] ; real_T B_18_280_2240 [ 5 ]
; real_T B_18_285_2280 [ 5 ] ; real_T B_18_290_2320 [ 5 ] ; real_T
B_18_295_2360 [ 5 ] ; real_T B_18_300_2400 ; real_T B_18_301_2408 [ 9 ] ;
real_T B_18_310_2480 [ 9 ] ; real_T B_18_319_2552 [ 9 ] ; real_T
B_18_328_2624 ; real_T B_18_329_2632 ; real_T B_18_330_2640 ; real_T
B_18_331_2648 ; real_T B_18_332_2656 ; real_T B_18_333_2664 ; real_T
B_18_334_2672 ; real_T B_18_335_2680 ; real_T B_18_336_2688 ; real_T
B_18_337_2696 ; real_T B_18_338_2704 ; real_T B_18_339_2712 ; real_T
B_18_340_2720 ; real_T B_18_341_2728 ; real_T B_18_342_2736 ; real_T
B_18_343_2744 ; real_T B_18_344_2752 ; real_T B_18_345_2760 ; real_T
B_18_346_2768 ; real_T B_18_347_2776 ; real_T B_18_348_2784 [ 20 ] ; real_T
B_18_368_2944 [ 20 ] ; real_T B_18_388_3104 [ 20 ] ; real_T B_18_408_3264 [
20 ] ; real_T B_18_428_3424 [ 20 ] ; real_T B_18_448_3584 [ 20 ] ; real_T
B_18_468_3744 [ 20 ] ; real_T B_18_488_3904 ; real_T B_18_489_3912 ; real_T
B_18_490_3920 ; real_T B_18_491_3928 ; real_T B_18_492_3936 ; real_T
B_18_493_3944 [ 5 ] ; real_T B_18_498_3984 [ 5 ] ; real_T B_18_503_4024 [ 5 ]
; real_T B_18_508_4064 [ 5 ] ; real_T B_18_513_4104 [ 5 ] ; real_T
B_18_518_4144 ; real_T B_18_519_4152 ; real_T B_18_520_4160 ; real_T
B_18_521_4168 ; real_T B_18_522_4176 ; real_T B_18_523_4184 ; real_T
B_18_524_4192 ; real_T B_18_525_4200 ; real_T B_18_526_4208 ; real_T
B_18_527_4216 ; real_T B_18_528_4224 ; real_T B_18_529_4232 ; real_T
B_18_530_4240 ; real_T B_18_531_4248 [ 2 ] ; real_T B_18_533_4264 ; real_T
B_18_534_4272 ; real_T B_18_535_4280 ; real_T B_18_536_4288 ; real_T
B_18_537_4296 ; real_T B_18_538_4304 [ 2 ] ; real_T B_18_540_4320 ; real_T
B_18_541_4328 ; real_T B_18_542_4336 ; real_T B_18_543_4344 ; real_T
B_18_544_4352 ; real_T B_18_545_4360 ; real_T B_18_546_4368 ; real_T
B_18_547_4376 ; real_T B_18_548_4384 [ 2 ] ; real_T B_18_550_4400 ; real_T
B_18_551_4408 ; real_T B_18_552_4416 ; real_T B_18_553_4424 ; real_T
B_18_554_4432 ; real_T B_18_555_4440 [ 2 ] ; real_T B_18_557_4456 ; real_T
B_18_558_4464 ; real_T B_18_559_4472 ; real_T B_18_560_4480 ; real_T
B_18_561_4488 ; real_T B_18_562_4496 ; real_T B_18_563_4504 ; real_T
B_18_564_4512 ; real_T B_18_565_4520 ; real_T B_18_566_4528 ; real_T
B_18_567_4536 ; real_T B_18_568_4544 ; real_T B_18_569_4552 ; real_T
B_18_570_4560 ; real_T B_18_571_4568 ; real_T B_18_572_4576 ; real_T
B_18_573_4584 ; real_T B_18_574_4592 ; real_T B_18_575_4600 ; real_T
B_18_576_4608 ; real_T B_18_577_4616 ; real_T B_18_578_4624 ; real_T
B_18_579_4632 ; real_T B_18_580_4640 ; real_T B_18_581_4648 ; real_T
B_18_582_4656 ; real_T B_18_583_4664 ; real_T B_18_584_4672 ; real_T
B_18_585_4680 ; real_T B_18_586_4688 ; real_T B_18_587_4696 ; real_T
B_18_588_4704 ; real_T B_18_589_4712 ; real_T B_18_590_4720 ; real_T
B_18_591_4728 ; real_T B_18_592_4736 ; real_T B_18_593_4744 ; real_T
B_18_594_4752 ; real_T B_18_595_4760 ; real_T B_18_596_4768 ; real_T
B_18_597_4776 ; real_T B_18_598_4784 ; real_T B_18_599_4792 ; real_T
B_18_600_4800 ; real_T B_18_601_4808 ; real_T B_18_602_4816 ; real_T
B_18_603_4824 [ 4 ] ; real_T B_18_607_4856 [ 4 ] ; real_T B_18_611_4888 [ 4 ]
; real_T B_18_615_4920 [ 9 ] ; real_T B_18_624_4992 [ 9 ] ; real_T
B_18_633_5064 [ 9 ] ; real_T B_18_642_5136 [ 9 ] ; real_T B_18_651_5208 [ 9 ]
; real_T B_18_660_5280 [ 9 ] ; real_T B_18_669_5352 [ 9 ] ; real_T
B_18_678_5424 [ 9 ] ; real_T B_18_687_5496 [ 9 ] ; real_T B_18_696_5568 [ 9 ]
; real_T B_18_705_5640 [ 9 ] ; real_T B_18_714_5712 [ 9 ] ; real_T
B_18_723_5784 [ 9 ] ; real_T B_18_732_5856 [ 9 ] ; real_T B_18_741_5928 [ 9 ]
; real_T B_18_750_6000 [ 9 ] ; real_T B_18_759_6072 [ 9 ] ; real_T
B_18_768_6144 [ 9 ] ; real_T B_18_777_6216 [ 9 ] ; real_T B_18_786_6288 [ 9 ]
; real_T B_18_795_6360 [ 20 ] ; real_T B_18_815_6520 ; real_T B_18_816_6528 ;
real_T B_18_817_6536 [ 20 ] ; real_T B_18_837_6696 [ 20 ] ; real_T
B_18_857_6856 [ 20 ] ; real_T B_18_877_7016 [ 20 ] ; real_T B_18_897_7176 [
20 ] ; real_T B_18_917_7336 [ 5 ] ; real_T B_18_922_7376 ; real_T
B_18_923_7384 ; real_T B_18_924_7392 ; real_T B_18_925_7400 [ 5 ] ; real_T
B_18_930_7440 [ 5 ] ; real_T B_18_935_7480 ; real_T B_18_936_7488 [ 5 ] ;
real_T B_18_941_7528 ; real_T B_18_942_7536 [ 5 ] ; real_T B_18_947_7576 [ 5
] ; real_T B_18_952_7616 ; real_T B_18_953_7624 [ 5 ] ; real_T B_18_958_7664
; real_T B_18_959_7672 [ 5 ] ; real_T B_18_964_7712 ; real_T B_18_965_7720 [
5 ] ; real_T B_18_970_7760 ; real_T B_18_971_7768 ; real_T B_18_972_7776 [ 5
] ; real_T B_18_977_7816 ; real_T B_18_978_7824 ; real_T B_18_979_7832 ;
real_T B_18_980_7840 [ 5 ] ; real_T B_18_985_7880 ; real_T B_18_986_7888 ;
real_T B_18_987_7896 ; real_T B_18_988_7904 [ 5 ] ; real_T B_18_993_7944 ;
real_T B_18_994_7952 ; real_T B_18_995_7960 ; real_T B_18_996_7968 ; real_T
B_18_997_7976 ; real_T B_18_998_7984 ; real_T B_18_999_7992 ; real_T
B_18_1000_8000 ; real_T B_18_1001_8008 ; real_T B_18_1002_8016 ; real_T
B_18_1003_8024 ; real_T B_18_1004_8032 ; real_T B_18_1005_8040 ; real_T
B_18_1006_8048 ; real_T B_18_1007_8056 ; real_T B_18_1008_8064 ; real_T
B_18_1009_8072 ; real_T B_17_1010_8080 ; real_T B_16_1011_8088 [ 40 ] ;
real_T B_16_1051_8408 [ 5 ] ; real_T B_16_1056_8448 ; real_T B_15_1057_8456 ;
real_T B_14_1058_8464 ; real_T B_13_1059_8472 ; real_T B_12_1060_8480 ;
real_T B_11_1061_8488 ; real_T B_10_1062_8496 ; real_T B_9_1063_8504 ; real_T
B_8_1064_8512 ; real_T B_7_1065_8520 ; real_T B_6_1066_8528 ; real_T
B_5_1067_8536 ; real_T B_4_1068_8544 ; real_T B_3_1069_8552 ; real_T
B_2_1070_8560 ; real_T B_1_1071_8568 ; real_T B_0_1072_8576 ; int32_T
B_18_1073_8584 ; int32_T B_18_1074_8588 ; boolean_T B_18_1075_8592 ;
boolean_T B_18_1076_8593 ; boolean_T B_18_1077_8594 ; boolean_T
B_18_1078_8595 ; boolean_T B_18_1079_8596 ; boolean_T B_18_1080_8597 ;
boolean_T B_18_1081_8598 ; boolean_T B_18_1082_8599 ; boolean_T
B_18_1083_8600 ; boolean_T B_18_1084_8601 ; boolean_T B_18_1085_8602 ;
boolean_T B_18_1086_8603 ; boolean_T B_18_1087_8604 ; boolean_T
B_18_1088_8605 ; boolean_T B_18_1089_8606 ; boolean_T B_18_1090_8607 ;
boolean_T B_18_1091_8608 ; boolean_T B_18_1092_8609 ; boolean_T
B_18_1093_8610 ; boolean_T B_18_1094_8611 ; boolean_T B_18_1095_8612 ;
boolean_T B_18_1096_8613 ; boolean_T B_18_1097_8614 ; boolean_T
B_18_1098_8615 ; boolean_T B_18_1099_8616 ; boolean_T B_18_1100_8617 ;
boolean_T B_18_1101_8618 ; boolean_T B_18_1102_8619 ; char_T
pad_B_18_1102_8619 [ 4 ] ; } ; struct DW_sm_aileron_actuator_T { real_T
INTERNAL_1_1_1_Discrete ; real_T INTERNAL_1_1_1_FirstOutput ; real_T
INPUT_2_1_1_Discrete [ 2 ] ; real_T INPUT_1_1_1_Discrete [ 2 ] ; real_T
INPUT_3_1_1_Discrete [ 2 ] ; real_T STATE_4_Discrete [ 3 ] ; real_T
INTERNAL_2_1_1_Discrete [ 2 ] ; real_T INTERNAL_3_1_1_Discrete ; real_T
INTERNAL_3_1_1_FirstOutput ; real_T INPUT_5_1_1_Discrete [ 2 ] ; real_T
INPUT_4_1_1_Discrete [ 2 ] ; real_T INPUT_6_1_1_Discrete [ 2 ] ; real_T
INTERNAL_4_1_1_Discrete [ 2 ] ; real_T INTERNAL_5_1_1_Discrete ; real_T
INTERNAL_5_1_1_FirstOutput ; real_T INPUT_8_1_1_Discrete [ 2 ] ; real_T
INPUT_7_1_1_Discrete [ 2 ] ; real_T INPUT_9_1_1_Discrete [ 2 ] ; real_T
INTERNAL_6_1_1_Discrete [ 2 ] ; real_T INPUT_10_1_1_Discrete [ 2 ] ; real_T
UD_DSTATE ; real_T Integrator_DSTATE ; real_T Integrator_DSTATE_j ; real_T
UD_DSTATE_a ; real_T UD_DSTATE_p ; real_T Integrator_DSTATE_l ; real_T
Integrator_DSTATE_e ; real_T UD_DSTATE_h ; real_T Integrator_DSTATE_f ;
real_T Integrator_DSTATE_lo ; real_T UD_DSTATE_i ; real_T
INPUT_1_1_1_Discrete_b [ 2 ] ; real_T INPUT_2_1_1_Discrete_k [ 2 ] ; real_T
INPUT_3_1_1_Discrete_l [ 2 ] ; real_T LTI_STATE_1_FirstOutput ; real_T
STATE_1_Discrete ; real_T OUTPUT_1_0_Discrete ; real_T OUTPUT_4_0_Discrete ;
real_T STATE_3_Discrete ; real_T OUTPUT_3_0_Discrete ; real_T
STATE_2_Discrete ; real_T OUTPUT_2_0_Discrete ; real_T OUTPUT_1_1_Discrete ;
real_T TimeStampA ; real_T LastUAtTimeA ; real_T TimeStampB ; real_T
LastUAtTimeB ; real_T Memory5_PreviousInput [ 5 ] ; real_T
Memory6_PreviousInput ; real_T Memory_PreviousInput ; real_T
Memory_PreviousInput_j ; real_T Memory1_PreviousInput ; real_T TimeStampA_j ;
real_T LastUAtTimeA_o ; real_T TimeStampB_f ; real_T LastUAtTimeB_n ; real_T
Memory_PreviousInput_p ; real_T OUTPUT_4_1_Discrete ; real_T
OUTPUT_3_1_Discrete ; real_T OUTPUT_2_1_Discrete ; struct { real_T
modelTStart ; } TransportDelay_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_i ; struct { real_T modelTStart ; }
TransportDelay1_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_h ; void * STATE_1_Simulator ; void * STATE_1_SimData ;
void * STATE_1_DiagMgr ; void * STATE_1_ZcLogger ; void * STATE_1_TsInfo ;
void * OUTPUT_1_0_Simulator ; void * OUTPUT_1_0_SimData ; void *
OUTPUT_1_0_DiagMgr ; void * OUTPUT_1_0_ZcLogger ; void * OUTPUT_1_0_TsInfo ;
void * FromWs_PWORK [ 3 ] ; void * STATE_4_Simulator ; void * STATE_4_SimData
; void * STATE_4_DiagMgr ; void * STATE_4_ZcLogger ; void * STATE_4_TsInfo ;
void * OUTPUT_4_0_Simulator ; void * OUTPUT_4_0_SimData ; void *
OUTPUT_4_0_DiagMgr ; void * OUTPUT_4_0_ZcLogger ; void * OUTPUT_4_0_TsInfo ;
void * STATE_3_Simulator ; void * STATE_3_SimData ; void * STATE_3_DiagMgr ;
void * STATE_3_ZcLogger ; void * STATE_3_TsInfo ; void * OUTPUT_3_0_Simulator
; void * OUTPUT_3_0_SimData ; void * OUTPUT_3_0_DiagMgr ; void *
OUTPUT_3_0_ZcLogger ; void * OUTPUT_3_0_TsInfo ; void * STATE_2_Simulator ;
void * STATE_2_SimData ; void * STATE_2_DiagMgr ; void * STATE_2_ZcLogger ;
void * STATE_2_TsInfo ; void * OUTPUT_2_0_Simulator ; void *
OUTPUT_2_0_SimData ; void * OUTPUT_2_0_DiagMgr ; void * OUTPUT_2_0_ZcLogger ;
void * OUTPUT_2_0_TsInfo ; void * OUTPUT_1_1_Simulator ; void *
OUTPUT_1_1_SimData ; void * OUTPUT_1_1_DiagMgr ; void * OUTPUT_1_1_ZcLogger ;
void * OUTPUT_1_1_TsInfo ; void * Scope3_PWORK [ 2 ] ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_Actuator_at_outport_0_PWORK ; struct
{ void * AQHandles ; } TAQSigLogging_InsertedFor_From_at_outport_0_PWORK ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Mechanical_at_outport_0_PWORK ; void * Scope_PWORK
; struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ; void *
Displacemet_change1_PWORK [ 6 ] ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_n ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay1_PWORK ; void * OUTPUT_4_1_Simulator ; void *
OUTPUT_4_1_SimData ; void * OUTPUT_4_1_DiagMgr ; void * OUTPUT_4_1_ZcLogger ;
void * OUTPUT_4_1_TsInfo ; void * PWM_PWORK ; void * OUTPUT_3_1_Simulator ;
void * OUTPUT_3_1_SimData ; void * OUTPUT_3_1_DiagMgr ; void *
OUTPUT_3_1_ZcLogger ; void * OUTPUT_3_1_TsInfo ; void * PWM_PWORK_f ; void *
OUTPUT_2_1_Simulator ; void * OUTPUT_2_1_SimData ; void * OUTPUT_2_1_DiagMgr
; void * OUTPUT_2_1_ZcLogger ; void * OUTPUT_2_1_TsInfo ; void * PWM_PWORK_b
; void * Scope2_PWORK [ 2 ] ; void * Scope1_PWORK [ 2 ] ; void *
Scope_PWORK_j [ 2 ] ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_d ; void * LTI_SINK_1_RtwLogger ; void *
LTI_SINK_1_RtwLogBuffer ; void * LTI_SINK_1_RtwLogFcnManager ; void *
SINK_1_RtwLogger ; void * SINK_1_RtwLogBuffer ; void *
SINK_1_RtwLogFcnManager ; void * SINK_2_RtwLogger ; void *
SINK_2_RtwLogBuffer ; void * SINK_2_RtwLogFcnManager ; void *
SINK_3_RtwLogger ; void * SINK_3_RtwLogBuffer ; void *
SINK_3_RtwLogFcnManager ; void * SINK_4_RtwLogger ; void *
SINK_4_RtwLogBuffer ; void * SINK_4_RtwLogFcnManager ; void *
RTP_2_RtpManager ; void * RTP_3_RtpManager ; void * RTP_4_RtpManager ;
int32_T MATLABFunction3_sysIdxToRun ; int32_T MATLABFunction1_sysIdxToRun ;
int32_T Diagnoser1_sysIdxToRun ; int32_T MedianFilter2_sysIdxToRun ; int32_T
MedianFilter1_sysIdxToRun ; int32_T MedianFilter_sysIdxToRun ; int32_T
MedianFilter2_sysIdxToRun_k ; int32_T MedianFilter1_sysIdxToRun_o ; int32_T
MedianFilter_sysIdxToRun_m ; int32_T MATLABFunction_sysIdxToRun ; int32_T
MATLABFunction_sysIdxToRun_h ; int32_T MATLABFunction_sysIdxToRun_i ; int32_T
Diagnoser3_sysIdxToRun ; int32_T Diagnoser1_sysIdxToRun_h ; int32_T
DIagnoser2_sysIdxToRun ; int32_T DES_Stateflow3_sysIdxToRun ; int32_T
DES_Stateflow2_sysIdxToRun ; int32_T DES_Stateflow1_sysIdxToRun ; int_T
STATE_1_Modes ; int_T OUTPUT_1_0_Modes ; int_T FromWs_IWORK ; int_T
STATE_4_Modes [ 7 ] ; int_T OUTPUT_4_0_Modes ; int_T STATE_3_Modes [ 4 ] ;
int_T OUTPUT_3_0_Modes ; int_T STATE_2_Modes [ 4 ] ; int_T OUTPUT_2_0_Modes ;
int_T OUTPUT_1_1_Modes ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK ; struct
{ int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_i ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay1_IWORK ; int_T OUTPUT_4_1_Modes ; int_T OUTPUT_3_1_Modes ;
int_T OUTPUT_2_1_Modes ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_n ;
int32_T STATE_4_MASS_MATRIX_PR ; int32_T STATE_3_MASS_MATRIX_PR ; int32_T
STATE_2_MASS_MATRIX_PR ; int8_T Integrator_PrevResetState ; int8_T
Integrator_PrevResetState_m ; int8_T Integrator_PrevResetState_f ; int8_T
Integrator_PrevResetState_ms ; int8_T Integrator_PrevResetState_p ; int8_T
Integrator_PrevResetState_k ; uint8_T Integrator_IC_LOADING ; uint8_T
Integrator_IC_LOADING_h ; uint8_T Integrator_IC_LOADING_a ; uint8_T
Integrator_IC_LOADING_m ; uint8_T Integrator_IC_LOADING_j ; uint8_T
Integrator_IC_LOADING_k ; boolean_T STATE_1_FirstOutput ; boolean_T
OUTPUT_1_0_FirstOutput ; boolean_T STATE_4_FirstOutput ; boolean_T
OUTPUT_4_0_FirstOutput ; boolean_T STATE_3_FirstOutput ; boolean_T
OUTPUT_3_0_FirstOutput ; boolean_T STATE_2_FirstOutput ; boolean_T
OUTPUT_2_0_FirstOutput ; boolean_T OUTPUT_1_1_FirstOutput ; boolean_T
Memory4_PreviousInput ; boolean_T Memory2_PreviousInput ; boolean_T
OUTPUT_4_1_FirstOutput ; boolean_T OUTPUT_3_1_FirstOutput ; boolean_T
OUTPUT_2_1_FirstOutput ; boolean_T LTI_STATE_1_ReInit ; boolean_T
RTP_2_SetParametersNeeded ; boolean_T RTP_3_SetParametersNeeded ; boolean_T
RTP_4_SetParametersNeeded ; char_T pad_RTP_4_SetParametersNeeded [ 2 ] ; } ;
struct X_sm_aileron_actuator_T { real_T
sm_aileron_actuatorMechanicalRev_MR_ActMountRzq [ 26 ] ; real_T
sm_aileron_actuatoroutputFiltered_1513730075_0 ; real_T u5_CSTATE ; real_T
u0HzLowpassFilter_CSTATE ; real_T Limits55_CSTATE ; real_T
u0HzLowpassFilter_CSTATE_e ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_1DC_Motorw [ 10 ] ; real_T
sm_aileron_actuatoroutputFiltered_553247611_0 ; real_T u5_CSTATE_f ; real_T
u0HzLowpassFilter_CSTATE_d ; real_T Limits55_CSTATE_g ; real_T
u0HzLowpassFilter_CSTATE_b ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_2DC_Motorw [ 9 ] ; real_T
sm_aileron_actuatoroutputFiltered_2948142811_0 ; real_T u5_CSTATE_n ; real_T
u0HzLowpassFilter_CSTATE_i ; real_T Limits55_CSTATE_p ; real_T
u0HzLowpassFilter_CSTATE_p ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_3DC_Motorw [ 8 ] ; } ; struct
XDot_sm_aileron_actuator_T { real_T
sm_aileron_actuatorMechanicalRev_MR_ActMountRzq [ 26 ] ; real_T
sm_aileron_actuatoroutputFiltered_1513730075_0 ; real_T u5_CSTATE ; real_T
u0HzLowpassFilter_CSTATE ; real_T Limits55_CSTATE ; real_T
u0HzLowpassFilter_CSTATE_e ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_1DC_Motorw [ 10 ] ; real_T
sm_aileron_actuatoroutputFiltered_553247611_0 ; real_T u5_CSTATE_f ; real_T
u0HzLowpassFilter_CSTATE_d ; real_T Limits55_CSTATE_g ; real_T
u0HzLowpassFilter_CSTATE_b ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_2DC_Motorw [ 9 ] ; real_T
sm_aileron_actuatoroutputFiltered_2948142811_0 ; real_T u5_CSTATE_n ; real_T
u0HzLowpassFilter_CSTATE_i ; real_T Limits55_CSTATE_p ; real_T
u0HzLowpassFilter_CSTATE_p ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_3DC_Motorw [ 8 ] ; } ; struct
XDis_sm_aileron_actuator_T { boolean_T
sm_aileron_actuatorMechanicalRev_MR_ActMountRzq [ 26 ] ; boolean_T
sm_aileron_actuatoroutputFiltered_1513730075_0 ; boolean_T u5_CSTATE ;
boolean_T u0HzLowpassFilter_CSTATE ; boolean_T Limits55_CSTATE ; boolean_T
u0HzLowpassFilter_CSTATE_e ; boolean_T
sm_aileron_actuatorActuatorElectricLeadscrew_1DC_Motorw [ 10 ] ; boolean_T
sm_aileron_actuatoroutputFiltered_553247611_0 ; boolean_T u5_CSTATE_f ;
boolean_T u0HzLowpassFilter_CSTATE_d ; boolean_T Limits55_CSTATE_g ;
boolean_T u0HzLowpassFilter_CSTATE_b ; boolean_T
sm_aileron_actuatorActuatorElectricLeadscrew_2DC_Motorw [ 9 ] ; boolean_T
sm_aileron_actuatoroutputFiltered_2948142811_0 ; boolean_T u5_CSTATE_n ;
boolean_T u0HzLowpassFilter_CSTATE_i ; boolean_T Limits55_CSTATE_p ;
boolean_T u0HzLowpassFilter_CSTATE_p ; boolean_T
sm_aileron_actuatorActuatorElectricLeadscrew_3DC_Motorw [ 8 ] ; } ; struct
MassMatrix_sm_aileron_actuator_T { int_T ir [ 57 ] ; int_T jc [ 69 ] ; real_T
pr [ 57 ] ; } ; struct ConstB_sm_aileron_actuator_T { real_T B_18_1103_8624 ;
real_T B_18_1104_8632 ; real_T B_18_1105_8640 ; real_T B_18_1106_8648 ;
real_T B_18_1107_8656 ; real_T B_18_1108_8664 ; } ;
#define sm_aileron_actuator_rtC(S) ((ConstB_sm_aileron_actuator_T *) _ssGetConstBlockIO(S))
struct P_sm_aileron_actuator_T_ { real_T P_0 [ 2 ] ; real_T P_1 ; real_T P_2
[ 2 ] ; real_T P_3 ; real_T P_4 [ 2 ] ; real_T P_5 ; real_T P_6 [ 2 ] ;
real_T P_7 ; real_T P_8 [ 2 ] ; real_T P_9 ; real_T P_10 [ 2 ] ; real_T P_11
; real_T P_12 [ 2 ] ; real_T P_13 ; real_T P_14 [ 2 ] ; real_T P_15 ; real_T
P_16 [ 2 ] ; real_T P_17 ; real_T P_18 [ 2 ] ; real_T P_19 ; real_T P_20 [ 2
] ; real_T P_21 ; real_T P_22 [ 2 ] ; real_T P_23 ; real_T P_24 [ 2 ] ;
real_T P_25 ; real_T P_26 [ 2 ] ; real_T P_27 ; real_T P_28 [ 2 ] ; real_T
P_29 ; real_T P_30 [ 2 ] ; real_T P_31 ; real_T P_32 ; real_T P_33 ; real_T
P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39 [
11 ] ; real_T P_40 [ 11 ] ; real_T P_41 [ 11 ] ; real_T P_42 [ 11 ] ; real_T
P_43 [ 11 ] ; real_T P_44 [ 11 ] ; real_T P_45 ; real_T P_46 ; real_T P_47 [
27 ] ; real_T P_48 [ 27 ] ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T
P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ;
real_T P_58 ; real_T P_59 ; real_T P_60 ; real_T P_61 ; real_T P_62 ; real_T
P_63 ; real_T P_64 ; real_T P_65 ; real_T P_66 ; real_T P_67 ; real_T P_68 ;
real_T P_69 ; real_T P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 ; real_T
P_74 ; real_T P_75 ; real_T P_76 ; real_T P_77 ; real_T P_78 ; real_T P_79 ;
real_T P_80 ; real_T P_81 ; real_T P_82 ; real_T P_83 ; real_T P_84 ; real_T
P_85 ; real_T P_86 ; real_T P_87 ; real_T P_88 ; real_T P_89 ; real_T P_90 ;
real_T P_91 ; real_T P_92 ; real_T P_93 ; real_T P_94 ; real_T P_95 ; real_T
P_96 ; real_T P_97 ; real_T P_98 ; real_T P_99 ; real_T P_100 ; real_T P_101
; real_T P_102 ; real_T P_103 ; real_T P_104 ; real_T P_105 ; real_T P_106 ;
real_T P_107 ; real_T P_108 ; real_T P_109 ; real_T P_110 ; real_T P_111 ;
real_T P_112 ; real_T P_113 ; real_T P_114 ; real_T P_115 ; real_T P_116 ;
real_T P_117 ; real_T P_118 ; real_T P_119 [ 5 ] ; real_T P_120 ; real_T
P_121 [ 9 ] ; real_T P_122 [ 9 ] ; real_T P_123 ; real_T P_124 ; real_T P_125
; real_T P_126 ; real_T P_127 [ 5 ] ; real_T P_128 [ 5 ] ; real_T P_129 ;
real_T P_130 ; real_T P_131 ; real_T P_132 ; real_T P_133 ; real_T P_134 ;
real_T P_135 ; real_T P_136 ; real_T P_137 ; real_T P_138 ; real_T P_139 ;
real_T P_140 ; real_T P_141 ; real_T P_142 ; real_T P_143 ; real_T P_144 ;
real_T P_145 ; real_T P_146 ; real_T P_147 ; real_T P_148 ; real_T P_149 ;
real_T P_150 ; real_T P_151 ; real_T P_152 ; real_T P_153 ; real_T P_154 ;
real_T P_155 ; real_T P_156 ; real_T P_157 ; real_T P_158 ; real_T P_159 ;
real_T P_160 ; real_T P_161 ; real_T P_162 ; real_T P_163 ; real_T P_164 ;
real_T P_165 ; real_T P_166 ; real_T P_167 ; real_T P_168 ; real_T P_169 ;
real_T P_170 ; real_T P_171 ; real_T P_172 ; real_T P_173 ; real_T P_174 ;
real_T P_175 ; real_T P_176 ; real_T P_177 ; real_T P_178 ; real_T P_179 ;
real_T P_180 ; real_T P_181 ; real_T P_182 ; real_T P_183 ; real_T P_184 ;
real_T P_185 ; real_T P_186 [ 9 ] ; real_T P_187 [ 9 ] ; real_T P_188 [ 9 ] ;
real_T P_189 [ 9 ] ; real_T P_190 [ 9 ] ; real_T P_191 [ 9 ] ; real_T P_192 [
9 ] ; real_T P_193 [ 9 ] ; real_T P_194 [ 9 ] ; real_T P_195 [ 9 ] ; real_T
P_196 [ 9 ] ; real_T P_197 [ 9 ] ; real_T P_198 [ 9 ] ; real_T P_199 [ 9 ] ;
real_T P_200 [ 9 ] ; real_T P_201 [ 9 ] ; real_T P_202 [ 9 ] ; real_T P_203 [
9 ] ; real_T P_204 [ 9 ] ; real_T P_205 [ 9 ] ; real_T P_206 [ 20 ] ; real_T
P_207 ; real_T P_208 ; real_T P_209 [ 20 ] ; real_T P_210 [ 20 ] ; real_T
P_211 [ 20 ] ; real_T P_212 [ 20 ] ; real_T P_213 [ 20 ] ; real_T P_214 [ 5 ]
; real_T P_215 ; real_T P_216 ; real_T P_217 ; real_T P_218 [ 5 ] ; real_T
P_219 [ 5 ] ; real_T P_220 ; real_T P_221 [ 5 ] ; real_T P_222 ; real_T P_223
[ 5 ] ; real_T P_224 [ 5 ] ; real_T P_225 ; real_T P_226 [ 5 ] ; real_T P_227
; real_T P_228 [ 5 ] ; real_T P_229 ; real_T P_230 [ 5 ] ; real_T P_231 ;
real_T P_232 ; real_T P_233 [ 5 ] ; real_T P_234 ; real_T P_235 ; real_T
P_236 ; real_T P_237 [ 5 ] ; real_T P_238 ; real_T P_239 ; real_T P_240 ;
real_T P_241 [ 5 ] ; real_T P_242 ; real_T P_243 ; real_T P_244 ; real_T
P_245 ; real_T P_246 ; real_T P_247 ; real_T P_248 ; real_T P_249 ; real_T
P_250 ; real_T P_251 ; real_T P_252 ; real_T P_253 ; real_T P_254 ; real_T
P_255 ; boolean_T P_256 ; boolean_T P_257 ; char_T pad_P_257 [ 6 ] ; } ;
extern P_sm_aileron_actuator_T sm_aileron_actuator_rtDefaultP ; extern const
ConstB_sm_aileron_actuator_T sm_aileron_actuator_rtInvariant ;
#endif
