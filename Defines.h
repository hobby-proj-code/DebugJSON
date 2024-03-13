
#define MAP_SIZE            128

#include <stdio.h>
//#define DEBUG(...) printf(__VA_ARGS__) 
//#define DEBUG_FINAL(...) printf(__VA_ARGS__) 
//#define DEBUG_SEARCH(...) printf(__VA_ARGS__) 
//#define DEBUG_LEVEL_COUNTER DEBUG("%s::%d, levelCounter::%d, value::%d\r\n", "JS_ARRAY_OR_OBJECT", IS_OBJECT, levelCounter, isArrayOrObject[levelCounter/sizeof(int)]);
#define DEBUG_LEVEL_COUNTER 
#define DEBUG(...)
#define DEBUG_SEARCH(...)
#define DEBUG_FINAL(...)

#define SET_OBJECT isArrayOrObject[++levelCounter/sizeof(int)] |=  (1<<(levelCounter%sizeof(int)))
#define SET_ARRAY  isArrayOrObject[++levelCounter/sizeof(int)] &= ~(1<<(levelCounter%sizeof(int)))
#define IS_OBJECT  isArrayOrObject[levelCounter/sizeof(int)]    &  (1<<(levelCounter%sizeof(int)))
#define UNSET      levelCounter--


#define MAP_BEAUTIFY \
BTS_EXIT, 	/*0  NUL (null)                                */\
BTS_EXIT, 	/*1  SOH (start of heading)                    */\
BTS_EXIT, 	/*2  STX (start of text)                       */\
BTS_EXIT, 	/*3  ETX (end of text)                         */\
BTS_EXIT, 	/*4  EOT (end of transmission)                 */\
BTS_EXIT, 	/*5  ENQ (enquiry)                             */\
BTS_EXIT, 	/*6  ACK (acknowledge)                         */\
BTS_EXIT, 	/*7  BEL (bell)                                */\
BTS_EXIT, 	/*8  BS  (backspace)                           */\
BTS_BEAUTIFY, 	/*9  TAB (horizontal tab)                      */\
BTS_BEAUTIFY, 	/*10  LF  (NL line feed, new line)             */\
BTS_EXIT, 	/*11  VT  (vertical tab)                       */\
BTS_EXIT, 	/*12  FF  (NP form feed, new page)             */\
BTS_BEAUTIFY, 	/*13  CR  (carriage return)                    */\
BTS_EXIT, 	/*14  SO  (shift out)                          */\
BTS_EXIT, 	/*15  SI  (shift in)                           */\
BTS_EXIT, 	/*16  DLE (data link escape)                   */\
BTS_EXIT, 	/*17  DC1 (device control 1)                   */\
BTS_EXIT, 	/*18  DC2 (device control 2)                   */\
BTS_EXIT, 	/*19  DC3 (device control 3)                   */\
BTS_EXIT, 	/*20  DC4 (device control 4)                   */\
BTS_EXIT, 	/*21  NAK (negative acknowledge)               */\
BTS_EXIT, 	/*22  SYN (synchronous idle)                   */\
BTS_EXIT, 	/*23  ETB (end of trans. block)                */\
BTS_EXIT, 	/*24  CAN (cancel)                             */\
BTS_EXIT, 	/*25  EM  (end of medium)                      */\
BTS_EXIT, 	/*26  SUB (substitute)                         */\
BTS_EXIT, 	/*27  ESC (escape)                             */\
BTS_EXIT, 	/*28  FS  (file separator)                     */\
BTS_EXIT, 	/*29  GS  (group separator)                    */\
BTS_EXIT, 	/*30  RS  (record separator)                   */\
BTS_EXIT, 	/*31  US  (unit separator)                     */\
BTS_BEAUTIFY,/*32  SPACE                                    */\
BTS_EXIT, 	/*33  !                                        */\
BTS_EXIT, 	/*34  "                                        */\
BTS_EXIT, 	/*35  #                                        */\
BTS_EXIT, 	/*36  $                                        */\
BTS_EXIT, 	/*37  %                                        */\
BTS_EXIT, 	/*38  &                                        */\
BTS_EXIT, 	/*39  '                                        */\
BTS_EXIT, 	/*40  (                                        */\
BTS_EXIT, 	/*41  )                                        */\
BTS_EXIT, 	/*42  *                                        */\
BTS_EXIT, 	/*43  +                                        */\
BTS_EXIT, 	/*44  ,                                        */\
BTS_EXIT, 	/*45  -                                        */\
BTS_EXIT, 	/*46  .                                        */\
BTS_EXIT, 	/*47  /                                        */\
BTS_EXIT, 	/*48  0                                        */\
BTS_EXIT, 	/*49  1                                        */\
BTS_EXIT, 	/*50  2                                        */\
BTS_EXIT, 	/*51  3                                        */\
BTS_EXIT, 	/*52  4                                        */\
BTS_EXIT, 	/*53  5                                        */\
BTS_EXIT, 	/*54  6                                        */\
BTS_EXIT, 	/*55  7                                        */\
BTS_EXIT, 	/*56  8                                        */\
BTS_EXIT, 	/*57  9                                        */\
BTS_EXIT, 	/*58  :                                        */\
BTS_EXIT, 	/*59  ;                                        */\
BTS_EXIT, 	/*60  <                                        */\
BTS_EXIT, 	/*61  =                                        */\
BTS_EXIT, 	/*62  >                                        */\
BTS_EXIT, 	/*63  ?                                        */\
BTS_EXIT, 	/*64  @                                        */\
BTS_EXIT, 	/*65  A                                        */\
BTS_EXIT, 	/*66  B                                        */\
BTS_EXIT, 	/*67  C                                        */\
BTS_EXIT, 	/*68  D                                        */\
BTS_EXIT, 	/*69  E                                        */\
BTS_EXIT, 	/*70  F                                        */\
BTS_EXIT, 	/*71  G                                        */\
BTS_EXIT, 	/*72  H                                        */\
BTS_EXIT, 	/*73  I                                        */\
BTS_EXIT, 	/*74  J                                        */\
BTS_EXIT, 	/*75  K                                        */\
BTS_EXIT, 	/*76  L                                        */\
BTS_EXIT, 	/*77  M                                        */\
BTS_EXIT, 	/*78  N                                        */\
BTS_EXIT, 	/*79  O                                        */\
BTS_EXIT, 	/*80  P                                        */\
BTS_EXIT, 	/*81  Q                                        */\
BTS_EXIT, 	/*82  R                                        */\
BTS_EXIT, 	/*83  S                                        */\
BTS_EXIT, 	/*84  T                                        */\
BTS_EXIT, 	/*85  U                                        */\
BTS_EXIT, 	/*86  V                                        */\
BTS_EXIT, 	/*87  W                                        */\
BTS_EXIT, 	/*88  X                                        */\
BTS_EXIT, 	/*89  Y                                        */\
BTS_EXIT, 	/*90  Z                                        */\
BTS_EXIT, 	/*91  [                                        */\
BTS_EXIT, 	/*92  \                                        */\
BTS_EXIT, 	/*93  ]                                        */\
BTS_EXIT, 	/*94  ^                                        */\
BTS_EXIT, 	/*95  _                                        */\
BTS_EXIT, 	/*96  `                                        */\
BTS_EXIT, 	/*97  a                                        */\
BTS_EXIT, 	/*98  b                                        */\
BTS_EXIT, 	/*99  c                                        */\
BTS_EXIT, 	/*100  d                                       */\
BTS_EXIT, 	/*101  e                                       */\
BTS_EXIT, 	/*102  f                                       */\
BTS_EXIT, 	/*103  g                                       */\
BTS_EXIT, 	/*104  h                                       */\
BTS_EXIT, 	/*105  i                                       */\
BTS_EXIT, 	/*106  j                                       */\
BTS_EXIT, 	/*107  k                                       */\
BTS_EXIT, 	/*108  l                                       */\
BTS_EXIT, 	/*109  m                                       */\
BTS_EXIT, 	/*110  n                                       */\
BTS_EXIT, 	/*111  o                                       */\
BTS_EXIT, 	/*112  p                                       */\
BTS_EXIT, 	/*113  q                                       */\
BTS_EXIT, 	/*114  r                                       */\
BTS_EXIT, 	/*115  s                                       */\
BTS_EXIT, 	/*116  t                                       */\
BTS_EXIT, 	/*117  u                                       */\
BTS_EXIT, 	/*118  v                                       */\
BTS_EXIT, 	/*119  w                                       */\
BTS_EXIT, 	/*120  x                                       */\
BTS_EXIT, 	/*121  y                                       */\
BTS_EXIT, 	/*122  z                                       */\
BTS_EXIT, 	/*123  {                                       */\
BTS_EXIT, 	/*124  |                                       */\
BTS_EXIT, 	/*125  }                                       */\
BTS_EXIT, 	/*126  ~                                       */\
BTS_EXIT	/*127  DEL                                     */

