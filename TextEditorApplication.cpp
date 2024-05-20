/*##########################################################################
System Requirements
--------------------
1. Implement Text Editor Application like Notepad
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Display text
2. Copy and Paste lines of text
3. Redo and Undo last command
4. Insert and delete lines

##########################################################################*/

#include <bits/stdc++.h>

using namespace std;

class Notepad
{
    vector<string> allContent;
    stack<vector<string>> undoStack;
    stack<vector<string>> redoStack;

    // Own Clipboard
    vector<string> buffer;

    vector<string> split(string text, char delim)
    {
        vector<string> result;
        auto pos = text.find(delim);
        int i = 0; // starting position for search in the given text

        while (pos != string::npos)
        {
            result.push_back(text.substr(i, pos - i));
            i = ++pos;
            pos = text.find(delim, pos);
        }
        result.push_back(text.substr(i, text.length()));
        return result;
    }

public:
    Notepad(string pText)
    {
        // assuming some delimiter to distinguish between lines
        // Spliting lines from the delimiter
        allContent = split(pText, '\n');
    }

    void display()
    {
        for (string s : allContent)
        {
            cout << s << endl;
        }
    }

    bool display(int n, int m)
    {
        if (static_cast<size_t>(n) > allContent.size())
        {
            cout << " The value of n exceeds lines in the file\n";
            return false;
        }

        if (static_cast<size_t>(m) > allContent.size())
        {
            cout << " The value of m exceeds lines in the file\n";
            return false;
        }

        if (n > m)
        {
            cout << " The value of n exceeds the value m\n";
            return false;
        }

        for (int i = n - 1; i < m; i++)
        {
            cout << allContent[i] << endl;
        }
        return true;
    }

    bool insert(int n, string pText)
    {
        if (static_cast<size_t>(n) > allContent.size())
        {
            cout << " The value of n exceeds lines in the file\n";
            return false;
        }
        undoStack.push(allContent);
        string *content = &(*(allContent.begin() + n - 1));
        *content += pText;
        return true;
    }

    bool Delete(int n)
    {
        if (static_cast<size_t>(n) > allContent.size())
        {
            cout << " The value of n exceeds lines in the file\n";
            return false;
        }
        undoStack.push(allContent);
        allContent.erase(allContent.begin() + n - 1);
        return true;
    }
    bool Delete(int n, int m)
    {
        if (static_cast<size_t>(n) > allContent.size())
        {
            cout << " The value of n exceeds lines in the file\n";
            return false;
        }

        if (static_cast<size_t>(m) > allContent.size())
        {
            cout << " The value of m exceeds lines in the file\n";
            return false;
        }

        if (n > m)
        {
            cout << " The value of n exceeds the value m\n";
            return false;
        }
        undoStack.push(allContent);
        allContent.erase(allContent.begin() + n - 1, allContent.begin() + m);
        return true;
    }

    bool copy(int n, int m)
    {
        if (static_cast<size_t>(n) > allContent.size())
        {
            cout << " The value of n exceeds lines in the file\n";
            return false;
        }

        if (static_cast<size_t>(m) > allContent.size())
        {
            cout << " The value of m exceeds lines in the file\n";
            return false;
        }

        if (n > m)
        {
            cout << " The value of n exceeds the value m\n";
            return false;
        }

        buffer.clear();
        for (auto it = allContent.begin() + n - 1; it != allContent.begin() + m; it++)
        {
            buffer.push_back(*it);
        }
        return true;
    }

    // Paste copied content to given line n
    bool paste(int n)
    {
        if (static_cast<size_t>(n) > allContent.size())
        {
            cout << " The value of n exceeds lines in the file\n";
            return false;
        }

        if (buffer.empty())
            return false;
        undoStack.push(allContent);
        allContent.insert(allContent.begin() + n - 1, buffer.begin(), buffer.end());
        return true;
    }
    bool undo()
    {
        if (undoStack.empty())
        {
            cout << " Noting to undo\n";
            return false;
        }

        redoStack.push(allContent);
        allContent = undoStack.top();
        undoStack.pop();
        return true;
    }
    bool redo()
    {
        if (redoStack.empty())
        {
            cout << " Noting to redo\n";
            return false;
        }

        undoStack.push(allContent);
        allContent = redoStack.top();
        redoStack.pop();
        return true;
    }
};

int main()
{
    // Initialize the Notepad with some text
    Notepad myNotepad("This is line 1\nThis is line 2\nThis is line 3");

    // Display the contents of the Notepad
    cout << "Original Contents:" << endl;
    myNotepad.display();
    cout << endl;

    // Display lines from 'n' to 'm'
    cout << "Displaying lines from 2 to 3:" << endl;
    myNotepad.display(2, 3);
    cout << endl;

    // Insert text at line '2'
    cout << "Inserting ' Inserted Text ' at line 2:" << endl;
    myNotepad.insert(2, " Inserted Text ");
    myNotepad.display();
    cout << endl;

    // Delete line '2'
    cout << "Deleting line 2:" << endl;
    myNotepad.Delete(2);
    myNotepad.display();
    cout << endl;

    // Undo the last operation
    cout << "Undoing the last operation:" << endl;
    myNotepad.undo();
    myNotepad.display();
    cout << endl;

    // Redo the last undone operation
    cout << "Redoing the last undone operation:" << endl;
    myNotepad.redo();
    myNotepad.display();
    cout << endl;

    // Copy lines from '1' to '2'
    cout << "Copying lines from 1 to 2:" << endl;
    myNotepad.copy(1, 2);

    // Paste the copied content at line '3'
    cout << "Pasting the copied content at line 3:" << endl;
    myNotepad.paste(3);
    myNotepad.display();
    cout << endl;

    return 0;
}