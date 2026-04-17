#ifndef sm_aileron_actuator_acc_h_
#define sm_aileron_actuator_acc_h_
#include <cmath>
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
#include "rt_nonfinite.h"
#include "sm_aileron_actuator_acc_types.h"
#include "mwmathutil.h"
#include <stddef.h>
extern "C" {
#include "rtGetInf.h"
}
#include "rt_defines.h"
struct B_findnode_sm_aileron_actuator_T { real_T B_8_1662_12856 ; real_T
B_6_1663_12864 ; real_T B_6_1664_12872 ; real_T B_6_1665_12880 [ 51 ] ;
uint8_T B_8_1716_13288 ; uint8_T B_8_1717_13289 ; uint8_T B_8_1718_13290 ;
uint8_T B_8_1719_13291 ; uint8_T B_8_1720_13292 [ 51 ] ; uint8_T
B_7_1771_13343 ; uint8_T B_6_1772_13344 ; uint8_T B_6_1773_13345 [ 102 ] ;
uint8_T B_5_1875_13447 ; uint8_T B_4_1876_13448 ; boolean_T B_8_1877_13449 ;
boolean_T B_8_1878_13450 ; boolean_T B_8_1879_13451 ; boolean_T
B_8_1880_13452 ; boolean_T B_8_1881_13453 ; boolean_T B_8_1882_13454 ;
boolean_T B_8_1883_13455 ; boolean_T B_8_1884_13456 ; boolean_T
B_8_1885_13457 [ 51 ] ; boolean_T B_6_1936_13508 ; boolean_T B_3_1937_13509 ;
char_T pad_B_3_1937_13509 [ 2 ] ; } ; struct
DW_findnode_sm_aileron_actuator_T { int32_T findnode_sysIdxToRun ; int32_T
Returnm_sysIdxToRun ; int32_T Updatem_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport3_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitch1Inport1_sysIdxToRun ; uint8_T Nodevariablem_DSTATE ;
int8_T Returnm_SubsysRanBC ; int8_T Updatem_SubsysRanBC ; char_T
pad_Updatem_SubsysRanBC [ 5 ] ; } ; struct B_sm_aileron_actuator_T { void *
B_23_0_0 ; real_T B_24_1_8 [ 26 ] ; real_T B_24_27_216 [ 10 ] ; real_T
B_24_37_296 ; real_T B_24_38_304 ; real_T B_24_39_312 [ 3 ] ; real_T
B_24_42_336 ; real_T B_24_43_344 ; real_T B_24_44_352 ; real_T B_24_45_360 ;
real_T B_24_46_368 ; real_T B_24_47_376 ; real_T B_24_48_384 ; real_T
B_24_49_392 ; real_T B_24_50_400 ; real_T B_24_51_408 ; real_T B_24_52_416 ;
real_T B_24_53_424 ; real_T B_24_54_432 [ 4 ] ; real_T B_24_58_464 ; real_T
B_24_59_472 ; real_T B_24_60_480 ; real_T B_24_61_488 ; real_T B_24_62_496 ;
real_T B_24_63_504 ; real_T B_24_64_512 ; real_T B_24_65_520 ; real_T
B_24_66_528 ; real_T B_24_67_536 ; real_T B_24_68_544 ; real_T B_24_69_552 ;
real_T B_24_70_560 ; real_T B_24_71_568 ; real_T B_24_72_576 ; real_T
B_24_73_584 [ 4 ] ; real_T B_24_77_616 ; real_T B_24_78_624 ; real_T
B_24_79_632 [ 4 ] ; real_T B_24_83_664 ; real_T B_24_84_672 [ 4 ] ; real_T
B_24_88_704 [ 13 ] ; real_T B_24_101_808 [ 7 ] ; real_T B_24_108_864 [ 4 ] ;
real_T B_24_112_896 [ 4 ] ; real_T B_24_116_928 ; real_T B_24_117_936 ;
real_T B_24_118_944 ; real_T B_24_119_952 ; real_T B_24_120_960 ; real_T
B_24_121_968 ; real_T B_24_122_976 ; real_T B_24_123_984 ; real_T
B_24_124_992 ; real_T B_24_125_1000 ; real_T B_24_126_1008 ; real_T
B_24_127_1016 ; real_T B_24_128_1024 ; real_T B_24_129_1032 ; real_T
B_24_130_1040 [ 4 ] ; real_T B_24_134_1072 ; real_T B_24_135_1080 ; real_T
B_24_136_1088 [ 4 ] ; real_T B_24_140_1120 ; real_T B_24_141_1128 [ 4 ] ;
real_T B_24_145_1160 [ 13 ] ; real_T B_24_158_1264 [ 7 ] ; real_T
B_24_165_1320 [ 4 ] ; real_T B_24_169_1352 [ 4 ] ; real_T B_24_173_1384 ;
real_T B_24_174_1392 ; real_T B_24_175_1400 ; real_T B_24_176_1408 ; real_T
B_24_177_1416 ; real_T B_24_178_1424 ; real_T B_24_179_1432 ; real_T
B_24_180_1440 ; real_T B_24_181_1448 ; real_T B_24_182_1456 ; real_T
B_24_183_1464 ; real_T B_24_184_1472 ; real_T B_24_185_1480 ; real_T
B_24_186_1488 ; real_T B_24_187_1496 [ 4 ] ; real_T B_24_191_1528 ; real_T
B_24_192_1536 ; real_T B_24_193_1544 [ 4 ] ; real_T B_24_197_1576 ; real_T
B_24_198_1584 [ 4 ] ; real_T B_24_202_1616 [ 12 ] ; real_T B_24_214_1712 [ 7
] ; real_T B_24_221_1768 [ 4 ] ; real_T B_24_225_1800 [ 4 ] ; real_T
B_24_229_1832 [ 3 ] ; real_T B_24_232_1856 [ 2 ] ; real_T B_24_234_1872 ;
real_T B_24_235_1880 ; real_T B_24_236_1888 ; real_T B_24_237_1896 ; real_T
B_24_238_1904 ; real_T B_24_239_1912 ; real_T B_24_240_1920 ; real_T
B_24_241_1928 ; real_T B_24_242_1936 ; real_T B_24_243_1944 ; real_T
B_24_244_1952 ; real_T B_24_245_1960 [ 2 ] ; real_T B_24_247_1976 ; real_T
B_24_248_1984 ; real_T B_24_249_1992 ; real_T B_24_250_2000 ; real_T
B_24_251_2008 ; real_T B_24_252_2016 ; real_T B_24_253_2024 ; real_T
B_24_254_2032 ; real_T B_24_255_2040 ; real_T B_24_256_2048 ; real_T
B_24_257_2056 ; real_T B_24_258_2064 ; real_T B_24_259_2072 [ 2 ] ; real_T
B_24_261_2088 ; real_T B_24_262_2096 ; real_T B_24_263_2104 ; real_T
B_24_264_2112 ; real_T B_24_265_2120 ; real_T B_24_266_2128 [ 2 ] ; real_T
B_24_268_2144 ; real_T B_24_269_2152 ; real_T B_24_270_2160 ; real_T
B_24_271_2168 ; real_T B_24_272_2176 ; real_T B_24_273_2184 ; real_T
B_24_274_2192 ; real_T B_24_275_2200 ; real_T B_24_276_2208 [ 2 ] ; real_T
B_24_278_2224 ; real_T B_24_279_2232 ; real_T B_24_280_2240 ; real_T
B_24_281_2248 ; real_T B_24_282_2256 ; real_T B_24_283_2264 [ 2 ] ; real_T
B_24_285_2280 ; real_T B_24_286_2288 ; real_T B_24_287_2296 ; real_T
B_24_288_2304 ; real_T B_24_289_2312 ; real_T B_24_290_2320 ; real_T
B_24_291_2328 ; real_T B_24_292_2336 ; real_T B_24_293_2344 [ 5 ] ; real_T
B_24_298_2384 ; real_T B_24_299_2392 [ 5 ] ; real_T B_24_304_2432 [ 5 ] ;
real_T B_24_309_2472 [ 5 ] ; real_T B_24_314_2512 [ 5 ] ; real_T
B_24_319_2552 [ 5 ] ; real_T B_24_324_2592 [ 5 ] ; real_T B_24_329_2632 [ 5 ]
; real_T B_24_334_2672 [ 5 ] ; real_T B_24_339_2712 ; real_T B_24_340_2720 [
9 ] ; real_T B_24_349_2792 [ 9 ] ; real_T B_24_358_2864 [ 9 ] ; real_T
B_24_367_2936 ; real_T B_24_368_2944 ; real_T B_24_369_2952 ; real_T
B_24_370_2960 ; real_T B_24_371_2968 ; real_T B_24_372_2976 ; real_T
B_24_373_2984 ; real_T B_24_374_2992 ; real_T B_24_375_3000 ; real_T
B_24_376_3008 ; real_T B_24_377_3016 ; real_T B_24_378_3024 ; real_T
B_24_379_3032 ; real_T B_24_380_3040 ; real_T B_24_381_3048 ; real_T
B_24_382_3056 ; real_T B_24_383_3064 ; real_T B_24_384_3072 ; real_T
B_24_385_3080 ; real_T B_24_386_3088 ; real_T B_24_387_3096 [ 20 ] ; real_T
B_24_407_3256 [ 20 ] ; real_T B_24_427_3416 [ 20 ] ; real_T B_24_447_3576 [
20 ] ; real_T B_24_467_3736 [ 20 ] ; real_T B_24_487_3896 [ 20 ] ; real_T
B_24_507_4056 [ 20 ] ; real_T B_24_527_4216 ; real_T B_24_528_4224 ; real_T
B_24_529_4232 ; real_T B_24_530_4240 ; real_T B_24_531_4248 ; real_T
B_24_532_4256 [ 5 ] ; real_T B_24_537_4296 [ 5 ] ; real_T B_24_542_4336 [ 5 ]
; real_T B_24_547_4376 [ 5 ] ; real_T B_24_552_4416 [ 5 ] ; real_T
B_24_557_4456 ; real_T B_24_558_4464 ; real_T B_24_559_4472 ; real_T
B_24_560_4480 ; real_T B_24_561_4488 ; real_T B_24_562_4496 ; real_T
B_24_563_4504 ; real_T B_24_564_4512 ; real_T B_24_565_4520 ; real_T
B_24_566_4528 ; real_T B_24_567_4536 ; real_T B_24_568_4544 ; real_T
B_24_569_4552 ; real_T B_24_570_4560 ; real_T B_24_571_4568 ; real_T
B_24_572_4576 ; real_T B_24_573_4584 ; real_T B_24_574_4592 ; real_T
B_24_575_4600 ; real_T B_24_576_4608 ; real_T B_24_577_4616 ; real_T
B_24_578_4624 ; real_T B_24_579_4632 ; real_T B_24_580_4640 ; real_T
B_24_581_4648 ; real_T B_24_582_4656 ; real_T B_24_583_4664 ; real_T
B_24_584_4672 ; real_T B_24_585_4680 ; real_T B_24_586_4688 ; real_T
B_24_587_4696 ; real_T B_24_588_4704 ; real_T B_24_589_4712 ; real_T
B_24_590_4720 ; real_T B_24_591_4728 ; real_T B_24_592_4736 ; real_T
B_24_593_4744 ; real_T B_24_594_4752 ; real_T B_24_595_4760 ; real_T
B_24_596_4768 ; real_T B_24_597_4776 ; real_T B_24_598_4784 ; real_T
B_24_599_4792 [ 4 ] ; real_T B_24_603_4824 [ 4 ] ; real_T B_24_607_4856 [ 4 ]
; real_T B_24_611_4888 [ 9 ] ; real_T B_24_620_4960 [ 9 ] ; real_T
B_24_629_5032 [ 9 ] ; real_T B_24_638_5104 [ 9 ] ; real_T B_24_647_5176 [ 9 ]
; real_T B_24_656_5248 [ 9 ] ; real_T B_24_665_5320 [ 9 ] ; real_T
B_24_674_5392 [ 9 ] ; real_T B_24_683_5464 [ 9 ] ; real_T B_24_692_5536 [ 9 ]
; real_T B_24_701_5608 [ 9 ] ; real_T B_24_710_5680 [ 9 ] ; real_T
B_24_719_5752 [ 9 ] ; real_T B_24_728_5824 [ 9 ] ; real_T B_24_737_5896 [ 9 ]
; real_T B_24_746_5968 [ 9 ] ; real_T B_24_755_6040 [ 9 ] ; real_T
B_24_764_6112 [ 9 ] ; real_T B_24_773_6184 [ 9 ] ; real_T B_24_782_6256 [ 9 ]
; real_T B_24_791_6328 [ 20 ] ; real_T B_24_811_6488 ; real_T B_24_812_6496 ;
real_T B_24_813_6504 [ 20 ] ; real_T B_24_833_6664 [ 20 ] ; real_T
B_24_853_6824 [ 20 ] ; real_T B_24_873_6984 [ 20 ] ; real_T B_24_893_7144 [
20 ] ; real_T B_24_913_7304 [ 5 ] ; real_T B_24_918_7344 ; real_T
B_24_919_7352 ; real_T B_24_920_7360 [ 5 ] ; real_T B_24_925_7400 [ 5 ] ;
real_T B_24_930_7440 ; real_T B_24_931_7448 [ 5 ] ; real_T B_24_936_7488 [ 5
] ; real_T B_24_941_7528 ; real_T B_24_942_7536 ; real_T B_24_943_7544 [ 5 ]
; real_T B_24_948_7584 ; real_T B_24_949_7592 [ 5 ] ; real_T B_24_954_7632 ;
real_T B_24_955_7640 [ 5 ] ; real_T B_24_960_7680 ; real_T B_24_961_7688 [ 5
] ; real_T B_24_966_7728 ; real_T B_24_967_7736 ; real_T B_24_968_7744 [ 5 ]
; real_T B_24_973_7784 ; real_T B_24_974_7792 ; real_T B_24_975_7800 ; real_T
B_24_976_7808 [ 5 ] ; real_T B_24_981_7848 ; real_T B_24_982_7856 ; real_T
B_24_983_7864 ; real_T B_24_984_7872 [ 5 ] ; real_T B_24_989_7912 ; real_T
B_24_990_7920 ; real_T B_24_991_7928 ; real_T B_24_992_7936 ; real_T
B_24_993_7944 ; real_T B_24_994_7952 ; real_T B_24_995_7960 ; real_T
B_24_996_7968 ; real_T B_24_997_7976 ; real_T B_24_998_7984 ; real_T
B_24_999_7992 ; real_T B_24_1000_8000 ; real_T B_24_1001_8008 ; real_T
B_24_1002_8016 ; real_T B_24_1003_8024 ; real_T B_24_1004_8032 ; real_T
B_24_1005_8040 ; real_T B_23_1006_8048 ; real_T B_22_1007_8056 [ 40 ] ;
real_T B_22_1047_8376 [ 5 ] ; real_T B_22_1052_8416 ; real_T B_21_1053_8424 ;
real_T B_20_1054_8432 ; real_T B_19_1055_8440 ; real_T B_18_1056_8448 ;
real_T B_17_1057_8456 ; real_T B_16_1058_8464 ; real_T B_15_1059_8472 ;
real_T B_14_1060_8480 ; real_T B_13_1061_8488 ; real_T B_12_1062_8496 ;
real_T B_11_1063_8504 ; real_T B_10_1064_8512 [ 5 ] ; real_T B_10_1069_8552 [
8 ] ; real_T B_10_1077_8616 [ 8 ] ; real_T B_10_1085_8680 [ 8 ] ; real_T
B_10_1093_8744 [ 8 ] ; real_T B_10_1101_8808 [ 8 ] ; real_T B_10_1109_8872 ;
real_T B_10_1110_8880 ; real_T B_10_1111_8888 [ 408 ] ; real_T
B_10_1519_12152 [ 64 ] ; real_T B_10_1583_12664 [ 8 ] ; real_T B_2_1591_12728
; real_T B_1_1592_12736 ; real_T B_0_1593_12744 ; int32_T B_24_1594_12752 ;
int32_T B_24_1595_12756 ; uint16_T B_10_1596_12760 [ 8 ] ; uint16_T
B_10_1604_12776 ; uint16_T B_10_1605_12778 ; uint16_T B_10_1606_12780 ;
uint16_T B_9_1607_12782 ; uint16_T B_9_1608_12784 [ 8 ] ; uint16_T
B_9_1616_12800 ; uint16_T B_9_1617_12802 [ 8 ] ; uint16_T B_9_1625_12818 ;
boolean_T B_24_1626_12820 ; boolean_T B_24_1627_12821 ; boolean_T
B_24_1628_12822 ; boolean_T B_24_1629_12823 ; boolean_T B_24_1630_12824 ;
boolean_T B_24_1631_12825 ; boolean_T B_24_1632_12826 ; boolean_T
B_24_1633_12827 ; boolean_T B_24_1634_12828 ; boolean_T B_24_1635_12829 ;
boolean_T B_24_1636_12830 ; boolean_T B_24_1637_12831 ; boolean_T
B_24_1638_12832 ; boolean_T B_24_1639_12833 ; boolean_T B_24_1640_12834 ;
boolean_T B_24_1641_12835 ; boolean_T B_24_1642_12836 ; boolean_T
B_24_1643_12837 ; boolean_T B_24_1644_12838 ; boolean_T B_24_1645_12839 ;
boolean_T B_24_1646_12840 ; boolean_T B_24_1647_12841 ; boolean_T
B_24_1648_12842 ; boolean_T B_24_1649_12843 ; boolean_T B_24_1650_12844 ;
boolean_T B_24_1651_12845 ; boolean_T B_24_1652_12846 ; boolean_T
B_24_1653_12847 ; boolean_T B_10_1654_12848 [ 8 ] ;
B_findnode_sm_aileron_actuator_T findnode ; } ; struct
DW_sm_aileron_actuator_T { real_T INTERNAL_1_1_1_Discrete ; real_T
INTERNAL_1_1_1_FirstOutput ; real_T INPUT_1_1_1_Discrete [ 2 ] ; real_T
INPUT_2_1_1_Discrete [ 2 ] ; real_T INPUT_3_1_1_Discrete [ 2 ] ; real_T
INTERNAL_2_1_1_Discrete [ 2 ] ; real_T INTERNAL_3_1_1_Discrete ; real_T
INTERNAL_3_1_1_FirstOutput ; real_T INPUT_4_1_1_Discrete [ 2 ] ; real_T
INPUT_5_1_1_Discrete [ 2 ] ; real_T INPUT_6_1_1_Discrete [ 2 ] ; real_T
INTERNAL_4_1_1_Discrete [ 2 ] ; real_T INTERNAL_5_1_1_Discrete ; real_T
INTERNAL_5_1_1_FirstOutput ; real_T INPUT_7_1_1_Discrete [ 2 ] ; real_T
INPUT_8_1_1_Discrete [ 2 ] ; real_T INPUT_9_1_1_Discrete [ 2 ] ; real_T
INTERNAL_6_1_1_Discrete [ 2 ] ; real_T INPUT_10_1_1_Discrete [ 2 ] ; real_T
Integrator_DSTATE ; real_T UD_DSTATE ; real_T UD_DSTATE_l ; real_T
Integrator_DSTATE_h ; real_T UD_DSTATE_p ; real_T Integrator_DSTATE_l ;
real_T Integrator_DSTATE_e ; real_T UD_DSTATE_h ; real_T Integrator_DSTATE_f
; real_T Integrator_DSTATE_lo ; real_T UD_DSTATE_i ; real_T
INPUT_1_1_1_Discrete_b [ 2 ] ; real_T INPUT_2_1_1_Discrete_k [ 2 ] ; real_T
INPUT_3_1_1_Discrete_l [ 2 ] ; real_T LTI_STATE_1_FirstOutput ; real_T
STATE_1_Discrete ; real_T STATE_1_ZcValueStore ; real_T OUTPUT_1_0_Discrete ;
real_T OUTPUT_1_0_ZcValueStore ; real_T STATE_4_Discrete ; real_T
STATE_4_ZcValueStore ; real_T OUTPUT_4_0_Discrete ; real_T
OUTPUT_4_0_ZcValueStore ; real_T STATE_3_Discrete ; real_T
STATE_3_ZcValueStore ; real_T OUTPUT_3_0_Discrete ; real_T
OUTPUT_3_0_ZcValueStore ; real_T STATE_2_Discrete ; real_T
STATE_2_ZcValueStore ; real_T OUTPUT_2_0_Discrete ; real_T
OUTPUT_2_0_ZcValueStore ; real_T OUTPUT_1_1_Discrete ; real_T
OUTPUT_1_1_ZcValueStore ; real_T TimeStampA ; real_T LastUAtTimeA ; real_T
TimeStampB ; real_T LastUAtTimeB ; real_T Memory_PreviousInput ; real_T
TimeStampA_o ; real_T LastUAtTimeA_l ; real_T TimeStampB_f ; real_T
LastUAtTimeB_g ; real_T Memory1_PreviousInput ; real_T TimeStampA_f ; real_T
LastUAtTimeA_m ; real_T TimeStampB_e ; real_T LastUAtTimeB_c ; real_T
Memory5_PreviousInput [ 5 ] ; real_T Memory6_PreviousInput ; real_T
Memory1_PreviousInput_m ; real_T Memory_PreviousInput_j ; real_T
OUTPUT_4_1_Discrete ; real_T OUTPUT_4_1_ZcValueStore ; real_T
OUTPUT_3_1_Discrete ; real_T OUTPUT_3_1_ZcValueStore ; real_T
OUTPUT_2_1_Discrete ; real_T OUTPUT_2_1_ZcValueStore ; struct { real_T
modelTStart ; } TransportDelay1_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { real_T modelTStart ; } TransportDelay_RWORK_o
; struct { real_T modelTStart ; } TransportDelay1_RWORK_m ; void *
STATE_1_Simulator ; void * STATE_1_SimData ; void * STATE_1_DiagMgr ; void *
STATE_1_ZcLogger ; void * STATE_1_TsInfo ; void * OUTPUT_1_0_Simulator ; void
* OUTPUT_1_0_SimData ; void * OUTPUT_1_0_DiagMgr ; void * OUTPUT_1_0_ZcLogger
; void * OUTPUT_1_0_TsInfo ; void * FromWs_PWORK [ 3 ] ; void *
STATE_4_Simulator ; void * STATE_4_SimData ; void * STATE_4_DiagMgr ; void *
STATE_4_ZcLogger ; void * STATE_4_TsInfo ; void * OUTPUT_4_0_Simulator ; void
* OUTPUT_4_0_SimData ; void * OUTPUT_4_0_DiagMgr ; void * OUTPUT_4_0_ZcLogger
; void * OUTPUT_4_0_TsInfo ; void * STATE_3_Simulator ; void *
STATE_3_SimData ; void * STATE_3_DiagMgr ; void * STATE_3_ZcLogger ; void *
STATE_3_TsInfo ; void * OUTPUT_3_0_Simulator ; void * OUTPUT_3_0_SimData ;
void * OUTPUT_3_0_DiagMgr ; void * OUTPUT_3_0_ZcLogger ; void *
OUTPUT_3_0_TsInfo ; void * STATE_2_Simulator ; void * STATE_2_SimData ; void
* STATE_2_DiagMgr ; void * STATE_2_ZcLogger ; void * STATE_2_TsInfo ; void *
OUTPUT_2_0_Simulator ; void * OUTPUT_2_0_SimData ; void * OUTPUT_2_0_DiagMgr
; void * OUTPUT_2_0_ZcLogger ; void * OUTPUT_2_0_TsInfo ; void *
OUTPUT_1_1_Simulator ; void * OUTPUT_1_1_SimData ; void * OUTPUT_1_1_DiagMgr
; void * OUTPUT_1_1_ZcLogger ; void * OUTPUT_1_1_TsInfo ; void * Scope_PWORK
[ 4 ] ; struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Actuator_at_outport_0_PWORK ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_From_at_outport_0_PWORK ; struct {
void * AQHandles ; } TAQSigLogging_InsertedFor_Mechanical_at_outport_0_PWORK
; void * Scope_PWORK_k ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay1_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK ; void * Scope_PWORK_p ; void * Scope1_PWORK [ 5 ] ;
struct { void * AQHandles ; } _asyncqueue_inserted_for_ToWorkspace_PWORK ;
struct { void * AQHandles ; } _asyncqueue_inserted_for_ToWorkspace1_PWORK ;
struct { void * AQHandles ; } _asyncqueue_inserted_for_ToWorkspace2_PWORK ;
struct { void * AQHandles ; } _asyncqueue_inserted_for_ToWorkspace3_PWORK ;
struct { void * AQHandles ; } _asyncqueue_inserted_for_ToWorkspace4_PWORK ;
struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_g ; struct { void
* TUbufferPtrs [ 2 ] ; } TransportDelay1_PWORK_a ; void *
OUTPUT_4_1_Simulator ; void * OUTPUT_4_1_SimData ; void * OUTPUT_4_1_DiagMgr
; void * OUTPUT_4_1_ZcLogger ; void * OUTPUT_4_1_TsInfo ; void * PWM_PWORK ;
void * OUTPUT_3_1_Simulator ; void * OUTPUT_3_1_SimData ; void *
OUTPUT_3_1_DiagMgr ; void * OUTPUT_3_1_ZcLogger ; void * OUTPUT_3_1_TsInfo ;
void * PWM_PWORK_f ; void * OUTPUT_2_1_Simulator ; void * OUTPUT_2_1_SimData
; void * OUTPUT_2_1_DiagMgr ; void * OUTPUT_2_1_ZcLogger ; void *
OUTPUT_2_1_TsInfo ; void * PWM_PWORK_b ; void * Scope2_PWORK [ 2 ] ; void *
Scope1_PWORK_e [ 2 ] ; void * Scope_PWORK_j [ 2 ] ; void *
LTI_SINK_1_RtwLogger ; void * LTI_SINK_1_RtwLogBuffer ; void *
LTI_SINK_1_RtwLogFcnManager ; void * SINK_1_RtwLogger ; void *
SINK_1_RtwLogBuffer ; void * SINK_1_RtwLogFcnManager ; void *
SINK_2_RtwLogger ; void * SINK_2_RtwLogBuffer ; void *
SINK_2_RtwLogFcnManager ; void * SINK_3_RtwLogger ; void *
SINK_3_RtwLogBuffer ; void * SINK_3_RtwLogFcnManager ; void *
SINK_4_RtwLogger ; void * SINK_4_RtwLogBuffer ; void *
SINK_4_RtwLogFcnManager ; void * RTP_2_RtpManager ; void * RTP_3_RtpManager ;
void * RTP_4_RtpManager ; int32_T MATLABFunction3_sysIdxToRun ; int32_T
MATLABFunction1_sysIdxToRun ; int32_T MedianFilter2_sysIdxToRun ; int32_T
MedianFilter1_sysIdxToRun ; int32_T MedianFilter_sysIdxToRun ; int32_T
MedianFilter2_sysIdxToRun_k ; int32_T MedianFilter1_sysIdxToRun_o ; int32_T
MedianFilter_sysIdxToRun_m ; int32_T MATLABFunction_sysIdxToRun ; int32_T
MATLABFunction_sysIdxToRun_h ; int32_T MATLABFunction_sysIdxToRun_i ; int32_T
DES_Stateflow2_sysIdxToRun ; int32_T DES_Model_Aileron_sysIdxToRun ; int32_T
ClassificationTreePredict_sysIdxToRun ; int32_T
IndexVectorReversed_sysIdxToRun ; int32_T DES_Stateflow3_sysIdxToRun ;
int32_T DES_Stateflow2_sysIdxToRun_m ; int32_T DES_Stateflow1_sysIdxToRun ;
int_T STATE_1_Modes ; int_T OUTPUT_1_0_Modes ; int_T FromWs_IWORK ; int_T
STATE_4_Modes [ 4 ] ; int_T OUTPUT_4_0_Modes ; int_T STATE_3_Modes [ 4 ] ;
int_T OUTPUT_3_0_Modes ; int_T STATE_2_Modes [ 4 ] ; int_T OUTPUT_2_0_Modes ;
int_T OUTPUT_1_1_Modes ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay1_IWORK ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_f ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay1_IWORK_p ;
int_T OUTPUT_4_1_Modes ; int_T OUTPUT_3_1_Modes ; int_T OUTPUT_2_1_Modes ;
int32_T STATE_4_MASS_MATRIX_PR ; int32_T STATE_3_MASS_MATRIX_PR ; int32_T
STATE_2_MASS_MATRIX_PR ; int8_T Integrator_PrevResetState ; int8_T
Integrator_PrevResetState_m ; int8_T Integrator_PrevResetState_f ; int8_T
Integrator_PrevResetState_ms ; int8_T Integrator_PrevResetState_p ; int8_T
Integrator_PrevResetState_k ; uint8_T STATE_1_ZcSignalDir ; uint8_T
STATE_1_ZcStateStore ; uint8_T OUTPUT_1_0_ZcSignalDir ; uint8_T
OUTPUT_1_0_ZcStateStore ; uint8_T STATE_4_ZcSignalDir ; uint8_T
STATE_4_ZcStateStore ; uint8_T OUTPUT_4_0_ZcSignalDir ; uint8_T
OUTPUT_4_0_ZcStateStore ; uint8_T STATE_3_ZcSignalDir ; uint8_T
STATE_3_ZcStateStore ; uint8_T OUTPUT_3_0_ZcSignalDir ; uint8_T
OUTPUT_3_0_ZcStateStore ; uint8_T STATE_2_ZcSignalDir ; uint8_T
STATE_2_ZcStateStore ; uint8_T OUTPUT_2_0_ZcSignalDir ; uint8_T
OUTPUT_2_0_ZcStateStore ; uint8_T OUTPUT_1_1_ZcSignalDir ; uint8_T
OUTPUT_1_1_ZcStateStore ; uint8_T Integrator_IC_LOADING ; uint8_T
Integrator_IC_LOADING_p ; uint8_T Integrator_IC_LOADING_a ; uint8_T
Integrator_IC_LOADING_m ; uint8_T Integrator_IC_LOADING_j ; uint8_T
Integrator_IC_LOADING_k ; uint8_T OUTPUT_4_1_ZcSignalDir ; uint8_T
OUTPUT_4_1_ZcStateStore ; uint8_T OUTPUT_3_1_ZcSignalDir ; uint8_T
OUTPUT_3_1_ZcStateStore ; uint8_T OUTPUT_2_1_ZcSignalDir ; uint8_T
OUTPUT_2_1_ZcStateStore ; boolean_T STATE_1_FirstOutput ; boolean_T
OUTPUT_1_0_FirstOutput ; boolean_T STATE_4_FirstOutput ; boolean_T
OUTPUT_4_0_FirstOutput ; boolean_T STATE_3_FirstOutput ; boolean_T
OUTPUT_3_0_FirstOutput ; boolean_T STATE_2_FirstOutput ; boolean_T
OUTPUT_2_0_FirstOutput ; boolean_T OUTPUT_1_1_FirstOutput ; boolean_T
Memory4_PreviousInput ; boolean_T Memory2_PreviousInput ; boolean_T
OUTPUT_4_1_FirstOutput ; boolean_T OUTPUT_3_1_FirstOutput ; boolean_T
OUTPUT_2_1_FirstOutput ; boolean_T LTI_STATE_1_ReInit ; boolean_T
RTP_2_SetParametersNeeded ; boolean_T RTP_3_SetParametersNeeded ; boolean_T
RTP_4_SetParametersNeeded ; char_T pad_RTP_4_SetParametersNeeded [ 6 ] ;
DW_findnode_sm_aileron_actuator_T findnode ; } ; struct
X_sm_aileron_actuator_T { real_T
sm_aileron_actuatorMechanicalRev_MR_ActMountRzq [ 26 ] ; real_T
sm_aileron_actuatoroutputFiltered_1513730075_0 ; real_T u5_CSTATE ; real_T
u0HzLowpassFilter_CSTATE ; real_T Limits55_CSTATE ; real_T
u0HzLowpassFilter_CSTATE_e ; real_T
sm_aileron_actuatorActuatorElectricLeadscrew_1DC_Motorw [ 9 ] ; real_T
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
sm_aileron_actuatorActuatorElectricLeadscrew_1DC_Motorw [ 9 ] ; real_T
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
sm_aileron_actuatorActuatorElectricLeadscrew_1DC_Motorw [ 9 ] ; boolean_T
sm_aileron_actuatoroutputFiltered_553247611_0 ; boolean_T u5_CSTATE_f ;
boolean_T u0HzLowpassFilter_CSTATE_d ; boolean_T Limits55_CSTATE_g ;
boolean_T u0HzLowpassFilter_CSTATE_b ; boolean_T
sm_aileron_actuatorActuatorElectricLeadscrew_2DC_Motorw [ 9 ] ; boolean_T
sm_aileron_actuatoroutputFiltered_2948142811_0 ; boolean_T u5_CSTATE_n ;
boolean_T u0HzLowpassFilter_CSTATE_i ; boolean_T Limits55_CSTATE_p ;
boolean_T u0HzLowpassFilter_CSTATE_p ; boolean_T
sm_aileron_actuatorActuatorElectricLeadscrew_3DC_Motorw [ 8 ] ; } ; struct
MassMatrix_sm_aileron_actuator_T { int_T ir [ 58 ] ; int_T jc [ 68 ] ; real_T
pr [ 58 ] ; } ; struct ConstB_findnode_sm_aileron_actuator_T { uint8_T
B_8_1945_13562 ; char_T pad_B_8_1945_13562 [ 7 ] ; } ; struct
ConstB_sm_aileron_actuator_T { real_T B_24_1938_13512 ; real_T
B_24_1939_13520 ; real_T B_24_1940_13528 ; real_T B_24_1941_13536 ; real_T
B_24_1942_13544 ; real_T B_24_1943_13552 ; uint16_T B_9_1944_13560 ; char_T
pad_B_9_1944_13560 [ 6 ] ; ConstB_findnode_sm_aileron_actuator_T findnode ; }
;
#define sm_aileron_actuator_rtC(S) ((ConstB_sm_aileron_actuator_T *) _ssGetConstBlockIO(S))
struct P_findnode_sm_aileron_actuator_T_ { real_T P_0 [ 51 ] ; boolean_T P_1
; boolean_T P_2 ; boolean_T P_3 [ 51 ] ; uint8_T P_4 ; uint8_T P_5 [ 102 ] ;
uint8_T P_6 ; uint8_T P_7 ; uint8_T P_8 ; uint8_T P_9 ; uint8_T P_10 ;
uint8_T P_11 [ 51 ] ; char_T pad_P_11 [ 4 ] ; } ; struct
P_sm_aileron_actuator_T_ { real_T P_0 [ 2 ] ; real_T P_1 ; real_T P_2 [ 2 ] ;
real_T P_3 ; real_T P_4 [ 2 ] ; real_T P_5 ; real_T P_6 [ 2 ] ; real_T P_7 ;
real_T P_8 [ 2 ] ; real_T P_9 ; real_T P_10 [ 2 ] ; real_T P_11 ; real_T P_12
[ 2 ] ; real_T P_13 ; real_T P_14 [ 2 ] ; real_T P_15 ; real_T P_16 [ 2 ] ;
real_T P_17 ; real_T P_18 [ 2 ] ; real_T P_19 ; real_T P_20 [ 2 ] ; real_T
P_21 ; real_T P_22 [ 2 ] ; real_T P_23 ; real_T P_24 [ 408 ] ; real_T P_25 [
64 ] ; real_T P_26 [ 8 ] ; real_T P_27 ; real_T P_28 ; real_T P_29 ; real_T
P_30 ; real_T P_31 ; real_T P_32 ; real_T P_33 ; real_T P_34 [ 11 ] ; real_T
P_35 [ 11 ] ; real_T P_36 [ 11 ] ; real_T P_37 [ 11 ] ; real_T P_38 [ 11 ] ;
real_T P_39 [ 11 ] ; real_T P_40 ; real_T P_41 ; real_T P_42 [ 27 ] ; real_T
P_43 [ 27 ] ; real_T P_44 ; real_T P_45 ; real_T P_46 ; real_T P_47 ; real_T
P_48 ; real_T P_49 ; real_T P_50 ; real_T P_51 ; real_T P_52 ; real_T P_53 ;
real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T P_58 ; real_T
P_59 ; real_T P_60 ; real_T P_61 ; real_T P_62 ; real_T P_63 ; real_T P_64 ;
real_T P_65 ; real_T P_66 ; real_T P_67 ; real_T P_68 ; real_T P_69 ; real_T
P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 ; real_T P_74 ; real_T P_75 ;
real_T P_76 ; real_T P_77 ; real_T P_78 ; real_T P_79 ; real_T P_80 ; real_T
P_81 ; real_T P_82 ; real_T P_83 ; real_T P_84 ; real_T P_85 ; real_T P_86 ;
real_T P_87 ; real_T P_88 ; real_T P_89 ; real_T P_90 ; real_T P_91 ; real_T
P_92 ; real_T P_93 ; real_T P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ;
real_T P_98 ; real_T P_99 ; real_T P_100 ; real_T P_101 ; real_T P_102 ;
real_T P_103 ; real_T P_104 ; real_T P_105 ; real_T P_106 ; real_T P_107 ;
real_T P_108 ; real_T P_109 ; real_T P_110 ; real_T P_111 ; real_T P_112 ;
real_T P_113 ; real_T P_114 ; real_T P_115 ; real_T P_116 ; real_T P_117 ;
real_T P_118 ; real_T P_119 ; real_T P_120 ; real_T P_121 ; real_T P_122 ;
real_T P_123 ; real_T P_124 ; real_T P_125 ; real_T P_126 ; real_T P_127 ;
real_T P_128 ; real_T P_129 ; real_T P_130 ; real_T P_131 ; real_T P_132 ;
real_T P_133 ; real_T P_134 ; real_T P_135 ; real_T P_136 ; real_T P_137 ;
real_T P_138 ; real_T P_139 ; real_T P_140 ; real_T P_141 ; real_T P_142 ;
real_T P_143 ; real_T P_144 ; real_T P_145 ; real_T P_146 ; real_T P_147 ;
real_T P_148 ; real_T P_149 ; real_T P_150 ; real_T P_151 ; real_T P_152 [ 5
] ; real_T P_153 ; real_T P_154 [ 9 ] ; real_T P_155 [ 9 ] ; real_T P_156 ;
real_T P_157 ; real_T P_158 ; real_T P_159 ; real_T P_160 [ 5 ] ; real_T
P_161 [ 5 ] ; real_T P_162 ; real_T P_163 ; real_T P_164 ; real_T P_165 ;
real_T P_166 ; real_T P_167 ; real_T P_168 ; real_T P_169 ; real_T P_170 ;
real_T P_171 ; real_T P_172 ; real_T P_173 ; real_T P_174 ; real_T P_175 ;
real_T P_176 ; real_T P_177 ; real_T P_178 ; real_T P_179 ; real_T P_180 ;
real_T P_181 [ 9 ] ; real_T P_182 [ 9 ] ; real_T P_183 [ 9 ] ; real_T P_184 [
9 ] ; real_T P_185 [ 9 ] ; real_T P_186 [ 9 ] ; real_T P_187 [ 9 ] ; real_T
P_188 [ 9 ] ; real_T P_189 [ 9 ] ; real_T P_190 [ 9 ] ; real_T P_191 [ 9 ] ;
real_T P_192 [ 9 ] ; real_T P_193 [ 9 ] ; real_T P_194 [ 9 ] ; real_T P_195 [
9 ] ; real_T P_196 [ 9 ] ; real_T P_197 [ 9 ] ; real_T P_198 [ 9 ] ; real_T
P_199 [ 9 ] ; real_T P_200 [ 9 ] ; real_T P_201 [ 20 ] ; real_T P_202 ;
real_T P_203 ; real_T P_204 [ 20 ] ; real_T P_205 [ 20 ] ; real_T P_206 [ 20
] ; real_T P_207 [ 20 ] ; real_T P_208 [ 20 ] ; real_T P_209 [ 5 ] ; real_T
P_210 ; real_T P_211 ; real_T P_212 [ 5 ] ; real_T P_213 [ 5 ] ; real_T P_214
; real_T P_215 [ 5 ] ; real_T P_216 [ 5 ] ; real_T P_217 ; real_T P_218 ;
real_T P_219 [ 5 ] ; real_T P_220 ; real_T P_221 [ 5 ] ; real_T P_222 ;
real_T P_223 [ 5 ] ; real_T P_224 ; real_T P_225 [ 5 ] ; real_T P_226 ;
real_T P_227 ; real_T P_228 [ 5 ] ; real_T P_229 ; real_T P_230 ; real_T
P_231 ; real_T P_232 [ 5 ] ; real_T P_233 ; real_T P_234 ; real_T P_235 ;
real_T P_236 [ 5 ] ; real_T P_237 ; real_T P_238 ; real_T P_239 ; real_T
P_240 ; real_T P_241 ; real_T P_242 ; real_T P_243 ; real_T P_244 ; real_T
P_245 ; real_T P_246 ; real_T P_247 ; real_T P_248 ; real_T P_249 ; real_T
P_250 ; uint16_T P_251 ; uint16_T P_252 ; uint16_T P_253 ; uint16_T P_254 ;
boolean_T P_255 ; boolean_T P_256 ; char_T pad_P_256 [ 6 ] ;
P_findnode_sm_aileron_actuator_T findnode ; } ; extern
P_sm_aileron_actuator_T sm_aileron_actuator_rtDefaultP ; extern const
ConstB_sm_aileron_actuator_T sm_aileron_actuator_rtInvariant ;
#endif
