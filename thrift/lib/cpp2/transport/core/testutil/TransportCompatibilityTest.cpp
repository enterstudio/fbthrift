/*
 * Copyright 2017-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "thrift/lib/cpp2/transport/core/testutil/TransportCompatibilityTest.h"

#include <folly/Baton.h>
#include <folly/ScopeGuard.h>
#include <folly/io/async/EventBase.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <thrift/lib/cpp2/transport/core/ThriftClient.h>
#include <thrift/lib/cpp2/transport/util/ConnectionManager.h>
#include "thrift/lib/cpp2/transport/core/testutil/gen-cpp2/TestService.h"

namespace apache {
namespace thrift {

using namespace testing;

DEFINE_string(host, "::1", "host to connect to");

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace testutil::testservice;

TransportCompatibilityTest::TransportCompatibilityTest()
    : workerThread_("TransportCompatibilityTest_WorkerThread") {
  setupServer();
}

// Tears down after the test.
TransportCompatibilityTest::~TransportCompatibilityTest() {
  stopServer();
}

// Event handler to attach to the Thrift server so we know when it is
// ready to serve and also so we can determine the port it is
// listening on.
class TransportCompatibilityTestEventHandler
    : public server::TServerEventHandler {
 public:
  // This is a callback that is called when the Thrift server has
  // initialized and is ready to serve RPCs.
  void preServe(const folly::SocketAddress* address) override {
    port_ = address->getPort();
    baton_.post();
  }

  int32_t waitForPortAssignment() {
    baton_.wait();
    return port_;
  }

 private:
  folly::Baton<> baton_;
  int32_t port_;
};

void TransportCompatibilityTest::addRoutingHandler(
    std::unique_ptr<TransportRoutingHandler> routingHandler) {
  DCHECK(server_) << "First call setupServer() function";

  server_->addRoutingHandler(std::move(routingHandler));
}

ThriftServer* TransportCompatibilityTest::getServer() {
  DCHECK(server_) << "First call setupServer() function";

  return server_.get();
}

void TransportCompatibilityTest::setupServer() {
  DCHECK(!server_) << "First close the server with stopServer()";

  handler_ = std::make_shared<StrictMock<TestServiceMock>>();
  auto cpp2PFac =
      std::make_shared<ThriftServerAsyncProcessorFactory<TestServiceMock>>(
          handler_);

  server_ = std::make_unique<ThriftServer>();
  server_->setPort(0);
  server_->setProcessorFactory(cpp2PFac);
}

void TransportCompatibilityTest::startServer() {
  DCHECK(server_) << "First call setupServer() function";

  auto eventHandler =
      std::make_shared<TransportCompatibilityTestEventHandler>();
  server_->setServerEventHandler(eventHandler);
  server_->setup();

  // Get the port that the server has bound to
  port_ = eventHandler->waitForPortAssignment();
}

void TransportCompatibilityTest::stopServer() {
  if (server_) {
    server_->cleanUp();
    server_.reset();
    handler_.reset();
  }
}

void TransportCompatibilityTest::connectToServer(
    folly::Function<void(std::unique_ptr<TestServiceAsyncClient>)> callMe) {
  connectToServer([callMe = std::move(callMe)](
                      std::unique_ptr<TestServiceAsyncClient> client,
                      auto) mutable { callMe(std::move(client)); });
}

void TransportCompatibilityTest::connectToServer(
    folly::Function<void(
        std::unique_ptr<TestServiceAsyncClient>,
        std::shared_ptr<ClientConnectionIf>)> callMe) {
  auto mgr = ConnectionManager::getInstance();
  CHECK_GT(port_, 0) << "Check if the server has started already";
  auto connection = mgr->getConnection(FLAGS_host, port_);
  auto channel = ThriftClient::Ptr(
      new ThriftClient(connection, workerThread_.getEventBase()));
  channel->setProtocolId(apache::thrift::protocol::T_COMPACT_PROTOCOL);

  auto client = std::make_unique<TestServiceAsyncClient>(std::move(channel));

  callMe(std::move(client), std::move(connection));
}

class TimeoutTestCallback : public RequestCallback {
 public:
  TimeoutTestCallback(bool shouldTimeout) : shouldTimeout_(shouldTimeout) {}
  virtual ~TimeoutTestCallback() {
    EXPECT_TRUE(callbackReceived_);
  }
  void requestSent() override {
    EXPECT_FALSE(requestSentCalled_);
    requestSentCalled_ = true;
  }
  void replyReceived(ClientReceiveState&& /*crs*/) override {
    EXPECT_TRUE(requestSentCalled_);
    EXPECT_FALSE(callbackReceived_);
    EXPECT_FALSE(shouldTimeout_);
    callbackReceived_ = true;
  }
  void requestError(ClientReceiveState&& crs) override {
    EXPECT_TRUE(requestSentCalled_);
    EXPECT_TRUE(crs.isException());
    EXPECT_TRUE(crs.exception().is_compatible_with<TTransportException>());
    EXPECT_FALSE(callbackReceived_);
    EXPECT_TRUE(shouldTimeout_);
    callbackReceived_ = true;
  }
  bool callbackReceived() {
    return callbackReceived_;
  }

 private:
  bool shouldTimeout_;
  bool requestSentCalled_{false};
  bool callbackReceived_{false};
};

