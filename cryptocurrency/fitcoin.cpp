#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <algorithm>
#include <pthread.h>
#include <semaphore.h>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <atomic>
using namespace std;


class CFITCoin;
class CCVUTCoin;
class CCustomer;

typedef struct shared_ptr<CFITCoin>  AFITCoin;
typedef struct shared_ptr<CCVUTCoin> ACVUTCoin;
typedef struct shared_ptr<CCustomer> ACustomer;
//=================================================================================================
class CFITCoin {
public:
  CFITCoin (const vector<uint32_t> & vectors, int distMax): m_Vectors(vectors), m_DistMax(distMax), m_Count(0) {}
  virtual ~CFITCoin (void) = default;
  vector<uint32_t> m_Vectors;
  int              m_DistMax;
  uint64_t         m_Count;
};
//=================================================================================================
class CCVUTCoin {
public:
  CCVUTCoin (const vector<uint8_t> & data, int distMin, int distMax): m_Data(data), m_DistMin(distMin), m_DistMax(distMax), m_Count(0) {}
  virtual ~CCVUTCoin (void) = default;
  vector<uint8_t> m_Data;
  int             m_DistMin;
  int             m_DistMax;
  uint64_t        m_Count;
};
//=================================================================================================
class CCustomer {
public:
  virtual ~CCustomer(void) = default;
  virtual AFITCoin FITCoinGen(void) = 0;
  virtual ACVUTCoin CVUTCoinGen(void) = 0;
  virtual void FITCoinAccept(AFITCoin x) = 0;
  virtual void CVUTCoinAccept(ACVUTCoin x) = 0;
};
//=================================================================================================
#endif /* __PROGTEST__ */

struct STask {
  uint      id, cnt;
  bool      init, fin, pop;
  AFITCoin  fc;
  ACVUTCoin cc;

  mutex        mtx;
  uint64_t     test, limit, iter, step, xtra;
  vector<uint> mask;

  STask(uint idx, AFITCoin x)  : id(idx), cnt(0), init(false), fin(false), pop(false), fc(x), cc(nullptr), test(0), limit(0), iter(ULONG_MAX), step(0), xtra(0) {}
  STask(uint idx, ACVUTCoin x) : id(idx), cnt(0), init(false), fin(false), pop(false), fc(nullptr), cc(x), test(1), limit(0), iter(ULONG_MAX), step(0), xtra(0) {}
};

struct SSeat {
  ACustomer     cust;
  uint          id;
  uint          task_cnt;
  bool          fit_fin, cvut_fin;
  queue<STask*> buff;

  mutex              mtx;
  condition_variable cv;

  SSeat(ACustomer c) : cust(c), id(-1), task_cnt(0), fit_fin(false), cvut_fin(false) {}
};

class CRig {
private:
  /* STATIC PRIVATE */
  static map<uint, vector<uint64_t>> pascal;
  static const uint                  seat_max;
  static const uint                  task_max;

  static void setMask(AFITCoin x, vector<uint>& mask) {
    uint ucb = 0;
    for(uint i = 0; i+1 < x->m_Vectors.size(); ++i)
      ucb |= x->m_Vectors[i] ^ x->m_Vectors[i+1];
    for(uint i = 0; i < 32; ++i)
      if((ucb >> i) & 1u) mask.push_back(i);
  }

  static void setPascal(const vector<uint>& mask) {
    uint cb = 32-mask.size();
    if(pascal.find(cb) != pascal.end()) return;

    vector<uint64_t> vec;
    vec.push_back(1);
    for(uint i = 1; i <= cb; ++i) {
      if(i <= cb/2) vec.push_back(vec[i-1] * (cb+1-i) / i);
      else vec.push_back(vec[cb-i]);
    }
    pascal.emplace(cb, vec);
  }

  static uint getDistance(const uint32_t& vec, const uint64_t& test, const vector<uint>& mask) {
    uint dist = 0;
    for(uint i = 0; i < mask.size(); ++i) {
      if(((vec >> mask[i]) ^ (test >> i)) & 1u) dist++;
      if(dist == mask.size()) return dist;
    }
    return dist;
  }

