#include <libpi/value.hpp>
#include <libpi/bool.hpp>
#include <libpi/int.hpp>
#include <libpi/float.hpp>
#include <libpi/string.hpp>
#include <libpi/session.hpp>
#include <libpi/task/link.hpp>
#include <libpi/task/worker.hpp>
#include <thread>
#include <signal.h>
using namespace std;
// HEADERS {{{
#include <iostream>
// }}}
// Value declerations {{{
std::vector<char*> _args;
shared_ptr<libpi::Int> intval_0(new libpi::Int("0"));
shared_ptr<libpi::String> stringval_d41d8cd98f00b204e9800998ecf8427e(new libpi::String(""));
shared_ptr<libpi::Int> intval_1(new libpi::Int("1"));
shared_ptr<libpi::String> label_6a5f1f3d587b4238521a743687b6780f(new libpi::String("^append"));
shared_ptr<libpi::String> label_1da62d864765a583145933d5f61f186b(new libpi::String("^apply"));
shared_ptr<libpi::String> label_c4eb37d961ed515432d3dda9a05c0b8b(new libpi::String("^cons"));
shared_ptr<libpi::String> label_bce51f3c33744c4a6709abcff0f1fa54(new libpi::String("^copy"));
shared_ptr<libpi::String> label_186e35ef3dd9939615c442c30b801a33(new libpi::String("^end"));
shared_ptr<libpi::String> label_d0ff8d7ab3dcf009ed4db0dcac30d36b(new libpi::String("^pop"));
shared_ptr<libpi::String> label_e8d75b609730111d0f0a88f103c3016f(new libpi::String("^erase"));
shared_ptr<libpi::String> label_efcf5b49097e1969bd7fd24fb0c37fe6(new libpi::String("^filter"));
shared_ptr<libpi::String> label_0e5493ddbd005506c18850a500dfea96(new libpi::String("^call"));
shared_ptr<libpi::String> label_c08ee33285bc55f31dfe5248a00e7171(new libpi::String("^val"));
shared_ptr<libpi::String> label_a02d9bff9bc696a25d4e49dd4f301a44(new libpi::String("^err"));
shared_ptr<libpi::String> label_957b92409b25b63db6dbd2b026b7cd2f(new libpi::String("^get"));
shared_ptr<libpi::String> label_12e9fca472d02f299c6b8b606bf0318e(new libpi::String("^insert"));
shared_ptr<libpi::String> label_93fdfd9a3eb61044995b53f81eaaa15f(new libpi::String("^length"));
shared_ptr<libpi::String> label_9334638e4f24e41deb03eaf513559051(new libpi::String("^set"));
shared_ptr<libpi::String> label_cdbea80e30ba932a7d0b51a4202d726a(new libpi::String("^snoc"));
shared_ptr<libpi::String> label_55759b104bd58e2c0ac92ac12ef96ed4(new libpi::String("^takeover"));
shared_ptr<libpi::String> stringval_046e3d1f08decc804b88d3b89cd6b186(new libpi::String("Index out of bounds"));
shared_ptr<libpi::String> label_6e5c72314d264d39de32bf8a473ee1d3(new libpi::String("^nil"));
shared_ptr<libpi::String> label_f693b54352d7c81bcd5f4e46fc8ea4f5(new libpi::String("^leq"));
shared_ptr<libpi::String> label_7a11449475519daf7b8b8621e5b6eacc(new libpi::String("^nl"));
shared_ptr<libpi::String> label_64f10e23bd5ddafc2b83ba3a50254359(new libpi::String("^int"));
shared_ptr<libpi::Int> intval_2(new libpi::Int("2"));
shared_ptr<libpi::Int> intval_20000(new libpi::Int("20000"));
// }}}
// Task Types {{{
class TaskMain : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
};
class Task___SIM__ConsoleSession2 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__f6055;
    shared_ptr<libpi::Value> var___SIM__i6058;
    shared_ptr<libpi::Value> var___SIM__s6049;
    shared_ptr<libpi::Value> var___SIM__str6059;
    shared_ptr<libpi::Value> var___SIM__val6056;
    shared_ptr<libpi::Value> var___SIM__val6057;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6049=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
};
class Task___SIM__Console1 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__s6048;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
};
class Task___SIM__X4 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__lhs6078;
    shared_ptr<libpi::Value> var___SIM__lhs6080;
    shared_ptr<libpi::Value> var___SIM__lhs6082;
    shared_ptr<libpi::Value> var___SIM__lhs6084;
    shared_ptr<libpi::Value> var___SIM__lhs6086;
    shared_ptr<libpi::Value> var___SIM__rhs6079;
    shared_ptr<libpi::Value> var___SIM__rhs6081;
    shared_ptr<libpi::Value> var___SIM__rhs6083;
    shared_ptr<libpi::Value> var___SIM__rhs6085;
    shared_ptr<libpi::Value> var___SIM__rhs6087;
    shared_ptr<libpi::Value> var___SIM__s6062;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6062=val;}
};
class Task___SIM__FlatRight25 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6061;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6061=val;}
};
class Task___SIM__StdOrderInt3 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6061;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__StartService5 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__i6088;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6088=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__X7 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__lhs6107;
    shared_ptr<libpi::Value> var___SIM__lhs6109;
    shared_ptr<libpi::Value> var___SIM__lhs6111;
    shared_ptr<libpi::Value> var___SIM__lhs6113;
    shared_ptr<libpi::Value> var___SIM__lhs6115;
    shared_ptr<libpi::Value> var___SIM__rhs6108;
    shared_ptr<libpi::Value> var___SIM__rhs6110;
    shared_ptr<libpi::Value> var___SIM__rhs6112;
    shared_ptr<libpi::Value> var___SIM__rhs6114;
    shared_ptr<libpi::Value> var___SIM__rhs6116;
    shared_ptr<libpi::Value> var___SIM__s6091;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6091=val;}
};
class Task___SIM__FlatRight26 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6090;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6090=val;}
};
class Task___SIM__StdOrderFloat6 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6090;
    shared_ptr<libpi::Value> var___SIM__stdorder_Float6089;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Float6089=val;}
};
class Task___SIM__StartService8 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__i6117;
    shared_ptr<libpi::Value> var___SIM__stdorder_Float6089;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6117=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Float6089=val;}
};
class Task___SIM__X10 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__lhs6136;
    shared_ptr<libpi::Value> var___SIM__lhs6138;
    shared_ptr<libpi::Value> var___SIM__lhs6140;
    shared_ptr<libpi::Value> var___SIM__lhs6142;
    shared_ptr<libpi::Value> var___SIM__lhs6144;
    shared_ptr<libpi::Value> var___SIM__rhs6137;
    shared_ptr<libpi::Value> var___SIM__rhs6139;
    shared_ptr<libpi::Value> var___SIM__rhs6141;
    shared_ptr<libpi::Value> var___SIM__rhs6143;
    shared_ptr<libpi::Value> var___SIM__rhs6145;
    shared_ptr<libpi::Value> var___SIM__s6120;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6120=val;}
};
class Task___SIM__FlatRight27 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6119;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6119=val;}
};
class Task___SIM__StdOrderString9 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6119;
    shared_ptr<libpi::Value> var___SIM__stdorder_String6118;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_String6118=val;}
};
class Task___SIM__StartService11 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__i6146;
    shared_ptr<libpi::Value> var___SIM__stdorder_String6118;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6146=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_String6118=val;}
};
class Task___SIM__Cons14 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__cpy6254;
    shared_ptr<libpi::Value> var___SIM__f6223;
    shared_ptr<libpi::Value> var___SIM__f6226;
    shared_ptr<libpi::Value> var___SIM__fc6222;
    shared_ptr<libpi::Value> var___SIM__fc6225;
    shared_ptr<libpi::Value> var___SIM__filter6258;
    shared_ptr<libpi::Value> var___SIM__filter6262;
    shared_ptr<libpi::Value> var___SIM__h6224;
    shared_ptr<libpi::Value> var___SIM__h6227;
    shared_ptr<libpi::Value> var___SIM__h6228;
    shared_ptr<libpi::Value> var___SIM__h6256;
    shared_ptr<libpi::Value> var___SIM__h6260;
    shared_ptr<libpi::Value> var___SIM__h6264;
    shared_ptr<libpi::Value> var___SIM__h6272;
    shared_ptr<libpi::Value> var___SIM__h6300;
    shared_ptr<libpi::Value> var___SIM__h6301;
    shared_ptr<libpi::Value> var___SIM__head6173;
    shared_ptr<libpi::Value> var___SIM__i6255;
    shared_ptr<libpi::Value> var___SIM__i6266;
    shared_ptr<libpi::Value> var___SIM__i6271;
    shared_ptr<libpi::Value> var___SIM__i6298;
    shared_ptr<libpi::Value> var___SIM__keep6259;
    shared_ptr<libpi::Value> var___SIM__keep6263;
    shared_ptr<libpi::Value> var___SIM__l16229;
    shared_ptr<libpi::Value> var___SIM__l16273;
    shared_ptr<libpi::Value> var___SIM__l26230;
    shared_ptr<libpi::Value> var___SIM__l26274;
    shared_ptr<libpi::Value> var___SIM__l6220;
    shared_ptr<libpi::Value> var___SIM__l6221;
    shared_ptr<libpi::Value> var___SIM__l6297;
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__msg6267;
    shared_ptr<libpi::Value> var___SIM__msg6268;
    shared_ptr<libpi::Value> var___SIM__t6253;
    shared_ptr<libpi::Value> var___SIM__t6257;
    shared_ptr<libpi::Value> var___SIM__t6261;
    shared_ptr<libpi::Value> var___SIM__t6265;
    shared_ptr<libpi::Value> var___SIM__tail6174;
    shared_ptr<libpi::Value> var___SIM__that6302;
    shared_ptr<libpi::Value> var___SIM__this6175;
    shared_ptr<libpi::Value> var___SIM__v6269;
    shared_ptr<libpi::Value> var___SIM__v6270;
    shared_ptr<libpi::Value> var___SIM__v6299;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__head6173=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__tail6174=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__this6175=val;}
    inline void SetArg3(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
};
class Task___SIM__Nil13 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__cpy6330;
    shared_ptr<libpi::Value> var___SIM__f6304;
    shared_ptr<libpi::Value> var___SIM__filter6332;
    shared_ptr<libpi::Value> var___SIM__head6305;
    shared_ptr<libpi::Value> var___SIM__head6339;
    shared_ptr<libpi::Value> var___SIM__i6331;
    shared_ptr<libpi::Value> var___SIM__i6333;
    shared_ptr<libpi::Value> var___SIM__i6334;
    shared_ptr<libpi::Value> var___SIM__i6337;
    shared_ptr<libpi::Value> var___SIM__l16306;
    shared_ptr<libpi::Value> var___SIM__l16340;
    shared_ptr<libpi::Value> var___SIM__l26307;
    shared_ptr<libpi::Value> var___SIM__l26341;
    shared_ptr<libpi::Value> var___SIM__l6303;
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__tail6336;
    shared_ptr<libpi::Value> var___SIM__that6364;
    shared_ptr<libpi::Value> var___SIM__this6150;
    shared_ptr<libpi::Value> var___SIM__v6335;
    shared_ptr<libpi::Value> var___SIM__v6338;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__this6150=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
};
class Task___SIM__FlatRight28 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__s6148;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__s6148=val;}
};
class Task___SIM__List12 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__s6148;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
};
class Task___SIM__StartService15 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__i6365;
    shared_ptr<libpi::Value> var___SIM__list6147;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6365=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
};
class Task___SIM__FlatRight29 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__cmp6370;
    shared_ptr<libpi::Value> var___SIM__cmp6380;
    shared_ptr<libpi::Value> var___SIM__from6371;
    shared_ptr<libpi::Value> var___SIM__from6381;
    shared_ptr<libpi::Value> var___SIM__leq6373;
    shared_ptr<libpi::Value> var___SIM__leq6383;
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__next6369;
    shared_ptr<libpi::Value> var___SIM__next6379;
    shared_ptr<libpi::Value> var___SIM__range6366;
    shared_ptr<libpi::Value> var___SIM__res6376;
    shared_ptr<libpi::Value> var___SIM__res6377;
    shared_ptr<libpi::Value> var___SIM__res6386;
    shared_ptr<libpi::Value> var___SIM__res6387;
    shared_ptr<libpi::Value> var___SIM__s6367;
    shared_ptr<libpi::Value> var___SIM__succ6374;
    shared_ptr<libpi::Value> var___SIM__succ6384;
    shared_ptr<libpi::Value> var___SIM__tail6375;
    shared_ptr<libpi::Value> var___SIM__tail6385;
    shared_ptr<libpi::Value> var___SIM__to6372;
    shared_ptr<libpi::Value> var___SIM__to6382;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__range6366=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__s6367=val;}
};
class Task___SIM__Range16 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__range6366;
    shared_ptr<libpi::Value> var___SIM__s6367;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__range6366=val;}
};
class Task___SIM__StartService17 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__i6388;
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__range6366;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6388=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__range6366=val;}
};
class Task___SIM__X19 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__n6394;
    shared_ptr<libpi::Value> var___SIM__this6391;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__this6391=val;}
};
class Task___SIM__FlatRight30 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6390;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6390=val;}
};
class Task___SIM__StdSucc_Int18 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__s6390;
    shared_ptr<libpi::Value> var___SIM__stdsucc_Int6389;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__stdsucc_Int6389=val;}
};
class Task___SIM__StartService20 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__i6395;
    shared_ptr<libpi::Value> var___SIM__stdsucc_Int6389;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6395=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdsucc_Int6389=val;}
};
class Task___SIM__Filter22 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__d6398;
    shared_ptr<libpi::Value> var___SIM__n6402;
    shared_ptr<libpi::Value> var___SIM__this6399;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__this6399=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__d6398=val;}
};
class Task___SIM__FlatRight31 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__d6398;
    shared_ptr<libpi::Value> var___SIM__s6397;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__s6397=val;}
};
class Task___SIM__FilterMultipla21 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__filtermultipla6396;
    shared_ptr<libpi::Value> var___SIM__s6397;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__filtermultipla6396=val;}
};
class Task___SIM__StartService23 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__filtermultipla6396;
    shared_ptr<libpi::Value> var___SIM__i6403;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__i6403=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__filtermultipla6396=val;}
};
class Task___SIM__Eratosthenes24 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__c6429;
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__filter6430;
    shared_ptr<libpi::Value> var___SIM__filtermultipla6396;
    shared_ptr<libpi::Value> var___SIM__list6404;
    shared_ptr<libpi::Value> var___SIM__p6427;
    shared_ptr<libpi::Value> var___SIM__tail6428;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__list6404=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__filtermultipla6396=val;}
};
class Task___SIM__FlatRight32 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__filtermultipla6396;
    shared_ptr<libpi::Value> var___SIM__ilist6455;
    shared_ptr<libpi::Value> var___SIM__iorder6457;
    shared_ptr<libpi::Value> var___SIM__isucc6456;
    shared_ptr<libpi::Value> var___SIM__range6366;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    shared_ptr<libpi::Value> var___SIM__stdsucc_Int6389;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__filtermultipla6396=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__range6366=val;}
    inline void SetArg3(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
    inline void SetArg4(const shared_ptr<libpi::Value> &val) {var___SIM__stdsucc_Int6389=val;}
};
class Task___SIM__FlatRight33 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__filtermultipla6396;
    shared_ptr<libpi::Value> var___SIM__range6366;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    shared_ptr<libpi::Value> var___SIM__stdsucc_Int6389;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__range6366=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
    inline void SetArg3(const shared_ptr<libpi::Value> &val) {var___SIM__stdsucc_Int6389=val;}
};
class Task___SIM__FlatRight34 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__range6366;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    shared_ptr<libpi::Value> var___SIM__stdsucc_Int6389;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__range6366=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__FlatRight35 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__range6366;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__list6147=val;}
    inline void SetArg2(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__FlatRight36 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__list6147;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__FlatRight37 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    shared_ptr<libpi::Value> var___SIM__stdorder_String6118;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__FlatRight38 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__stdorder_Float6089;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
    inline void SetArg1(const shared_ptr<libpi::Value> &val) {var___SIM__stdorder_Int6060=val;}
};
class Task___SIM__FlatRight39 : public libpi::task::Task
{ public:
    shared_ptr<libpi::Value> var___SIM__console6047;
    shared_ptr<libpi::Value> var___SIM__stdorder_Int6060;
    inline void SetArg0(const shared_ptr<libpi::Value> &val) {var___SIM__console6047=val;}
};
// }}}
// All Methods {{{
inline bool _methods(shared_ptr<libpi::task::Task> &_task)
{ size_t _steps=0;
  void *_label=_task->GetLabel();
  if (_label!=NULL)
    goto *_label;
  method_Main:
  #define _this ((TaskMain*)_task.get())
  { // Main {{{

    _task->SetLabel(&&__SIM__checkpoint_17713);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17713:

    _this->var___SIM__console6047.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17714);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17714:

    { Task___SIM__FlatRight39 *__SIM__task17715(new Task___SIM__FlatRight39());
      __SIM__task17715->SetWorker(&_task->GetWorker());
      __SIM__task17715->SetArg0(_this->var___SIM__console6047);
      __SIM__task17715->SetLabel(&&method___SIM__FlatRight39);
      shared_ptr<libpi::task::Task> __SIM__task17716(__SIM__task17715);
      _task->GetWorker().AddTask(__SIM__task17716);
    }

    _task->SetLabel(&&__SIM__checkpoint_17718);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17718:

    { Task___SIM__Console1 *__SIM__task17717 = new Task___SIM__Console1();
      __SIM__task17717->SetWorker(&_task->GetWorker());
      __SIM__task17717->SetArg0(_this->var___SIM__console6047);
      __SIM__task17717->SetLabel(&&method___SIM__Console1);
      _task.reset(__SIM__task17717);
    }
    goto method___SIM__Console1;
  } // }}}

