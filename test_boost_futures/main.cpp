//  (C) Copyright 2008 Anthony Williams 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"
#include "n2561_future.hpp"
#include <assert.h>
#include <utility>
#include <memory>
#include <string>

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename T>
    typename boost::remove_reference<T>::type&& move(T&& t)
    {
        return t;
    }
#else
    template<typename T>
    boost::detail::thread_move_t<T> move(T& t)
    {
        return boost::move(t);
    }
#endif

struct X
{
private:
    
    X(X& other);
    
public:
    
    int i;
    
    X():
        i(42)
    {}
#ifdef BOOST_HAS_RVALUE_REFS
    X(X&& other):
        i(other.i)
    {
        other.i=0;
    }
#else
    X(boost::detail::thread_move_t<X> other):
        i(other->i)
    {
        other->i=0;
    }
    operator boost::detail::thread_move_t<X>()
    {
        return boost::detail::thread_move_t<X>(*this);
    }
#endif
    ~X()
    {}
};

int make_int()
{
    return 42;
}

int throw_runtime_error()
{
    throw std::runtime_error("42");
}

void set_promise_thread(jss::promise<int>* p)
{
    p->set_value(42);
}

struct my_exception
{};

void set_promise_exception_thread(jss::promise<int>* p)
{
    p->set_exception(boost::copy_exception(my_exception()));
}


void test_store_value_from_thread()
{
    jss::promise<int> pi2;
    jss::unique_future<int> fi2=pi2.get_future();
    boost::thread(set_promise_thread,&pi2);
    int j=fi2.get();
    assert(j==42);
    assert(fi2.is_ready());
    assert(fi2.has_value());
    assert(!fi2.has_exception());
    assert(fi2.get_state()==jss::future_state::ready);
}


void test_store_exception()
{
    jss::promise<int> pi3;
    jss::unique_future<int> fi3=pi3.get_future();
    boost::thread(set_promise_exception_thread,&pi3);
    try
    {
        fi3.get();
        assert(false);
    }
    catch(my_exception)
    {
        assert(true);
    }
    
    assert(fi3.is_ready());
    assert(!fi3.has_value());
    assert(fi3.has_exception());
    assert(fi3.get_state()==jss::future_state::ready);
}

void test_initial_state()
{
    jss::unique_future<int> fi;
    assert(!fi.is_ready());
    assert(!fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::uninitialized);
    int i;
    try
    {
        i=fi.get();
        assert(false);
    }
    catch(jss::future_uninitialized)
    {
        assert(true);
    }
}

void test_waiting_future()
{
    jss::promise<int> pi;
    jss::unique_future<int> fi;
    fi=pi.get_future();

    int i=0;
    assert(!fi.is_ready());
    assert(!fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::waiting);
    assert(i==0);
}

void test_cannot_get_future_twice()
{
    jss::promise<int> pi;
    pi.get_future();

    try
    {
        pi.get_future();
        assert(false);
    }
    catch(jss::future_already_retrieved&)
    {
        assert(true);
    }
}

void test_set_value_updates_future_state()
{
    jss::promise<int> pi;
    jss::unique_future<int> fi;
    fi=pi.get_future();

    pi.set_value(42);
    
    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::ready);
}

void test_set_value_can_be_retrieved()
{
    jss::promise<int> pi;
    jss::unique_future<int> fi;
    fi=pi.get_future();

    pi.set_value(42);
    
    int i=0;
    assert(i=fi.get());
    assert(i==42);
    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::ready);
}

void test_set_value_can_be_moved()
{
//     jss::promise<int> pi;
//     jss::unique_future<int> fi;
//     fi=pi.get_future();

//     pi.set_value(42);
    
//     int i=0;
//     assert(i=fi.get());
//     assert(i==42);
//     assert(fi.is_ready());
//     assert(fi.has_value());
//     assert(!fi.has_exception());
//     assert(fi.get_state()==jss::future_state::ready);
}

void test_future_from_packaged_task_is_waiting()
{
    jss::packaged_task<int> pt(make_int);
    jss::unique_future<int> fi=pt.get_future();
    int i=0;
    assert(!fi.is_ready());
    assert(!fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::waiting);
    assert(i==0);
}