void TransportCompatibilityTest::callSleep(
    TestServiceAsyncClient* client,
    int32_t timeoutMs,
    int32_t sleepMs) {
  auto cb = std::make_unique<TimeoutTestCallback>(timeoutMs < sleepMs);
  RpcOptions opts;
  opts.setTimeout(std::chrono::milliseconds(timeoutMs));
  client->sleep(opts, std::move(cb), sleepMs);
}

void TransportCompatibilityTest::TestRequestResponse_Simple() {
  connectToServer([this](std::unique_ptr<TestServiceAsyncClient> client) {
    EXPECT_CALL(*handler_.get(), sumTwoNumbers_(1, 2)).Times(2);
    EXPECT_CALL(*handler_.get(), add_(1));
    EXPECT_CALL(*handler_.get(), add_(2));
    EXPECT_CALL(*handler_.get(), add_(5));

    // Send a message
    EXPECT_EQ(3, client->sync_sumTwoNumbers(1, 2));
    EXPECT_EQ(1, client->sync_add(1));

    auto future = client->future_add(2);
    EXPECT_EQ(3, future.get());

    EXPECT_EQ(3, client->sync_sumTwoNumbers(1, 2));
    EXPECT_EQ(8, client->sync_add(5));
  });
}

void TransportCompatibilityTest::TestRequestResponse_MultipleClients() {
  const int clientCount = 10;
  EXPECT_CALL(*handler_.get(), sumTwoNumbers_(1, 2)).Times(2 * clientCount);
  EXPECT_CALL(*handler_.get(), add_(1)).Times(clientCount);
  EXPECT_CALL(*handler_.get(), add_(2)).Times(clientCount);
  EXPECT_CALL(*handler_.get(), add_(5)).Times(clientCount);

  auto lambda = [](std::unique_ptr<TestServiceAsyncClient> client) {
    // Send a message
    EXPECT_EQ(3, client->sync_sumTwoNumbers(1, 2));
    EXPECT_LE(1, client->sync_add(1));

    auto future = client->future_add(2);
    EXPECT_LE(3, future.get());

    EXPECT_EQ(3, client->sync_sumTwoNumbers(1, 2));
    EXPECT_LE(8, client->sync_add(5));
  };

  std::vector<folly::ScopedEventBaseThread> threads(clientCount);
  std::vector<folly::Promise<folly::Unit>> promises(clientCount);
  std::vector<folly::Future<folly::Unit>> futures;
  for (int i = 0; i < clientCount; ++i) {
    auto& promise = promises[i];
    futures.emplace_back(promise.getFuture());
    threads[i].getEventBase()->runInEventBaseThread([&promise, lambda, this]() {
      connectToServer(lambda);
      promise.setValue();
    });
  }
  folly::collectAll(futures);
  threads.clear();
}

