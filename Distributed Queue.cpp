#include <bits/stdc++.h>
using namespace std;

/*##########################################################################
System Requirements
--------------------
Design an In-Memory Distributed Queue like Kafka.

Core Use Cases
--------------
1. The queue should be in-memory and should not require access to the file system.
2. There can be multiple topics in the queue.
3. A (string) message can be published on a topic by a producer/publisher and consumers/subscribers can subscribe to the topic to receive the messages.
4. There can be multiple producers and consumers.
5. A producer can publish to multiple topics.
6. A consumer can listen to multiple topics.
7. The consumer should print "<consumer_id> received <message>" on receiving the message.
8. The queue system should be multi-threaded, i.e., messages can be produced or consumed in parallel by different producers/consumers.

##########################################################################*/

class Message
{
private:
    std::string content;

public:
    explicit Message(const std::string &content) : content(content) {}

    std::string getContent() const { return content; }
};

class IProducer
{
public:
    virtual ~IProducer() = default;
    virtual void produceMessage(ITopic *topic, Message *message) = 0;
};

class IConsumer
{
public:
    virtual void receiveMessage(const Message &message) = 0;
    virtual ~IConsumer() = default;
};

class Consumer : public IConsumer
{
private:
    std::string consumerId;
    set<std::string> subscribedTopics;

public:
    explicit Consumer(const std::string &id) : consumerId(id) {}

    void receiveMessage(const Message &message) override
    {
        std::cout << consumerId << " received " << message.getContent() << std::endl;
    }

    void addSubscribedTopic(const std::string &topicName)
    {
        subscribedTopics.insert(topicName);
    }

    void removeSubscribedTopic(const std::string &topicName)
    {
        subscribedTopics.erase(topicName);
    }

    bool isSubscribedTo(const std::string &topicName) const
    {
        return subscribedTopics.find(topicName) != subscribedTopics.end();
    }

    const std::set<std::string> &getSubscribedTopics() const
    {
        return subscribedTopics;
    }
};

class Producer
{
public:
    void produceMessage(ITopic *topic, const std::string &content)
    {
        Message *message = new Message(content);
        topic->addMessage(message);
    }
};

class ITopic
{
public:
    virtual void addMessage(Message *message) = 0;
    virtual Message *getNextMessage() = 0;
    virtual void subscribe(Consumer *consumer) = 0;
    virtual void unsubscribe(Consumer *consumer) = 0;
    virtual std::set<Consumer *> getSubscribers() const = 0;
    virtual ~ITopic() = default;
};

class Topic : public ITopic
{
private:
    std::string name;
    std::deque<Message *> messageQueue;
    std::set<Consumers *> subscribers;
    std::mutex queueMutex;
    std::condition_variable queueCV;

public:
    explicit Topic(const std::string &name) : name(name) {}

    void addMessage(Message *message) override
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        messageQueue.push_back(message);
        queueCV.notify_all();
    }

    Message *getNextMessage() override
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        while (messageQueue.empty())
        {
            queueCV.wait(lock);
        }
        if (messageQueue.empty())
            return nullptr;
        Message *message = messageQueue.front();
        messageQueue.pop_front();
        return message;
    }

    void subscribe(Consumer *consumer) override
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        subscribers.insert(consumer);
    }

    void unsubscribe(Consumer *consumer) override
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        subscribers.erase(consumer);
    }

    std::set<Consumer *> getSubscribers() const override
    {
        return subscribers;
    }

    std::string getName() const { return name; }

    void notifySubscribers()
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        for (auto &subscriber : subscribers)
        {
            while (!messageQueue.empty())
            {
                Message *message = messageQueue.front();
                messageQueue.pop_front();
                subscriber->receiveMessage(*message);
                delete message; // Clean up message after sending
            }
        }
    }
};

void producerFunction(Producer *producer, Topic *topic, const std::vector<std::string> &messages)
{
    for (const auto &message : messages)
    {
        producer->produceMessage(topic, new Message(message));
    }
}

void consumerFunction(Consumer *consumer, Topic *topic)
{
    while (true)
    {
        Message *message = topic->getNextMessage();
        if (message)
        {
            consumer->receiveMessage(*message);
            delete message; // Clean up message after processing
        }
    }
}

int main()
{
    // Create topics
    Topic topic1("topic1");
    Topic topic2("topic2");

    // Create consumers
    Consumer consumer1("consumer1");
    Consumer consumer2("consumer2");
    Consumer consumer3("consumer3");
    Consumer consumer4("consumer4");
    Consumer consumer5("consumer5");

    // Subscribe consumers to topics
    topic1.subscribe(&consumer1);
    topic1.subscribe(&consumer2);
    topic1.subscribe(&consumer3);
    topic1.subscribe(&consumer4);
    topic1.subscribe(&consumer5);

    topic2.subscribe(&consumer1);
    topic2.subscribe(&consumer3);
    topic2.subscribe(&consumer4);

    consumer1.addSubscribedTopic("topic1");
    consumer1.addSubscribedTopic("topic2");
    consumer2.addSubscribedTopic("topic1");
    consumer3.addSubscribedTopic("topic1");
    consumer3.addSubscribedTopic("topic2");
    consumer4.addSubscribedTopic("topic1");
    consumer4.addSubscribedTopic("topic2");
    consumer5.addSubscribedTopic("topic1");

    // Create producers
    Producer producer1;
    Producer producer2;

    // Messages to publish
    std::vector<std::string> messagesProducer1 = {"Message 1", "Message 2"};
    std::vector<std::string> messagesProducer2 = {"Message 3", "Message 5"};

    // Launch producer threads
    std::thread producerThread1(producerFunction, &producer1, &topic1, messagesProducer1);
    std::thread producerThread2(producerFunction, &producer2, &topic1, {"Message 3"});
    std::thread producerThread3(producerFunction, &producer1, &topic2, {"Message 4"});
    std::thread producerThread4(producerFunction, &producer2, &topic2, messagesProducer2);

    // Launch consumer threads
    std::thread consumerThread1(consumerFunction, &consumer1, &topic1);
    std::thread consumerThread2(consumerFunction, &consumer2, &topic1);
    std::thread consumerThread3(consumerFunction, &consumer3, &topic1);
    std::thread consumerThread4(consumerFunction, &consumer4, &topic1);
    std::thread consumerThread5(consumerFunction, &consumer5, &topic1);
    std::thread consumerThread6(consumerFunction, &consumer1, &topic2);
    std::thread consumerThread7(consumerFunction, &consumer3, &topic2);
    std::thread consumerThread8(consumerFunction, &consumer4, &topic2);

    // Join threads
    producerThread1.join();
    producerThread2.join();
    producerThread3.join();
    producerThread4.join();

    consumerThread1.join();
    consumerThread2.join();
    consumerThread3.join();
    consumerThread4.join();
    consumerThread5.join();
    consumerThread6.join();
    consumerThread7.join();
    consumerThread8.join();

    return 0;
}