void test_invoking_a_packaged_task_populates_future()
{
    jss::packaged_task<int> pt(make_int);
    jss::unique_future<int> fi=pt.get_future();

    pt();

    int i=0;
    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::ready);
    assert(i=fi.get());
    assert(i==42);
}

void test_invoking_a_packaged_task_twice_throws()
{
    jss::packaged_task<int> pt(make_int);

    pt();
    try
    {
        pt();
        assert(false);
    }
    catch(jss::task_already_started)
    {
        assert(true);
    }
}


void test_cannot_get_future_twice_from_task()
{
    jss::packaged_task<int> pt(make_int);
    pt.get_future();
    try
    {
        pt.get_future();
        assert(false);
    }
    catch(jss::future_already_retrieved)
    {
        assert(true);
    }
}

void test_task_stores_exception_if_function_throws()
{
    jss::packaged_task<int> pt(throw_runtime_error);
    jss::unique_future<int> fi=pt.get_future();

    pt();

    int i=0;
    assert(fi.is_ready());
    assert(!fi.has_value());
    assert(fi.has_exception());
    assert(fi.get_state()==jss::future_state::ready);
    try
    {
        fi.get();
        assert(false);
    }
    catch(std::exception&)
    {
        assert(true);
    }
    catch(...)
    {
        assert(!"Unknown exception thrown");
    }
    
}

void test_void_promise()
{
    jss::promise<void> p;
    jss::unique_future<void> f=p.get_future();
    p.set_value();
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    assert(f.get_state()==jss::future_state::ready);
    f.get();
}

void test_reference_promise()
{
    jss::promise<int&> p;
    jss::unique_future<int&> f=p.get_future();
    int i=42;
    p.set_value(i);
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    assert(f.get_state()==jss::future_state::ready);
    assert(&f.get()==&i);
}

void do_nothing()
{}

void test_task_returning_void()
{
    jss::packaged_task<void> pt(do_nothing);
    jss::unique_future<void> fi=pt.get_future();

    pt();

    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::ready);
}

int global_ref_target=0;

int& return_ref()
{
    return global_ref_target;
}

void test_task_returning_reference()
{
    jss::packaged_task<int&> pt(return_ref);
    jss::unique_future<int&> fi=pt.get_future();

    pt();

    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==jss::future_state::ready);
    int& i=fi.get();
    assert(&i==&global_ref_target);
}

void test_shared_future()
{
    jss::packaged_task<int> pt(make_int);
    jss::unique_future<int> fi=pt.get_future();

    jss::shared_future<int> sf(::move(fi));
    assert(fi.get_state()==jss::future_state::uninitialized);

    pt();

    int i=0;
    assert(sf.is_ready());
    assert(sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==jss::future_state::ready);
    assert(i=sf.get());
    assert(i==42);
}

void test_copies_of_shared_future_become_ready_together()
{
    jss::packaged_task<int> pt(make_int);
    jss::unique_future<int> fi=pt.get_future();

    jss::shared_future<int> sf(::move(fi));
    jss::shared_future<int> sf2(sf);
    jss::shared_future<int> sf3;
    sf3=sf;
    assert(sf.get_state()==jss::future_state::waiting);
    assert(sf2.get_state()==jss::future_state::waiting);
    assert(sf3.get_state()==jss::future_state::waiting);

    pt();

    int i=0;
    assert(sf.is_ready());
    assert(sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==jss::future_state::ready);
    assert(i=sf.get());
    assert(i==42);
    i=0;
    assert(sf2.is_ready());
    assert(sf2.has_value());
    assert(!sf2.has_exception());
    assert(sf2.get_state()==jss::future_state::ready);
    assert(i=sf2.get());
    assert(i==42);
    i=0;
    assert(sf3.is_ready());
    assert(sf3.has_value());
    assert(!sf3.has_exception());
    assert(sf3.get_state()==jss::future_state::ready);
    assert(i=sf3.get());
    assert(i==42);
}

void test_shared_future_can_be_move_assigned_from_unique_future()
{
    jss::packaged_task<int> pt(make_int);
    jss::unique_future<int> fi=pt.get_future();

    jss::shared_future<int> sf;
    sf=::move(fi);
    assert(fi.get_state()==jss::future_state::uninitialized);

    int i=0;
    assert(!sf.is_ready());
    assert(!sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==jss::future_state::waiting);
}

