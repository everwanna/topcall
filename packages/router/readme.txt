80M = 80*1024*1024 = 80*1024k

single package: 1k

the tps is: 80*1024k/1k = 80k packets cross idc. It should be good enough for now.
If we need more powerful cross-idc push system, try put push_d into proxy mode, put evetyhing in db and
let the other side fetch from in-momory db(mongo).