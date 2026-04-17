#include "sm_aileron_actuator_acc.h"
#include "sm_aileron_actuator_acc_types.h"
#include "rtwtypes.h"
#include "mwmathutil.h"
#include <string.h>
#include "sm_aileron_actuator_acc_private.h"
#include "multiword_types.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
#ifdef __cplusplus
extern "C" {
#endif
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#ifdef __cplusplus
}
#endif
#endif
boolean_T sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T tail = *
tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ;
real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) {
numBuffer = 3 ; xBuf = * uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz -
1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && !
isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T * tempX = (
NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024 ; if (
newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU = (
real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if ( tempU
== ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * uBufPtr = tempU ; } else { * tailPtr =
testIdx ; } return ( true ) ; } real_T
sm_aileron_actuator_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T
tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2 , u1 , u2
; real_T * tBuf = uBuf + bufSz ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real_T look1_binlxpw ( real_T
u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; real_T yL_0d0 ; uint32_T bpIdx ; uint32_T iLeft ; uint32_T
iRght ; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / (
bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx =
maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght - iLeft > 1U )
{ if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; }
bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [
iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ; frac = ( u0
- bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; }
yL_0d0 = table [ iLeft ] ; return ( table [ iLeft + 1U ] - yL_0d0 ) * frac +
yL_0d0 ; } void rt_ssGetBlockPath ( SimStruct * S , int_T sysIdx , int_T
blkIdx , char_T * * path ) { _ssGetBlockPath ( S , sysIdx , blkIdx , path ) ;
} void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * castedS = (
SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( castedS ) ) {
_ssSet_slErrMsg ( castedS , diag ) ; } else { _ssDiscardDiagnostic ( castedS
, diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag )
{ _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } static void
mdlOutputs ( SimStruct * S , int_T tid ) { B_sm_aileron_actuator_T * _rtB ;
DW_sm_aileron_actuator_T * _rtDW ; P_sm_aileron_actuator_T * _rtP ;
X_sm_aileron_actuator_T * _rtX ; real_T B_18_310_2480_0 ; real_T
B_18_388_3104_0 ; real_T B_18_468_3744_0 ; real_T tmp ; real_T tmp_0 ; real_T
tmp_1 ; real_T tmp_2 ; real_T tmp_3 ; real_T tmp_4 ; real_T tmp_5 ; real_T
tmp_6 ; real_T tmp_7 ; real_T tmp_8 ; real_T tmp_9 ; real_T tmp_a ; real_T
tmp_b ; real_T tmp_c ; real_T tmp_d ; real_T tmp_e ; real_T u0 ; real_T u1 ;
real_T u2 ; real_T * lastU ; int32_T i ; uint32_T intVal ; uint32_T u ; _rtDW
= ( ( DW_sm_aileron_actuator_T * ) ssGetRootDWork ( S ) ) ; _rtX = ( (
X_sm_aileron_actuator_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ; ssCallAccelRunBlock
( S , 18 , 1 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 18 , 2 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_37_296 = _rtP -> P_38 * _rtB ->
B_18_27_216 [ 6 ] ; _rtB -> B_18_38_304 = look1_binlxpw ( _rtB -> B_18_37_296
, _rtP -> P_40 , _rtP -> P_39 , 10U ) ; ssCallAccelRunBlock ( S , 18 , 5 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_42_336 = look1_binlxpw ( _rtB ->
B_18_39_312 [ 0 ] , _rtP -> P_42 , _rtP -> P_41 , 10U ) ; _rtB -> B_18_43_344
= _rtB -> B_18_39_312 [ 1 ] * _rtB -> B_18_39_312 [ 1 ] ; _rtB -> B_18_44_352
= look1_binlxpw ( _rtB -> B_18_39_312 [ 2 ] , _rtP -> P_44 , _rtP -> P_43 ,
10U ) ; _rtB -> B_18_45_360 = _rtB -> B_18_38_304 * _rtB -> B_18_42_336 *
_rtB -> B_18_43_344 * _rtB -> B_18_44_352 ; _rtB -> B_18_46_368 = _rtP ->
P_45 * _rtB -> B_18_45_360 ; _rtB -> B_18_47_376 = _rtP -> P_46 * _rtB ->
B_18_46_368 ; _rtB -> B_18_48_384 = ssGetT ( S ) ; _rtB -> B_18_50_400 = _rtB
-> B_18_48_384 - _rtB -> B_18_49_392 ; _rtB -> B_18_51_408 =
muDoubleScalarRem ( _rtB -> B_18_50_400 , _rtB -> B_18_1006_8048 ) ; _rtB ->
B_18_52_416 = look1_binlxpw ( _rtB -> B_18_51_408 , _rtP -> P_48 , _rtP ->
P_47 , 26U ) ; _rtB -> B_18_53_424 = _rtB -> B_18_52_416 ; if ( _rtDW ->
INTERNAL_1_1_1_FirstOutput == 0.0 ) { _rtDW -> INTERNAL_1_1_1_FirstOutput =
1.0 ; _rtX -> sm_aileron_actuatoroutputFiltered_1513730075_0 = _rtB ->
B_18_27_216 [ 1 ] ; } _rtB -> B_18_54_432 [ 0 ] = _rtX ->
sm_aileron_actuatoroutputFiltered_1513730075_0 ; _rtB -> B_18_54_432 [ 1 ] =
( _rtB -> B_18_27_216 [ 1 ] - _rtX ->
sm_aileron_actuatoroutputFiltered_1513730075_0 ) * 100.0 ; _rtB ->
B_18_54_432 [ 2 ] = 0.0 ; _rtB -> B_18_54_432 [ 3 ] = 0.0 ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> u5_CSTATE >= _rtP -> P_50 ) {
if ( _rtX -> u5_CSTATE != _rtP -> P_50 ) { _rtX -> u5_CSTATE = _rtP -> P_50 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else if ( ( _rtX ->
u5_CSTATE <= _rtP -> P_51 ) && ( _rtX -> u5_CSTATE != _rtP -> P_51 ) ) { _rtX
-> u5_CSTATE = _rtP -> P_51 ; ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; } _rtB -> B_18_58_464 = _rtX -> u5_CSTATE ; } else if ( _rtX -> u5_CSTATE
>= _rtP -> P_50 ) { _rtB -> B_18_58_464 = _rtP -> P_50 ; } else if ( _rtX ->
u5_CSTATE <= _rtP -> P_51 ) { _rtB -> B_18_58_464 = _rtP -> P_51 ; } else {
_rtB -> B_18_58_464 = _rtX -> u5_CSTATE ; } _rtB -> B_18_59_472 = 0.0 ; _rtB
-> B_18_59_472 += _rtP -> P_53 * _rtX -> u0HzLowpassFilter_CSTATE ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> Limits55_CSTATE >= _rtP -> P_55
) { if ( _rtX -> Limits55_CSTATE != _rtP -> P_55 ) { _rtX -> Limits55_CSTATE
= _rtP -> P_55 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else
if ( ( _rtX -> Limits55_CSTATE <= _rtP -> P_56 ) && ( _rtX -> Limits55_CSTATE
!= _rtP -> P_56 ) ) { _rtX -> Limits55_CSTATE = _rtP -> P_56 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB -> B_18_60_480 =
_rtX -> Limits55_CSTATE ; } else if ( _rtX -> Limits55_CSTATE >= _rtP -> P_55
) { _rtB -> B_18_60_480 = _rtP -> P_55 ; } else if ( _rtX -> Limits55_CSTATE
<= _rtP -> P_56 ) { _rtB -> B_18_60_480 = _rtP -> P_56 ; } else { _rtB ->
B_18_60_480 = _rtX -> Limits55_CSTATE ; } _rtB -> B_18_61_488 = 0.0 ; _rtB ->
B_18_61_488 += _rtP -> P_58 * _rtX -> u0HzLowpassFilter_CSTATE_e ; _rtB ->
B_18_62_496 = _rtB -> B_18_53_424 - _rtB -> B_18_37_296 ; _rtB -> B_18_63_504
= _rtB -> B_18_62_496 - _rtB -> B_18_61_488 ; _rtB -> B_18_64_512 = _rtP ->
P_59 * _rtB -> B_18_63_504 ; _rtB -> B_18_65_520 = _rtP -> P_60 * _rtB ->
B_18_64_512 ; _rtB -> B_18_66_528 = _rtB -> B_18_60_480 + _rtB -> B_18_65_520
; u0 = _rtB -> B_18_66_528 ; u1 = _rtP -> P_62 ; u2 = _rtP -> P_61 ; if ( u0
> u2 ) { _rtB -> B_18_67_536 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_67_536 = u1 ; } else { _rtB -> B_18_67_536 = u0 ; } _rtB -> B_18_68_544
= _rtB -> B_18_67_536 - _rtB -> B_18_59_472 ; _rtB -> B_18_69_552 = _rtP ->
P_63 * _rtB -> B_18_68_544 ; _rtB -> B_18_70_560 = _rtB -> B_18_58_464 + _rtB
-> B_18_69_552 ; _rtB -> B_18_71_568 = ( _rtB -> B_18_70_560 < 0.0 ) ; _rtB
-> B_18_72_576 = _rtP -> P_64 * _rtB -> B_18_71_568 ; _rtB -> B_18_73_584 [ 0
] = _rtB -> B_18_72_576 ; _rtB -> B_18_73_584 [ 1 ] = 0.0 ; _rtB ->
B_18_73_584 [ 2 ] = 0.0 ; _rtB -> B_18_73_584 [ 3 ] = 0.0 ; _rtB ->
B_18_77_616 = muDoubleScalarAbs ( _rtB -> B_18_70_560 ) ; u0 = _rtB ->
B_18_77_616 ; u1 = _rtP -> P_66 ; u2 = _rtP -> P_65 ; if ( u0 > u2 ) { _rtB
-> B_18_78_624 = u2 ; } else if ( u0 < u1 ) { _rtB -> B_18_78_624 = u1 ; }
else { _rtB -> B_18_78_624 = u0 ; } _rtB -> B_18_79_632 [ 0 ] = _rtB ->
B_18_78_624 ; _rtB -> B_18_79_632 [ 1 ] = 0.0 ; _rtB -> B_18_79_632 [ 2 ] =
0.0 ; _rtB -> B_18_79_632 [ 3 ] = 0.0 ; _rtB -> B_18_83_664 = ssGetT ( S ) ;
ssCallAccelRunBlock ( S , 6 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_84_672
[ 0 ] = _rtB -> B_6_1066_8528 ; _rtB -> B_18_84_672 [ 1 ] = 0.0 ; _rtB ->
B_18_84_672 [ 2 ] = 0.0 ; _rtB -> B_18_84_672 [ 3 ] = 0.0 ;
ssCallAccelRunBlock ( S , 18 , 43 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 44 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_113_904 [ 0 ] = _rtB -> B_18_106_848 [ 0 ] ; _rtB -> B_18_113_904 [ 1 ]
= 0.0 ; _rtB -> B_18_113_904 [ 2 ] = 0.0 ; _rtB -> B_18_113_904 [ 3 ] = 0.0 ;
if ( _rtDW -> INTERNAL_3_1_1_FirstOutput == 0.0 ) { _rtDW ->
INTERNAL_3_1_1_FirstOutput = 1.0 ; _rtX ->
sm_aileron_actuatoroutputFiltered_553247611_0 = _rtB -> B_18_27_216 [ 3 ] ; }
_rtB -> B_18_117_936 [ 0 ] = _rtX ->
sm_aileron_actuatoroutputFiltered_553247611_0 ; _rtB -> B_18_117_936 [ 1 ] =
( _rtB -> B_18_27_216 [ 3 ] - _rtX ->
sm_aileron_actuatoroutputFiltered_553247611_0 ) * 100.0 ; _rtB ->
B_18_117_936 [ 2 ] = 0.0 ; _rtB -> B_18_117_936 [ 3 ] = 0.0 ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> u5_CSTATE_f >= _rtP -> P_68 ) {
if ( _rtX -> u5_CSTATE_f != _rtP -> P_68 ) { _rtX -> u5_CSTATE_f = _rtP ->
P_68 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else if ( (
_rtX -> u5_CSTATE_f <= _rtP -> P_69 ) && ( _rtX -> u5_CSTATE_f != _rtP ->
P_69 ) ) { _rtX -> u5_CSTATE_f = _rtP -> P_69 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB -> B_18_121_968 =
_rtX -> u5_CSTATE_f ; } else if ( _rtX -> u5_CSTATE_f >= _rtP -> P_68 ) {
_rtB -> B_18_121_968 = _rtP -> P_68 ; } else if ( _rtX -> u5_CSTATE_f <= _rtP
-> P_69 ) { _rtB -> B_18_121_968 = _rtP -> P_69 ; } else { _rtB ->
B_18_121_968 = _rtX -> u5_CSTATE_f ; } _rtB -> B_18_122_976 = 0.0 ; _rtB ->
B_18_122_976 += _rtP -> P_71 * _rtX -> u0HzLowpassFilter_CSTATE_d ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> Limits55_CSTATE_g >= _rtP ->
P_73 ) { if ( _rtX -> Limits55_CSTATE_g != _rtP -> P_73 ) { _rtX ->
Limits55_CSTATE_g = _rtP -> P_73 ; ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } } else if ( ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_74 ) && ( _rtX
-> Limits55_CSTATE_g != _rtP -> P_74 ) ) { _rtX -> Limits55_CSTATE_g = _rtP
-> P_74 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB ->
B_18_123_984 = _rtX -> Limits55_CSTATE_g ; } else if ( _rtX ->
Limits55_CSTATE_g >= _rtP -> P_73 ) { _rtB -> B_18_123_984 = _rtP -> P_73 ; }
else if ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_74 ) { _rtB -> B_18_123_984
= _rtP -> P_74 ; } else { _rtB -> B_18_123_984 = _rtX -> Limits55_CSTATE_g ;
} _rtB -> B_18_124_992 = 0.0 ; _rtB -> B_18_124_992 += _rtP -> P_76 * _rtX ->
u0HzLowpassFilter_CSTATE_b ; _rtB -> B_18_125_1000 = _rtB -> B_18_62_496 -
_rtB -> B_18_124_992 ; _rtB -> B_18_126_1008 = _rtP -> P_77 * _rtB ->
B_18_125_1000 ; _rtB -> B_18_127_1016 = _rtP -> P_78 * _rtB -> B_18_126_1008
; _rtB -> B_18_128_1024 = _rtB -> B_18_123_984 + _rtB -> B_18_127_1016 ; u0 =
_rtB -> B_18_128_1024 ; u1 = _rtP -> P_80 ; u2 = _rtP -> P_79 ; if ( u0 > u2
) { _rtB -> B_18_129_1032 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_129_1032 = u1 ; } else { _rtB -> B_18_129_1032 = u0 ; } _rtB ->
B_18_130_1040 = _rtB -> B_18_129_1032 - _rtB -> B_18_122_976 ; _rtB ->
B_18_131_1048 = _rtP -> P_81 * _rtB -> B_18_130_1040 ; _rtB -> B_18_132_1056
= _rtB -> B_18_121_968 + _rtB -> B_18_131_1048 ; _rtB -> B_18_133_1064 = (
_rtB -> B_18_132_1056 < 0.0 ) ; _rtB -> B_18_134_1072 = _rtP -> P_82 * _rtB
-> B_18_133_1064 ; _rtB -> B_18_135_1080 [ 0 ] = _rtB -> B_18_134_1072 ; _rtB
-> B_18_135_1080 [ 1 ] = 0.0 ; _rtB -> B_18_135_1080 [ 2 ] = 0.0 ; _rtB ->
B_18_135_1080 [ 3 ] = 0.0 ; _rtB -> B_18_139_1112 = muDoubleScalarAbs ( _rtB
-> B_18_132_1056 ) ; u0 = _rtB -> B_18_139_1112 ; u1 = _rtP -> P_84 ; u2 =
_rtP -> P_83 ; if ( u0 > u2 ) { _rtB -> B_18_140_1120 = u2 ; } else if ( u0 <
u1 ) { _rtB -> B_18_140_1120 = u1 ; } else { _rtB -> B_18_140_1120 = u0 ; }
_rtB -> B_18_141_1128 [ 0 ] = _rtB -> B_18_140_1120 ; _rtB -> B_18_141_1128 [
1 ] = 0.0 ; _rtB -> B_18_141_1128 [ 2 ] = 0.0 ; _rtB -> B_18_141_1128 [ 3 ] =
0.0 ; _rtB -> B_18_145_1160 = ssGetT ( S ) ; ssCallAccelRunBlock ( S , 7 , 0
, SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_146_1168 [ 0 ] = _rtB -> B_7_1065_8520
; _rtB -> B_18_146_1168 [ 1 ] = 0.0 ; _rtB -> B_18_146_1168 [ 2 ] = 0.0 ;
_rtB -> B_18_146_1168 [ 3 ] = 0.0 ; ssCallAccelRunBlock ( S , 18 , 68 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 18 , 69 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_170_1360 [ 0 ] = _rtB -> B_18_163_1304 [
0 ] ; _rtB -> B_18_170_1360 [ 1 ] = 0.0 ; _rtB -> B_18_170_1360 [ 2 ] = 0.0 ;
_rtB -> B_18_170_1360 [ 3 ] = 0.0 ; if ( _rtDW -> INTERNAL_5_1_1_FirstOutput
== 0.0 ) { _rtDW -> INTERNAL_5_1_1_FirstOutput = 1.0 ; _rtX ->
sm_aileron_actuatoroutputFiltered_2948142811_0 = _rtB -> B_18_27_216 [ 5 ] ;
} _rtB -> B_18_174_1392 [ 0 ] = _rtX ->
sm_aileron_actuatoroutputFiltered_2948142811_0 ; _rtB -> B_18_174_1392 [ 1 ]
= ( _rtB -> B_18_27_216 [ 5 ] - _rtX ->
sm_aileron_actuatoroutputFiltered_2948142811_0 ) * 100.0 ; _rtB ->
B_18_174_1392 [ 2 ] = 0.0 ; _rtB -> B_18_174_1392 [ 3 ] = 0.0 ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> u5_CSTATE_n >= _rtP -> P_86 ) {
if ( _rtX -> u5_CSTATE_n != _rtP -> P_86 ) { _rtX -> u5_CSTATE_n = _rtP ->
P_86 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else if ( (
_rtX -> u5_CSTATE_n <= _rtP -> P_87 ) && ( _rtX -> u5_CSTATE_n != _rtP ->
P_87 ) ) { _rtX -> u5_CSTATE_n = _rtP -> P_87 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB -> B_18_178_1424 =
_rtX -> u5_CSTATE_n ; } else if ( _rtX -> u5_CSTATE_n >= _rtP -> P_86 ) {
_rtB -> B_18_178_1424 = _rtP -> P_86 ; } else if ( _rtX -> u5_CSTATE_n <=
_rtP -> P_87 ) { _rtB -> B_18_178_1424 = _rtP -> P_87 ; } else { _rtB ->
B_18_178_1424 = _rtX -> u5_CSTATE_n ; } _rtB -> B_18_179_1432 = 0.0 ; _rtB ->
B_18_179_1432 += _rtP -> P_89 * _rtX -> u0HzLowpassFilter_CSTATE_i ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> Limits55_CSTATE_p >= _rtP ->
P_91 ) { if ( _rtX -> Limits55_CSTATE_p != _rtP -> P_91 ) { _rtX ->
Limits55_CSTATE_p = _rtP -> P_91 ; ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } } else if ( ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_92 ) && ( _rtX
-> Limits55_CSTATE_p != _rtP -> P_92 ) ) { _rtX -> Limits55_CSTATE_p = _rtP
-> P_92 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB ->
B_18_180_1440 = _rtX -> Limits55_CSTATE_p ; } else if ( _rtX ->
Limits55_CSTATE_p >= _rtP -> P_91 ) { _rtB -> B_18_180_1440 = _rtP -> P_91 ;
} else if ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_92 ) { _rtB ->
B_18_180_1440 = _rtP -> P_92 ; } else { _rtB -> B_18_180_1440 = _rtX ->
Limits55_CSTATE_p ; } _rtB -> B_18_181_1448 = 0.0 ; _rtB -> B_18_181_1448 +=
_rtP -> P_94 * _rtX -> u0HzLowpassFilter_CSTATE_p ; _rtB -> B_18_182_1456 =
_rtB -> B_18_62_496 - _rtB -> B_18_181_1448 ; _rtB -> B_18_183_1464 = _rtP ->
P_95 * _rtB -> B_18_182_1456 ; _rtB -> B_18_184_1472 = _rtP -> P_96 * _rtB ->
B_18_183_1464 ; _rtB -> B_18_185_1480 = _rtB -> B_18_180_1440 + _rtB ->
B_18_184_1472 ; u0 = _rtB -> B_18_185_1480 ; u1 = _rtP -> P_98 ; u2 = _rtP ->
P_97 ; if ( u0 > u2 ) { _rtB -> B_18_186_1488 = u2 ; } else if ( u0 < u1 ) {
_rtB -> B_18_186_1488 = u1 ; } else { _rtB -> B_18_186_1488 = u0 ; } _rtB ->
B_18_187_1496 = _rtB -> B_18_186_1488 - _rtB -> B_18_179_1432 ; _rtB ->
B_18_188_1504 = _rtP -> P_99 * _rtB -> B_18_187_1496 ; _rtB -> B_18_189_1512
= _rtB -> B_18_178_1424 + _rtB -> B_18_188_1504 ; _rtB -> B_18_190_1520 = (
_rtB -> B_18_189_1512 < 0.0 ) ; _rtB -> B_18_191_1528 = _rtP -> P_100 * _rtB
-> B_18_190_1520 ; _rtB -> B_18_192_1536 [ 0 ] = _rtB -> B_18_191_1528 ; _rtB
-> B_18_192_1536 [ 1 ] = 0.0 ; _rtB -> B_18_192_1536 [ 2 ] = 0.0 ; _rtB ->
B_18_192_1536 [ 3 ] = 0.0 ; _rtB -> B_18_196_1568 = muDoubleScalarAbs ( _rtB
-> B_18_189_1512 ) ; u0 = _rtB -> B_18_196_1568 ; u1 = _rtP -> P_102 ; u2 =
_rtP -> P_101 ; if ( u0 > u2 ) { _rtB -> B_18_197_1576 = u2 ; } else if ( u0
< u1 ) { _rtB -> B_18_197_1576 = u1 ; } else { _rtB -> B_18_197_1576 = u0 ; }
_rtB -> B_18_198_1584 [ 0 ] = _rtB -> B_18_197_1576 ; _rtB -> B_18_198_1584 [
1 ] = 0.0 ; _rtB -> B_18_198_1584 [ 2 ] = 0.0 ; _rtB -> B_18_198_1584 [ 3 ] =
0.0 ; _rtB -> B_18_202_1616 = ssGetT ( S ) ; ssCallAccelRunBlock ( S , 8 , 0
, SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_203_1624 [ 0 ] = _rtB -> B_8_1064_8512
; _rtB -> B_18_203_1624 [ 1 ] = 0.0 ; _rtB -> B_18_203_1624 [ 2 ] = 0.0 ;
_rtB -> B_18_203_1624 [ 3 ] = 0.0 ; ssCallAccelRunBlock ( S , 18 , 93 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 18 , 94 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_226_1808 [ 0 ] = _rtB -> B_18_219_1752 [
0 ] ; _rtB -> B_18_226_1808 [ 1 ] = 0.0 ; _rtB -> B_18_226_1808 [ 2 ] = 0.0 ;
_rtB -> B_18_226_1808 [ 3 ] = 0.0 ; _rtB -> B_18_230_1840 [ 0 ] = _rtB ->
B_18_47_376 ; _rtB -> B_18_230_1840 [ 1 ] = 0.0 ; _rtB -> B_18_230_1840 [ 2 ]
= 0.0 ; _rtB -> B_18_230_1840 [ 3 ] = 0.0 ; ssCallAccelRunBlock ( S , 18 , 97
, SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
ssCallAccelRunBlock ( S , 18 , 98 , SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_Actuator_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Actuator_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_18_234_1872 [ 0 ] + 0 ) ; }
} { if ( _rtDW -> TAQSigLogging_InsertedFor_From_at_outport_0_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_From_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_18_53_424 + 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_Mechanical_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Mechanical_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_18_37_296 + 0 ) ; } } _rtB
-> B_18_237_1896 = _rtB -> B_18_53_424 * _rtP -> P_103 ; _rtB ->
B_18_238_1904 = _rtDW -> UD_DSTATE ; _rtB -> B_18_239_1912 = _rtB ->
B_18_237_1896 - _rtB -> B_18_238_1904 ; _rtB -> B_18_1075_8592 = ( _rtB ->
B_18_239_1912 > _rtB -> B_18_922_7376 ) ; for ( i = 0 ; i < 5 ; i ++ ) { if (
_rtB -> B_18_1075_8592 ) { _rtB -> B_18_240_1920 [ i ] = _rtB ->
B_18_947_7576 [ i ] ; } else { _rtB -> B_18_240_1920 [ i ] = 0.0 ; } } _rtB
-> B_18_247_1976 = _rtP -> P_105 ; _rtB -> B_18_248_1984 = _rtB ->
B_18_247_1976 - _rtB -> B_18_245_1960 [ 0 ] ; _rtB -> B_18_1076_8593 = ( _rtB
-> B_18_248_1984 <= sm_aileron_actuator_rtC ( S ) -> B_18_1106_8648 ) ; _rtB
-> B_18_1077_8594 = ( _rtB -> B_18_1076_8593 && _rtB -> B_18_1100_8617 ) ; }
_rtB -> B_18_249_1992 = _rtP -> P_106 * _rtB -> B_18_106_848 [ 6 ] ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING != 0 ) { _rtDW -> Integrator_DSTATE = _rtB ->
B_18_249_1992 ; if ( _rtDW -> Integrator_DSTATE > _rtP -> P_108 ) { _rtDW ->
Integrator_DSTATE = _rtP -> P_108 ; } else if ( _rtDW -> Integrator_DSTATE <
_rtP -> P_109 ) { _rtDW -> Integrator_DSTATE = _rtP -> P_109 ; } } if ( _rtB
-> B_18_1077_8594 || ( _rtDW -> Integrator_PrevResetState != 0 ) ) { _rtDW ->
Integrator_DSTATE = _rtB -> B_18_249_1992 ; if ( _rtDW -> Integrator_DSTATE >
_rtP -> P_108 ) { _rtDW -> Integrator_DSTATE = _rtP -> P_108 ; } else if (
_rtDW -> Integrator_DSTATE < _rtP -> P_109 ) { _rtDW -> Integrator_DSTATE =
_rtP -> P_109 ; } } _rtB -> B_18_250_2000 = _rtDW -> Integrator_DSTATE ; u0 =
_rtB -> B_18_250_2000 ; u1 = _rtP -> P_111 ; u2 = _rtP -> P_110 ; if ( u0 >
u2 ) { _rtB -> B_18_251_2008 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_251_2008 = u1 ; } else { _rtB -> B_18_251_2008 = u0 ; }
ssCallAccelRunBlock ( S , 12 , 0 , SS_CALL_MDL_OUTPUTS ) ; } if ( ( _rtDW ->
TimeStampA >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB >= ssGetT ( S ) ) ) {
_rtB -> B_18_252_2016 = 0.0 ; } else { u0 = _rtDW -> TimeStampA ; lastU = &
_rtDW -> LastUAtTimeA ; if ( _rtDW -> TimeStampA < _rtDW -> TimeStampB ) { if
( _rtDW -> TimeStampB < ssGetT ( S ) ) { u0 = _rtDW -> TimeStampB ; lastU = &
_rtDW -> LastUAtTimeB ; } } else if ( _rtDW -> TimeStampA >= ssGetT ( S ) ) {
u0 = _rtDW -> TimeStampB ; lastU = & _rtDW -> LastUAtTimeB ; } u0 = ssGetT (
S ) - u0 ; _rtB -> B_18_252_2016 = ( _rtB -> B_12_1060_8480 - * lastU ) / u0
; } _rtB -> B_18_1078_8595 = ( _rtB -> B_18_252_2016 > _rtB -> B_18_923_7384
) ; for ( i = 0 ; i < 5 ; i ++ ) { if ( _rtB -> B_18_1078_8595 ) { _rtB ->
B_18_253_2024 [ i ] = _rtB -> B_18_942_7536 [ i ] ; } else { _rtB ->
B_18_253_2024 [ i ] = 0.0 ; } } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0
) { _rtB -> B_18_260_2080 = _rtP -> P_112 ; _rtB -> B_18_261_2088 = _rtB ->
B_18_260_2080 - _rtB -> B_18_258_2064 [ 0 ] ; _rtB -> B_18_1079_8596 = ( _rtB
-> B_18_261_2088 <= sm_aileron_actuator_rtC ( S ) -> B_18_1103_8624 ) ; _rtB
-> B_18_1080_8597 = ( _rtB -> B_18_1079_8596 && _rtB -> B_18_1097_8614 ) ; }
_rtB -> B_18_262_2096 = _rtP -> P_113 * _rtB -> B_18_234_1872 [ 0 ] ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_h != 0 ) { _rtDW -> Integrator_DSTATE_j = _rtB ->
B_18_262_2096 ; if ( _rtDW -> Integrator_DSTATE_j > _rtP -> P_115 ) { _rtDW
-> Integrator_DSTATE_j = _rtP -> P_115 ; } else if ( _rtDW ->
Integrator_DSTATE_j < _rtP -> P_116 ) { _rtDW -> Integrator_DSTATE_j = _rtP
-> P_116 ; } } if ( _rtB -> B_18_1080_8597 || ( _rtDW ->
Integrator_PrevResetState_m != 0 ) ) { _rtDW -> Integrator_DSTATE_j = _rtB ->
B_18_262_2096 ; if ( _rtDW -> Integrator_DSTATE_j > _rtP -> P_115 ) { _rtDW
-> Integrator_DSTATE_j = _rtP -> P_115 ; } else if ( _rtDW ->
Integrator_DSTATE_j < _rtP -> P_116 ) { _rtDW -> Integrator_DSTATE_j = _rtP
-> P_116 ; } } _rtB -> B_18_263_2104 = _rtDW -> Integrator_DSTATE_j ; u0 =
_rtB -> B_18_263_2104 ; u1 = _rtP -> P_118 ; u2 = _rtP -> P_117 ; if ( u0 >
u2 ) { _rtB -> B_18_264_2112 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_264_2112 = u1 ; } else { _rtB -> B_18_264_2112 = u0 ; }
ssCallAccelRunBlock ( S , 9 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_1081_8598 = ( _rtB -> B_9_1063_8504 < _rtB -> B_18_941_7528 ) ; _rtB ->
B_18_1082_8599 = _rtDW -> Memory4_PreviousInput ; _rtB -> B_18_1083_8600 =
_rtDW -> Memory2_PreviousInput ; for ( i = 0 ; i < 5 ; i ++ ) { if ( _rtB ->
B_18_1081_8598 ) { _rtB -> B_18_265_2120 [ i ] = _rtB -> B_18_953_7624 [ i ]
; } else { _rtB -> B_18_265_2120 [ i ] = 0.0 ; } if ( _rtB -> B_18_1082_8599
) { _rtB -> B_18_270_2160 [ i ] = _rtB -> B_18_965_7720 [ i ] ; } else { _rtB
-> B_18_270_2160 [ i ] = 0.0 ; } if ( _rtB -> B_18_1083_8600 ) { _rtB ->
B_18_275_2200 [ i ] = _rtB -> B_18_959_7672 [ i ] ; } else { _rtB ->
B_18_275_2200 [ i ] = 0.0 ; } } _rtB -> B_18_1084_8601 = ( _rtB ->
B_18_239_1912 == _rtB -> B_18_924_7392 ) ; for ( i = 0 ; i < 5 ; i ++ ) { if
( _rtB -> B_18_1084_8601 ) { _rtB -> B_18_280_2240 [ i ] = _rtB ->
B_18_925_7400 [ i ] ; } else { _rtB -> B_18_280_2240 [ i ] = 0.0 ; } } } _rtB
-> B_18_1085_8602 = ( _rtB -> B_18_252_2016 < _rtB -> B_18_952_7616 ) ; for (
i = 0 ; i < 5 ; i ++ ) { if ( _rtB -> B_18_1085_8602 ) { _rtB ->
B_18_285_2280 [ i ] = _rtB -> B_18_930_7440 [ i ] ; } else { _rtB ->
B_18_285_2280 [ i ] = 0.0 ; } } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0
) { _rtB -> B_18_1086_8603 = ( _rtB -> B_9_1063_8504 > _rtB -> B_18_935_7480
) ; intVal = 0U ; for ( i = 0 ; i < 5 ; i ++ ) { if ( _rtB -> B_18_1086_8603
) { _rtB -> B_18_290_2320 [ i ] = _rtB -> B_18_936_7488 [ i ] ; } else { _rtB
-> B_18_290_2320 [ i ] = 0.0 ; } u0 = _rtDW -> Memory5_PreviousInput [ i ] ;
_rtB -> B_18_295_2360 [ i ] = u0 ; u = static_cast < uint32_T > ( u0 ) ;
intVal <<= 1U ; intVal |= u ; } _rtB -> B_18_1073_8584 = static_cast <
int32_T > ( intVal ) ; _rtB -> B_18_300_2400 = _rtDW -> Memory6_PreviousInput
; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_16_1011_8088 [ i ] = _rtB ->
B_18_240_1920 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_16_1011_8088
[ i + 5 ] = _rtB -> B_18_253_2024 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) {
_rtB -> B_16_1011_8088 [ i + 10 ] = _rtB -> B_18_265_2120 [ i ] ; } for ( i =
0 ; i < 5 ; i ++ ) { _rtB -> B_16_1011_8088 [ i + 15 ] = _rtB ->
B_18_270_2160 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_16_1011_8088
[ i + 20 ] = _rtB -> B_18_275_2200 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) {
_rtB -> B_16_1011_8088 [ i + 25 ] = _rtB -> B_18_280_2240 [ i ] ; } for ( i =
0 ; i < 5 ; i ++ ) { _rtB -> B_16_1011_8088 [ i + 30 ] = _rtB ->
B_18_285_2280 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_16_1011_8088
[ i + 35 ] = _rtB -> B_18_290_2320 [ i ] ; } ssCallAccelRunBlock ( S , 16 , 1
, SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_301_2408 [ 0 ] = _rtP -> P_121 [ 0 ] +
_rtB -> B_16_1051_8408 [ 1 ] ; _rtB -> B_18_301_2408 [ 1 ] = _rtP -> P_121 [
1 ] + _rtB -> B_16_1051_8408 [ 2 ] ; _rtB -> B_18_301_2408 [ 2 ] = _rtP ->
P_121 [ 2 ] + _rtB -> B_16_1051_8408 [ 3 ] ; _rtB -> B_18_301_2408 [ 3 ] =
_rtP -> P_121 [ 3 ] + _rtB -> B_16_1051_8408 [ 4 ] ; for ( i = 0 ; i < 5 ; i
++ ) { _rtB -> B_18_301_2408 [ i + 4 ] = _rtP -> P_121 [ i + 4 ] + _rtB ->
B_18_295_2360 [ i ] ; } u0 = 0.0 ; u1 = 0.0 ; u2 = 0.0 ; tmp = 0.0 ; tmp_0 =
0.0 ; B_18_388_3104_0 = 0.0 ; B_18_468_3744_0 = 0.0 ; tmp_1 = 0.0 ; tmp_2 =
0.0 ; tmp_3 = 0.0 ; tmp_4 = 0.0 ; tmp_5 = 0.0 ; tmp_6 = 0.0 ; tmp_7 = 0.0 ;
tmp_8 = 0.0 ; tmp_9 = 0.0 ; tmp_a = 0.0 ; tmp_b = 0.0 ; tmp_c = 0.0 ; tmp_d =
0.0 ; for ( i = 0 ; i < 9 ; i ++ ) { B_18_310_2480_0 = _rtP -> P_122 [ i ] *
_rtB -> B_18_301_2408 [ i ] ; _rtB -> B_18_310_2480 [ i ] = B_18_310_2480_0 ;
B_18_310_2480_0 += _rtP -> P_123 ; _rtB -> B_18_319_2552 [ i ] =
B_18_310_2480_0 ; tmp_e = _rtB -> B_18_615_4920 [ i ] ; u0 += tmp_e *
B_18_310_2480_0 ; tmp_e = _rtB -> B_18_714_5712 [ i ] ; u1 += tmp_e *
B_18_310_2480_0 ; tmp_e = _rtB -> B_18_732_5856 [ i ] ; u2 += tmp_e *
B_18_310_2480_0 ; tmp_e = _rtB -> B_18_741_5928 [ i ] ; tmp += tmp_e *
B_18_310_2480_0 ; tmp_e = _rtB -> B_18_750_6000 [ i ] ; tmp_0 += tmp_e *
B_18_310_2480_0 ; tmp_e = _rtB -> B_18_759_6072 [ i ] ; B_18_388_3104_0 +=
tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_768_6144 [ i ] ;
B_18_468_3744_0 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_777_6216 [
i ] ; tmp_1 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_786_6288 [ i ]
; tmp_2 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_624_4992 [ i ] ;
tmp_3 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_633_5064 [ i ] ;
tmp_4 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_642_5136 [ i ] ;
tmp_5 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_651_5208 [ i ] ;
tmp_6 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_660_5280 [ i ] ;
tmp_7 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_669_5352 [ i ] ;
tmp_8 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_678_5424 [ i ] ;
tmp_9 += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_687_5496 [ i ] ;
tmp_a += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_696_5568 [ i ] ;
tmp_b += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_705_5640 [ i ] ;
tmp_c += tmp_e * B_18_310_2480_0 ; tmp_e = _rtB -> B_18_723_5784 [ i ] ;
tmp_d += tmp_e * B_18_310_2480_0 ; } _rtB -> B_18_328_2624 = u0 ; _rtB ->
B_18_329_2632 = u1 ; _rtB -> B_18_330_2640 = u2 ; _rtB -> B_18_331_2648 = tmp
; _rtB -> B_18_332_2656 = tmp_0 ; _rtB -> B_18_333_2664 = B_18_388_3104_0 ;
_rtB -> B_18_334_2672 = B_18_468_3744_0 ; _rtB -> B_18_335_2680 = tmp_1 ;
_rtB -> B_18_336_2688 = tmp_2 ; _rtB -> B_18_337_2696 = tmp_3 ; _rtB ->
B_18_338_2704 = tmp_4 ; _rtB -> B_18_339_2712 = tmp_5 ; _rtB -> B_18_340_2720
= tmp_6 ; _rtB -> B_18_341_2728 = tmp_7 ; _rtB -> B_18_342_2736 = tmp_8 ;
_rtB -> B_18_343_2744 = tmp_9 ; _rtB -> B_18_344_2752 = tmp_a ; _rtB ->
B_18_345_2760 = tmp_b ; _rtB -> B_18_346_2768 = tmp_c ; _rtB -> B_18_347_2776
= tmp_d ; _rtB -> B_18_348_2784 [ 0 ] = _rtB -> B_18_328_2624 + _rtB ->
B_18_795_6360 [ 0 ] ; _rtB -> B_18_348_2784 [ 1 ] = _rtB -> B_18_329_2632 +
_rtB -> B_18_795_6360 [ 1 ] ; _rtB -> B_18_348_2784 [ 2 ] = _rtB ->
B_18_330_2640 + _rtB -> B_18_795_6360 [ 2 ] ; _rtB -> B_18_348_2784 [ 3 ] =
_rtB -> B_18_331_2648 + _rtB -> B_18_795_6360 [ 3 ] ; _rtB -> B_18_348_2784 [
4 ] = _rtB -> B_18_332_2656 + _rtB -> B_18_795_6360 [ 4 ] ; _rtB ->
B_18_348_2784 [ 5 ] = _rtB -> B_18_333_2664 + _rtB -> B_18_795_6360 [ 5 ] ;
_rtB -> B_18_348_2784 [ 6 ] = _rtB -> B_18_334_2672 + _rtB -> B_18_795_6360 [
6 ] ; _rtB -> B_18_348_2784 [ 7 ] = _rtB -> B_18_335_2680 + _rtB ->
B_18_795_6360 [ 7 ] ; _rtB -> B_18_348_2784 [ 8 ] = _rtB -> B_18_336_2688 +
_rtB -> B_18_795_6360 [ 8 ] ; _rtB -> B_18_348_2784 [ 9 ] = _rtB ->
B_18_337_2696 + _rtB -> B_18_795_6360 [ 9 ] ; _rtB -> B_18_348_2784 [ 10 ] =
_rtB -> B_18_338_2704 + _rtB -> B_18_795_6360 [ 10 ] ; _rtB -> B_18_348_2784
[ 11 ] = _rtB -> B_18_339_2712 + _rtB -> B_18_795_6360 [ 11 ] ; _rtB ->
B_18_348_2784 [ 12 ] = _rtB -> B_18_340_2720 + _rtB -> B_18_795_6360 [ 12 ] ;
_rtB -> B_18_348_2784 [ 13 ] = _rtB -> B_18_341_2728 + _rtB -> B_18_795_6360
[ 13 ] ; _rtB -> B_18_348_2784 [ 14 ] = _rtB -> B_18_342_2736 + _rtB ->
B_18_795_6360 [ 14 ] ; _rtB -> B_18_348_2784 [ 15 ] = _rtB -> B_18_343_2744 +
_rtB -> B_18_795_6360 [ 15 ] ; _rtB -> B_18_348_2784 [ 16 ] = _rtB ->
B_18_344_2752 + _rtB -> B_18_795_6360 [ 16 ] ; _rtB -> B_18_348_2784 [ 17 ] =
_rtB -> B_18_345_2760 + _rtB -> B_18_795_6360 [ 17 ] ; _rtB -> B_18_348_2784
[ 18 ] = _rtB -> B_18_346_2768 + _rtB -> B_18_795_6360 [ 18 ] ; _rtB ->
B_18_348_2784 [ 19 ] = _rtB -> B_18_347_2776 + _rtB -> B_18_795_6360 [ 19 ] ;
u0 = 0.0 ; u1 = 0.0 ; u2 = 0.0 ; tmp = 0.0 ; tmp_0 = 0.0 ; for ( i = 0 ; i <
20 ; i ++ ) { _rtB -> B_18_368_2944 [ i ] = _rtP -> P_124 * _rtB ->
B_18_348_2784 [ i ] ; B_18_388_3104_0 = muDoubleScalarExp ( _rtB ->
B_18_368_2944 [ i ] ) ; _rtB -> B_18_388_3104 [ i ] = B_18_388_3104_0 ;
B_18_388_3104_0 += _rtB -> B_18_815_6520 ; _rtB -> B_18_408_3264 [ i ] =
B_18_388_3104_0 ; B_18_388_3104_0 = 1.0 / B_18_388_3104_0 ; _rtB ->
B_18_428_3424 [ i ] = B_18_388_3104_0 ; B_18_388_3104_0 *= _rtP -> P_125 ;
_rtB -> B_18_448_3584 [ i ] = B_18_388_3104_0 ; B_18_468_3744_0 =
B_18_388_3104_0 - _rtB -> B_18_816_6528 ; _rtB -> B_18_468_3744 [ i ] =
B_18_468_3744_0 ; B_18_388_3104_0 = _rtB -> B_18_817_6536 [ i ] ; u0 +=
B_18_388_3104_0 * B_18_468_3744_0 ; B_18_388_3104_0 = _rtB -> B_18_837_6696 [
i ] ; u1 += B_18_388_3104_0 * B_18_468_3744_0 ; B_18_388_3104_0 = _rtB ->
B_18_857_6856 [ i ] ; u2 += B_18_388_3104_0 * B_18_468_3744_0 ;
B_18_388_3104_0 = _rtB -> B_18_877_7016 [ i ] ; tmp += B_18_388_3104_0 *
B_18_468_3744_0 ; B_18_388_3104_0 = _rtB -> B_18_897_7176 [ i ] ; tmp_0 +=
B_18_388_3104_0 * B_18_468_3744_0 ; } _rtB -> B_18_488_3904 = u0 ; _rtB ->
B_18_489_3912 = u1 ; _rtB -> B_18_490_3920 = u2 ; _rtB -> B_18_491_3928 = tmp
; _rtB -> B_18_492_3936 = tmp_0 ; _rtB -> B_18_493_3944 [ 0 ] = _rtB ->
B_18_488_3904 + _rtB -> B_18_917_7336 [ 0 ] ; _rtB -> B_18_493_3944 [ 1 ] =
_rtB -> B_18_489_3912 + _rtB -> B_18_917_7336 [ 1 ] ; _rtB -> B_18_493_3944 [
2 ] = _rtB -> B_18_490_3920 + _rtB -> B_18_917_7336 [ 2 ] ; _rtB ->
B_18_493_3944 [ 3 ] = _rtB -> B_18_491_3928 + _rtB -> B_18_917_7336 [ 3 ] ;
_rtB -> B_18_493_3944 [ 4 ] = _rtB -> B_18_492_3936 + _rtB -> B_18_917_7336 [
4 ] ; for ( i = 0 ; i < 5 ; i ++ ) { u0 = _rtB -> B_18_493_3944 [ i ] + _rtP
-> P_126 ; _rtB -> B_18_498_3984 [ i ] = u0 ; u0 *= _rtP -> P_127 [ i ] ;
_rtB -> B_18_503_4024 [ i ] = u0 ; _rtB -> B_18_508_4064 [ i ] = u0 + _rtP ->
P_128 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_18_513_4104 [ i ] =
muDoubleScalarRound ( _rtB -> B_18_508_4064 [ i ] ) ; } i = ssIsSampleHit ( S
, 1 , 0 ) ; if ( i != 0 ) { intVal = 0U ; for ( i = 0 ; i < 5 ; i ++ ) { u =
static_cast < uint32_T > ( _rtB -> B_18_513_4104 [ i ] ) ; intVal <<= 1U ;
intVal |= u ; } _rtB -> B_18_1074_8588 = static_cast < int32_T > ( intVal ) ;
ssCallAccelRunBlock ( S , 17 , 0 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 186 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 187 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_518_4144 = _rtDW -> Memory_PreviousInput ; ssCallAccelRunBlock ( S , 15
, 0 , SS_CALL_MDL_OUTPUTS ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay = simTime - _rtP -> P_130 ; _rtB -> B_18_519_4152 =
sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW ->
TransportDelay_IWORK . Head , _rtP -> P_131 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK . Head ] == ssGetT ( S ) )
) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_18_520_4160 = _rtDW -> Memory_PreviousInput_j ; _rtB -> B_18_521_4168 =
_rtDW -> Memory1_PreviousInput ; ssCallAccelRunBlock ( S , 18 , 193 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_1087_8604 = ( _rtB -> B_18_520_4160 >
_rtB -> B_18_958_7664 ) ; _rtB -> B_18_1088_8605 = ( _rtB -> B_18_521_4168 <
_rtB -> B_18_964_7712 ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_n . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_134 ; _rtB -> B_18_522_4176 =
sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_i . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_i . Last , _rtDW -> TransportDelay_IWORK_i . Tail ,
_rtDW -> TransportDelay_IWORK_i . Head , _rtP -> P_135 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_i .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_i . Head ] == ssGetT ( S )
) ) ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay1_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_136 ; _rtB -> B_18_523_4184 =
sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay1_IWORK . CircularBufSize , & _rtDW ->
TransportDelay1_IWORK . Last , _rtDW -> TransportDelay1_IWORK . Tail , _rtDW
-> TransportDelay1_IWORK . Head , _rtP -> P_137 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay1_IWORK .
CircularBufSize ) [ _rtDW -> TransportDelay1_IWORK . Head ] == ssGetT ( S ) )
) ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_18_524_4192 = _rtB -> B_18_37_296 * _rtP -> P_138 ; _rtB -> B_18_525_4200 =
_rtDW -> UD_DSTATE_a ; _rtB -> B_18_526_4208 = _rtB -> B_18_524_4192 - _rtB
-> B_18_525_4200 ; _rtB -> B_18_527_4216 = _rtB -> B_18_27_216 [ 0 ] * _rtP
-> P_140 ; _rtB -> B_18_528_4224 = _rtDW -> UD_DSTATE_p ; _rtB ->
B_18_529_4232 = _rtB -> B_18_527_4216 - _rtB -> B_18_528_4224 ; } if ( (
_rtDW -> TimeStampA_j >= ssGetT ( S ) ) && ( _rtDW -> TimeStampB_f >= ssGetT
( S ) ) ) { _rtB -> B_18_530_4240 = 0.0 ; } else { u0 = _rtDW -> TimeStampA_j
; lastU = & _rtDW -> LastUAtTimeA_o ; if ( _rtDW -> TimeStampA_j < _rtDW ->
TimeStampB_f ) { if ( _rtDW -> TimeStampB_f < ssGetT ( S ) ) { u0 = _rtDW ->
TimeStampB_f ; lastU = & _rtDW -> LastUAtTimeB_n ; } } else if ( _rtDW ->
TimeStampA_j >= ssGetT ( S ) ) { u0 = _rtDW -> TimeStampB_f ; lastU = & _rtDW
-> LastUAtTimeB_n ; } u0 = ssGetT ( S ) - u0 ; _rtB -> B_18_530_4240 = ( _rtB
-> B_18_529_4232 - * lastU ) / u0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if (
i != 0 ) { ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_533_4264 = _rtP -> P_142 ; _rtB -> B_18_534_4272 = _rtB -> B_18_533_4264
- _rtB -> B_18_531_4248 [ 0 ] ; _rtB -> B_18_1089_8606 = ( _rtB ->
B_18_534_4272 <= sm_aileron_actuator_rtC ( S ) -> B_18_1104_8632 ) ; _rtB ->
B_18_1090_8607 = ( _rtB -> B_18_1089_8606 && _rtB -> B_18_1098_8615 ) ; }
_rtB -> B_18_535_4280 = _rtP -> P_143 * _rtB -> B_18_234_1872 [ 1 ] ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_a != 0 ) { _rtDW -> Integrator_DSTATE_l = _rtB ->
B_18_535_4280 ; if ( _rtDW -> Integrator_DSTATE_l > _rtP -> P_145 ) { _rtDW
-> Integrator_DSTATE_l = _rtP -> P_145 ; } else if ( _rtDW ->
Integrator_DSTATE_l < _rtP -> P_146 ) { _rtDW -> Integrator_DSTATE_l = _rtP
-> P_146 ; } } if ( _rtB -> B_18_1090_8607 || ( _rtDW ->
Integrator_PrevResetState_f != 0 ) ) { _rtDW -> Integrator_DSTATE_l = _rtB ->
B_18_535_4280 ; if ( _rtDW -> Integrator_DSTATE_l > _rtP -> P_145 ) { _rtDW
-> Integrator_DSTATE_l = _rtP -> P_145 ; } else if ( _rtDW ->
Integrator_DSTATE_l < _rtP -> P_146 ) { _rtDW -> Integrator_DSTATE_l = _rtP
-> P_146 ; } } _rtB -> B_18_536_4288 = _rtDW -> Integrator_DSTATE_l ; u0 =
_rtB -> B_18_536_4288 ; u1 = _rtP -> P_148 ; u2 = _rtP -> P_147 ; if ( u0 >
u2 ) { _rtB -> B_18_537_4296 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_537_4296 = u1 ; } else { _rtB -> B_18_537_4296 = u0 ; }
ssCallAccelRunBlock ( S , 10 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_540_4320 = _rtP -> P_149 ; _rtB -> B_18_541_4328 = _rtB -> B_18_540_4320
- _rtB -> B_18_538_4304 [ 0 ] ; _rtB -> B_18_1091_8608 = ( _rtB ->
B_18_541_4328 <= sm_aileron_actuator_rtC ( S ) -> B_18_1107_8656 ) ; _rtB ->
B_18_1092_8609 = ( _rtB -> B_18_1091_8608 && _rtB -> B_18_1101_8618 ) ; }
_rtB -> B_18_542_4336 = _rtP -> P_150 * _rtB -> B_18_163_1304 [ 6 ] ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_m != 0 ) { _rtDW -> Integrator_DSTATE_e = _rtB ->
B_18_542_4336 ; if ( _rtDW -> Integrator_DSTATE_e > _rtP -> P_152 ) { _rtDW
-> Integrator_DSTATE_e = _rtP -> P_152 ; } else if ( _rtDW ->
Integrator_DSTATE_e < _rtP -> P_153 ) { _rtDW -> Integrator_DSTATE_e = _rtP
-> P_153 ; } } if ( _rtB -> B_18_1092_8609 || ( _rtDW ->
Integrator_PrevResetState_ms != 0 ) ) { _rtDW -> Integrator_DSTATE_e = _rtB
-> B_18_542_4336 ; if ( _rtDW -> Integrator_DSTATE_e > _rtP -> P_152 ) {
_rtDW -> Integrator_DSTATE_e = _rtP -> P_152 ; } else if ( _rtDW ->
Integrator_DSTATE_e < _rtP -> P_153 ) { _rtDW -> Integrator_DSTATE_e = _rtP
-> P_153 ; } } _rtB -> B_18_543_4344 = _rtDW -> Integrator_DSTATE_e ; u0 =
_rtB -> B_18_543_4344 ; u1 = _rtP -> P_155 ; u2 = _rtP -> P_154 ; if ( u0 >
u2 ) { _rtB -> B_18_544_4352 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_544_4352 = u1 ; } else { _rtB -> B_18_544_4352 = u0 ; }
ssCallAccelRunBlock ( S , 13 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_545_4360 = _rtB -> B_18_27_216 [ 2 ] * _rtP -> P_156 ; _rtB ->
B_18_546_4368 = _rtDW -> UD_DSTATE_h ; _rtB -> B_18_547_4376 = _rtB ->
B_18_545_4360 - _rtB -> B_18_546_4368 ; ssCallAccelRunBlock ( S , 1 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_18_550_4400 = _rtP -> P_158 ; _rtB ->
B_18_551_4408 = _rtB -> B_18_550_4400 - _rtB -> B_18_548_4384 [ 0 ] ; _rtB ->
B_18_1093_8610 = ( _rtB -> B_18_551_4408 <= sm_aileron_actuator_rtC ( S ) ->
B_18_1105_8640 ) ; _rtB -> B_18_1094_8611 = ( _rtB -> B_18_1093_8610 && _rtB
-> B_18_1099_8616 ) ; } _rtB -> B_18_552_4416 = _rtP -> P_159 * _rtB ->
B_18_234_1872 [ 2 ] ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if (
_rtDW -> Integrator_IC_LOADING_j != 0 ) { _rtDW -> Integrator_DSTATE_f = _rtB
-> B_18_552_4416 ; if ( _rtDW -> Integrator_DSTATE_f > _rtP -> P_161 ) {
_rtDW -> Integrator_DSTATE_f = _rtP -> P_161 ; } else if ( _rtDW ->
Integrator_DSTATE_f < _rtP -> P_162 ) { _rtDW -> Integrator_DSTATE_f = _rtP
-> P_162 ; } } if ( _rtB -> B_18_1094_8611 || ( _rtDW ->
Integrator_PrevResetState_p != 0 ) ) { _rtDW -> Integrator_DSTATE_f = _rtB ->
B_18_552_4416 ; if ( _rtDW -> Integrator_DSTATE_f > _rtP -> P_161 ) { _rtDW
-> Integrator_DSTATE_f = _rtP -> P_161 ; } else if ( _rtDW ->
Integrator_DSTATE_f < _rtP -> P_162 ) { _rtDW -> Integrator_DSTATE_f = _rtP
-> P_162 ; } } _rtB -> B_18_553_4424 = _rtDW -> Integrator_DSTATE_f ; u0 =
_rtB -> B_18_553_4424 ; u1 = _rtP -> P_164 ; u2 = _rtP -> P_163 ; if ( u0 >
u2 ) { _rtB -> B_18_554_4432 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_554_4432 = u1 ; } else { _rtB -> B_18_554_4432 = u0 ; }
ssCallAccelRunBlock ( S , 11 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_557_4456 = _rtP -> P_165 ; _rtB -> B_18_558_4464 = _rtB -> B_18_557_4456
- _rtB -> B_18_555_4440 [ 0 ] ; _rtB -> B_18_1095_8612 = ( _rtB ->
B_18_558_4464 <= sm_aileron_actuator_rtC ( S ) -> B_18_1108_8664 ) ; _rtB ->
B_18_1096_8613 = ( _rtB -> B_18_1095_8612 && _rtB -> B_18_1102_8619 ) ; }
_rtB -> B_18_559_4472 = _rtP -> P_166 * _rtB -> B_18_219_1752 [ 6 ] ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_k != 0 ) { _rtDW -> Integrator_DSTATE_lo = _rtB ->
B_18_559_4472 ; if ( _rtDW -> Integrator_DSTATE_lo > _rtP -> P_168 ) { _rtDW
-> Integrator_DSTATE_lo = _rtP -> P_168 ; } else if ( _rtDW ->
Integrator_DSTATE_lo < _rtP -> P_169 ) { _rtDW -> Integrator_DSTATE_lo = _rtP
-> P_169 ; } } if ( _rtB -> B_18_1096_8613 || ( _rtDW ->
Integrator_PrevResetState_k != 0 ) ) { _rtDW -> Integrator_DSTATE_lo = _rtB
-> B_18_559_4472 ; if ( _rtDW -> Integrator_DSTATE_lo > _rtP -> P_168 ) {
_rtDW -> Integrator_DSTATE_lo = _rtP -> P_168 ; } else if ( _rtDW ->
Integrator_DSTATE_lo < _rtP -> P_169 ) { _rtDW -> Integrator_DSTATE_lo = _rtP
-> P_169 ; } } _rtB -> B_18_560_4480 = _rtDW -> Integrator_DSTATE_lo ; u0 =
_rtB -> B_18_560_4480 ; u1 = _rtP -> P_171 ; u2 = _rtP -> P_170 ; if ( u0 >
u2 ) { _rtB -> B_18_561_4488 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_18_561_4488 = u1 ; } else { _rtB -> B_18_561_4488 = u0 ; }
ssCallAccelRunBlock ( S , 14 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_18_562_4496 = _rtB -> B_18_27_216 [ 4 ] * _rtP -> P_172 ; _rtB ->
B_18_563_4504 = _rtDW -> UD_DSTATE_i ; _rtB -> B_18_564_4512 = _rtB ->
B_18_562_4496 - _rtB -> B_18_563_4504 ; ssCallAccelRunBlock ( S , 2 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 3 , 0 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> B_18_565_4520 = _rtDW -> Memory_PreviousInput_p ;
ssCallAccelRunBlock ( S , 4 , 0 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock
( S , 5 , 0 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_18_566_4528 = _rtP -> P_175
* _rtB -> B_18_68_544 ; _rtB -> B_18_567_4536 = _rtP -> P_176 * _rtB ->
B_18_63_504 ; _rtB -> B_18_568_4544 = _rtP -> P_177 * _rtB -> B_18_567_4536 ;
ssCallAccelRunBlock ( S , 18 , 300 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 18 ,
301 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_18_570_4560 = _rtP -> P_178 * _rtB
-> B_18_130_1040 ; _rtB -> B_18_571_4568 = _rtP -> P_179 * _rtB ->
B_18_125_1000 ; _rtB -> B_18_572_4576 = _rtP -> P_180 * _rtB -> B_18_571_4568
; ssCallAccelRunBlock ( S , 18 , 386 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 18 ,
387 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_18_574_4592 = _rtP -> P_181 * _rtB
-> B_18_187_1496 ; _rtB -> B_18_575_4600 = _rtP -> P_182 * _rtB ->
B_18_182_1456 ; _rtB -> B_18_576_4608 = _rtP -> P_183 * _rtB -> B_18_575_4600
; ssCallAccelRunBlock ( S , 18 , 472 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 18 ,
473 , SS_CALL_MDL_OUTPUTS ) ; u0 = muDoubleScalarMax ( _rtB -> B_18_258_2064
[ 0 ] , _rtB -> B_18_260_2080 ) ; _rtB -> B_18_578_4624 = u0 ; _rtB ->
B_18_579_4632 = static_cast < real_T > ( _rtB -> B_18_578_4624 == 0.0 ) *
2.2204460492503131e-16 + _rtB -> B_18_578_4624 ; } _rtB -> B_18_580_4640 =
_rtB -> B_18_262_2096 - _rtB -> B_18_264_2112 ; _rtB -> B_18_581_4648 = 1.0 /
_rtB -> B_18_579_4632 * _rtB -> B_18_580_4640 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { u0 = muDoubleScalarMax ( _rtB -> B_18_531_4248 [ 0 ] ,
_rtB -> B_18_533_4264 ) ; _rtB -> B_18_582_4656 = u0 ; _rtB -> B_18_583_4664
= static_cast < real_T > ( _rtB -> B_18_582_4656 == 0.0 ) *
2.2204460492503131e-16 + _rtB -> B_18_582_4656 ; } _rtB -> B_18_584_4672 =
_rtB -> B_18_535_4280 - _rtB -> B_18_537_4296 ; _rtB -> B_18_585_4680 = 1.0 /
_rtB -> B_18_583_4664 * _rtB -> B_18_584_4672 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { u0 = muDoubleScalarMax ( _rtB -> B_18_548_4384 [ 0 ] ,
_rtB -> B_18_550_4400 ) ; _rtB -> B_18_586_4688 = u0 ; _rtB -> B_18_587_4696
= static_cast < real_T > ( _rtB -> B_18_586_4688 == 0.0 ) *
2.2204460492503131e-16 + _rtB -> B_18_586_4688 ; } _rtB -> B_18_588_4704 =
_rtB -> B_18_552_4416 - _rtB -> B_18_554_4432 ; _rtB -> B_18_589_4712 = 1.0 /
_rtB -> B_18_587_4696 * _rtB -> B_18_588_4704 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { u0 = muDoubleScalarMax ( _rtB -> B_18_245_1960 [ 0 ] ,
_rtB -> B_18_247_1976 ) ; _rtB -> B_18_590_4720 = u0 ; _rtB -> B_18_591_4728
= static_cast < real_T > ( _rtB -> B_18_590_4720 == 0.0 ) *
2.2204460492503131e-16 + _rtB -> B_18_590_4720 ; } _rtB -> B_18_592_4736 =
_rtB -> B_18_249_1992 - _rtB -> B_18_251_2008 ; _rtB -> B_18_593_4744 = 1.0 /
_rtB -> B_18_591_4728 * _rtB -> B_18_592_4736 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { u0 = muDoubleScalarMax ( _rtB -> B_18_538_4304 [ 0 ] ,
_rtB -> B_18_540_4320 ) ; _rtB -> B_18_594_4752 = u0 ; _rtB -> B_18_595_4760
= static_cast < real_T > ( _rtB -> B_18_594_4752 == 0.0 ) *
2.2204460492503131e-16 + _rtB -> B_18_594_4752 ; } _rtB -> B_18_596_4768 =
_rtB -> B_18_542_4336 - _rtB -> B_18_544_4352 ; _rtB -> B_18_597_4776 = 1.0 /
_rtB -> B_18_595_4760 * _rtB -> B_18_596_4768 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { u0 = muDoubleScalarMax ( _rtB -> B_18_555_4440 [ 0 ] ,
_rtB -> B_18_557_4456 ) ; _rtB -> B_18_598_4784 = u0 ; _rtB -> B_18_599_4792
= static_cast < real_T > ( _rtB -> B_18_598_4784 == 0.0 ) *
2.2204460492503131e-16 + _rtB -> B_18_598_4784 ; } _rtB -> B_18_600_4800 =
_rtB -> B_18_559_4472 - _rtB -> B_18_561_4488 ; _rtB -> B_18_601_4808 = 1.0 /
_rtB -> B_18_599_4792 * _rtB -> B_18_600_4800 ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 18 , 542 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 18 , 544 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 546 , SS_CALL_MDL_OUTPUTS ) ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_d . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP ->
P_184 ; _rtB -> B_18_602_4816 = sm_aileron_actuator_acc_rt_TDelayInterpolate
( tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_n .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_n . Last , _rtDW ->
TransportDelay_IWORK_n . Tail , _rtDW -> TransportDelay_IWORK_n . Head , _rtP
-> P_185 , 1 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer +
_rtDW -> TransportDelay_IWORK_n . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_n . Head ] == ssGetT ( S ) ) ) ) ; } _rtB ->
B_18_603_4824 [ 0 ] = _rtB -> B_18_106_848 [ 6 ] ; _rtB -> B_18_603_4824 [ 1
] = 0.0 ; _rtB -> B_18_603_4824 [ 2 ] = 0.0 ; _rtB -> B_18_603_4824 [ 3 ] =
0.0 ; _rtB -> B_18_607_4856 [ 0 ] = _rtB -> B_18_163_1304 [ 6 ] ; _rtB ->
B_18_607_4856 [ 1 ] = 0.0 ; _rtB -> B_18_607_4856 [ 2 ] = 0.0 ; _rtB ->
B_18_607_4856 [ 3 ] = 0.0 ; _rtB -> B_18_611_4888 [ 0 ] = _rtB ->
B_18_219_1752 [ 6 ] ; _rtB -> B_18_611_4888 [ 1 ] = 0.0 ; _rtB ->
B_18_611_4888 [ 2 ] = 0.0 ; _rtB -> B_18_611_4888 [ 3 ] = 0.0 ;
ssCallAccelRunBlock ( S , 18 , 554 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 555 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 1283 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 1284 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 1285 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 1286 , SS_CALL_MDL_OUTPUTS ) ;
UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID2 ( SimStruct * S ,
int_T tid ) { B_sm_aileron_actuator_T * _rtB ; P_sm_aileron_actuator_T * _rtP
; int32_T i ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ;
_rtB = ( ( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ; memcpy (
& _rtB -> B_18_615_4920 [ 0 ] , & _rtP -> P_186 [ 0 ] , 9U * sizeof ( real_T
) ) ; memcpy ( & _rtB -> B_18_624_4992 [ 0 ] , & _rtP -> P_187 [ 0 ] , 9U *
sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_633_5064 [ 0 ] , & _rtP ->
P_188 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_642_5136 [ 0
] , & _rtP -> P_189 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_18_651_5208 [ 0 ] , & _rtP -> P_190 [ 0 ] , 9U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_18_660_5280 [ 0 ] , & _rtP -> P_191 [ 0 ] , 9U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_18_669_5352 [ 0 ] , & _rtP -> P_192 [ 0 ]
, 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_678_5424 [ 0 ] , & _rtP
-> P_193 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_687_5496
[ 0 ] , & _rtP -> P_194 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_18_696_5568 [ 0 ] , & _rtP -> P_195 [ 0 ] , 9U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_18_705_5640 [ 0 ] , & _rtP -> P_196 [ 0 ] , 9U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_18_714_5712 [ 0 ] , & _rtP -> P_197 [ 0 ]
, 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_723_5784 [ 0 ] , & _rtP
-> P_198 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_732_5856
[ 0 ] , & _rtP -> P_199 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_18_741_5928 [ 0 ] , & _rtP -> P_200 [ 0 ] , 9U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_18_750_6000 [ 0 ] , & _rtP -> P_201 [ 0 ] , 9U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_18_759_6072 [ 0 ] , & _rtP -> P_202 [ 0 ]
, 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_768_6144 [ 0 ] , & _rtP
-> P_203 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_777_6216
[ 0 ] , & _rtP -> P_204 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_18_786_6288 [ 0 ] , & _rtP -> P_205 [ 0 ] , 9U * sizeof ( real_T ) ) ; _rtB
-> B_18_815_6520 = _rtP -> P_207 ; _rtB -> B_18_816_6528 = _rtP -> P_208 ;
memcpy ( & _rtB -> B_18_795_6360 [ 0 ] , & _rtP -> P_206 [ 0 ] , 20U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_18_817_6536 [ 0 ] , & _rtP -> P_209 [ 0 ]
, 20U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_837_6696 [ 0 ] , & _rtP
-> P_210 [ 0 ] , 20U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_18_857_6856
[ 0 ] , & _rtP -> P_211 [ 0 ] , 20U * sizeof ( real_T ) ) ; memcpy ( & _rtB
-> B_18_877_7016 [ 0 ] , & _rtP -> P_212 [ 0 ] , 20U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_18_897_7176 [ 0 ] , & _rtP -> P_213 [ 0 ] , 20U * sizeof
( real_T ) ) ; _rtB -> B_18_922_7376 = _rtP -> P_215 ; _rtB -> B_18_923_7384
= _rtP -> P_216 ; _rtB -> B_18_924_7392 = _rtP -> P_217 ; _rtB ->
B_18_935_7480 = _rtP -> P_220 ; _rtB -> B_18_941_7528 = _rtP -> P_222 ; _rtB
-> B_18_952_7616 = _rtP -> P_225 ; _rtB -> B_18_958_7664 = _rtP -> P_227 ;
_rtB -> B_18_964_7712 = _rtP -> P_229 ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB
-> B_18_917_7336 [ i ] = _rtP -> P_214 [ i ] ; _rtB -> B_18_925_7400 [ i ] =
_rtP -> P_218 [ i ] ; _rtB -> B_18_930_7440 [ i ] = _rtP -> P_219 [ i ] ;
_rtB -> B_18_936_7488 [ i ] = _rtP -> P_221 [ i ] ; _rtB -> B_18_942_7536 [ i
] = _rtP -> P_223 [ i ] ; _rtB -> B_18_947_7576 [ i ] = _rtP -> P_224 [ i ] ;
_rtB -> B_18_953_7624 [ i ] = _rtP -> P_226 [ i ] ; _rtB -> B_18_959_7672 [ i
] = _rtP -> P_228 [ i ] ; _rtB -> B_18_965_7720 [ i ] = _rtP -> P_230 [ i ] ;
} _rtB -> B_18_970_7760 = _rtP -> P_231 ; _rtB -> B_18_971_7768 = _rtP ->
P_232 ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_18_972_7776 [ i ] = _rtP ->
P_233 [ i ] ; } _rtB -> B_18_977_7816 = _rtP -> P_234 ; _rtB -> B_18_978_7824
= _rtP -> P_235 ; _rtB -> B_18_979_7832 = _rtP -> P_236 ; for ( i = 0 ; i < 5
; i ++ ) { _rtB -> B_18_980_7840 [ i ] = _rtP -> P_237 [ i ] ; } _rtB ->
B_18_985_7880 = _rtP -> P_238 ; _rtB -> B_18_986_7888 = _rtP -> P_239 ; _rtB
-> B_18_987_7896 = _rtP -> P_240 ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB ->
B_18_988_7904 [ i ] = _rtP -> P_241 [ i ] ; } _rtB -> B_18_993_7944 = _rtP ->
P_242 ; _rtB -> B_18_994_7952 = _rtP -> P_243 ; _rtB -> B_18_995_7960 = _rtP
-> P_244 ; _rtB -> B_18_1097_8614 = ( _rtB -> B_18_994_7952 < _rtB ->
B_18_995_7960 ) ; _rtB -> B_18_996_7968 = _rtP -> P_245 ; _rtB ->
B_18_997_7976 = _rtP -> P_246 ; _rtB -> B_18_1098_8615 = ( _rtB ->
B_18_996_7968 < _rtB -> B_18_997_7976 ) ; _rtB -> B_18_998_7984 = _rtP ->
P_247 ; _rtB -> B_18_999_7992 = _rtP -> P_248 ; _rtB -> B_18_1099_8616 = (
_rtB -> B_18_998_7984 < _rtB -> B_18_999_7992 ) ; _rtB -> B_18_1000_8000 =
_rtP -> P_249 ; _rtB -> B_18_1001_8008 = _rtP -> P_250 ; _rtB ->
B_18_1100_8617 = ( _rtB -> B_18_1000_8000 < _rtB -> B_18_1001_8008 ) ; _rtB
-> B_18_1002_8016 = _rtP -> P_251 ; _rtB -> B_18_1003_8024 = _rtP -> P_252 ;
_rtB -> B_18_1101_8618 = ( _rtB -> B_18_1002_8016 < _rtB -> B_18_1003_8024 )
; _rtB -> B_18_1004_8032 = _rtP -> P_253 ; _rtB -> B_18_1005_8040 = _rtP ->
P_254 ; _rtB -> B_18_1102_8619 = ( _rtB -> B_18_1004_8032 < _rtB ->
B_18_1005_8040 ) ; _rtB -> B_18_1006_8048 = _rtP -> P_255 ;
ssCallAccelRunBlock ( S , 18 , 1555 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 1556 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 18 , 1557 , SS_CALL_MDL_OUTPUTS ) ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { B_sm_aileron_actuator_T
* _rtB ; DW_sm_aileron_actuator_T * _rtDW ; P_sm_aileron_actuator_T * _rtP ;
real_T * lastU ; int32_T i ; _rtDW = ( ( DW_sm_aileron_actuator_T * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp
( S ) ) ; _rtB = ( ( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 18 , 1 , SS_CALL_MDL_UPDATE ) ; ssCallAccelRunBlock
( S , 18 , 43 , SS_CALL_MDL_UPDATE ) ; ssCallAccelRunBlock ( S , 18 , 68 ,
SS_CALL_MDL_UPDATE ) ; ssCallAccelRunBlock ( S , 18 , 93 , SS_CALL_MDL_UPDATE
) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> UD_DSTATE =
_rtB -> B_18_237_1896 ; _rtDW -> Integrator_IC_LOADING = 0U ; _rtDW ->
Integrator_DSTATE += _rtP -> P_107 * _rtB -> B_18_593_4744 ; if ( _rtDW ->
Integrator_DSTATE > _rtP -> P_108 ) { _rtDW -> Integrator_DSTATE = _rtP ->
P_108 ; } else if ( _rtDW -> Integrator_DSTATE < _rtP -> P_109 ) { _rtDW ->
Integrator_DSTATE = _rtP -> P_109 ; } _rtDW -> Integrator_PrevResetState =
static_cast < int8_T > ( _rtB -> B_18_1077_8594 ) ; } if ( _rtDW ->
TimeStampA == ( rtInf ) ) { _rtDW -> TimeStampA = ssGetT ( S ) ; lastU = &
_rtDW -> LastUAtTimeA ; } else if ( _rtDW -> TimeStampB == ( rtInf ) ) {
_rtDW -> TimeStampB = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB ; } else
if ( _rtDW -> TimeStampA < _rtDW -> TimeStampB ) { _rtDW -> TimeStampA =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA ; } else { _rtDW -> TimeStampB
= ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB ; } * lastU = _rtB ->
B_12_1060_8480 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Integrator_IC_LOADING_h = 0U ; _rtDW -> Integrator_DSTATE_j += _rtP -> P_114
* _rtB -> B_18_581_4648 ; if ( _rtDW -> Integrator_DSTATE_j > _rtP -> P_115 )
{ _rtDW -> Integrator_DSTATE_j = _rtP -> P_115 ; } else if ( _rtDW ->
Integrator_DSTATE_j < _rtP -> P_116 ) { _rtDW -> Integrator_DSTATE_j = _rtP
-> P_116 ; } _rtDW -> Integrator_PrevResetState_m = static_cast < int8_T > (
_rtB -> B_18_1080_8597 ) ; _rtDW -> Memory4_PreviousInput = _rtB ->
B_18_1088_8605 ; _rtDW -> Memory2_PreviousInput = _rtB -> B_18_1087_8604 ;
for ( i = 0 ; i < 5 ; i ++ ) { _rtDW -> Memory5_PreviousInput [ i ] = _rtB ->
B_18_513_4104 [ i ] ; } _rtDW -> Memory6_PreviousInput = _rtB ->
B_16_1056_8448 ; _rtDW -> Memory_PreviousInput = _rtB -> B_18_519_4152 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK . Head = ( ( _rtDW -> TransportDelay_IWORK . Head < (
_rtDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK
. Head == _rtDW -> TransportDelay_IWORK . Tail ) { if ( !
sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK . CircularBufSize , & _rtDW -> TransportDelay_IWORK .
Tail , & _rtDW -> TransportDelay_IWORK . Head , & _rtDW ->
TransportDelay_IWORK . Last , simTime - _rtP -> P_130 , uBuffer , ( boolean_T
) 0 , false , & _rtDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
uBuffer + _rtDW -> TransportDelay_IWORK . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK . Head ] = _rtB -> B_18_526_4208 ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_j = _rtB ->
B_18_522_4176 ; _rtDW -> Memory1_PreviousInput = _rtB -> B_18_523_4184 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_n .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_i . Head = ( ( _rtDW -> TransportDelay_IWORK_i . Head <
( _rtDW -> TransportDelay_IWORK_i . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_i . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_i . Head == _rtDW -> TransportDelay_IWORK_i . Tail ) {
if ( ! sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_i . CircularBufSize , & _rtDW -> TransportDelay_IWORK_i
. Tail , & _rtDW -> TransportDelay_IWORK_i . Head , & _rtDW ->
TransportDelay_IWORK_i . Last , simTime - _rtP -> P_134 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_i . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_i . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_i . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_i . Head ] = _rtB -> B_18_526_4208 ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay1_PWORK . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay1_IWORK . Head = (
( _rtDW -> TransportDelay1_IWORK . Head < ( _rtDW -> TransportDelay1_IWORK .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay1_IWORK . Head + 1 ) : 0 )
; if ( _rtDW -> TransportDelay1_IWORK . Head == _rtDW ->
TransportDelay1_IWORK . Tail ) { if ( !
sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay1_IWORK . CircularBufSize , & _rtDW -> TransportDelay1_IWORK .
Tail , & _rtDW -> TransportDelay1_IWORK . Head , & _rtDW ->
TransportDelay1_IWORK . Last , simTime - _rtP -> P_136 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay1_IWORK . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay1_IWORK . CircularBufSize ) [ _rtDW ->
TransportDelay1_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay1_IWORK . Head ] = _rtB -> B_18_529_4232 ; } i = ssIsSampleHit
( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> UD_DSTATE_a = _rtB -> B_18_524_4192
; _rtDW -> UD_DSTATE_p = _rtB -> B_18_527_4216 ; } if ( _rtDW -> TimeStampA_j
== ( rtInf ) ) { _rtDW -> TimeStampA_j = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeA_o ; } else if ( _rtDW -> TimeStampB_f == ( rtInf ) ) { _rtDW ->
TimeStampB_f = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_n ; } else if (
_rtDW -> TimeStampA_j < _rtDW -> TimeStampB_f ) { _rtDW -> TimeStampA_j =
ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_o ; } else { _rtDW ->
TimeStampB_f = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_n ; } * lastU =
_rtB -> B_18_529_4232 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
_rtDW -> Integrator_IC_LOADING_a = 0U ; _rtDW -> Integrator_DSTATE_l += _rtP
-> P_144 * _rtB -> B_18_585_4680 ; if ( _rtDW -> Integrator_DSTATE_l > _rtP
-> P_145 ) { _rtDW -> Integrator_DSTATE_l = _rtP -> P_145 ; } else if ( _rtDW
-> Integrator_DSTATE_l < _rtP -> P_146 ) { _rtDW -> Integrator_DSTATE_l =
_rtP -> P_146 ; } _rtDW -> Integrator_PrevResetState_f = static_cast < int8_T
> ( _rtB -> B_18_1090_8607 ) ; _rtDW -> Integrator_IC_LOADING_m = 0U ; _rtDW
-> Integrator_DSTATE_e += _rtP -> P_151 * _rtB -> B_18_597_4776 ; if ( _rtDW
-> Integrator_DSTATE_e > _rtP -> P_152 ) { _rtDW -> Integrator_DSTATE_e =
_rtP -> P_152 ; } else if ( _rtDW -> Integrator_DSTATE_e < _rtP -> P_153 ) {
_rtDW -> Integrator_DSTATE_e = _rtP -> P_153 ; } _rtDW ->
Integrator_PrevResetState_ms = static_cast < int8_T > ( _rtB ->
B_18_1092_8609 ) ; _rtDW -> UD_DSTATE_h = _rtB -> B_18_545_4360 ; _rtDW ->
Integrator_IC_LOADING_j = 0U ; _rtDW -> Integrator_DSTATE_f += _rtP -> P_160
* _rtB -> B_18_589_4712 ; if ( _rtDW -> Integrator_DSTATE_f > _rtP -> P_161 )
{ _rtDW -> Integrator_DSTATE_f = _rtP -> P_161 ; } else if ( _rtDW ->
Integrator_DSTATE_f < _rtP -> P_162 ) { _rtDW -> Integrator_DSTATE_f = _rtP
-> P_162 ; } _rtDW -> Integrator_PrevResetState_p = static_cast < int8_T > (
_rtB -> B_18_1094_8611 ) ; _rtDW -> Integrator_IC_LOADING_k = 0U ; _rtDW ->
Integrator_DSTATE_lo += _rtP -> P_167 * _rtB -> B_18_601_4808 ; if ( _rtDW ->
Integrator_DSTATE_lo > _rtP -> P_168 ) { _rtDW -> Integrator_DSTATE_lo = _rtP
-> P_168 ; } else if ( _rtDW -> Integrator_DSTATE_lo < _rtP -> P_169 ) {
_rtDW -> Integrator_DSTATE_lo = _rtP -> P_169 ; } _rtDW ->
Integrator_PrevResetState_k = static_cast < int8_T > ( _rtB -> B_18_1096_8613
) ; _rtDW -> UD_DSTATE_i = _rtB -> B_18_562_4496 ; _rtDW ->
Memory_PreviousInput_p = _rtB -> B_18_602_4816 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_n . Head = ( ( _rtDW
-> TransportDelay_IWORK_n . Head < ( _rtDW -> TransportDelay_IWORK_n .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_n . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_n . Head == _rtDW ->
TransportDelay_IWORK_n . Tail ) { if ( !
sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize , & _rtDW -> TransportDelay_IWORK_n
. Tail , & _rtDW -> TransportDelay_IWORK_n . Head , & _rtDW ->
TransportDelay_IWORK_n . Last , simTime - _rtP -> P_184 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_n . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } (
* uBuffer + _rtDW -> TransportDelay_IWORK_n . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_n . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_n . Head ] = _rtB -> B_18_526_4208 ; }
ssCallAccelRunBlock ( S , 18 , 555 , SS_CALL_MDL_UPDATE ) ; UNUSED_PARAMETER
( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID2 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_sm_aileron_actuator_T * _rtB
; P_sm_aileron_actuator_T * _rtP ; XDot_sm_aileron_actuator_T * _rtXdot ;
X_sm_aileron_actuator_T * _rtX ; _rtXdot = ( ( XDot_sm_aileron_actuator_T * )
ssGetdX ( S ) ) ; _rtX = ( ( X_sm_aileron_actuator_T * ) ssGetContStates ( S
) ) ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ; _rtB = (
( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 18 , 1 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_1513730075_0 = ( _rtB -> B_18_27_216 [ 1 ]
- _rtX -> sm_aileron_actuatoroutputFiltered_1513730075_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE > _rtP -> P_51 ) && ( _rtX -> u5_CSTATE < _rtP -> P_50 ) )
|| ( ( _rtX -> u5_CSTATE <= _rtP -> P_51 ) && ( _rtB -> B_18_566_4528 > 0.0 )
) || ( ( _rtX -> u5_CSTATE >= _rtP -> P_50 ) && ( _rtB -> B_18_566_4528 < 0.0
) ) ) { _rtXdot -> u5_CSTATE = _rtB -> B_18_566_4528 ; } else { _rtXdot ->
u5_CSTATE = 0.0 ; } _rtXdot -> u0HzLowpassFilter_CSTATE = 0.0 ; _rtXdot ->
u0HzLowpassFilter_CSTATE += _rtP -> P_52 * _rtX -> u0HzLowpassFilter_CSTATE ;
_rtXdot -> u0HzLowpassFilter_CSTATE += _rtB -> B_18_106_848 [ 1 ] ; if ( ( (
_rtX -> Limits55_CSTATE > _rtP -> P_56 ) && ( _rtX -> Limits55_CSTATE < _rtP
-> P_55 ) ) || ( ( _rtX -> Limits55_CSTATE <= _rtP -> P_56 ) && ( _rtB ->
B_18_568_4544 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE >= _rtP -> P_55 ) && (
_rtB -> B_18_568_4544 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE = _rtB ->
B_18_568_4544 ; } else { _rtXdot -> Limits55_CSTATE = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_e = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_e +=
_rtP -> P_57 * _rtX -> u0HzLowpassFilter_CSTATE_e ; _rtXdot ->
u0HzLowpassFilter_CSTATE_e += _rtB -> B_18_106_848 [ 2 ] ;
ssCallAccelRunBlock ( S , 18 , 43 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_553247611_0 = ( _rtB -> B_18_27_216 [ 3 ] -
_rtX -> sm_aileron_actuatoroutputFiltered_553247611_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE_f > _rtP -> P_69 ) && ( _rtX -> u5_CSTATE_f < _rtP -> P_68
) ) || ( ( _rtX -> u5_CSTATE_f <= _rtP -> P_69 ) && ( _rtB -> B_18_570_4560 >
0.0 ) ) || ( ( _rtX -> u5_CSTATE_f >= _rtP -> P_68 ) && ( _rtB ->
B_18_570_4560 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_f = _rtB -> B_18_570_4560 ;
} else { _rtXdot -> u5_CSTATE_f = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_d = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_d +=
_rtP -> P_70 * _rtX -> u0HzLowpassFilter_CSTATE_d ; _rtXdot ->
u0HzLowpassFilter_CSTATE_d += _rtB -> B_18_163_1304 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_g > _rtP -> P_74 ) && ( _rtX -> Limits55_CSTATE_g < _rtP ->
P_73 ) ) || ( ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_74 ) && ( _rtB ->
B_18_572_4576 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_g >= _rtP -> P_73 ) &&
( _rtB -> B_18_572_4576 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_g = _rtB ->
B_18_572_4576 ; } else { _rtXdot -> Limits55_CSTATE_g = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_b = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_b +=
_rtP -> P_75 * _rtX -> u0HzLowpassFilter_CSTATE_b ; _rtXdot ->
u0HzLowpassFilter_CSTATE_b += _rtB -> B_18_163_1304 [ 2 ] ;
ssCallAccelRunBlock ( S , 18 , 68 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_2948142811_0 = ( _rtB -> B_18_27_216 [ 5 ]
- _rtX -> sm_aileron_actuatoroutputFiltered_2948142811_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE_n > _rtP -> P_87 ) && ( _rtX -> u5_CSTATE_n < _rtP -> P_86
) ) || ( ( _rtX -> u5_CSTATE_n <= _rtP -> P_87 ) && ( _rtB -> B_18_574_4592 >
0.0 ) ) || ( ( _rtX -> u5_CSTATE_n >= _rtP -> P_86 ) && ( _rtB ->
B_18_574_4592 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_n = _rtB -> B_18_574_4592 ;
} else { _rtXdot -> u5_CSTATE_n = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_i = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_i +=
_rtP -> P_88 * _rtX -> u0HzLowpassFilter_CSTATE_i ; _rtXdot ->
u0HzLowpassFilter_CSTATE_i += _rtB -> B_18_219_1752 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_p > _rtP -> P_92 ) && ( _rtX -> Limits55_CSTATE_p < _rtP ->
P_91 ) ) || ( ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_92 ) && ( _rtB ->
B_18_576_4608 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_p >= _rtP -> P_91 ) &&
( _rtB -> B_18_576_4608 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_p = _rtB ->
B_18_576_4608 ; } else { _rtXdot -> Limits55_CSTATE_p = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_p = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_p +=
_rtP -> P_93 * _rtX -> u0HzLowpassFilter_CSTATE_p ; _rtXdot ->
u0HzLowpassFilter_CSTATE_p += _rtB -> B_18_219_1752 [ 2 ] ;
ssCallAccelRunBlock ( S , 18 , 93 , SS_CALL_MDL_DERIVATIVES ) ; }
#define MDL_PROJECTION
static void mdlProjection ( SimStruct * S ) { ssCallAccelRunBlock ( S , 18 ,
1 , SS_CALL_MDL_PROJECTION ) ; ssCallAccelRunBlock ( S , 18 , 43 ,
SS_CALL_MDL_PROJECTION ) ; ssCallAccelRunBlock ( S , 18 , 68 ,
SS_CALL_MDL_PROJECTION ) ; ssCallAccelRunBlock ( S , 18 , 93 ,
SS_CALL_MDL_PROJECTION ) ; }
#define MDL_FORCINGFUNCTION
static void mdlForcingFunction ( SimStruct * S ) { B_sm_aileron_actuator_T *
_rtB ; P_sm_aileron_actuator_T * _rtP ; XDot_sm_aileron_actuator_T * _rtXdot
; X_sm_aileron_actuator_T * _rtX ; _rtXdot = ( ( XDot_sm_aileron_actuator_T *
) ssGetdX ( S ) ) ; _rtX = ( ( X_sm_aileron_actuator_T * ) ssGetContStates (
S ) ) ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ; _rtB =
( ( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 18 , 1 , SS_CALL_MDL_FORCINGFUNCTION ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_1513730075_0 = ( _rtB -> B_18_27_216 [ 1 ]
- _rtX -> sm_aileron_actuatoroutputFiltered_1513730075_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE > _rtP -> P_51 ) && ( _rtX -> u5_CSTATE < _rtP -> P_50 ) )
|| ( ( _rtX -> u5_CSTATE <= _rtP -> P_51 ) && ( _rtB -> B_18_566_4528 > 0.0 )
) || ( ( _rtX -> u5_CSTATE >= _rtP -> P_50 ) && ( _rtB -> B_18_566_4528 < 0.0
) ) ) { _rtXdot -> u5_CSTATE = _rtB -> B_18_566_4528 ; } else { _rtXdot ->
u5_CSTATE = 0.0 ; } _rtXdot -> u0HzLowpassFilter_CSTATE = 0.0 ; _rtXdot ->
u0HzLowpassFilter_CSTATE += _rtP -> P_52 * _rtX -> u0HzLowpassFilter_CSTATE ;
_rtXdot -> u0HzLowpassFilter_CSTATE += _rtB -> B_18_106_848 [ 1 ] ; if ( ( (
_rtX -> Limits55_CSTATE > _rtP -> P_56 ) && ( _rtX -> Limits55_CSTATE < _rtP
-> P_55 ) ) || ( ( _rtX -> Limits55_CSTATE <= _rtP -> P_56 ) && ( _rtB ->
B_18_568_4544 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE >= _rtP -> P_55 ) && (
_rtB -> B_18_568_4544 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE = _rtB ->
B_18_568_4544 ; } else { _rtXdot -> Limits55_CSTATE = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_e = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_e +=
_rtP -> P_57 * _rtX -> u0HzLowpassFilter_CSTATE_e ; _rtXdot ->
u0HzLowpassFilter_CSTATE_e += _rtB -> B_18_106_848 [ 2 ] ;
ssCallAccelRunBlock ( S , 18 , 43 , SS_CALL_MDL_FORCINGFUNCTION ) ; _rtXdot
-> sm_aileron_actuatoroutputFiltered_553247611_0 = ( _rtB -> B_18_27_216 [ 3
] - _rtX -> sm_aileron_actuatoroutputFiltered_553247611_0 ) * 100.0 ; if ( (
( _rtX -> u5_CSTATE_f > _rtP -> P_69 ) && ( _rtX -> u5_CSTATE_f < _rtP ->
P_68 ) ) || ( ( _rtX -> u5_CSTATE_f <= _rtP -> P_69 ) && ( _rtB ->
B_18_570_4560 > 0.0 ) ) || ( ( _rtX -> u5_CSTATE_f >= _rtP -> P_68 ) && (
_rtB -> B_18_570_4560 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_f = _rtB ->
B_18_570_4560 ; } else { _rtXdot -> u5_CSTATE_f = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_d = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_d +=
_rtP -> P_70 * _rtX -> u0HzLowpassFilter_CSTATE_d ; _rtXdot ->
u0HzLowpassFilter_CSTATE_d += _rtB -> B_18_163_1304 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_g > _rtP -> P_74 ) && ( _rtX -> Limits55_CSTATE_g < _rtP ->
P_73 ) ) || ( ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_74 ) && ( _rtB ->
B_18_572_4576 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_g >= _rtP -> P_73 ) &&
( _rtB -> B_18_572_4576 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_g = _rtB ->
B_18_572_4576 ; } else { _rtXdot -> Limits55_CSTATE_g = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_b = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_b +=
_rtP -> P_75 * _rtX -> u0HzLowpassFilter_CSTATE_b ; _rtXdot ->
u0HzLowpassFilter_CSTATE_b += _rtB -> B_18_163_1304 [ 2 ] ;
ssCallAccelRunBlock ( S , 18 , 68 , SS_CALL_MDL_FORCINGFUNCTION ) ; _rtXdot
-> sm_aileron_actuatoroutputFiltered_2948142811_0 = ( _rtB -> B_18_27_216 [ 5
] - _rtX -> sm_aileron_actuatoroutputFiltered_2948142811_0 ) * 100.0 ; if ( (
( _rtX -> u5_CSTATE_n > _rtP -> P_87 ) && ( _rtX -> u5_CSTATE_n < _rtP ->
P_86 ) ) || ( ( _rtX -> u5_CSTATE_n <= _rtP -> P_87 ) && ( _rtB ->
B_18_574_4592 > 0.0 ) ) || ( ( _rtX -> u5_CSTATE_n >= _rtP -> P_86 ) && (
_rtB -> B_18_574_4592 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_n = _rtB ->
B_18_574_4592 ; } else { _rtXdot -> u5_CSTATE_n = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_i = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_i +=
_rtP -> P_88 * _rtX -> u0HzLowpassFilter_CSTATE_i ; _rtXdot ->
u0HzLowpassFilter_CSTATE_i += _rtB -> B_18_219_1752 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_p > _rtP -> P_92 ) && ( _rtX -> Limits55_CSTATE_p < _rtP ->
P_91 ) ) || ( ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_92 ) && ( _rtB ->
B_18_576_4608 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_p >= _rtP -> P_91 ) &&
( _rtB -> B_18_576_4608 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_p = _rtB ->
B_18_576_4608 ; } else { _rtXdot -> Limits55_CSTATE_p = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_p = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_p +=
_rtP -> P_93 * _rtX -> u0HzLowpassFilter_CSTATE_p ; _rtXdot ->
u0HzLowpassFilter_CSTATE_p += _rtB -> B_18_219_1752 [ 2 ] ;
ssCallAccelRunBlock ( S , 18 , 93 , SS_CALL_MDL_FORCINGFUNCTION ) ; }
#define MDL_MASSMATRIX
static void mdlMassMatrix ( SimStruct * S ) { ssCallAccelRunBlock ( S , 18 ,
1 , SS_CALL_MDL_MASSMATRIX ) ; ssCallAccelRunBlock ( S , 18 , 43 ,
SS_CALL_MDL_MASSMATRIX ) ; ssCallAccelRunBlock ( S , 18 , 68 ,
SS_CALL_MDL_MASSMATRIX ) ; ssCallAccelRunBlock ( S , 18 , 93 ,
SS_CALL_MDL_MASSMATRIX ) ; } static void mdlInitializeSizes ( SimStruct * S )
{ ssSetChecksumVal ( S , 0 , 914280408U ) ; ssSetChecksumVal ( S , 1 ,
1546892604U ) ; ssSetChecksumVal ( S , 2 , 40044263U ) ; ssSetChecksumVal ( S
, 3 , 3819180283U ) ; { mxArray * slVerStructMat = ( NULL ) ; mxArray *
slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ; int status
= mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" ) ; if (
status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 ,
"Version" ) ; if ( slVerMat == ( NULL ) ) { status = 1 ; } else { status =
mxGetString ( slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "23.2" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork ( S ) != ( SLSize )
sizeof ( DW_sm_aileron_actuator_T ) ) { static char msg [ 256 ] ; snprintf (
msg , 256 , "Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofDWork ( S ) , ( unsigned long ) sizeof ( DW_sm_aileron_actuator_T
) ) ; ssSetErrorStatus ( S , msg ) ; } if ( ssGetSizeofGlobalBlockIO ( S ) !=
( SLSize ) sizeof ( B_sm_aileron_actuator_T ) ) { static char msg [ 256 ] ;
snprintf ( msg , 256 , "Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofGlobalBlockIO ( S ) , ( unsigned long ) sizeof (
B_sm_aileron_actuator_T ) ) ; ssSetErrorStatus ( S , msg ) ; } { int
ssSizeofParams ; ssGetSizeofParams ( S , & ssSizeofParams ) ; if (
ssSizeofParams != sizeof ( P_sm_aileron_actuator_T ) ) { static char msg [
256 ] ; snprintf ( msg , 256 ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file (%d vs %lu)." , ssSizeofParams , (
unsigned long ) sizeof ( P_sm_aileron_actuator_T ) ) ; ssSetErrorStatus ( S ,
msg ) ; } } _ssSetModelRtp ( S , ( real_T * ) &
sm_aileron_actuator_rtDefaultP ) ; _ssSetConstBlockIO ( S , &
sm_aileron_actuator_rtInvariant ) ; if ( ssGetSizeofDWork ( S ) == ( SLSize )
sizeof ( DW_sm_aileron_actuator_T ) ) { { ( ( DW_sm_aileron_actuator_T * )
ssGetRootDWork ( S ) ) -> STATE_4_MASS_MATRIX_PR = 31 ; ( (
DW_sm_aileron_actuator_T * ) ssGetRootDWork ( S ) ) -> STATE_3_MASS_MATRIX_PR
= 41 ; ( ( DW_sm_aileron_actuator_T * ) ssGetRootDWork ( S ) ) ->
STATE_2_MASS_MATRIX_PR = 52 ; } } rt_InitInfAndNaN ( sizeof ( real_T ) ) ; (
( P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_108 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_109 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_110 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_111 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_115 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_116 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_117 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_118 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_145 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_146 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_147 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_148 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_152 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_153 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_154 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_155 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_161 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_162 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_163 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_164 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_168 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_169 = rtMinusInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_170 = rtInf ; ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) -> P_171 = rtMinusInf ; }
static void mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS
; SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ;
callSysFcns = ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ]
= ( SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 1 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 2 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 3 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 4 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 5 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 6 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 7 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 8 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 9 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 10 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 11 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; }
static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