void test_shared_future_void()
{
    jss::packaged_task<void> pt(do_nothing);
    jss::unique_future<void> fi=pt.get_future();

    jss::shared_future<void> sf(::move(fi));
    assert(fi.get_state()==jss::future_state::uninitialized);

    pt();

    assert(sf.is_ready());
    assert(sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==jss::future_state::ready);
    sf.get();
}

void test_shared_future_ref()
{
    jss::promise<int&> p;
    jss::shared_future<int&> f(p.get_future());
    int i=42;
    p.set_value(i);
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    assert(f.get_state()==jss::future_state::ready);
    assert(&f.get()==&i);
}

void test_can_get_a_second_future_from_a_moved_promise()
{
    jss::promise<int> pi;
    jss::unique_future<int> fi=pi.get_future();
    
    jss::promise<int> pi2(::move(pi));
    jss::unique_future<int> fi2=pi.get_future();

    pi2.set_value(3);
    assert(fi.is_ready());
    assert(!fi2.is_ready());
    assert(fi.get()==3);
    pi.set_value(42);
    assert(fi2.is_ready());
    assert(fi2.get()==42);
}

void test_can_get_a_second_future_from_a_moved_void_promise()
{
    jss::promise<void> pi;
    jss::unique_future<void> fi=pi.get_future();
    
    jss::promise<void> pi2(::move(pi));
    jss::unique_future<void> fi2=pi.get_future();

    pi2.set_value();
    assert(fi.is_ready());
    assert(!fi2.is_ready());
    pi.set_value();
    assert(fi2.is_ready());
}

void test_unique_future_for_move_only_udt()
{
    jss::promise<X> pt;
    jss::unique_future<X> fi=pt.get_future();

    pt.set_value(X());
    X res(fi.get());
    assert(res.i==42);
}

void test_unique_future_for_string()
{
    jss::promise<std::string> pt;
    jss::unique_future<std::string> fi=pt.get_future();

    pt.set_value(std::string("hello"));
    std::string res(fi.get());
    assert(res=="hello");

    jss::promise<std::string> pt2;
    fi=pt2.get_future();

    std::string const s="goodbye";
    
    pt2.set_value(s);
    res=fi.get();
    assert(res=="goodbye");

    jss::promise<std::string> pt3;
    fi=pt3.get_future();

    std::string s2="foo";
    
    pt3.set_value(s2);
    res=fi.get();
    assert(res=="foo");
}

boost::mutex callback_mutex;
unsigned callback_called=0;

void wait_callback(jss::promise<int>& pi)
{
    boost::lock_guard<boost::mutex> lk(callback_mutex);
    ++callback_called;
    try
    {
        pi.set_value(42);
    }
    catch(...)
    {
    }
}

void do_nothing_callback(jss::promise<int>& pi)
{
    boost::lock_guard<boost::mutex> lk(callback_mutex);
    ++callback_called;
}

void test_wait_callback()
{
    callback_called=0;
    jss::promise<int> pi;
    jss::unique_future<int> fi=pi.get_future();
    pi.set_wait_callback(wait_callback);
    fi.wait();
    assert(callback_called);
    assert(fi.get()==42);
    fi.wait();
    fi.wait();
    assert(callback_called==1);
}

void test_wait_callback_with_timed_wait()
{
    callback_called=0;
    jss::promise<int> pi;
    jss::unique_future<int> fi=pi.get_future();
    pi.set_wait_callback(do_nothing_callback);
    bool success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(callback_called);
    assert(!success);
    success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(!success);
    success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(!success);
    assert(callback_called==3);
    pi.set_value(42);
    success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(success);
    assert(callback_called==3);
    assert(fi.get()==42);
    assert(callback_called==3);
}


void wait_callback_for_task(jss::packaged_task<int>& pt)
{
    boost::lock_guard<boost::mutex> lk(callback_mutex);
    ++callback_called;
    try
    {
        pt();
    }
    catch(...)
    {
    }
}


void test_wait_callback_for_packaged_task()
{
    callback_called=0;
    jss::packaged_task<int> pt(make_int);
    jss::unique_future<int> fi=pt.get_future();
    pt.set_wait_callback(wait_callback_for_task);
    fi.wait();
    assert(callback_called);
    assert(fi.get()==42);
    fi.wait();
    fi.wait();
    assert(callback_called==1);
}