/* Procedure implementations */
  method___SIM__ConsoleSession2: // {{{
  #undef _this
  #define _this ((Task___SIM__ConsoleSession2*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16729);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16729:

    _task->SetLabel(&&__SIM__checkpoint___SIM__ConsoleSession216728);
    if (!((libpi::Session*)_this->var___SIM__s6049.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__ConsoleSession216728:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__s6049.get())->Close(true);
    _this->var___SIM__s6049.reset();

    _task->SetLabel(&&__SIM__checkpoint_16731);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16731:

    { Task___SIM__Console1 *__SIM__task16730 = new Task___SIM__Console1();
      __SIM__task16730->SetWorker(&_task->GetWorker());
      __SIM__task16730->SetArg0(_this->var___SIM__console6047);
      __SIM__task16730->SetLabel(&&method___SIM__Console1);
      _task.reset(__SIM__task16730);
    }
    goto method___SIM__Console1;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^flt")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16733);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16733:

    // ~s6049[2] >> ~f6055;
    _task->SetLabel(&&__SIM__receive16732);
    if (!((libpi::Session*)_this->var___SIM__s6049.get())->Receive(1,_task,_this->var___SIM__f6055))
      return false;
    __SIM__receive16732:

    _task->SetLabel(&&__SIM__checkpoint_16736);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16736:

  // HOST STATEMENT BEGIN
    std::cout << ((libpi::Float*) _this->var___SIM__f6055 .get())->ToString() << std::flush;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_16735);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16735:

    { Task___SIM__ConsoleSession2 *__SIM__task16734 = new Task___SIM__ConsoleSession2();
      __SIM__task16734->SetWorker(&_task->GetWorker());
      __SIM__task16734->SetArg0(_this->var___SIM__s6049);
      __SIM__task16734->SetArg1(_this->var___SIM__console6047);
      __SIM__task16734->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16734);
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^input")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16738);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16738:

    _task->SetLabel(&&__SIM__checkpoint___SIM__ConsoleSession216737);
    if (!((libpi::Session*)_this->var___SIM__s6049.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__ConsoleSession216737:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^int")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16739);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16739:

    {
      _this->var___SIM__val6056=intval_0;
    }

    _task->SetLabel(&&__SIM__checkpoint_16744);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16744:

  // HOST STATEMENT BEGIN
    { long _l; std::cin >> _l;  _this->var___SIM__val6056 =shared_ptr<libpi::Int>(new libpi::Int(_l)); }  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_16741);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16741:

    // ~s6049[2] << ~val6056;
    { 
      ((libpi::Session*)_this->var___SIM__s6049.get())->Send(1,_task,_this->var___SIM__val6056);
    }

    _task->SetLabel(&&__SIM__checkpoint_16743);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16743:

    { Task___SIM__ConsoleSession2 *__SIM__task16742 = new Task___SIM__ConsoleSession2();
      __SIM__task16742->SetWorker(&_task->GetWorker());
      __SIM__task16742->SetArg0(_this->var___SIM__s6049);
      __SIM__task16742->SetArg1(_this->var___SIM__console6047);
      __SIM__task16742->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16742);
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^str")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16745);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16745:

    {
      _this->var___SIM__val6057=stringval_d41d8cd98f00b204e9800998ecf8427e;
    }

    _task->SetLabel(&&__SIM__checkpoint_16750);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16750:

  // HOST STATEMENT BEGIN
    { std::string _s; std::cin >> _s;  _this->var___SIM__val6057 =shared_ptr<libpi::String>(new libpi::String(_s)); }  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_16747);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16747:

    // ~s6049[2] << ~val6057;
    { 
      ((libpi::Session*)_this->var___SIM__s6049.get())->Send(1,_task,_this->var___SIM__val6057);
    }

    _task->SetLabel(&&__SIM__checkpoint_16749);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16749:

    { Task___SIM__ConsoleSession2 *__SIM__task16748 = new Task___SIM__ConsoleSession2();
      __SIM__task16748->SetWorker(&_task->GetWorker());
      __SIM__task16748->SetArg0(_this->var___SIM__s6049);
      __SIM__task16748->SetArg1(_this->var___SIM__console6047);
      __SIM__task16748->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16748);
    }
    goto method___SIM__ConsoleSession2;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^int")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16752);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16752:

    // ~s6049[2] >> ~i6058;
    _task->SetLabel(&&__SIM__receive16751);
    if (!((libpi::Session*)_this->var___SIM__s6049.get())->Receive(1,_task,_this->var___SIM__i6058))
      return false;
    __SIM__receive16751:

    _task->SetLabel(&&__SIM__checkpoint_16755);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16755:

  // HOST STATEMENT BEGIN
    std::cout << ((libpi::Int*) _this->var___SIM__i6058 .get())->ToString() << std::flush;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_16754);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16754:

    { Task___SIM__ConsoleSession2 *__SIM__task16753 = new Task___SIM__ConsoleSession2();
      __SIM__task16753->SetWorker(&_task->GetWorker());
      __SIM__task16753->SetArg0(_this->var___SIM__s6049);
      __SIM__task16753->SetArg1(_this->var___SIM__console6047);
      __SIM__task16753->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16753);
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^nl")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16758);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16758:

  // HOST STATEMENT BEGIN
    std::cout << std::endl;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_16757);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16757:

    { Task___SIM__ConsoleSession2 *__SIM__task16756 = new Task___SIM__ConsoleSession2();
      __SIM__task16756->SetWorker(&_task->GetWorker());
      __SIM__task16756->SetArg0(_this->var___SIM__s6049);
      __SIM__task16756->SetArg1(_this->var___SIM__console6047);
      __SIM__task16756->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16756);
    }
    goto method___SIM__ConsoleSession2;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^str")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16760);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16760:

    // ~s6049[2] >> ~str6059;
    _task->SetLabel(&&__SIM__receive16759);
    if (!((libpi::Session*)_this->var___SIM__s6049.get())->Receive(1,_task,_this->var___SIM__str6059))
      return false;
    __SIM__receive16759:

    _task->SetLabel(&&__SIM__checkpoint_16763);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16763:

  // HOST STATEMENT BEGIN
    std::cout << ((libpi::String*) _this->var___SIM__str6059 .get())->GetValue() << std::flush;  // HOST STATEMENT END

    _task->SetLabel(&&__SIM__checkpoint_16762);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16762:

    { Task___SIM__ConsoleSession2 *__SIM__task16761 = new Task___SIM__ConsoleSession2();
      __SIM__task16761->SetWorker(&_task->GetWorker());
      __SIM__task16761->SetArg0(_this->var___SIM__s6049);
      __SIM__task16761->SetArg1(_this->var___SIM__console6047);
      __SIM__task16761->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16761);
    }
    goto method___SIM__ConsoleSession2;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__Console1: // {{{
  #undef _this
  #define _this ((Task___SIM__Console1*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16764);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16764:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__Console116765);
        if (!((libpi::task::Link*)_this->var___SIM__console6047.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__Console116765:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6048.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_16767);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16767:

    { Task___SIM__ConsoleSession2 *__SIM__task16766 = new Task___SIM__ConsoleSession2();
      __SIM__task16766->SetWorker(&_task->GetWorker());
      __SIM__task16766->SetArg0(_this->var___SIM__s6048);
      __SIM__task16766->SetArg1(_this->var___SIM__console6047);
      __SIM__task16766->SetLabel(&&method___SIM__ConsoleSession2);
      _task.reset(__SIM__task16766);
    }
    goto method___SIM__ConsoleSession2;
  } // }}}
  method___SIM__X4: // {{{
  #undef _this
  #define _this ((Task___SIM__X4*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16769);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16769:

    _task->SetLabel(&&__SIM__checkpoint___SIM__X416768);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__X416768:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__s6062.get())->Close(true);
    _this->var___SIM__s6062.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^eq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16771);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16771:

    // ~s6062[2] >> ~lhs6078;
    _task->SetLabel(&&__SIM__receive16770);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__lhs6078))
      return false;
    __SIM__receive16770:

    _task->SetLabel(&&__SIM__checkpoint_16773);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16773:

    // ~s6062[2] >> ~rhs6079;
    _task->SetLabel(&&__SIM__receive16772);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__rhs6079))
      return false;
    __SIM__receive16772:

    _task->SetLabel(&&__SIM__checkpoint_16775);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16775:

    // ~s6062[2] << (~lhs6078 = ~rhs6079);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16776((*((libpi::Int*)_this->var___SIM__lhs6078.get())) == (*((libpi::Int*)_this->var___SIM__rhs6079.get())));
      ((libpi::Session*)_this->var___SIM__s6062.get())->Send(1,_task,__SIM__binop16776);
    }

    _task->SetLabel(&&__SIM__checkpoint_16778);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16778:

    { Task___SIM__X4 *__SIM__task16777 = new Task___SIM__X4();
      __SIM__task16777->SetWorker(&_task->GetWorker());
      __SIM__task16777->SetArg0(_this->var___SIM__s6062);
      __SIM__task16777->SetLabel(&&method___SIM__X4);
      _task.reset(__SIM__task16777);
    }
    goto method___SIM__X4;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^geq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16780);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16780:

    // ~s6062[2] >> ~lhs6080;
    _task->SetLabel(&&__SIM__receive16779);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__lhs6080))
      return false;
    __SIM__receive16779:

    _task->SetLabel(&&__SIM__checkpoint_16782);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16782:

    // ~s6062[2] >> ~rhs6081;
    _task->SetLabel(&&__SIM__receive16781);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__rhs6081))
      return false;
    __SIM__receive16781:

    _task->SetLabel(&&__SIM__checkpoint_16784);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16784:

    // ~s6062[2] << (~lhs6080 >= ~rhs6081);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16785((*((libpi::Int*)_this->var___SIM__lhs6080.get())) >= (*((libpi::Int*)_this->var___SIM__rhs6081.get())));
      ((libpi::Session*)_this->var___SIM__s6062.get())->Send(1,_task,__SIM__binop16785);
    }

    _task->SetLabel(&&__SIM__checkpoint_16787);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16787:

    { Task___SIM__X4 *__SIM__task16786 = new Task___SIM__X4();
      __SIM__task16786->SetWorker(&_task->GetWorker());
      __SIM__task16786->SetArg0(_this->var___SIM__s6062);
      __SIM__task16786->SetLabel(&&method___SIM__X4);
      _task.reset(__SIM__task16786);
    }
    goto method___SIM__X4;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^greater")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16789);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16789:

    // ~s6062[2] >> ~lhs6082;
    _task->SetLabel(&&__SIM__receive16788);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__lhs6082))
      return false;
    __SIM__receive16788:

    _task->SetLabel(&&__SIM__checkpoint_16791);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16791:

    // ~s6062[2] >> ~rhs6083;
    _task->SetLabel(&&__SIM__receive16790);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__rhs6083))
      return false;
    __SIM__receive16790:

    _task->SetLabel(&&__SIM__checkpoint_16793);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16793:

    // ~s6062[2] << (~rhs6083 < ~lhs6082);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16794((*((libpi::Int*)_this->var___SIM__rhs6083.get())) < (*((libpi::Int*)_this->var___SIM__lhs6082.get())));
      ((libpi::Session*)_this->var___SIM__s6062.get())->Send(1,_task,__SIM__binop16794);
    }

    _task->SetLabel(&&__SIM__checkpoint_16796);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16796:

    { Task___SIM__X4 *__SIM__task16795 = new Task___SIM__X4();
      __SIM__task16795->SetWorker(&_task->GetWorker());
      __SIM__task16795->SetArg0(_this->var___SIM__s6062);
      __SIM__task16795->SetLabel(&&method___SIM__X4);
      _task.reset(__SIM__task16795);
    }
    goto method___SIM__X4;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^leq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16798);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16798:

    // ~s6062[2] >> ~lhs6084;
    _task->SetLabel(&&__SIM__receive16797);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__lhs6084))
      return false;
    __SIM__receive16797:

    _task->SetLabel(&&__SIM__checkpoint_16800);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16800:

    // ~s6062[2] >> ~rhs6085;
    _task->SetLabel(&&__SIM__receive16799);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__rhs6085))
      return false;
    __SIM__receive16799:

    _task->SetLabel(&&__SIM__checkpoint_16802);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16802:

    // ~s6062[2] << (~lhs6084 <= ~rhs6085);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16803((*((libpi::Int*)_this->var___SIM__lhs6084.get())) <= (*((libpi::Int*)_this->var___SIM__rhs6085.get())));
      ((libpi::Session*)_this->var___SIM__s6062.get())->Send(1,_task,__SIM__binop16803);
    }

    _task->SetLabel(&&__SIM__checkpoint_16805);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16805:

    { Task___SIM__X4 *__SIM__task16804 = new Task___SIM__X4();
      __SIM__task16804->SetWorker(&_task->GetWorker());
      __SIM__task16804->SetArg0(_this->var___SIM__s6062);
      __SIM__task16804->SetLabel(&&method___SIM__X4);
      _task.reset(__SIM__task16804);
    }
    goto method___SIM__X4;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^less")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16807);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16807:

    // ~s6062[2] >> ~lhs6086;
    _task->SetLabel(&&__SIM__receive16806);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__lhs6086))
      return false;
    __SIM__receive16806:

    _task->SetLabel(&&__SIM__checkpoint_16809);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16809:

    // ~s6062[2] >> ~rhs6087;
    _task->SetLabel(&&__SIM__receive16808);
    if (!((libpi::Session*)_this->var___SIM__s6062.get())->Receive(1,_task,_this->var___SIM__rhs6087))
      return false;
    __SIM__receive16808:

    _task->SetLabel(&&__SIM__checkpoint_16811);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16811:

    // ~s6062[2] << (~lhs6086 < ~rhs6087);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16812((*((libpi::Int*)_this->var___SIM__lhs6086.get())) < (*((libpi::Int*)_this->var___SIM__rhs6087.get())));
      ((libpi::Session*)_this->var___SIM__s6062.get())->Send(1,_task,__SIM__binop16812);
    }

    _task->SetLabel(&&__SIM__checkpoint_16814);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16814:

    { Task___SIM__X4 *__SIM__task16813 = new Task___SIM__X4();
      __SIM__task16813->SetWorker(&_task->GetWorker());
      __SIM__task16813->SetArg0(_this->var___SIM__s6062);
      __SIM__task16813->SetLabel(&&method___SIM__X4);
      _task.reset(__SIM__task16813);
    }
    goto method___SIM__X4;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight25: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight25*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16816);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16816:

    { Task___SIM__X4 *__SIM__task16815 = new Task___SIM__X4();
      __SIM__task16815->SetWorker(&_task->GetWorker());
      __SIM__task16815->SetArg0(_this->var___SIM__s6061);
      __SIM__task16815->SetLabel(&&method___SIM__X4);
      _task.reset(__SIM__task16815);
    }
    goto method___SIM__X4;
  } // }}}
  method___SIM__StdOrderInt3: // {{{
  #undef _this
  #define _this ((Task___SIM__StdOrderInt3*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16817);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16817:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__StdOrderInt316818);
        if (!((libpi::task::Link*)_this->var___SIM__stdorder_Int6060.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__StdOrderInt316818:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6061.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_16819);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16819:

    { Task___SIM__FlatRight25 *__SIM__task16820(new Task___SIM__FlatRight25());
      __SIM__task16820->SetWorker(&_task->GetWorker());
      __SIM__task16820->SetArg0(_this->var___SIM__s6061);
      __SIM__task16820->SetLabel(&&method___SIM__FlatRight25);
      shared_ptr<libpi::task::Task> __SIM__task16821(__SIM__task16820);
      _task->GetWorker().AddTask(__SIM__task16821);
    }

    _task->SetLabel(&&__SIM__checkpoint_16823);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16823:

    { Task___SIM__StdOrderInt3 *__SIM__task16822 = new Task___SIM__StdOrderInt3();
      __SIM__task16822->SetWorker(&_task->GetWorker());
      __SIM__task16822->SetArg0(_this->var___SIM__stdorder_Int6060);
      __SIM__task16822->SetLabel(&&method___SIM__StdOrderInt3);
      _task.reset(__SIM__task16822);
    }
    goto method___SIM__StdOrderInt3;
  } // }}}
  method___SIM__StartService5: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService5*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16826);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16826:

    {
      shared_ptr<libpi::Bool> __SIM__binop16827((*((libpi::Int*)_this->var___SIM__i6088.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop16827.get())->GetValue())
        goto __SIM____SIM__StartService5_true16824;
      else
        goto __SIM____SIM__StartService5_false16825;
    }
    __SIM____SIM__StartService5_true16824:

    _task->SetLabel(&&__SIM__checkpoint_16835);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16835:

    { Task___SIM__StdOrderInt3 *__SIM__task16834 = new Task___SIM__StdOrderInt3();
      __SIM__task16834->SetWorker(&_task->GetWorker());
      __SIM__task16834->SetArg0(_this->var___SIM__stdorder_Int6060);
      __SIM__task16834->SetLabel(&&method___SIM__StdOrderInt3);
      _task.reset(__SIM__task16834);
    }
    goto method___SIM__StdOrderInt3;
    __SIM____SIM__StartService5_false16825:

    _task->SetLabel(&&__SIM__checkpoint_16828);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16828:

    { Task___SIM__StartService5 *__SIM__task16829(new Task___SIM__StartService5());
      __SIM__task16829->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop16833((*((libpi::Int*)_this->var___SIM__i6088.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task16829->SetArg0(__SIM__binop16833);
      __SIM__task16829->SetArg1(_this->var___SIM__stdorder_Int6060);
      __SIM__task16829->SetLabel(&&method___SIM__StartService5);
      shared_ptr<libpi::task::Task> __SIM__task16830(__SIM__task16829);
      _task->GetWorker().AddTask(__SIM__task16830);
    }

    _task->SetLabel(&&__SIM__checkpoint_16832);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16832:

    { Task___SIM__StdOrderInt3 *__SIM__task16831 = new Task___SIM__StdOrderInt3();
      __SIM__task16831->SetWorker(&_task->GetWorker());
      __SIM__task16831->SetArg0(_this->var___SIM__stdorder_Int6060);
      __SIM__task16831->SetLabel(&&method___SIM__StdOrderInt3);
      _task.reset(__SIM__task16831);
    }
    goto method___SIM__StdOrderInt3;
  } // }}}
  method___SIM__X7: // {{{
  #undef _this
  #define _this ((Task___SIM__X7*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16837);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16837:

    _task->SetLabel(&&__SIM__checkpoint___SIM__X716836);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__X716836:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__s6091.get())->Close(true);
    _this->var___SIM__s6091.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^eq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16839);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16839:

    // ~s6091[2] >> ~lhs6107;
    _task->SetLabel(&&__SIM__receive16838);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__lhs6107))
      return false;
    __SIM__receive16838:

    _task->SetLabel(&&__SIM__checkpoint_16841);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16841:

    // ~s6091[2] >> ~rhs6108;
    _task->SetLabel(&&__SIM__receive16840);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__rhs6108))
      return false;
    __SIM__receive16840:

    _task->SetLabel(&&__SIM__checkpoint_16843);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16843:

    // ~s6091[2] << (~lhs6107 = ~rhs6108);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16844((*((libpi::Float*)_this->var___SIM__lhs6107.get())) == (*((libpi::Float*)_this->var___SIM__rhs6108.get())));
      ((libpi::Session*)_this->var___SIM__s6091.get())->Send(1,_task,__SIM__binop16844);
    }

    _task->SetLabel(&&__SIM__checkpoint_16846);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16846:

    { Task___SIM__X7 *__SIM__task16845 = new Task___SIM__X7();
      __SIM__task16845->SetWorker(&_task->GetWorker());
      __SIM__task16845->SetArg0(_this->var___SIM__s6091);
      __SIM__task16845->SetLabel(&&method___SIM__X7);
      _task.reset(__SIM__task16845);
    }
    goto method___SIM__X7;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^geq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16848);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16848:

    // ~s6091[2] >> ~lhs6109;
    _task->SetLabel(&&__SIM__receive16847);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__lhs6109))
      return false;
    __SIM__receive16847:

    _task->SetLabel(&&__SIM__checkpoint_16850);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16850:

    // ~s6091[2] >> ~rhs6110;
    _task->SetLabel(&&__SIM__receive16849);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__rhs6110))
      return false;
    __SIM__receive16849:

    _task->SetLabel(&&__SIM__checkpoint_16852);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16852:

    // ~s6091[2] << (~lhs6109 >= ~rhs6110);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16853((*((libpi::Float*)_this->var___SIM__lhs6109.get())) >= (*((libpi::Float*)_this->var___SIM__rhs6110.get())));
      ((libpi::Session*)_this->var___SIM__s6091.get())->Send(1,_task,__SIM__binop16853);
    }

    _task->SetLabel(&&__SIM__checkpoint_16855);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16855:

    { Task___SIM__X7 *__SIM__task16854 = new Task___SIM__X7();
      __SIM__task16854->SetWorker(&_task->GetWorker());
      __SIM__task16854->SetArg0(_this->var___SIM__s6091);
      __SIM__task16854->SetLabel(&&method___SIM__X7);
      _task.reset(__SIM__task16854);
    }
    goto method___SIM__X7;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^greater")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16857);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16857:

    // ~s6091[2] >> ~lhs6111;
    _task->SetLabel(&&__SIM__receive16856);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__lhs6111))
      return false;
    __SIM__receive16856:

    _task->SetLabel(&&__SIM__checkpoint_16859);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16859:

    // ~s6091[2] >> ~rhs6112;
    _task->SetLabel(&&__SIM__receive16858);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__rhs6112))
      return false;
    __SIM__receive16858:

    _task->SetLabel(&&__SIM__checkpoint_16861);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16861:

    // ~s6091[2] << (~rhs6112 < ~lhs6111);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16862((*((libpi::Float*)_this->var___SIM__rhs6112.get())) < (*((libpi::Float*)_this->var___SIM__lhs6111.get())));
      ((libpi::Session*)_this->var___SIM__s6091.get())->Send(1,_task,__SIM__binop16862);
    }

    _task->SetLabel(&&__SIM__checkpoint_16864);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16864:

    { Task___SIM__X7 *__SIM__task16863 = new Task___SIM__X7();
      __SIM__task16863->SetWorker(&_task->GetWorker());
      __SIM__task16863->SetArg0(_this->var___SIM__s6091);
      __SIM__task16863->SetLabel(&&method___SIM__X7);
      _task.reset(__SIM__task16863);
    }
    goto method___SIM__X7;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^leq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16866);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16866:

    // ~s6091[2] >> ~lhs6113;
    _task->SetLabel(&&__SIM__receive16865);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__lhs6113))
      return false;
    __SIM__receive16865:

    _task->SetLabel(&&__SIM__checkpoint_16868);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16868:

    // ~s6091[2] >> ~rhs6114;
    _task->SetLabel(&&__SIM__receive16867);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__rhs6114))
      return false;
    __SIM__receive16867:

    _task->SetLabel(&&__SIM__checkpoint_16870);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16870:

    // ~s6091[2] << (~lhs6113 <= ~rhs6114);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16871((*((libpi::Float*)_this->var___SIM__lhs6113.get())) <= (*((libpi::Float*)_this->var___SIM__rhs6114.get())));
      ((libpi::Session*)_this->var___SIM__s6091.get())->Send(1,_task,__SIM__binop16871);
    }

    _task->SetLabel(&&__SIM__checkpoint_16873);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16873:

    { Task___SIM__X7 *__SIM__task16872 = new Task___SIM__X7();
      __SIM__task16872->SetWorker(&_task->GetWorker());
      __SIM__task16872->SetArg0(_this->var___SIM__s6091);
      __SIM__task16872->SetLabel(&&method___SIM__X7);
      _task.reset(__SIM__task16872);
    }
    goto method___SIM__X7;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^less")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16875);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16875:

    // ~s6091[2] >> ~lhs6115;
    _task->SetLabel(&&__SIM__receive16874);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__lhs6115))
      return false;
    __SIM__receive16874:

    _task->SetLabel(&&__SIM__checkpoint_16877);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16877:

    // ~s6091[2] >> ~rhs6116;
    _task->SetLabel(&&__SIM__receive16876);
    if (!((libpi::Session*)_this->var___SIM__s6091.get())->Receive(1,_task,_this->var___SIM__rhs6116))
      return false;
    __SIM__receive16876:

    _task->SetLabel(&&__SIM__checkpoint_16879);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16879:

    // ~s6091[2] << (~lhs6115 < ~rhs6116);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16880((*((libpi::Float*)_this->var___SIM__lhs6115.get())) < (*((libpi::Float*)_this->var___SIM__rhs6116.get())));
      ((libpi::Session*)_this->var___SIM__s6091.get())->Send(1,_task,__SIM__binop16880);
    }

    _task->SetLabel(&&__SIM__checkpoint_16882);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16882:

    { Task___SIM__X7 *__SIM__task16881 = new Task___SIM__X7();
      __SIM__task16881->SetWorker(&_task->GetWorker());
      __SIM__task16881->SetArg0(_this->var___SIM__s6091);
      __SIM__task16881->SetLabel(&&method___SIM__X7);
      _task.reset(__SIM__task16881);
    }
    goto method___SIM__X7;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight26: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight26*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16884);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16884:

    { Task___SIM__X7 *__SIM__task16883 = new Task___SIM__X7();
      __SIM__task16883->SetWorker(&_task->GetWorker());
      __SIM__task16883->SetArg0(_this->var___SIM__s6090);
      __SIM__task16883->SetLabel(&&method___SIM__X7);
      _task.reset(__SIM__task16883);
    }
    goto method___SIM__X7;
  } // }}}
  method___SIM__StdOrderFloat6: // {{{
  #undef _this
  #define _this ((Task___SIM__StdOrderFloat6*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16885);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16885:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__StdOrderFloat616886);
        if (!((libpi::task::Link*)_this->var___SIM__stdorder_Float6089.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__StdOrderFloat616886:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6090.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_16887);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16887:

    { Task___SIM__FlatRight26 *__SIM__task16888(new Task___SIM__FlatRight26());
      __SIM__task16888->SetWorker(&_task->GetWorker());
      __SIM__task16888->SetArg0(_this->var___SIM__s6090);
      __SIM__task16888->SetLabel(&&method___SIM__FlatRight26);
      shared_ptr<libpi::task::Task> __SIM__task16889(__SIM__task16888);
      _task->GetWorker().AddTask(__SIM__task16889);
    }

    _task->SetLabel(&&__SIM__checkpoint_16891);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16891:

    { Task___SIM__StdOrderFloat6 *__SIM__task16890 = new Task___SIM__StdOrderFloat6();
      __SIM__task16890->SetWorker(&_task->GetWorker());
      __SIM__task16890->SetArg0(_this->var___SIM__stdorder_Float6089);
      __SIM__task16890->SetLabel(&&method___SIM__StdOrderFloat6);
      _task.reset(__SIM__task16890);
    }
    goto method___SIM__StdOrderFloat6;
  } // }}}
  method___SIM__StartService8: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService8*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16894);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16894:

    {
      shared_ptr<libpi::Bool> __SIM__binop16895((*((libpi::Int*)_this->var___SIM__i6117.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop16895.get())->GetValue())
        goto __SIM____SIM__StartService8_true16892;
      else
        goto __SIM____SIM__StartService8_false16893;
    }
    __SIM____SIM__StartService8_true16892:

    _task->SetLabel(&&__SIM__checkpoint_16903);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16903:

    { Task___SIM__StdOrderFloat6 *__SIM__task16902 = new Task___SIM__StdOrderFloat6();
      __SIM__task16902->SetWorker(&_task->GetWorker());
      __SIM__task16902->SetArg0(_this->var___SIM__stdorder_Float6089);
      __SIM__task16902->SetLabel(&&method___SIM__StdOrderFloat6);
      _task.reset(__SIM__task16902);
    }
    goto method___SIM__StdOrderFloat6;
    __SIM____SIM__StartService8_false16893:

    _task->SetLabel(&&__SIM__checkpoint_16896);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16896:

    { Task___SIM__StartService8 *__SIM__task16897(new Task___SIM__StartService8());
      __SIM__task16897->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop16901((*((libpi::Int*)_this->var___SIM__i6117.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task16897->SetArg0(__SIM__binop16901);
      __SIM__task16897->SetArg1(_this->var___SIM__stdorder_Float6089);
      __SIM__task16897->SetLabel(&&method___SIM__StartService8);
      shared_ptr<libpi::task::Task> __SIM__task16898(__SIM__task16897);
      _task->GetWorker().AddTask(__SIM__task16898);
    }

    _task->SetLabel(&&__SIM__checkpoint_16900);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16900:

    { Task___SIM__StdOrderFloat6 *__SIM__task16899 = new Task___SIM__StdOrderFloat6();
      __SIM__task16899->SetWorker(&_task->GetWorker());
      __SIM__task16899->SetArg0(_this->var___SIM__stdorder_Float6089);
      __SIM__task16899->SetLabel(&&method___SIM__StdOrderFloat6);
      _task.reset(__SIM__task16899);
    }
    goto method___SIM__StdOrderFloat6;
  } // }}}
  method___SIM__X10: // {{{
  #undef _this
  #define _this ((Task___SIM__X10*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16905);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16905:

    _task->SetLabel(&&__SIM__checkpoint___SIM__X1016904);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__X1016904:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__s6120.get())->Close(true);
    _this->var___SIM__s6120.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^eq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16907);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16907:

    // ~s6120[2] >> ~lhs6136;
    _task->SetLabel(&&__SIM__receive16906);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__lhs6136))
      return false;
    __SIM__receive16906:

    _task->SetLabel(&&__SIM__checkpoint_16909);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16909:

    // ~s6120[2] >> ~rhs6137;
    _task->SetLabel(&&__SIM__receive16908);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__rhs6137))
      return false;
    __SIM__receive16908:

    _task->SetLabel(&&__SIM__checkpoint_16911);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16911:

    // ~s6120[2] << (~lhs6136 = ~rhs6137);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16912((*((libpi::String*)_this->var___SIM__lhs6136.get())) == (*((libpi::String*)_this->var___SIM__rhs6137.get())));
      ((libpi::Session*)_this->var___SIM__s6120.get())->Send(1,_task,__SIM__binop16912);
    }

    _task->SetLabel(&&__SIM__checkpoint_16914);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16914:

    { Task___SIM__X10 *__SIM__task16913 = new Task___SIM__X10();
      __SIM__task16913->SetWorker(&_task->GetWorker());
      __SIM__task16913->SetArg0(_this->var___SIM__s6120);
      __SIM__task16913->SetLabel(&&method___SIM__X10);
      _task.reset(__SIM__task16913);
    }
    goto method___SIM__X10;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^geq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16916);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16916:

    // ~s6120[2] >> ~lhs6138;
    _task->SetLabel(&&__SIM__receive16915);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__lhs6138))
      return false;
    __SIM__receive16915:

    _task->SetLabel(&&__SIM__checkpoint_16918);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16918:

    // ~s6120[2] >> ~rhs6139;
    _task->SetLabel(&&__SIM__receive16917);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__rhs6139))
      return false;
    __SIM__receive16917:

    _task->SetLabel(&&__SIM__checkpoint_16920);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16920:

    // ~s6120[2] << (~lhs6138 >= ~rhs6139);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16921((*((libpi::String*)_this->var___SIM__lhs6138.get())) >= (*((libpi::String*)_this->var___SIM__rhs6139.get())));
      ((libpi::Session*)_this->var___SIM__s6120.get())->Send(1,_task,__SIM__binop16921);
    }

    _task->SetLabel(&&__SIM__checkpoint_16923);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16923:

    { Task___SIM__X10 *__SIM__task16922 = new Task___SIM__X10();
      __SIM__task16922->SetWorker(&_task->GetWorker());
      __SIM__task16922->SetArg0(_this->var___SIM__s6120);
      __SIM__task16922->SetLabel(&&method___SIM__X10);
      _task.reset(__SIM__task16922);
    }
    goto method___SIM__X10;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^greater")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16925);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16925:

    // ~s6120[2] >> ~lhs6140;
    _task->SetLabel(&&__SIM__receive16924);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__lhs6140))
      return false;
    __SIM__receive16924:

    _task->SetLabel(&&__SIM__checkpoint_16927);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16927:

    // ~s6120[2] >> ~rhs6141;
    _task->SetLabel(&&__SIM__receive16926);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__rhs6141))
      return false;
    __SIM__receive16926:

    _task->SetLabel(&&__SIM__checkpoint_16929);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16929:

    // ~s6120[2] << (~rhs6141 < ~lhs6140);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16930((*((libpi::String*)_this->var___SIM__rhs6141.get())) < (*((libpi::String*)_this->var___SIM__lhs6140.get())));
      ((libpi::Session*)_this->var___SIM__s6120.get())->Send(1,_task,__SIM__binop16930);
    }

    _task->SetLabel(&&__SIM__checkpoint_16932);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16932:

    { Task___SIM__X10 *__SIM__task16931 = new Task___SIM__X10();
      __SIM__task16931->SetWorker(&_task->GetWorker());
      __SIM__task16931->SetArg0(_this->var___SIM__s6120);
      __SIM__task16931->SetLabel(&&method___SIM__X10);
      _task.reset(__SIM__task16931);
    }
    goto method___SIM__X10;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^leq")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16934);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16934:

    // ~s6120[2] >> ~lhs6142;
    _task->SetLabel(&&__SIM__receive16933);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__lhs6142))
      return false;
    __SIM__receive16933:

    _task->SetLabel(&&__SIM__checkpoint_16936);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16936:

    // ~s6120[2] >> ~rhs6143;
    _task->SetLabel(&&__SIM__receive16935);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__rhs6143))
      return false;
    __SIM__receive16935:

    _task->SetLabel(&&__SIM__checkpoint_16938);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16938:

    // ~s6120[2] << (~lhs6142 <= ~rhs6143);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16939((*((libpi::String*)_this->var___SIM__lhs6142.get())) <= (*((libpi::String*)_this->var___SIM__rhs6143.get())));
      ((libpi::Session*)_this->var___SIM__s6120.get())->Send(1,_task,__SIM__binop16939);
    }

    _task->SetLabel(&&__SIM__checkpoint_16941);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16941:

    { Task___SIM__X10 *__SIM__task16940 = new Task___SIM__X10();
      __SIM__task16940->SetWorker(&_task->GetWorker());
      __SIM__task16940->SetArg0(_this->var___SIM__s6120);
      __SIM__task16940->SetLabel(&&method___SIM__X10);
      _task.reset(__SIM__task16940);
    }
    goto method___SIM__X10;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^less")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16943);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16943:

    // ~s6120[2] >> ~lhs6144;
    _task->SetLabel(&&__SIM__receive16942);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__lhs6144))
      return false;
    __SIM__receive16942:

    _task->SetLabel(&&__SIM__checkpoint_16945);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16945:

    // ~s6120[2] >> ~rhs6145;
    _task->SetLabel(&&__SIM__receive16944);
    if (!((libpi::Session*)_this->var___SIM__s6120.get())->Receive(1,_task,_this->var___SIM__rhs6145))
      return false;
    __SIM__receive16944:

    _task->SetLabel(&&__SIM__checkpoint_16947);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16947:

    // ~s6120[2] << (~lhs6144 < ~rhs6145);
    { 
      shared_ptr<libpi::Bool> __SIM__binop16948((*((libpi::String*)_this->var___SIM__lhs6144.get())) < (*((libpi::String*)_this->var___SIM__rhs6145.get())));
      ((libpi::Session*)_this->var___SIM__s6120.get())->Send(1,_task,__SIM__binop16948);
    }

    _task->SetLabel(&&__SIM__checkpoint_16950);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16950:

    { Task___SIM__X10 *__SIM__task16949 = new Task___SIM__X10();
      __SIM__task16949->SetWorker(&_task->GetWorker());
      __SIM__task16949->SetArg0(_this->var___SIM__s6120);
      __SIM__task16949->SetLabel(&&method___SIM__X10);
      _task.reset(__SIM__task16949);
    }
    goto method___SIM__X10;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight27: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight27*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16952);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16952:

    { Task___SIM__X10 *__SIM__task16951 = new Task___SIM__X10();
      __SIM__task16951->SetWorker(&_task->GetWorker());
      __SIM__task16951->SetArg0(_this->var___SIM__s6119);
      __SIM__task16951->SetLabel(&&method___SIM__X10);
      _task.reset(__SIM__task16951);
    }
    goto method___SIM__X10;
  } // }}}
  method___SIM__StdOrderString9: // {{{
  #undef _this
  #define _this ((Task___SIM__StdOrderString9*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16953);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16953:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__StdOrderString916954);
        if (!((libpi::task::Link*)_this->var___SIM__stdorder_String6118.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__StdOrderString916954:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6119.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_16955);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16955:

    { Task___SIM__FlatRight27 *__SIM__task16956(new Task___SIM__FlatRight27());
      __SIM__task16956->SetWorker(&_task->GetWorker());
      __SIM__task16956->SetArg0(_this->var___SIM__s6119);
      __SIM__task16956->SetLabel(&&method___SIM__FlatRight27);
      shared_ptr<libpi::task::Task> __SIM__task16957(__SIM__task16956);
      _task->GetWorker().AddTask(__SIM__task16957);
    }

    _task->SetLabel(&&__SIM__checkpoint_16959);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16959:

    { Task___SIM__StdOrderString9 *__SIM__task16958 = new Task___SIM__StdOrderString9();
      __SIM__task16958->SetWorker(&_task->GetWorker());
      __SIM__task16958->SetArg0(_this->var___SIM__stdorder_String6118);
      __SIM__task16958->SetLabel(&&method___SIM__StdOrderString9);
      _task.reset(__SIM__task16958);
    }
    goto method___SIM__StdOrderString9;
  } // }}}
  method___SIM__StartService11: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService11*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16962);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16962:

    {
      shared_ptr<libpi::Bool> __SIM__binop16963((*((libpi::Int*)_this->var___SIM__i6146.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop16963.get())->GetValue())
        goto __SIM____SIM__StartService11_true16960;
      else
        goto __SIM____SIM__StartService11_false16961;
    }
    __SIM____SIM__StartService11_true16960:

    _task->SetLabel(&&__SIM__checkpoint_16971);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16971:

    { Task___SIM__StdOrderString9 *__SIM__task16970 = new Task___SIM__StdOrderString9();
      __SIM__task16970->SetWorker(&_task->GetWorker());
      __SIM__task16970->SetArg0(_this->var___SIM__stdorder_String6118);
      __SIM__task16970->SetLabel(&&method___SIM__StdOrderString9);
      _task.reset(__SIM__task16970);
    }
    goto method___SIM__StdOrderString9;
    __SIM____SIM__StartService11_false16961:

    _task->SetLabel(&&__SIM__checkpoint_16964);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16964:

    { Task___SIM__StartService11 *__SIM__task16965(new Task___SIM__StartService11());
      __SIM__task16965->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop16969((*((libpi::Int*)_this->var___SIM__i6146.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task16965->SetArg0(__SIM__binop16969);
      __SIM__task16965->SetArg1(_this->var___SIM__stdorder_String6118);
      __SIM__task16965->SetLabel(&&method___SIM__StartService11);
      shared_ptr<libpi::task::Task> __SIM__task16966(__SIM__task16965);
      _task->GetWorker().AddTask(__SIM__task16966);
    }

    _task->SetLabel(&&__SIM__checkpoint_16968);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16968:

    { Task___SIM__StdOrderString9 *__SIM__task16967 = new Task___SIM__StdOrderString9();
      __SIM__task16967->SetWorker(&_task->GetWorker());
      __SIM__task16967->SetArg0(_this->var___SIM__stdorder_String6118);
      __SIM__task16967->SetLabel(&&method___SIM__StdOrderString9);
      _task.reset(__SIM__task16967);
    }
    goto method___SIM__StdOrderString9;
  } // }}}
  method___SIM__Cons14: // {{{
  #undef _this
  #define _this ((Task___SIM__Cons14*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_16973);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16973:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Cons1416972);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Cons1416972:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^append")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16976);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16976:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__Cons14_true16974;
      else
        goto __SIM____SIM__Cons14_false16975;
    }
    __SIM____SIM__Cons14_true16974:

    _task->SetLabel(&&__SIM__checkpoint_16989);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16989:

    // ~this6175[2] >> ~l6220;
    _task->SetLabel(&&__SIM__receive16988);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__l6220))
      return false;
    __SIM__receive16988:

    _task->SetLabel(&&__SIM__checkpoint_16991);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16991:

    // ~tail6174[1] << ^append;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_6a5f1f3d587b4238521a743687b6780f);

    _task->SetLabel(&&__SIM__checkpoint_16993);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16993:

    // ~tail6174[1] << ~l6220;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__l6220);
    }
  _this->var___SIM__l6220.reset();

    _task->SetLabel(&&__SIM__checkpoint_16995);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16995:

    { Task___SIM__Cons14 *__SIM__task16994 = new Task___SIM__Cons14();
      __SIM__task16994->SetWorker(&_task->GetWorker());
      __SIM__task16994->SetArg0(_this->var___SIM__head6173);
      __SIM__task16994->SetArg1(_this->var___SIM__tail6174);
      __SIM__task16994->SetArg2(_this->var___SIM__this6175);
      __SIM__task16994->SetArg3(_this->var___SIM__list6147);
      __SIM__task16994->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task16994);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false16975:

    _task->SetLabel(&&__SIM__checkpoint_16981);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16981:

    // ~tail6174[1] << ^append;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_6a5f1f3d587b4238521a743687b6780f);

    _task->SetLabel(&&__SIM__checkpoint_16983);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16983:

    // ~this6175[2] >> ~l6221;
    _task->SetLabel(&&__SIM__receive16982);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__l6221))
      return false;
    __SIM__receive16982:

    _task->SetLabel(&&__SIM__checkpoint_16985);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16985:

    // ~tail6174[1] << ~l6221;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__l6221);
    }
  _this->var___SIM__l6221.reset();

    _task->SetLabel(&&__SIM__checkpoint_16987);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16987:

    { Task___SIM__Cons14 *__SIM__task16986 = new Task___SIM__Cons14();
      __SIM__task16986->SetWorker(&_task->GetWorker());
      __SIM__task16986->SetArg0(_this->var___SIM__head6173);
      __SIM__task16986->SetArg1(_this->var___SIM__tail6174);
      __SIM__task16986->SetArg2(_this->var___SIM__this6175);
      __SIM__task16986->SetArg3(_this->var___SIM__list6147);
      __SIM__task16986->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task16986);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^apply")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_16998);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_16998:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__Cons14_true16996;
      else
        goto __SIM____SIM__Cons14_false16997;
    }
    __SIM____SIM__Cons14_true16996:

    _task->SetLabel(&&__SIM__checkpoint_17017);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17017:

    // ~this6175[2] >> ~fc6222;
    _task->SetLabel(&&__SIM__receive17016);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__fc6222))
      return false;
    __SIM__receive17016:

    _task->SetLabel(&&__SIM__checkpoint_17018);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17018:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__fc6222.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Cons1417019);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__f6223))
        return false;
      __SIM__receive___SIM__Cons1417019:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17021);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17021:

    // ~f6223[1] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__f6223.get())->Send(0,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17023);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17023:

    // ~f6223[1] >> ~h6224;
    _task->SetLabel(&&__SIM__receive17022);
    if (!((libpi::Session*)_this->var___SIM__f6223.get())->Receive(0,_task,_this->var___SIM__h6224))
      return false;
    __SIM__receive17022:
    // Session complete
    ((libpi::Session*)_this->var___SIM__f6223.get())->Close(true);
    _this->var___SIM__f6223.reset();

    _task->SetLabel(&&__SIM__checkpoint_17025);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17025:

    // ~tail6174[1] << ^apply;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_1da62d864765a583145933d5f61f186b);

    _task->SetLabel(&&__SIM__checkpoint_17027);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17027:

    // ~tail6174[1] << ~fc6222;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__fc6222);
    }

    _task->SetLabel(&&__SIM__checkpoint_17029);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17029:

    { Task___SIM__Cons14 *__SIM__task17028 = new Task___SIM__Cons14();
      __SIM__task17028->SetWorker(&_task->GetWorker());
      __SIM__task17028->SetArg0(_this->var___SIM__h6224);
      __SIM__task17028->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17028->SetArg2(_this->var___SIM__this6175);
      __SIM__task17028->SetArg3(_this->var___SIM__list6147);
      __SIM__task17028->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17028);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false16997:

    _task->SetLabel(&&__SIM__checkpoint_17003);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17003:

    // ~this6175[2] >> ~fc6225;
    _task->SetLabel(&&__SIM__receive17002);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__fc6225))
      return false;
    __SIM__receive17002:

    _task->SetLabel(&&__SIM__checkpoint_17004);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17004:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__fc6225.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Cons1417005);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__f6226))
        return false;
      __SIM__receive___SIM__Cons1417005:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17007);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17007:

    // ~f6226[1] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__f6226.get())->Send(0,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17009);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17009:

    // ~tail6174[1] << ^apply;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_1da62d864765a583145933d5f61f186b);

    _task->SetLabel(&&__SIM__checkpoint_17011);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17011:

    // ~tail6174[1] << ~fc6225;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__fc6225);
    }

    _task->SetLabel(&&__SIM__checkpoint_17013);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17013:

    // ~f6226[1] >> ~h6227;
    _task->SetLabel(&&__SIM__receive17012);
    if (!((libpi::Session*)_this->var___SIM__f6226.get())->Receive(0,_task,_this->var___SIM__h6227))
      return false;
    __SIM__receive17012:
    // Session complete
    ((libpi::Session*)_this->var___SIM__f6226.get())->Close(true);
    _this->var___SIM__f6226.reset();

    _task->SetLabel(&&__SIM__checkpoint_17015);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17015:

    { Task___SIM__Cons14 *__SIM__task17014 = new Task___SIM__Cons14();
      __SIM__task17014->SetWorker(&_task->GetWorker());
      __SIM__task17014->SetArg0(_this->var___SIM__h6227);
      __SIM__task17014->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17014->SetArg2(_this->var___SIM__this6175);
      __SIM__task17014->SetArg3(_this->var___SIM__list6147);
      __SIM__task17014->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17014);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^cons")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17031);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17031:

    // ~this6175[2] >> ~h6228;
    _task->SetLabel(&&__SIM__receive17030);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__h6228))
      return false;
    __SIM__receive17030:

    _task->SetLabel(&&__SIM__checkpoint_17032);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17032:

    {
      _task->tmps.clear();
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l16229_in17033;
    __SIM____SIM__l16229_in17033.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16229_in17033.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l16229_out17034;
    __SIM____SIM__l16229_out17034.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16229_out17034.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
      _this->var___SIM__l16229.reset(new libpi::Session(1, 2, __SIM____SIM__l16229_in17033,__SIM____SIM__l16229_out17034));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l26230_in17035;
    __SIM____SIM__l26230_in17035.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
    __SIM____SIM__l26230_in17035.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l26230_out17036;
    __SIM____SIM__l26230_out17036.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    __SIM____SIM__l26230_out17036.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
      _this->var___SIM__l26230.reset(new libpi::Session(2, 2, __SIM____SIM__l26230_in17035,__SIM____SIM__l26230_out17036));
      _this->tmps.clear();
    }

    _task->SetLabel(&&__SIM__checkpoint_17037);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17037:

    { Task___SIM__Cons14 *__SIM__task17038(new Task___SIM__Cons14());
      __SIM__task17038->SetWorker(&_task->GetWorker());
      __SIM__task17038->SetArg0(_this->var___SIM__head6173);
      __SIM__task17038->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17038->SetArg2(_this->var___SIM__l16229);
      __SIM__task17038->SetArg3(_this->var___SIM__list6147);
      __SIM__task17038->SetLabel(&&method___SIM__Cons14);
      shared_ptr<libpi::task::Task> __SIM__task17039(__SIM__task17038);
      _task->GetWorker().AddTask(__SIM__task17039);
    }

    _task->SetLabel(&&__SIM__checkpoint_17041);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17041:

    { Task___SIM__Cons14 *__SIM__task17040 = new Task___SIM__Cons14();
      __SIM__task17040->SetWorker(&_task->GetWorker());
      __SIM__task17040->SetArg0(_this->var___SIM__h6228);
      __SIM__task17040->SetArg1(_this->var___SIM__l26230);
      __SIM__task17040->SetArg2(_this->var___SIM__this6175);
      __SIM__task17040->SetArg3(_this->var___SIM__list6147);
      __SIM__task17040->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17040);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^copy")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17043);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17043:

    // ~tail6174[1] << ^copy;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_bce51f3c33744c4a6709abcff0f1fa54);

    _task->SetLabel(&&__SIM__checkpoint_17045);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17045:

    // ~tail6174[1] >> ~t6253;
    _task->SetLabel(&&__SIM__receive17044);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__t6253))
      return false;
    __SIM__receive17044:

    _task->SetLabel(&&__SIM__checkpoint_17046);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17046:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Cons1417047);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__cpy6254))
        return false;
      __SIM__receive___SIM__Cons1417047:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17049);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17049:

    // ~cpy6254[1] << ^cons;
    ((libpi::Session*)_this->var___SIM__cpy6254.get())->Send(0,_task,label_c4eb37d961ed515432d3dda9a05c0b8b);

    _task->SetLabel(&&__SIM__checkpoint_17051);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17051:

    // ~cpy6254[1] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__cpy6254.get())->Send(0,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17053);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17053:

    // ~cpy6254[1] << ^append;
    ((libpi::Session*)_this->var___SIM__cpy6254.get())->Send(0,_task,label_6a5f1f3d587b4238521a743687b6780f);

    _task->SetLabel(&&__SIM__checkpoint_17055);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17055:

    // ~cpy6254[1] << ~t6253;
    { 
      ((libpi::Session*)_this->var___SIM__cpy6254.get())->Send(0,_task,_this->var___SIM__t6253);
    }
  _this->var___SIM__t6253.reset();

    _task->SetLabel(&&__SIM__checkpoint_17057);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17057:

    // ~this6175[2] << ~cpy6254;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__cpy6254);
    }
  _this->var___SIM__cpy6254.reset();

    _task->SetLabel(&&__SIM__checkpoint_17059);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17059:

    { Task___SIM__Cons14 *__SIM__task17058 = new Task___SIM__Cons14();
      __SIM__task17058->SetWorker(&_task->GetWorker());
      __SIM__task17058->SetArg0(_this->var___SIM__head6173);
      __SIM__task17058->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17058->SetArg2(_this->var___SIM__this6175);
      __SIM__task17058->SetArg3(_this->var___SIM__list6147);
      __SIM__task17058->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17058);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__this6175.get())->Close(true);
    _this->var___SIM__this6175.reset();

    _task->SetLabel(&&__SIM__checkpoint_17061);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17061:

    // ~tail6174[1] << ^end;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^erase")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17063);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17063:

    // ~this6175[2] >> ~i6255;
    _task->SetLabel(&&__SIM__receive17062);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__i6255))
      return false;
    __SIM__receive17062:

    _task->SetLabel(&&__SIM__checkpoint_17066);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17066:

    {
      shared_ptr<libpi::Bool> __SIM__binop17067((*((libpi::Int*)_this->var___SIM__i6255.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17067.get())->GetValue())
        goto __SIM____SIM__Cons14_true17064;
      else
        goto __SIM____SIM__Cons14_false17065;
    }
    __SIM____SIM__Cons14_true17064:

    _task->SetLabel(&&__SIM__checkpoint_17076);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17076:

    // ~tail6174[1] << ^pop;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_d0ff8d7ab3dcf009ed4db0dcac30d36b);

    _task->SetLabel(&&__SIM__checkpoint_17078);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17078:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Cons1417077);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Cons1417077:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^cons")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17080);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17080:

    // ~tail6174[1] >> ~h6256;
    _task->SetLabel(&&__SIM__receive17079);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__h6256))
      return false;
    __SIM__receive17079:

    _task->SetLabel(&&__SIM__checkpoint_17082);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17082:

    // ~tail6174[1] >> ~t6257;
    _task->SetLabel(&&__SIM__receive17081);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__t6257))
      return false;
    __SIM__receive17081:
    // Session complete
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();

    _task->SetLabel(&&__SIM__checkpoint_17084);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17084:

    { Task___SIM__Cons14 *__SIM__task17083 = new Task___SIM__Cons14();
      __SIM__task17083->SetWorker(&_task->GetWorker());
      __SIM__task17083->SetArg0(_this->var___SIM__h6256);
      __SIM__task17083->SetArg1(_this->var___SIM__t6257);
      __SIM__task17083->SetArg2(_this->var___SIM__this6175);
      __SIM__task17083->SetArg3(_this->var___SIM__list6147);
      __SIM__task17083->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17083);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^nil")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();

    _task->SetLabel(&&__SIM__checkpoint_17086);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17086:

    { Task___SIM__Nil13 *__SIM__task17085 = new Task___SIM__Nil13();
      __SIM__task17085->SetWorker(&_task->GetWorker());
      __SIM__task17085->SetArg0(_this->var___SIM__this6175);
      __SIM__task17085->SetArg1(_this->var___SIM__list6147);
      __SIM__task17085->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17085);
    }
    goto method___SIM__Nil13;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
    __SIM____SIM__Cons14_false17065:

    _task->SetLabel(&&__SIM__checkpoint_17069);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17069:

    // ~tail6174[1] << ^erase;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_e8d75b609730111d0f0a88f103c3016f);

    _task->SetLabel(&&__SIM__checkpoint_17071);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17071:

    // ~tail6174[1] << (~i6255 - 1);
    { 
      shared_ptr<libpi::Int> __SIM__binop17072((*((libpi::Int*)_this->var___SIM__i6255.get())) - (*((libpi::Int*)intval_1.get())));
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,__SIM__binop17072);
    }

    _task->SetLabel(&&__SIM__checkpoint_17074);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17074:

    { Task___SIM__Cons14 *__SIM__task17073 = new Task___SIM__Cons14();
      __SIM__task17073->SetWorker(&_task->GetWorker());
      __SIM__task17073->SetArg0(_this->var___SIM__head6173);
      __SIM__task17073->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17073->SetArg2(_this->var___SIM__this6175);
      __SIM__task17073->SetArg3(_this->var___SIM__list6147);
      __SIM__task17073->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17073);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^filter")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17089);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17089:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__Cons14_true17087;
      else
        goto __SIM____SIM__Cons14_false17088;
    }
    __SIM____SIM__Cons14_true17087:

    _task->SetLabel(&&__SIM__checkpoint_17123);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17123:

    // ~this6175[2] >> ~filter6258;
    _task->SetLabel(&&__SIM__receive17122);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__filter6258))
      return false;
    __SIM__receive17122:

    _task->SetLabel(&&__SIM__checkpoint_17125);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17125:

    // ~filter6258[1] << ^call;
    ((libpi::Session*)_this->var___SIM__filter6258.get())->Send(0,_task,label_0e5493ddbd005506c18850a500dfea96);

    _task->SetLabel(&&__SIM__checkpoint_17127);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17127:

    // ~filter6258[1] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__filter6258.get())->Send(0,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17129);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17129:

    // ~filter6258[1] >> ~keep6259;
    _task->SetLabel(&&__SIM__receive17128);
    if (!((libpi::Session*)_this->var___SIM__filter6258.get())->Receive(0,_task,_this->var___SIM__keep6259))
      return false;
    __SIM__receive17128:

    _task->SetLabel(&&__SIM__checkpoint_17131);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17131:

    // ~tail6174[1] << ^filter;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_efcf5b49097e1969bd7fd24fb0c37fe6);

    _task->SetLabel(&&__SIM__checkpoint_17133);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17133:

    // ~tail6174[1] << ~filter6258;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__filter6258);
    }
  _this->var___SIM__filter6258.reset();

    _task->SetLabel(&&__SIM__checkpoint_17136);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17136:

    {
      if (((libpi::Bool*)_this->var___SIM__keep6259.get())->GetValue())
        goto __SIM____SIM__Cons14_true17134;
      else
        goto __SIM____SIM__Cons14_false17135;
    }
    __SIM____SIM__Cons14_true17134:

    _task->SetLabel(&&__SIM__checkpoint_17150);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17150:

    { Task___SIM__Cons14 *__SIM__task17149 = new Task___SIM__Cons14();
      __SIM__task17149->SetWorker(&_task->GetWorker());
      __SIM__task17149->SetArg0(_this->var___SIM__head6173);
      __SIM__task17149->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17149->SetArg2(_this->var___SIM__this6175);
      __SIM__task17149->SetArg3(_this->var___SIM__list6147);
      __SIM__task17149->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17149);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17135:

    _task->SetLabel(&&__SIM__checkpoint_17138);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17138:

    // ~tail6174[1] << ^pop;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_d0ff8d7ab3dcf009ed4db0dcac30d36b);

    _task->SetLabel(&&__SIM__checkpoint_17140);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17140:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Cons1417139);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Cons1417139:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^cons")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17142);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17142:

    // ~tail6174[1] >> ~h6260;
    _task->SetLabel(&&__SIM__receive17141);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__h6260))
      return false;
    __SIM__receive17141:

    _task->SetLabel(&&__SIM__checkpoint_17144);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17144:

    // ~tail6174[1] >> ~t6261;
    _task->SetLabel(&&__SIM__receive17143);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__t6261))
      return false;
    __SIM__receive17143:
    // Session complete
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();

    _task->SetLabel(&&__SIM__checkpoint_17146);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17146:

    { Task___SIM__Cons14 *__SIM__task17145 = new Task___SIM__Cons14();
      __SIM__task17145->SetWorker(&_task->GetWorker());
      __SIM__task17145->SetArg0(_this->var___SIM__h6260);
      __SIM__task17145->SetArg1(_this->var___SIM__t6261);
      __SIM__task17145->SetArg2(_this->var___SIM__this6175);
      __SIM__task17145->SetArg3(_this->var___SIM__list6147);
      __SIM__task17145->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17145);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^nil")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();

    _task->SetLabel(&&__SIM__checkpoint_17148);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17148:

    { Task___SIM__Nil13 *__SIM__task17147 = new Task___SIM__Nil13();
      __SIM__task17147->SetWorker(&_task->GetWorker());
      __SIM__task17147->SetArg0(_this->var___SIM__this6175);
      __SIM__task17147->SetArg1(_this->var___SIM__list6147);
      __SIM__task17147->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17147);
    }
    goto method___SIM__Nil13;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
    __SIM____SIM__Cons14_false17088:

    _task->SetLabel(&&__SIM__checkpoint_17094);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17094:

    // ~this6175[2] >> ~filter6262;
    _task->SetLabel(&&__SIM__receive17093);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__filter6262))
      return false;
    __SIM__receive17093:

    _task->SetLabel(&&__SIM__checkpoint_17096);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17096:

    // ~filter6262[1] << ^call;
    ((libpi::Session*)_this->var___SIM__filter6262.get())->Send(0,_task,label_0e5493ddbd005506c18850a500dfea96);

    _task->SetLabel(&&__SIM__checkpoint_17098);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17098:

    // ~filter6262[1] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__filter6262.get())->Send(0,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17100);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17100:

    // ~tail6174[1] << ^filter;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_efcf5b49097e1969bd7fd24fb0c37fe6);

    _task->SetLabel(&&__SIM__checkpoint_17102);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17102:

    // ~filter6262[1] >> ~keep6263;
    _task->SetLabel(&&__SIM__receive17101);
    if (!((libpi::Session*)_this->var___SIM__filter6262.get())->Receive(0,_task,_this->var___SIM__keep6263))
      return false;
    __SIM__receive17101:

    _task->SetLabel(&&__SIM__checkpoint_17104);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17104:

    // ~tail6174[1] << ~filter6262;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__filter6262);
    }
  _this->var___SIM__filter6262.reset();

    _task->SetLabel(&&__SIM__checkpoint_17107);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17107:

    {
      if (((libpi::Bool*)_this->var___SIM__keep6263.get())->GetValue())
        goto __SIM____SIM__Cons14_true17105;
      else
        goto __SIM____SIM__Cons14_false17106;
    }
    __SIM____SIM__Cons14_true17105:

    _task->SetLabel(&&__SIM__checkpoint_17121);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17121:

    { Task___SIM__Cons14 *__SIM__task17120 = new Task___SIM__Cons14();
      __SIM__task17120->SetWorker(&_task->GetWorker());
      __SIM__task17120->SetArg0(_this->var___SIM__head6173);
      __SIM__task17120->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17120->SetArg2(_this->var___SIM__this6175);
      __SIM__task17120->SetArg3(_this->var___SIM__list6147);
      __SIM__task17120->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17120);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17106:

    _task->SetLabel(&&__SIM__checkpoint_17109);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17109:

    // ~tail6174[1] << ^pop;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_d0ff8d7ab3dcf009ed4db0dcac30d36b);

    _task->SetLabel(&&__SIM__checkpoint_17111);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17111:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Cons1417110);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Cons1417110:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^cons")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17113);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17113:

    // ~tail6174[1] >> ~h6264;
    _task->SetLabel(&&__SIM__receive17112);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__h6264))
      return false;
    __SIM__receive17112:

    _task->SetLabel(&&__SIM__checkpoint_17115);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17115:

    // ~tail6174[1] >> ~t6265;
    _task->SetLabel(&&__SIM__receive17114);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__t6265))
      return false;
    __SIM__receive17114:
    // Session complete
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();

    _task->SetLabel(&&__SIM__checkpoint_17117);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17117:

    { Task___SIM__Cons14 *__SIM__task17116 = new Task___SIM__Cons14();
      __SIM__task17116->SetWorker(&_task->GetWorker());
      __SIM__task17116->SetArg0(_this->var___SIM__h6264);
      __SIM__task17116->SetArg1(_this->var___SIM__t6265);
      __SIM__task17116->SetArg2(_this->var___SIM__this6175);
      __SIM__task17116->SetArg3(_this->var___SIM__list6147);
      __SIM__task17116->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17116);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^nil")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Close(true);
    _this->var___SIM__tail6174.reset();

    _task->SetLabel(&&__SIM__checkpoint_17119);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17119:

    { Task___SIM__Nil13 *__SIM__task17118 = new Task___SIM__Nil13();
      __SIM__task17118->SetWorker(&_task->GetWorker());
      __SIM__task17118->SetArg0(_this->var___SIM__this6175);
      __SIM__task17118->SetArg1(_this->var___SIM__list6147);
      __SIM__task17118->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17118);
    }
    goto method___SIM__Nil13;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^get")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17152);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17152:

    // ~this6175[2] >> ~i6266;
    _task->SetLabel(&&__SIM__receive17151);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__i6266))
      return false;
    __SIM__receive17151:

    _task->SetLabel(&&__SIM__checkpoint_17155);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17155:

    {
      shared_ptr<libpi::Bool> __SIM__binop17156((*((libpi::Int*)_this->var___SIM__i6266.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17156.get())->GetValue())
        goto __SIM____SIM__Cons14_true17153;
      else
        goto __SIM____SIM__Cons14_false17154;
    }
    __SIM____SIM__Cons14_true17153:

    _task->SetLabel(&&__SIM__checkpoint_17209);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17209:

    // ~this6175[2] << ^val;
    ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,label_c08ee33285bc55f31dfe5248a00e7171);

    _task->SetLabel(&&__SIM__checkpoint_17211);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17211:

    // ~this6175[2] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17213);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17213:

    { Task___SIM__Cons14 *__SIM__task17212 = new Task___SIM__Cons14();
      __SIM__task17212->SetWorker(&_task->GetWorker());
      __SIM__task17212->SetArg0(_this->var___SIM__head6173);
      __SIM__task17212->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17212->SetArg2(_this->var___SIM__this6175);
      __SIM__task17212->SetArg3(_this->var___SIM__list6147);
      __SIM__task17212->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17212);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17154:

    _task->SetLabel(&&__SIM__checkpoint_17158);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17158:

    // ~tail6174[1] << ^get;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_957b92409b25b63db6dbd2b026b7cd2f);

    _task->SetLabel(&&__SIM__checkpoint_17160);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17160:

    // ~tail6174[1] << (~i6266 - 1);
    { 
      shared_ptr<libpi::Int> __SIM__binop17161((*((libpi::Int*)_this->var___SIM__i6266.get())) - (*((libpi::Int*)intval_1.get())));
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,__SIM__binop17161);
    }

    _task->SetLabel(&&__SIM__checkpoint_17163);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17163:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Cons1417162);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Cons1417162:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^err")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17166);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17166:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__Cons14_true17164;
      else
        goto __SIM____SIM__Cons14_false17165;
    }
    __SIM____SIM__Cons14_true17164:

    _task->SetLabel(&&__SIM__checkpoint_17179);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17179:

    // ~tail6174[1] >> ~msg6267;
    _task->SetLabel(&&__SIM__receive17178);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__msg6267))
      return false;
    __SIM__receive17178:

    _task->SetLabel(&&__SIM__checkpoint_17181);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17181:

    // ~this6175[2] << ^err;
    ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,label_a02d9bff9bc696a25d4e49dd4f301a44);

    _task->SetLabel(&&__SIM__checkpoint_17183);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17183:

    // ~this6175[2] << ~msg6267;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__msg6267);
    }

    _task->SetLabel(&&__SIM__checkpoint_17185);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17185:

    { Task___SIM__Cons14 *__SIM__task17184 = new Task___SIM__Cons14();
      __SIM__task17184->SetWorker(&_task->GetWorker());
      __SIM__task17184->SetArg0(_this->var___SIM__head6173);
      __SIM__task17184->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17184->SetArg2(_this->var___SIM__this6175);
      __SIM__task17184->SetArg3(_this->var___SIM__list6147);
      __SIM__task17184->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17184);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17165:

    _task->SetLabel(&&__SIM__checkpoint_17171);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17171:

    // ~this6175[2] << ^err;
    ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,label_a02d9bff9bc696a25d4e49dd4f301a44);

    _task->SetLabel(&&__SIM__checkpoint_17173);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17173:

    // ~tail6174[1] >> ~msg6268;
    _task->SetLabel(&&__SIM__receive17172);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__msg6268))
      return false;
    __SIM__receive17172:

    _task->SetLabel(&&__SIM__checkpoint_17175);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17175:

    // ~this6175[2] << ~msg6268;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__msg6268);
    }

    _task->SetLabel(&&__SIM__checkpoint_17177);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17177:

    { Task___SIM__Cons14 *__SIM__task17176 = new Task___SIM__Cons14();
      __SIM__task17176->SetWorker(&_task->GetWorker());
      __SIM__task17176->SetArg0(_this->var___SIM__head6173);
      __SIM__task17176->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17176->SetArg2(_this->var___SIM__this6175);
      __SIM__task17176->SetArg3(_this->var___SIM__list6147);
      __SIM__task17176->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17176);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^val")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17188);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17188:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__Cons14_true17186;
      else
        goto __SIM____SIM__Cons14_false17187;
    }
    __SIM____SIM__Cons14_true17186:

    _task->SetLabel(&&__SIM__checkpoint_17201);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17201:

    // ~tail6174[1] >> ~v6269;
    _task->SetLabel(&&__SIM__receive17200);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__v6269))
      return false;
    __SIM__receive17200:

    _task->SetLabel(&&__SIM__checkpoint_17203);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17203:

    // ~this6175[2] << ^val;
    ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,label_c08ee33285bc55f31dfe5248a00e7171);

    _task->SetLabel(&&__SIM__checkpoint_17205);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17205:

    // ~this6175[2] << ~v6269;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__v6269);
    }

    _task->SetLabel(&&__SIM__checkpoint_17207);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17207:

    { Task___SIM__Cons14 *__SIM__task17206 = new Task___SIM__Cons14();
      __SIM__task17206->SetWorker(&_task->GetWorker());
      __SIM__task17206->SetArg0(_this->var___SIM__head6173);
      __SIM__task17206->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17206->SetArg2(_this->var___SIM__this6175);
      __SIM__task17206->SetArg3(_this->var___SIM__list6147);
      __SIM__task17206->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17206);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17187:

    _task->SetLabel(&&__SIM__checkpoint_17193);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17193:

    // ~this6175[2] << ^val;
    ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,label_c08ee33285bc55f31dfe5248a00e7171);

    _task->SetLabel(&&__SIM__checkpoint_17195);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17195:

    // ~tail6174[1] >> ~v6270;
    _task->SetLabel(&&__SIM__receive17194);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__v6270))
      return false;
    __SIM__receive17194:

    _task->SetLabel(&&__SIM__checkpoint_17197);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17197:

    // ~this6175[2] << ~v6270;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__v6270);
    }

    _task->SetLabel(&&__SIM__checkpoint_17199);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17199:

    { Task___SIM__Cons14 *__SIM__task17198 = new Task___SIM__Cons14();
      __SIM__task17198->SetWorker(&_task->GetWorker());
      __SIM__task17198->SetArg0(_this->var___SIM__head6173);
      __SIM__task17198->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17198->SetArg2(_this->var___SIM__this6175);
      __SIM__task17198->SetArg3(_this->var___SIM__list6147);
      __SIM__task17198->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17198);
    }
    goto method___SIM__Cons14;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^insert")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17215);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17215:

    // ~this6175[2] >> ~i6271;
    _task->SetLabel(&&__SIM__receive17214);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__i6271))
      return false;
    __SIM__receive17214:

    _task->SetLabel(&&__SIM__checkpoint_17217);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17217:

    // ~this6175[2] >> ~h6272;
    _task->SetLabel(&&__SIM__receive17216);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__h6272))
      return false;
    __SIM__receive17216:

    _task->SetLabel(&&__SIM__checkpoint_17220);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17220:

    {
      shared_ptr<libpi::Bool> __SIM__binop17221((*((libpi::Int*)_this->var___SIM__i6271.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17221.get())->GetValue())
        goto __SIM____SIM__Cons14_true17218;
      else
        goto __SIM____SIM__Cons14_false17219;
    }
    __SIM____SIM__Cons14_true17218:

    _task->SetLabel(&&__SIM__checkpoint_17231);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17231:

    {
      _task->tmps.clear();
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l16273_in17232;
    __SIM____SIM__l16273_in17232.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16273_in17232.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l16273_out17233;
    __SIM____SIM__l16273_out17233.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16273_out17233.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
      _this->var___SIM__l16273.reset(new libpi::Session(1, 2, __SIM____SIM__l16273_in17232,__SIM____SIM__l16273_out17233));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l26274_in17234;
    __SIM____SIM__l26274_in17234.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
    __SIM____SIM__l26274_in17234.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l26274_out17235;
    __SIM____SIM__l26274_out17235.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    __SIM____SIM__l26274_out17235.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
      _this->var___SIM__l26274.reset(new libpi::Session(2, 2, __SIM____SIM__l26274_in17234,__SIM____SIM__l26274_out17235));
      _this->tmps.clear();
    }

    _task->SetLabel(&&__SIM__checkpoint_17236);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17236:

    { Task___SIM__Cons14 *__SIM__task17237(new Task___SIM__Cons14());
      __SIM__task17237->SetWorker(&_task->GetWorker());
      __SIM__task17237->SetArg0(_this->var___SIM__head6173);
      __SIM__task17237->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17237->SetArg2(_this->var___SIM__l16273);
      __SIM__task17237->SetArg3(_this->var___SIM__list6147);
      __SIM__task17237->SetLabel(&&method___SIM__Cons14);
      shared_ptr<libpi::task::Task> __SIM__task17238(__SIM__task17237);
      _task->GetWorker().AddTask(__SIM__task17238);
    }

    _task->SetLabel(&&__SIM__checkpoint_17240);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17240:

    { Task___SIM__Cons14 *__SIM__task17239 = new Task___SIM__Cons14();
      __SIM__task17239->SetWorker(&_task->GetWorker());
      __SIM__task17239->SetArg0(_this->var___SIM__h6272);
      __SIM__task17239->SetArg1(_this->var___SIM__l26274);
      __SIM__task17239->SetArg2(_this->var___SIM__this6175);
      __SIM__task17239->SetArg3(_this->var___SIM__list6147);
      __SIM__task17239->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17239);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17219:

    _task->SetLabel(&&__SIM__checkpoint_17223);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17223:

    // ~tail6174[1] << ^insert;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_12e9fca472d02f299c6b8b606bf0318e);

    _task->SetLabel(&&__SIM__checkpoint_17225);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17225:

    // ~tail6174[1] << (~i6271 - 1);
    { 
      shared_ptr<libpi::Int> __SIM__binop17226((*((libpi::Int*)_this->var___SIM__i6271.get())) - (*((libpi::Int*)intval_1.get())));
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,__SIM__binop17226);
    }

    _task->SetLabel(&&__SIM__checkpoint_17228);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17228:

    // ~tail6174[1] << ~h6272;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__h6272);
    }

    _task->SetLabel(&&__SIM__checkpoint_17230);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17230:

    { Task___SIM__Cons14 *__SIM__task17229 = new Task___SIM__Cons14();
      __SIM__task17229->SetWorker(&_task->GetWorker());
      __SIM__task17229->SetArg0(_this->var___SIM__head6173);
      __SIM__task17229->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17229->SetArg2(_this->var___SIM__this6175);
      __SIM__task17229->SetArg3(_this->var___SIM__list6147);
      __SIM__task17229->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17229);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^length")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17242);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17242:

    // ~tail6174[1] << ^length;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_93fdfd9a3eb61044995b53f81eaaa15f);

    _task->SetLabel(&&__SIM__checkpoint_17244);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17244:

    // ~tail6174[1] >> ~l6297;
    _task->SetLabel(&&__SIM__receive17243);
    if (!((libpi::Session*)_this->var___SIM__tail6174.get())->Receive(0,_task,_this->var___SIM__l6297))
      return false;
    __SIM__receive17243:

    _task->SetLabel(&&__SIM__checkpoint_17246);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17246:

    // ~this6175[2] << (~l6297 + 1);
    { 
      shared_ptr<libpi::Int> __SIM__binop17247((*((libpi::Int*)_this->var___SIM__l6297.get())) + (*((libpi::Int*)intval_1.get())));
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,__SIM__binop17247);
    }

    _task->SetLabel(&&__SIM__checkpoint_17249);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17249:

    { Task___SIM__Cons14 *__SIM__task17248 = new Task___SIM__Cons14();
      __SIM__task17248->SetWorker(&_task->GetWorker());
      __SIM__task17248->SetArg0(_this->var___SIM__head6173);
      __SIM__task17248->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17248->SetArg2(_this->var___SIM__this6175);
      __SIM__task17248->SetArg3(_this->var___SIM__list6147);
      __SIM__task17248->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17248);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^pop")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17251);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17251:

    // ~this6175[2] << ^cons;
    ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,label_c4eb37d961ed515432d3dda9a05c0b8b);

    _task->SetLabel(&&__SIM__checkpoint_17253);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17253:

    // ~this6175[2] << ~head6173;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__head6173);
    }

    _task->SetLabel(&&__SIM__checkpoint_17255);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17255:

    // ~this6175[2] << ~tail6174;
    { 
      ((libpi::Session*)_this->var___SIM__this6175.get())->Send(1,_task,_this->var___SIM__tail6174);
    }
  _this->var___SIM__tail6174.reset();
  ((libpi::Session*)_this->var___SIM__this6175.get())->Close(true);
  _this->var___SIM__this6175.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^set")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17257);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17257:

    // ~this6175[2] >> ~i6298;
    _task->SetLabel(&&__SIM__receive17256);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__i6298))
      return false;
    __SIM__receive17256:

    _task->SetLabel(&&__SIM__checkpoint_17259);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17259:

    // ~this6175[2] >> ~v6299;
    _task->SetLabel(&&__SIM__receive17258);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__v6299))
      return false;
    __SIM__receive17258:

    _task->SetLabel(&&__SIM__checkpoint_17262);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17262:

    {
      shared_ptr<libpi::Bool> __SIM__binop17263((*((libpi::Int*)_this->var___SIM__i6298.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17263.get())->GetValue())
        goto __SIM____SIM__Cons14_true17260;
      else
        goto __SIM____SIM__Cons14_false17261;
    }
    __SIM____SIM__Cons14_true17260:

    _task->SetLabel(&&__SIM__checkpoint_17274);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17274:

    { Task___SIM__Cons14 *__SIM__task17273 = new Task___SIM__Cons14();
      __SIM__task17273->SetWorker(&_task->GetWorker());
      __SIM__task17273->SetArg0(_this->var___SIM__v6299);
      __SIM__task17273->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17273->SetArg2(_this->var___SIM__this6175);
      __SIM__task17273->SetArg3(_this->var___SIM__list6147);
      __SIM__task17273->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17273);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17261:

    _task->SetLabel(&&__SIM__checkpoint_17265);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17265:

    // ~tail6174[1] << ^set;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_9334638e4f24e41deb03eaf513559051);

    _task->SetLabel(&&__SIM__checkpoint_17267);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17267:

    // ~tail6174[1] << (~i6298 - 1);
    { 
      shared_ptr<libpi::Int> __SIM__binop17268((*((libpi::Int*)_this->var___SIM__i6298.get())) - (*((libpi::Int*)intval_1.get())));
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,__SIM__binop17268);
    }

    _task->SetLabel(&&__SIM__checkpoint_17270);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17270:

    // ~tail6174[1] << ~v6299;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__v6299);
    }

    _task->SetLabel(&&__SIM__checkpoint_17272);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17272:

    { Task___SIM__Cons14 *__SIM__task17271 = new Task___SIM__Cons14();
      __SIM__task17271->SetWorker(&_task->GetWorker());
      __SIM__task17271->SetArg0(_this->var___SIM__head6173);
      __SIM__task17271->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17271->SetArg2(_this->var___SIM__this6175);
      __SIM__task17271->SetArg3(_this->var___SIM__list6147);
      __SIM__task17271->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17271);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^snoc")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17277);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17277:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__Cons14_true17275;
      else
        goto __SIM____SIM__Cons14_false17276;
    }
    __SIM____SIM__Cons14_true17275:

    _task->SetLabel(&&__SIM__checkpoint_17290);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17290:

    // ~this6175[2] >> ~h6300;
    _task->SetLabel(&&__SIM__receive17289);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__h6300))
      return false;
    __SIM__receive17289:

    _task->SetLabel(&&__SIM__checkpoint_17292);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17292:

    // ~tail6174[1] << ^snoc;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_cdbea80e30ba932a7d0b51a4202d726a);

    _task->SetLabel(&&__SIM__checkpoint_17294);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17294:

    // ~tail6174[1] << ~h6300;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__h6300);
    }

    _task->SetLabel(&&__SIM__checkpoint_17296);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17296:

    { Task___SIM__Cons14 *__SIM__task17295 = new Task___SIM__Cons14();
      __SIM__task17295->SetWorker(&_task->GetWorker());
      __SIM__task17295->SetArg0(_this->var___SIM__head6173);
      __SIM__task17295->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17295->SetArg2(_this->var___SIM__this6175);
      __SIM__task17295->SetArg3(_this->var___SIM__list6147);
      __SIM__task17295->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17295);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Cons14_false17276:

    _task->SetLabel(&&__SIM__checkpoint_17282);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17282:

    // ~tail6174[1] << ^snoc;
    ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,label_cdbea80e30ba932a7d0b51a4202d726a);

    _task->SetLabel(&&__SIM__checkpoint_17284);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17284:

    // ~this6175[2] >> ~h6301;
    _task->SetLabel(&&__SIM__receive17283);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__h6301))
      return false;
    __SIM__receive17283:

    _task->SetLabel(&&__SIM__checkpoint_17286);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17286:

    // ~tail6174[1] << ~h6301;
    { 
      ((libpi::Session*)_this->var___SIM__tail6174.get())->Send(0,_task,_this->var___SIM__h6301);
    }

    _task->SetLabel(&&__SIM__checkpoint_17288);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17288:

    { Task___SIM__Cons14 *__SIM__task17287 = new Task___SIM__Cons14();
      __SIM__task17287->SetWorker(&_task->GetWorker());
      __SIM__task17287->SetArg0(_this->var___SIM__head6173);
      __SIM__task17287->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17287->SetArg2(_this->var___SIM__this6175);
      __SIM__task17287->SetArg3(_this->var___SIM__list6147);
      __SIM__task17287->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17287);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^takeover")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17298);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17298:

    // ~this6175[2] >> ~that6302;
    _task->SetLabel(&&__SIM__receive17297);
    if (!((libpi::Session*)_this->var___SIM__this6175.get())->Receive(1,_task,_this->var___SIM__that6302))
      return false;
    __SIM__receive17297:
    // Session complete
    ((libpi::Session*)_this->var___SIM__this6175.get())->Close(true);
    _this->var___SIM__this6175.reset();

    _task->SetLabel(&&__SIM__checkpoint_17300);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17300:

    { Task___SIM__Cons14 *__SIM__task17299 = new Task___SIM__Cons14();
      __SIM__task17299->SetWorker(&_task->GetWorker());
      __SIM__task17299->SetArg0(_this->var___SIM__head6173);
      __SIM__task17299->SetArg1(_this->var___SIM__tail6174);
      __SIM__task17299->SetArg2(_this->var___SIM__that6302);
      __SIM__task17299->SetArg3(_this->var___SIM__list6147);
      __SIM__task17299->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17299);
    }
    goto method___SIM__Cons14;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__Nil13: // {{{
  #undef _this
  #define _this ((Task___SIM__Nil13*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17302);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17302:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Nil1317301);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Nil1317301:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^append")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17304);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17304:

    // ~this6150[2] >> ~l6303;
    _task->SetLabel(&&__SIM__receive17303);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__l6303))
      return false;
    __SIM__receive17303:

    _task->SetLabel(&&__SIM__checkpoint_17306);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17306:

    // ~l6303[1] << ^takeover;
    ((libpi::Session*)_this->var___SIM__l6303.get())->Send(0,_task,label_55759b104bd58e2c0ac92ac12ef96ed4);

    _task->SetLabel(&&__SIM__checkpoint_17308);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17308:

    // ~l6303[1] << ~this6150;
    { 
      ((libpi::Session*)_this->var___SIM__l6303.get())->Send(0,_task,_this->var___SIM__this6150);
    }
  _this->var___SIM__this6150.reset();
  ((libpi::Session*)_this->var___SIM__l6303.get())->Close(true);
  _this->var___SIM__l6303.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^apply")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17310);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17310:

    // ~this6150[2] >> ~f6304;
    _task->SetLabel(&&__SIM__receive17309);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__f6304))
      return false;
    __SIM__receive17309:

    _task->SetLabel(&&__SIM__checkpoint_17312);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17312:

    { Task___SIM__Nil13 *__SIM__task17311 = new Task___SIM__Nil13();
      __SIM__task17311->SetWorker(&_task->GetWorker());
      __SIM__task17311->SetArg0(_this->var___SIM__this6150);
      __SIM__task17311->SetArg1(_this->var___SIM__list6147);
      __SIM__task17311->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17311);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^cons")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17314);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17314:

    // ~this6150[2] >> ~head6305;
    _task->SetLabel(&&__SIM__receive17313);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__head6305))
      return false;
    __SIM__receive17313:

    _task->SetLabel(&&__SIM__checkpoint_17315);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17315:

    {
      _task->tmps.clear();
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l16306_in17316;
    __SIM____SIM__l16306_in17316.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16306_in17316.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l16306_out17317;
    __SIM____SIM__l16306_out17317.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16306_out17317.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
      _this->var___SIM__l16306.reset(new libpi::Session(1, 2, __SIM____SIM__l16306_in17316,__SIM____SIM__l16306_out17317));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l26307_in17318;
    __SIM____SIM__l26307_in17318.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
    __SIM____SIM__l26307_in17318.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l26307_out17319;
    __SIM____SIM__l26307_out17319.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    __SIM____SIM__l26307_out17319.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
      _this->var___SIM__l26307.reset(new libpi::Session(2, 2, __SIM____SIM__l26307_in17318,__SIM____SIM__l26307_out17319));
      _this->tmps.clear();
    }

    _task->SetLabel(&&__SIM__checkpoint_17320);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17320:

    { Task___SIM__Nil13 *__SIM__task17321(new Task___SIM__Nil13());
      __SIM__task17321->SetWorker(&_task->GetWorker());
      __SIM__task17321->SetArg0(_this->var___SIM__l16306);
      __SIM__task17321->SetArg1(_this->var___SIM__list6147);
      __SIM__task17321->SetLabel(&&method___SIM__Nil13);
      shared_ptr<libpi::task::Task> __SIM__task17322(__SIM__task17321);
      _task->GetWorker().AddTask(__SIM__task17322);
    }

    _task->SetLabel(&&__SIM__checkpoint_17324);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17324:

    { Task___SIM__Cons14 *__SIM__task17323 = new Task___SIM__Cons14();
      __SIM__task17323->SetWorker(&_task->GetWorker());
      __SIM__task17323->SetArg0(_this->var___SIM__head6305);
      __SIM__task17323->SetArg1(_this->var___SIM__l26307);
      __SIM__task17323->SetArg2(_this->var___SIM__this6150);
      __SIM__task17323->SetArg3(_this->var___SIM__list6147);
      __SIM__task17323->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17323);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^copy")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17325);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17325:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Nil1317326);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__cpy6330))
        return false;
      __SIM__receive___SIM__Nil1317326:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17328);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17328:

    // ~this6150[2] << ~cpy6330;
    { 
      ((libpi::Session*)_this->var___SIM__this6150.get())->Send(1,_task,_this->var___SIM__cpy6330);
    }
  _this->var___SIM__cpy6330.reset();

    _task->SetLabel(&&__SIM__checkpoint_17330);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17330:

    { Task___SIM__Nil13 *__SIM__task17329 = new Task___SIM__Nil13();
      __SIM__task17329->SetWorker(&_task->GetWorker());
      __SIM__task17329->SetArg0(_this->var___SIM__this6150);
      __SIM__task17329->SetArg1(_this->var___SIM__list6147);
      __SIM__task17329->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17329);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__this6150.get())->Close(true);
    _this->var___SIM__this6150.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^erase")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17332);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17332:

    // ~this6150[2] >> ~i6331;
    _task->SetLabel(&&__SIM__receive17331);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__i6331))
      return false;
    __SIM__receive17331:

    _task->SetLabel(&&__SIM__checkpoint_17334);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17334:

    { Task___SIM__Nil13 *__SIM__task17333 = new Task___SIM__Nil13();
      __SIM__task17333->SetWorker(&_task->GetWorker());
      __SIM__task17333->SetArg0(_this->var___SIM__this6150);
      __SIM__task17333->SetArg1(_this->var___SIM__list6147);
      __SIM__task17333->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17333);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^filter")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17336);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17336:

    // ~this6150[2] >> ~filter6332;
    _task->SetLabel(&&__SIM__receive17335);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__filter6332))
      return false;
    __SIM__receive17335:

    _task->SetLabel(&&__SIM__checkpoint_17338);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17338:

    // ~filter6332[1] << ^end;
    ((libpi::Session*)_this->var___SIM__filter6332.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__filter6332.get())->Close(true);
    _this->var___SIM__filter6332.reset();

    _task->SetLabel(&&__SIM__checkpoint_17340);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17340:

    { Task___SIM__Nil13 *__SIM__task17339 = new Task___SIM__Nil13();
      __SIM__task17339->SetWorker(&_task->GetWorker());
      __SIM__task17339->SetArg0(_this->var___SIM__this6150);
      __SIM__task17339->SetArg1(_this->var___SIM__list6147);
      __SIM__task17339->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17339);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^get")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17342);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17342:

    // ~this6150[2] >> ~i6333;
    _task->SetLabel(&&__SIM__receive17341);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__i6333))
      return false;
    __SIM__receive17341:

    _task->SetLabel(&&__SIM__checkpoint_17344);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17344:

    // ~this6150[2] << ^err;
    ((libpi::Session*)_this->var___SIM__this6150.get())->Send(1,_task,label_a02d9bff9bc696a25d4e49dd4f301a44);

    _task->SetLabel(&&__SIM__checkpoint_17346);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17346:

    // ~this6150[2] << "Index out of bounds";
    { 
      ((libpi::Session*)_this->var___SIM__this6150.get())->Send(1,_task,stringval_046e3d1f08decc804b88d3b89cd6b186);
    }

    _task->SetLabel(&&__SIM__checkpoint_17348);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17348:

    { Task___SIM__Nil13 *__SIM__task17347 = new Task___SIM__Nil13();
      __SIM__task17347->SetWorker(&_task->GetWorker());
      __SIM__task17347->SetArg0(_this->var___SIM__this6150);
      __SIM__task17347->SetArg1(_this->var___SIM__list6147);
      __SIM__task17347->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17347);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^insert")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17350);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17350:

    // ~this6150[2] >> ~i6334;
    _task->SetLabel(&&__SIM__receive17349);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__i6334))
      return false;
    __SIM__receive17349:

    _task->SetLabel(&&__SIM__checkpoint_17352);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17352:

    // ~this6150[2] >> ~v6335;
    _task->SetLabel(&&__SIM__receive17351);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__v6335))
      return false;
    __SIM__receive17351:

    _task->SetLabel(&&__SIM__checkpoint_17355);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17355:

    {
      shared_ptr<libpi::Bool> __SIM__binop17356((*((libpi::Int*)_this->var___SIM__i6334.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17356.get())->GetValue())
        goto __SIM____SIM__Nil13_true17353;
      else
        goto __SIM____SIM__Nil13_false17354;
    }
    __SIM____SIM__Nil13_true17353:

    _task->SetLabel(&&__SIM__checkpoint_17359);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17359:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Nil1317360);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__tail6336))
        return false;
      __SIM__receive___SIM__Nil1317360:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17362);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17362:

    { Task___SIM__Cons14 *__SIM__task17361 = new Task___SIM__Cons14();
      __SIM__task17361->SetWorker(&_task->GetWorker());
      __SIM__task17361->SetArg0(_this->var___SIM__v6335);
      __SIM__task17361->SetArg1(_this->var___SIM__tail6336);
      __SIM__task17361->SetArg2(_this->var___SIM__this6150);
      __SIM__task17361->SetArg3(_this->var___SIM__list6147);
      __SIM__task17361->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17361);
    }
    goto method___SIM__Cons14;
    __SIM____SIM__Nil13_false17354:

    _task->SetLabel(&&__SIM__checkpoint_17358);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17358:

    { Task___SIM__Nil13 *__SIM__task17357 = new Task___SIM__Nil13();
      __SIM__task17357->SetWorker(&_task->GetWorker());
      __SIM__task17357->SetArg0(_this->var___SIM__this6150);
      __SIM__task17357->SetArg1(_this->var___SIM__list6147);
      __SIM__task17357->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17357);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^length")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17364);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17364:

    // ~this6150[2] << 0;
    { 
      ((libpi::Session*)_this->var___SIM__this6150.get())->Send(1,_task,intval_0);
    }

    _task->SetLabel(&&__SIM__checkpoint_17366);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17366:

    { Task___SIM__Nil13 *__SIM__task17365 = new Task___SIM__Nil13();
      __SIM__task17365->SetWorker(&_task->GetWorker());
      __SIM__task17365->SetArg0(_this->var___SIM__this6150);
      __SIM__task17365->SetArg1(_this->var___SIM__list6147);
      __SIM__task17365->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17365);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^pop")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17368);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17368:

    // ~this6150[2] << ^nil;
    ((libpi::Session*)_this->var___SIM__this6150.get())->Send(1,_task,label_6e5c72314d264d39de32bf8a473ee1d3);
    ((libpi::Session*)_this->var___SIM__this6150.get())->Close(true);
    _this->var___SIM__this6150.reset();
    return false;

    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^set")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17370);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17370:

    // ~this6150[2] >> ~i6337;
    _task->SetLabel(&&__SIM__receive17369);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__i6337))
      return false;
    __SIM__receive17369:

    _task->SetLabel(&&__SIM__checkpoint_17372);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17372:

    // ~this6150[2] >> ~v6338;
    _task->SetLabel(&&__SIM__receive17371);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__v6338))
      return false;
    __SIM__receive17371:

    _task->SetLabel(&&__SIM__checkpoint_17374);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17374:

    { Task___SIM__Nil13 *__SIM__task17373 = new Task___SIM__Nil13();
      __SIM__task17373->SetWorker(&_task->GetWorker());
      __SIM__task17373->SetArg0(_this->var___SIM__this6150);
      __SIM__task17373->SetArg1(_this->var___SIM__list6147);
      __SIM__task17373->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17373);
    }
    goto method___SIM__Nil13;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^snoc")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17376);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17376:

    // ~this6150[2] >> ~head6339;
    _task->SetLabel(&&__SIM__receive17375);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__head6339))
      return false;
    __SIM__receive17375:

    _task->SetLabel(&&__SIM__checkpoint_17377);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17377:

    {
      _task->tmps.clear();
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
      _task->tmps.push_back(std::shared_ptr<libpi::Value>(new libpi::task::Channel()));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l16340_in17378;
    __SIM____SIM__l16340_in17378.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16340_in17378.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l16340_out17379;
    __SIM____SIM__l16340_out17379.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]));
    __SIM____SIM__l16340_out17379.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
      _this->var___SIM__l16340.reset(new libpi::Session(1, 2, __SIM____SIM__l16340_in17378,__SIM____SIM__l16340_out17379));
    vector<shared_ptr<libpi::Channel >> __SIM____SIM__l26341_in17380;
    __SIM____SIM__l26341_in17380.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[2]));
    __SIM____SIM__l26341_in17380.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
    vector<shared_ptr<libpi::Channel> > __SIM____SIM__l26341_out17381;
    __SIM____SIM__l26341_out17381.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[1]));
    __SIM____SIM__l26341_out17381.push_back(dynamic_pointer_cast<libpi::Channel>(_task->tmps[3]));
      _this->var___SIM__l26341.reset(new libpi::Session(2, 2, __SIM____SIM__l26341_in17380,__SIM____SIM__l26341_out17381));
      _this->tmps.clear();
    }

    _task->SetLabel(&&__SIM__checkpoint_17382);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17382:

    { Task___SIM__Nil13 *__SIM__task17383(new Task___SIM__Nil13());
      __SIM__task17383->SetWorker(&_task->GetWorker());
      __SIM__task17383->SetArg0(_this->var___SIM__l16340);
      __SIM__task17383->SetArg1(_this->var___SIM__list6147);
      __SIM__task17383->SetLabel(&&method___SIM__Nil13);
      shared_ptr<libpi::task::Task> __SIM__task17384(__SIM__task17383);
      _task->GetWorker().AddTask(__SIM__task17384);
    }

    _task->SetLabel(&&__SIM__checkpoint_17386);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17386:

    { Task___SIM__Cons14 *__SIM__task17385 = new Task___SIM__Cons14();
      __SIM__task17385->SetWorker(&_task->GetWorker());
      __SIM__task17385->SetArg0(_this->var___SIM__head6339);
      __SIM__task17385->SetArg1(_this->var___SIM__l26341);
      __SIM__task17385->SetArg2(_this->var___SIM__this6150);
      __SIM__task17385->SetArg3(_this->var___SIM__list6147);
      __SIM__task17385->SetLabel(&&method___SIM__Cons14);
      _task.reset(__SIM__task17385);
    }
    goto method___SIM__Cons14;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^takeover")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17388);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17388:

    // ~this6150[2] >> ~that6364;
    _task->SetLabel(&&__SIM__receive17387);
    if (!((libpi::Session*)_this->var___SIM__this6150.get())->Receive(1,_task,_this->var___SIM__that6364))
      return false;
    __SIM__receive17387:
    // Session complete
    ((libpi::Session*)_this->var___SIM__this6150.get())->Close(true);
    _this->var___SIM__this6150.reset();

    _task->SetLabel(&&__SIM__checkpoint_17390);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17390:

    { Task___SIM__Nil13 *__SIM__task17389 = new Task___SIM__Nil13();
      __SIM__task17389->SetWorker(&_task->GetWorker());
      __SIM__task17389->SetArg0(_this->var___SIM__that6364);
      __SIM__task17389->SetArg1(_this->var___SIM__list6147);
      __SIM__task17389->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17389);
    }
    goto method___SIM__Nil13;
    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight28: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight28*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17392);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17392:

    { Task___SIM__Nil13 *__SIM__task17391 = new Task___SIM__Nil13();
      __SIM__task17391->SetWorker(&_task->GetWorker());
      __SIM__task17391->SetArg0(_this->var___SIM__s6148);
      __SIM__task17391->SetArg1(_this->var___SIM__list6147);
      __SIM__task17391->SetLabel(&&method___SIM__Nil13);
      _task.reset(__SIM__task17391);
    }
    goto method___SIM__Nil13;
  } // }}}
  method___SIM__List12: // {{{
  #undef _this
  #define _this ((Task___SIM__List12*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17393);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17393:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__List1217394);
        if (!((libpi::task::Link*)_this->var___SIM__list6147.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__List1217394:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6148.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_17395);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17395:

    { Task___SIM__FlatRight28 *__SIM__task17396(new Task___SIM__FlatRight28());
      __SIM__task17396->SetWorker(&_task->GetWorker());
      __SIM__task17396->SetArg0(_this->var___SIM__list6147);
      __SIM__task17396->SetArg1(_this->var___SIM__s6148);
      __SIM__task17396->SetLabel(&&method___SIM__FlatRight28);
      shared_ptr<libpi::task::Task> __SIM__task17397(__SIM__task17396);
      _task->GetWorker().AddTask(__SIM__task17397);
    }

    _task->SetLabel(&&__SIM__checkpoint_17399);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17399:

    { Task___SIM__List12 *__SIM__task17398 = new Task___SIM__List12();
      __SIM__task17398->SetWorker(&_task->GetWorker());
      __SIM__task17398->SetArg0(_this->var___SIM__list6147);
      __SIM__task17398->SetLabel(&&method___SIM__List12);
      _task.reset(__SIM__task17398);
    }
    goto method___SIM__List12;
  } // }}}
  method___SIM__StartService15: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService15*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17402);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17402:

    {
      shared_ptr<libpi::Bool> __SIM__binop17403((*((libpi::Int*)_this->var___SIM__i6365.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17403.get())->GetValue())
        goto __SIM____SIM__StartService15_true17400;
      else
        goto __SIM____SIM__StartService15_false17401;
    }
    __SIM____SIM__StartService15_true17400:

    _task->SetLabel(&&__SIM__checkpoint_17411);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17411:

    { Task___SIM__List12 *__SIM__task17410 = new Task___SIM__List12();
      __SIM__task17410->SetWorker(&_task->GetWorker());
      __SIM__task17410->SetArg0(_this->var___SIM__list6147);
      __SIM__task17410->SetLabel(&&method___SIM__List12);
      _task.reset(__SIM__task17410);
    }
    goto method___SIM__List12;
    __SIM____SIM__StartService15_false17401:

    _task->SetLabel(&&__SIM__checkpoint_17404);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17404:

    { Task___SIM__StartService15 *__SIM__task17405(new Task___SIM__StartService15());
      __SIM__task17405->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop17409((*((libpi::Int*)_this->var___SIM__i6365.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task17405->SetArg0(__SIM__binop17409);
      __SIM__task17405->SetArg1(_this->var___SIM__list6147);
      __SIM__task17405->SetLabel(&&method___SIM__StartService15);
      shared_ptr<libpi::task::Task> __SIM__task17406(__SIM__task17405);
      _task->GetWorker().AddTask(__SIM__task17406);
    }

    _task->SetLabel(&&__SIM__checkpoint_17408);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17408:

    { Task___SIM__List12 *__SIM__task17407 = new Task___SIM__List12();
      __SIM__task17407->SetWorker(&_task->GetWorker());
      __SIM__task17407->SetArg0(_this->var___SIM__list6147);
      __SIM__task17407->SetLabel(&&method___SIM__List12);
      _task.reset(__SIM__task17407);
    }
    goto method___SIM__List12;
  } // }}}
  method___SIM__FlatRight29: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight29*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17414);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17414:

    {
      if (libpi::task::Worker::TargetTasks<=((libpi::task::Worker_Pool*)&_task->GetWorker())->GetActiveTasks().size())
        goto __SIM____SIM__FlatRight29_true17412;
      else
        goto __SIM____SIM__FlatRight29_false17413;
    }
    __SIM____SIM__FlatRight29_true17412:

    _task->SetLabel(&&__SIM__checkpoint_17478);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17478:

    // ~s6367[2] >> ~next6369;
    _task->SetLabel(&&__SIM__receive17477);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__next6369))
      return false;
    __SIM__receive17477:

    _task->SetLabel(&&__SIM__checkpoint_17480);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17480:

    // ~s6367[2] >> ~cmp6370;
    _task->SetLabel(&&__SIM__receive17479);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__cmp6370))
      return false;
    __SIM__receive17479:

    _task->SetLabel(&&__SIM__checkpoint_17482);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17482:

    // ~s6367[2] >> ~from6371;
    _task->SetLabel(&&__SIM__receive17481);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__from6371))
      return false;
    __SIM__receive17481:

    _task->SetLabel(&&__SIM__checkpoint_17484);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17484:

    // ~s6367[2] >> ~to6372;
    _task->SetLabel(&&__SIM__receive17483);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__to6372))
      return false;
    __SIM__receive17483:

    _task->SetLabel(&&__SIM__checkpoint_17486);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17486:

    // ~cmp6370[1] << ^leq;
    ((libpi::Session*)_this->var___SIM__cmp6370.get())->Send(0,_task,label_f693b54352d7c81bcd5f4e46fc8ea4f5);

    _task->SetLabel(&&__SIM__checkpoint_17488);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17488:

    // ~cmp6370[1] << ~from6371;
    { 
      ((libpi::Session*)_this->var___SIM__cmp6370.get())->Send(0,_task,_this->var___SIM__from6371);
    }

    _task->SetLabel(&&__SIM__checkpoint_17490);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17490:

    // ~cmp6370[1] << ~to6372;
    { 
      ((libpi::Session*)_this->var___SIM__cmp6370.get())->Send(0,_task,_this->var___SIM__to6372);
    }

    _task->SetLabel(&&__SIM__checkpoint_17492);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17492:

    // ~cmp6370[1] >> ~leq6373;
    _task->SetLabel(&&__SIM__receive17491);
    if (!((libpi::Session*)_this->var___SIM__cmp6370.get())->Receive(0,_task,_this->var___SIM__leq6373))
      return false;
    __SIM__receive17491:

    _task->SetLabel(&&__SIM__checkpoint_17495);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17495:

    {
      if (((libpi::Bool*)_this->var___SIM__leq6373.get())->GetValue())
        goto __SIM____SIM__FlatRight29_true17493;
      else
        goto __SIM____SIM__FlatRight29_false17494;
    }
    __SIM____SIM__FlatRight29_true17493:

    _task->SetLabel(&&__SIM__checkpoint_17507);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17507:

    // ~next6369[1] << ^call;
    ((libpi::Session*)_this->var___SIM__next6369.get())->Send(0,_task,label_0e5493ddbd005506c18850a500dfea96);

    _task->SetLabel(&&__SIM__checkpoint_17509);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17509:

    // ~next6369[1] << ~from6371;
    { 
      ((libpi::Session*)_this->var___SIM__next6369.get())->Send(0,_task,_this->var___SIM__from6371);
    }

    _task->SetLabel(&&__SIM__checkpoint_17511);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17511:

    // ~next6369[1] >> ~succ6374;
    _task->SetLabel(&&__SIM__receive17510);
    if (!((libpi::Session*)_this->var___SIM__next6369.get())->Receive(0,_task,_this->var___SIM__succ6374))
      return false;
    __SIM__receive17510:

    _task->SetLabel(&&__SIM__checkpoint_17512);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17512:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__range6366.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight2917513);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__tail6375))
        return false;
      __SIM__receive___SIM__FlatRight2917513:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17515);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17515:

    // ~tail6375[1] << ~next6369;
    { 
      ((libpi::Session*)_this->var___SIM__tail6375.get())->Send(0,_task,_this->var___SIM__next6369);
    }
  _this->var___SIM__next6369.reset();

    _task->SetLabel(&&__SIM__checkpoint_17517);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17517:

    // ~tail6375[1] << ~cmp6370;
    { 
      ((libpi::Session*)_this->var___SIM__tail6375.get())->Send(0,_task,_this->var___SIM__cmp6370);
    }
  _this->var___SIM__cmp6370.reset();

    _task->SetLabel(&&__SIM__checkpoint_17519);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17519:

    // ~tail6375[1] << ~succ6374;
    { 
      ((libpi::Session*)_this->var___SIM__tail6375.get())->Send(0,_task,_this->var___SIM__succ6374);
    }

    _task->SetLabel(&&__SIM__checkpoint_17521);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17521:

    // ~tail6375[1] << ~to6372;
    { 
      ((libpi::Session*)_this->var___SIM__tail6375.get())->Send(0,_task,_this->var___SIM__to6372);
    }

    _task->SetLabel(&&__SIM__checkpoint_17522);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17522:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight2917523);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__res6376))
        return false;
      __SIM__receive___SIM__FlatRight2917523:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17525);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17525:

    // ~res6376[1] << ^cons;
    ((libpi::Session*)_this->var___SIM__res6376.get())->Send(0,_task,label_c4eb37d961ed515432d3dda9a05c0b8b);

    _task->SetLabel(&&__SIM__checkpoint_17527);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17527:

    // ~res6376[1] << ~from6371;
    { 
      ((libpi::Session*)_this->var___SIM__res6376.get())->Send(0,_task,_this->var___SIM__from6371);
    }

    _task->SetLabel(&&__SIM__checkpoint_17529);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17529:

    // ~res6376[1] << ^append;
    ((libpi::Session*)_this->var___SIM__res6376.get())->Send(0,_task,label_6a5f1f3d587b4238521a743687b6780f);

    _task->SetLabel(&&__SIM__checkpoint_17531);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17531:

    // ~res6376[1] << ~tail6375;
    { 
      ((libpi::Session*)_this->var___SIM__res6376.get())->Send(0,_task,_this->var___SIM__tail6375);
    }
  _this->var___SIM__tail6375.reset();

    _task->SetLabel(&&__SIM__checkpoint_17533);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17533:

    // ~res6376[1] << ^takeover;
    ((libpi::Session*)_this->var___SIM__res6376.get())->Send(0,_task,label_55759b104bd58e2c0ac92ac12ef96ed4);

    _task->SetLabel(&&__SIM__checkpoint_17535);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17535:

    // ~res6376[1] << ~s6367;
    { 
      ((libpi::Session*)_this->var___SIM__res6376.get())->Send(0,_task,_this->var___SIM__s6367);
    }
  _this->var___SIM__s6367.reset();
  ((libpi::Session*)_this->var___SIM__res6376.get())->Close(true);
  _this->var___SIM__res6376.reset();
    return false;

    __SIM____SIM__FlatRight29_false17494:

    _task->SetLabel(&&__SIM__checkpoint_17497);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17497:

    // ~next6369[1] << ^end;
    ((libpi::Session*)_this->var___SIM__next6369.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__next6369.get())->Close(true);
    _this->var___SIM__next6369.reset();

    _task->SetLabel(&&__SIM__checkpoint_17499);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17499:

    // ~cmp6370[1] << ^end;
    ((libpi::Session*)_this->var___SIM__cmp6370.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__cmp6370.get())->Close(true);
    _this->var___SIM__cmp6370.reset();

    _task->SetLabel(&&__SIM__checkpoint_17500);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17500:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight2917501);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__res6377))
        return false;
      __SIM__receive___SIM__FlatRight2917501:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17503);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17503:

    // ~res6377[1] << ^takeover;
    ((libpi::Session*)_this->var___SIM__res6377.get())->Send(0,_task,label_55759b104bd58e2c0ac92ac12ef96ed4);

    _task->SetLabel(&&__SIM__checkpoint_17505);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17505:

    // ~res6377[1] << ~s6367;
    { 
      ((libpi::Session*)_this->var___SIM__res6377.get())->Send(0,_task,_this->var___SIM__s6367);
    }
  _this->var___SIM__s6367.reset();
  ((libpi::Session*)_this->var___SIM__res6377.get())->Close(true);
  _this->var___SIM__res6377.reset();
    return false;

    __SIM____SIM__FlatRight29_false17413:

    _task->SetLabel(&&__SIM__checkpoint_17419);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17419:

    // ~s6367[2] >> ~next6379;
    _task->SetLabel(&&__SIM__receive17418);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__next6379))
      return false;
    __SIM__receive17418:

    _task->SetLabel(&&__SIM__checkpoint_17421);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17421:

    // ~s6367[2] >> ~cmp6380;
    _task->SetLabel(&&__SIM__receive17420);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__cmp6380))
      return false;
    __SIM__receive17420:

    _task->SetLabel(&&__SIM__checkpoint_17423);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17423:

    // ~cmp6380[1] << ^leq;
    ((libpi::Session*)_this->var___SIM__cmp6380.get())->Send(0,_task,label_f693b54352d7c81bcd5f4e46fc8ea4f5);

    _task->SetLabel(&&__SIM__checkpoint_17425);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17425:

    // ~s6367[2] >> ~from6381;
    _task->SetLabel(&&__SIM__receive17424);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__from6381))
      return false;
    __SIM__receive17424:

    _task->SetLabel(&&__SIM__checkpoint_17427);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17427:

    // ~cmp6380[1] << ~from6381;
    { 
      ((libpi::Session*)_this->var___SIM__cmp6380.get())->Send(0,_task,_this->var___SIM__from6381);
    }

    _task->SetLabel(&&__SIM__checkpoint_17429);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17429:

    // ~s6367[2] >> ~to6382;
    _task->SetLabel(&&__SIM__receive17428);
    if (!((libpi::Session*)_this->var___SIM__s6367.get())->Receive(1,_task,_this->var___SIM__to6382))
      return false;
    __SIM__receive17428:

    _task->SetLabel(&&__SIM__checkpoint_17431);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17431:

    // ~cmp6380[1] << ~to6382;
    { 
      ((libpi::Session*)_this->var___SIM__cmp6380.get())->Send(0,_task,_this->var___SIM__to6382);
    }

    _task->SetLabel(&&__SIM__checkpoint_17433);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17433:

    // ~cmp6380[1] >> ~leq6383;
    _task->SetLabel(&&__SIM__receive17432);
    if (!((libpi::Session*)_this->var___SIM__cmp6380.get())->Receive(0,_task,_this->var___SIM__leq6383))
      return false;
    __SIM__receive17432:

    _task->SetLabel(&&__SIM__checkpoint_17436);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17436:

    {
      if (((libpi::Bool*)_this->var___SIM__leq6383.get())->GetValue())
        goto __SIM____SIM__FlatRight29_true17434;
      else
        goto __SIM____SIM__FlatRight29_false17435;
    }
    __SIM____SIM__FlatRight29_true17434:

    _task->SetLabel(&&__SIM__checkpoint_17448);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17448:

    // ~next6379[1] << ^call;
    ((libpi::Session*)_this->var___SIM__next6379.get())->Send(0,_task,label_0e5493ddbd005506c18850a500dfea96);

    _task->SetLabel(&&__SIM__checkpoint_17450);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17450:

    // ~next6379[1] << ~from6381;
    { 
      ((libpi::Session*)_this->var___SIM__next6379.get())->Send(0,_task,_this->var___SIM__from6381);
    }

    _task->SetLabel(&&__SIM__checkpoint_17452);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17452:

    // ~next6379[1] >> ~succ6384;
    _task->SetLabel(&&__SIM__receive17451);
    if (!((libpi::Session*)_this->var___SIM__next6379.get())->Receive(0,_task,_this->var___SIM__succ6384))
      return false;
    __SIM__receive17451:

    _task->SetLabel(&&__SIM__checkpoint_17453);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17453:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__range6366.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight2917454);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__tail6385))
        return false;
      __SIM__receive___SIM__FlatRight2917454:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17456);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17456:

    // ~tail6385[1] << ~next6379;
    { 
      ((libpi::Session*)_this->var___SIM__tail6385.get())->Send(0,_task,_this->var___SIM__next6379);
    }
  _this->var___SIM__next6379.reset();

    _task->SetLabel(&&__SIM__checkpoint_17458);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17458:

    // ~tail6385[1] << ~cmp6380;
    { 
      ((libpi::Session*)_this->var___SIM__tail6385.get())->Send(0,_task,_this->var___SIM__cmp6380);
    }
  _this->var___SIM__cmp6380.reset();

    _task->SetLabel(&&__SIM__checkpoint_17460);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17460:

    // ~tail6385[1] << ~succ6384;
    { 
      ((libpi::Session*)_this->var___SIM__tail6385.get())->Send(0,_task,_this->var___SIM__succ6384);
    }

    _task->SetLabel(&&__SIM__checkpoint_17462);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17462:

    // ~tail6385[1] << ~to6382;
    { 
      ((libpi::Session*)_this->var___SIM__tail6385.get())->Send(0,_task,_this->var___SIM__to6382);
    }

    _task->SetLabel(&&__SIM__checkpoint_17463);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17463:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight2917464);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__res6386))
        return false;
      __SIM__receive___SIM__FlatRight2917464:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17466);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17466:

    // ~res6386[1] << ^cons;
    ((libpi::Session*)_this->var___SIM__res6386.get())->Send(0,_task,label_c4eb37d961ed515432d3dda9a05c0b8b);

    _task->SetLabel(&&__SIM__checkpoint_17468);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17468:

    // ~res6386[1] << ~from6381;
    { 
      ((libpi::Session*)_this->var___SIM__res6386.get())->Send(0,_task,_this->var___SIM__from6381);
    }

    _task->SetLabel(&&__SIM__checkpoint_17470);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17470:

    // ~res6386[1] << ^append;
    ((libpi::Session*)_this->var___SIM__res6386.get())->Send(0,_task,label_6a5f1f3d587b4238521a743687b6780f);

    _task->SetLabel(&&__SIM__checkpoint_17472);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17472:

    // ~res6386[1] << ~tail6385;
    { 
      ((libpi::Session*)_this->var___SIM__res6386.get())->Send(0,_task,_this->var___SIM__tail6385);
    }
  _this->var___SIM__tail6385.reset();

    _task->SetLabel(&&__SIM__checkpoint_17474);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17474:

    // ~res6386[1] << ^takeover;
    ((libpi::Session*)_this->var___SIM__res6386.get())->Send(0,_task,label_55759b104bd58e2c0ac92ac12ef96ed4);

    _task->SetLabel(&&__SIM__checkpoint_17476);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17476:

    // ~res6386[1] << ~s6367;
    { 
      ((libpi::Session*)_this->var___SIM__res6386.get())->Send(0,_task,_this->var___SIM__s6367);
    }
  _this->var___SIM__s6367.reset();
  ((libpi::Session*)_this->var___SIM__res6386.get())->Close(true);
  _this->var___SIM__res6386.reset();
    return false;

    __SIM____SIM__FlatRight29_false17435:

    _task->SetLabel(&&__SIM__checkpoint_17438);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17438:

    // ~next6379[1] << ^end;
    ((libpi::Session*)_this->var___SIM__next6379.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__next6379.get())->Close(true);
    _this->var___SIM__next6379.reset();

    _task->SetLabel(&&__SIM__checkpoint_17440);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17440:

    // ~cmp6380[1] << ^end;
    ((libpi::Session*)_this->var___SIM__cmp6380.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__cmp6380.get())->Close(true);
    _this->var___SIM__cmp6380.reset();

    _task->SetLabel(&&__SIM__checkpoint_17441);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17441:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__list6147.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight2917442);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__res6387))
        return false;
      __SIM__receive___SIM__FlatRight2917442:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17444);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17444:

    // ~res6387[1] << ^takeover;
    ((libpi::Session*)_this->var___SIM__res6387.get())->Send(0,_task,label_55759b104bd58e2c0ac92ac12ef96ed4);

    _task->SetLabel(&&__SIM__checkpoint_17446);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17446:

    // ~res6387[1] << ~s6367;
    { 
      ((libpi::Session*)_this->var___SIM__res6387.get())->Send(0,_task,_this->var___SIM__s6367);
    }
  _this->var___SIM__s6367.reset();
  ((libpi::Session*)_this->var___SIM__res6387.get())->Close(true);
  _this->var___SIM__res6387.reset();
    return false;

  } // }}}
  method___SIM__Range16: // {{{
  #undef _this
  #define _this ((Task___SIM__Range16*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17536);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17536:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__Range1617537);
        if (!((libpi::task::Link*)_this->var___SIM__range6366.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__Range1617537:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6367.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_17538);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17538:

    { Task___SIM__FlatRight29 *__SIM__task17539(new Task___SIM__FlatRight29());
      __SIM__task17539->SetWorker(&_task->GetWorker());
      __SIM__task17539->SetArg0(_this->var___SIM__list6147);
      __SIM__task17539->SetArg1(_this->var___SIM__range6366);
      __SIM__task17539->SetArg2(_this->var___SIM__s6367);
      __SIM__task17539->SetLabel(&&method___SIM__FlatRight29);
      shared_ptr<libpi::task::Task> __SIM__task17540(__SIM__task17539);
      _task->GetWorker().AddTask(__SIM__task17540);
    }

    _task->SetLabel(&&__SIM__checkpoint_17542);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17542:

    { Task___SIM__Range16 *__SIM__task17541 = new Task___SIM__Range16();
      __SIM__task17541->SetWorker(&_task->GetWorker());
      __SIM__task17541->SetArg0(_this->var___SIM__list6147);
      __SIM__task17541->SetArg1(_this->var___SIM__range6366);
      __SIM__task17541->SetLabel(&&method___SIM__Range16);
      _task.reset(__SIM__task17541);
    }
    goto method___SIM__Range16;
  } // }}}
  method___SIM__StartService17: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService17*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17545);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17545:

    {
      shared_ptr<libpi::Bool> __SIM__binop17546((*((libpi::Int*)_this->var___SIM__i6388.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17546.get())->GetValue())
        goto __SIM____SIM__StartService17_true17543;
      else
        goto __SIM____SIM__StartService17_false17544;
    }
    __SIM____SIM__StartService17_true17543:

    _task->SetLabel(&&__SIM__checkpoint_17554);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17554:

    { Task___SIM__Range16 *__SIM__task17553 = new Task___SIM__Range16();
      __SIM__task17553->SetWorker(&_task->GetWorker());
      __SIM__task17553->SetArg0(_this->var___SIM__list6147);
      __SIM__task17553->SetArg1(_this->var___SIM__range6366);
      __SIM__task17553->SetLabel(&&method___SIM__Range16);
      _task.reset(__SIM__task17553);
    }
    goto method___SIM__Range16;
    __SIM____SIM__StartService17_false17544:

    _task->SetLabel(&&__SIM__checkpoint_17547);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17547:

    { Task___SIM__StartService17 *__SIM__task17548(new Task___SIM__StartService17());
      __SIM__task17548->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop17552((*((libpi::Int*)_this->var___SIM__i6388.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task17548->SetArg0(__SIM__binop17552);
      __SIM__task17548->SetArg1(_this->var___SIM__list6147);
      __SIM__task17548->SetArg2(_this->var___SIM__range6366);
      __SIM__task17548->SetLabel(&&method___SIM__StartService17);
      shared_ptr<libpi::task::Task> __SIM__task17549(__SIM__task17548);
      _task->GetWorker().AddTask(__SIM__task17549);
    }

    _task->SetLabel(&&__SIM__checkpoint_17551);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17551:

    { Task___SIM__Range16 *__SIM__task17550 = new Task___SIM__Range16();
      __SIM__task17550->SetWorker(&_task->GetWorker());
      __SIM__task17550->SetArg0(_this->var___SIM__list6147);
      __SIM__task17550->SetArg1(_this->var___SIM__range6366);
      __SIM__task17550->SetLabel(&&method___SIM__Range16);
      _task.reset(__SIM__task17550);
    }
    goto method___SIM__Range16;
  } // }}}
  method___SIM__X19: // {{{
  #undef _this
  #define _this ((Task___SIM__X19*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17556);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17556:

    _task->SetLabel(&&__SIM__checkpoint___SIM__X1917555);
    if (!((libpi::Session*)_this->var___SIM__this6391.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__X1917555:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^call")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17558);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17558:

    // ~this6391[2] >> ~n6394;
    _task->SetLabel(&&__SIM__receive17557);
    if (!((libpi::Session*)_this->var___SIM__this6391.get())->Receive(1,_task,_this->var___SIM__n6394))
      return false;
    __SIM__receive17557:

    _task->SetLabel(&&__SIM__checkpoint_17560);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17560:

    // ~this6391[2] << (~n6394 + 1);
    { 
      shared_ptr<libpi::Int> __SIM__binop17561((*((libpi::Int*)_this->var___SIM__n6394.get())) + (*((libpi::Int*)intval_1.get())));
      ((libpi::Session*)_this->var___SIM__this6391.get())->Send(1,_task,__SIM__binop17561);
    }

    _task->SetLabel(&&__SIM__checkpoint_17563);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17563:

    { Task___SIM__X19 *__SIM__task17562 = new Task___SIM__X19();
      __SIM__task17562->SetWorker(&_task->GetWorker());
      __SIM__task17562->SetArg0(_this->var___SIM__this6391);
      __SIM__task17562->SetLabel(&&method___SIM__X19);
      _task.reset(__SIM__task17562);
    }
    goto method___SIM__X19;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__this6391.get())->Close(true);
    _this->var___SIM__this6391.reset();
    return false;

    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight30: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight30*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17565);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17565:

    { Task___SIM__X19 *__SIM__task17564 = new Task___SIM__X19();
      __SIM__task17564->SetWorker(&_task->GetWorker());
      __SIM__task17564->SetArg0(_this->var___SIM__s6390);
      __SIM__task17564->SetLabel(&&method___SIM__X19);
      _task.reset(__SIM__task17564);
    }
    goto method___SIM__X19;
  } // }}}
  method___SIM__StdSucc_Int18: // {{{
  #undef _this
  #define _this ((Task___SIM__StdSucc_Int18*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17566);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17566:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__StdSucc_Int1817567);
        if (!((libpi::task::Link*)_this->var___SIM__stdsucc_Int6389.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__StdSucc_Int1817567:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6390.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_17568);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17568:

    { Task___SIM__FlatRight30 *__SIM__task17569(new Task___SIM__FlatRight30());
      __SIM__task17569->SetWorker(&_task->GetWorker());
      __SIM__task17569->SetArg0(_this->var___SIM__s6390);
      __SIM__task17569->SetLabel(&&method___SIM__FlatRight30);
      shared_ptr<libpi::task::Task> __SIM__task17570(__SIM__task17569);
      _task->GetWorker().AddTask(__SIM__task17570);
    }

    _task->SetLabel(&&__SIM__checkpoint_17572);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17572:

    { Task___SIM__StdSucc_Int18 *__SIM__task17571 = new Task___SIM__StdSucc_Int18();
      __SIM__task17571->SetWorker(&_task->GetWorker());
      __SIM__task17571->SetArg0(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17571->SetLabel(&&method___SIM__StdSucc_Int18);
      _task.reset(__SIM__task17571);
    }
    goto method___SIM__StdSucc_Int18;
  } // }}}
  method___SIM__StartService20: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService20*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17575);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17575:

    {
      shared_ptr<libpi::Bool> __SIM__binop17576((*((libpi::Int*)_this->var___SIM__i6395.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17576.get())->GetValue())
        goto __SIM____SIM__StartService20_true17573;
      else
        goto __SIM____SIM__StartService20_false17574;
    }
    __SIM____SIM__StartService20_true17573:

    _task->SetLabel(&&__SIM__checkpoint_17584);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17584:

    { Task___SIM__StdSucc_Int18 *__SIM__task17583 = new Task___SIM__StdSucc_Int18();
      __SIM__task17583->SetWorker(&_task->GetWorker());
      __SIM__task17583->SetArg0(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17583->SetLabel(&&method___SIM__StdSucc_Int18);
      _task.reset(__SIM__task17583);
    }
    goto method___SIM__StdSucc_Int18;
    __SIM____SIM__StartService20_false17574:

    _task->SetLabel(&&__SIM__checkpoint_17577);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17577:

    { Task___SIM__StartService20 *__SIM__task17578(new Task___SIM__StartService20());
      __SIM__task17578->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop17582((*((libpi::Int*)_this->var___SIM__i6395.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task17578->SetArg0(__SIM__binop17582);
      __SIM__task17578->SetArg1(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17578->SetLabel(&&method___SIM__StartService20);
      shared_ptr<libpi::task::Task> __SIM__task17579(__SIM__task17578);
      _task->GetWorker().AddTask(__SIM__task17579);
    }

    _task->SetLabel(&&__SIM__checkpoint_17581);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17581:

    { Task___SIM__StdSucc_Int18 *__SIM__task17580 = new Task___SIM__StdSucc_Int18();
      __SIM__task17580->SetWorker(&_task->GetWorker());
      __SIM__task17580->SetArg0(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17580->SetLabel(&&method___SIM__StdSucc_Int18);
      _task.reset(__SIM__task17580);
    }
    goto method___SIM__StdSucc_Int18;
  } // }}}
  method___SIM__Filter22: // {{{
  #undef _this
  #define _this ((Task___SIM__Filter22*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17586);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17586:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Filter2217585);
    if (!((libpi::Session*)_this->var___SIM__this6399.get())->Receive(1,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Filter2217585:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^call")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17588);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17588:

    // ~this6399[2] >> ~n6402;
    _task->SetLabel(&&__SIM__receive17587);
    if (!((libpi::Session*)_this->var___SIM__this6399.get())->Receive(1,_task,_this->var___SIM__n6402))
      return false;
    __SIM__receive17587:

    _task->SetLabel(&&__SIM__checkpoint_17590);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17590:

    // ~this6399[2] << not (((~n6402 / ~d6398) * ~d6398) = ~n6402);
    { 
      shared_ptr<libpi::Int> __SIM__binop17594((*((libpi::Int*)_this->var___SIM__n6402.get())) / (*((libpi::Int*)_this->var___SIM__d6398.get())));
      shared_ptr<libpi::Int> __SIM__binop17593((*((libpi::Int*)__SIM__binop17594.get())) * (*((libpi::Int*)_this->var___SIM__d6398.get())));
      shared_ptr<libpi::Bool> __SIM__binop17592((*((libpi::Int*)__SIM__binop17593.get())) == (*((libpi::Int*)_this->var___SIM__n6402.get())));
      shared_ptr<libpi::Bool> __SIM__unop17591=libpi::Bool::GetInstance(!((libpi::Bool*)__SIM__binop17592.get())->GetValue());
      ((libpi::Session*)_this->var___SIM__this6399.get())->Send(1,_task,__SIM__unop17591);
    }

    _task->SetLabel(&&__SIM__checkpoint_17596);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17596:

    { Task___SIM__Filter22 *__SIM__task17595 = new Task___SIM__Filter22();
      __SIM__task17595->SetWorker(&_task->GetWorker());
      __SIM__task17595->SetArg0(_this->var___SIM__this6399);
      __SIM__task17595->SetArg1(_this->var___SIM__d6398);
      __SIM__task17595->SetLabel(&&method___SIM__Filter22);
      _task.reset(__SIM__task17595);
    }
    goto method___SIM__Filter22;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^end")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__this6399.get())->Close(true);
    _this->var___SIM__this6399.reset();
    return false;

    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight31: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight31*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17598);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17598:

    // ~s6397[2] >> ~d6398;
    _task->SetLabel(&&__SIM__receive17597);
    if (!((libpi::Session*)_this->var___SIM__s6397.get())->Receive(1,_task,_this->var___SIM__d6398))
      return false;
    __SIM__receive17597:

    _task->SetLabel(&&__SIM__checkpoint_17600);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17600:

    { Task___SIM__Filter22 *__SIM__task17599 = new Task___SIM__Filter22();
      __SIM__task17599->SetWorker(&_task->GetWorker());
      __SIM__task17599->SetArg0(_this->var___SIM__s6397);
      __SIM__task17599->SetArg1(_this->var___SIM__d6398);
      __SIM__task17599->SetLabel(&&method___SIM__Filter22);
      _task.reset(__SIM__task17599);
    }
    goto method___SIM__Filter22;
  } // }}}
  method___SIM__FilterMultipla21: // {{{
  #undef _this
  #define _this ((Task___SIM__FilterMultipla21*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17601);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17601:

    { // link
      _task->tmps.clear();
        _task->SetLabel(&&__SIM__receive___SIM__FilterMultipla2117602);
        if (!((libpi::task::Link*)_this->var___SIM__filtermultipla6396.get())->GetChannels()[0]->Receive(_task,_task->tmp))
          return false;
        __SIM__receive___SIM__FilterMultipla2117602:
        _task->tmps.push_back(_task->tmp);
      vector<vector<shared_ptr<libpi::Channel> > > inChannels(2);
      vector<vector<shared_ptr<libpi::Channel> > > outChannels(2);
      // Optimize vector inserts
        inChannels[0].resize(2);
        outChannels[0].resize(2);
        inChannels[1].resize(2);
        outChannels[1].resize(2);
      // Create channels
      inChannels[0][0].reset(new libpi::task::Channel());
      inChannels[0][1].reset(new libpi::task::Channel());
      inChannels[1][0]=dynamic_pointer_cast<libpi::Channel>(_task->tmps[0]);
      inChannels[1][1].reset(new libpi::task::Channel());
      outChannels[0][0]=inChannels[0][0];
      outChannels[0][1]=inChannels[1][0];
      outChannels[1][0]=inChannels[0][1];
      outChannels[1][1]=inChannels[1][1];
      // Send sessions
      { shared_ptr<libpi::Session> _s(new libpi::Session(1,2, inChannels[1], outChannels[1]));
        ((libpi::Channel*)_task->tmps[0].get())->Send(_task,_s);
      }
      // Create local session
      _this->var___SIM__s6397.reset(new libpi::Session(0,2,inChannels[0],outChannels[0]));
    }

    _task->SetLabel(&&__SIM__checkpoint_17603);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17603:

    { Task___SIM__FlatRight31 *__SIM__task17604(new Task___SIM__FlatRight31());
      __SIM__task17604->SetWorker(&_task->GetWorker());
      __SIM__task17604->SetArg0(_this->var___SIM__s6397);
      __SIM__task17604->SetLabel(&&method___SIM__FlatRight31);
      shared_ptr<libpi::task::Task> __SIM__task17605(__SIM__task17604);
      _task->GetWorker().AddTask(__SIM__task17605);
    }

    _task->SetLabel(&&__SIM__checkpoint_17607);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17607:

    { Task___SIM__FilterMultipla21 *__SIM__task17606 = new Task___SIM__FilterMultipla21();
      __SIM__task17606->SetWorker(&_task->GetWorker());
      __SIM__task17606->SetArg0(_this->var___SIM__filtermultipla6396);
      __SIM__task17606->SetLabel(&&method___SIM__FilterMultipla21);
      _task.reset(__SIM__task17606);
    }
    goto method___SIM__FilterMultipla21;
  } // }}}
  method___SIM__StartService23: // {{{
  #undef _this
  #define _this ((Task___SIM__StartService23*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17610);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17610:

    {
      shared_ptr<libpi::Bool> __SIM__binop17611((*((libpi::Int*)_this->var___SIM__i6403.get())) <= (*((libpi::Int*)intval_0.get())));
      if (((libpi::Bool*)__SIM__binop17611.get())->GetValue())
        goto __SIM____SIM__StartService23_true17608;
      else
        goto __SIM____SIM__StartService23_false17609;
    }
    __SIM____SIM__StartService23_true17608:

    _task->SetLabel(&&__SIM__checkpoint_17619);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17619:

    { Task___SIM__FilterMultipla21 *__SIM__task17618 = new Task___SIM__FilterMultipla21();
      __SIM__task17618->SetWorker(&_task->GetWorker());
      __SIM__task17618->SetArg0(_this->var___SIM__filtermultipla6396);
      __SIM__task17618->SetLabel(&&method___SIM__FilterMultipla21);
      _task.reset(__SIM__task17618);
    }
    goto method___SIM__FilterMultipla21;
    __SIM____SIM__StartService23_false17609:

    _task->SetLabel(&&__SIM__checkpoint_17612);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17612:

    { Task___SIM__StartService23 *__SIM__task17613(new Task___SIM__StartService23());
      __SIM__task17613->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__binop17617((*((libpi::Int*)_this->var___SIM__i6403.get())) - (*((libpi::Int*)intval_1.get())));
      __SIM__task17613->SetArg0(__SIM__binop17617);
      __SIM__task17613->SetArg1(_this->var___SIM__filtermultipla6396);
      __SIM__task17613->SetLabel(&&method___SIM__StartService23);
      shared_ptr<libpi::task::Task> __SIM__task17614(__SIM__task17613);
      _task->GetWorker().AddTask(__SIM__task17614);
    }

    _task->SetLabel(&&__SIM__checkpoint_17616);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17616:

    { Task___SIM__FilterMultipla21 *__SIM__task17615 = new Task___SIM__FilterMultipla21();
      __SIM__task17615->SetWorker(&_task->GetWorker());
      __SIM__task17615->SetArg0(_this->var___SIM__filtermultipla6396);
      __SIM__task17615->SetLabel(&&method___SIM__FilterMultipla21);
      _task.reset(__SIM__task17615);
    }
    goto method___SIM__FilterMultipla21;
  } // }}}
  method___SIM__Eratosthenes24: // {{{
  #undef _this
  #define _this ((Task___SIM__Eratosthenes24*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17621);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17621:

    // ~list6404[1] << ^pop;
    ((libpi::Session*)_this->var___SIM__list6404.get())->Send(0,_task,label_d0ff8d7ab3dcf009ed4db0dcac30d36b);

    _task->SetLabel(&&__SIM__checkpoint_17623);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17623:

    _task->SetLabel(&&__SIM__checkpoint___SIM__Eratosthenes2417622);
    if (!((libpi::Session*)_this->var___SIM__list6404.get())->Receive(0,_task,_task->tmp)) // Receive label to tmp
      return false;
    __SIM__checkpoint___SIM__Eratosthenes2417622:
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^cons")
    {
      _task->tmp.reset();

    _task->SetLabel(&&__SIM__checkpoint_17625);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17625:

    // ~list6404[1] >> ~p6427;
    _task->SetLabel(&&__SIM__receive17624);
    if (!((libpi::Session*)_this->var___SIM__list6404.get())->Receive(0,_task,_this->var___SIM__p6427))
      return false;
    __SIM__receive17624:

    _task->SetLabel(&&__SIM__checkpoint_17627);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17627:

    // ~list6404[1] >> ~tail6428;
    _task->SetLabel(&&__SIM__receive17626);
    if (!((libpi::Session*)_this->var___SIM__list6404.get())->Receive(0,_task,_this->var___SIM__tail6428))
      return false;
    __SIM__receive17626:
    // Session complete
    ((libpi::Session*)_this->var___SIM__list6404.get())->Close(true);
    _this->var___SIM__list6404.reset();

    _task->SetLabel(&&__SIM__checkpoint_17628);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17628:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__console6047.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Eratosthenes2417629);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__c6429))
        return false;
      __SIM__receive___SIM__Eratosthenes2417629:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17631);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17631:

    // ~c6429[1] << ^int;
    ((libpi::Session*)_this->var___SIM__c6429.get())->Send(0,_task,label_64f10e23bd5ddafc2b83ba3a50254359);

    _task->SetLabel(&&__SIM__checkpoint_17633);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17633:

    // ~c6429[1] << ~p6427;
    { 
      ((libpi::Session*)_this->var___SIM__c6429.get())->Send(0,_task,_this->var___SIM__p6427);
    }

    _task->SetLabel(&&__SIM__checkpoint_17635);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17635:

    // ~c6429[1] << ^nl;
    ((libpi::Session*)_this->var___SIM__c6429.get())->Send(0,_task,label_7a11449475519daf7b8b8621e5b6eacc);

    _task->SetLabel(&&__SIM__checkpoint_17637);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17637:

    // ~c6429[1] << ^end;
    ((libpi::Session*)_this->var___SIM__c6429.get())->Send(0,_task,label_186e35ef3dd9939615c442c30b801a33);
    ((libpi::Session*)_this->var___SIM__c6429.get())->Close(true);
    _this->var___SIM__c6429.reset();

    _task->SetLabel(&&__SIM__checkpoint_17638);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17638:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__filtermultipla6396.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__Eratosthenes2417639);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__filter6430))
        return false;
      __SIM__receive___SIM__Eratosthenes2417639:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17641);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17641:

    // ~filter6430[1] << ~p6427;
    { 
      ((libpi::Session*)_this->var___SIM__filter6430.get())->Send(0,_task,_this->var___SIM__p6427);
    }

    _task->SetLabel(&&__SIM__checkpoint_17643);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17643:

    // ~tail6428[1] << ^filter;
    ((libpi::Session*)_this->var___SIM__tail6428.get())->Send(0,_task,label_efcf5b49097e1969bd7fd24fb0c37fe6);

    _task->SetLabel(&&__SIM__checkpoint_17645);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17645:

    // ~tail6428[1] << ~filter6430;
    { 
      ((libpi::Session*)_this->var___SIM__tail6428.get())->Send(0,_task,_this->var___SIM__filter6430);
    }
  _this->var___SIM__filter6430.reset();

    _task->SetLabel(&&__SIM__checkpoint_17647);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17647:

    { Task___SIM__Eratosthenes24 *__SIM__task17646 = new Task___SIM__Eratosthenes24();
      __SIM__task17646->SetWorker(&_task->GetWorker());
      __SIM__task17646->SetArg0(_this->var___SIM__tail6428);
      __SIM__task17646->SetArg1(_this->var___SIM__console6047);
      __SIM__task17646->SetArg2(_this->var___SIM__filtermultipla6396);
      __SIM__task17646->SetLabel(&&method___SIM__Eratosthenes24);
      _task.reset(__SIM__task17646);
    }
    goto method___SIM__Eratosthenes24;
    }
    else 
    if (((libpi::String*)_task->tmp.get())->GetValue()=="^nil")
    {
      _task->tmp.reset();
    ((libpi::Session*)_this->var___SIM__list6404.get())->Close(true);
    _this->var___SIM__list6404.reset();
    return false;

    }
    else throw string("Unknown branch: ")+((libpi::String*)_task->tmp.get())->GetValue();
  } // }}}
  method___SIM__FlatRight32: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight32*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17648);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17648:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__range6366.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight3217649);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__ilist6455))
        return false;
      __SIM__receive___SIM__FlatRight3217649:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17650);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17650:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__stdsucc_Int6389.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight3217651);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__isucc6456))
        return false;
      __SIM__receive___SIM__FlatRight3217651:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17652);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17652:

    { _task->tmp.reset(new libpi::task::Channel);
      ((libpi::task::Link*)(_this->var___SIM__stdorder_Int6060.get()))->GetChannels()[0]->Send(_task,_task->tmp);
      _task->SetLabel(&&__SIM__receive___SIM__FlatRight3217653);
      if (!((libpi::task::Channel*)_task->tmp.get())->Receive(_task,_this->var___SIM__iorder6457))
        return false;
      __SIM__receive___SIM__FlatRight3217653:
      _task->tmp.reset();
    }

    _task->SetLabel(&&__SIM__checkpoint_17655);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17655:

    // ~ilist6455[1] << ~isucc6456;
    { 
      ((libpi::Session*)_this->var___SIM__ilist6455.get())->Send(0,_task,_this->var___SIM__isucc6456);
    }
  _this->var___SIM__isucc6456.reset();

    _task->SetLabel(&&__SIM__checkpoint_17657);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17657:

    // ~ilist6455[1] << ~iorder6457;
    { 
      ((libpi::Session*)_this->var___SIM__ilist6455.get())->Send(0,_task,_this->var___SIM__iorder6457);
    }
  _this->var___SIM__iorder6457.reset();

    _task->SetLabel(&&__SIM__checkpoint_17659);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17659:

    // ~ilist6455[1] << 2;
    { 
      ((libpi::Session*)_this->var___SIM__ilist6455.get())->Send(0,_task,intval_2);
    }

    _task->SetLabel(&&__SIM__checkpoint_17661);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17661:

    // ~ilist6455[1] << 20000;
    { 
      ((libpi::Session*)_this->var___SIM__ilist6455.get())->Send(0,_task,intval_20000);
    }

    _task->SetLabel(&&__SIM__checkpoint_17663);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17663:

    { Task___SIM__Eratosthenes24 *__SIM__task17662 = new Task___SIM__Eratosthenes24();
      __SIM__task17662->SetWorker(&_task->GetWorker());
      __SIM__task17662->SetArg0(_this->var___SIM__ilist6455);
      __SIM__task17662->SetArg1(_this->var___SIM__console6047);
      __SIM__task17662->SetArg2(_this->var___SIM__filtermultipla6396);
      __SIM__task17662->SetLabel(&&method___SIM__Eratosthenes24);
      _task.reset(__SIM__task17662);
    }
    goto method___SIM__Eratosthenes24;
  } // }}}
  method___SIM__FlatRight33: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight33*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17664);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17664:

    _this->var___SIM__filtermultipla6396.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17665);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17665:

    { Task___SIM__FlatRight32 *__SIM__task17666(new Task___SIM__FlatRight32());
      __SIM__task17666->SetWorker(&_task->GetWorker());
      __SIM__task17666->SetArg0(_this->var___SIM__console6047);
      __SIM__task17666->SetArg1(_this->var___SIM__filtermultipla6396);
      __SIM__task17666->SetArg2(_this->var___SIM__range6366);
      __SIM__task17666->SetArg3(_this->var___SIM__stdorder_Int6060);
      __SIM__task17666->SetArg4(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17666->SetLabel(&&method___SIM__FlatRight32);
      shared_ptr<libpi::task::Task> __SIM__task17667(__SIM__task17666);
      _task->GetWorker().AddTask(__SIM__task17667);
    }

    _task->SetLabel(&&__SIM__checkpoint_17670);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17670:

    { Task___SIM__StartService23 *__SIM__task17668 = new Task___SIM__StartService23();
      __SIM__task17668->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17669(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17668->SetArg0(__SIM__systemexp17669);
      __SIM__task17668->SetArg1(_this->var___SIM__filtermultipla6396);
      __SIM__task17668->SetLabel(&&method___SIM__StartService23);
      _task.reset(__SIM__task17668);
    }
    goto method___SIM__StartService23;
  } // }}}
  method___SIM__FlatRight34: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight34*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17671);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17671:

    _this->var___SIM__stdsucc_Int6389.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17672);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17672:

    { Task___SIM__FlatRight33 *__SIM__task17673(new Task___SIM__FlatRight33());
      __SIM__task17673->SetWorker(&_task->GetWorker());
      __SIM__task17673->SetArg0(_this->var___SIM__console6047);
      __SIM__task17673->SetArg1(_this->var___SIM__range6366);
      __SIM__task17673->SetArg2(_this->var___SIM__stdorder_Int6060);
      __SIM__task17673->SetArg3(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17673->SetLabel(&&method___SIM__FlatRight33);
      shared_ptr<libpi::task::Task> __SIM__task17674(__SIM__task17673);
      _task->GetWorker().AddTask(__SIM__task17674);
    }

    _task->SetLabel(&&__SIM__checkpoint_17677);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17677:

    { Task___SIM__StartService20 *__SIM__task17675 = new Task___SIM__StartService20();
      __SIM__task17675->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17676(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17675->SetArg0(__SIM__systemexp17676);
      __SIM__task17675->SetArg1(_this->var___SIM__stdsucc_Int6389);
      __SIM__task17675->SetLabel(&&method___SIM__StartService20);
      _task.reset(__SIM__task17675);
    }
    goto method___SIM__StartService20;
  } // }}}
  method___SIM__FlatRight35: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight35*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17678);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17678:

    _this->var___SIM__range6366.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17679);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17679:

    { Task___SIM__FlatRight34 *__SIM__task17680(new Task___SIM__FlatRight34());
      __SIM__task17680->SetWorker(&_task->GetWorker());
      __SIM__task17680->SetArg0(_this->var___SIM__console6047);
      __SIM__task17680->SetArg1(_this->var___SIM__range6366);
      __SIM__task17680->SetArg2(_this->var___SIM__stdorder_Int6060);
      __SIM__task17680->SetLabel(&&method___SIM__FlatRight34);
      shared_ptr<libpi::task::Task> __SIM__task17681(__SIM__task17680);
      _task->GetWorker().AddTask(__SIM__task17681);
    }

    _task->SetLabel(&&__SIM__checkpoint_17684);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17684:

    { Task___SIM__StartService17 *__SIM__task17682 = new Task___SIM__StartService17();
      __SIM__task17682->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17683(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17682->SetArg0(__SIM__systemexp17683);
      __SIM__task17682->SetArg1(_this->var___SIM__list6147);
      __SIM__task17682->SetArg2(_this->var___SIM__range6366);
      __SIM__task17682->SetLabel(&&method___SIM__StartService17);
      _task.reset(__SIM__task17682);
    }
    goto method___SIM__StartService17;
  } // }}}
  method___SIM__FlatRight36: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight36*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17685);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17685:

    _this->var___SIM__list6147.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17686);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17686:

    { Task___SIM__FlatRight35 *__SIM__task17687(new Task___SIM__FlatRight35());
      __SIM__task17687->SetWorker(&_task->GetWorker());
      __SIM__task17687->SetArg0(_this->var___SIM__console6047);
      __SIM__task17687->SetArg1(_this->var___SIM__list6147);
      __SIM__task17687->SetArg2(_this->var___SIM__stdorder_Int6060);
      __SIM__task17687->SetLabel(&&method___SIM__FlatRight35);
      shared_ptr<libpi::task::Task> __SIM__task17688(__SIM__task17687);
      _task->GetWorker().AddTask(__SIM__task17688);
    }

    _task->SetLabel(&&__SIM__checkpoint_17691);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17691:

    { Task___SIM__StartService15 *__SIM__task17689 = new Task___SIM__StartService15();
      __SIM__task17689->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17690(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17689->SetArg0(__SIM__systemexp17690);
      __SIM__task17689->SetArg1(_this->var___SIM__list6147);
      __SIM__task17689->SetLabel(&&method___SIM__StartService15);
      _task.reset(__SIM__task17689);
    }
    goto method___SIM__StartService15;
  } // }}}
  method___SIM__FlatRight37: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight37*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17692);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17692:

    _this->var___SIM__stdorder_String6118.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17693);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17693:

    { Task___SIM__FlatRight36 *__SIM__task17694(new Task___SIM__FlatRight36());
      __SIM__task17694->SetWorker(&_task->GetWorker());
      __SIM__task17694->SetArg0(_this->var___SIM__console6047);
      __SIM__task17694->SetArg1(_this->var___SIM__stdorder_Int6060);
      __SIM__task17694->SetLabel(&&method___SIM__FlatRight36);
      shared_ptr<libpi::task::Task> __SIM__task17695(__SIM__task17694);
      _task->GetWorker().AddTask(__SIM__task17695);
    }

    _task->SetLabel(&&__SIM__checkpoint_17698);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17698:

    { Task___SIM__StartService11 *__SIM__task17696 = new Task___SIM__StartService11();
      __SIM__task17696->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17697(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17696->SetArg0(__SIM__systemexp17697);
      __SIM__task17696->SetArg1(_this->var___SIM__stdorder_String6118);
      __SIM__task17696->SetLabel(&&method___SIM__StartService11);
      _task.reset(__SIM__task17696);
    }
    goto method___SIM__StartService11;
  } // }}}
  method___SIM__FlatRight38: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight38*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17699);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17699:

    _this->var___SIM__stdorder_Float6089.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17700);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17700:

    { Task___SIM__FlatRight37 *__SIM__task17701(new Task___SIM__FlatRight37());
      __SIM__task17701->SetWorker(&_task->GetWorker());
      __SIM__task17701->SetArg0(_this->var___SIM__console6047);
      __SIM__task17701->SetArg1(_this->var___SIM__stdorder_Int6060);
      __SIM__task17701->SetLabel(&&method___SIM__FlatRight37);
      shared_ptr<libpi::task::Task> __SIM__task17702(__SIM__task17701);
      _task->GetWorker().AddTask(__SIM__task17702);
    }

    _task->SetLabel(&&__SIM__checkpoint_17705);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17705:

    { Task___SIM__StartService8 *__SIM__task17703 = new Task___SIM__StartService8();
      __SIM__task17703->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17704(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17703->SetArg0(__SIM__systemexp17704);
      __SIM__task17703->SetArg1(_this->var___SIM__stdorder_Float6089);
      __SIM__task17703->SetLabel(&&method___SIM__StartService8);
      _task.reset(__SIM__task17703);
    }
    goto method___SIM__StartService8;
  } // }}}
  method___SIM__FlatRight39: // {{{
  #undef _this
  #define _this ((Task___SIM__FlatRight39*)_task.get())
  {

    _task->SetLabel(&&__SIM__checkpoint_17706);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17706:

    _this->var___SIM__stdorder_Int6060.reset(new libpi::task::Link(2));

    _task->SetLabel(&&__SIM__checkpoint_17707);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17707:

    { Task___SIM__FlatRight38 *__SIM__task17708(new Task___SIM__FlatRight38());
      __SIM__task17708->SetWorker(&_task->GetWorker());
      __SIM__task17708->SetArg0(_this->var___SIM__console6047);
      __SIM__task17708->SetArg1(_this->var___SIM__stdorder_Int6060);
      __SIM__task17708->SetLabel(&&method___SIM__FlatRight38);
      shared_ptr<libpi::task::Task> __SIM__task17709(__SIM__task17708);
      _task->GetWorker().AddTask(__SIM__task17709);
    }

    _task->SetLabel(&&__SIM__checkpoint_17712);
    if (++_steps>=libpi::task::Task::MaxSteps) return true;
    __SIM__checkpoint_17712:

    { Task___SIM__StartService5 *__SIM__task17710 = new Task___SIM__StartService5();
      __SIM__task17710->SetWorker(&_task->GetWorker());
      shared_ptr<libpi::Int> __SIM__systemexp17711(new libpi::Int(libpi::task::Worker::TargetTasks));
      __SIM__task17710->SetArg0(__SIM__systemexp17711);
      __SIM__task17710->SetArg1(_this->var___SIM__stdorder_Int6060);
      __SIM__task17710->SetLabel(&&method___SIM__StartService5);
      _task.reset(__SIM__task17710);
    }
    goto method___SIM__StartService5;
  } // }}}
} // }}}
void libpi::task::Worker_Pool::Work() // {{{
{ try
  { while (true) // Continue until termination
    { // Find next task
      while (myActiveTasks.empty())
      { ourIdleWorkersLock.Lock();
        if (ourIdleWorkersSize==libpi::task::Worker::Workers-1) // Test if program is complete
        { shared_ptr<Task> nullTask;
          while (!ourIdleWorkers.empty())
          { ourIdleWorkers.front()->EmployTask(nullTask);
            ourIdleWorkers.pop();
          }
          EmployTask(nullTask);
          ourIdleWorkersSize=0;
          ourIdleWorkersLock.Release();
          break;
        }
        else
        {
          ourIdleWorkers.push(this);
          ++ourIdleWorkersSize;
          ourIdleWorkersLock.Release();
          myWaitLock.Lock();
        }
      }
      shared_ptr<Task> task=myActiveTasks.front();
      myActiveTasks.pop();

      resume_task:
      if (!task)
        break;
      if (_methods(task))
      { if (myActiveTasks.empty())
          goto resume_task;
        else QueueTask(task);
      }
      //else
      //  --ActiveTasks;
    }
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
  }
  return;
} // }}}
void *_start_worker(void *arg) // {{{
{ libpi::task::Worker *worker=(libpi::task::Worker*)arg;
  worker->Work();
  delete worker;
} // }}}
inline void _start_workers() // {{{
{ pthread_t x[libpi::task::Worker::Workers];
  pthread_attr_t y;
  pthread_attr_init(&y);
  pthread_attr_setstacksize(&y,16384);
  //pthread_attr_setdetachstate(&y,PTHREAD_CREATE_DETACHED);
  for (size_t wc=0; wc<libpi::task::Worker::Workers; ++wc)
  { libpi::task::Worker *_worker=new libpi::task::Worker_Pool();
    if (wc==0)
    { // Create main task
      shared_ptr<libpi::task::Task> _main(new TaskMain());
      _main->SetLabel(NULL);
      _main->SetWorker(_worker);
      _worker->AddTask(_main);
    }
    // Bind thread to specific core
    //cpu_set_t cpuset;
    //CPU_ZERO(&cpuset);
    //CPU_SET(wc%cores, &cpuset);
    //pthread_setaffinity_np(x[wc],sizeof(cpu_set_t), &cpuset);
    pthread_create(&(x[wc]),&y,_start_worker,(void*)_worker);
  }
  for (size_t wc=0; wc<libpi::task::Worker::TargetTasks; ++wc)
    pthread_join(x[wc],NULL);
} // }}}
/*Start process, and its continuations */
int main(int argc, char **argv) // {{{
{ // PARSE ARGS!!
  for (int i=0; i<argc; ++i)
  { if (string(argv[i])=="-pi_tprocs" && i+1<argc)
      libpi::task::Worker::TargetTasks=atoi(argv[++i]);
    if (string(argv[i])=="-pi_workers" && i+1<argc)
      libpi::task::Worker::Workers=atoi(argv[++i]);
    else
      _args.push_back(argv[i]);
  }
  try
  { signal(SIGCHLD, SIG_IGN); // Fork optimization
    // Start workers
    _start_workers();
    //munmap(_aprocs,sizeof(int));
  } catch (const string &error)
  { cerr << "Error: " << error << endl;
    return 1;
  }
  return 0;
} // }}}
