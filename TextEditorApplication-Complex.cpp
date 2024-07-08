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

// Memento class
class EditorState
{
private:
    vector<string> content;

public:
    EditorState(vector<string> &content)
        : content(content) {}

    vector<string> getContent() const
    {
        return content;
    }
};

// Originator class
class Editor
{
private:
    vector<string> content;
    vector<string> clipboard;

public:
    void display() const
    {
        for (const auto &line : content)
        {
            cout << line << endl;
        }
    }

    void display(int n, int m) const
    {
        if (n < 1 || m > content.size() || n > m)
        {
            cout << "Invalid range" << endl;
            return;
        }
        for (int i = n - 1; i < m; ++i)
        {
            cout << content[i] << endl;
        }
    }

    void insert(int n, const string &text)
    {
        if (n < 1 || n > content.size() + 1)
        {
            cout << "Invalid line number" << endl;
            return;
        }
        content.insert(content.begin() + n - 1, text);
    }

    void deleteLine(int n)
    {
        if (n < 1 || n > content.size())
        {
            cout << "Invalid line number" << endl;
            return;
        }
        content.erase(content.begin() + n - 1);
    }

    void deleteRange(int n, int m)
    {
        if (n < 1 || m > content.size() || n > m)
        {
            cout << "Invalid range" << endl;
            return;
        }
        content.erase(content.begin() + n - 1, content.begin() + m);
    }

    void copy(int n, int m)
    {
        if (n < 1 || m > content.size() || n > m)
        {
            cout << "Invalid range" << endl;
            return;
        }
        clipboard.clear();
        for (int i = n - 1; i < m; ++i)
        {
            clipboard.push_back(content[i]);
        }
    }

    void paste(int n)
    {
        if (n < 1 || n > content.size() + 1)
        {
            cout << "Invalid line number" << endl;
            return;
        }
        content.insert(content.begin() + n - 1, clipboard.begin(), clipboard.end());
    }

    EditorState save() const
    {
        return EditorState(content);
    }

    void restore(const EditorState &state)
    {
        content = state.getContent();
    }
};

// Caretaker class
class EditorHistory
{
private:
    stack<EditorState> undoHistory;
    stack<EditorState> redoHistory;

public:
    void saveState(const EditorState &state)
    {
        undoHistory.push(state);
        while (!redoHistory.empty())
        {
            redoHistory.pop();
        }
    }

    EditorState undo()
    {
        if (undoHistory.empty())
        {
            throw out_of_range("No states to undo.");
        }
        EditorState state = undoHistory.top();
        undoHistory.pop();
        redoHistory.push(state);
        return state;
    }

    EditorState redo()
    {
        if (redoHistory.empty())
        {
            throw out_of_range("No states to redo.");
        }
        EditorState state = redoHistory.top();
        redoHistory.pop();
        undoHistory.push(state);
        return state;
    }

    bool canUndo() const
    {
        return !undoHistory.empty();
    }

    bool canRedo() const
    {
        return !redoHistory.empty();
    }
};

// Command interface
class Command
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// ConcreteCommand for inserting text
class InsertTextCommand : public Command
{
private:
    Editor *editor;
    EditorHistory *history;
    int line;
    string text;

public:
    InsertTextCommand(Editor *editor, EditorHistory *history, int line, const string &text)
        : editor(editor), history(history), line(line), text(text) {}

    void execute() override
    {
        history->saveState(editor->save());
        editor->insert(line, text);
    }

    void undo() override
    {
        if (history->canUndo())
        {
            editor->restore(history->undo());
        }
    }
};

// ConcreteCommand for deleting text
class DeleteLineCommand : public Command
{
private:
    Editor *editor;
    EditorHistory *history;
    int line;

public:
    DeleteLineCommand(Editor *editor, EditorHistory *history, int line)
        : editor(editor), history(history), line(line) {}

    void execute() override
    {
        history->saveState(editor->save());
        editor->deleteLine(line);
    }

    void undo() override
    {
        if (history->canUndo())
        {
            editor->restore(history->undo());
        }
    }
};

// ConcreteCommand for deleting range
class DeleteRangeCommand : public Command
{
private:
    Editor *editor;
    EditorHistory *history;
    int startLine;
    int endLine;

public:
    DeleteRangeCommand(Editor *editor, EditorHistory *history, int startLine, int endLine)
        : editor(editor), history(history), startLine(startLine), endLine(endLine) {}

