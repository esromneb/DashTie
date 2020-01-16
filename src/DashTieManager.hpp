#pragma once

#include <typeinfo>
#include <future>
#include <functional>
#include <atomic>
#include <chrono>

#include "DashTie.hpp"
#include "3rd/json/single_include/nlohmann/json.hpp"

#define GET_DASHDIE_INTO_MOCK() true

class NetDriver;

class DashTieManager
{
public:
    // DashTieManager();

    NetDriver* netDriver = 0;


    ///
    /// Dashboard stuff
    /// void tieDashboardInt(int32_t *p, uint32_t uuid, std::vector<std::string> & path);

    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, std::vector<std::string> & path) {
        DashTie *tie = new DashTie();

        nlohmann::json j_test = *p;

        bool found = true;

        // edit this table to add a new type
        // also edit buildString()
        // see https://stackoverflow.com/questions/36577718/same-typeid-name-but-not-stdis-same
        static_assert(
               std::is_same<T,size_t>::value
            || std::is_same<T,int32_t>::value
            || std::is_same<T,double>::value
            || std::is_same<T,bool>::value
            || std::is_same<T,std::vector<double>>::value
            || std::is_same<T,uint32_t>::value
            , "Illegal type in tieDashboard()");

        if( typeid(T) == typeid(size_t) ) {
            tie->type = dash_type_t::dash_type_size_t;
#ifdef DEBUG_PRINT_TIE_TICKLE
            std::cout << "found type_t" << std::endl;
#endif
        } else if ( typeid(T) == typeid(int32_t) ) {
            tie->type = dash_type_t::dash_type_int32_t;
#ifdef DEBUG_PRINT_TIE_TICKLE            
            std::cout << "found int32_t" << std::endl;
#endif
        } else if ( typeid(T) == typeid(double) ) {
            tie->type = dash_type_t::dash_type_double;
#ifdef DEBUG_PRINT_TIE_TICKLE            
            std::cout << "found double" << std::endl;
#endif
        } else if ( typeid(T) == typeid(bool) ) {
            tie->type = dash_type_t::dash_type_bool;
#ifdef DEBUG_PRINT_TIE_TICKLE            
            std::cout << "found bool" << std::endl;
#endif
        } else if ( typeid(T) == typeid(std::vector<double>) ) {
            tie->type = dash_type_t::dash_type_vec_double;
#ifdef DEBUG_PRINT_TIE_TICKLE            
            std::cout << "found vector<double>" << std::endl;
#endif   
        } else if ( typeid(T) == typeid(uint32_t) ) {
            tie->type = dash_type_t::dash_type_uint32_t;
#ifdef DEBUG_PRINT_TIE_TICKLE            
            std::cout << "found uint32_t" << std::endl;
#endif
        } else {
            std::cout << " tieDashboard doesn't know about that type" << std::endl;
            
            found = false;
        }



        tie->p = (void*)p;
        tie->uuid = uuid;
        tie->path = path;

#ifdef DEBUG_PRINT_TIE_TICKLE
        std::cout << "tied type enum: " << (int)tie->type << std::endl;
        std::cout << "  tied ptr " << (void*)p << std::endl;
        std::cout << "  tied path[0] " << tie->path[0] << std::endl;
        if( tie->path.size() > 1 ) {
            std::cout << "  tied path[1] " << tie->path[1] << std::endl;
        }
        if( tie->path.size() > 2 ) {
            std::cout << "  tied path[2] " << tie->path[2] << std::endl;
        }
        std::cout << "  tied uuid " << tie->uuid << std::endl;
#endif

        // assert later here because we want to know debug
        assert(found);
        (void)found;

        std::unique_lock<std::mutex> lock(_ties_mutex);
        _ties.push_back(tie);
    }

    template <typename T>
    void ticklePath(T p, const uint32_t uuid, std::string a) {
        // FIXME static here is weird??
        static DashTie tie = DashTie();
        static T saved2 = p;
        saved2 = p; // copy to here
        std::vector<std::string> path = {a};
        
        tie.p = (void*)&saved2;
        tie.uuid = uuid;
        tie.path = path;

        ticklePathInternal(&tie);
    }

    template <typename T>
    void ticklePath(T p, const uint32_t uuid,
        std::string a,
        std::string b
        ) {
        static DashTie tie = DashTie();
        static T saved2 = p;
        saved2 = p; // copy to here
        std::vector<std::string> path = {a,b};
        // std::cout << "fn: " << saved2 << std::endl;
        tie.p = (void*)&saved2;
        tie.uuid = uuid;
        tie.path = path;

        ticklePathInternal(&tie);
    }

    template <typename T>
    void ticklePath(T p, const uint32_t uuid,
        std::string a,
        std::string b,
        std::string c
        ) {
        static DashTie tie = DashTie();
        static T saved2 = p;
        saved2 = p; // copy to here
        std::vector<std::string> path = {a,b,c};
        
        tie.p = (void*)&saved2;
        tie.uuid = uuid;
        tie.path = path;

        ticklePathInternal(&tie);
    }


    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, std::string a) {
        std::vector<std::string> path = {a};
        tieDashboard<T>(p, uuid, path);
    }

    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, 
        std::string a,
        std::string b
        ) {
        std::vector<std::string> path = {a,b};
        tieDashboard<T>(p, uuid, path);
    }

    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, 
        std::string a,
        std::string b,
        std::string c
        ) {
        std::vector<std::string> path = {a,b,c};
        tieDashboard<T>(p, uuid, path);
    }

    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, 
        std::string a,
        std::string b,
        std::string c,
        std::string d
        ) {
        std::vector<std::string> path = {a,b,c,d};
        tieDashboard<T>(p, uuid, path);
    }

    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, 
        std::string a,
        std::string b,
        std::string c,
        std::string d,
        std::string e
        ) {
        std::vector<std::string> path = {a,b,c,d,e};
        tieDashboard<T>(p, uuid, path);
    }

    template <typename T>
    void tieDashboard(T *p, const uint32_t uuid, 
        std::string a,
        std::string b,
        std::string c,
        std::string d,
        std::string e,
        std::string f
        ) {
        std::vector<std::string> path = {a,b,c,d,e,f};
        tieDashboard<T>(p, uuid, path);
    }

    // void tieDashboard(size_t *p, uint32_t uuid, 
    //     std::string a,
    //     std::string b
    //     ) {
    //     std::vector<std::string> path = {a,b};
    //     tieDashboard(p, uuid, path);
    // }

    void tickle(const void *p);
    void tickle(const int32_t *p);
    void tickle(const uint32_t *p);
    void tickle(const double *p);
    void tickle(const size_t *p);
    void tickleAll(const size_t start = 0, const size_t stop = 0);
    std::vector<DashTie*> _ties;
    void tickleLog(const std::string tag, const std::string message);
    void dashTieSetup();
    void ticklePathInternal(DashTie* saved);
    uint64_t runtime_session_id; // changes each time we restart the app, set in dashTieSetup based on clock and pid
    mutable std::mutex _ties_mutex; // protects _ties
    

};


class NetDriver
{
public:
    void updateDashboard(const std::string& msg);
};
