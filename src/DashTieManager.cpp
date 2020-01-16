#include "DashTieManager.hpp"
#include "DashTie.hpp"
#include <unistd.h>

using namespace std;





void DashTieManager::tickle(const int32_t *p) {
    tickle((const void*)p);
}

void DashTieManager::tickle(const uint32_t *p) {
    tickle((const void*)p);
}

void DashTieManager::tickle(const double *p) {
    tickle((const void*)p);
}

void DashTieManager::tickle(const size_t *p) {
    tickle((const void*)p);
}



// internal
static void __tickleExecute(DashTie* saved, NetDriver* nd) {

    bool it_was_the_first = false;

    // if this is first time, always load
    if(saved->first_run) {
        saved->first_run = false;
        saved->load();
#ifdef DEBUG_PRINT_DASH_UPDATES
        cout << saved->getPathStr() << " first run"  << endl;
#endif

        it_was_the_first = true; // local bool
    }


    if(saved->hasChanged() || it_was_the_first) {
#ifdef DEBUG_PRINT_DASH_UPDATES
        cout << saved->getPathStr() << " has changed" << endl;
#endif
        auto update_as_string = saved->buildString();
        nd->updateDashboard(update_as_string);

        saved->load();
    } else {
#ifdef DEBUG_PRINT_DASH_UPDATES
        cout << saved->getPathStr() << " skipped"  << endl;
#endif
    }

}

static void __tickleExecuteSingle(DashTie* saved, NetDriver* nd) {

    constexpr bool it_was_the_first = true;

    // if this is first time, always load
    // if(saved->first_run) {
    //     saved->first_run = false;
    //     // saved->load();
    // }


    if(it_was_the_first) {
#ifdef DEBUG_PRINT_DASH_UPDATES
        cout << saved->getPathStr() << " has changed" << endl;
#endif
        auto update_as_string = saved->buildString();
        nd->updateDashboard(update_as_string);

        // saved->load();
    }
}


// already have string built?
static void __dumbTickleExecute(std::string s, NetDriver* nd) {
    nd->updateDashboard(s);
}

// with one _ is launcher for bg
static void _dumbTickleExecute(std::string& s, NetDriver* nd) {
    auto fut = std::async(std::launch::async, [](std::string s, NetDriver* _nd){ // no ref
                            __dumbTickleExecute(s, _nd);
                        }, s, nd);

}

///
/// Launches a future, so that we can do the compute in the background (life save)
///
static void _tickleExecute(DashTie* saved, NetDriver* nd) {
    auto fut = std::async(std::launch::async, [](DashTie* saved, NetDriver* _nd){
                            __tickleExecute(saved, _nd);
                        }, saved, nd);

}

static void _tickleExecuteSingle(DashTie* saved, NetDriver* nd) {
    auto fut = std::async(std::launch::async, [](DashTie* saved, NetDriver* _nd){
                            __tickleExecuteSingle(saved, _nd);
                        }, saved, nd);
}


void DashTieManager::tickle(const void *p) {
    bool found = false;
    size_t i = 0;
    DashTie* saved;
    {
        std::unique_lock<std::mutex> lock(_ties_mutex);
        for( auto tie : _ties ) {
            if(tie->p == p) {
                saved = tie;
                found = true;
                break;
            }
            i++;
        }
    }

    if(found) {
        // cout << "found " << i << endl;
    } else {
        cout << "tickle() ";
        cout << "not found ptr " << p << endl;
        return;
    }

    // auto fut = std::async(std::launch::async, [](RadioEstimate* r, std::vector<uint32_t> chunk){
    //                         r->dspRunResiduePhase(chunk);
    //                     }, radios[0], cfo_buf_chunk);

    _tickleExecute(saved, netDriver);
}

void DashTieManager::ticklePathInternal(DashTie* saved) {
    _tickleExecuteSingle(saved, netDriver);
}

void DashTieManager::tickleAll(size_t start , size_t stop ) {

    std::vector<DashTie*> _ties_copy;

    {
        std::unique_lock<std::mutex> lock(_ties_mutex);
        _ties_copy = _ties;
    }

    cout << "About to tickle() " << _ties_copy.size() << " ties" << endl;
    size_t i = 0;
    for( auto tie : _ties_copy ) {
        _tickleExecute(tie, netDriver);
        i++;
    }
}


// bad name, alwasy sends a log for the DashTie system.  needs a beter name or whatever
void DashTieManager::tickleLog(const std::string tag, const std::string message) {

    // create an empty structure (null)
    nlohmann::json j;

    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["t"] = tag;

    // add a Boolean that is stored as bool
    j["m"] = message;

    // session (this is LONG we could shorten it somehow?)
    j["s"] = runtime_session_id;

    // cout << j << endl;

    // TIE_LOG_RANGE

    // buildStringDumb

    // (uint32_t uuid, std::vector<std::string> path, T& j) {

    std::vector<std::string> path;
    path.push_back("l");

        std::string res;

    // res = dashTieBuildStringDumb<nlohmann::json>(TIE_LOG_RANGE, path, j);

    res = dashTieBuildStringUpdateDumb<nlohmann::json>(TIE_LOG_RANGE, j);

    // cout << "final res" << res << endl;

    _dumbTickleExecute(res, netDriver);



}

void DashTieManager::dashTieSetup() {
    /// time_sess_id
    /// based on clock and pid


      const uint64_t seed_start = std::time(0);
      uint64_t val =  seed_start << 16;
      const uint32_t pp = getppid() & 0xffff;
      val = val | pp;


      // 48 bit mask
      runtime_session_id = val % 0xffffffffffff;
// python
// >>> hex(2**48 - 1)
//  '0xffffffffffff'

      // cout << "seed_start: " << seed_start << " val " << val << " pp " << pp << endl;

      // unsigned int fixed_seed = 0; // set to non zero to use

      // fixed_seed = 1525241634; // after 1900

      // if(fixed_seed != 0) {
      //   seed_start = fixed_seed;
      //   cout << "starting with hard-coded seed " << seed_start << endl;
      // } else {
      //   cout << "starting with random seed " << seed_start << endl;
      // }


      // srand(seed_start);
}


void NetDriver::updateDashboard(const std::string& msg) {
    // cout << "write " << msg << "\n";

    // std::unique_lock<std::mutex> lock(_dash_sock_mutex);
    // client->write(msg);
}
