//
// detail/weak_impl_handler.h
// --------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef WEAK_IMPL_HANDLER_H_1A3CF0E2_1A7C_4889_8C48_843FBDD9CFD0
#define WEAK_IMPL_HANDLER_H_1A3CF0E2_1A7C_4889_8C48_843FBDD9CFD0

#include <boost/asio.hpp>
#include <boost/weak_ptr.hpp>

namespace libkafka_asio
{
namespace detail
{

template<typename Impl, typename Handler>
class WeakImplNullaryHandlerType
{
public:
  WeakImplNullaryHandlerType(boost::weak_ptr<Impl> impl,
                             const Handler& handler) :
    impl_(impl),
    handler_(handler)
  {
  }

  void operator()()
  {
    if (impl_.expired())
    {
      return;
    }
    handler_();
  }

private:
  boost::weak_ptr<Impl> impl_;
  Handler handler_;
};

template<typename Impl, typename Handler>
class WeakImplErrorHandlerType
{
public:
  WeakImplErrorHandlerType(boost::weak_ptr<Impl> impl,
                           const Handler& handler) :
    impl_(impl),
    handler_(handler)
  {
  }

  void operator()(const boost::system::error_code& error)
  {
    if (impl_.expired())
    {
      return;
    }
    handler_(error);
  }

private:
  boost::weak_ptr<Impl> impl_;
  Handler handler_;
};

template<typename Impl, typename Handler>
class WeakImplIteratorHandlerType
{
public:
  WeakImplIteratorHandlerType(boost::weak_ptr<Impl> impl,
                              const Handler& handler) :
    impl_(impl),
    handler_(handler)
  {
  }

  void operator()(const boost::system::error_code& error,
                  const boost::asio::ip::tcp::resolver::iterator& iterator)
  {
    if (impl_.expired())
    {
      return;
    }
    handler_(error, iterator);
  }

private:
  boost::weak_ptr<Impl> impl_;
  Handler handler_;
};

template<typename Impl, typename Handler>
class WeakImplTxHandlerType
{
public:
  WeakImplTxHandlerType(boost::weak_ptr<Impl> impl,
                        const Handler& handler) :
    impl_(impl),
    handler_(handler)
  {
  }

  void operator()(const boost::system::error_code& error,
                  size_t transferred_bytes)
  {
    if (impl_.expired())
    {
      return;
    }
    handler_(error, transferred_bytes);
  }

private:
  boost::weak_ptr<Impl> impl_;
  Handler handler_;
};

template<typename Impl>
struct WeakImpl
{

  template<typename Handler>
  static WeakImplNullaryHandlerType<Impl, Handler>
  NullaryHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplNullaryHandlerType<Impl, Handler>(impl, handler);
  }

  template<typename Handler>
  static WeakImplErrorHandlerType<Impl, Handler>
  ErrorHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplErrorHandlerType<Impl, Handler>(impl, handler);
  }

  template<typename Handler>
  static WeakImplIteratorHandlerType<Impl, Handler>
  ResolveHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplIteratorHandlerType<Impl, Handler>(impl, handler);
  }

  template<typename Handler>
  static WeakImplIteratorHandlerType<Impl, Handler>
  ConnectHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplIteratorHandlerType<Impl, Handler>(impl, handler);
  }

  template<typename Handler>
  static WeakImplTxHandlerType<Impl, Handler>
  WriteHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplTxHandlerType<Impl, Handler>(impl, handler);
  }

  template<typename Handler>
  static WeakImplTxHandlerType<Impl, Handler>
  ReadHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplTxHandlerType<Impl, Handler>(impl, handler);
  }

  template<typename Handler>
  static WeakImplErrorHandlerType<Impl, Handler>
  DeadlineHandler(boost::weak_ptr<Impl> impl, const Handler& handler)
  {
    return WeakImplErrorHandlerType<Impl, Handler>(impl, handler);
  }

};

}  // namespace detail
}  // namespace libkafka_asio

#endif // WEAK_IMPL_HANDLER_H_1A3CF0E2_1A7C_4889_8C48_843FBDD9CFD0