#define MAP_TOKEN_STS_TOKEN \
STS_ERROR, 		/*0  NUL (null)                                */\
STS_TOKEN, 		/*1  SOH (start of heading)                    */\
STS_TOKEN, 		/*2  STX (start of text)                       */\
STS_TOKEN, 		/*3  ETX (end of text)                         */\
STS_TOKEN, 		/*4  EOT (end of transmission)                 */\
STS_TOKEN, 		/*5  ENQ (enquiry)                             */\
STS_TOKEN, 		/*6  ACK (acknowledge)                         */\
STS_TOKEN, 		/*7  BEL (bell)                                */\
STS_TOKEN, 		/*8  BS  (backspace)                           */\
STS_TOKEN, 		/*9  TAB (horizontal tab)                      */\
STS_TOKEN, 		/*10  LF  (NL line feed, new line)             */\
STS_TOKEN, 		/*11  VT  (vertical tab)                       */\
STS_TOKEN, 		/*12  FF  (NP form feed, new page)             */\
STS_TOKEN, 		/*13  CR  (carriage return)                    */\
STS_TOKEN, 		/*14  SO  (shift out)                          */\
STS_TOKEN, 		/*15  SI  (shift in)                           */\
STS_TOKEN, 		/*16  DLE (data link escape)                   */\
STS_TOKEN, 		/*17  DC1 (device control 1)                   */\
STS_TOKEN, 		/*18  DC2 (device control 2)                   */\
STS_TOKEN, 		/*19  DC3 (device control 3)                   */\
STS_TOKEN, 		/*20  DC4 (device control 4)                   */\
STS_TOKEN, 		/*21  NAK (negative acknowledge)               */\
STS_TOKEN, 		/*22  SYN (synchronous idle)                   */\
STS_TOKEN, 		/*23  ETB (end of trans. block)                */\
STS_TOKEN, 		/*24  CAN (cancel)                             */\
STS_TOKEN, 		/*25  EM  (end of medium)                      */\
STS_TOKEN, 		/*26  SUB (substitute)                         */\
STS_TOKEN, 		/*27  ESC (escape)                             */\
STS_TOKEN, 		/*28  FS  (file separator)                     */\
STS_TOKEN, 		/*29  GS  (group separator)                    */\
STS_TOKEN, 		/*30  RS  (record separator)                   */\
STS_TOKEN, 		/*31  US  (unit separator)                     */\
STS_TOKEN,		/*32  SPACE                                    */\
STS_TOKEN, 		/*33  !                                        */\
STS_EXIT, 		/*34  "                                        */\
STS_TOKEN, 		/*35  #                                        */\
STS_TOKEN, 		/*36  $                                        */\
STS_TOKEN, 		/*37  %                                        */\
STS_TOKEN, 		/*38  &                                        */\
STS_TOKEN, 		/*39  '                                        */\
STS_TOKEN, 		/*40  (                                        */\
STS_TOKEN, 		/*41  )                                        */\
STS_TOKEN, 		/*42  *                                        */\
STS_TOKEN, 		/*43  +                                        */\
STS_TOKEN, 		/*44  ,                                        */\
STS_TOKEN, 		/*45  -                                        */\
STS_TOKEN, 		/*46  .                                        */\
STS_TOKEN, 		/*47  /                                        */\
STS_TOKEN, 		/*48  0                                        */\
STS_TOKEN, 		/*49  1                                        */\
STS_TOKEN, 		/*50  2                                        */\
STS_TOKEN, 		/*51  3                                        */\
STS_TOKEN, 		/*52  4                                        */\
STS_TOKEN, 		/*53  5                                        */\
STS_TOKEN, 		/*54  6                                        */\
STS_TOKEN, 		/*55  7                                        */\
STS_TOKEN, 		/*56  8                                        */\
STS_TOKEN, 		/*57  9                                        */\
STS_TOKEN, 		/*58  :                                        */\
STS_TOKEN, 		/*59  ;                                        */\
STS_TOKEN, 		/*60  <                                        */\
STS_TOKEN, 		/*61  =                                        */\
STS_TOKEN, 		/*62  >                                        */\
STS_TOKEN, 		/*63  ?                                        */\
STS_TOKEN, 		/*64  @                                        */\
STS_TOKEN, 		/*65  A                                        */\
STS_TOKEN, 		/*66  B                                        */\
STS_TOKEN, 		/*67  C                                        */\
STS_TOKEN, 		/*68  D                                        */\
STS_TOKEN, 		/*69  E                                        */\
STS_TOKEN, 		/*70  F                                        */\
STS_TOKEN, 		/*71  G                                        */\
STS_TOKEN, 		/*72  H                                        */\
STS_TOKEN, 		/*73  I                                        */\
STS_TOKEN, 		/*74  J                                        */\
STS_TOKEN, 		/*75  K                                        */\
STS_TOKEN, 		/*76  L                                        */\
STS_TOKEN, 		/*77  M                                        */\
STS_TOKEN, 		/*78  N                                        */\
STS_TOKEN, 		/*79  O                                        */\
STS_TOKEN, 		/*80  P                                        */\
STS_TOKEN, 		/*81  Q                                        */\
STS_TOKEN, 		/*82  R                                        */\
STS_TOKEN, 		/*83  S                                        */\
STS_TOKEN, 		/*84  T                                        */\
STS_TOKEN, 		/*85  U                                        */\
STS_TOKEN, 		/*86  V                                        */\
STS_TOKEN, 		/*87  W                                        */\
STS_TOKEN, 		/*88  X                                        */\
STS_TOKEN, 		/*89  Y                                        */\
STS_TOKEN, 		/*90  Z                                        */\
STS_TOKEN, 		/*91  [                                        */\
STS_ESCAPE,		/*92  \                                        */\
STS_TOKEN, 		/*93  ]                                        */\
STS_TOKEN, 		/*94  ^                                        */\
STS_TOKEN, 		/*95  _                                        */\
STS_TOKEN, 		/*96  `                                        */\
STS_TOKEN, 		/*97  a                                        */\
STS_TOKEN, 		/*98  b                                        */\
STS_TOKEN, 		/*99  c                                        */\
STS_TOKEN, 		/*100  d                                       */\
STS_TOKEN, 		/*101  e                                       */\
STS_TOKEN, 		/*102  f                                       */\
STS_TOKEN, 		/*103  g                                       */\
STS_TOKEN, 		/*104  h                                       */\
STS_TOKEN, 		/*105  i                                       */\
STS_TOKEN, 		/*106  j                                       */\
STS_TOKEN, 		/*107  k                                       */\
STS_TOKEN, 		/*108  l                                       */\
STS_TOKEN, 		/*109  m                                       */\
STS_TOKEN, 		/*110  n                                       */\
STS_TOKEN, 		/*111  o                                       */\
STS_TOKEN, 		/*112  p                                       */\
STS_TOKEN, 		/*113  q                                       */\
STS_TOKEN, 		/*114  r                                       */\
STS_TOKEN, 		/*115  s                                       */\
STS_TOKEN, 		/*116  t                                       */\
STS_TOKEN, 		/*117  u                                       */\
STS_TOKEN, 		/*118  v                                       */\
STS_TOKEN, 		/*119  w                                       */\
STS_TOKEN, 		/*120  x                                       */\
STS_TOKEN, 		/*121  y                                       */\
STS_TOKEN, 		/*122  z                                       */\
STS_TOKEN, 		/*123  {                                       */\
STS_TOKEN, 		/*124  |                                       */\
STS_TOKEN, 		/*125  }                                       */\
STS_TOKEN, 		/*126  ~                                       */\
STS_TOKEN		/*127  DEL                                     */

