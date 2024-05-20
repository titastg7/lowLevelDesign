/*##########################################################################
System Requirements
--------------------
1. User Profile Management: Enabling creation and management of user profiles.
2. Connection Management: Enable users to connect with each other.
3. Job Posting and Application: Facilitate posting job listings and applying for them
4. Feed and Postings: Display a feed of posts and activities from connections.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Creating and Updating User Profiles
2. Adding and Managing Connections
3. Posting and Applying for Jobs
4. Viewing and Creating Posts in the Feed
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

class User
{
private:
    string userId;
    string name;
    vector<User *> connections;
    vector<Post *> posts;

public:
    User(const string &name) : name(name)
    {
        userId = generateUserId();
    }

    void connect(User *user)
    {
        connections.push_back(user);
    }

    void post(const string &content)
    {
        posts.push_back(new Post(this, content));
    }

    const string &getUserId() const
    {
        return userId;
    }

    const string &getName() const
    {
        return name;
    }

    const vector<User *> &getConnections() const
    {
        return connections;
    }

    const vector<Post *> &getPosts() const
    {
        return posts;
    }

private:
    string generateUserId()
    {
        return "USR_" + to_string(time(nullptr));
    }

    // Getters and setters...
};

class Connection
{
private:
    User *user1;
    User *user2;

public:
    Connection(User *user1, User *user2)
        : user1(user1), user2(user2)
    {
        establishConnection();
    }

private:
    void establishConnection()
    {
        user1->connect(user2);
        user2->connect(user1);
    }

    // Getters and setters...
};

class Comment
{
private:
    string commentId;
    string content;
    User *author;
    string generateCommentId()
    {
        return "COMMENT_" + to_string(time(nullptr));
    }

public:
    Comment(const string &content, User *author)
        : content(content), author(author)
    {
        commentId = generateCommentId();
    }

    const string &getCommentId() const
    {
        return commentId;
    }
};

class Post
{
private:
    User *author;
    string content;
    long timestamp;
    vector<Comment *> comments;

public:
    Post(User *author, const string &content)
        : author(author), content(content)
    {
        timestamp = time(nullptr);
    }
    void addComment(Comment *comment)
    {
        comments.push_back(comment);
    }
    // Getters and setters...
};

class Job
{
private:
    std::string jobId;
    std::string title;
    std::string description;

    std::string generateJobId()
    {
        return "J-" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    }

public:
    Job(const std::string &title, const std::string &description)
        : title(title), description(description)
    {
        jobId = generateJobId();
    }

    const std::string &getJobId() const
    {
        return jobId;
    }

    const std::string &getTitle() const
    {
        return title;
    }

    const std::string &getDescription() const
    {
        return description;
    }

    // Getters and setters...
};

class LinkedInSystem
{
private:
    std::vector<User *> users;
    std::vector<Job *> jobs;
    std::vector<Post *> posts;

public:
    LinkedInSystem() {}

    void addUser(User *user)
    {
        users.push_back(user);
    }

    void addJob(Job *job)
    {
        jobs.push_back(job);
    }

    void addPost(Post *post)
    {
        posts.push_back(post);
    }

    vector<Post *> getFeed(User *user)
    {
        vector<Post *> feed;
        for (User *connect : user->getConnections())
        {
            feed.insert(feed.end(), connect->getPosts().begin(), connect->getPosts().end());
        }
        return feed;
    }

    void postComment(User *user, Post *post, const string &commentText)
    {
        post->addComment(new Comment(commentText, user));
    }
    // Other necessary methods...
};

int main()
{
    LinkedInSystem linkedInSystem;

    User *user1 = new User("User1");
    User *user2 = new User("User2");
    User *user3 = new User("User3");

    linkedInSystem.addUser(user1);
    linkedInSystem.addUser(user2);
    linkedInSystem.addUser(user3);

    Connection *connection1 = new Connection(user1, user2);
    Connection *connection2 = new Connection(user1, user3);

    Job *job1 = new Job("Software Engineer", "Description 1");
    Job *job2 = new Job("Data Scientist", "Description 2");

    linkedInSystem.addJob(job1);
    linkedInSystem.addJob(job2);

    user1->post("Hello, connections!");

    std::vector<Post *> feed = linkedInSystem.getFeed(user1);
    for (Post *post : feed)
    {
        std::cout << "Author: " << post->getAuthor()->getName() << ", Content: " << post->getContent() << std::endl;
    }

    delete user1;
    delete user2;
    delete user3;
    delete connection1;
    delete connection2;
    delete job1;
    delete job2;

    for (Post *post : feed)
    {
        delete post;
    }

    return 0;
}