void TransportCompatibilityTest::TestRequestResponse_ExpectedException() {
  EXPECT_THROW(
      connectToServer(
          [&](auto client) { client->sync_throwExpectedException(1); }),
      TestServiceException);

  EXPECT_THROW(
      connectToServer(
          [&](auto client) { client->future_throwExpectedException(1).get(); }),
      TestServiceException);
}

void TransportCompatibilityTest::TestRequestResponse_UnexpectedException() {
  EXPECT_THROW(
      connectToServer(
          [&](auto client) { client->sync_throwUnexpectedException(2); }),
      apache::thrift::TApplicationException);

  EXPECT_THROW(
      connectToServer([&](auto client) {
        client->future_throwUnexpectedException(2).get();
      }),
      apache::thrift::TApplicationException);
}

void TransportCompatibilityTest::TestRequestResponse_Timeout() {
  // Note: This test requires sufficient number of CPU threads on the
  // server so that the sleep calls are not backed up.
  // Warning: This test may be flaky due to use of timeouts.
  connectToServer([this](std::unique_ptr<TestServiceAsyncClient> client) {
    // These are all async calls.  The first batch of calls get
    // dispatched immediately, then there is a sleep, and then the
    // second batch of calls get dispatched.  All calls have separate
    // timeouts and different delays on the server side.
    callSleep(client.get(), 1, 100);
    callSleep(client.get(), 100, 0);
    callSleep(client.get(), 1, 100);
    callSleep(client.get(), 100, 0);
    callSleep(client.get(), 2000, 500);
    /* sleep override */
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    callSleep(client.get(), 1, 100);
    callSleep(client.get(), 100, 0);
    /* Sleep to give time for all callbacks to be completed */
    /* sleep override */
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  });
}

void TransportCompatibilityTest::TestRequestResponse_Header() {
  connectToServer([](std::unique_ptr<TestServiceAsyncClient> client) {
    { // Sync
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      client->sync_headers(rpcOptions);
      auto keyValue = rpcOptions.getReadHeaders();
      EXPECT_NE(keyValue.end(), keyValue.find("header_from_server"));
      EXPECT_STREQ("1", keyValue.find("header_from_server")->second.c_str());
    }

    { // Future
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      auto future = client->header_future_headers(rpcOptions);
      auto tHeader = future.get().second;
      auto keyValue = tHeader->getHeaders();
      EXPECT_NE(keyValue.end(), keyValue.find("header_from_server"));
      EXPECT_STREQ("1", keyValue.find("header_from_server")->second.c_str());
    }

    { // Callback
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      folly::Promise<folly::Unit> executed;
      auto future = executed.getFuture();
      client->headers(
          rpcOptions,
          std::unique_ptr<RequestCallback>(
              new FunctionReplyCallback([&](ClientReceiveState&& state) {
                auto keyValue = state.header()->getHeaders();
                EXPECT_NE(keyValue.end(), keyValue.find("header_from_server"));
                EXPECT_STREQ(
                    "1", keyValue.find("header_from_server")->second.c_str());

                auto exw = TestServiceAsyncClient::recv_wrapped_headers(state);
                EXPECT_FALSE(exw);
                executed.setValue();
              })));
      auto& waited = future.wait(folly::Duration(100));
      EXPECT_TRUE(waited.isReady());
    }
  });
}