  static uint64_t countFit(AFITCoin x, const uint64_t& test, const vector<uint>& mask) {
    uint     cb   = 32-mask.size();
    uint     dist = 0;
    uint64_t cnt  = 0;
    for(uint32_t vec : x->m_Vectors) {
      dist = max(dist, getDistance(vec, test, mask));
      if(dist == mask.size()) break;
    }
    for(uint i = 0; i < pascal[cb].size() && dist <= (uint)x->m_DistMax; ++i, ++dist)
      cnt += pascal[cb][i];
    return cnt;
  }

  static string getSuffix(ACVUTCoin x, const uint64_t& test) {
    string s;
    for(uint64_t i = 0; i < test; ++i)
      s.insert(0, (((x->m_Data[x->m_Data.size()-(i/8)-1] >> (7-(i%8)))) & 1u) ? "1" : "0");
    return s;
  }

  static char getPrefix(ACVUTCoin x, uint64_t idx) {
    return (((x->m_Data[(idx/8)] >> (idx%8))) & 1u) ? '1' : '0';
  }

  static uint checkFix(ACVUTCoin x, const char pref, const string& suff, const vector<uint64_t>& prev, vector<uint64_t>& curr) {
    bool flag = curr.front() <= (uint)x->m_DistMax;
    for(uint32_t i = 1; i-1 < suff.size(); ++i) {
      if(pref == suff[i-1]) curr.push_back(prev[i-1]);
      else curr.push_back(min(min(curr[i-1], prev[i-1]), prev[i]) + 1);
      if(!flag) flag = curr.back() <= (uint)x->m_DistMax;
    }
    return (curr.back() >= (uint)x->m_DistMin && curr.back() <= (uint)x->m_DistMax) ? flag ? 0 : 1 : flag ? 2 : 3;
  }

  static uint64_t countCvut(ACVUTCoin x, const uint64_t& test) {
    uint     val  = 0;
    uint64_t cnt  = 0;
    string   suff = getSuffix(x, test);
    vector<uint64_t> prev, curr;
    for(uint64_t i = 0; i <= suff.size(); ++i)
      prev.push_back(i);

    for(uint64_t i = 1; i <= x->m_Data.size()*8; ++i) {
      char pref = getPrefix(x, i-1);
      curr.push_back(i);
      val = checkFix(x, pref, suff, prev, curr);
      if(val < 2) cnt++;
      if(val % 2) break;
      prev.swap(curr);
      curr.clear();
    }
    return cnt;
  }

  /* MEMBER PRIVATE */
  uint               cust_cnt, wrkr_cnt;
  bool               cust_stop;
  vector<SSeat*>     seat_vec;
  queue<STask*>      task_que;
  queue<STask*>      work_que;

  mutex              mtx_cnt, mtx_seat, mtx_que, mtx_work;
  condition_variable cv_seat, cv_que, cv_work, cv_stop;

  uint findSeat() {
    for(uint i = 0; i < seat_max; ++i)
      if(!seat_vec[i]) return i;
    return seat_max;
  }

  void beHelper(SSeat* seat, bool fit) {
    STask* task;
    // ON DUTY
    while(true) {
      // GET QUERY
      fit ?
      task = new STask(seat->id, seat->cust->FITCoinGen()) :
      task = new STask(seat->id, seat->cust->CVUTCoinGen());

      // QUIT IF TASK IS EMPTY
      if(!task->fc && !task->cc) break;
      seat->mtx.lock();
      seat->task_cnt++;
      seat->mtx.unlock();

      // PUSH TASK TO QUE
      unique_lock<mutex> ul_que(mtx_que);
      while(task_que.size() > task_max)
        cv_que.wait(ul_que);
      task_que.push(task);
      //printf("HELPER: %lu\n", task_que.size());
      ul_que.unlock();
      cv_work.notify_all();
    }

    // SIGNAL FINISH
    unique_lock<mutex> ul_cust(seat->mtx);
    fit ? seat->fit_fin = true : seat->cvut_fin = true;
    seat->cv.notify_one();
    delete task;
  }