void test_packaged_task_can_be_moved()
{
    jss::packaged_task<int> pt(make_int);

    jss::unique_future<int> fi=pt.get_future();

    assert(!fi.is_ready());
    
    jss::packaged_task<int> pt2=::move(pt);

    assert(!fi.is_ready());
    try
    {
        pt();
        assert(!"Can invoke moved task!");
    }
    catch(jss::task_moved&)
    {
    }

    assert(!fi.is_ready());

    pt2();
    
    assert(fi.is_ready());
}

void test_destroying_a_promise_stores_broken_promise()
{
    jss::unique_future<int> f;
    
    {
        jss::promise<int> p;
        f=p.get_future();
    }
    assert(f.is_ready());
    assert(f.has_exception());
    try
    {
        f.get();
    }
    catch(jss::broken_promise&)
    {
    }
}

void test_destroying_a_packaged_task_stores_broken_promise()
{
    jss::unique_future<int> f;
    
    {
        jss::packaged_task<int> p(make_int);
        f=p.get_future();
    }
    assert(f.is_ready());
    assert(f.has_exception());
    try
    {
        f.get();
    }
    catch(jss::broken_promise&)
    {
    }
}

int make_int_slowly()
{
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    return 42;
}

void test_wait_for_either_of_two_futures_1()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    
    boost::thread(::move(pt));
    
    unsigned const future=jss::wait_for_any(f1,f2);
    
    assert(future==0);
    assert(f1.is_ready());
    assert(!f2.is_ready());
    assert(f1.get()==42);
}

void test_wait_for_either_of_two_futures_2()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    
    boost::thread(::move(pt2));
    
    unsigned const future=jss::wait_for_any(f1,f2);
    
    assert(future==1);
    assert(!f1.is_ready());
    assert(f2.is_ready());
    assert(f2.get()==42);
}

void test_wait_for_either_of_three_futures_1()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    
    boost::thread(::move(pt));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3);
    
    assert(future==0);
    assert(f1.is_ready());
    assert(!f2.is_ready());
    assert(!f3.is_ready());
    assert(f1.get()==42);
}

void test_wait_for_either_of_three_futures_2()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    
    boost::thread(::move(pt2));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3);
    
    assert(future==1);
    assert(!f1.is_ready());
    assert(f2.is_ready());
    assert(!f3.is_ready());
    assert(f2.get()==42);
}

void test_wait_for_either_of_three_futures_3()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    
    boost::thread(::move(pt3));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3);
    
    assert(future==2);
    assert(!f1.is_ready());
    assert(!f2.is_ready());
    assert(f3.is_ready());
    assert(f3.get()==42);
}

void test_wait_for_either_of_four_futures_1()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    
    boost::thread(::move(pt));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4);
    
    assert(future==0);
    assert(f1.is_ready());
    assert(!f2.is_ready());
    assert(!f3.is_ready());
    assert(!f4.is_ready());
    assert(f1.get()==42);
}

void test_wait_for_either_of_four_futures_2()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    
    boost::thread(::move(pt2));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4);
    
    assert(future==1);
    assert(!f1.is_ready());
    assert(f2.is_ready());
    assert(!f3.is_ready());
    assert(!f4.is_ready());
    assert(f2.get()==42);
}

void test_wait_for_either_of_four_futures_3()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    
    boost::thread(::move(pt3));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4);
    
    assert(future==2);
    assert(!f1.is_ready());
    assert(!f2.is_ready());
    assert(f3.is_ready());
    assert(!f4.is_ready());
    assert(f3.get()==42);
}

void test_wait_for_either_of_four_futures_4()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    
    boost::thread(::move(pt4));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4);
    
    assert(future==3);
    assert(!f1.is_ready());
    assert(!f2.is_ready());
    assert(!f3.is_ready());
    assert(f4.is_ready());
    assert(f4.get()==42);
}

void test_wait_for_either_of_five_futures_1()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    jss::packaged_task<int> pt5(make_int_slowly);
    jss::unique_future<int> f5(pt5.get_future());
    
    boost::thread(::move(pt));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4,f5);
    
    assert(future==0);
    assert(f1.is_ready());
    assert(!f2.is_ready());
    assert(!f3.is_ready());
    assert(!f4.is_ready());
    assert(!f5.is_ready());
    assert(f1.get()==42);
}

