/*##########################################################################
System Requirements
--------------------
1. User Management: Users can register, log in, and manage their profiles.
2. Question Handling: Users can post, view, and search for questions.
3. Answering Questions: Users can answer posted questions.
4. Comment System: Users can comment on both questions and answers.
5. Voting System: Users can upvote or downvote questions and answers.

---------------------------------------------------------------------------------
Core Use Cases
--------------

1. Posting Questions
2. Answering Questions
3. Commenting on Questions and Answers
4. Voting on Questions and Answers

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

class Question
{
private:
    std::string title;
    std::string content;
    User *askedBy;
    std::vector<Answer *> answers;
    std::vector<Comment *> comments;
    std::vector<Vote *> votes;

public:
    Question(const std::string &title, const std::string &content, User *askedBy)
        : title(title), content(content), askedBy(askedBy) {}

    // Add an answer to this question
    void addAnswer(Answer *answer)
    {
        answers.push_back(answer);
    }

    // Add a comment to this question
    void addComment(Comment *comment)
    {
        comments.push_back(comment);
    }

    // Add a vote to this question
    void addVote(Vote *vote)
    {
        votes.push_back(vote);
    }

    // Getters and setters...
};

class User
{
private:
    std::string username;
    std::string password; // For simplicity, not encrypted here

public:
    User(const std::string &username, const std::string &password)
        : username(username), password(password) {}

    // Post a question
    Question *postQuestion(const std::string &title, const std::string &content)
    {
        return new Question(title, content, this);
    }

    // Post an answer
    Answer *postAnswer(Question *question, const std::string &answerText)
    {
        Answer *answer = new Answer(this, answerText);
        question->addAnswer(answer);
        return answer;
    }
    // Getters and setters...
};

class Answer
{
private:
    User *responder;
    std::string answerText;
    std::vector<Comment *> comments;
    std::vector<Vote *> votes;

public:
    Answer(User *responder, const std::string &answerText)
        : responder(responder), answerText(answerText) {}

    // Add a comment to this answer
    void addComment(Comment *comment)
    {
        comments.push_back(comment);
    }

    // Add a vote to this answer
    void addVote(Vote *vote)
    {
        votes.push_back(vote);
    }

    // Getters and setters...
};

class Comment
{
private:
    User *commenter;
    std::string text;

public:
    Comment(User *commenter, const std::string &text)
        : commenter(commenter), text(text) {}

    // Getters and setters...
};

class Vote
{
private:
    User *voter;
    bool isUpvote;

public:
    Vote(User *voter, bool isUpvote)
        : voter(voter), isUpvote(isUpvote) {}

    // Getters and setters...
};

class StackOverflow
{
private:
    std::vector<User> users;
    std::vector<Question> questions;

public:
    // User Management
    void registerUser(const std::string &username, const std::string &password)
    {
        users.emplace_back(username, password);
    }

    User *findUser(const std::string &username)
    {
        auto it = std::find_if(users.begin(), users.end(), [&username](const User &user)
                               { return user.getUsername() == username; });

        return (it != users.end()) ? &(*it) : nullptr;
    }

    // Question Handling
    Question *postQuestion(const std::string &username, const std::string &title, const std::string &content)
    {
        User *user = findUser(username);
        if (user)
        {
            questions.emplace_back(title, content, *user);
            return &questions.back();
        }

        return nullptr;
    }

    Question *findQuestion(const std::string &title)
    {
        auto it = std::find_if(questions.begin(), questions.end(), [&title](const Question &question)
                               { return question.getTitle() == title; });

        return (it != questions.end()) ? &(*it) : nullptr;
    }

    // Answering Questions
    Answer *postAnswer(const std::string &username, const std::string &questionTitle, const std::string &answerText)
    {
        User *user = findUser(username);
        Question *question = findQuestion(questionTitle);

        if (user && question)
        {
            question->addAnswer(Answer(*user, answerText));
            return &question->addAnswer(Answer(*user, answerText));
        }

        return nullptr;
    }

    // Comment System
    void postComment(const std::string &username, const std::string &questionTitle, const std::string &commentText)
    {
        User *user = findUser(username);
        Question *question = findQuestion(questionTitle);

        if (user && question)
        {
            question->addComment(Comment(*user, commentText));
        }
    }

    // Voting System
    void vote(const std::string &username, const std::string &questionTitle, bool isUpvote)
    {
        User *user = findUser(username);
        Question *question = findQuestion(questionTitle);

        if (user && question)
        {
            question->addVote(Vote(*user, isUpvote));
        }
    }
};

int main()
{
    StackOverflow stackOverflow;

    // Register users
    stackOverflow.registerUser("User1", "password1");
    stackOverflow.registerUser("User2", "password2");

    // Post questions
    stackOverflow.postQuestion("User1", "How to use C++ vectors?", "I'm having trouble with vectors...");
    stackOverflow.postQuestion("User2", "Best practices for memory management in C++?", "Any tips for managing memory in C++?");

    // Post answers
    stackOverflow.postAnswer("User2", "How to use C++ vectors?", "You can use the push_back method to add elements to a vector.");
    stackOverflow.postAnswer("User1", "Best practices for memory management in C++?", "Use smart pointers to manage memory automatically.");

    // Post comments
    stackOverflow.postComment("User1", "How to use C++ vectors?", "Thanks for the help!");
    stackOverflow.postComment("User2", "Best practices for memory management in C++?", "I have a similar question.");

    // Vote on questions
    stackOverflow.vote("User1", "How to use C++ vectors?", true);
    stackOverflow.vote("User2", "Best practices for memory management in C++?", false);

    return 0;
}