void TransportCompatibilityTest::
    TestRequestResponse_Header_ExpectedException() {
  connectToServer([](std::unique_ptr<TestServiceAsyncClient> client) {
    { // Sync
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      rpcOptions.setWriteHeader("expected_exception", "1");
      bool thrown = false;
      try {
        client->sync_headers(rpcOptions);
      } catch (const std::exception& ex) {
        thrown = true;
      }
      EXPECT_TRUE(thrown);
      auto keyValue = rpcOptions.getReadHeaders();
      EXPECT_NE(keyValue.end(), keyValue.find("header_from_server"));
    }

    { // Future
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      rpcOptions.setWriteHeader("expected_exception", "1");
      auto future = client->header_future_headers(rpcOptions);
      auto& waited = future.wait();
      auto& ftry = waited.getTry();
      EXPECT_TRUE(ftry.hasException());
      EXPECT_THAT(
          ftry.tryGetExceptionObject()->what(),
          HasSubstr("TestServiceException"));
    }

    { // Callback
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      rpcOptions.setWriteHeader("expected_exception", "1");
      folly::Promise<folly::Unit> executed;
      auto future = executed.getFuture();
      client->headers(
          rpcOptions,
          std::unique_ptr<RequestCallback>(
              new FunctionReplyCallback([&](ClientReceiveState&& state) {
                auto exw = TestServiceAsyncClient::recv_wrapped_headers(state);
                EXPECT_TRUE(exw.get_exception());
                EXPECT_THAT(
                    exw.what().c_str(), HasSubstr("TestServiceException"));
                executed.setValue();
              })));
      auto& waited = future.wait(folly::Duration(100));
      CHECK(waited.isReady());
    }
  });
}

void TransportCompatibilityTest::
    TestRequestResponse_Header_UnexpectedException() {
  connectToServer([](std::unique_ptr<TestServiceAsyncClient> client) {
    { // Sync
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      rpcOptions.setWriteHeader("unexpected_exception", "1");
      EXPECT_THROW(
          client->sync_headers(rpcOptions),
          apache::thrift::TApplicationException);
      auto keyValue = rpcOptions.getReadHeaders();
      EXPECT_NE(keyValue.end(), keyValue.find("header_from_server"));
    }

    { // Future
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      rpcOptions.setWriteHeader("unexpected_exception", "1");
      auto future = client->header_future_headers(rpcOptions);
      EXPECT_THROW(future.get(), apache::thrift::TApplicationException);
    }

    { // Callback
      apache::thrift::RpcOptions rpcOptions;
      rpcOptions.setWriteHeader("header_from_client", "2");
      rpcOptions.setWriteHeader("unexpected_exception", "1");
      folly::Promise<folly::Unit> executed;
      auto future = executed.getFuture();
      client->headers(
          rpcOptions,
          std::unique_ptr<RequestCallback>(
              new FunctionReplyCallback([&](ClientReceiveState&& state) {
                auto exw = TestServiceAsyncClient::recv_wrapped_headers(state);
                EXPECT_TRUE(exw.get_exception());
                EXPECT_THAT(
                    exw.what().c_str(), HasSubstr("TApplicationException"));
                executed.setValue();
              })));
      auto& waited = future.wait(folly::Duration(100));
      EXPECT_TRUE(waited.isReady());
    }
  });
}

void TransportCompatibilityTest::TestRequestResponse_Saturation() {
  connectToServer([this](
                      std::unique_ptr<TestServiceAsyncClient> client,
                      std::shared_ptr<ClientConnectionIf> connection) {
    EXPECT_CALL(*handler_.get(), add_(3)).Times(2);
    // note that no EXPECT_CALL for add_(5)
    connection->getEventBase()->runInEventBaseThreadAndWait(
        [&]() { connection->setMaxPendingRequests(0u); });
    EXPECT_THROW(client->sync_add(5), TTransportException);

    connection->getEventBase()->runInEventBaseThreadAndWait(
        [&]() { connection->setMaxPendingRequests(1u); });
    EXPECT_EQ(3, client->sync_add(3));
    EXPECT_EQ(6, client->sync_add(3));
  });
}

void TransportCompatibilityTest::TestRequestResponse_Connection_CloseNow() {
  connectToServer([](std::unique_ptr<TestServiceAsyncClient> client,
                     std::shared_ptr<ClientConnectionIf> connection) {
    // It should not reach to server: no EXPECT_CALL for add_(3)

    // Observe the behavior if the connection is closed already
    connection->getEventBase()->runInEventBaseThreadAndWait(
        [&]() { connection->closeNow(); });

    try {
      client->sync_add(3);
      EXPECT_TRUE(false) << "sync_add should have thrown";
    } catch (TTransportException& ex) {
      EXPECT_EQ(TTransportException::NOT_OPEN, ex.getType());
    }
  });
}