void test_wait_for_either_of_five_futures_2()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    jss::packaged_task<int> pt5(make_int_slowly);
    jss::unique_future<int> f5(pt5.get_future());
    
    boost::thread(::move(pt2));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4,f5);
    
    assert(future==1);
    assert(!f1.is_ready());
    assert(f2.is_ready());
    assert(!f3.is_ready());
    assert(!f4.is_ready());
    assert(!f5.is_ready());
    assert(f2.get()==42);
}
void test_wait_for_either_of_five_futures_3()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    jss::packaged_task<int> pt5(make_int_slowly);
    jss::unique_future<int> f5(pt5.get_future());
    
    boost::thread(::move(pt3));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4,f5);
    
    assert(future==2);
    assert(!f1.is_ready());
    assert(!f2.is_ready());
    assert(f3.is_ready());
    assert(!f4.is_ready());
    assert(!f5.is_ready());
    assert(f3.get()==42);
}
void test_wait_for_either_of_five_futures_4()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    jss::packaged_task<int> pt5(make_int_slowly);
    jss::unique_future<int> f5(pt5.get_future());
    
    boost::thread(::move(pt4));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4,f5);
    
    assert(future==3);
    assert(!f1.is_ready());
    assert(!f2.is_ready());
    assert(!f3.is_ready());
    assert(f4.is_ready());
    assert(!f5.is_ready());
    assert(f4.get()==42);
}
void test_wait_for_either_of_five_futures_5()
{
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> f1(pt.get_future());
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> f2(pt2.get_future());
    jss::packaged_task<int> pt3(make_int_slowly);
    jss::unique_future<int> f3(pt3.get_future());
    jss::packaged_task<int> pt4(make_int_slowly);
    jss::unique_future<int> f4(pt4.get_future());
    jss::packaged_task<int> pt5(make_int_slowly);
    jss::unique_future<int> f5(pt5.get_future());
    
    boost::thread(::move(pt5));
    
    unsigned const future=jss::wait_for_any(f1,f2,f3,f4,f5);
    
    assert(future==4);
    assert(!f1.is_ready());
    assert(!f2.is_ready());
    assert(!f3.is_ready());
    assert(!f4.is_ready());
    assert(f5.is_ready());
    assert(f5.get()==42);
}

void test_wait_for_either_invokes_callbacks()
{
    callback_called=0;
    jss::packaged_task<int> pt(make_int_slowly);
    jss::unique_future<int> fi=pt.get_future();
    jss::packaged_task<int> pt2(make_int_slowly);
    jss::unique_future<int> fi2=pt2.get_future();
    pt.set_wait_callback(wait_callback_for_task);

    boost::thread(::move(pt));
    
    jss::wait_for_any(fi,fi2);
    assert(callback_called==1);
    assert(fi.get()==42);
}

void test_wait_for_any_from_range()
{
    unsigned const count=10;
    for(unsigned i=0;i<count;++i)
    {
        jss::packaged_task<int> tasks[count];
        jss::unique_future<int> futures[count];
        for(unsigned j=0;j<count;++j)
        {
            tasks[j]=jss::packaged_task<int>(make_int_slowly);
            futures[j]=tasks[j].get_future();
        }
        boost::thread(::move(tasks[i]));
    
        jss::unique_future<int>* const future=jss::wait_for_any(futures,futures+count);
    
        assert(future==(futures+i));
        for(unsigned j=0;j<count;++j)
        {
            if(j!=i)
            {
                assert(!futures[j].is_ready());
            }
            else
            {
                assert(futures[j].is_ready());
            }
        }
        assert(futures[i].get()==42);
    }
}

void test_wait_for_all_from_range()
{
    unsigned const count=10;
    jss::unique_future<int> futures[count];
    for(unsigned j=0;j<count;++j)
    {
        jss::packaged_task<int> task(make_int_slowly);
        futures[j]=task.get_future();
        boost::thread(::move(task));
    }
    
    jss::wait_for_all(futures,futures+count);
    
    for(unsigned j=0;j<count;++j)
    {
        assert(futures[j].is_ready());
    }
}