    void execute() override
    {
        history->saveState(editor->save());
        editor->deleteRange(startLine, endLine);
    }

    void undo() override
    {
        if (history->canUndo())
        {
            editor->restore(history->undo());
        }
    }
};

// ConcreteCommand for copying text
class CopyTextCommand : public Command
{
private:
    Editor *editor;
    int startLine;
    int endLine;

public:
    CopyTextCommand(Editor *editor, int startLine, int endLine)
        : editor(editor), startLine(startLine), endLine(endLine) {}

    void execute() override
    {
        editor->copy(startLine, endLine);
    }

    void undo() override
    {
        // No undo for copy
    }
};

// ConcreteCommand for pasting text
class PasteTextCommand : public Command
{
private:
    Editor *editor;
    EditorHistory *history;
    int line;

public:
    PasteTextCommand(Editor *editor, EditorHistory *history, int line)
        : editor(editor), history(history), line(line) {}

    void execute() override
    {
        history->saveState(editor->save());
        editor->paste(line);
    }

    void undo() override
    {
        if (history->canUndo())
        {
            editor->restore(history->undo());
        }
    }
};

// Invoker class
class EditorInvoker
{
private:
    vector<Command *> commandHistory;
    stack<Command *> redoStack;

public:
    void executeCommand(Command *command)
    {
        command->execute();
        commandHistory.push_back(command);
        while (!redoStack.empty())
        {
            redoStack.pop();
        }
    }

    void undoCommand()
    {
        if (!commandHistory.empty())
        {
            Command *command = commandHistory.back();
            command->undo();
            redoStack.push(command);
            commandHistory.pop_back();
        }
    }

    void redoCommand()
    {
        if (!redoStack.empty())
        {
            Command *command = redoStack.top();
            command->execute();
            commandHistory.push_back(command);
            redoStack.pop();
        }
    }
};

int main()
{
    Editor editor;
    EditorHistory history;
    EditorInvoker invoker;

    // Insert text
    Command *insertTextCommand1 = new InsertTextCommand(&editor, &history, 1, "Line 1");
    Command *insertTextCommand2 = new InsertTextCommand(&editor, &history, 2, "Line 2");
    invoker.executeCommand(insertTextCommand1);
    invoker.executeCommand(insertTextCommand2);
    editor.display();

    // Delete a line
    Command *deleteLineCommand = new DeleteLineCommand(&editor, &history, 1);
    invoker.executeCommand(deleteLineCommand);
    editor.display();

    // Undo delete
    invoker.undoCommand();
    editor.display();

    // Redo delete
    invoker.redoCommand();
    editor.display();

    // Insert more text
    Command *insertTextCommand3 = new InsertTextCommand(&editor, &history, 2, "Inserted Line");
    invoker.executeCommand(insertTextCommand3);
    editor.display();

    // Copy text
    Command *copyTextCommand = new CopyTextCommand(&editor, 1, 2);
    invoker.executeCommand(copyTextCommand);

    // Paste text
    Command *pasteTextCommand = new PasteTextCommand(&editor, &history, 3);
    invoker.executeCommand(pasteTextCommand);
    editor.display();

    // Clean up
    delete insertTextCommand1;
    delete insertTextCommand2;
    delete insertTextCommand3;
    delete deleteLineCommand;
    delete copyTextCommand;
    delete pasteTextCommand;

    return 0;
}

/*
Memento Pattern:
-----------------------
EditorState Class: This class represents the state of the Editor (originator) and stores the content vector.
Editor Class: Acts as the originator. It has methods to modify the content (insert, delete, copy, paste) and methods (save and restore)
to save and restore its state using EditorState.

EditorHistory Class: Acts as the caretaker. It maintains undoHistory and redoHistory stacks to manage saved states (mementos).
It provides methods (saveState, undo, redo, canUndo, canRedo) to save, retrieve, and manage states.

Command Pattern:
------------------------
Command Interface: The Command class declares execute and undo methods.
Concrete Commands: InsertTextCommand, DeleteLineCommand, DeleteRangeCommand, CopyTextCommand, and PasteTextCommand implement
the Command interface. Each command encapsulates a specific operation (execute) on the Editor and provides an undo method to
reverse that operation.

*/
