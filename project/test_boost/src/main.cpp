#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <map>

#include "boost/filesystem.hpp"
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/asio.hpp>

using namespace std;

void print(const boost::system::error_code&)
{
    cout << "Hello, world!" << endl;
}

void handle_wait(const boost::system::error_code& error,
    boost::asio::deadline_timer& t,
    int& count)
{
    if (!error)
    {
        cout << "async_wait" << endl;
        t.expires_at(t.expires_at() + boost::posix_time::seconds(3));
        t.async_wait(boost::bind(handle_wait,
            boost::asio::placeholders::error,
            boost::ref(t),
            boost::ref(count)));
    }
}

// 同步方法  
void test_timer_syn()
{
    boost::asio::io_service ios;
    boost::asio::deadline_timer t(ios, boost::posix_time::seconds(3));
    //cout << t.expires_at() << endl;
    t.wait();
    cout << "Hello syn deadline_timer!" << endl;
}

// 异步方法: 3秒后执行print方法.   
void test_timer_asyn()
{
    boost::asio::io_service io;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
    t.async_wait(print);
    cout << "After async_wait..." << endl;
    io.run();
}

// 异步循环执行方法:   
void test_timer_asyn_loop()
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io);
    size_t a = t.expires_from_now(boost::posix_time::seconds(3));

    int count = 0;
    t.async_wait(boost::bind(handle_wait,
        boost::asio::placeholders::error,
        boost::ref(t),
        boost::ref(count)));
    io.run();
}

int main()
{
    boost::asio::io_service ioservice;
    boost::asio::io_service my_io_service;
    boost::asio::ip::tcp::resolver resolver(my_io_service);
    boost::asio::ip::tcp::resolver::query query("www.qq.com", "http");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end; // End marker.  

    while (iter != end)
    {
        boost::asio::ip::tcp::endpoint endpoint = *iter++;
        std::cout << endpoint << std::endl;
    }
    getchar();
	return 0;
}
