/*
 * Copyright 2012, Yunjie Lu.  All rights reserved.
 * https://github.com/lyjdamzwf/chaos
 *
 * Use of this source code is governed by a BSD-style
 * license that can be found in the License file.
 */

#ifndef _CHAOS_THREAD_H_
#define _CHAOS_THREAD_H_

/*! 
 *  @file           thread.h
 *  @author         yunjie.lu
 *  @email          lyjdamzwf@gmail.com
 *  @weibo          http://weibo.com/crazyprogramerlyj
 *  @date           2012.3.18
 *  @brief          thread
 *  @last changed
 *
 */

#include <string>
#include <vector>

#include "utility_inc.h"
#include "async_method_inc.h"
#include "log_inc.h"

#include "mutex.h"
#include "condition.h"
#include "rwlock.h"

namespace chaos
{

namespace thread
{

using namespace std;
using namespace chaos::utility;
using namespace chaos::async_method;
using namespace chaos::log;

//! yunjie: �߳����װ
class thread_t: private noncopyable_t
{
public:
    typedef             void* (*thread_func_t) (void*);

public:
    thread_t(const string& name_ = string("thread_t"), bool joinable_ = true)
        : m_name(name_), 
          m_joinable(joinable_),
          m_thread(0), 
          m_alive(false), 
          m_complete(false)
{
}

    virtual ~thread_t() 
    {
        m_thread_func.release();
    }

    /* ��ȡ��ǰ�̱߳�� */
    static pthread_t get_current_thread_id() { return ::pthread_self(); }

    /* ʹ�߳�˯��ָ��ʱ��(��) */
    static void sleep(const long sec_) { ::sleep(sec_); }

    /* ʹ�߳�˯��ָ��ʱ��(����) */
    static void msleep(const long msec_) { ::usleep(1000 * msec_); }

    /* ʹ�߳�˯��ָ��ʱ��(΢��) */
    static void usleep(const long usec_) { ::usleep(usec_); }

    /* ����߳��Ƿ�������״̬ */
    const bool is_alive() const { return m_alive; }

    /* ����߳��Ƿ���joinable�� */
    const bool is_joinable() const { return m_joinable; }
    const bool is_final() { return m_complete; }
    void final() { m_complete = true; }

    void join();
    bool start(async_method_t func_);

    void cond_wait();
    void cond_wait(struct timeval& now_, uint64_t timeout_second_, uint64_t timeout_microsecond_);
    void cond_signal();


    static void *thread_func(void *arg);

    bool operator == (const thread_t* other_) const
    {
        return pthread_equal(m_thread, other_->m_thread) != 0;
    }

    bool operator != (const thread_t *other_) const
    {
        return !operator==(other_);
    }

    const std::string& get_thread_name() const { return m_name; }

    const pthread_t& get_thread_id() const { return m_thread; }

private:
    string                          m_name;		    /* �߳����� */
    mutex_t                         m_lock;			/* ������   */
    condition_t                     m_cond;			/* �������� */

    bool                            m_joinable;	    /* �߳�Joinable���� */
    pthread_t                       m_thread;		/* �̱߳�� */

    volatile bool                   m_alive;		/* �߳��Ƿ������� */
    volatile bool                   m_complete;		/* �߳��Ƿ񽫽��� */

    async_method_t                  m_thread_func;
};

}

}

#endif /*_CHAOS_THREAD_H_ */