void test_wait_for_all_two_futures()
{
    unsigned const count=2;
    jss::unique_future<int> futures[count];
    for(unsigned j=0;j<count;++j)
    {
        jss::packaged_task<int> task(make_int_slowly);
        futures[j]=task.get_future();
        boost::thread(::move(task));
    }
    
    jss::wait_for_all(futures[0],futures[1]);
    
    for(unsigned j=0;j<count;++j)
    {
        assert(futures[j].is_ready());
    }
}

void test_wait_for_all_three_futures()
{
    unsigned const count=3;
    jss::unique_future<int> futures[count];
    for(unsigned j=0;j<count;++j)
    {
        jss::packaged_task<int> task(make_int_slowly);
        futures[j]=task.get_future();
        boost::thread(::move(task));
    }
    
    jss::wait_for_all(futures[0],futures[1],futures[2]);
    
    for(unsigned j=0;j<count;++j)
    {
        assert(futures[j].is_ready());
    }
}

void test_wait_for_all_four_futures()
{
    unsigned const count=4;
    jss::unique_future<int> futures[count];
    for(unsigned j=0;j<count;++j)
    {
        jss::packaged_task<int> task(make_int_slowly);
        futures[j]=task.get_future();
        boost::thread(::move(task));
    }
    
    jss::wait_for_all(futures[0],futures[1],futures[2],futures[3]);
    
    for(unsigned j=0;j<count;++j)
    {
        assert(futures[j].is_ready());
    }
}

void test_wait_for_all_five_futures()
{
    unsigned const count=5;
    jss::unique_future<int> futures[count];
    for(unsigned j=0;j<count;++j)
    {
        jss::packaged_task<int> task(make_int_slowly);
        futures[j]=task.get_future();
        boost::thread(::move(task));
    }
    
    jss::wait_for_all(futures[0],futures[1],futures[2],futures[3],futures[4]);
    
    for(unsigned j=0;j<count;++j)
    {
        assert(futures[j].is_ready());
    }
}


int main()
{
    test_initial_state();
    //test_waiting_future();
    //test_cannot_get_future_twice();
    //test_set_value_updates_future_state();
    //test_set_value_can_be_retrieved();
    //test_set_value_can_be_moved();
    //test_store_value_from_thread();
    //test_store_exception();
    //test_future_from_packaged_task_is_waiting();
    //test_invoking_a_packaged_task_populates_future();
    //test_invoking_a_packaged_task_twice_throws();
    //test_cannot_get_future_twice_from_task();
    //test_task_stores_exception_if_function_throws();
    //test_void_promise();
    //test_reference_promise();
    //test_task_returning_void();
    //test_task_returning_reference();
    //test_shared_future();
    //test_copies_of_shared_future_become_ready_together();
    //test_shared_future_can_be_move_assigned_from_unique_future();
    //test_shared_future_void();
    //test_shared_future_ref();
    //test_can_get_a_second_future_from_a_moved_promise();
    //test_can_get_a_second_future_from_a_moved_void_promise();
    //test_unique_future_for_move_only_udt();
    //test_unique_future_for_string();
    //test_wait_callback();
    //test_wait_callback_with_timed_wait();
    //test_wait_callback_for_packaged_task();
    //test_packaged_task_can_be_moved();
    //test_destroying_a_promise_stores_broken_promise();
    //test_destroying_a_packaged_task_stores_broken_promise();
    //test_wait_for_either_of_two_futures_1();
    //test_wait_for_either_of_two_futures_2();
    //test_wait_for_either_of_three_futures_1();
    //test_wait_for_either_of_three_futures_2();
    //test_wait_for_either_of_three_futures_3();
    //test_wait_for_either_of_four_futures_1();
    //test_wait_for_either_of_four_futures_2();
    //test_wait_for_either_of_four_futures_3();
    //test_wait_for_either_of_four_futures_4();
    //test_wait_for_either_of_five_futures_1();
    //test_wait_for_either_of_five_futures_2();
    //test_wait_for_either_of_five_futures_3();
    //test_wait_for_either_of_five_futures_4();
    //test_wait_for_either_of_five_futures_5();
    //test_wait_for_either_invokes_callbacks();
    //test_wait_for_any_from_range();
    //test_wait_for_all_from_range();
    //test_wait_for_all_two_futures();
    //test_wait_for_all_three_futures();
    //test_wait_for_all_four_futures();
    //test_wait_for_all_five_futures();

	std::cin.get();
}