void TransportCompatibilityTest::TestRequestResponse_ServerQueueTimeout() {
  connectToServer([this](
                      std::unique_ptr<TestServiceAsyncClient> client) mutable {
    int32_t numCores = sysconf(_SC_NPROCESSORS_ONLN);
    int callCount = numCores + 1; // more than the core count!
    // EXPECT_CALL(*handler_.get(), sleep_(1000)).Times(AtMost(198));

    // Queue expiration - executes some of the tasks ( = thread count)
    server_->setQueueTimeout(std::chrono::milliseconds(10));
    std::vector<folly::Future<folly::Unit>> futures(callCount);
    for (int i = 0; i < callCount; ++i) {
      futures[i] = client->future_sleep(100);
    }
    int taskTimeoutCount = 0;
    int successCount = 0;
    for (auto& future : futures) {
      auto& waitedFuture = future.wait();
      auto& triedFuture = waitedFuture.getTry();
      if (triedFuture.withException([](TTransportException& ex) {
            EXPECT_EQ(TTransportException::TIMED_OUT, ex.getType());
          })) {
        ++taskTimeoutCount;
      } else {
        ++successCount;
      }
    }
    EXPECT_LE(1, taskTimeoutCount) << "at least 1 task is expected to timeout";
    EXPECT_LE(1, successCount) << "at least 1 task is expected to succeed";

    // Task expires - even though starts executing the tasks, all expires
    server_->setQueueTimeout(std::chrono::milliseconds(1000));
    server_->setUseClientTimeout(false);
    server_->setTaskExpireTime(std::chrono::milliseconds(1));
    for (int i = 0; i < callCount; ++i) {
      futures[i] = client->future_sleep(100 + i);
    }
    taskTimeoutCount = 0;
    for (auto& future : futures) {
      auto& waitedFuture = future.wait();
      auto& triedFuture = waitedFuture.getTry();
      if (triedFuture.withException([](TTransportException& ex) {
            EXPECT_EQ(TTransportException::TIMED_OUT, ex.getType());
          })) {
        ++taskTimeoutCount;
      }
    }
    EXPECT_EQ(callCount, taskTimeoutCount)
        << "all tasks are expected to be timed out";
  });
}

void TransportCompatibilityTest::TestRequestResponse_ResponseSizeTooBig() {
  connectToServer([this](std::unique_ptr<TestServiceAsyncClient> client) {
    // Execute the function, but fail when sending the response
    EXPECT_CALL(*handler_.get(), hello_(_));

    server_->setMaxResponseSize(1);
    try {
      std::string longName(1, 'f');
      std::string result;
      client->sync_hello(result, longName);
      EXPECT_TRUE(false) << "sync_hello should have thrown";
    } catch (TApplicationException& ex) {
      EXPECT_EQ(TApplicationException::INTERNAL_ERROR, ex.getType());
    }
  });
}

void TransportCompatibilityTest::TestOneway_Simple() {
  connectToServer([this](std::unique_ptr<TestServiceAsyncClient> client) {
    EXPECT_CALL(*handler_.get(), add_(0));
    EXPECT_CALL(*handler_.get(), addAfterDelay_(0, 5));

    client->sync_addAfterDelay(0, 5);
    // Sleep a bit for oneway call to complete on server
    /* sleep override */
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_EQ(5, client->sync_add(0));
  });
}

