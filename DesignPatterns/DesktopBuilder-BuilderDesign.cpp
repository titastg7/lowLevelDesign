#include <bits/stdc++.h>
using namespace std;

class Desktop
{
    string monitor, keyboard, mouse, speaker, ram, processor, motherBoard;

public:
    void setMonitor(string pMonitor)
    {
        monitor = pMonitor;
    }
    void setKeyboard(string pKeyboard)
    {
        keyboard = pKeyboard;
    }
    void setMouse(string pMouse)
    {
        mouse = pMouse;
    }
    void setSpeaker(string pSpeaker)
    {
        speaker = pSpeaker;
    }
    void setRam(string pRam)
    {
        ram = pRam;
    }
    void setProcessor(string pProcessor)
    {
        processor = pProcessor;
    }
    void setMotherBoard(string pMotherBoard)
    {
        motherBoard = pMotherBoard;
    }
    void showSpecs()
    {
        cout << " Monitor " << monitor << endl;
        cout << " Keyboard " << keyboard << endl;
        cout << " Mouse " << mouse << endl;
        cout << " Speaker " << speaker << endl;
        cout << " Ram " << ram << endl;
        cout << " Processor " << processor << endl;
        cout << " MotherBoard " << motherBoard << endl;
    }
};

class DesktopBuilder
{
protected:
    Desktop *desktop;

public:
    DesktopBuilder()
    {
        desktop = new Desktop();
    }
    virtual void buildMonitor() = 0;
    virtual void buildKeyboard() = 0;
    virtual void buildMouse() = 0;
    virtual void buildSpeaker() = 0;
    virtual void buildRam() = 0;
    virtual void buildProcessor() = 0;
    virtual void buildMotherBoard() = 0;
    virtual Desktop *getDesktop()
    {
        return desktop;
    }
};

class DellDesktopBuilder : public DesktopBuilder
{

public:
    void buildMonitor()
    {
        desktop->setMonitor("Dell Monitor");
    }
    void buildKeyboard()
    {
        desktop->setKeyboard("Dell Keyboard");
    }
    void buildMouse()
    {
        desktop->setMouse("Dell Mouse");
    }
    void buildSpeaker()
    {
            desktop->setSpeaker"Dell Speaker");
    }
    void buildRam()
    {
        desktop->setRam("Dell Ram");
    }
    void buildProcessor()
    {
        desktop->setProcessor("Dell Processor");
    }
    void buildMotherBoard()
    {
        desktop->setMotherBoard("Dell MotherBoard");
    }
};

class HPDesktopBuilder : public DesktopBuilder
{

public:
    void buildMonitor()
    {
        desktop->setMonitor("HP Monitor");
    }
    void buildKeyboard()
    {
        desktop->setKeyboard("HP Keyboard");
    }
    void buildMouse()
    {
        desktop->setMouse("HP Mouse");
    }
    void buildSpeaker()
    {
            desktop->setSpeaker"HP Speaker");
    }
    void buildRam()
    {
        desktop->setRam("HP Ram");
    }
    void buildProcessor()
    {
        desktop->setProcessor("HP Processor");
    }
    void buildMotherBoard()
    {
        desktop->setMotherBoard("HP MotherBoard");
    }
};

class DesktopDirector
{
private:
    DesktopBuilder *desktopBuilder;

public:
    DesktopDirector(DesktopBuilder *pDesktopBuilder)
    {
        desktopBuilder = pDesktopBuilder;
    }
    Desktop *getDesktop()
    {
        return desktopBuilder->getDesktop();
    }
    Desktop *BuildDesktop()
    {
        desktopBuilder->buildMonitor();
        desktopBuilder->buildMouse();
        desktopBuilder->buildKeyboard();
        desktopBuilder->buildRam();
        desktopBuilder->buildProcessor();
        desktopBuilder->buildSpeaker();
        desktopBuilder->buildMotherBoard();
        return getDesktop();
    }
};

int main()
{
    HPDesktopBuilder *hp = new HPDesktopBuilder();
    DellDesktopBuilder *dell = new DellDesktopBuilder();

    DesktopDirector *d1 = new DesktopDirector(hp);
    DesktopDirector *d2 = new DesktopBuilder(dell);

    Desktop *desktop1 = d1->BuildDesktop();
    Desktop *desktop2 = d2->BuildDesktop();

    desktop1->showSpecs();
    desktop2->showSpecs();

    return 0;
}