#define MAP_TOKEN_STS_ESCAPE \
STS_ERROR, 		/*0  NUL (null)                                */\
STS_ERROR, 		/*1  SOH (start of heading)                    */\
STS_ERROR, 		/*2  STX (start of text)                       */\
STS_ERROR, 		/*3  ETX (end of text)                         */\
STS_ERROR, 		/*4  EOT (end of transmission)                 */\
STS_ERROR, 		/*5  ENQ (enquiry)                             */\
STS_ERROR, 		/*6  ACK (acknowledge)                         */\
STS_ERROR, 		/*7  BEL (bell)                                */\
STS_ERROR, 		/*8  BS  (backspace)                           */\
STS_ERROR, 		/*9  TAB (horizontal tab)                      */\
STS_ERROR, 		/*10  LF  (NL line feed, new line)             */\
STS_ERROR, 		/*11  VT  (vertical tab)                       */\
STS_ERROR, 		/*12  FF  (NP form feed, new page)             */\
STS_ERROR, 		/*13  CR  (carriage return)                    */\
STS_ERROR, 		/*14  SO  (shift out)                          */\
STS_ERROR, 		/*15  SI  (shift in)                           */\
STS_ERROR, 		/*16  DLE (data link escape)                   */\
STS_ERROR, 		/*17  DC1 (device control 1)                   */\
STS_ERROR, 		/*18  DC2 (device control 2)                   */\
STS_ERROR, 		/*19  DC3 (device control 3)                   */\
STS_ERROR, 		/*20  DC4 (device control 4)                   */\
STS_ERROR, 		/*21  NAK (negative acknowledge)               */\
STS_ERROR, 		/*22  SYN (synchronous idle)                   */\
STS_ERROR, 		/*23  ETB (end of trans. block)                */\
STS_ERROR, 		/*24  CAN (cancel)                             */\
STS_ERROR, 		/*25  EM  (end of medium)                      */\
STS_ERROR, 		/*26  SUB (substitute)                         */\
STS_ERROR, 		/*27  ESC (escape)                             */\
STS_ERROR, 		/*28  FS  (file separator)                     */\
STS_ERROR, 		/*29  GS  (group separator)                    */\
STS_ERROR, 		/*30  RS  (record separator)                   */\
STS_ERROR, 		/*31  US  (unit separator)                     */\
STS_ERROR,		/*32  SPACE                                    */\
STS_ERROR, 		/*33  !                                        */\
STS_TOKEN, 		/*34  "                                        */\
STS_ERROR, 		/*35  #                                        */\
STS_ERROR, 		/*36  $                                        */\
STS_ERROR, 		/*37  %                                        */\
STS_ERROR, 		/*38  &                                        */\
STS_ERROR, 		/*39  '                                        */\
STS_ERROR, 		/*40  (                                        */\
STS_ERROR, 		/*41  )                                        */\
STS_ERROR, 		/*42  *                                        */\
STS_ERROR, 		/*43  +                                        */\
STS_ERROR, 		/*44  ,                                        */\
STS_ERROR, 		/*45  -                                        */\
STS_ERROR, 		/*46  .                                        */\
STS_TOKEN, 		/*47  /                                        */\
STS_ERROR, 		/*48  0                                        */\
STS_ERROR, 		/*49  1                                        */\
STS_ERROR, 		/*50  2                                        */\
STS_ERROR, 		/*51  3                                        */\
STS_ERROR, 		/*52  4                                        */\
STS_ERROR, 		/*53  5                                        */\
STS_ERROR, 		/*54  6                                        */\
STS_ERROR, 		/*55  7                                        */\
STS_ERROR, 		/*56  8                                        */\
STS_ERROR, 		/*57  9                                        */\
STS_ERROR, 		/*58  :                                        */\
STS_ERROR, 		/*59  ;                                        */\
STS_ERROR, 		/*60  <                                        */\
STS_ERROR, 		/*61  =                                        */\
STS_ERROR, 		/*62  >                                        */\
STS_ERROR, 		/*63  ?                                        */\
STS_ERROR, 		/*64  @                                        */\
STS_ERROR, 		/*65  A                                        */\
STS_ERROR, 		/*66  B                                        */\
STS_ERROR, 		/*67  C                                        */\
STS_ERROR, 		/*68  D                                        */\
STS_ERROR, 		/*69  E                                        */\
STS_ERROR, 		/*70  F                                        */\
STS_ERROR, 		/*71  G                                        */\
STS_ERROR, 		/*72  H                                        */\
STS_ERROR, 		/*73  I                                        */\
STS_ERROR, 		/*74  J                                        */\
STS_ERROR, 		/*75  K                                        */\
STS_ERROR, 		/*76  L                                        */\
STS_ERROR, 		/*77  M                                        */\
STS_ERROR, 		/*78  N                                        */\
STS_ERROR, 		/*79  O                                        */\
STS_ERROR, 		/*80  P                                        */\
STS_ERROR, 		/*81  Q                                        */\
STS_ERROR, 		/*82  R                                        */\
STS_ERROR, 		/*83  S                                        */\
STS_ERROR, 		/*84  T                                        */\
STS_ERROR, 		/*85  U                                        */\
STS_ERROR, 		/*86  V                                        */\
STS_ERROR, 		/*87  W                                        */\
STS_ERROR, 		/*88  X                                        */\
STS_ERROR, 		/*89  Y                                        */\
STS_ERROR, 		/*90  Z                                        */\
STS_ERROR, 		/*91  [                                        */\
STS_TOKEN,		/*92  \                                        */\
STS_ERROR, 		/*93  ]                                        */\
STS_ERROR, 		/*94  ^                                        */\
STS_ERROR, 		/*95  _                                        */\
STS_ERROR, 		/*96  `                                        */\
STS_ERROR, 		/*97  a                                        */\
STS_TOKEN, 		/*98  b                                        */\
STS_ERROR, 		/*99  c                                        */\
STS_ERROR, 		/*100  d                                       */\
STS_ERROR, 		/*101  e                                       */\
STS_TOKEN, 		/*102  f                                       */\
STS_ERROR, 		/*103  g                                       */\
STS_ERROR, 		/*104  h                                       */\
STS_ERROR, 		/*105  i                                       */\
STS_ERROR, 		/*106  j                                       */\
STS_ERROR, 		/*107  k                                       */\
STS_ERROR, 		/*108  l                                       */\
STS_ERROR, 		/*109  m                                       */\
STS_TOKEN, 		/*110  n                                       */\
STS_ERROR, 		/*111  o                                       */\
STS_ERROR, 		/*112  p                                       */\
STS_ERROR, 		/*113  q                                       */\
STS_TOKEN, 		/*114  r                                       */\
STS_ERROR, 		/*115  s                                       */\
STS_TOKEN, 		/*116  t                                       */\
STS_ERROR, 		/*117  u                                       */\
STS_ERROR, 		/*118  v                                       */\
STS_ERROR, 		/*119  w                                       */\
STS_ERROR, 		/*120  x                                       */\
STS_ERROR, 		/*121  y                                       */\
STS_ERROR, 		/*122  z                                       */\
STS_ERROR, 		/*123  {                                       */\
STS_ERROR, 		/*124  |                                       */\
STS_ERROR, 		/*125  }                                       */\
STS_ERROR, 		/*126  ~                                       */\
STS_ERROR		/*127  DEL                                     */