  void beSubmitter(SSeat* seat) {
    // CREATE HELPER THREADS
    thread(&CRig::beHelper, this, seat, true).detach();
    thread(&CRig::beHelper, this, seat, false).detach();

    // ON DUTY
    while(true) {
      unique_lock<mutex> ul_cust(seat->mtx);
      while(seat->buff.empty()) {
        // CHECK IF CUSTOMER IS DONE
        if(seat->fit_fin && seat->cvut_fin && !seat->task_cnt) {
          // EMPTY SEAT
          ul_cust.unlock();
          mtx_seat.lock();
          seat_vec[seat->id] = nullptr;
          mtx_seat.unlock();
          cv_seat.notify_one();

          // LEAVE
          unique_lock<mutex> ul_cnt(mtx_cnt);
          cust_cnt--;
          if(!cust_cnt) cv_stop.notify_one();
          delete seat;
          return;
        }
        seat->cv.wait(ul_cust);
      }

      // RETURN TASKS TO CUSTOMER
      seat->task_cnt--;
      STask* task = seat->buff.front();
      seat->buff.pop();
      ul_cust.unlock();
      task->mtx.lock();
      task->fc ?
      seat->cust->FITCoinAccept(task->fc) :
      seat->cust->CVUTCoinAccept(task->cc);
      task->mtx.unlock();
      //printf("Task Submitted\n");
      delete task;
    }
  }

  void initTask(STask* task) {
    uint wrk_load = 5000;
    uint task_size;

    if(task->fc) {
      setMask(task->fc, task->mask);
      setPascal(task->mask);
      task->limit = (1u << task->mask.size());
      task->fc->m_Count = 0;

      task_size = task->limit * task->fc->m_Vectors.size();
    }
    else {
      task->limit = task->cc->m_Data.size()*8;
      task->cc->m_Count = 0;

      task_size = task->limit * task->limit;
    }

    task->step = max(wrk_load * task->limit / task_size, (uint64_t)1);
    task->iter = task->limit / task->step;
    if(!task->iter) task->iter++;
    task->xtra = task->limit % task->step;

    //printf("t_size %u\tt_iter %lu\t-->\t", task_size, task->iter);
    //printf("t_step %lu\tt_xtra %lu \n", task->step, task->xtra);

    task->init  = true;
  }

  void solveTask(STask* task) {
    uint64_t beg, end;

    if(task->fc) {
      while(true) {
        if(task->fin) break;
        beg = task->test;
        task->test = end = min(beg + task->step + (task->xtra ? 1 : 0), task->limit);
        //printf("FIT\t%lu : %lu : %lu\n", task->test, task->limit, task->step);
        if(task->test == task->limit) task->fin = true;
        if(task->xtra) --task->xtra;
        if(task->iter) --task->iter;
        task->mtx.unlock();

        uint64_t cnt = 0;
        for(;beg < end; ++beg)
          cnt += countFit(task->fc, beg, task->mask);

        task->mtx.lock();
        task->fc->m_Count += cnt;
      }
    }

    else {
      while(true) {
        if(task->fin) break;
        beg = task->test;
        task->test = end = min(beg + task->step + (task->xtra ? 1 : 0), task->limit);
        //printf("CVUT\t%lu : %lu : %lu\n", task->test, task->limit, task->step);
        if(task->test == task->limit) {
          task->fin = true;
          ++end;
        }
        if(task->xtra) --task->xtra;
        if(task->iter) --task->iter;
        task->mtx.unlock();

        uint64_t cnt = 0;
        for(;beg < end; ++beg)
          cnt += countCvut(task->cc, beg);

        task->mtx.lock();
        task->cc->m_Count += cnt;
      }
    }
  }

