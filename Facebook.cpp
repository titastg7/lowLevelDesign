/*##########################################################################
System Requirements
--------------------
1. User Profile Management: Enabling creation and management of user profiles.
2. Friendship Management: Allowing users to connect as friends.
3. Posting Updates: Permitting users to post updates and view others' updates.
4. Feed Generation: Displaying a feed composed of friends' posts.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Creating and Updating User Profiles
2. Managing Friendships
3. Creating Posts
4. Viewing the Feed
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

class User;

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

class Friendship
{
private:
    User *user1;
    User *user2;

public:
    Friendship(User *user1, User *user2)
        : user1(user1), user2(user2)
    {
        establishFriendship();
    }

private:
    void establishFriendship()
    {
        user1->addFriend(user2);
        user2->addFriend(user1);
    }

    // Getters and setters...
};

class User
{
private:
    string userId;
    string name;
    vector<User *> friends;
    vector<Post *> posts;

public:
    User(const string &name) : name(name)
    {
        userId = generateUserId();
    }

    void addFriend(User *user)
    {
        friends.push_back(user);
    }

    void postUpdate(const string &content)
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

    const vector<User *> &getFriends() const
    {
        return friends;
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

class SocialNetworkSystem
{
private:
    vector<User *> users;
    vector<Friendship *> friendships;

public:
    void addUser(User *user)
    {
        users.push_back(user);
    }

    void addFriendship(Friendship *friendship)
    {
        friendships.push_back(friendship);
    }

    vector<Post *> getFeed(User *user)
    {
        vector<Post *> feed;
        for (User *friend : user->getFriends())
        {
            feed.insert(feed.end(), friend->getPosts().begin(), friend->getPosts().end());
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
    // Create social network system instance
    SocialNetworkSystem socialNetwork;

    // Create users
    User *user1 = new User("User1");
    User *user2 = new User("User2");
    User *user3 = new User("User3");

    // Add users to the social network
    socialNetwork.addUser(user1);
    socialNetwork.addUser(user2);
    socialNetwork.addUser(user3);

    // Create friendships
    Friendship *friendship1 = new Friendship(user1, user2);
    Friendship *friendship2 = new Friendship(user1, user3);

    // Add friendships to the social network
    socialNetwork.addFriendship(friendship1);
    socialNetwork.addFriendship(friendship2);

    // User1 posts an update
    user1->postUpdate("Hello, friends!");

    // Retrieve User1's feed
    vector<Post *> feed = socialNetwork.getFeed(user1);
    for (Post *post : feed)
    {
        cout << "Author: " << post->getAuthor()->getName() << ", Content: " << post->getContent() << endl;
    }

    // Clean up dynamically allocated memory
    delete user1;
    delete user2;
    delete user3;
    delete friendship1;
    delete friendship2;

    // Free posts memory
    for (Post *post : feed)
    {
        delete post;
    }

    return 0;
}