#define MAP_VALUE_TYPE_SEARCH \
VTS_ERROR, 		/*0  NUL (null)                                */\
VTS_ERROR, 		/*1  SOH (start of heading)                    */\
VTS_ERROR, 		/*2  STX (start of text)                       */\
VTS_ERROR, 		/*3  ETX (end of text)                         */\
VTS_ERROR, 		/*4  EOT (end of transmission)                 */\
VTS_ERROR, 		/*5  ENQ (enquiry)                             */\
VTS_ERROR, 		/*6  ACK (acknowledge)                         */\
VTS_ERROR, 		/*7  BEL (bell)                                */\
VTS_ERROR, 		/*8  BS  (backspace)                           */\
VTS_ERROR, 		/*9  TAB (horizontal tab)                      */\
VTS_ERROR, 		/*10  LF  (NL line feed, new line)             */\
VTS_ERROR, 		/*11  VT  (vertical tab)                       */\
VTS_ERROR, 		/*12  FF  (NP form feed, new page)             */\
VTS_ERROR, 		/*13  CR  (carriage return)                    */\
VTS_ERROR, 		/*14  SO  (shift out)                          */\
VTS_ERROR, 		/*15  SI  (shift in)                           */\
VTS_ERROR, 		/*16  DLE (data link escape)                   */\
VTS_ERROR, 		/*17  DC1 (device control 1)                   */\
VTS_ERROR, 		/*18  DC2 (device control 2)                   */\
VTS_ERROR, 		/*19  DC3 (device control 3)                   */\
VTS_ERROR, 		/*20  DC4 (device control 4)                   */\
VTS_ERROR, 		/*21  NAK (negative acknowledge)               */\
VTS_ERROR, 		/*22  SYN (synchronous idle)                   */\
VTS_ERROR, 		/*23  ETB (end of trans. block)                */\
VTS_ERROR, 		/*24  CAN (cancel)                             */\
VTS_ERROR, 		/*25  EM  (end of medium)                      */\
VTS_ERROR, 		/*26  SUB (substitute)                         */\
VTS_ERROR, 		/*27  ESC (escape)                             */\
VTS_ERROR, 		/*28  FS  (file separator)                     */\
VTS_ERROR, 		/*29  GS  (group separator)                    */\
VTS_ERROR, 		/*30  RS  (record separator)                   */\
VTS_ERROR, 		/*31  US  (unit separator)                     */\
VTS_ERROR,		/*32  SPACE                                    */\
VTS_ERROR, 		/*33  !                                        */\
VTS_STRING, 		/*34  "                                        */\
VTS_ERROR, 		/*35  #                                        */\
VTS_ERROR, 		/*36  $                                        */\
VTS_ERROR, 		/*37  %                                        */\
VTS_ERROR, 		/*38  &                                        */\
VTS_ERROR, 		/*39  '                                        */\
VTS_ERROR, 		/*40  (                                        */\
VTS_ERROR, 		/*41  )                                        */\
VTS_ERROR, 		/*42  *                                        */\
VTS_ERROR, 		/*43  +                                        */\
VTS_ERROR, 		/*44  ,                                        */\
VTS_NUMBER, 		/*45  -                                        */\
VTS_NUMBER, 		/*46  .                                        */\
VTS_ERROR, 		/*47  /                                        */\
VTS_NUMBER, 		/*48  0                                        */\
VTS_NUMBER, 		/*49  1                                        */\
VTS_NUMBER, 		/*50  2                                        */\
VTS_NUMBER, 		/*51  3                                        */\
VTS_NUMBER, 		/*52  4                                        */\
VTS_NUMBER, 		/*53  5                                        */\
VTS_NUMBER, 		/*54  6                                        */\
VTS_NUMBER, 		/*55  7                                        */\
VTS_NUMBER, 		/*56  8                                        */\
VTS_NUMBER, 		/*57  9                                        */\
VTS_ERROR, 		/*58  :                                        */\
VTS_ERROR, 		/*59  ;                                        */\
VTS_ERROR, 		/*60  <                                        */\
VTS_ERROR, 		/*61  =                                        */\
VTS_ERROR, 		/*62  >                                        */\
VTS_ERROR, 		/*63  ?                                        */\
VTS_ERROR, 		/*64  @                                        */\
VTS_ERROR, 		/*65  A                                        */\
VTS_ERROR, 		/*66  B                                        */\
VTS_ERROR, 		/*67  C                                        */\
VTS_ERROR, 		/*68  D                                        */\
VTS_ERROR, 		/*69  E                                        */\
VTS_ERROR, 		/*70  F                                        */\
VTS_ERROR, 		/*71  G                                        */\
VTS_ERROR, 		/*72  H                                        */\
VTS_ERROR, 		/*73  I                                        */\
VTS_ERROR, 		/*74  J                                        */\
VTS_ERROR, 		/*75  K                                        */\
VTS_ERROR, 		/*76  L                                        */\
VTS_ERROR, 		/*77  M                                        */\
VTS_ERROR, 		/*78  N                                        */\
VTS_ERROR, 		/*79  O                                        */\
VTS_ERROR, 		/*80  P                                        */\
VTS_ERROR, 		/*81  Q                                        */\
VTS_ERROR, 		/*82  R                                        */\
VTS_ERROR, 		/*83  S                                        */\
VTS_ERROR, 		/*84  T                                        */\
VTS_ERROR, 		/*85  U                                        */\
VTS_ERROR, 		/*86  V                                        */\
VTS_ERROR, 		/*87  W                                        */\
VTS_ERROR, 		/*88  X                                        */\
VTS_ERROR, 		/*89  Y                                        */\
VTS_ERROR, 		/*90  Z                                        */\
VTS_ARRAY, 		/*91  [                                        */\
VTS_ERROR,		/*92  \                                        */\
VTS_ERROR, 		/*93  ]                                        */\
VTS_ERROR, 		/*94  ^                                        */\
VTS_ERROR, 		/*95  _                                        */\
VTS_ERROR, 		/*96  `                                        */\
VTS_ERROR, 		/*97  a                                        */\
VTS_ERROR, 		/*98  b                                        */\
VTS_ERROR, 		/*99  c                                        */\
VTS_ERROR, 		/*100  d                                       */\
VTS_ERROR, 		/*101  e                                       */\
VTS_BOOL_FALSE,	/*102  f                                       */\
VTS_ERROR, 		/*103  g                                       */\
VTS_ERROR, 		/*104  h                                       */\
VTS_ERROR, 		/*105  i                                       */\
VTS_ERROR, 		/*106  j                                       */\
VTS_ERROR, 		/*107  k                                       */\
VTS_ERROR, 		/*108  l                                       */\
VTS_ERROR, 		/*109  m                                       */\
VTS_NULL, 		/*110  n                                       */\
VTS_ERROR, 		/*111  o                                       */\
VTS_ERROR, 		/*112  p                                       */\
VTS_ERROR, 		/*113  q                                       */\
VTS_ERROR, 		/*114  r                                       */\
VTS_ERROR, 		/*115  s                                       */\
VTS_BOOL_TRUE,	/*116  t                                       */\
VTS_ERROR, 		/*117  u                                       */\
VTS_ERROR, 		/*118  v                                       */\
VTS_ERROR, 		/*119  w                                       */\
VTS_ERROR, 		/*120  x                                       */\
VTS_ERROR, 		/*121  y                                       */\
VTS_ERROR, 		/*122  z                                       */\
VTS_OBJECT, 		/*123  {                                       */\
VTS_ERROR, 		/*124  |                                       */\
VTS_ERROR, 		/*125  }                                       */\
VTS_ERROR, 		/*126  ~                                       */\
VTS_ERROR			/*127  DEL                                     */