  void beWorker() {
    STask* task = nullptr;
    // ON DUTY
    while(true) {
      // WAIT FOR TASK
      unique_lock<mutex> ul_que(mtx_que);
      //printf("WORKER: %lu\n", task_que.size());
      while(task_que.empty()) {
        // CHECK "STOP" CALL
        unique_lock<mutex> ul_cnt(mtx_cnt);
        if(cust_stop) {
          wrkr_cnt--;
          if(!wrkr_cnt) cv_stop.notify_one();
          cv_work.notify_all();
          return;
        }
        ul_cnt.unlock();
        cv_work.wait(ul_que);
      }

      // ENTER TASK
      task = task_que.front();
      //printf("ENTER TASK\t%u : %d\n", task->cnt, task->pop);
      if(task->mtx.try_lock()) {
        task->cnt++;

        if(task->fin || task->cnt > task->iter) {
          task->pop = true;
          task_que.pop();
          ul_que.unlock();
          cv_que.notify_one();
        }
        else if(!task->init) {
          task_que.pop();
          ul_que.unlock();

          initTask(task);

          ul_que.lock();
          task_que.push(task);
          ul_que.unlock();
          cv_work.notify_all();

          solveTask(task);
        }
        else {
          ul_que.unlock();
          solveTask(task);
        }

        // LEAVE TASK
        task->cnt--;
        //printf("LEAVE TASK\t%u : %d\n", task->cnt, task->pop);

        if(task->pop && task->fin && !task->cnt) {
          unique_lock<mutex> ul_cust(seat_vec[task->id]->mtx);
          seat_vec[task->id]->buff.push(task);
          ul_cust.unlock();
          seat_vec[task->id]->cv.notify_one();
        }
        task->mtx.unlock();
      }
      else ul_que.unlock();
    }
  }

public:
  static void Solve(AFITCoin x) {
    uint64_t     test;
    vector<uint> mask;

    x->m_Count = 0;
    setMask(x, mask);
    setPascal(mask);

    for(test = 0; test < (1u << mask.size()); ++test)
      x->m_Count += countFit(x, test, mask);
  }

  static void Solve(ACVUTCoin x) {
    uint64_t test;

    x->m_Count = 0;

    for(test = 1; test <= x->m_Data.size()*8; ++test)
      x->m_Count += countCvut(x, test);
  }

  CRig(void) : cust_cnt(0), wrkr_cnt(0), cust_stop(false) {
    for(uint i = 0; i < seat_max; ++i)
      seat_vec.push_back(nullptr);
  }

  ~CRig(void) {}

  void Start(int thrCnt) {
    //cout << "START " << thrCnt << endl;
    wrkr_cnt = thrCnt;
    for(int i = 0; i < thrCnt; ++i)
      thread(&CRig::beWorker, this).detach();
  }

  void Stop(void) {
    //cout << "STOP\t" << wrkr_cnt << endl;
    unique_lock<mutex> ul_cnt(mtx_cnt);
    // WAIT CUSTOMERS TO LEAVE
    while(cust_cnt)
      cv_stop.wait(ul_cnt);
    cust_stop = true;
    ul_cnt.unlock();
    unique_lock<mutex> ul_que(mtx_que);
    cv_work.notify_all();
    // WAIT WORKERS TO LEAVE
    while(wrkr_cnt)
      cv_stop.wait(ul_que);
    cust_stop = false;
  }

  void AddCustomer(ACustomer c) {
    //cout << "ADD" << endl;
    mtx_cnt.lock();
    cust_cnt++;
    mtx_cnt.unlock();

    // FIND SEAT
    SSeat* seat = new SSeat(c);
    unique_lock<mutex> ul_seat(mtx_seat);
    seat->id = findSeat();
    while(seat->id == seat_max) {
      cv_seat.wait(ul_seat);
      seat->id = findSeat();
    }
    seat_vec[seat->id] = seat;
    ul_seat.unlock();
    thread(&CRig::beSubmitter, this, seat).detach();
  }
};

map<uint, vector<uint64_t>> CRig::pascal;
const uint   CRig::seat_max = 5;
const uint   CRig::task_max = 5;

#ifndef __PROGTEST__
#include "test.inc"
#endif /* __PROGTEST__ */