void TransportCompatibilityTest::TestOneway_WithDelay() {
  connectToServer([this](std::unique_ptr<TestServiceAsyncClient> client) {
    EXPECT_CALL(*handler_.get(), add_(0)).Times(2);
    EXPECT_CALL(*handler_.get(), addAfterDelay_(800, 5));

    // Perform an add on the server after a delay
    client->sync_addAfterDelay(800, 5);
    // Call add to get result before the previous addAfterDelay takes
    // place - this verifies that the addAfterDelay call is really
    // oneway.
    EXPECT_EQ(0, client->sync_add(0));
    // Sleep to wait for oneway call to complete on server
    /* sleep override */
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_EQ(5, client->sync_add(0));
  });
}

void TransportCompatibilityTest::TestOneway_Saturation() {
  connectToServer([this](
                      std::unique_ptr<TestServiceAsyncClient> client,
                      std::shared_ptr<ClientConnectionIf> connection) {
    EXPECT_CALL(*handler_.get(), add_(3));
    // note that no EXPECT_CALL for addAfterDelay_(0, 5)
    EXPECT_CALL(*handler_.get(), addAfterDelay_(100, 5));
    EXPECT_CALL(*handler_.get(), addAfterDelay_(50, 5));

    connection->getEventBase()->runInEventBaseThreadAndWait(
        [&]() { connection->setMaxPendingRequests(0u); });
    client->sync_addAfterDelay(0, 5);

    // the first call is not completed as the connection was saturated
    connection->getEventBase()->runInEventBaseThreadAndWait(
        [&]() { connection->setMaxPendingRequests(1u); });
    EXPECT_EQ(3, client->sync_add(3));

    // Client should be able to issue both of these functions as
    // SINGLE_REQUEST_NO_RESPONSE doesn't need to wait for server response
    client->sync_addAfterDelay(100, 5);
    client->sync_addAfterDelay(50, 5); // TODO: H2 fails in this call.
  });
}

void TransportCompatibilityTest::TestOneway_UnexpectedException() {
  connectToServer([this](std::unique_ptr<TestServiceAsyncClient> client) {
    EXPECT_CALL(*handler_.get(), onewayThrowsUnexpectedException_(100));
    EXPECT_CALL(*handler_.get(), onewayThrowsUnexpectedException_(0));
    client->sync_onewayThrowsUnexpectedException(100);
    client->sync_onewayThrowsUnexpectedException(0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  });
}

void TransportCompatibilityTest::TestOneway_Connection_CloseNow() {
  connectToServer([](std::unique_ptr<TestServiceAsyncClient> client,
                     std::shared_ptr<ClientConnectionIf> connection) {
    // It should not reach server - no EXPECT_CALL for addAfterDelay_(0, 5)

    // Observe the behavior if the connection is closed already
    connection->getEventBase()->runInEventBaseThreadAndWait(
        [&]() { connection->closeNow(); });

    EXPECT_NO_THROW(client->sync_addAfterDelay(0, 5));
  });
}

void TransportCompatibilityTest::TestOneway_ServerQueueTimeout() {
  // TODO: Even though we observe that the timeout functionality works fine for
  // Oneway PRC calls, the AsyncProcesor still executes the `cancelled`
  // requests.
  connectToServer(
      [this](std::unique_ptr<TestServiceAsyncClient> client) mutable {
        int32_t numCores = sysconf(_SC_NPROCESSORS_ONLN);
        int callCount = numCores + 1; // more than the core count!

        // TODO: fixme T22871783: Oneway tasks don't get cancelled
        EXPECT_CALL(*handler_.get(), addAfterDelay_(100, 5))
            .Times(AtMost(2 * callCount));

        server_->setQueueTimeout(std::chrono::milliseconds(1));
        for (int i = 0; i < callCount; ++i) {
          EXPECT_NO_THROW(client->sync_addAfterDelay(100, 5));
        }

        server_->setQueueTimeout(std::chrono::milliseconds(1000));
        server_->setUseClientTimeout(false);
        server_->setTaskExpireTime(std::chrono::milliseconds(1));
        for (int i = 0; i < callCount; ++i) {
          EXPECT_NO_THROW(client->sync_addAfterDelay(100, 5));
        }
      });
}

} // namespace thrift
} // namespace apache