#define MAP_NUMBER_TOKEN \
NTS_ERROR, 		/*0  NUL (null)                                */\
NTS_ERROR, 		/*1  SOH (start of heading)                    */\
NTS_ERROR, 		/*2  STX (start of text)                       */\
NTS_ERROR, 		/*3  ETX (end of text)                         */\
NTS_ERROR, 		/*4  EOT (end of transmission)                 */\
NTS_ERROR, 		/*5  ENQ (enquiry)                             */\
NTS_ERROR, 		/*6  ACK (acknowledge)                         */\
NTS_ERROR, 		/*7  BEL (bell)                                */\
NTS_ERROR, 		/*8  BS  (backspace)                           */\
NTS_ERROR, 		/*9  TAB (horizontal tab)                      */\
NTS_ERROR, 		/*10  LF  (NL line feed, new line)             */\
NTS_ERROR, 		/*11  VT  (vertical tab)                       */\
NTS_ERROR, 		/*12  FF  (NP form feed, new page)             */\
NTS_ERROR, 		/*13  CR  (carriage return)                    */\
NTS_ERROR, 		/*14  SO  (shift out)                          */\
NTS_ERROR, 		/*15  SI  (shift in)                           */\
NTS_ERROR, 		/*16  DLE (data link escape)                   */\
NTS_ERROR, 		/*17  DC1 (device control 1)                   */\
NTS_ERROR, 		/*18  DC2 (device control 2)                   */\
NTS_ERROR, 		/*19  DC3 (device control 3)                   */\
NTS_ERROR, 		/*20  DC4 (device control 4)                   */\
NTS_ERROR, 		/*21  NAK (negative acknowledge)               */\
NTS_ERROR, 		/*22  SYN (synchronous idle)                   */\
NTS_ERROR, 		/*23  ETB (end of trans. block)                */\
NTS_ERROR, 		/*24  CAN (cancel)                             */\
NTS_ERROR, 		/*25  EM  (end of medium)                      */\
NTS_ERROR, 		/*26  SUB (substitute)                         */\
NTS_ERROR, 		/*27  ESC (escape)                             */\
NTS_ERROR, 		/*28  FS  (file separator)                     */\
NTS_ERROR, 		/*29  GS  (group separator)                    */\
NTS_ERROR, 		/*30  RS  (record separator)                   */\
NTS_ERROR, 		/*31  US  (unit separator)                     */\
NTS_ERROR,		/*32  SPACE                                    */\
NTS_ERROR, 		/*33  !                                        */\
NTS_ERROR, 		/*34  "                                        */\
NTS_ERROR, 		/*35  #                                        */\
NTS_ERROR, 		/*36  $                                        */\
NTS_ERROR, 		/*37  %                                        */\
NTS_ERROR, 		/*38  &                                        */\
NTS_ERROR, 		/*39  '                                        */\
NTS_ERROR, 		/*40  (                                        */\
NTS_ERROR, 		/*41  )                                        */\
NTS_ERROR, 		/*42  *                                        */\
NTS_ERROR, 		/*43  +                                        */\
NTS_ERROR, 		/*44  ,                                        */\
NTS_ERROR, 		/*45  -                                        */\
NTS_DECIMAL, 		/*46  .                                        */\
NTS_ERROR, 		/*47  /                                        */\
NTS_NUMBER, 		/*48  0                                        */\
NTS_NUMBER, 		/*49  1                                        */\
NTS_NUMBER, 		/*50  2                                        */\
NTS_NUMBER, 		/*51  3                                        */\
NTS_NUMBER, 		/*52  4                                        */\
NTS_NUMBER, 		/*53  5                                        */\
NTS_NUMBER, 		/*54  6                                        */\
NTS_NUMBER, 		/*55  7                                        */\
NTS_NUMBER, 		/*56  8                                        */\
NTS_NUMBER, 		/*57  9                                        */\
NTS_ERROR, 		/*58  :                                        */\
NTS_ERROR, 		/*59  ;                                        */\
NTS_ERROR, 		/*60  <                                        */\
NTS_ERROR, 		/*61  =                                        */\
NTS_ERROR, 		/*62  >                                        */\
NTS_ERROR, 		/*63  ?                                        */\
NTS_ERROR, 		/*64  @                                        */\
NTS_ERROR, 		/*65  A                                        */\
NTS_ERROR, 		/*66  B                                        */\
NTS_ERROR, 		/*67  C                                        */\
NTS_ERROR, 		/*68  D                                        */\
NTS_ERROR, 		/*69  E                                        */\
NTS_ERROR, 		/*70  F                                        */\
NTS_ERROR, 		/*71  G                                        */\
NTS_ERROR, 		/*72  H                                        */\
NTS_ERROR, 		/*73  I                                        */\
NTS_ERROR, 		/*74  J                                        */\
NTS_ERROR, 		/*75  K                                        */\
NTS_ERROR, 		/*76  L                                        */\
NTS_ERROR, 		/*77  M                                        */\
NTS_ERROR, 		/*78  N                                        */\
NTS_ERROR, 		/*79  O                                        */\
NTS_ERROR, 		/*80  P                                        */\
NTS_ERROR, 		/*81  Q                                        */\
NTS_ERROR, 		/*82  R                                        */\
NTS_ERROR, 		/*83  S                                        */\
NTS_ERROR, 		/*84  T                                        */\
NTS_ERROR, 		/*85  U                                        */\
NTS_ERROR, 		/*86  V                                        */\
NTS_ERROR, 		/*87  W                                        */\
NTS_ERROR, 		/*88  X                                        */\
NTS_ERROR, 		/*89  Y                                        */\
NTS_ERROR, 		/*90  Z                                        */\
NTS_ERROR, 		/*91  [                                        */\
NTS_ERROR,		/*92  \                                        */\
NTS_ERROR, 		/*93  ]                                        */\
NTS_ERROR, 		/*94  ^                                        */\
NTS_ERROR, 		/*95  _                                        */\
NTS_ERROR, 		/*96  `                                        */\
NTS_ERROR, 		/*97  a                                        */\
NTS_ERROR, 		/*98  b                                        */\
NTS_ERROR, 		/*99  c                                        */\
NTS_ERROR, 		/*100  d                                       */\
NTS_ERROR, 		/*101  e                                       */\
NTS_ERROR,		/*102  f                                       */\
NTS_ERROR, 		/*103  g                                       */\
NTS_ERROR, 		/*104  h                                       */\
NTS_ERROR, 		/*105  i                                       */\
NTS_ERROR, 		/*106  j                                       */\
NTS_ERROR, 		/*107  k                                       */\
NTS_ERROR, 		/*108  l                                       */\
NTS_ERROR, 		/*109  m                                       */\
NTS_ERROR, 		/*110  n                                       */\
NTS_ERROR, 		/*111  o                                       */\
NTS_ERROR, 		/*112  p                                       */\
NTS_ERROR, 		/*113  q                                       */\
NTS_ERROR, 		/*114  r                                       */\
NTS_ERROR, 		/*115  s                                       */\
NTS_ERROR,		/*116  t                                       */\
NTS_ERROR, 		/*117  u                                       */\
NTS_ERROR, 		/*118  v                                       */\
NTS_ERROR, 		/*119  w                                       */\
NTS_ERROR, 		/*120  x                                       */\
NTS_ERROR, 		/*121  y                                       */\
NTS_ERROR, 		/*122  z                                       */\
NTS_ERROR, 		/*123  {                                       */\
NTS_ERROR, 		/*124  |                                       */\
NTS_ERROR, 		/*125  }                                       */\
NTS_ERROR, 		/*126  ~                                       */\
NTS_ERROR			/*127  DEL                                     */
