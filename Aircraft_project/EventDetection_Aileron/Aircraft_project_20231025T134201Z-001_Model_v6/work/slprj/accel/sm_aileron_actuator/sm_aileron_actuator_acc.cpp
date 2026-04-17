#include "sm_aileron_actuator_acc.h"
#include <stddef.h>
#include <float.h>
#include "mwmathutil.h"
#include "rtwtypes.h"
#include "sm_aileron_actuator_acc_types.h"
#include "sm_aileron_actuator_acc_private.h"
#include <string.h>
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
yL_0d0 ; } void rt_ssGetBlockPath ( void * S , int_T sysIdx , int_T blkIdx ,
char_T * * path ) { _ssGetBlockPath ( ( SimStruct * ) S , sysIdx , blkIdx ,
path ) ; } void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct *
castedS = ( SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( castedS ) )
{ _ssSet_slErrMsg ( castedS , diag ) ; } else { _ssDiscardDiagnostic (
castedS , diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void
* diag ) { _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } void
sm_aileron_actuator_findnode_Init ( SimStruct * S ,
B_findnode_sm_aileron_actuator_T * localB , DW_findnode_sm_aileron_actuator_T
* localDW , P_findnode_sm_aileron_actuator_T * localP ) { localDW ->
Nodevariablem_DSTATE = localP -> P_8 ; localB -> B_6_1772_13344 = localP ->
P_4 ; localB -> B_7_1771_13343 = localP -> P_6 ; } void
sm_aileron_actuator_findnode_Reset ( SimStruct * S ,
DW_findnode_sm_aileron_actuator_T * localDW ,
P_findnode_sm_aileron_actuator_T * localP ) { localDW -> Nodevariablem_DSTATE
= localP -> P_8 ; } void sm_aileron_actuator_findnode ( SimStruct * S , const
real_T rtu_X [ 5 ] , B_findnode_sm_aileron_actuator_T * localB , const
ConstB_findnode_sm_aileron_actuator_T * localC ,
DW_findnode_sm_aileron_actuator_T * localDW ,
P_findnode_sm_aileron_actuator_T * localP ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
sm_aileron_actuator_findnode_Reset ( S , localDW , localP ) ; do { localB ->
B_8_1716_13288 = localDW -> Nodevariablem_DSTATE ; localB -> B_8_1717_13289 =
localB -> B_8_1720_13292 [ localB -> B_8_1716_13288 - 1 ] ; if ( localB ->
B_8_1717_13289 > localP -> P_9 ) { localB -> B_8_1718_13290 = localB ->
B_8_1717_13289 ; } else { localB -> B_8_1718_13290 = localB -> B_8_1719_13291
; } localB -> B_8_1662_12856 = rtu_X [ localB -> B_8_1718_13290 - 1 ] ;
localB -> B_8_1877_13449 = muDoubleScalarIsNaN ( localB -> B_8_1662_12856 ) ;
localB -> B_8_1878_13450 = ! localB -> B_8_1877_13449 ; localB ->
B_8_1879_13451 = localB -> B_8_1885_13457 [ localB -> B_8_1716_13288 - 1 ] ;
localB -> B_8_1880_13452 = ! localB -> B_8_1879_13451 ; if ( localB ->
B_8_1884_13456 ) { localB -> B_3_1937_13509 = ( localB -> B_8_1717_13289 !=
localC -> B_8_1945_13562 ) ; localB -> B_8_1881_13453 = localB ->
B_3_1937_13509 ; } else { localB -> B_8_1881_13453 = localB -> B_8_1883_13455
; } localB -> B_8_1882_13454 = ( localB -> B_8_1878_13450 && localB ->
B_8_1880_13452 && localB -> B_8_1881_13453 ) ; if ( localB -> B_8_1882_13454
) { localB -> B_6_1663_12864 = rtu_X [ localB -> B_8_1717_13289 - 1 ] ;
localB -> B_6_1664_12872 = localB -> B_6_1665_12880 [ localB ->
B_8_1716_13288 - 1 ] ; localB -> B_6_1936_13508 = ( localB -> B_6_1663_12864
< localB -> B_6_1664_12872 ) ; if ( localB -> B_6_1936_13508 ) { localB ->
B_4_1876_13448 = localB -> B_6_1773_13345 [ localB -> B_8_1716_13288 - 1 ] ;
localB -> B_6_1772_13344 = localB -> B_4_1876_13448 ; } else { localB ->
B_5_1875_13447 = localB -> B_6_1773_13345 [ localB -> B_8_1716_13288 + 50 ] ;
localB -> B_6_1772_13344 = localB -> B_5_1875_13447 ; } localDW ->
Updatem_SubsysRanBC = 4 ; } else { localB -> B_7_1771_13343 = localB ->
B_8_1716_13288 ; localDW -> Returnm_SubsysRanBC = 4 ; } localDW ->
Nodevariablem_DSTATE = localB -> B_6_1772_13344 ; } while ( localB ->
B_8_1882_13454 ) ; } void sm_aileron_actuator_findnodeTID2 ( SimStruct * S ,
B_findnode_sm_aileron_actuator_T * localB , P_findnode_sm_aileron_actuator_T
* localP ) { int32_T i ; localB -> B_8_1719_13291 = localP -> P_10 ; localB
-> B_8_1883_13455 = localP -> P_1 ; localB -> B_8_1884_13456 = localP -> P_2
; for ( i = 0 ; i < 51 ; i ++ ) { localB -> B_8_1720_13292 [ i ] = localP ->
P_11 [ i ] ; localB -> B_8_1885_13457 [ i ] = localP -> P_3 [ i ] ; } memcpy
( & localB -> B_6_1773_13345 [ 0 ] , & localP -> P_5 [ 0 ] , 102U * sizeof (
uint8_T ) ) ; memcpy ( & localB -> B_6_1665_12880 [ 0 ] , & localP -> P_0 [ 0
] , 51U * sizeof ( real_T ) ) ; } void sm_aileron_actuator_findnode_Term (
SimStruct * const S ) { UNUSED_PARAMETER ( S ) ; } static void mdlOutputs (
SimStruct * S , int_T tid ) { B_sm_aileron_actuator_T * _rtB ;
DW_sm_aileron_actuator_T * _rtDW ; P_sm_aileron_actuator_T * _rtP ;
X_sm_aileron_actuator_T * _rtX ; real_T tmp [ 8 ] ; real_T B_24_349_2792_0 ;
real_T B_24_427_3416_0 ; real_T B_24_507_4056_0 ; real_T tmp_0 ; real_T tmp_1
; real_T tmp_2 ; real_T tmp_3 ; real_T tmp_4 ; real_T tmp_5 ; real_T tmp_6 ;
real_T tmp_7 ; real_T tmp_8 ; real_T tmp_9 ; real_T tmp_a ; real_T tmp_b ;
real_T tmp_c ; real_T tmp_d ; real_T tmp_e ; real_T tmp_f ; real_T u0 ;
real_T u1 ; real_T u2 ; real_T * lastU ; real_T * tmp_g ; int32_T i ; int32_T
isHit ; uint32_T intVal ; uint32_T u ; uint16_T B_9_1608_12784_0 ; uint16_T
s38_iter ; _rtDW = ( ( DW_sm_aileron_actuator_T * ) ssGetRootDWork ( S ) ) ;
_rtX = ( ( X_sm_aileron_actuator_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ; ssCallAccelRunBlock
( S , 24 , 1 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 24 , 2 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_37_296 = _rtP -> P_33 * _rtB ->
B_24_27_216 [ 6 ] ; _rtB -> B_24_38_304 = look1_binlxpw ( _rtB -> B_24_37_296
, _rtP -> P_35 , _rtP -> P_34 , 10U ) ; ssCallAccelRunBlock ( S , 24 , 5 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_42_336 = look1_binlxpw ( _rtB ->
B_24_39_312 [ 0 ] , _rtP -> P_37 , _rtP -> P_36 , 10U ) ; _rtB -> B_24_43_344
= _rtB -> B_24_39_312 [ 1 ] * _rtB -> B_24_39_312 [ 1 ] ; _rtB -> B_24_44_352
= look1_binlxpw ( _rtB -> B_24_39_312 [ 2 ] , _rtP -> P_39 , _rtP -> P_38 ,
10U ) ; _rtB -> B_24_45_360 = _rtB -> B_24_38_304 * _rtB -> B_24_42_336 *
_rtB -> B_24_43_344 * _rtB -> B_24_44_352 ; _rtB -> B_24_46_368 = _rtP ->
P_40 * _rtB -> B_24_45_360 ; _rtB -> B_24_47_376 = _rtP -> P_41 * _rtB ->
B_24_46_368 ; _rtB -> B_24_48_384 = ssGetT ( S ) ; _rtB -> B_24_50_400 = _rtB
-> B_24_48_384 - _rtB -> B_24_49_392 ; _rtB -> B_24_51_408 =
muDoubleScalarRem ( _rtB -> B_24_50_400 , _rtB -> B_24_1002_8016 ) ; _rtB ->
B_24_52_416 = look1_binlxpw ( _rtB -> B_24_51_408 , _rtP -> P_43 , _rtP ->
P_42 , 26U ) ; _rtB -> B_24_53_424 = _rtB -> B_24_52_416 ; if ( _rtDW ->
INTERNAL_1_1_1_FirstOutput == 0.0 ) { _rtDW -> INTERNAL_1_1_1_FirstOutput =
1.0 ; _rtX -> sm_aileron_actuatoroutputFiltered_1513730075_0 = _rtB ->
B_24_27_216 [ 1 ] ; } _rtB -> B_24_54_432 [ 0 ] = _rtX ->
sm_aileron_actuatoroutputFiltered_1513730075_0 ; _rtB -> B_24_54_432 [ 1 ] =
( _rtB -> B_24_27_216 [ 1 ] - _rtX ->
sm_aileron_actuatoroutputFiltered_1513730075_0 ) * 100.0 ; _rtB ->
B_24_54_432 [ 2 ] = 0.0 ; _rtB -> B_24_54_432 [ 3 ] = 0.0 ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> u5_CSTATE >= _rtP -> P_45 ) {
if ( _rtX -> u5_CSTATE != _rtP -> P_45 ) { _rtX -> u5_CSTATE = _rtP -> P_45 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else if ( ( _rtX ->
u5_CSTATE <= _rtP -> P_46 ) && ( _rtX -> u5_CSTATE != _rtP -> P_46 ) ) { _rtX
-> u5_CSTATE = _rtP -> P_46 ; ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; } _rtB -> B_24_58_464 = _rtX -> u5_CSTATE ; } else if ( _rtX -> u5_CSTATE
>= _rtP -> P_45 ) { _rtB -> B_24_58_464 = _rtP -> P_45 ; } else if ( _rtX ->
u5_CSTATE <= _rtP -> P_46 ) { _rtB -> B_24_58_464 = _rtP -> P_46 ; } else {
_rtB -> B_24_58_464 = _rtX -> u5_CSTATE ; } _rtB -> B_24_59_472 = 0.0 ; _rtB
-> B_24_59_472 += _rtP -> P_48 * _rtX -> u0HzLowpassFilter_CSTATE ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> Limits55_CSTATE >= _rtP -> P_50
) { if ( _rtX -> Limits55_CSTATE != _rtP -> P_50 ) { _rtX -> Limits55_CSTATE
= _rtP -> P_50 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else
if ( ( _rtX -> Limits55_CSTATE <= _rtP -> P_51 ) && ( _rtX -> Limits55_CSTATE
!= _rtP -> P_51 ) ) { _rtX -> Limits55_CSTATE = _rtP -> P_51 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB -> B_24_60_480 =
_rtX -> Limits55_CSTATE ; } else if ( _rtX -> Limits55_CSTATE >= _rtP -> P_50
) { _rtB -> B_24_60_480 = _rtP -> P_50 ; } else if ( _rtX -> Limits55_CSTATE
<= _rtP -> P_51 ) { _rtB -> B_24_60_480 = _rtP -> P_51 ; } else { _rtB ->
B_24_60_480 = _rtX -> Limits55_CSTATE ; } _rtB -> B_24_61_488 = 0.0 ; _rtB ->
B_24_61_488 += _rtP -> P_53 * _rtX -> u0HzLowpassFilter_CSTATE_e ; _rtB ->
B_24_62_496 = _rtB -> B_24_53_424 - _rtB -> B_24_37_296 ; _rtB -> B_24_63_504
= _rtB -> B_24_62_496 - _rtB -> B_24_61_488 ; _rtB -> B_24_64_512 = _rtP ->
P_54 * _rtB -> B_24_63_504 ; _rtB -> B_24_65_520 = _rtP -> P_55 * _rtB ->
B_24_64_512 ; _rtB -> B_24_66_528 = _rtB -> B_24_60_480 + _rtB -> B_24_65_520
; u0 = _rtB -> B_24_66_528 ; u1 = _rtP -> P_57 ; u2 = _rtP -> P_56 ; if ( u0
> u2 ) { _rtB -> B_24_67_536 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_24_67_536 = u1 ; } else { _rtB -> B_24_67_536 = u0 ; } _rtB -> B_24_68_544
= _rtB -> B_24_67_536 - _rtB -> B_24_59_472 ; _rtB -> B_24_69_552 = _rtP ->
P_58 * _rtB -> B_24_68_544 ; _rtB -> B_24_70_560 = _rtB -> B_24_58_464 + _rtB
-> B_24_69_552 ; _rtB -> B_24_71_568 = muDoubleScalarAbs ( _rtB ->
B_24_70_560 ) ; u0 = _rtB -> B_24_71_568 ; u1 = _rtP -> P_60 ; u2 = _rtP ->
P_59 ; if ( u0 > u2 ) { _rtB -> B_24_72_576 = u2 ; } else if ( u0 < u1 ) {
_rtB -> B_24_72_576 = u1 ; } else { _rtB -> B_24_72_576 = u0 ; } _rtB ->
B_24_73_584 [ 0 ] = _rtB -> B_24_72_576 ; _rtB -> B_24_73_584 [ 1 ] = 0.0 ;
_rtB -> B_24_73_584 [ 2 ] = 0.0 ; _rtB -> B_24_73_584 [ 3 ] = 0.0 ; _rtB ->
B_24_77_616 = ( _rtB -> B_24_70_560 < 0.0 ) ; _rtB -> B_24_78_624 = _rtP ->
P_61 * _rtB -> B_24_77_616 ; _rtB -> B_24_79_632 [ 0 ] = _rtB -> B_24_78_624
; _rtB -> B_24_79_632 [ 1 ] = 0.0 ; _rtB -> B_24_79_632 [ 2 ] = 0.0 ; _rtB ->
B_24_79_632 [ 3 ] = 0.0 ; _rtB -> B_24_83_664 = ssGetT ( S ) ;
ssCallAccelRunBlock ( S , 13 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_84_672 [ 0 ] = _rtB -> B_13_1061_8488 ; _rtB -> B_24_84_672 [ 1 ] = 0.0
; _rtB -> B_24_84_672 [ 2 ] = 0.0 ; _rtB -> B_24_84_672 [ 3 ] = 0.0 ;
ssCallAccelRunBlock ( S , 24 , 43 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 44 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_108_864 [ 0 ] = _rtB -> B_24_101_808 [ 0 ] ; _rtB -> B_24_108_864 [ 1 ]
= 0.0 ; _rtB -> B_24_108_864 [ 2 ] = 0.0 ; _rtB -> B_24_108_864 [ 3 ] = 0.0 ;
if ( _rtDW -> INTERNAL_3_1_1_FirstOutput == 0.0 ) { _rtDW ->
INTERNAL_3_1_1_FirstOutput = 1.0 ; _rtX ->
sm_aileron_actuatoroutputFiltered_553247611_0 = _rtB -> B_24_27_216 [ 3 ] ; }
_rtB -> B_24_112_896 [ 0 ] = _rtX ->
sm_aileron_actuatoroutputFiltered_553247611_0 ; _rtB -> B_24_112_896 [ 1 ] =
( _rtB -> B_24_27_216 [ 3 ] - _rtX ->
sm_aileron_actuatoroutputFiltered_553247611_0 ) * 100.0 ; _rtB ->
B_24_112_896 [ 2 ] = 0.0 ; _rtB -> B_24_112_896 [ 3 ] = 0.0 ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> u5_CSTATE_f >= _rtP -> P_63 ) {
if ( _rtX -> u5_CSTATE_f != _rtP -> P_63 ) { _rtX -> u5_CSTATE_f = _rtP ->
P_63 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else if ( (
_rtX -> u5_CSTATE_f <= _rtP -> P_64 ) && ( _rtX -> u5_CSTATE_f != _rtP ->
P_64 ) ) { _rtX -> u5_CSTATE_f = _rtP -> P_64 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB -> B_24_116_928 =
_rtX -> u5_CSTATE_f ; } else if ( _rtX -> u5_CSTATE_f >= _rtP -> P_63 ) {
_rtB -> B_24_116_928 = _rtP -> P_63 ; } else if ( _rtX -> u5_CSTATE_f <= _rtP
-> P_64 ) { _rtB -> B_24_116_928 = _rtP -> P_64 ; } else { _rtB ->
B_24_116_928 = _rtX -> u5_CSTATE_f ; } _rtB -> B_24_117_936 = 0.0 ; _rtB ->
B_24_117_936 += _rtP -> P_66 * _rtX -> u0HzLowpassFilter_CSTATE_d ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> Limits55_CSTATE_g >= _rtP ->
P_68 ) { if ( _rtX -> Limits55_CSTATE_g != _rtP -> P_68 ) { _rtX ->
Limits55_CSTATE_g = _rtP -> P_68 ; ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } } else if ( ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_69 ) && ( _rtX
-> Limits55_CSTATE_g != _rtP -> P_69 ) ) { _rtX -> Limits55_CSTATE_g = _rtP
-> P_69 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB ->
B_24_118_944 = _rtX -> Limits55_CSTATE_g ; } else if ( _rtX ->
Limits55_CSTATE_g >= _rtP -> P_68 ) { _rtB -> B_24_118_944 = _rtP -> P_68 ; }
else if ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_69 ) { _rtB -> B_24_118_944
= _rtP -> P_69 ; } else { _rtB -> B_24_118_944 = _rtX -> Limits55_CSTATE_g ;
} _rtB -> B_24_119_952 = 0.0 ; _rtB -> B_24_119_952 += _rtP -> P_71 * _rtX ->
u0HzLowpassFilter_CSTATE_b ; _rtB -> B_24_120_960 = _rtB -> B_24_62_496 -
_rtB -> B_24_119_952 ; _rtB -> B_24_121_968 = _rtP -> P_72 * _rtB ->
B_24_120_960 ; _rtB -> B_24_122_976 = _rtP -> P_73 * _rtB -> B_24_121_968 ;
_rtB -> B_24_123_984 = _rtB -> B_24_118_944 + _rtB -> B_24_122_976 ; u0 =
_rtB -> B_24_123_984 ; u1 = _rtP -> P_75 ; u2 = _rtP -> P_74 ; if ( u0 > u2 )
{ _rtB -> B_24_124_992 = u2 ; } else if ( u0 < u1 ) { _rtB -> B_24_124_992 =
u1 ; } else { _rtB -> B_24_124_992 = u0 ; } _rtB -> B_24_125_1000 = _rtB ->
B_24_124_992 - _rtB -> B_24_117_936 ; _rtB -> B_24_126_1008 = _rtP -> P_76 *
_rtB -> B_24_125_1000 ; _rtB -> B_24_127_1016 = _rtB -> B_24_116_928 + _rtB
-> B_24_126_1008 ; _rtB -> B_24_128_1024 = muDoubleScalarAbs ( _rtB ->
B_24_127_1016 ) ; u0 = _rtB -> B_24_128_1024 ; u1 = _rtP -> P_78 ; u2 = _rtP
-> P_77 ; if ( u0 > u2 ) { _rtB -> B_24_129_1032 = u2 ; } else if ( u0 < u1 )
{ _rtB -> B_24_129_1032 = u1 ; } else { _rtB -> B_24_129_1032 = u0 ; } _rtB
-> B_24_130_1040 [ 0 ] = _rtB -> B_24_129_1032 ; _rtB -> B_24_130_1040 [ 1 ]
= 0.0 ; _rtB -> B_24_130_1040 [ 2 ] = 0.0 ; _rtB -> B_24_130_1040 [ 3 ] = 0.0
; _rtB -> B_24_134_1072 = ( _rtB -> B_24_127_1016 < 0.0 ) ; _rtB ->
B_24_135_1080 = _rtP -> P_79 * _rtB -> B_24_134_1072 ; _rtB -> B_24_136_1088
[ 0 ] = _rtB -> B_24_135_1080 ; _rtB -> B_24_136_1088 [ 1 ] = 0.0 ; _rtB ->
B_24_136_1088 [ 2 ] = 0.0 ; _rtB -> B_24_136_1088 [ 3 ] = 0.0 ; _rtB ->
B_24_140_1120 = ssGetT ( S ) ; ssCallAccelRunBlock ( S , 14 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_141_1128 [ 0 ] = _rtB -> B_14_1060_8480
; _rtB -> B_24_141_1128 [ 1 ] = 0.0 ; _rtB -> B_24_141_1128 [ 2 ] = 0.0 ;
_rtB -> B_24_141_1128 [ 3 ] = 0.0 ; ssCallAccelRunBlock ( S , 24 , 68 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 24 , 69 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_165_1320 [ 0 ] = _rtB -> B_24_158_1264 [
0 ] ; _rtB -> B_24_165_1320 [ 1 ] = 0.0 ; _rtB -> B_24_165_1320 [ 2 ] = 0.0 ;
_rtB -> B_24_165_1320 [ 3 ] = 0.0 ; if ( _rtDW -> INTERNAL_5_1_1_FirstOutput
== 0.0 ) { _rtDW -> INTERNAL_5_1_1_FirstOutput = 1.0 ; _rtX ->
sm_aileron_actuatoroutputFiltered_2948142811_0 = _rtB -> B_24_27_216 [ 5 ] ;
} _rtB -> B_24_169_1352 [ 0 ] = _rtX ->
sm_aileron_actuatoroutputFiltered_2948142811_0 ; _rtB -> B_24_169_1352 [ 1 ]
= ( _rtB -> B_24_27_216 [ 5 ] - _rtX ->
sm_aileron_actuatoroutputFiltered_2948142811_0 ) * 100.0 ; _rtB ->
B_24_169_1352 [ 2 ] = 0.0 ; _rtB -> B_24_169_1352 [ 3 ] = 0.0 ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> u5_CSTATE_n >= _rtP -> P_81 ) {
if ( _rtX -> u5_CSTATE_n != _rtP -> P_81 ) { _rtX -> u5_CSTATE_n = _rtP ->
P_81 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } else if ( (
_rtX -> u5_CSTATE_n <= _rtP -> P_82 ) && ( _rtX -> u5_CSTATE_n != _rtP ->
P_82 ) ) { _rtX -> u5_CSTATE_n = _rtP -> P_82 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB -> B_24_173_1384 =
_rtX -> u5_CSTATE_n ; } else if ( _rtX -> u5_CSTATE_n >= _rtP -> P_81 ) {
_rtB -> B_24_173_1384 = _rtP -> P_81 ; } else if ( _rtX -> u5_CSTATE_n <=
_rtP -> P_82 ) { _rtB -> B_24_173_1384 = _rtP -> P_82 ; } else { _rtB ->
B_24_173_1384 = _rtX -> u5_CSTATE_n ; } _rtB -> B_24_174_1392 = 0.0 ; _rtB ->
B_24_174_1392 += _rtP -> P_84 * _rtX -> u0HzLowpassFilter_CSTATE_i ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( _rtX -> Limits55_CSTATE_p >= _rtP ->
P_86 ) { if ( _rtX -> Limits55_CSTATE_p != _rtP -> P_86 ) { _rtX ->
Limits55_CSTATE_p = _rtP -> P_86 ; ssSetBlockStateForSolverChangedAtMajorStep
( S ) ; } } else if ( ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_87 ) && ( _rtX
-> Limits55_CSTATE_p != _rtP -> P_87 ) ) { _rtX -> Limits55_CSTATE_p = _rtP
-> P_87 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtB ->
B_24_175_1400 = _rtX -> Limits55_CSTATE_p ; } else if ( _rtX ->
Limits55_CSTATE_p >= _rtP -> P_86 ) { _rtB -> B_24_175_1400 = _rtP -> P_86 ;
} else if ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_87 ) { _rtB ->
B_24_175_1400 = _rtP -> P_87 ; } else { _rtB -> B_24_175_1400 = _rtX ->
Limits55_CSTATE_p ; } _rtB -> B_24_176_1408 = 0.0 ; _rtB -> B_24_176_1408 +=
_rtP -> P_89 * _rtX -> u0HzLowpassFilter_CSTATE_p ; _rtB -> B_24_177_1416 =
_rtB -> B_24_62_496 - _rtB -> B_24_176_1408 ; _rtB -> B_24_178_1424 = _rtP ->
P_90 * _rtB -> B_24_177_1416 ; _rtB -> B_24_179_1432 = _rtP -> P_91 * _rtB ->
B_24_178_1424 ; _rtB -> B_24_180_1440 = _rtB -> B_24_175_1400 + _rtB ->
B_24_179_1432 ; u0 = _rtB -> B_24_180_1440 ; u1 = _rtP -> P_93 ; u2 = _rtP ->
P_92 ; if ( u0 > u2 ) { _rtB -> B_24_181_1448 = u2 ; } else if ( u0 < u1 ) {
_rtB -> B_24_181_1448 = u1 ; } else { _rtB -> B_24_181_1448 = u0 ; } _rtB ->
B_24_182_1456 = _rtB -> B_24_181_1448 - _rtB -> B_24_174_1392 ; _rtB ->
B_24_183_1464 = _rtP -> P_94 * _rtB -> B_24_182_1456 ; _rtB -> B_24_184_1472
= _rtB -> B_24_173_1384 + _rtB -> B_24_183_1464 ; _rtB -> B_24_185_1480 =
muDoubleScalarAbs ( _rtB -> B_24_184_1472 ) ; u0 = _rtB -> B_24_185_1480 ; u1
= _rtP -> P_96 ; u2 = _rtP -> P_95 ; if ( u0 > u2 ) { _rtB -> B_24_186_1488 =
u2 ; } else if ( u0 < u1 ) { _rtB -> B_24_186_1488 = u1 ; } else { _rtB ->
B_24_186_1488 = u0 ; } _rtB -> B_24_187_1496 [ 0 ] = _rtB -> B_24_186_1488 ;
_rtB -> B_24_187_1496 [ 1 ] = 0.0 ; _rtB -> B_24_187_1496 [ 2 ] = 0.0 ; _rtB
-> B_24_187_1496 [ 3 ] = 0.0 ; _rtB -> B_24_191_1528 = ( _rtB ->
B_24_184_1472 < 0.0 ) ; _rtB -> B_24_192_1536 = _rtP -> P_97 * _rtB ->
B_24_191_1528 ; _rtB -> B_24_193_1544 [ 0 ] = _rtB -> B_24_192_1536 ; _rtB ->
B_24_193_1544 [ 1 ] = 0.0 ; _rtB -> B_24_193_1544 [ 2 ] = 0.0 ; _rtB ->
B_24_193_1544 [ 3 ] = 0.0 ; _rtB -> B_24_197_1576 = ssGetT ( S ) ;
ssCallAccelRunBlock ( S , 15 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_198_1584 [ 0 ] = _rtB -> B_15_1059_8472 ; _rtB -> B_24_198_1584 [ 1 ] =
0.0 ; _rtB -> B_24_198_1584 [ 2 ] = 0.0 ; _rtB -> B_24_198_1584 [ 3 ] = 0.0 ;
ssCallAccelRunBlock ( S , 24 , 93 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 94 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_221_1768 [ 0 ] = _rtB -> B_24_214_1712 [ 0 ] ; _rtB -> B_24_221_1768 [ 1
] = 0.0 ; _rtB -> B_24_221_1768 [ 2 ] = 0.0 ; _rtB -> B_24_221_1768 [ 3 ] =
0.0 ; _rtB -> B_24_225_1800 [ 0 ] = _rtB -> B_24_47_376 ; _rtB ->
B_24_225_1800 [ 1 ] = 0.0 ; _rtB -> B_24_225_1800 [ 2 ] = 0.0 ; _rtB ->
B_24_225_1800 [ 3 ] = 0.0 ; ssCallAccelRunBlock ( S , 24 , 97 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { _rtB -> B_24_234_1872 = _rtP -> P_98 ; _rtB -> B_24_235_1880 = _rtB ->
B_24_234_1872 - _rtB -> B_24_232_1856 [ 0 ] ; _rtB -> B_24_1626_12820 = (
_rtB -> B_24_235_1880 <= sm_aileron_actuator_rtC ( S ) -> B_24_1938_13512 ) ;
_rtB -> B_24_1627_12821 = ( _rtB -> B_24_1626_12820 && _rtB ->
B_24_1648_12842 ) ; } _rtB -> B_24_236_1888 = _rtP -> P_99 * _rtB ->
B_24_229_1832 [ 0 ] ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtDW -> Integrator_IC_LOADING != 0 ) { _rtDW -> Integrator_DSTATE =
_rtB -> B_24_236_1888 ; if ( _rtDW -> Integrator_DSTATE > _rtP -> P_101 ) {
_rtDW -> Integrator_DSTATE = _rtP -> P_101 ; } else if ( _rtDW ->
Integrator_DSTATE < _rtP -> P_102 ) { _rtDW -> Integrator_DSTATE = _rtP ->
P_102 ; } } if ( _rtB -> B_24_1627_12821 || ( _rtDW ->
Integrator_PrevResetState != 0 ) ) { _rtDW -> Integrator_DSTATE = _rtB ->
B_24_236_1888 ; if ( _rtDW -> Integrator_DSTATE > _rtP -> P_101 ) { _rtDW ->
Integrator_DSTATE = _rtP -> P_101 ; } else if ( _rtDW -> Integrator_DSTATE <
_rtP -> P_102 ) { _rtDW -> Integrator_DSTATE = _rtP -> P_102 ; } } _rtB ->
B_24_237_1896 = _rtDW -> Integrator_DSTATE ; u0 = _rtB -> B_24_237_1896 ; u1
= _rtP -> P_104 ; u2 = _rtP -> P_103 ; if ( u0 > u2 ) { _rtB -> B_24_238_1904
= u2 ; } else if ( u0 < u1 ) { _rtB -> B_24_238_1904 = u1 ; } else { _rtB ->
B_24_238_1904 = u0 ; } ssCallAccelRunBlock ( S , 16 , 0 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> B_24_239_1912 = _rtB -> B_24_37_296 * _rtP -> P_105 ; _rtB ->
B_24_240_1920 = _rtDW -> UD_DSTATE ; _rtB -> B_24_241_1928 = _rtB ->
B_24_239_1912 - _rtB -> B_24_240_1920 ; _rtB -> B_24_242_1936 = _rtB ->
B_24_53_424 * _rtP -> P_107 ; _rtB -> B_24_243_1944 = _rtDW -> UD_DSTATE_l ;
_rtB -> B_24_244_1952 = _rtB -> B_24_242_1936 - _rtB -> B_24_243_1944 ; _rtB
-> B_24_247_1976 = _rtP -> P_109 ; _rtB -> B_24_248_1984 = _rtB ->
B_24_247_1976 - _rtB -> B_24_245_1960 [ 0 ] ; _rtB -> B_24_1628_12822 = (
_rtB -> B_24_248_1984 <= sm_aileron_actuator_rtC ( S ) -> B_24_1941_13536 ) ;
_rtB -> B_24_1629_12823 = ( _rtB -> B_24_1628_12822 && _rtB ->
B_24_1651_12845 ) ; } _rtB -> B_24_249_1992 = _rtP -> P_110 * _rtB ->
B_24_101_808 [ 6 ] ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtDW -> Integrator_IC_LOADING_p != 0 ) { _rtDW -> Integrator_DSTATE_h
= _rtB -> B_24_249_1992 ; if ( _rtDW -> Integrator_DSTATE_h > _rtP -> P_112 )
{ _rtDW -> Integrator_DSTATE_h = _rtP -> P_112 ; } else if ( _rtDW ->
Integrator_DSTATE_h < _rtP -> P_113 ) { _rtDW -> Integrator_DSTATE_h = _rtP
-> P_113 ; } } if ( _rtB -> B_24_1629_12823 || ( _rtDW ->
Integrator_PrevResetState_m != 0 ) ) { _rtDW -> Integrator_DSTATE_h = _rtB ->
B_24_249_1992 ; if ( _rtDW -> Integrator_DSTATE_h > _rtP -> P_112 ) { _rtDW
-> Integrator_DSTATE_h = _rtP -> P_112 ; } else if ( _rtDW ->
Integrator_DSTATE_h < _rtP -> P_113 ) { _rtDW -> Integrator_DSTATE_h = _rtP
-> P_113 ; } } _rtB -> B_24_250_2000 = _rtDW -> Integrator_DSTATE_h ; u0 =
_rtB -> B_24_250_2000 ; u1 = _rtP -> P_115 ; u2 = _rtP -> P_114 ; if ( u0 >
u2 ) { _rtB -> B_24_251_2008 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_24_251_2008 = u1 ; } else { _rtB -> B_24_251_2008 = u0 ; }
ssCallAccelRunBlock ( S , 19 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_252_2016 = _rtB -> B_24_27_216 [ 0 ] * _rtP -> P_116 ; _rtB ->
B_24_253_2024 = _rtDW -> UD_DSTATE_p ; _rtB -> B_24_254_2032 = _rtB ->
B_24_252_2016 - _rtB -> B_24_253_2024 ; } if ( ( _rtDW -> TimeStampA >=
ssGetT ( S ) ) && ( _rtDW -> TimeStampB >= ssGetT ( S ) ) ) { _rtB ->
B_24_255_2040 = 0.0 ; } else { u0 = _rtDW -> TimeStampA ; lastU = & _rtDW ->
LastUAtTimeA ; if ( _rtDW -> TimeStampA < _rtDW -> TimeStampB ) { if ( _rtDW
-> TimeStampB < ssGetT ( S ) ) { u0 = _rtDW -> TimeStampB ; lastU = & _rtDW
-> LastUAtTimeB ; } } else if ( _rtDW -> TimeStampA >= ssGetT ( S ) ) { u0 =
_rtDW -> TimeStampB ; lastU = & _rtDW -> LastUAtTimeB ; } u0 = ssGetT ( S ) -
u0 ; _rtB -> B_24_255_2040 = ( _rtB -> B_24_254_2032 - * lastU ) / u0 ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock
( S , 0 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_256_2048 = _rtDW ->
Memory_PreviousInput ; } if ( ( _rtDW -> TimeStampA_o >= ssGetT ( S ) ) && (
_rtDW -> TimeStampB_f >= ssGetT ( S ) ) ) { _rtB -> B_24_257_2056 = 0.0 ; }
else { u0 = _rtDW -> TimeStampA_o ; lastU = & _rtDW -> LastUAtTimeA_l ; if (
_rtDW -> TimeStampA_o < _rtDW -> TimeStampB_f ) { if ( _rtDW -> TimeStampB_f
< ssGetT ( S ) ) { u0 = _rtDW -> TimeStampB_f ; lastU = & _rtDW ->
LastUAtTimeB_g ; } } else if ( _rtDW -> TimeStampA_o >= ssGetT ( S ) ) { u0 =
_rtDW -> TimeStampB_f ; lastU = & _rtDW -> LastUAtTimeB_g ; } u0 = ssGetT ( S
) - u0 ; _rtB -> B_24_257_2056 = ( _rtB -> B_19_1055_8440 - * lastU ) / u0 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_24_258_2064 = _rtDW -> Memory1_PreviousInput ; ssCallAccelRunBlock ( S , 11
, 0 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_10_1064_8512 [ 0 ] = _rtB ->
B_24_244_1952 ; _rtB -> B_10_1064_8512 [ 1 ] = _rtB -> B_24_257_2056 ; _rtB
-> B_10_1064_8512 [ 2 ] = _rtB -> B_16_1058_8464 ; _rtB -> B_10_1064_8512 [ 3
] = _rtB -> B_24_258_2064 ; _rtB -> B_10_1064_8512 [ 4 ] = _rtB ->
B_24_256_2048 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
sm_aileron_actuator_findnode ( S , _rtB -> B_10_1064_8512 , & _rtB ->
findnode , & sm_aileron_actuator_rtC ( S ) -> findnode , & _rtDW -> findnode
, & _rtP -> findnode ) ; isHit = _rtB -> findnode . B_7_1771_13343 ; for ( i
= 0 ; i < 8 ; i ++ ) { u0 = _rtB -> B_10_1111_8888 [ ( 51 * i + isHit ) - 1 ]
; _rtB -> B_10_1069_8552 [ i ] = u0 ; _rtB -> B_10_1077_8616 [ i ] = u0 ;
_rtB -> B_10_1085_8680 [ i ] = u0 ; _rtB -> B_10_1093_8744 [ i ] = u0 ; tmp [
i ] = u0 ; } tmp_g = & _rtB -> B_10_1519_12152 [ 0 ] ; for ( isHit = 0 ;
isHit < 8 ; isHit ++ ) { u0 = 0.0 ; for ( i = 0 ; i < 8 ; i ++ ) { u0 +=
tmp_g [ ( isHit << 3 ) + i ] * tmp [ i ] ; } _rtB -> B_10_1101_8808 [ isHit ]
= u0 ; } u0 = _rtB -> B_10_1101_8808 [ 0 ] ; for ( isHit = 0 ; isHit < 7 ;
isHit ++ ) { u0 = muDoubleScalarMin ( u0 , _rtB -> B_10_1101_8808 [ isHit + 1
] ) ; } _rtB -> B_10_1109_8872 = u0 ; for ( i = 0 ; i < 8 ; i ++ ) { _rtB ->
B_10_1654_12848 [ i ] = ( _rtB -> B_10_1109_8872 == _rtB -> B_10_1101_8808 [
i ] ) ; } for ( s38_iter = 1U ; s38_iter <= _rtP -> P_252 ; s38_iter =
static_cast < uint16_T > ( s38_iter + 1 ) ) { _rtB -> B_9_1607_12782 =
s38_iter ; _rtB -> B_9_1616_12800 = static_cast < uint16_T > ( (
sm_aileron_actuator_rtC ( S ) -> B_9_1944_13560 + _rtB -> B_9_1625_12818 ) -
_rtB -> B_9_1607_12782 ) ; for ( i = 0 ; i < 8 ; i ++ ) { u0 =
muDoubleScalarFloor ( _rtB -> B_10_1101_8808 [ i ] ) ; if (
muDoubleScalarIsNaN ( u0 ) || muDoubleScalarIsInf ( u0 ) ) { u0 = 0.0 ; }
else { u0 = muDoubleScalarRem ( u0 , 65536.0 ) ; } if ( u0 < 0.0 ) {
B_9_1608_12784_0 = static_cast < uint16_T > ( - static_cast < int16_T > (
static_cast < uint16_T > ( - u0 ) ) ) ; } else { B_9_1608_12784_0 =
static_cast < uint16_T > ( u0 ) ; } _rtB -> B_9_1608_12784 [ i ] =
B_9_1608_12784_0 ; if ( s38_iter == 1 ) { _rtB -> B_9_1617_12802 [ i ] =
B_9_1608_12784_0 ; } } _rtB -> B_9_1617_12802 [ _rtB -> B_9_1607_12782 - 1 ]
= _rtB -> B_9_1616_12800 ; } for ( i = 0 ; i < 8 ; i ++ ) { if ( _rtB ->
B_10_1654_12848 [ i ] ) { _rtB -> B_10_1596_12760 [ i ] = _rtB ->
B_9_1617_12802 [ i ] ; } else { _rtB -> B_10_1596_12760 [ i ] = 0U ; } }
s38_iter = _rtB -> B_10_1596_12760 [ 0 ] ; for ( isHit = 0 ; isHit < 7 ;
isHit ++ ) { B_9_1608_12784_0 = _rtB -> B_10_1596_12760 [ isHit + 1 ] ;
s38_iter = muIntScalarMax_uint16 ( s38_iter , B_9_1608_12784_0 ) ; } _rtB ->
B_10_1604_12776 = s38_iter ; _rtB -> B_10_1605_12778 = static_cast < uint16_T
> ( _rtB -> B_10_1606_12780 - _rtB -> B_10_1604_12776 ) ; _rtB ->
B_10_1110_8880 = _rtB -> B_10_1583_12664 [ _rtB -> B_10_1605_12778 - 1 ] ;
ssCallAccelRunBlock ( S , 12 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_261_2088 = _rtP -> P_120 ; _rtB -> B_24_262_2096 = _rtB -> B_24_261_2088
- _rtB -> B_24_259_2072 [ 0 ] ; _rtB -> B_24_1630_12824 = ( _rtB ->
B_24_262_2096 <= sm_aileron_actuator_rtC ( S ) -> B_24_1939_13520 ) ; _rtB ->
B_24_1631_12825 = ( _rtB -> B_24_1630_12824 && _rtB -> B_24_1649_12843 ) ; }
_rtB -> B_24_263_2104 = _rtP -> P_121 * _rtB -> B_24_229_1832 [ 1 ] ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_a != 0 ) { _rtDW -> Integrator_DSTATE_l = _rtB ->
B_24_263_2104 ; if ( _rtDW -> Integrator_DSTATE_l > _rtP -> P_123 ) { _rtDW
-> Integrator_DSTATE_l = _rtP -> P_123 ; } else if ( _rtDW ->
Integrator_DSTATE_l < _rtP -> P_124 ) { _rtDW -> Integrator_DSTATE_l = _rtP
-> P_124 ; } } if ( _rtB -> B_24_1631_12825 || ( _rtDW ->
Integrator_PrevResetState_f != 0 ) ) { _rtDW -> Integrator_DSTATE_l = _rtB ->
B_24_263_2104 ; if ( _rtDW -> Integrator_DSTATE_l > _rtP -> P_123 ) { _rtDW
-> Integrator_DSTATE_l = _rtP -> P_123 ; } else if ( _rtDW ->
Integrator_DSTATE_l < _rtP -> P_124 ) { _rtDW -> Integrator_DSTATE_l = _rtP
-> P_124 ; } } _rtB -> B_24_264_2112 = _rtDW -> Integrator_DSTATE_l ; u0 =
_rtB -> B_24_264_2112 ; u1 = _rtP -> P_126 ; u2 = _rtP -> P_125 ; if ( u0 >
u2 ) { _rtB -> B_24_265_2120 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_24_265_2120 = u1 ; } else { _rtB -> B_24_265_2120 = u0 ; }
ssCallAccelRunBlock ( S , 17 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_268_2144 = _rtP -> P_127 ; _rtB -> B_24_269_2152 = _rtB -> B_24_268_2144
- _rtB -> B_24_266_2128 [ 0 ] ; _rtB -> B_24_1632_12826 = ( _rtB ->
B_24_269_2152 <= sm_aileron_actuator_rtC ( S ) -> B_24_1942_13544 ) ; _rtB ->
B_24_1633_12827 = ( _rtB -> B_24_1632_12826 && _rtB -> B_24_1652_12846 ) ; }
_rtB -> B_24_270_2160 = _rtP -> P_128 * _rtB -> B_24_158_1264 [ 6 ] ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_m != 0 ) { _rtDW -> Integrator_DSTATE_e = _rtB ->
B_24_270_2160 ; if ( _rtDW -> Integrator_DSTATE_e > _rtP -> P_130 ) { _rtDW
-> Integrator_DSTATE_e = _rtP -> P_130 ; } else if ( _rtDW ->
Integrator_DSTATE_e < _rtP -> P_131 ) { _rtDW -> Integrator_DSTATE_e = _rtP
-> P_131 ; } } if ( _rtB -> B_24_1633_12827 || ( _rtDW ->
Integrator_PrevResetState_ms != 0 ) ) { _rtDW -> Integrator_DSTATE_e = _rtB
-> B_24_270_2160 ; if ( _rtDW -> Integrator_DSTATE_e > _rtP -> P_130 ) {
_rtDW -> Integrator_DSTATE_e = _rtP -> P_130 ; } else if ( _rtDW ->
Integrator_DSTATE_e < _rtP -> P_131 ) { _rtDW -> Integrator_DSTATE_e = _rtP
-> P_131 ; } } _rtB -> B_24_271_2168 = _rtDW -> Integrator_DSTATE_e ; u0 =
_rtB -> B_24_271_2168 ; u1 = _rtP -> P_133 ; u2 = _rtP -> P_132 ; if ( u0 >
u2 ) { _rtB -> B_24_272_2176 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_24_272_2176 = u1 ; } else { _rtB -> B_24_272_2176 = u0 ; }
ssCallAccelRunBlock ( S , 20 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_273_2184 = _rtB -> B_24_27_216 [ 2 ] * _rtP -> P_134 ; _rtB ->
B_24_274_2192 = _rtDW -> UD_DSTATE_h ; _rtB -> B_24_275_2200 = _rtB ->
B_24_273_2184 - _rtB -> B_24_274_2192 ; ssCallAccelRunBlock ( S , 1 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_278_2224 = _rtP -> P_136 ; _rtB ->
B_24_279_2232 = _rtB -> B_24_278_2224 - _rtB -> B_24_276_2208 [ 0 ] ; _rtB ->
B_24_1634_12828 = ( _rtB -> B_24_279_2232 <= sm_aileron_actuator_rtC ( S ) ->
B_24_1940_13528 ) ; _rtB -> B_24_1635_12829 = ( _rtB -> B_24_1634_12828 &&
_rtB -> B_24_1650_12844 ) ; } _rtB -> B_24_280_2240 = _rtP -> P_137 * _rtB ->
B_24_229_1832 [ 2 ] ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ if ( _rtDW -> Integrator_IC_LOADING_j != 0 ) { _rtDW -> Integrator_DSTATE_f
= _rtB -> B_24_280_2240 ; if ( _rtDW -> Integrator_DSTATE_f > _rtP -> P_139 )
{ _rtDW -> Integrator_DSTATE_f = _rtP -> P_139 ; } else if ( _rtDW ->
Integrator_DSTATE_f < _rtP -> P_140 ) { _rtDW -> Integrator_DSTATE_f = _rtP
-> P_140 ; } } if ( _rtB -> B_24_1635_12829 || ( _rtDW ->
Integrator_PrevResetState_p != 0 ) ) { _rtDW -> Integrator_DSTATE_f = _rtB ->
B_24_280_2240 ; if ( _rtDW -> Integrator_DSTATE_f > _rtP -> P_139 ) { _rtDW
-> Integrator_DSTATE_f = _rtP -> P_139 ; } else if ( _rtDW ->
Integrator_DSTATE_f < _rtP -> P_140 ) { _rtDW -> Integrator_DSTATE_f = _rtP
-> P_140 ; } } _rtB -> B_24_281_2248 = _rtDW -> Integrator_DSTATE_f ; u0 =
_rtB -> B_24_281_2248 ; u1 = _rtP -> P_142 ; u2 = _rtP -> P_141 ; if ( u0 >
u2 ) { _rtB -> B_24_282_2256 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_24_282_2256 = u1 ; } else { _rtB -> B_24_282_2256 = u0 ; }
ssCallAccelRunBlock ( S , 18 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_285_2280 = _rtP -> P_143 ; _rtB -> B_24_286_2288 = _rtB -> B_24_285_2280
- _rtB -> B_24_283_2264 [ 0 ] ; _rtB -> B_24_1636_12830 = ( _rtB ->
B_24_286_2288 <= sm_aileron_actuator_rtC ( S ) -> B_24_1943_13552 ) ; _rtB ->
B_24_1637_12831 = ( _rtB -> B_24_1636_12830 && _rtB -> B_24_1653_12847 ) ; }
_rtB -> B_24_287_2296 = _rtP -> P_144 * _rtB -> B_24_214_1712 [ 6 ] ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( _rtDW ->
Integrator_IC_LOADING_k != 0 ) { _rtDW -> Integrator_DSTATE_lo = _rtB ->
B_24_287_2296 ; if ( _rtDW -> Integrator_DSTATE_lo > _rtP -> P_146 ) { _rtDW
-> Integrator_DSTATE_lo = _rtP -> P_146 ; } else if ( _rtDW ->
Integrator_DSTATE_lo < _rtP -> P_147 ) { _rtDW -> Integrator_DSTATE_lo = _rtP
-> P_147 ; } } if ( _rtB -> B_24_1637_12831 || ( _rtDW ->
Integrator_PrevResetState_k != 0 ) ) { _rtDW -> Integrator_DSTATE_lo = _rtB
-> B_24_287_2296 ; if ( _rtDW -> Integrator_DSTATE_lo > _rtP -> P_146 ) {
_rtDW -> Integrator_DSTATE_lo = _rtP -> P_146 ; } else if ( _rtDW ->
Integrator_DSTATE_lo < _rtP -> P_147 ) { _rtDW -> Integrator_DSTATE_lo = _rtP
-> P_147 ; } } _rtB -> B_24_288_2304 = _rtDW -> Integrator_DSTATE_lo ; u0 =
_rtB -> B_24_288_2304 ; u1 = _rtP -> P_149 ; u2 = _rtP -> P_148 ; if ( u0 >
u2 ) { _rtB -> B_24_289_2312 = u2 ; } else if ( u0 < u1 ) { _rtB ->
B_24_289_2312 = u1 ; } else { _rtB -> B_24_289_2312 = u0 ; }
ssCallAccelRunBlock ( S , 21 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_290_2320 = _rtB -> B_24_27_216 [ 4 ] * _rtP -> P_150 ; _rtB ->
B_24_291_2328 = _rtDW -> UD_DSTATE_i ; _rtB -> B_24_292_2336 = _rtB ->
B_24_290_2320 - _rtB -> B_24_291_2328 ; ssCallAccelRunBlock ( S , 2 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 24 , 177 ,
SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_Actuator_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Actuator_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_24_229_1832 [ 0 ] + 0 ) ; }
} { if ( _rtDW -> TAQSigLogging_InsertedFor_From_at_outport_0_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_From_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_24_53_424 + 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_Mechanical_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Mechanical_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_24_37_296 + 0 ) ; } } _rtB
-> B_24_1638_12832 = ( _rtB -> B_24_244_1952 > _rtB -> B_24_918_7344 ) ; for
( i = 0 ; i < 5 ; i ++ ) { if ( _rtB -> B_24_1638_12832 ) { _rtB ->
B_24_293_2344 [ i ] = _rtB -> B_24_925_7400 [ i ] ; } else { _rtB ->
B_24_293_2344 [ i ] = 0.0 ; } } } if ( ( _rtDW -> TimeStampA_f >= ssGetT ( S
) ) && ( _rtDW -> TimeStampB_e >= ssGetT ( S ) ) ) { _rtB -> B_24_298_2384 =
0.0 ; } else { u0 = _rtDW -> TimeStampA_f ; lastU = & _rtDW -> LastUAtTimeA_m
; if ( _rtDW -> TimeStampA_f < _rtDW -> TimeStampB_e ) { if ( _rtDW ->
TimeStampB_e < ssGetT ( S ) ) { u0 = _rtDW -> TimeStampB_e ; lastU = & _rtDW
-> LastUAtTimeB_c ; } } else if ( _rtDW -> TimeStampA_f >= ssGetT ( S ) ) {
u0 = _rtDW -> TimeStampB_e ; lastU = & _rtDW -> LastUAtTimeB_c ; } u0 =
ssGetT ( S ) - u0 ; _rtB -> B_24_298_2384 = ( _rtB -> B_19_1055_8440 - *
lastU ) / u0 ; } _rtB -> B_24_1639_12833 = ( _rtB -> B_24_298_2384 > _rtB ->
B_24_930_7440 ) ; for ( i = 0 ; i < 5 ; i ++ ) { if ( _rtB -> B_24_1639_12833
) { _rtB -> B_24_299_2392 [ i ] = _rtB -> B_24_936_7488 [ i ] ; } else { _rtB
-> B_24_299_2392 [ i ] = 0.0 ; } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { _rtB -> B_24_1640_12834 = ( _rtB -> B_16_1058_8464 < _rtB ->
B_24_948_7584 ) ; _rtB -> B_24_1641_12835 = _rtDW -> Memory4_PreviousInput ;
_rtB -> B_24_1642_12836 = _rtDW -> Memory2_PreviousInput ; for ( i = 0 ; i <
5 ; i ++ ) { if ( _rtB -> B_24_1640_12834 ) { _rtB -> B_24_304_2432 [ i ] =
_rtB -> B_24_949_7592 [ i ] ; } else { _rtB -> B_24_304_2432 [ i ] = 0.0 ; }
if ( _rtB -> B_24_1641_12835 ) { _rtB -> B_24_309_2472 [ i ] = _rtB ->
B_24_955_7640 [ i ] ; } else { _rtB -> B_24_309_2472 [ i ] = 0.0 ; } if (
_rtB -> B_24_1642_12836 ) { _rtB -> B_24_314_2512 [ i ] = _rtB ->
B_24_961_7688 [ i ] ; } else { _rtB -> B_24_314_2512 [ i ] = 0.0 ; } } _rtB
-> B_24_1643_12837 = ( _rtB -> B_24_244_1952 == _rtB -> B_24_919_7352 ) ; for
( i = 0 ; i < 5 ; i ++ ) { if ( _rtB -> B_24_1643_12837 ) { _rtB ->
B_24_319_2552 [ i ] = _rtB -> B_24_920_7360 [ i ] ; } else { _rtB ->
B_24_319_2552 [ i ] = 0.0 ; } } } _rtB -> B_24_1644_12838 = ( _rtB ->
B_24_298_2384 < _rtB -> B_24_941_7528 ) ; for ( i = 0 ; i < 5 ; i ++ ) { if (
_rtB -> B_24_1644_12838 ) { _rtB -> B_24_324_2592 [ i ] = _rtB ->
B_24_931_7448 [ i ] ; } else { _rtB -> B_24_324_2592 [ i ] = 0.0 ; } } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_24_1645_12839 =
( _rtB -> B_16_1058_8464 > _rtB -> B_24_942_7536 ) ; intVal = 0U ; for ( i =
0 ; i < 5 ; i ++ ) { if ( _rtB -> B_24_1645_12839 ) { _rtB -> B_24_329_2632 [
i ] = _rtB -> B_24_943_7544 [ i ] ; } else { _rtB -> B_24_329_2632 [ i ] =
0.0 ; } u0 = _rtDW -> Memory5_PreviousInput [ i ] ; _rtB -> B_24_334_2672 [ i
] = u0 ; u = static_cast < uint32_T > ( u0 ) ; intVal <<= 1U ; intVal |= u ;
} _rtB -> B_24_1594_12752 = static_cast < int32_T > ( intVal ) ; _rtB ->
B_24_339_2712 = _rtDW -> Memory6_PreviousInput ; } for ( i = 0 ; i < 5 ; i ++
) { _rtB -> B_22_1007_8056 [ i ] = _rtB -> B_24_293_2344 [ i ] ; } for ( i =
0 ; i < 5 ; i ++ ) { _rtB -> B_22_1007_8056 [ i + 5 ] = _rtB -> B_24_299_2392
[ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_22_1007_8056 [ i + 10 ] =
_rtB -> B_24_304_2432 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB ->
B_22_1007_8056 [ i + 15 ] = _rtB -> B_24_309_2472 [ i ] ; } for ( i = 0 ; i <
5 ; i ++ ) { _rtB -> B_22_1007_8056 [ i + 20 ] = _rtB -> B_24_314_2512 [ i ]
; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_22_1007_8056 [ i + 25 ] = _rtB
-> B_24_319_2552 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB ->
B_22_1007_8056 [ i + 30 ] = _rtB -> B_24_324_2592 [ i ] ; } for ( i = 0 ; i <
5 ; i ++ ) { _rtB -> B_22_1007_8056 [ i + 35 ] = _rtB -> B_24_329_2632 [ i ]
; } ssCallAccelRunBlock ( S , 22 , 1 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_340_2720 [ 0 ] = _rtP -> P_154 [ 0 ] + _rtB -> B_22_1047_8376 [ 1 ] ;
_rtB -> B_24_340_2720 [ 1 ] = _rtP -> P_154 [ 1 ] + _rtB -> B_22_1047_8376 [
2 ] ; _rtB -> B_24_340_2720 [ 2 ] = _rtP -> P_154 [ 2 ] + _rtB ->
B_22_1047_8376 [ 3 ] ; _rtB -> B_24_340_2720 [ 3 ] = _rtP -> P_154 [ 3 ] +
_rtB -> B_22_1047_8376 [ 4 ] ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB ->
B_24_340_2720 [ i + 4 ] = _rtP -> P_154 [ i + 4 ] + _rtB -> B_24_334_2672 [ i
] ; } u0 = 0.0 ; u1 = 0.0 ; u2 = 0.0 ; tmp_0 = 0.0 ; tmp_1 = 0.0 ;
B_24_427_3416_0 = 0.0 ; B_24_507_4056_0 = 0.0 ; tmp_2 = 0.0 ; tmp_3 = 0.0 ;
tmp_4 = 0.0 ; tmp_5 = 0.0 ; tmp_6 = 0.0 ; tmp_7 = 0.0 ; tmp_8 = 0.0 ; tmp_9 =
0.0 ; tmp_a = 0.0 ; tmp_b = 0.0 ; tmp_c = 0.0 ; tmp_d = 0.0 ; tmp_e = 0.0 ;
for ( i = 0 ; i < 9 ; i ++ ) { B_24_349_2792_0 = _rtP -> P_155 [ i ] * _rtB
-> B_24_340_2720 [ i ] ; _rtB -> B_24_349_2792 [ i ] = B_24_349_2792_0 ;
B_24_349_2792_0 += _rtP -> P_156 ; _rtB -> B_24_358_2864 [ i ] =
B_24_349_2792_0 ; tmp_f = _rtB -> B_24_611_4888 [ i ] ; u0 += tmp_f *
B_24_349_2792_0 ; tmp_f = _rtB -> B_24_710_5680 [ i ] ; u1 += tmp_f *
B_24_349_2792_0 ; tmp_f = _rtB -> B_24_728_5824 [ i ] ; u2 += tmp_f *
B_24_349_2792_0 ; tmp_f = _rtB -> B_24_737_5896 [ i ] ; tmp_0 += tmp_f *
B_24_349_2792_0 ; tmp_f = _rtB -> B_24_746_5968 [ i ] ; tmp_1 += tmp_f *
B_24_349_2792_0 ; tmp_f = _rtB -> B_24_755_6040 [ i ] ; B_24_427_3416_0 +=
tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_764_6112 [ i ] ;
B_24_507_4056_0 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_773_6184 [
i ] ; tmp_2 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_782_6256 [ i ]
; tmp_3 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_620_4960 [ i ] ;
tmp_4 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_629_5032 [ i ] ;
tmp_5 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_638_5104 [ i ] ;
tmp_6 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_647_5176 [ i ] ;
tmp_7 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_656_5248 [ i ] ;
tmp_8 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_665_5320 [ i ] ;
tmp_9 += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_674_5392 [ i ] ;
tmp_a += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_683_5464 [ i ] ;
tmp_b += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_692_5536 [ i ] ;
tmp_c += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_701_5608 [ i ] ;
tmp_d += tmp_f * B_24_349_2792_0 ; tmp_f = _rtB -> B_24_719_5752 [ i ] ;
tmp_e += tmp_f * B_24_349_2792_0 ; } _rtB -> B_24_367_2936 = u0 ; _rtB ->
B_24_368_2944 = u1 ; _rtB -> B_24_369_2952 = u2 ; _rtB -> B_24_370_2960 =
tmp_0 ; _rtB -> B_24_371_2968 = tmp_1 ; _rtB -> B_24_372_2976 =
B_24_427_3416_0 ; _rtB -> B_24_373_2984 = B_24_507_4056_0 ; _rtB ->
B_24_374_2992 = tmp_2 ; _rtB -> B_24_375_3000 = tmp_3 ; _rtB -> B_24_376_3008
= tmp_4 ; _rtB -> B_24_377_3016 = tmp_5 ; _rtB -> B_24_378_3024 = tmp_6 ;
_rtB -> B_24_379_3032 = tmp_7 ; _rtB -> B_24_380_3040 = tmp_8 ; _rtB ->
B_24_381_3048 = tmp_9 ; _rtB -> B_24_382_3056 = tmp_a ; _rtB -> B_24_383_3064
= tmp_b ; _rtB -> B_24_384_3072 = tmp_c ; _rtB -> B_24_385_3080 = tmp_d ;
_rtB -> B_24_386_3088 = tmp_e ; _rtB -> B_24_387_3096 [ 0 ] = _rtB ->
B_24_367_2936 + _rtB -> B_24_791_6328 [ 0 ] ; _rtB -> B_24_387_3096 [ 1 ] =
_rtB -> B_24_368_2944 + _rtB -> B_24_791_6328 [ 1 ] ; _rtB -> B_24_387_3096 [
2 ] = _rtB -> B_24_369_2952 + _rtB -> B_24_791_6328 [ 2 ] ; _rtB ->
B_24_387_3096 [ 3 ] = _rtB -> B_24_370_2960 + _rtB -> B_24_791_6328 [ 3 ] ;
_rtB -> B_24_387_3096 [ 4 ] = _rtB -> B_24_371_2968 + _rtB -> B_24_791_6328 [
4 ] ; _rtB -> B_24_387_3096 [ 5 ] = _rtB -> B_24_372_2976 + _rtB ->
B_24_791_6328 [ 5 ] ; _rtB -> B_24_387_3096 [ 6 ] = _rtB -> B_24_373_2984 +
_rtB -> B_24_791_6328 [ 6 ] ; _rtB -> B_24_387_3096 [ 7 ] = _rtB ->
B_24_374_2992 + _rtB -> B_24_791_6328 [ 7 ] ; _rtB -> B_24_387_3096 [ 8 ] =
_rtB -> B_24_375_3000 + _rtB -> B_24_791_6328 [ 8 ] ; _rtB -> B_24_387_3096 [
9 ] = _rtB -> B_24_376_3008 + _rtB -> B_24_791_6328 [ 9 ] ; _rtB ->
B_24_387_3096 [ 10 ] = _rtB -> B_24_377_3016 + _rtB -> B_24_791_6328 [ 10 ] ;
_rtB -> B_24_387_3096 [ 11 ] = _rtB -> B_24_378_3024 + _rtB -> B_24_791_6328
[ 11 ] ; _rtB -> B_24_387_3096 [ 12 ] = _rtB -> B_24_379_3032 + _rtB ->
B_24_791_6328 [ 12 ] ; _rtB -> B_24_387_3096 [ 13 ] = _rtB -> B_24_380_3040 +
_rtB -> B_24_791_6328 [ 13 ] ; _rtB -> B_24_387_3096 [ 14 ] = _rtB ->
B_24_381_3048 + _rtB -> B_24_791_6328 [ 14 ] ; _rtB -> B_24_387_3096 [ 15 ] =
_rtB -> B_24_382_3056 + _rtB -> B_24_791_6328 [ 15 ] ; _rtB -> B_24_387_3096
[ 16 ] = _rtB -> B_24_383_3064 + _rtB -> B_24_791_6328 [ 16 ] ; _rtB ->
B_24_387_3096 [ 17 ] = _rtB -> B_24_384_3072 + _rtB -> B_24_791_6328 [ 17 ] ;
_rtB -> B_24_387_3096 [ 18 ] = _rtB -> B_24_385_3080 + _rtB -> B_24_791_6328
[ 18 ] ; _rtB -> B_24_387_3096 [ 19 ] = _rtB -> B_24_386_3088 + _rtB ->
B_24_791_6328 [ 19 ] ; u0 = 0.0 ; u1 = 0.0 ; u2 = 0.0 ; tmp_0 = 0.0 ; tmp_1 =
0.0 ; for ( i = 0 ; i < 20 ; i ++ ) { _rtB -> B_24_407_3256 [ i ] = _rtP ->
P_157 * _rtB -> B_24_387_3096 [ i ] ; B_24_427_3416_0 = muDoubleScalarExp (
_rtB -> B_24_407_3256 [ i ] ) ; _rtB -> B_24_427_3416 [ i ] = B_24_427_3416_0
; B_24_427_3416_0 += _rtB -> B_24_811_6488 ; _rtB -> B_24_447_3576 [ i ] =
B_24_427_3416_0 ; B_24_427_3416_0 = 1.0 / B_24_427_3416_0 ; _rtB ->
B_24_467_3736 [ i ] = B_24_427_3416_0 ; B_24_427_3416_0 *= _rtP -> P_158 ;
_rtB -> B_24_487_3896 [ i ] = B_24_427_3416_0 ; B_24_507_4056_0 =
B_24_427_3416_0 - _rtB -> B_24_812_6496 ; _rtB -> B_24_507_4056 [ i ] =
B_24_507_4056_0 ; B_24_427_3416_0 = _rtB -> B_24_813_6504 [ i ] ; u0 +=
B_24_427_3416_0 * B_24_507_4056_0 ; B_24_427_3416_0 = _rtB -> B_24_833_6664 [
i ] ; u1 += B_24_427_3416_0 * B_24_507_4056_0 ; B_24_427_3416_0 = _rtB ->
B_24_853_6824 [ i ] ; u2 += B_24_427_3416_0 * B_24_507_4056_0 ;
B_24_427_3416_0 = _rtB -> B_24_873_6984 [ i ] ; tmp_0 += B_24_427_3416_0 *
B_24_507_4056_0 ; B_24_427_3416_0 = _rtB -> B_24_893_7144 [ i ] ; tmp_1 +=
B_24_427_3416_0 * B_24_507_4056_0 ; } _rtB -> B_24_527_4216 = u0 ; _rtB ->
B_24_528_4224 = u1 ; _rtB -> B_24_529_4232 = u2 ; _rtB -> B_24_530_4240 =
tmp_0 ; _rtB -> B_24_531_4248 = tmp_1 ; _rtB -> B_24_532_4256 [ 0 ] = _rtB ->
B_24_527_4216 + _rtB -> B_24_913_7304 [ 0 ] ; _rtB -> B_24_532_4256 [ 1 ] =
_rtB -> B_24_528_4224 + _rtB -> B_24_913_7304 [ 1 ] ; _rtB -> B_24_532_4256 [
2 ] = _rtB -> B_24_529_4232 + _rtB -> B_24_913_7304 [ 2 ] ; _rtB ->
B_24_532_4256 [ 3 ] = _rtB -> B_24_530_4240 + _rtB -> B_24_913_7304 [ 3 ] ;
_rtB -> B_24_532_4256 [ 4 ] = _rtB -> B_24_531_4248 + _rtB -> B_24_913_7304 [
4 ] ; for ( i = 0 ; i < 5 ; i ++ ) { u0 = _rtB -> B_24_532_4256 [ i ] + _rtP
-> P_159 ; _rtB -> B_24_537_4296 [ i ] = u0 ; u0 *= _rtP -> P_160 [ i ] ;
_rtB -> B_24_542_4336 [ i ] = u0 ; _rtB -> B_24_547_4376 [ i ] = u0 + _rtP ->
P_161 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_24_552_4416 [ i ] =
muDoubleScalarRound ( _rtB -> B_24_547_4376 [ i ] ) ; } isHit = ssIsSampleHit
( S , 1 , 0 ) ; if ( isHit != 0 ) { intVal = 0U ; for ( isHit = 0 ; isHit < 5
; isHit ++ ) { u = static_cast < uint32_T > ( _rtB -> B_24_552_4416 [ isHit ]
) ; intVal <<= 1U ; intVal |= u ; } _rtB -> B_24_1595_12756 = static_cast <
int32_T > ( intVal ) ; ssCallAccelRunBlock ( S , 23 , 0 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 24 , 244 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 245 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_557_4456 = _rtDW -> Memory1_PreviousInput_m ; _rtB -> B_24_1646_12840 =
( _rtB -> B_24_557_4456 < _rtB -> B_24_954_7632 ) ; } { real_T * * uBuffer =
( real_T * * ) & _rtDW -> TransportDelay1_PWORK . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_163 ; _rtB
-> B_24_558_4464 = sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay
, 0.0 , * uBuffer , _rtDW -> TransportDelay1_IWORK . CircularBufSize , &
_rtDW -> TransportDelay1_IWORK . Last , _rtDW -> TransportDelay1_IWORK . Tail
, _rtDW -> TransportDelay1_IWORK . Head , _rtP -> P_164 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay1_IWORK .
CircularBufSize ) [ _rtDW -> TransportDelay1_IWORK . Head ] == ssGetT ( S ) )
) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_24_559_4472 = _rtDW -> Memory_PreviousInput_j ; _rtB -> B_24_1647_12841 = (
_rtB -> B_24_559_4472 > _rtB -> B_24_960_7680 ) ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_166 ; _rtB
-> B_24_560_4480 = sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay
, 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW
-> TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW
-> TransportDelay_IWORK . Head , _rtP -> P_167 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK . Head ] == ssGetT ( S ) )
) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
ssCallAccelRunBlock ( S , 24 , 252 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 253 , SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
_asyncqueue_inserted_for_ToWorkspace_PWORK . AQHandles && ssGetLogOutput ( S
) ) { sdiWriteSignal ( _rtDW -> _asyncqueue_inserted_for_ToWorkspace_PWORK .
AQHandles , ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_24_244_1952 + 0
) ; } } { if ( _rtDW -> _asyncqueue_inserted_for_ToWorkspace1_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
_asyncqueue_inserted_for_ToWorkspace1_PWORK . AQHandles , ssGetTaskTime ( S ,
1 ) , ( char * ) & _rtB -> B_24_257_2056 + 0 ) ; } } { if ( _rtDW ->
_asyncqueue_inserted_for_ToWorkspace2_PWORK . AQHandles && ssGetLogOutput ( S
) ) { sdiWriteSignal ( _rtDW -> _asyncqueue_inserted_for_ToWorkspace2_PWORK .
AQHandles , ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_16_1058_8464 + 0
) ; } } { if ( _rtDW -> _asyncqueue_inserted_for_ToWorkspace3_PWORK .
AQHandles && ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
_asyncqueue_inserted_for_ToWorkspace3_PWORK . AQHandles , ssGetTaskTime ( S ,
1 ) , ( char * ) & _rtB -> B_24_258_2064 + 0 ) ; } } { if ( _rtDW ->
_asyncqueue_inserted_for_ToWorkspace4_PWORK . AQHandles && ssGetLogOutput ( S
) ) { sdiWriteSignal ( _rtDW -> _asyncqueue_inserted_for_ToWorkspace4_PWORK .
AQHandles , ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_24_256_2048 + 0
) ; } } } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_g . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_168 ; _rtB -> B_24_561_4488 =
sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_f . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_f . Last , _rtDW -> TransportDelay_IWORK_f . Tail ,
_rtDW -> TransportDelay_IWORK_f . Head , _rtP -> P_169 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] == ssGetT ( S )
) ) ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay1_PWORK_a . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay = simTime - _rtP -> P_170 ; _rtB -> B_24_562_4496 =
sm_aileron_actuator_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay1_IWORK_p . CircularBufSize , & _rtDW ->
TransportDelay1_IWORK_p . Last , _rtDW -> TransportDelay1_IWORK_p . Tail ,
_rtDW -> TransportDelay1_IWORK_p . Head , _rtP -> P_171 , 1 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay1_IWORK_p .
CircularBufSize ) [ _rtDW -> TransportDelay1_IWORK_p . Head ] == ssGetT ( S )
) ) ) ; } _rtB -> B_24_563_4504 = _rtP -> P_172 * _rtB -> B_24_68_544 ; _rtB
-> B_24_564_4512 = _rtP -> P_173 * _rtB -> B_24_63_504 ; _rtB ->
B_24_565_4520 = _rtP -> P_174 * _rtB -> B_24_564_4512 ; ssCallAccelRunBlock (
S , 24 , 307 , SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ;
if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 24 , 308 , SS_CALL_MDL_OUTPUTS
) ; } _rtB -> B_24_567_4536 = _rtP -> P_175 * _rtB -> B_24_125_1000 ; _rtB ->
B_24_568_4544 = _rtP -> P_176 * _rtB -> B_24_120_960 ; _rtB -> B_24_569_4552
= _rtP -> P_177 * _rtB -> B_24_568_4544 ; ssCallAccelRunBlock ( S , 24 , 393
, SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit !=
0 ) { ssCallAccelRunBlock ( S , 24 , 394 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_24_571_4568 = _rtP -> P_178 * _rtB -> B_24_182_1456 ; _rtB -> B_24_572_4576
= _rtP -> P_179 * _rtB -> B_24_177_1416 ; _rtB -> B_24_573_4584 = _rtP ->
P_180 * _rtB -> B_24_572_4576 ; ssCallAccelRunBlock ( S , 24 , 479 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { ssCallAccelRunBlock ( S , 24 , 480 , SS_CALL_MDL_OUTPUTS ) ; u0 =
muDoubleScalarMax ( _rtB -> B_24_232_1856 [ 0 ] , _rtB -> B_24_234_1872 ) ;
_rtB -> B_24_575_4600 = u0 ; _rtB -> B_24_576_4608 = static_cast < real_T > (
_rtB -> B_24_575_4600 == 0.0 ) * 2.2204460492503131e-16 + _rtB ->
B_24_575_4600 ; } _rtB -> B_24_577_4616 = _rtB -> B_24_236_1888 - _rtB ->
B_24_238_1904 ; _rtB -> B_24_578_4624 = 1.0 / _rtB -> B_24_576_4608 * _rtB ->
B_24_577_4616 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { u0
= muDoubleScalarMax ( _rtB -> B_24_259_2072 [ 0 ] , _rtB -> B_24_261_2088 ) ;
_rtB -> B_24_579_4632 = u0 ; _rtB -> B_24_580_4640 = static_cast < real_T > (
_rtB -> B_24_579_4632 == 0.0 ) * 2.2204460492503131e-16 + _rtB ->
B_24_579_4632 ; } _rtB -> B_24_581_4648 = _rtB -> B_24_263_2104 - _rtB ->
B_24_265_2120 ; _rtB -> B_24_582_4656 = 1.0 / _rtB -> B_24_580_4640 * _rtB ->
B_24_581_4648 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { u0
= muDoubleScalarMax ( _rtB -> B_24_276_2208 [ 0 ] , _rtB -> B_24_278_2224 ) ;
_rtB -> B_24_583_4664 = u0 ; _rtB -> B_24_584_4672 = static_cast < real_T > (
_rtB -> B_24_583_4664 == 0.0 ) * 2.2204460492503131e-16 + _rtB ->
B_24_583_4664 ; } _rtB -> B_24_585_4680 = _rtB -> B_24_280_2240 - _rtB ->
B_24_282_2256 ; _rtB -> B_24_586_4688 = 1.0 / _rtB -> B_24_584_4672 * _rtB ->
B_24_585_4680 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { u0
= muDoubleScalarMax ( _rtB -> B_24_245_1960 [ 0 ] , _rtB -> B_24_247_1976 ) ;
_rtB -> B_24_587_4696 = u0 ; _rtB -> B_24_588_4704 = static_cast < real_T > (
_rtB -> B_24_587_4696 == 0.0 ) * 2.2204460492503131e-16 + _rtB ->
B_24_587_4696 ; } _rtB -> B_24_589_4712 = _rtB -> B_24_249_1992 - _rtB ->
B_24_251_2008 ; _rtB -> B_24_590_4720 = 1.0 / _rtB -> B_24_588_4704 * _rtB ->
B_24_589_4712 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { u0
= muDoubleScalarMax ( _rtB -> B_24_266_2128 [ 0 ] , _rtB -> B_24_268_2144 ) ;
_rtB -> B_24_591_4728 = u0 ; _rtB -> B_24_592_4736 = static_cast < real_T > (
_rtB -> B_24_591_4728 == 0.0 ) * 2.2204460492503131e-16 + _rtB ->
B_24_591_4728 ; } _rtB -> B_24_593_4744 = _rtB -> B_24_270_2160 - _rtB ->
B_24_272_2176 ; _rtB -> B_24_594_4752 = 1.0 / _rtB -> B_24_592_4736 * _rtB ->
B_24_593_4744 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { u0
= muDoubleScalarMax ( _rtB -> B_24_283_2264 [ 0 ] , _rtB -> B_24_285_2280 ) ;
_rtB -> B_24_595_4760 = u0 ; _rtB -> B_24_596_4768 = static_cast < real_T > (
_rtB -> B_24_595_4760 == 0.0 ) * 2.2204460492503131e-16 + _rtB ->
B_24_595_4760 ; } _rtB -> B_24_597_4776 = _rtB -> B_24_287_2296 - _rtB ->
B_24_289_2312 ; _rtB -> B_24_598_4784 = 1.0 / _rtB -> B_24_596_4768 * _rtB ->
B_24_597_4776 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
ssCallAccelRunBlock ( S , 24 , 549 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 551 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 553 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_24_599_4792 [ 0 ] = _rtB -> B_24_101_808 [ 6 ] ; _rtB -> B_24_599_4792 [ 1
] = 0.0 ; _rtB -> B_24_599_4792 [ 2 ] = 0.0 ; _rtB -> B_24_599_4792 [ 3 ] =
0.0 ; _rtB -> B_24_603_4824 [ 0 ] = _rtB -> B_24_158_1264 [ 6 ] ; _rtB ->
B_24_603_4824 [ 1 ] = 0.0 ; _rtB -> B_24_603_4824 [ 2 ] = 0.0 ; _rtB ->
B_24_603_4824 [ 3 ] = 0.0 ; _rtB -> B_24_607_4856 [ 0 ] = _rtB ->
B_24_214_1712 [ 6 ] ; _rtB -> B_24_607_4856 [ 1 ] = 0.0 ; _rtB ->
B_24_607_4856 [ 2 ] = 0.0 ; _rtB -> B_24_607_4856 [ 3 ] = 0.0 ;
ssCallAccelRunBlock ( S , 24 , 560 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 561 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 1289 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 1290 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 1291 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 24 , 1292 , SS_CALL_MDL_OUTPUTS ) ;
UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID2 ( SimStruct * S ,
int_T tid ) { B_sm_aileron_actuator_T * _rtB ; P_sm_aileron_actuator_T * _rtP
; int32_T i ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ;
_rtB = ( ( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ; memcpy (
& _rtB -> B_24_611_4888 [ 0 ] , & _rtP -> P_181 [ 0 ] , 9U * sizeof ( real_T
) ) ; memcpy ( & _rtB -> B_24_620_4960 [ 0 ] , & _rtP -> P_182 [ 0 ] , 9U *
sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_629_5032 [ 0 ] , & _rtP ->
P_183 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_638_5104 [ 0
] , & _rtP -> P_184 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_24_647_5176 [ 0 ] , & _rtP -> P_185 [ 0 ] , 9U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_24_656_5248 [ 0 ] , & _rtP -> P_186 [ 0 ] , 9U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_24_665_5320 [ 0 ] , & _rtP -> P_187 [ 0 ]
, 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_674_5392 [ 0 ] , & _rtP
-> P_188 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_683_5464
[ 0 ] , & _rtP -> P_189 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_24_692_5536 [ 0 ] , & _rtP -> P_190 [ 0 ] , 9U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_24_701_5608 [ 0 ] , & _rtP -> P_191 [ 0 ] , 9U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_24_710_5680 [ 0 ] , & _rtP -> P_192 [ 0 ]
, 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_719_5752 [ 0 ] , & _rtP
-> P_193 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_728_5824
[ 0 ] , & _rtP -> P_194 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_24_737_5896 [ 0 ] , & _rtP -> P_195 [ 0 ] , 9U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_24_746_5968 [ 0 ] , & _rtP -> P_196 [ 0 ] , 9U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_24_755_6040 [ 0 ] , & _rtP -> P_197 [ 0 ]
, 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_764_6112 [ 0 ] , & _rtP
-> P_198 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_773_6184
[ 0 ] , & _rtP -> P_199 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_24_782_6256 [ 0 ] , & _rtP -> P_200 [ 0 ] , 9U * sizeof ( real_T ) ) ; _rtB
-> B_24_811_6488 = _rtP -> P_202 ; _rtB -> B_24_812_6496 = _rtP -> P_203 ;
memcpy ( & _rtB -> B_24_791_6328 [ 0 ] , & _rtP -> P_201 [ 0 ] , 20U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_24_813_6504 [ 0 ] , & _rtP -> P_204 [ 0 ]
, 20U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_833_6664 [ 0 ] , & _rtP
-> P_205 [ 0 ] , 20U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_853_6824
[ 0 ] , & _rtP -> P_206 [ 0 ] , 20U * sizeof ( real_T ) ) ; memcpy ( & _rtB
-> B_24_873_6984 [ 0 ] , & _rtP -> P_207 [ 0 ] , 20U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_24_893_7144 [ 0 ] , & _rtP -> P_208 [ 0 ] , 20U * sizeof
( real_T ) ) ; _rtB -> B_24_918_7344 = _rtP -> P_210 ; _rtB -> B_24_919_7352
= _rtP -> P_211 ; _rtB -> B_24_930_7440 = _rtP -> P_214 ; _rtB ->
B_24_941_7528 = _rtP -> P_217 ; _rtB -> B_24_942_7536 = _rtP -> P_218 ; _rtB
-> B_24_948_7584 = _rtP -> P_220 ; _rtB -> B_24_954_7632 = _rtP -> P_222 ;
_rtB -> B_24_960_7680 = _rtP -> P_224 ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB
-> B_24_913_7304 [ i ] = _rtP -> P_209 [ i ] ; _rtB -> B_24_920_7360 [ i ] =
_rtP -> P_212 [ i ] ; _rtB -> B_24_925_7400 [ i ] = _rtP -> P_213 [ i ] ;
_rtB -> B_24_931_7448 [ i ] = _rtP -> P_215 [ i ] ; _rtB -> B_24_936_7488 [ i
] = _rtP -> P_216 [ i ] ; _rtB -> B_24_943_7544 [ i ] = _rtP -> P_219 [ i ] ;
_rtB -> B_24_949_7592 [ i ] = _rtP -> P_221 [ i ] ; _rtB -> B_24_955_7640 [ i
] = _rtP -> P_223 [ i ] ; _rtB -> B_24_961_7688 [ i ] = _rtP -> P_225 [ i ] ;
} memcpy ( & _rtB -> B_10_1111_8888 [ 0 ] , & _rtP -> P_24 [ 0 ] , 408U *
sizeof ( real_T ) ) ; sm_aileron_actuator_findnodeTID2 ( S , & _rtB ->
findnode , & _rtP -> findnode ) ; memcpy ( & _rtB -> B_10_1519_12152 [ 0 ] ,
& _rtP -> P_25 [ 0 ] , sizeof ( real_T ) << 6U ) ; _rtB -> B_9_1625_12818 =
_rtP -> P_253 ; memcpy ( & _rtB -> B_10_1583_12664 [ 0 ] , & _rtP -> P_26 [ 0
] , sizeof ( real_T ) << 3U ) ; _rtB -> B_10_1606_12780 = _rtP -> P_254 ;
_rtB -> B_24_966_7728 = _rtP -> P_226 ; _rtB -> B_24_967_7736 = _rtP -> P_227
; for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_24_968_7744 [ i ] = _rtP -> P_228
[ i ] ; } _rtB -> B_24_973_7784 = _rtP -> P_229 ; _rtB -> B_24_974_7792 =
_rtP -> P_230 ; _rtB -> B_24_975_7800 = _rtP -> P_231 ; for ( i = 0 ; i < 5 ;
i ++ ) { _rtB -> B_24_976_7808 [ i ] = _rtP -> P_232 [ i ] ; } _rtB ->
B_24_981_7848 = _rtP -> P_233 ; _rtB -> B_24_982_7856 = _rtP -> P_234 ; _rtB
-> B_24_983_7864 = _rtP -> P_235 ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB ->
B_24_984_7872 [ i ] = _rtP -> P_236 [ i ] ; } _rtB -> B_24_989_7912 = _rtP ->
P_237 ; _rtB -> B_24_990_7920 = _rtP -> P_238 ; _rtB -> B_24_991_7928 = _rtP
-> P_239 ; _rtB -> B_24_1648_12842 = ( _rtB -> B_24_990_7920 < _rtB ->
B_24_991_7928 ) ; _rtB -> B_24_992_7936 = _rtP -> P_240 ; _rtB ->
B_24_993_7944 = _rtP -> P_241 ; _rtB -> B_24_1649_12843 = ( _rtB ->
B_24_992_7936 < _rtB -> B_24_993_7944 ) ; _rtB -> B_24_994_7952 = _rtP ->
P_242 ; _rtB -> B_24_995_7960 = _rtP -> P_243 ; _rtB -> B_24_1650_12844 = (
_rtB -> B_24_994_7952 < _rtB -> B_24_995_7960 ) ; _rtB -> B_24_996_7968 =
_rtP -> P_244 ; _rtB -> B_24_997_7976 = _rtP -> P_245 ; _rtB ->
B_24_1651_12845 = ( _rtB -> B_24_996_7968 < _rtB -> B_24_997_7976 ) ; _rtB ->
B_24_998_7984 = _rtP -> P_246 ; _rtB -> B_24_999_7992 = _rtP -> P_247 ; _rtB
-> B_24_1652_12846 = ( _rtB -> B_24_998_7984 < _rtB -> B_24_999_7992 ) ; _rtB
-> B_24_1000_8000 = _rtP -> P_248 ; _rtB -> B_24_1001_8008 = _rtP -> P_249 ;
_rtB -> B_24_1653_12847 = ( _rtB -> B_24_1000_8000 < _rtB -> B_24_1001_8008 )
; _rtB -> B_24_1002_8016 = _rtP -> P_250 ; ssCallAccelRunBlock ( S , 24 ,
1561 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 24 , 1562 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 24 , 1563 ,
SS_CALL_MDL_OUTPUTS ) ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { B_sm_aileron_actuator_T
* _rtB ; DW_sm_aileron_actuator_T * _rtDW ; P_sm_aileron_actuator_T * _rtP ;
real_T * lastU ; int32_T i ; _rtDW = ( ( DW_sm_aileron_actuator_T * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp
( S ) ) ; _rtB = ( ( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 24 , 1 , SS_CALL_MDL_UPDATE ) ; ssCallAccelRunBlock
( S , 24 , 43 , SS_CALL_MDL_UPDATE ) ; ssCallAccelRunBlock ( S , 24 , 68 ,
SS_CALL_MDL_UPDATE ) ; ssCallAccelRunBlock ( S , 24 , 93 , SS_CALL_MDL_UPDATE
) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
Integrator_IC_LOADING = 0U ; _rtDW -> Integrator_DSTATE += _rtP -> P_100 *
_rtB -> B_24_578_4624 ; if ( _rtDW -> Integrator_DSTATE > _rtP -> P_101 ) {
_rtDW -> Integrator_DSTATE = _rtP -> P_101 ; } else if ( _rtDW ->
Integrator_DSTATE < _rtP -> P_102 ) { _rtDW -> Integrator_DSTATE = _rtP ->
P_102 ; } _rtDW -> Integrator_PrevResetState = static_cast < int8_T > ( _rtB
-> B_24_1627_12821 ) ; _rtDW -> UD_DSTATE = _rtB -> B_24_239_1912 ; _rtDW ->
UD_DSTATE_l = _rtB -> B_24_242_1936 ; _rtDW -> Integrator_IC_LOADING_p = 0U ;
_rtDW -> Integrator_DSTATE_h += _rtP -> P_111 * _rtB -> B_24_590_4720 ; if (
_rtDW -> Integrator_DSTATE_h > _rtP -> P_112 ) { _rtDW -> Integrator_DSTATE_h
= _rtP -> P_112 ; } else if ( _rtDW -> Integrator_DSTATE_h < _rtP -> P_113 )
{ _rtDW -> Integrator_DSTATE_h = _rtP -> P_113 ; } _rtDW ->
Integrator_PrevResetState_m = static_cast < int8_T > ( _rtB ->
B_24_1629_12823 ) ; _rtDW -> UD_DSTATE_p = _rtB -> B_24_252_2016 ; } if (
_rtDW -> TimeStampA == ( rtInf ) ) { _rtDW -> TimeStampA = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeA ; } else if ( _rtDW -> TimeStampB == ( rtInf
) ) { _rtDW -> TimeStampB = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB ;
} else if ( _rtDW -> TimeStampA < _rtDW -> TimeStampB ) { _rtDW -> TimeStampA
= ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA ; } else { _rtDW ->
TimeStampB = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB ; } * lastU =
_rtB -> B_24_254_2032 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
_rtDW -> Memory_PreviousInput = _rtB -> B_24_561_4488 ; } if ( _rtDW ->
TimeStampA_o == ( rtInf ) ) { _rtDW -> TimeStampA_o = ssGetT ( S ) ; lastU =
& _rtDW -> LastUAtTimeA_l ; } else if ( _rtDW -> TimeStampB_f == ( rtInf ) )
{ _rtDW -> TimeStampB_f = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_g ;
} else if ( _rtDW -> TimeStampA_o < _rtDW -> TimeStampB_f ) { _rtDW ->
TimeStampA_o = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_l ; } else {
_rtDW -> TimeStampB_f = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeB_g ; }
* lastU = _rtB -> B_19_1055_8440 ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i
!= 0 ) { _rtDW -> Memory1_PreviousInput = _rtB -> B_24_562_4496 ; _rtDW ->
Integrator_IC_LOADING_a = 0U ; _rtDW -> Integrator_DSTATE_l += _rtP -> P_122
* _rtB -> B_24_582_4656 ; if ( _rtDW -> Integrator_DSTATE_l > _rtP -> P_123 )
{ _rtDW -> Integrator_DSTATE_l = _rtP -> P_123 ; } else if ( _rtDW ->
Integrator_DSTATE_l < _rtP -> P_124 ) { _rtDW -> Integrator_DSTATE_l = _rtP
-> P_124 ; } _rtDW -> Integrator_PrevResetState_f = static_cast < int8_T > (
_rtB -> B_24_1631_12825 ) ; _rtDW -> Integrator_IC_LOADING_m = 0U ; _rtDW ->
Integrator_DSTATE_e += _rtP -> P_129 * _rtB -> B_24_594_4752 ; if ( _rtDW ->
Integrator_DSTATE_e > _rtP -> P_130 ) { _rtDW -> Integrator_DSTATE_e = _rtP
-> P_130 ; } else if ( _rtDW -> Integrator_DSTATE_e < _rtP -> P_131 ) { _rtDW
-> Integrator_DSTATE_e = _rtP -> P_131 ; } _rtDW ->
Integrator_PrevResetState_ms = static_cast < int8_T > ( _rtB ->
B_24_1633_12827 ) ; _rtDW -> UD_DSTATE_h = _rtB -> B_24_273_2184 ; _rtDW ->
Integrator_IC_LOADING_j = 0U ; _rtDW -> Integrator_DSTATE_f += _rtP -> P_138
* _rtB -> B_24_586_4688 ; if ( _rtDW -> Integrator_DSTATE_f > _rtP -> P_139 )
{ _rtDW -> Integrator_DSTATE_f = _rtP -> P_139 ; } else if ( _rtDW ->
Integrator_DSTATE_f < _rtP -> P_140 ) { _rtDW -> Integrator_DSTATE_f = _rtP
-> P_140 ; } _rtDW -> Integrator_PrevResetState_p = static_cast < int8_T > (
_rtB -> B_24_1635_12829 ) ; _rtDW -> Integrator_IC_LOADING_k = 0U ; _rtDW ->
Integrator_DSTATE_lo += _rtP -> P_145 * _rtB -> B_24_598_4784 ; if ( _rtDW ->
Integrator_DSTATE_lo > _rtP -> P_146 ) { _rtDW -> Integrator_DSTATE_lo = _rtP
-> P_146 ; } else if ( _rtDW -> Integrator_DSTATE_lo < _rtP -> P_147 ) {
_rtDW -> Integrator_DSTATE_lo = _rtP -> P_147 ; } _rtDW ->
Integrator_PrevResetState_k = static_cast < int8_T > ( _rtB ->
B_24_1637_12831 ) ; _rtDW -> UD_DSTATE_i = _rtB -> B_24_290_2320 ; } if (
_rtDW -> TimeStampA_f == ( rtInf ) ) { _rtDW -> TimeStampA_f = ssGetT ( S ) ;
lastU = & _rtDW -> LastUAtTimeA_m ; } else if ( _rtDW -> TimeStampB_e == (
rtInf ) ) { _rtDW -> TimeStampB_e = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_c ; } else if ( _rtDW -> TimeStampA_f < _rtDW -> TimeStampB_e )
{ _rtDW -> TimeStampA_f = ssGetT ( S ) ; lastU = & _rtDW -> LastUAtTimeA_m ;
} else { _rtDW -> TimeStampB_e = ssGetT ( S ) ; lastU = & _rtDW ->
LastUAtTimeB_c ; } * lastU = _rtB -> B_19_1055_8440 ; i = ssIsSampleHit ( S ,
1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory4_PreviousInput = _rtB ->
B_24_1646_12840 ; _rtDW -> Memory2_PreviousInput = _rtB -> B_24_1647_12841 ;
for ( i = 0 ; i < 5 ; i ++ ) { _rtDW -> Memory5_PreviousInput [ i ] = _rtB ->
B_24_552_4416 [ i ] ; } _rtDW -> Memory6_PreviousInput = _rtB ->
B_22_1052_8416 ; _rtDW -> Memory1_PreviousInput_m = _rtB -> B_24_558_4464 ; }
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay1_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay1_IWORK . Head = ( ( _rtDW -> TransportDelay1_IWORK . Head < (
_rtDW -> TransportDelay1_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay1_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay1_IWORK . Head == _rtDW -> TransportDelay1_IWORK . Tail ) { if
( ! sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay1_IWORK . CircularBufSize , & _rtDW -> TransportDelay1_IWORK .
Tail , & _rtDW -> TransportDelay1_IWORK . Head , & _rtDW ->
TransportDelay1_IWORK . Last , simTime - _rtP -> P_163 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay1_IWORK . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay1_IWORK . CircularBufSize
) [ _rtDW -> TransportDelay1_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW
-> TransportDelay1_IWORK . Head ] = _rtB -> B_24_254_2032 ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_j
= _rtB -> B_24_560_4480 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK . Head = ( ( _rtDW -> TransportDelay_IWORK .
Head < ( _rtDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK
. Head == _rtDW -> TransportDelay_IWORK . Tail ) { if ( !
sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK . CircularBufSize , & _rtDW -> TransportDelay_IWORK .
Tail , & _rtDW -> TransportDelay_IWORK . Head , & _rtDW ->
TransportDelay_IWORK . Last , simTime - _rtP -> P_166 , uBuffer , ( boolean_T
) 0 , false , & _rtDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK . CircularBufSize )
[ _rtDW -> TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK . Head ] = _rtB -> B_24_241_1928 ; } { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_g . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_f . Head =
( ( _rtDW -> TransportDelay_IWORK_f . Head < ( _rtDW ->
TransportDelay_IWORK_f . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_f . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_f . Head == _rtDW -> TransportDelay_IWORK_f . Tail ) {
if ( ! sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_f . CircularBufSize , & _rtDW -> TransportDelay_IWORK_f
. Tail , & _rtDW -> TransportDelay_IWORK_f . Head , & _rtDW ->
TransportDelay_IWORK_f . Last , simTime - _rtP -> P_168 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_f . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_f . Head ] = _rtB -> B_24_241_1928
; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay1_PWORK_a
. TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay1_IWORK_p . Head = ( ( _rtDW -> TransportDelay1_IWORK_p . Head
< ( _rtDW -> TransportDelay1_IWORK_p . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay1_IWORK_p . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay1_IWORK_p . Head == _rtDW -> TransportDelay1_IWORK_p . Tail ) {
if ( ! sm_aileron_actuator_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay1_IWORK_p . CircularBufSize , & _rtDW ->
TransportDelay1_IWORK_p . Tail , & _rtDW -> TransportDelay1_IWORK_p . Head ,
& _rtDW -> TransportDelay1_IWORK_p . Last , simTime - _rtP -> P_170 , uBuffer
, ( boolean_T ) 0 , false , & _rtDW -> TransportDelay1_IWORK_p .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW ->
TransportDelay1_IWORK_p . CircularBufSize ) [ _rtDW ->
TransportDelay1_IWORK_p . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay1_IWORK_p . Head ] = _rtB -> B_24_254_2032 ; }
ssCallAccelRunBlock ( S , 24 , 561 , SS_CALL_MDL_UPDATE ) ; UNUSED_PARAMETER
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
ssCallAccelRunBlock ( S , 24 , 1 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_1513730075_0 = ( _rtB -> B_24_27_216 [ 1 ]
- _rtX -> sm_aileron_actuatoroutputFiltered_1513730075_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE > _rtP -> P_46 ) && ( _rtX -> u5_CSTATE < _rtP -> P_45 ) )
|| ( ( _rtX -> u5_CSTATE <= _rtP -> P_46 ) && ( _rtB -> B_24_563_4504 > 0.0 )
) || ( ( _rtX -> u5_CSTATE >= _rtP -> P_45 ) && ( _rtB -> B_24_563_4504 < 0.0
) ) ) { _rtXdot -> u5_CSTATE = _rtB -> B_24_563_4504 ; } else { _rtXdot ->
u5_CSTATE = 0.0 ; } _rtXdot -> u0HzLowpassFilter_CSTATE = 0.0 ; _rtXdot ->
u0HzLowpassFilter_CSTATE += _rtP -> P_47 * _rtX -> u0HzLowpassFilter_CSTATE ;
_rtXdot -> u0HzLowpassFilter_CSTATE += _rtB -> B_24_101_808 [ 1 ] ; if ( ( (
_rtX -> Limits55_CSTATE > _rtP -> P_51 ) && ( _rtX -> Limits55_CSTATE < _rtP
-> P_50 ) ) || ( ( _rtX -> Limits55_CSTATE <= _rtP -> P_51 ) && ( _rtB ->
B_24_565_4520 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE >= _rtP -> P_50 ) && (
_rtB -> B_24_565_4520 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE = _rtB ->
B_24_565_4520 ; } else { _rtXdot -> Limits55_CSTATE = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_e = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_e +=
_rtP -> P_52 * _rtX -> u0HzLowpassFilter_CSTATE_e ; _rtXdot ->
u0HzLowpassFilter_CSTATE_e += _rtB -> B_24_101_808 [ 2 ] ;
ssCallAccelRunBlock ( S , 24 , 43 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_553247611_0 = ( _rtB -> B_24_27_216 [ 3 ] -
_rtX -> sm_aileron_actuatoroutputFiltered_553247611_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE_f > _rtP -> P_64 ) && ( _rtX -> u5_CSTATE_f < _rtP -> P_63
) ) || ( ( _rtX -> u5_CSTATE_f <= _rtP -> P_64 ) && ( _rtB -> B_24_567_4536 >
0.0 ) ) || ( ( _rtX -> u5_CSTATE_f >= _rtP -> P_63 ) && ( _rtB ->
B_24_567_4536 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_f = _rtB -> B_24_567_4536 ;
} else { _rtXdot -> u5_CSTATE_f = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_d = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_d +=
_rtP -> P_65 * _rtX -> u0HzLowpassFilter_CSTATE_d ; _rtXdot ->
u0HzLowpassFilter_CSTATE_d += _rtB -> B_24_158_1264 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_g > _rtP -> P_69 ) && ( _rtX -> Limits55_CSTATE_g < _rtP ->
P_68 ) ) || ( ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_69 ) && ( _rtB ->
B_24_569_4552 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_g >= _rtP -> P_68 ) &&
( _rtB -> B_24_569_4552 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_g = _rtB ->
B_24_569_4552 ; } else { _rtXdot -> Limits55_CSTATE_g = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_b = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_b +=
_rtP -> P_70 * _rtX -> u0HzLowpassFilter_CSTATE_b ; _rtXdot ->
u0HzLowpassFilter_CSTATE_b += _rtB -> B_24_158_1264 [ 2 ] ;
ssCallAccelRunBlock ( S , 24 , 68 , SS_CALL_MDL_DERIVATIVES ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_2948142811_0 = ( _rtB -> B_24_27_216 [ 5 ]
- _rtX -> sm_aileron_actuatoroutputFiltered_2948142811_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE_n > _rtP -> P_82 ) && ( _rtX -> u5_CSTATE_n < _rtP -> P_81
) ) || ( ( _rtX -> u5_CSTATE_n <= _rtP -> P_82 ) && ( _rtB -> B_24_571_4568 >
0.0 ) ) || ( ( _rtX -> u5_CSTATE_n >= _rtP -> P_81 ) && ( _rtB ->
B_24_571_4568 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_n = _rtB -> B_24_571_4568 ;
} else { _rtXdot -> u5_CSTATE_n = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_i = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_i +=
_rtP -> P_83 * _rtX -> u0HzLowpassFilter_CSTATE_i ; _rtXdot ->
u0HzLowpassFilter_CSTATE_i += _rtB -> B_24_214_1712 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_p > _rtP -> P_87 ) && ( _rtX -> Limits55_CSTATE_p < _rtP ->
P_86 ) ) || ( ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_87 ) && ( _rtB ->
B_24_573_4584 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_p >= _rtP -> P_86 ) &&
( _rtB -> B_24_573_4584 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_p = _rtB ->
B_24_573_4584 ; } else { _rtXdot -> Limits55_CSTATE_p = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_p = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_p +=
_rtP -> P_88 * _rtX -> u0HzLowpassFilter_CSTATE_p ; _rtXdot ->
u0HzLowpassFilter_CSTATE_p += _rtB -> B_24_214_1712 [ 2 ] ;
ssCallAccelRunBlock ( S , 24 , 93 , SS_CALL_MDL_DERIVATIVES ) ; }
#define MDL_PROJECTION
static void mdlProjection ( SimStruct * S ) { ssCallAccelRunBlock ( S , 24 ,
1 , SS_CALL_MDL_PROJECTION ) ; ssCallAccelRunBlock ( S , 24 , 43 ,
SS_CALL_MDL_PROJECTION ) ; ssCallAccelRunBlock ( S , 24 , 68 ,
SS_CALL_MDL_PROJECTION ) ; ssCallAccelRunBlock ( S , 24 , 93 ,
SS_CALL_MDL_PROJECTION ) ; }
#define MDL_FORCINGFUNCTION
static void mdlForcingFunction ( SimStruct * S ) { B_sm_aileron_actuator_T *
_rtB ; P_sm_aileron_actuator_T * _rtP ; XDot_sm_aileron_actuator_T * _rtXdot
; X_sm_aileron_actuator_T * _rtX ; _rtXdot = ( ( XDot_sm_aileron_actuator_T *
) ssGetdX ( S ) ) ; _rtX = ( ( X_sm_aileron_actuator_T * ) ssGetContStates (
S ) ) ; _rtP = ( ( P_sm_aileron_actuator_T * ) ssGetModelRtp ( S ) ) ; _rtB =
( ( B_sm_aileron_actuator_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 24 , 1 , SS_CALL_MDL_FORCINGFUNCTION ) ; _rtXdot ->
sm_aileron_actuatoroutputFiltered_1513730075_0 = ( _rtB -> B_24_27_216 [ 1 ]
- _rtX -> sm_aileron_actuatoroutputFiltered_1513730075_0 ) * 100.0 ; if ( ( (
_rtX -> u5_CSTATE > _rtP -> P_46 ) && ( _rtX -> u5_CSTATE < _rtP -> P_45 ) )
|| ( ( _rtX -> u5_CSTATE <= _rtP -> P_46 ) && ( _rtB -> B_24_563_4504 > 0.0 )
) || ( ( _rtX -> u5_CSTATE >= _rtP -> P_45 ) && ( _rtB -> B_24_563_4504 < 0.0
) ) ) { _rtXdot -> u5_CSTATE = _rtB -> B_24_563_4504 ; } else { _rtXdot ->
u5_CSTATE = 0.0 ; } _rtXdot -> u0HzLowpassFilter_CSTATE = 0.0 ; _rtXdot ->
u0HzLowpassFilter_CSTATE += _rtP -> P_47 * _rtX -> u0HzLowpassFilter_CSTATE ;
_rtXdot -> u0HzLowpassFilter_CSTATE += _rtB -> B_24_101_808 [ 1 ] ; if ( ( (
_rtX -> Limits55_CSTATE > _rtP -> P_51 ) && ( _rtX -> Limits55_CSTATE < _rtP
-> P_50 ) ) || ( ( _rtX -> Limits55_CSTATE <= _rtP -> P_51 ) && ( _rtB ->
B_24_565_4520 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE >= _rtP -> P_50 ) && (
_rtB -> B_24_565_4520 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE = _rtB ->
B_24_565_4520 ; } else { _rtXdot -> Limits55_CSTATE = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_e = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_e +=
_rtP -> P_52 * _rtX -> u0HzLowpassFilter_CSTATE_e ; _rtXdot ->
u0HzLowpassFilter_CSTATE_e += _rtB -> B_24_101_808 [ 2 ] ;
ssCallAccelRunBlock ( S , 24 , 43 , SS_CALL_MDL_FORCINGFUNCTION ) ; _rtXdot
-> sm_aileron_actuatoroutputFiltered_553247611_0 = ( _rtB -> B_24_27_216 [ 3
] - _rtX -> sm_aileron_actuatoroutputFiltered_553247611_0 ) * 100.0 ; if ( (
( _rtX -> u5_CSTATE_f > _rtP -> P_64 ) && ( _rtX -> u5_CSTATE_f < _rtP ->
P_63 ) ) || ( ( _rtX -> u5_CSTATE_f <= _rtP -> P_64 ) && ( _rtB ->
B_24_567_4536 > 0.0 ) ) || ( ( _rtX -> u5_CSTATE_f >= _rtP -> P_63 ) && (
_rtB -> B_24_567_4536 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_f = _rtB ->
B_24_567_4536 ; } else { _rtXdot -> u5_CSTATE_f = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_d = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_d +=
_rtP -> P_65 * _rtX -> u0HzLowpassFilter_CSTATE_d ; _rtXdot ->
u0HzLowpassFilter_CSTATE_d += _rtB -> B_24_158_1264 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_g > _rtP -> P_69 ) && ( _rtX -> Limits55_CSTATE_g < _rtP ->
P_68 ) ) || ( ( _rtX -> Limits55_CSTATE_g <= _rtP -> P_69 ) && ( _rtB ->
B_24_569_4552 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_g >= _rtP -> P_68 ) &&
( _rtB -> B_24_569_4552 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_g = _rtB ->
B_24_569_4552 ; } else { _rtXdot -> Limits55_CSTATE_g = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_b = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_b +=
_rtP -> P_70 * _rtX -> u0HzLowpassFilter_CSTATE_b ; _rtXdot ->
u0HzLowpassFilter_CSTATE_b += _rtB -> B_24_158_1264 [ 2 ] ;
ssCallAccelRunBlock ( S , 24 , 68 , SS_CALL_MDL_FORCINGFUNCTION ) ; _rtXdot
-> sm_aileron_actuatoroutputFiltered_2948142811_0 = ( _rtB -> B_24_27_216 [ 5
] - _rtX -> sm_aileron_actuatoroutputFiltered_2948142811_0 ) * 100.0 ; if ( (
( _rtX -> u5_CSTATE_n > _rtP -> P_82 ) && ( _rtX -> u5_CSTATE_n < _rtP ->
P_81 ) ) || ( ( _rtX -> u5_CSTATE_n <= _rtP -> P_82 ) && ( _rtB ->
B_24_571_4568 > 0.0 ) ) || ( ( _rtX -> u5_CSTATE_n >= _rtP -> P_81 ) && (
_rtB -> B_24_571_4568 < 0.0 ) ) ) { _rtXdot -> u5_CSTATE_n = _rtB ->
B_24_571_4568 ; } else { _rtXdot -> u5_CSTATE_n = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_i = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_i +=
_rtP -> P_83 * _rtX -> u0HzLowpassFilter_CSTATE_i ; _rtXdot ->
u0HzLowpassFilter_CSTATE_i += _rtB -> B_24_214_1712 [ 1 ] ; if ( ( ( _rtX ->
Limits55_CSTATE_p > _rtP -> P_87 ) && ( _rtX -> Limits55_CSTATE_p < _rtP ->
P_86 ) ) || ( ( _rtX -> Limits55_CSTATE_p <= _rtP -> P_87 ) && ( _rtB ->
B_24_573_4584 > 0.0 ) ) || ( ( _rtX -> Limits55_CSTATE_p >= _rtP -> P_86 ) &&
( _rtB -> B_24_573_4584 < 0.0 ) ) ) { _rtXdot -> Limits55_CSTATE_p = _rtB ->
B_24_573_4584 ; } else { _rtXdot -> Limits55_CSTATE_p = 0.0 ; } _rtXdot ->
u0HzLowpassFilter_CSTATE_p = 0.0 ; _rtXdot -> u0HzLowpassFilter_CSTATE_p +=
_rtP -> P_88 * _rtX -> u0HzLowpassFilter_CSTATE_p ; _rtXdot ->
u0HzLowpassFilter_CSTATE_p += _rtB -> B_24_214_1712 [ 2 ] ;
ssCallAccelRunBlock ( S , 24 , 93 , SS_CALL_MDL_FORCINGFUNCTION ) ; }
#define MDL_MASSMATRIX
static void mdlMassMatrix ( SimStruct * S ) { ssCallAccelRunBlock ( S , 24 ,
1 , SS_CALL_MDL_MASSMATRIX ) ; ssCallAccelRunBlock ( S , 24 , 43 ,
SS_CALL_MDL_MASSMATRIX ) ; ssCallAccelRunBlock ( S , 24 , 68 ,
SS_CALL_MDL_MASSMATRIX ) ; ssCallAccelRunBlock ( S , 24 , 93 ,
SS_CALL_MDL_MASSMATRIX ) ; } static void mdlInitializeSizes ( SimStruct * S )
{ ssSetChecksumVal ( S , 0 , 2442474124U ) ; ssSetChecksumVal ( S , 1 ,
2421367037U ) ; ssSetChecksumVal ( S , 2 , 2441011044U ) ; ssSetChecksumVal (
S , 3 , 4268522949U ) ; { mxArray * slVerStructMat = ( NULL ) ; mxArray *
slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ] ; int status
= mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver" ) ; if (
status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0 ,
"Version" ) ; if ( slVerMat == ( NULL ) ) { status = 1 ; } else { status =
mxGetString ( slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "24.1" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
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
= 42 ; ( ( DW_sm_aileron_actuator_T * ) ssGetRootDWork ( S ) ) ->
STATE_2_MASS_MATRIX_PR = 53 ; } } } static void mdlInitializeSampleTimes (
SimStruct * S ) { { SimStruct * childS ; SysOutputFcn * callSysFcns ; childS
= ssGetSFunction ( S , 0 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 1 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 2 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 3 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 4 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 5 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 6 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 7 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 8 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 9 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; }
slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
